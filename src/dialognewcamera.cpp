#include "dialognewcamera.h"
#include "ui_DialogNewCamera.h"
#include "mainwindow.h"
#include <QNetworkRequest>
#include <QNetworkReply>

DialogNewCamera::DialogNewCamera(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewCamera), output(nullptr), room(r)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    //hide error labels by default.
    ui->label_error_empty->hide();

    nm = new QNetworkAccessManager(this);
    connect(nm, &QNetworkAccessManager::sslErrors, [](QNetworkReply *reply, const QList<QSslError> &)
    {
        reply->ignoreSslErrors();
    });
}

DialogNewCamera::~DialogNewCamera()
{
    delete ui;
}

void DialogNewCamera::changeEvent(QEvent *e)
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

void DialogNewCamera::on_buttonBox_accepted()
{
    if (ui->edit_name->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;
    p.Add("name", ui->edit_name->text().toUtf8().constData());

    if (ui->tab_camera_model->currentIndex() == 0) //Axis
    {
        p.Add("type", "axis");
        p.Add("port", "80");
        p.Add("host", ui->axis_ip->text().toUtf8().constData());
        p.Add("model", ui->axis_cam->text().toUtf8().constData());
    }
    else if (ui->tab_camera_model->currentIndex() == 1) //Planet
    {
        p.Add("type", "planet");
        p.Add("port", "80");
        p.Add("host", ui->planet_ip->text().toUtf8().constData());
        p.Add("model", ui->planet_model->currentItem()->text().toUtf8().constData());
    }
    else if (ui->tab_camera_model->currentIndex() == 2) //Gadspot
    {
        p.Add("type", "gadspot");
        p.Add("port", "80");
        p.Add("host", ui->gadspot_ip->text().toUtf8().constData());
    }
    else if (ui->tab_camera_model->currentIndex() == 3) //Standard
    {
        p.Add("type", "standard_mjpeg");
        p.Add("port", "80");
        p.Add("host", ui->std_ip->text().toUtf8().constData());
        p.Add("url_jpeg", ui->std_url_jpeg->text().toUtf8().constData());
        p.Add("url_mjpeg", ui->std_url_mjpeg->text().toUtf8().constData());
        p.Add("url_mpeg", ui->std_url_mpeg4->text().toUtf8().constData());
    }
    else if (ui->tab_camera_model->currentIndex() == 4) //Synology
    {
        p.Add("type", "SynoSurveillanceStation");
        p.Add("url", ui->dsm_url->text().toUtf8().constData());
        p.Add("username", ui->dsm_username->text().toUtf8().constData());
        p.Add("password", ui->dsm_password->text().toUtf8().constData());

        auto sel = ui->treeWidgetDsmCam->selectedItems();
        if (sel.count() <= 0)
            p.Add("camera_id", "0");
        else
        {
            auto item = sel.at(0);
            p.Add("camera_id", item->text(0).toUtf8().constData());
        }
    }
    else if (ui->tab_camera_model->currentIndex() == 5) //Foscam
    {
        p.Add("type", "foscam");
        p.Add("port", ui->foscam_port->text().toUtf8().constData());
        p.Add("host", ui->foscam_ip->text().toUtf8().constData());
        p.Add("username", ui->foscam_username->text().toUtf8().constData());
        p.Add("password", ui->foscam_password->text().toUtf8().constData());
    }
    else
    {
        QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Type de caméra inconnu!"));

        return;
    }

    output = ListeRoom::Instance().createCamera(p, room);

    accept();
}

const QString SynoApi = "%1webapi/%2?api=%3&method=%4&version=%5";
const QString SynoApiInfo = "%1webapi/query.cgi?api=SYNO.API.Info&method=Query&version=1&query=%2";

void DialogNewCamera::on_pushButtonConnectDsm_clicked()
{
    ui->treeWidgetDsmCam->clear();
    ui->pushButtonConnectDsm->setEnabled(false);

    authUrl.clear();
    apiSid.clear();
    listUrl.clear();

    if (isRunning)
        return;
    isRunning = true;

    if (authUrl.isEmpty())
    {
        apiSid.clear();
        listUrl.clear();
        getApiInfo("SYNO.API.Auth", "Login", "2", [=](const QString &api)
        {
            if (api.isEmpty())
            {
                isRunning = false;
                ui->pushButtonConnectDsm->setEnabled(true);
                QMessageBox::warning(this, tr("Calaos Installer"), tr("Failed to get api to login"));
                return;
            }

            authUrl = api;
            tryLogin();
        });
    }
    else
    {
        tryLogin();
    }
}

void DialogNewCamera::tryLogin()
{
    if (apiSid.isEmpty())
    {
        login([=](const QString &sid)
        {
            if (sid.isEmpty())
            {
                authUrl.clear();
                apiSid.clear();
                listUrl.clear();
                isRunning = false;
                ui->pushButtonConnectDsm->setEnabled(true);
                QMessageBox::warning(this, tr("Calaos Installer"), tr("Failed to login"));
                return;
            }

            apiSid = sid;
            tryGetList();
        });
    }
    else
    {
        tryGetList();
    }
}

void DialogNewCamera::tryGetList()
{
    if (listUrl.isEmpty())
    {
        getApiInfo("SYNO.SurveillanceStation.Camera", "List", "1", [=](const QString &api)
        {
            if (api.isEmpty())
            {
                authUrl.clear();
                apiSid.clear();
                listUrl.clear();
                isRunning = false;
                ui->pushButtonConnectDsm->setEnabled(true);
                QMessageBox::warning(this, tr("Calaos Installer"), tr("Failed to get api to list cameras"));
                return;
            }

            listUrl = api;
            doGetList();
        });
    }
    else
    {
        doGetList();
    }
}

void DialogNewCamera::doGetList()
{
    listCameras([=](const QJsonArray &camList)
    {
        isRunning = false;
        ui->pushButtonConnectDsm->setEnabled(true);

        bool selected = false;

        for (int i = 0;i < camList.count();i++)
        {
            QJsonObject o = camList.at(i).toObject();
            if (o.contains("id"))
            {
                auto *item = new QTreeWidgetItem(ui->treeWidgetDsmCam);
                item->setText(0, QString::number(o["id"].toInt()));
                item->setText(1, o["name"].toString());
                if (!selected)
                {
                    item->setSelected(true);
                    selected = true;
                }
            }
        }
    });
}

void DialogNewCamera::listCameras(std::function<void(const QJsonArray &arr)> dataCb)
{
    QString url = listUrl;
    url += QStringLiteral("&_sid=%1").arg(apiSid);

    QUrl u(url);
    QNetworkRequest request(u);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto reply = nm->get(request);
    connect(reply, &QNetworkReply::finished, [=]()
    {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::warning(this, tr("Calaos Installer"), reply->errorString());
            dataCb({});
            return;
        }

        QByteArray bytes = reply->readAll();
        bool err;
        QJsonObject jdata = parseResult(bytes, err);

        if (err)
            QMessageBox::warning(this, tr("Calaos Installer"), tr("Failed to list cameras"));

        if (err || !jdata["cameras"].isArray())
        {
            dataCb({});
            return;
        }

        dataCb(jdata["cameras"].toArray());
    });
}

