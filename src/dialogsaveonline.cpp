#include "dialogsaveonline.h"
#include "ui_dialogsaveonline.h"

#include "ConfigOptions.h"
#include <Utils.h>
#include "json.h"

DialogSaveOnline::DialogSaveOnline(QString temp, QWidget *parent):
                QDialog(parent), ui(new Ui::DialogSaveOnline),
                currentDir(temp)
{
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

                QString json_data = "{";
                json_data += "\"cn_user\":\"" + ui->editUsername->text() + "\",";
                json_data += "\"cn_pass\":\"" + ui->editPass->text() + "\",";
                json_data += "\"action\":\"get_ip\"";
                json_data += "}";

                connect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(downloadFinishedCalaosFr(QNetworkReply*)));

                networkAccess.post(QNetworkRequest(url), json_data.toUtf8());
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
                QString s(bytes);

                bool parse_ok;

                QVariantMap result = Json::parse(s, parse_ok).toMap();

                if (result.contains("public_ip") && result.contains("private_ip"))
                {
                        if (result.contains("at_home") && result["at_home"].toBool())
                                uploadXmlFiles(result["private_ip"].toString());
                        else
                                uploadXmlFiles(result["public_ip"].toString());
                }
                else
                {
                        QMessageBox::critical(this, tr("Calaos Installer"),
                                              QString::fromUtf8("La connexion a échoué !\nVérifiez les identifiants/mot de passe."));

                        this->setEnabled(true);
                        delete spinner;
                }
        }
        else
        {
                QMessageBox::critical(this, tr("Calaos Installer"), "Erreur!\nLa connexion a échoué.\nErreur http: " + reply->errorString());

                this->setEnabled(true);
                delete spinner;
        }

        disconnect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(downloadFinishedCalaosFr(QNetworkReply*)));

        reply->deleteLater();
}

QString DialogSaveOnline::readFileBase64(QString fileName)
{
        QFile f(currentDir + "/" + fileName);
        f.open(QIODevice::ReadOnly);

        QByteArray file = f.readAll();
        f.close();

        return QString::fromAscii(file.toBase64().data());
}

void DialogSaveOnline::uploadXmlFiles(QString ip)
{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Etes vous sûr de vouloir envoyer cette configuration sur la centrale domotique à l'adresse IP: %1?").arg(ip),
                              QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
                currentIP = ip;
                QUrl url(QString("https://" + currentIP + "/api.php"));

                QString json_data = "{";
                json_data += "\"cn_user\":\"" + ui->editUsername->text() + "\",";
                json_data += "\"cn_pass\":\"" + ui->editPass->text() + "\",";
                json_data += "\"action\":\"set_files\",";
                json_data += "\"io.xml\":\"" + readFileBase64("io.xml") + "\",";
                json_data += "\"rules.xml\":\"" + readFileBase64("rules.xml") + "\",";
                json_data += "\"local_config.xml\":\"" + readFileBase64("local_config.xml") + "\",";
                json_data += "\"reload\":true";
                json_data += "}";

                connect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(uploadFinished(QNetworkReply*)));

                networkAccess.post(QNetworkRequest(url), json_data.toUtf8());
        }
        else
                reject();
}

void DialogSaveOnline::uploadFinished(QNetworkReply *reply)
{
        if (reply->error() == QNetworkReply::NoError)
        {
                QByteArray bytes = reply->readAll();
                QString s(bytes);

                bool parse_ok;
                QVariantMap result = Json::parse(s, parse_ok).toMap();

                if (result.contains("success"))
                {
                        QMessageBox::information(this, tr("Calaos Installer"),
                                              QString::fromUtf8("La configuration a été envoyé"));

                        accept();
                }
                else
                {
                        QMessageBox::critical(this, tr("Calaos Installer"),
                                              QString::fromUtf8("Erreur de chargement des fichiers !"));

                        this->setEnabled(true);
                        delete spinner;
                }
        }
        else
        {
                QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur de connexion !\nVérifiez vos identifiants/mot de passe\n") + "Erreur http: " + reply->errorString());

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
