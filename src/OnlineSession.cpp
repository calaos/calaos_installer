#include "OnlineSession.h"

#include "AdminCache.h"
#include "OnlineConfig.h"

#include <QNetworkReply>
#include <QNetworkRequest>

const int OnlineSession::kUnlockTimeoutMs;

OnlineSession::OnlineSession(QObject *parent):
    QObject(parent)
{
    // Boxes serve a self-signed certificate.
    connect(&m_network, &QNetworkAccessManager::sslErrors, this,
            [](QNetworkReply *reply, const QList<QSslError> &) { reply->ignoreSslErrors(); });
}

OnlineSession::~OnlineSession()
{
    AdminCache::wipe(m_adminPass);
    AdminCache::wipe(m_session);
    AdminCache::wipe(m_pass);
}

void OnlineSession::setServer(const QString &host)
{
    if (host == m_host && m_url.isValid())
        return;
    m_host = host;
    m_url = OnlineConfig::serverUrl(host);
    m_session.clear();
}

void OnlineSession::setCredentials(const QString &user, const QString &pass, const QString &adminPass)
{
    if (user != m_user)
        m_session.clear();
    m_user = user;
    m_pass = pass;

    AdminCache &cache = AdminCache::instance();
    // The cached password belongs to the user it was typed for.
    if (!m_host.isEmpty() && !cache.has(m_host, user))
        cache.invalidate(m_host);
    const AdminCache::Entry cached = cache.get(m_host, user);
    if (adminPass.isEmpty())
    {
        // Nothing typed: the box was already unlocked in this run.
        m_adminPass = cached.adminPass;
        if (m_session.isEmpty())
            m_session = cached.session;
    }
    else
    {
        if (adminPass != cached.adminPass)
            m_session.clear();
        m_adminPass = adminPass;
    }
}

void OnlineSession::getConfig()
{
    m_put = false;
    start();
}

void OnlineSession::putConfig(const QString &ioXml, const QString &rulesXml)
{
    m_put = true;
    m_ioXml = ioXml;
    m_rulesXml = rulesXml;
    start();
}

void OnlineSession::start()
{
    m_unlockTried = false;
    m_httpsRetried = false;
    m_refusalBeforeRetry.clear();
    send(STEP_CONFIG);
}

void OnlineSession::send(Step step)
{
    // A session is a bearer secret: never on a clear-text connection. The box answers
    // bad_session or https_required and the exchange moves to https.
    const QString session = m_url.scheme() == "https" ? m_session : QString();
    QByteArray body;
    if (step == STEP_UNLOCK)
        body = OnlineConfig::buildUnlockRequest(m_user, m_pass, m_adminPass);
    else if (m_put)
        body = OnlineConfig::buildPutRequest(m_user, m_pass, m_ioXml, m_rulesXml, session);
    else
        body = OnlineConfig::buildGetRequest(m_user, m_pass, session);

    QNetworkRequest request(m_url);
    // A redirect would replay the body (password or session) towards another host.
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::ManualRedirectPolicy);
    // The contract bounds the unlock answer; config keeps today's behaviour (a put may be slow).
    if (step == STEP_UNLOCK)
        request.setTransferTimeout(kUnlockTimeoutMs);
    QNetworkReply *reply = m_network.post(request, body);
    connect(reply, &QNetworkReply::finished, this, [this, reply, step]() { replied(reply, step); });
}

bool OnlineSession::moveToHttps()
{
    if (m_httpsRetried)
        return false;
    const QUrl https = OnlineConfig::httpsRetryUrl(m_url, m_httpsPort);
    if (!https.isValid())
        return false;
    m_httpsRetried = true;
    m_url = https;
    return true;
}

void OnlineSession::replied(QNetworkReply *reply, Step step)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        if (!m_refusalBeforeRetry.isEmpty())
            finish(true, QByteArray(m_refusalBeforeRetry));
        else
            finish(false, QByteArray(), reply->errorString());
        return;
    }

    // Only a retry that could not connect falls back to the refusal it replaced.
    m_refusalBeforeRetry.clear();

    const QByteArray body = reply->readAll();
    const QString code = OnlineConfig::refusalCode(body);

    if (code == "https_required" && moveToHttps())
    {
        m_refusalBeforeRetry = body;
        send(step);
        return;
    }

    if (step == STEP_UNLOCK)
    {
        if (!code.isEmpty())
        {
            // Only a refusal of the password itself makes the remembered one useless; a rate
            // limit or an unreachable daemon says nothing about it.
            if (code == "bad_password" || code == "admin_not_set")
                forgetAdminPassword();
            finish(true, body);
            return;
        }
        m_session = OnlineConfig::parseUnlockResponse(body);
        if (m_session.isEmpty())
            finish(true, R"({"success":"false","error":"no_admin_session"})");
        else
        {
            AdminCache::instance().store(m_host, m_user, m_adminPass, m_session);
            send(STEP_CONFIG);
        }
        return;
    }

    if (code == "bad_session")
    {
        // Sessions last 15 minutes: an expired one is replaced once with the password we hold.
        m_session.clear();
        AdminCache::instance().dropSession(m_host);
        if (!m_adminPass.isEmpty() && !m_unlockTried)
        {
            m_unlockTried = true;
            // The admin password never travels in clear.
            if (m_url.scheme() == "https" || moveToHttps())
                send(STEP_UNLOCK);
            else
                finish(true, R"({"success":"false","error":"https_required"})");
            return;
        }
        // The session was refused right after a successful unlock: let the user type the
        // password again instead of replaying a cache that cannot help.
        if (m_unlockTried)
            forgetAdminPassword();
    }

    finish(true, body);
}

void OnlineSession::forgetAdminPassword()
{
    AdminCache::wipe(m_adminPass);
    AdminCache::wipe(m_session);
    AdminCache::instance().invalidate(m_host);
}

void OnlineSession::finish(bool reached, const QByteArray &body, const QString &networkError)
{
    m_refusalBeforeRetry.clear();
    if (m_put)
        emit putFinished(reached, body, networkError);
    else
        emit getFinished(reached, body, networkError);
}
