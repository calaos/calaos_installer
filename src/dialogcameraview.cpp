#include "dialogcameraview.h"
#include "ui_DialogCameraView.h"
#include "mainwindow.h"
#include "networkerrortext.h"
#include <QAuthenticator>
#include <memory>

namespace
{
bool sameOrigin(const QUrl &a, const QUrl &b)
{
    return a.scheme() == b.scheme() && a.host() == b.host() && a.port(-1) == b.port(-1);
}
}

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
    qDebug() << url.toString(QUrl::RemoveUserInfo | QUrl::RemoveQuery);

    QNetworkAccessManager *netmanager = new QNetworkAccessManager(this);
    connect(netmanager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {

        if (reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::warning(this, "Calaos Installer", tr("Calaos Installer: Erreur !") + "\n" + networkErrorText(reply));

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

    // Credentials go through authenticationRequired, never in the request URL,
    // so no error string Qt builds can echo them (in any encoding).
    const QString user = url.userName();
    const QString pass = url.password();
    url.setUserInfo(QString());

    // Only same-origin redirects: the SSL tolerance and the credentials below
    // must never follow the reply to another host or port, nor across a scheme change (either way).
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::SameOriginRedirectPolicy);
    const QUrl origin = url;
    QNetworkReply *reply = netmanager->get(request);

    if (!user.isEmpty() || !pass.isEmpty())
    {
        auto answered = std::make_shared<bool>(false);
        connect(netmanager, &QNetworkAccessManager::authenticationRequired, this,
                [user, pass, answered, origin](QNetworkReply *r, QAuthenticator *auth) {
            if (*answered || !sameOrigin(r->url(), origin)) return;
            *answered = true;
            auth->setUser(user);
            auth->setPassword(pass);
        });
    }

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
