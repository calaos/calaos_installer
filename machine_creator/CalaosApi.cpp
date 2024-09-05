#include "CalaosApi.h"
#include <QStringBuilder>
#include "BLAKE2/ref/blake2.h"
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentRun>

#define CALAOS_API_ADDR      "https://calaos.fr/download/api"

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

    connect(n, &NetworkRequest::finishedJson, this, [=](int status, const QJsonDocument &jdoc)
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

NetworkRequest *CalaosApi::downloadImage(QString url, QString checksum, std::function<void (bool, QString)> callback)
{
    NetworkRequest *n = new NetworkRequest(url, NetworkRequest::HttpGet, this);
    n->setNetManager(netManager);
    n->setResultType(NetworkRequest::TypeFile);
    connect(n, &NetworkRequest::downloadProgress, this, &CalaosApi::downloadProgress);

    QFileInfo fileInfo(url);

    QString downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (downloadsFolder.isEmpty())
        downloadsFolder = QDir::homePath();

    QString fileName = downloadsFolder + "/" + fileInfo.fileName();

    calcHash(fileName, [=](QString hash)
    {
        if (QFile::exists(fileName) &&
            hash == checksum)
        {
            //No need to download the file again
            qDebug() << "File already downloaded and checksum matches!";
            callback(true, fileName);
            return;
        }

        auto localFile = new QFile(fileName);
        n->setFile(localFile);

        connect(n, &NetworkRequest::finished, this, [=](int status)
        {
            n->deleteLater();
            delete localFile;

            if (status == NetworkRequest::RequestSuccess)
            {
                calcHash(fileName, [=](QString hash)
                {
                    if (hash == checksum)
                        callback(true, fileName);
                    else
                    {
                        qWarning() << "Checksum does not match!";
                        callback(false, {});
                    }
                });
            }
            else
            {
                callback(false, {});
            }
        });

        if (!n->start())
        {
            delete n;
            delete localFile;
            callback(false, {});
        }
    });

    return n;
}

void CalaosApi::calcHash(QString filename, std::function<void (QString)> callback)
{
    QFutureWatcher<QString> *watcher = new QFutureWatcher<QString>();
    connect(watcher, &QFutureWatcher<QString>::finished, [=]()
    {
        watcher->deleteLater();
        callback(watcher->result());
    });

    QFuture<QString> f = QtConcurrent::run([=]()
    {
        return calcBlake2bHash(filename);
    });
    watcher->setFuture(f);
}

QString CalaosApi::calcBlake2bHash(QString filename)
{
    QFile file (filename);

    if (!file.open (QIODevice::ReadOnly))
        return {};

    const auto hashLen = 256 / 8; //Blake2b256
    const int bufferSize = 10240;
    blake2b_state blake2bContext;

    new (&blake2bContext) blake2b_state;
    blake2b_init(&blake2bContext, hashLen);

    while (!file.atEnd())
    {
        const QByteArray &buf = file.read (bufferSize);
        blake2b_update(&blake2bContext, reinterpret_cast<const uint8_t *>(buf.data()), buf.size());
    }

    QByteArray result;
    result.resize(hashLen);
    blake2b_final(&blake2bContext, reinterpret_cast<uint8_t *>(result.data()), hashLen);

    return result.toHex();
}
