#include "dialogopenonline.h"
#include "ui_DialogOpenOnline.h"

#include "ConfigOptions.h"
#include <Utils.h>
#include <QJsonDocument>
#include <QMessageBox>

DialogOpenOnline::DialogOpenOnline(QString temp, QWidget *parent):
    QDialog(parent), ui(new Ui::DialogOpenOnline),
    tempDir(temp)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    connect(&networkAccess, &QNetworkAccessManager::sslErrors,
            this, &DialogOpenOnline::sslErrors);

    ui->comboIP->lineEdit()->setText(ConfigOptions::Instance().getHost());
    ui->calaosfrCheck->setChecked(ConfigOptions::Instance().useCalaosFr());
    ui->editUsername->setText(ConfigOptions::Instance().getUsername());
    ui->editPass->setText(ConfigOptions::Instance().getPassword());
}

DialogOpenOnline::~DialogOpenOnline()
{
    delete ui;
}

void DialogOpenOnline::changeEvent(QEvent *e)
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

void DialogOpenOnline::on_buttonBox_accepted()
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

void DialogOpenOnline::on_calaosfrCheck_stateChanged(int)
{
    ui->comboIP->setEnabled(!ui->calaosfrCheck->isChecked());
}

void DialogOpenOnline::loadFromNetwork()
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

        connect(&networkAccess, &QNetworkAccessManager::finished,
                this, &DialogOpenOnline::downloadFinishedCalaosFr);

        networkAccess.post(QNetworkRequest(url), jdoc.toJson());
    }
    else
    {
        loadXmlFiles(ui->comboIP->currentText());
    }
}

void DialogOpenOnline::downloadFinishedCalaosFr(QNetworkReply *reply)
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
                loadXmlFiles(result["private_ip"].toString());
            else
                loadXmlFiles(result["public_ip"].toString());
        }
        else
        {
            QMessageBox::critical(this, tr("Calaos Installer"),
                                  tr("Failed to connect!\nPlease check your credentials.\nHTTP error: ") + reply->errorString());

            this->setEnabled(true);
            delete spinner;
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Calaos Installer"), tr("Error!\nFailed to connect.\nHTTP error: ") + reply->errorString());

        this->setEnabled(true);
        delete spinner;
    }

    disconnect(&networkAccess, &QNetworkAccessManager::finished,
               this, &DialogOpenOnline::downloadFinishedCalaosFr);

    reply->deleteLater();
}

void DialogOpenOnline::loadXmlFiles(QString ip)
{
    currentIP = ip;
    QUrl url(QString("https://" + currentIP + "/api.php"));

    if (ip.startsWith("http"))
        url = QUrl(ip);

    QJsonObject jroot;
    jroot["cn_user"] = ui->editUsername->text();
    jroot["cn_pass"] = ui->editPass->text();
    jroot["action"] = QStringLiteral("config");
    jroot["type"] = QStringLiteral("get");
    QJsonDocument jdoc(jroot);

    connect(&networkAccess, &QNetworkAccessManager::finished,
            this, &DialogOpenOnline::downloadFinishedFiles);

    networkAccess.post(QNetworkRequest(url), jdoc.toJson());
}

void DialogOpenOnline::saveXMLFile(QString filename, QString data)
{
    QFile xml;
    xml.setFileName(tempDir + "/" + filename);
    if (xml.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&xml);
        stream << data;
    }
    xml.close();
}

void DialogOpenOnline::downloadFinishedFiles(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QJsonParseError err;
        QJsonDocument jdoc = QJsonDocument::fromJson(bytes, &err);
        QVariantMap jroot = jdoc.object().toVariantMap();

        if (err.error == QJsonParseError::NoError &&
            jroot["success"].toString() == "true")
        {
            QVariantMap files = jroot["config_files"].toMap();

            if (files.contains("io.xml") &&
                files.contains("rules.xml")/* &&
                files.contains("local_config.xml")*/)
            {
                saveXMLFile("io.xml", files["io.xml"].toString());
                saveXMLFile("rules.xml", files["rules.xml"].toString());
                //saveXMLFile("local_config.xml", files["local_config.xml"].toString());

                accept();
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Calaos Installer"),
                                  tr("Failed to load config files!"));

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

    disconnect(&networkAccess, &QNetworkAccessManager::finished,
               this, &DialogOpenOnline::downloadFinishedFiles);

    reply->deleteLater();
}

void DialogOpenOnline::sslErrors(QNetworkReply *reply, const QList<QSslError> &)
{
    reply->ignoreSslErrors();
}

void DialogOpenOnline::on_buttonBox_rejected()
{
    reject();
}

void DialogOpenOnline::on_checkShowPass_toggled(bool checked)
{
    if (checked)
        ui->editPass->setEchoMode(QLineEdit::Normal);
    else
        ui->editPass->setEchoMode(QLineEdit::Password);
}
