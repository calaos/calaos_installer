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

    loadXmlFiles(ui->comboIP->currentText());
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

void DialogOpenOnline::on_pushButtonDelPref_clicked()
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

void DialogOpenOnline::on_comboIP_currentIndexChanged(int index)
{
    if (index < 0 || index >= auths.size())
        return;
    ui->editUsername->setText(auths[index].username);
    ui->editPass->setText(auths[index].password);
}
