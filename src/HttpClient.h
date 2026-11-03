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

    // GET request downloading to a file on disk. maxBytes > 0 fails the download, and removes
    // the file, as soon as the announced or received size exceeds it.
    void downloadFile(const QUrl &url, const QString &destPath,
                      std::function<void(bool success)> callback, qint64 maxBytes = 0);

    // Abort the ongoing request (getJson or downloadFile); its callback gets success = false
    // and lastError() says "Cancelled"
    void abort();

    // Longest stall (no byte received) tolerated, in ms. It is not a total duration: a slow but
    // steady download is not cut. 0 disables it.
    void setTransferTimeouts(int jsonMs, int downloadMs)
    {
        m_jsonTimeoutMs = jsonMs;
        m_downloadTimeoutMs = downloadMs;
    }

    // Total time allowed for a getJson() answer, in ms, whatever the progress. 0 disables it.
    void setJsonDeadline(int ms) { m_jsonDeadlineMs = ms; }

    // Largest getJson() answer accepted, in bytes
    void setMaxJsonBytes(qint64 bytes) { m_maxJsonBytes = bytes; }

    // Reason of the last failed getJson() or downloadFile(), without URL
    QString lastError() const { return m_lastError; }

    // HTTP status of the last request, 0 when none was received
    int lastStatus() const { return m_lastStatus; }

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    enum class Stop { None, Cancelled, Deadline, TooLarge };

    QString stopText(const QString &fallback) const;

    QNetworkAccessManager *m_nam;
    QNetworkReply *m_currentReply = nullptr;
    QString m_lastError;
    int m_lastStatus = 0;
    Stop m_stop = Stop::None;
    int m_jsonDeadlineMs = 30000;
    qint64 m_maxJsonBytes = 1024 * 1024;
    int m_jsonTimeoutMs = 15000;
    int m_downloadTimeoutMs = 60000;
};

#endif // HTTPCLIENT_H
