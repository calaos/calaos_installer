#include "HttpClient.h"
#include "networkerrortext.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QJsonParseError>

HttpClient::HttpClient(QObject *parent)
    : QObject(parent)
    , m_nam(new QNetworkAccessManager(this))
{
}

void HttpClient::getJson(const QUrl &url,
                         std::function<void(bool, const QJsonDocument &)> callback,
                         const QByteArray &accept)
{
    m_lastError.clear();
    m_lastStatus = 0;

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, "CalaosInstaller");
    if (!accept.isEmpty())
        req.setRawHeader("Accept", accept);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply *reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]()
    {
        reply->deleteLater();
        m_lastStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (reply->error() != QNetworkReply::NoError)
        {
            m_lastError = networkErrorText(reply);
            qWarning() << "HTTP error:" << m_lastError;
            callback(false, QJsonDocument());
            return;
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &err);
        if (err.error != QJsonParseError::NoError)
        {
            m_lastError = QString("Invalid JSON answer: %1").arg(err.errorString());
            qWarning() << m_lastError;
            callback(false, QJsonDocument());
            return;
        }

        callback(true, doc);
    });
}

void HttpClient::downloadFile(const QUrl &url, const QString &destPath,
                              std::function<void(bool)> callback)
{
    m_lastError.clear();
    m_lastStatus = 0;

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, "CalaosInstaller");
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply *reply = m_nam->get(req);
    m_currentReply = reply;

    QFile *file = new QFile(destPath, reply);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        m_lastError = file->errorString();
        qWarning() << "Cannot open file for writing:" << destPath;
        reply->abort();
        reply->deleteLater();
        m_currentReply = nullptr;
        callback(false);
        return;
    }

    connect(reply, &QNetworkReply::readyRead, this, [reply, file]()
    {
        file->write(reply->readAll());
    });

    connect(reply, &QNetworkReply::downloadProgress,
            this, &HttpClient::downloadProgress);

    connect(reply, &QNetworkReply::finished, this, [this, reply, file, callback]()
    {
        m_currentReply = nullptr;
        reply->deleteLater();
        m_lastStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (reply->error() != QNetworkReply::NoError)
        {
            m_lastError = networkErrorText(reply);
            qWarning() << "Download error:" << m_lastError;
            file->close();
            file->remove();
            callback(false);
            return;
        }

        // Write any remaining data
        if (reply->bytesAvailable() > 0)
            file->write(reply->readAll());
        file->close();

        callback(true);
    });
}

void HttpClient::abort()
{
    if (m_currentReply)
    {
        m_currentReply->abort();
        m_currentReply = nullptr;
    }
}
