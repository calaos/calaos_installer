#include "dialogsaveonline.h"
#include "ui_DialogSaveOnline.h"

#include "ConfigOptions.h"
#include <Utils.h>
#include <QJsonDocument>

DialogSaveOnline::DialogSaveOnline(QString temp, QWidget *parent):
    QDialog(parent), ui(new Ui::DialogSaveOnline),
    currentDir(temp)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    connect(&networkAccess, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)),
            this, SLOT(sslErrors(QNetworkReply*, const QList<QSslError> &)));

    ui->comboIP->lineEdit()->setText(ConfigOptions::Instance().getHost());
    ui->calaosfrCheck->setChecked(ConfigOptions::Instance().useCalaosFr());
    ui->editUsername->setText(ConfigOptions::Instance().getUsername());
    ui->editPass->setText(ConfigOptions::Instance().getPassword());
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

    ConfigOptions::Instance().setHost(ui->comboIP->lineEdit()->text());
    ConfigOptions::Instance().setUseCalaosFr(ui->calaosfrCheck->isChecked());
    ConfigOptions::Instance().setUsername(ui->editUsername->text());
    ConfigOptions::Instance().setPassword(ui->editPass->text());

    loadFromNetwork();
}

void DialogSaveOnline::on_calaosfrCheck_stateChanged(int)
{
    ui->comboIP->setEnabled(!ui->calaosfrCheck->isChecked());
}

void DialogSaveOnline::loadFromNetwork()
{
    if (ui->calaosfrCheck->isChecked())
    {
        //Search IP address from calaos network
        QString cn = CALAOS_NETWORK_URL "/api.php";
        QUrl url(cn);

        QJsonObject jroot;
        jroot["cn_user"] = ui->editUsername->text();
        jroot["cn_pass"] = ui->editPass->text();
        jroot["action"] = QStringLiteral("get_ip");
        QJsonDocument jdoc(jroot);

        connect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(downloadFinishedCalaosFr(QNetworkReply*)));

        networkAccess.post(QNetworkRequest(url), jdoc.toJson());
    }
    else
    {
        uploadXmlFiles(ui->comboIP->currentText());
    }
}

void DialogSaveOnline::downloadFinishedCalaosFr(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QJsonParseError err;
        QJsonDocument jdoc = QJsonDocument::fromJson(bytes, &err);
        QVariantMap result = jdoc.object().toVariantMap();

        if (err.error == QJsonParseError::NoError &&
            result.contains("public_ip") &&
            result.contains("private_ip"))
        {
            if (result.contains("at_home") && result["at_home"].toBool())
                uploadXmlFiles(result["private_ip"].toString());
            else
                uploadXmlFiles(result["public_ip"].toString());
        }
        else
        {
            QMessageBox::critical(this, tr("Calaos Installer"),
                                  tr("Failed to connect!\nPlease check your credentials."));

            this->setEnabled(true);
            delete spinner;
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Calaos Installer"), tr("Failed to connect!\nPlease check your credentials.\nHTTP error: ") + reply->errorString());

        this->setEnabled(true);
        delete spinner;
    }

    disconnect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(downloadFinishedCalaosFr(QNetworkReply*)));

    reply->deleteLater();
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

        QJsonObject jroot, jfiles;
        jroot["cn_user"] = ui->editUsername->text();
        jroot["cn_pass"] = ui->editPass->text();
        jroot["action"] = QStringLiteral("config");
        jroot["type"] = QStringLiteral("put");

        jfiles["io.xml"] = readFileContent("io.xml");
        jfiles["rules.xml"] = readFileContent("rules.xml");
        jfiles["local_config.xml"] = readFileContent("local_config.xml");

        jroot["config_files"] = jfiles;

        QJsonDocument jdoc(jroot);

        connect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(uploadFinished(QNetworkReply*)));

        networkAccess.post(QNetworkRequest(url), jdoc.toJson());
    }
    else
        reject();
}

void DialogSaveOnline::uploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QJsonParseError err;
        QJsonDocument jdoc = QJsonDocument::fromJson(bytes, &err);
        QVariantMap result = jdoc.object().toVariantMap();

        if (err.error == QJsonParseError::NoError &&
            result.contains("success") &&
            result["success"].toString() == "true")
        {
            QMessageBox::information(this, tr("Calaos Installer"),
                                     tr("Configuration successfully uploaded."));

            accept();
        }
        else
        {
            QMessageBox::critical(this, tr("Calaos Installer"),
                                  tr("Error when loading config files!"));

            this->setEnabled(true);
            delete spinner;
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Calaos Installer"), tr("Failed to connect!\nPlease check your credentials.\nHTTP error: ") + reply->errorString());

        this->setEnabled(true);
        delete spinner;
    }

    disconnect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(uploadFinished(QNetworkReply*)));

    reply->deleteLater();
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
