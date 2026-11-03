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

    // GET request returning parsed JSON
    void getJson(const QUrl &url,
                 std::function<void(bool success, const QJsonDocument &doc)> callback);

    // GET request downloading to a file on disk
    void downloadFile(const QUrl &url, const QString &destPath,
                      std::function<void(bool success)> callback);

    // Abort any ongoing download
    void abort();

    // Reason of the last failed downloadFile(), without URL
    QString lastError() const { return m_lastError; }

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkAccessManager *m_nam;
    QNetworkReply *m_currentReply = nullptr;
    QString m_lastError;
};

#endif // HTTPCLIENT_H
