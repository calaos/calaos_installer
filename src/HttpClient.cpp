#include "HttpClient.h"
#include "networkerrortext.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QJsonParseError>
#include <QTimer>
#include <memory>

HttpClient::HttpClient(QObject *parent)
    : QObject(parent)
    , m_nam(new QNetworkAccessManager(this))
{
}

QString HttpClient::stopText(const QString &fallback) const
{
    switch (m_stop)
    {
    case Stop::Cancelled: return tr("Cancelled");
    case Stop::Deadline: return tr("Timed out: no complete answer within %1 s").arg(m_jsonDeadlineMs / 1000.0, 0, 'g', 3);
    case Stop::TooLarge: return tr("The answer is larger than expected");
    case Stop::None: break;
    }
    return fallback;
}

void HttpClient::getJson(const QUrl &url,
                         std::function<void(bool, const QJsonDocument &)> callback,
                         const QByteArray &accept)
{
    m_lastError.clear();
    m_lastStatus = 0;
    m_stop = Stop::None;

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, "CalaosInstaller");
    if (!accept.isEmpty())
        req.setRawHeader("Accept", accept);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);
    req.setTransferTimeout(m_jsonTimeoutMs);

    QNetworkReply *reply = m_nam->get(req);
    m_currentReply = reply;
    auto body = std::make_shared<QByteArray>();

    // The transfer timeout only catches stalls: a peer sending a byte now and then needs this.
    if (m_jsonDeadlineMs > 0)
    {
        QTimer *deadline = new QTimer(reply);
        deadline->setSingleShot(true);
        connect(deadline, &QTimer::timeout, this, [this, reply]()
        {
            m_stop = Stop::Deadline;
            reply->abort();
        });
        deadline->start(m_jsonDeadlineMs);
    }

    connect(reply, &QNetworkReply::readyRead, this, [this, reply, body]()
    {
        body->append(reply->readAll());
        if (body->size() > m_maxJsonBytes)
        {
            m_stop = Stop::TooLarge;
            reply->abort();
        }
    });

    connect(reply, &QNetworkReply::finished, this, [this, reply, body, callback]()
    {
        m_currentReply = nullptr;
        reply->deleteLater();
        m_lastStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (reply->error() != QNetworkReply::NoError)
        {
            m_lastError = stopText(networkErrorText(reply));
            qWarning() << "HTTP error:" << m_lastError;
            callback(false, QJsonDocument());
            return;
        }

        body->append(reply->readAll());
        if (body->size() > m_maxJsonBytes)
        {
            m_stop = Stop::TooLarge;
            m_lastError = stopText(QString());
            callback(false, QJsonDocument());
            return;
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(*body, &err);
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
                              std::function<void(bool)> callback, qint64 maxBytes)
{
    m_lastError.clear();
    m_lastStatus = 0;
    m_stop = Stop::None;

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, "CalaosInstaller");
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);
    req.setTransferTimeout(m_downloadTimeoutMs);

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

    auto tooLarge = [this, reply, maxBytes](qint64 size)
    {
        if (maxBytes > 0 && size > maxBytes && m_stop == Stop::None)
        {
            m_stop = Stop::TooLarge;
            reply->abort();
        }
    };

    connect(reply, &QNetworkReply::readyRead, this, [this, reply, file, tooLarge]()
    {
        if (m_stop != Stop::None)
            return;
        const QByteArray chunk = reply->readAll();
        if (file->write(chunk) != chunk.size())
        {
            m_lastError = file->errorString();
            m_stop = Stop::Cancelled;
            reply->abort();
            return;
        }
        tooLarge(file->size());
    });

    connect(reply, &QNetworkReply::downloadProgress,
            this, &HttpClient::downloadProgress);
    // downloadProgress may not fire before the first body byte; the headers already announce the size.
    connect(reply, &QNetworkReply::metaDataChanged, this, [reply, tooLarge]()
    {
        tooLarge(reply->header(QNetworkRequest::ContentLengthHeader).toLongLong());
    });
    connect(reply, &QNetworkReply::downloadProgress, this, [tooLarge](qint64, qint64 total)
    {
        tooLarge(total);
    });

    connect(reply, &QNetworkReply::finished, this, [this, reply, file, maxBytes, callback]()
    {
        m_currentReply = nullptr;
        reply->deleteLater();
        m_lastStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (reply->error() != QNetworkReply::NoError)
        {
            if (m_lastError.isEmpty())
                m_lastError = stopText(networkErrorText(reply));
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

        if (maxBytes > 0 && file->size() > maxBytes)
        {
            m_stop = Stop::TooLarge;
            m_lastError = stopText(QString());
            file->remove();
            callback(false);
            return;
        }

        callback(true);
    });
}

void HttpClient::abort()
{
    if (m_currentReply)
    {
        m_stop = Stop::Cancelled;
        m_currentReply->abort();
    }
}
