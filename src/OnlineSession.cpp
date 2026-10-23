#include "OnlineSession.h"

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
    m_adminPass = adminPass;
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
    QByteArray body;
    if (step == STEP_UNLOCK)
        body = OnlineConfig::buildUnlockRequest(m_user, m_pass, m_adminPass);
    else if (m_put)
        body = OnlineConfig::buildPutRequest(m_user, m_pass, m_ioXml, m_rulesXml, m_session);
    else
        body = OnlineConfig::buildGetRequest(m_user, m_pass, m_session);

    QNetworkRequest request(m_url);
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
            finish(true, body);
            return;
        }
        m_session = OnlineConfig::parseUnlockResponse(body);
        if (m_session.isEmpty())
            finish(true, R"({"success":"false","error":"no_admin_session"})");
        else
            send(STEP_CONFIG);
        return;
    }

    if (code == "bad_session")
    {
        // A session we already held has expired: the user is asked for the password again
        // rather than unlocking behind their back.
        if (m_session.isEmpty() && !m_adminPass.isEmpty() && !m_unlockTried)
        {
            m_unlockTried = true;
            // The admin password never travels in clear.
            if (m_url.scheme() == "https" || moveToHttps())
                send(STEP_UNLOCK);
            else
                finish(true, R"({"success":"false","error":"https_required"})");
            return;
        }
        m_session.clear();
    }

    finish(true, body);
}

void OnlineSession::finish(bool reached, const QByteArray &body, const QString &networkError)
{
    m_refusalBeforeRetry.clear();
    if (m_put)
        emit putFinished(reached, body, networkError);
    else
        emit getFinished(reached, body, networkError);
}
