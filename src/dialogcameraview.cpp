#include "dialogcameraview.h"
#include "ui_dialogcameraview.h"
#include "mainwindow.h"

DialogCameraView::DialogCameraView(Camera *cam, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCameraView), http(NULL), buffer(NULL), camera(cam)
{
        ui->setupUi(this);
}

DialogCameraView::~DialogCameraView()
{
        disconnect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(downloadFinished(int, bool)));
        if (buffer) delete buffer;
        buffer = NULL;
        if (http) delete http;
        http = NULL;

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
        QUrl url(u.c_str());

        http = new QHttp(this);
        connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(downloadFinished(int, bool)));
        buffer = new QBuffer(&bytes);
        buffer->open(QIODevice::WriteOnly);
        http->setHost(url.host());
        request = http->get(url.path(), buffer);
}

void DialogCameraView::downloadFinished(int requestId, bool error)
{
        if (error)
        {
                QMessageBox::warning(this, tr("Calaos Installer: Erreur !"), http->errorString());

                return;
        }

        if (request == requestId)
        {
                QImage img;
                img.loadFromData(bytes);
                ui->label_cam->setPixmap(QPixmap::fromImage(img));

                delete buffer;
                buffer = NULL;
                delete http;
                http = NULL;

                DownloadPicture();
        }
}
