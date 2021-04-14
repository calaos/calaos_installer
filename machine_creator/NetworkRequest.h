#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

#include <QtCore>
#include <QtNetwork>

class NetworkRequest: public QObject
{
    Q_OBJECT

public:
    enum HttpType
    {
        HttpGet,
        HttpPost,
        HttpPut,
        HttpDelete,
        HttpHead,
    };

    enum RequestStatus
    {
        RequestSuccess,
        RequestHttpError,
        RequestJsonParseError
    };

    enum ResulType
    {
        TypeJson, //default
        TypeRawData,
        TypeFile
    };

public:
    NetworkRequest(QObject *parent = nullptr);
    NetworkRequest(QString url, HttpType type = HttpGet, QObject *parent = nullptr);
    ~NetworkRequest();

    void setNetManager(QNetworkAccessManager *netManager);

    void setUrl(QString u) { url = u; }
    const QString &getUrl() const { return url; }

    void setRequestType(HttpType t) { type = t; }
    HttpType getRequestType() { return type; }

    void setResultType(ResulType t) { resType = t; }
    ResulType getResultType() { return resType; }

    void setPostData(const QByteArray &data) { postData = data; }
    const QByteArray &getPostData() const { return postData; }

    void setFile(QFile *f) { dlFile = f; }
    QFile *getFile() const { return dlFile; }

    void setCustomHeader(QString header, QString value);

    QString getLastError() { return lastError; }

    void setCertificate(QString path);

public slots:
    bool start();
    void cancel();

signals:
    void finishedJson(int status, const QJsonDocument &doc);
    void finishedData(int status, const QByteArray &data);
    void finished(int status);
    void downloadProgress(qint64 bytesRead, qint64 totalBytes);

private slots:
    void nmFinishedJson();
    void nmFinishedData();
    void nmFinished();
    void nmReadyRead();
    void nmSslErrors(const QList<QSslError> &errors);
    void nmRedirected(const QUrl &url);

private:
    QNetworkAccessManager *netmanager = nullptr;
    QNetworkReply *reply = nullptr;

    QByteArray postData;
    QString url;
    HttpType type = HttpGet;
    ResulType resType = TypeJson;

    QMap<QString, QString> headers;

    QFile *dlFile = nullptr;

    bool delNm = false;

    QString lastError;

    QList<QSslCertificate> cacerts;
};

#endif // NETWORKREQUEST_H
