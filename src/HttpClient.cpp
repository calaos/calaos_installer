#include "HttpClient.h"
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
                         std::function<void(bool, const QJsonDocument &)> callback)
{
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, "CalaosInstaller");

    QNetworkReply *reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, callback]()
    {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError)
        {
            qWarning() << "HTTP error:" << reply->errorString();
            callback(false, QJsonDocument());
            return;
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &err);
        if (err.error != QJsonParseError::NoError)
        {
            qWarning() << "JSON parse error:" << err.errorString();
            callback(false, QJsonDocument());
            return;
        }

        callback(true, doc);
    });
}

void HttpClient::downloadFile(const QUrl &url, const QString &destPath,
                              std::function<void(bool)> callback)
{
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, "CalaosInstaller");
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply *reply = m_nam->get(req);
    m_currentReply = reply;

    QFile *file = new QFile(destPath, reply);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
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

        if (reply->error() != QNetworkReply::NoError)
        {
            qWarning() << "Download error:" << reply->errorString();
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
