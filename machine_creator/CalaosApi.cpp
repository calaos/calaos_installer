#include "CalaosApi.h"
#include <QStringBuilder>

#define CALAOS_API_ADDR      "https://calaos.fr/update"

CalaosApi::CalaosApi(QNetworkAccessManager *nm, QObject *parent):
    QObject(parent),
    netManager(nm)
{
    if (qEnvironmentVariableIsSet("CALAOS_API_ADDR"))
        calaosApiBase = qgetenv("CALAOS_API_ADDR");
    else
        calaosApiBase = CALAOS_API_ADDR;

    if (!netManager)
        netManager = new QNetworkAccessManager(this);
}

CalaosApi::~CalaosApi()
{
}

void CalaosApi::loadImages(std::function<void (bool, const QJsonArray &)> callback)
{
    QString url = calaosApiBase % "?versions=all";

    NetworkRequest *n = new NetworkRequest(url, NetworkRequest::HttpGet, this);
    n->setNetManager(netManager);
    n->setCustomHeader("Content-Type", "application/json");
    n->setResultType(NetworkRequest::TypeJson);

    connect(n, &NetworkRequest::finishedJson, [=](int status, const QJsonDocument &jdoc)
    {
        n->deleteLater();

        if (status == NetworkRequest::RequestSuccess)
        {
            callback(true, jdoc.array());
        }
        else
        {
            callback(false, {});
        }
    });

    if (!n->start())
    {
        delete n;
        callback(false, {});
    }
}
