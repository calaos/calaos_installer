#include "dialogsaveonline.h"
#include "ui_DialogSaveOnline.h"

#include "ConfigOptions.h"
#include <Utils.h>
#include <QJsonDocument>
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

        QJsonObject jroot, jfiles;
        jroot["cn_user"] = ui->editUsername->text();
        jroot["cn_pass"] = ui->editPass->text();
        jroot["action"] = QStringLiteral("config");
        jroot["type"] = QStringLiteral("put");

        jfiles["io.xml"] = readFileContent("io.xml");
        jfiles["rules.xml"] = readFileContent("rules.xml");
        //jfiles["local_config.xml"] = readFileContent("local_config.xml");

        jroot["config_files"] = jfiles;

        QJsonDocument jdoc(jroot);

        connect(&networkAccess, &QNetworkAccessManager::finished,
                this, &DialogSaveOnline::uploadFinished);

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

    disconnect(&networkAccess, &QNetworkAccessManager::finished,
               this, &DialogSaveOnline::uploadFinished);

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
