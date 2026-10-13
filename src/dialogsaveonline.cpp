#include "dialogsaveonline.h"
#include "ui_DialogSaveOnline.h"

#include "ConfigOptions.h"
#include <Utils.h>
#include "OnlineConfig.h"
#include <ListeRoom.h>
#include <QMessageBox>

DialogSaveOnline::DialogSaveOnline(QString temp, QWidget *parent):
    QDialog(parent), ui(new Ui::DialogSaveOnline),
    currentDir(temp)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    connect(&networkAccess, &QNetworkAccessManager::sslErrors,
            this, &DialogSaveOnline::sslErrors);

    auths = ConfigOptions::Instance().getAuthList();
    int curr = ConfigOptions::Instance().getCurrentAuthIndex();
    ui->comboIP->clear();
    for (int i = 0;i < auths.size();i++)
    {
        ui->comboIP->addItem(auths[i].host);
        if (i == curr)
        {
            ui->comboIP->setCurrentIndex(i);
            ui->editUsername->setText(auths[i].username);
            ui->editPass->setText(auths[i].password);
        }
    }
}

DialogSaveOnline::~DialogSaveOnline()
{
    delete ui;
}

void DialogSaveOnline::changeEvent(QEvent *e)
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

void DialogSaveOnline::on_buttonBox_accepted()
{
    this->setEnabled(false);

    spinner = new QAnimationLabel(":/img/loader.gif", this);
    ui->spinnerLayout->addWidget(spinner, 0, Qt::AlignCenter);
    spinner->start();

    //save the current auth
    CalaosAuth auth;
    auth.username = ui->editUsername->text();
    auth.password = ui->editPass->text();
    auth.host = ui->comboIP->currentText();
    ConfigOptions::Instance().addAuth(auth);

    auths = ConfigOptions::Instance().getAuthList();
    if (auths.size() != ui->comboIP->count())
    {
        ui->comboIP->addItem(auth.host);
        ui->comboIP->setCurrentIndex(ui->comboIP->count() - 1);
    }

    ConfigOptions::Instance().setCurrentAuthIndex(ui->comboIP->currentIndex());

    uploadXmlFiles(ui->comboIP->currentText());
}

QString DialogSaveOnline::readFileContent(QString fileName)
{
    QFile f(currentDir + "/" + fileName);
    f.open(QIODevice::ReadOnly);

    QByteArray file = f.readAll();
    f.close();

    return QString(file);
}

void DialogSaveOnline::uploadXmlFiles(QString ip)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Calaos Installer"),
                                  tr("Do you want to upload this configuration to Calaos Server at address %1?").arg(ip),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        currentIP = ip;
        QUrl url(QString("https://" + currentIP + "/api.php"));

        if (ip.startsWith("http"))
            url = QUrl(ip);

        // A server that was never read, or announced format 1, may have been updated since.
        if (OnlineConfig::mustProbeBeforePut(Calaos::ListeRoom::Instance().hasDevices(), currentIP))
            probeServerFormat(url);
        else
            putFiles(url);
    }
    else
        reject();
}

void DialogSaveOnline::putFiles(const QUrl &url)
{
    const QByteArray body = OnlineConfig::buildPutRequest(ui->editUsername->text(), ui->editPass->text(),
                                                          readFileContent("io.xml"), readFileContent("rules.xml"));

    QNetworkReply *reply = networkAccess.post(QNetworkRequest(url), body);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { uploadFinished(reply); });
}

void DialogSaveOnline::probeServerFormat(const QUrl &url)
{
    const QByteArray body = OnlineConfig::buildGetRequest(ui->editUsername->text(), ui->editPass->text());

    QNetworkReply *reply = networkAccess.post(QNetworkRequest(url), body);
    connect(reply, &QNetworkReply::finished, this, [this, reply, url]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError)
        {
            fail(tr("Failed to connect!\nPlease check your credentials.\nHTTP error: ") + reply->errorString());
            return;
        }

        const OnlineConfig::GetResult res = OnlineConfig::parseGetResponse(reply->readAll());
        if (!res.ok)
        {
            fail(tr("Error when loading config files!"));
            return;
        }

        OnlineConfig::rememberServerFormat(currentIP, res.ioFormat);

        if (!OnlineConfig::serverAcceptsDevices(res.ioFormat))
        {
            fail(tr("This server is too old to store devices: update calaos_server, "
                    "or remove the devices from this project."));
            return;
        }

        putFiles(url);
    });
}

void DialogSaveOnline::fail(const QString &message)
{
    QMessageBox::critical(this, tr("Calaos Installer"), message);

    this->setEnabled(true);
    delete spinner;
}

void DialogSaveOnline::uploadFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() == QNetworkReply::NoError)
    {
        const OnlineConfig::PutResult res = OnlineConfig::parsePutResponse(reply->readAll());

        if (res.ok)
        {
            QMessageBox::information(this, tr("Calaos Installer"),
                                     tr("Configuration successfully uploaded."));

            accept();
        }
        else if (!res.error.isEmpty())
            fail(tr("The server refused the configuration:\n%1").arg(res.error));
        else
            fail(tr("Error when loading config files!"));
    }
    else
        fail(tr("Failed to connect!\nPlease check your credentials.\nHTTP error: ") + reply->errorString());
}

void DialogSaveOnline::sslErrors(QNetworkReply *reply, const QList<QSslError> &)
{
    reply->ignoreSslErrors();
}

void DialogSaveOnline::on_buttonBox_rejected()
{
    reject();
}

void DialogSaveOnline::on_checkShowPass_toggled(bool checked)
{
    if (checked)
        ui->editPass->setEchoMode(QLineEdit::Normal);
    else
        ui->editPass->setEchoMode(QLineEdit::Password);
}

void DialogSaveOnline::on_pushButtonDelPref_clicked()
{
    QString host = auths[ui->comboIP->currentIndex()].host;
    int ret = QMessageBox::question(this, tr("Calaos Installer"),
                                    tr("Are you sure you want to delete this saved host (%1) ?").arg(host),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        ConfigOptions::Instance().deleteAuth(host);
        ui->comboIP->removeItem(ui->comboIP->currentIndex());
        auths = ConfigOptions::Instance().getAuthList();
    }
}

void DialogSaveOnline::on_comboIP_currentIndexChanged(int index)
{
    if (index < 0 || index >= auths.size())
        return;
    ui->editUsername->setText(auths[index].username);
    ui->editPass->setText(auths[index].password);
}
