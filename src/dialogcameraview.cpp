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

    qDebug() << u.c_str();

    QNetworkAccessManager *netmanager = new QNetworkAccessManager(this);
    connect(netmanager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {

        if (reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::warning(this, "Calaos Installer", tr("Calaos Installer: Erreur !"), reply->errorString());

            return;
        }

        qDebug() << QImageReader::supportedImageFormats();

        QByteArray res = reply->readAll();
        QImage img = QImage::fromData(res);
        reply->deleteLater();
        netmanager->deleteLater();

        if (img.isNull())
            QMessageBox::warning(this, "Calaos Installer", tr("Calaos Installer: Error, image is NULL !"));

        ui->label_cam->setPixmap(QPixmap::fromImage(img));

        DownloadPicture();
    });

    QNetworkRequest request(QUrl(u.c_str()));
    netmanager->get(request);
}
