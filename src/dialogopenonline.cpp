#include "dialogopenonline.h"
#include "ui_dialogopenonline.h"

#include "ConfigOptions.h"
#include <Utils.h>
#include "json.h"

DialogOpenOnline::DialogOpenOnline(QString temp, QWidget *parent):
                QDialog(parent), ui(new Ui::DialogOpenOnline),
                tempDir(temp)
{
        ui->setupUi(this);

        connect(&networkAccess, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)),
                this, SLOT(sslErrors(QNetworkReply*, const QList<QSslError> &)));

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
                loadXmlFiles(ui->comboIP->currentText());
        }
}

void DialogOpenOnline::downloadFinishedCalaosFr(QNetworkReply *reply)
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
                                loadXmlFiles(result["private_ip"].toString());
                        else
                                loadXmlFiles(result["public_ip"].toString());
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
                QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur!\nLa connexion a échoué.\n") + "Erreur http: " + reply->errorString());

                this->setEnabled(true);
                delete spinner;
        }

        disconnect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(downloadFinishedCalaosFr(QNetworkReply*)));

        reply->deleteLater();
}

void DialogOpenOnline::loadXmlFiles(QString ip)
{
        currentIP = ip;
        QUrl url(QString("https://" + currentIP + "/api.php"));

        QString json_data = "{";
        json_data += "\"cn_user\":\"" + ui->editUsername->text() + "\",";
        json_data += "\"cn_pass\":\"" + ui->editPass->text() + "\",";
        json_data += "\"action\":\"get_files\"";
        json_data += "}";

        connect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(downloadFinishedFiles(QNetworkReply*)));

        networkAccess.post(QNetworkRequest(url), json_data.toUtf8());
}

void DialogOpenOnline::saveXMLFile(QString filename, QString base64_data)
{
        QByteArray data = QByteArray::fromBase64(base64_data.toAscii());

        QFile xml;
        xml.setFileName(tempDir + "/" + filename);
        if (xml.open(QIODevice::WriteOnly | QIODevice::Text))
        {
                QTextStream flux(&xml);
                flux.setCodec("UTF-8");

                flux << QString::fromUtf8(data.data());
        }
        xml.close();
}

void DialogOpenOnline::downloadFinishedFiles(QNetworkReply *reply)
{
        if (reply->error() == QNetworkReply::NoError)
        {
                QByteArray bytes = reply->readAll();
                QString s(bytes);

                bool parse_ok;

                QVariantMap result = Json::parse(s, parse_ok).toMap();

                if (result.contains("io.xml") &&
                    result.contains("rules.xml") &&
                    result.contains("local_config.xml"))
                {
                        saveXMLFile("io.xml", result["io.xml"].toString());
                        saveXMLFile("rules.xml", result["rules.xml"].toString());
                        //TODO: Protect data here.
                        //saveXMLFile("local_config.xml", result["local_config.xml"].toString());

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
                this, SLOT(downloadFinishedFiles(QNetworkReply*)));

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
