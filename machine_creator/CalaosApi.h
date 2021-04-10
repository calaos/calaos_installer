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

private:
    QNetworkAccessManager *netManager;

    QString calaosApiBase;
};

#endif // CALAOSAPI_H
