#include "dialogcameraview.h"
#include "ui_DialogCameraView.h"
#include "mainwindow.h"

DialogCameraView::DialogCameraView(IOBase *cam, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCameraView), camera(cam)
{
    ui->setupUi(this);
}

DialogCameraView::~DialogCameraView()
{
    delete ui;
}

void DialogCameraView::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogCameraView::DownloadPicture()
{
    string u;

    if (camera->get_param("type") == "axis")
    {
        string cam = "1";
        if (camera->get_param("model") != "") cam = camera->get_param("model").c_str();
        u = "http://" + camera->get_param("host") + ":" + camera->get_param("port") + "/axis-cgi/jpg/image.cgi?camera=" + cam;
    }
    else if (camera->get_param("type") == "gadspot")
    {
        u = "http://" + camera->get_param("host") + ":" + camera->get_param("port") + "/Jpeg/CamImg.jpg";
    }
    else if (camera->get_param("type") == "planet")
    {
        if (camera->get_param("model") == "ICA-300" ||
            camera->get_param("model") == "ICA-302" ||
            camera->get_param("model") == "ICA-500")
        {
            u = "http://" + camera->get_param("host") + ":" + camera->get_param("port") + "/Jpeg/CamImg.jpg";
        }
        else
        {
            u = "http://" + camera->get_param("host") + ":" + camera->get_param("port") + "/goform/video2";
        }
    }
    else if (camera->get_param("type") == "standard_mjpeg")
    {
        u = camera->get_param("url_jpeg");
    }

    QUrl url(QString::fromStdString(u));
    qDebug() << url.toString(QUrl::RemovePassword);

    QNetworkAccessManager *netmanager = new QNetworkAccessManager(this);
    connect(netmanager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {

        if (reply->error() != QNetworkReply::NoError)
        {
            QString msg = reply->errorString();
            if (!reply->url().password().isEmpty())
                msg.replace(reply->url().password(), QStringLiteral("***"));
            QMessageBox::warning(this, "Calaos Installer", tr("Calaos Installer: Erreur !") + "\n" + msg);

            return;
        }

        qDebug() << QImageReader::supportedImageFormats();

        QByteArray res = reply->readAll();
        QImage img = QImage::fromData(res, "JPG");
        reply->deleteLater();
        netmanager->deleteLater();

        if (img.isNull())
            QMessageBox::warning(this, "Calaos Installer", tr("Calaos Installer: Error, image is NULL !"));

        ui->label_cam->setPixmap(QPixmap::fromImage(img));

        DownloadPicture();
    });

    QNetworkRequest request(url);
    QNetworkReply *reply = netmanager->get(request);

    // Cameras ship self-signed certificates: tolerate only that on this one reply.
    connect(reply, &QNetworkReply::sslErrors, this, [reply](const QList<QSslError> &errors) {
        QList<QSslError> ignorable;
        for (const QSslError &e : errors)
            if (DialogCameraView::isTolerableSslError(e.error()))
                ignorable << e;
        if (ignorable.size() == errors.size())
            reply->ignoreSslErrors(ignorable);
    });
}

bool DialogCameraView::isTolerableSslError(QSslError::SslError e)
{
    switch (e)
    {
    case QSslError::HostNameMismatch:
    case QSslError::SelfSignedCertificate:
    case QSslError::SelfSignedCertificateInChain:
    case QSslError::UnableToGetLocalIssuerCertificate:
    case QSslError::UnableToVerifyFirstCertificate:
    case QSslError::CertificateUntrusted:
        return true;
    default:
        return false;
    }
}