void DialogNewCamera::login(std::function<void(const QString &sid)> cb)
{
    QString url = authUrl;
    url += QStringLiteral("&format=2&account=%1&passwd=%2").arg(ui->dsm_username->text()).arg(ui->dsm_password->text());

    QUrl u(url);
    QNetworkRequest request(u);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto reply = nm->get(request);
    connect(reply, &QNetworkReply::finished, [=]()
    {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::warning(this, tr("Calaos Installer"), reply->errorString());
            cb({});
            return;
        }

        QByteArray bytes = reply->readAll();
        bool err;
        QJsonObject jdata = parseResult(bytes, err);

        if (err || !jdata["sid"].isString())
        {
            cb({});
            return;
        }

        cb(jdata["sid"].toString());
    });
}

void DialogNewCamera::getApiInfo(const QString &api, const QString &method, const QString &version,
                                 std::function<void(const QString &url)> cb)
{
    QString host = ui->dsm_url->text();
    if (!host.endsWith('/')) host.append('/');
    QString url = SynoApiInfo.arg(host).arg(api);

    QUrl u(url);
    QNetworkRequest request(u);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto reply = nm->get(request);
    connect(reply, &QNetworkReply::finished, [=]()
    {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::warning(this, tr("Calaos Installer"), reply->errorString());
            cb({});
            return;
        }

        QByteArray bytes = reply->readAll();
        bool err;
        QJsonObject jdata = parseResult(bytes, err);

        if (err || !jdata[api].isObject())
        {
            cb({});
            return;
        }

        QJsonObject japi = jdata[api].toObject();

        QString apiurl = SynoApi.arg(host)
                                .arg(japi["path"].toString())
                                .arg(api)
                                .arg(method)
                                .arg(version);

        cb({apiurl});
    });
}

QJsonObject DialogNewCamera::parseResult(const QString &data, bool &error)
{
    QJsonParseError jerr;
    QJsonDocument jdoc = QJsonDocument::fromJson(data.toLocal8Bit(), &jerr);
    error = true;

    if (jerr.error != QJsonParseError::NoError)
        return {};

    if (!jdoc.isObject())
        return {};

    QJsonObject o = jdoc.object();

    if (!o["success"].toBool())
        return {};

    if (!o["data"].isObject())
        return {};

    error = false;
    return o["data"].toObject();
}
