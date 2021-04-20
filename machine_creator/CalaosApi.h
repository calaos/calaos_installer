#ifndef CALAOSAPI_H
#define CALAOSAPI_H

#include <QObject>
#include "NetworkRequest.h"

class CalaosApi: public QObject
{
    Q_OBJECT
public:
    CalaosApi(QNetworkAccessManager *nm = nullptr, QObject *parent = nullptr);
    ~CalaosApi();

    void loadImages(std::function<void(bool success, const QJsonArray &imagelist)> callback);
    NetworkRequest *downloadImage(QString url, QString checksum, std::function<void(bool success, QString localFile)> callback);

    void calcHash(QString filename, std::function<void(QString hash)> callback);

    //this is the static blocking function
    static QString calcBlake2bHash(QString filename);

signals:
    void downloadProgress(qint64 bytesRead, qint64 totalBytes);

private:
    QNetworkAccessManager *netManager;

    QString calaosApiBase;
};

#endif // CALAOSAPI_H
