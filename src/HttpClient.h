#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <functional>

class QNetworkAccessManager;
class QNetworkReply;

class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject *parent = nullptr);

    // GET request returning parsed JSON; `accept` sets the Accept header when not empty
    void getJson(const QUrl &url,
                 std::function<void(bool success, const QJsonDocument &doc)> callback,
                 const QByteArray &accept = QByteArray());

    // GET request downloading to a file on disk
    void downloadFile(const QUrl &url, const QString &destPath,
                      std::function<void(bool success)> callback);

    // Abort any ongoing download
    void abort();

    // Reason of the last failed getJson() or downloadFile(), without URL
    QString lastError() const { return m_lastError; }

    // HTTP status of the last request, 0 when none was received
    int lastStatus() const { return m_lastStatus; }

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkAccessManager *m_nam;
    QNetworkReply *m_currentReply = nullptr;
    QString m_lastError;
    int m_lastStatus = 0;
};

#endif // HTTPCLIENT_H
