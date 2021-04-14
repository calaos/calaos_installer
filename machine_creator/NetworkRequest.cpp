#include "NetworkRequest.h"

NetworkRequest::NetworkRequest(QObject *parent):
    QObject(parent)
{
}

NetworkRequest::NetworkRequest(QString u, HttpType t, QObject *parent):
    QObject(parent),
    url(u),
    type(t)
{
}

NetworkRequest::~NetworkRequest()
{
    cancel();
    if (delNm)
        delete netmanager;
}

void NetworkRequest::setNetManager(QNetworkAccessManager *netManager)
{
    netmanager = netManager;
}

void NetworkRequest::setCustomHeader(QString header, QString value)
{
    headers[header] = value;
}

void NetworkRequest::setCertificate(QString path)
{
    cacerts = QSslCertificate::fromPath(path, QSsl::Pem, QSslCertificate::PatternSyntax::RegularExpression);
    QSslConfiguration::defaultConfiguration().addCaCertificates(cacerts);
}

bool NetworkRequest::start()
{
    if (reply)
    {
        qDebug() << "Request already in progress";
        return false;
    }

    if (!netmanager)
    {
        delNm = true;
        netmanager = new QNetworkAccessManager(this);
    }

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    lastError.clear();

    qDebug() << "[*] New request to " << url;

    //set ssl config
    QSslConfiguration sslConfig(QSslConfiguration::defaultConfiguration());
    sslConfig.setCaCertificates(cacerts);
    request.setSslConfiguration(sslConfig);

    auto it = headers.constBegin();
    while (it != headers.constEnd())
    {
        request.setRawHeader(it.key().toLocal8Bit(), it.value().toLocal8Bit());
        ++it;
    }

    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    if (type == HttpGet)
        reply = netmanager->get(request);
    else if (type == HttpPost)
        reply = netmanager->post(request, postData);
    else if (type == HttpPut)
        reply = netmanager->put(request, postData);
    else if (type == HttpDelete)
           reply = netmanager->deleteResource(request);
    else if (type == HttpHead)
        reply = netmanager->head(request);

    if (!reply)
    {
        qDebug() << "Failed to create request";
        return false;
    }

    connect(reply, &QNetworkReply::sslErrors, this, &NetworkRequest::nmSslErrors);
    connect(reply, &QNetworkReply::redirected, this, &NetworkRequest::nmRedirected);
    connect(reply, &QNetworkReply::downloadProgress, this, &NetworkRequest::downloadProgress);

    if (resType == TypeJson)
    {
        connect(reply, SIGNAL(finished()),
                this, SLOT(nmFinishedJson()));
    }
    else if (resType == TypeRawData)
    {
        connect(reply, SIGNAL(finished()),
                this, SLOT(nmFinishedData()));
    }
    else if (resType == TypeFile)
    {
        connect(reply, SIGNAL(finished()),
                this, SLOT(nmFinished()));
        connect(reply, SIGNAL(readyRead()),
                this, SLOT(nmReadyRead()));
        if (!dlFile)
        {
            lastError = "dlFile is invalid!, aborting request.";
            qWarning() << "dlFile is invalid!, aborting request.";
            reply->abort();
            return false;
        }
        if (!dlFile->isOpen())
        {
            if (!dlFile->open(QFile::WriteOnly | QIODevice::Truncate))
                qWarning() << "Error opening file '" << dlFile->fileName() << "' for write";
        }
    }

    return true;
}

void NetworkRequest::cancel()
{
    if (reply)
        reply->abort();
}

void NetworkRequest::nmReadyRead()
{
    dlFile->write(reply->readAll());
}

void NetworkRequest::nmFinished()
{
    if (reply->error() != QNetworkReply::NoError)
    {
        lastError = reply->errorString();
        qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
        QString data(reply->readAll());
        if (!data.isEmpty())
            qDebug().noquote() << "Request body: " << data;
        emit finished(RequestHttpError);

        reply->deleteLater();
        reply = nullptr;

        return;
    }

    if (reply->bytesAvailable() > 0)
        dlFile->write(reply->readAll());
    dlFile->close();

    reply->deleteLater();
    reply = nullptr;

    emit finished(RequestSuccess);
}

void NetworkRequest::nmFinishedJson()
{
    QJsonParseError err;
    QJsonDocument jdoc;

    if (reply->error() != QNetworkReply::NoError)
    {
        lastError = reply->errorString();
        qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
        QString data(reply->readAll());
        if (!data.isEmpty())
            qDebug().noquote() << "Request body: " << data;
        jdoc = QJsonDocument::fromJson(data.toUtf8(), &err);
        emit finishedJson(RequestHttpError, jdoc);

        reply->deleteLater();
        reply = nullptr;

        return;
    }

    QByteArray res = reply->readAll();
    jdoc = QJsonDocument::fromJson(res, &err);

    if (res.isEmpty())
    {
        //do not treat empty result as error
        err.error = QJsonParseError::NoError;
    }

    if (err.error != QJsonParseError::NoError)
    {
        lastError = "JSON parse error " + err.errorString() + " at offset: " + QString::number(err.offset);
        qWarning() << "JSON parse error " << err.errorString() << " at offset: " << err.offset;
        const int maxlen = 100;
        int start = err.offset - maxlen;
        if (start < 0) start = 0;
        qWarning() << res.mid(start, err.offset + maxlen);
        QString s;
        for (int i = start;i < err.offset + maxlen;i++)
            s += (i == err.offset?"^":"-");
        qWarning() << s;
    }
    else
        qDebug().noquote() << QString(jdoc.toJson());

    reply->deleteLater();
    reply = nullptr;

    emit finishedJson(err.error == QJsonParseError::NoError?RequestSuccess:RequestJsonParseError, jdoc);
}

void NetworkRequest::nmFinishedData()
{
    if (reply->error() != QNetworkReply::NoError)
    {
        lastError = reply->errorString();
        qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
        QString data(reply->readAll());
        if (!data.isEmpty())
            qDebug().noquote() << "Request body: " << data;
        emit finishedData(RequestHttpError, QByteArray());

        reply->deleteLater();
        reply = nullptr;

        return;
    }

    reply->deleteLater();
    emit finishedData(RequestSuccess, reply->readAll());

    reply = nullptr;
}

void NetworkRequest::nmSslErrors(const QList<QSslError> &errors)
{
    qWarning() << "SSL errors:";
    for (QSslError e: errors)
    {
        qWarning() << e.errorString();
    }

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->ignoreSslErrors();
}

void NetworkRequest::nmRedirected(const QUrl &url)
{
    qDebug() << "Redirected to " << url;
}
