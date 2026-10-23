#include "OnlineSession.h"

#include "OnlineConfig.h"

#include <QNetworkReply>
#include <QNetworkRequest>

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

    QNetworkReply *reply = m_network.post(QNetworkRequest(m_url), body);
    connect(reply, &QNetworkReply::finished, this, [this, reply, step]() { replied(reply, step); });
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

    const QByteArray body = reply->readAll();
    const QString code = OnlineConfig::refusalCode(body);

    if (!code.isEmpty() && !m_httpsRetried && OnlineConfig::shouldRetryOverHttps(m_url, code))
    {
        const QUrl https = OnlineConfig::httpsRetryUrl(m_url, m_httpsPort);
        if (https.isValid())
        {
            m_httpsRetried = true;
            m_refusalBeforeRetry = body;
            m_url = https;
            send(step);
            return;
        }
    }

    if (step == STEP_UNLOCK)
    {
        m_session = OnlineConfig::parseUnlockResponse(body);
        if (m_session.isEmpty())
            finish(true, body);
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
            send(STEP_UNLOCK);
            return;
        }
        m_session.clear();
    }

    finish(true, body);
}

void OnlineSession::finish(bool reached, const QByteArray &body, const QString &networkError)
{
    m_refusalBeforeRetry.clear();
    emit finished(reached, body, networkError);
}
