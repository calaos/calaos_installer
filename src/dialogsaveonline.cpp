#include "dialogsaveonline.h"
#include "ui_dialogsaveonline.h"

DialogSaveOnline::DialogSaveOnline(QString temp, QWidget *parent):
                QDialog(parent), ui(new Ui::DialogSaveOnline),
                currentDir(temp)
{
        ui->setupUi(this);

        connect(&networkAccess, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)),
                this, SLOT(sslErrors(QNetworkReply*, const QList<QSslError> &)));

        connect(&formPost, SIGNAL(uploadDone(QByteArray&)),
                this, SLOT(uploadFinished(QByteArray&)));
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
                //Search IP address from home.calaos.fr
                QUrl url("https://www.calaos.fr/cms/calaos_home/utils/connect_home.php?h=1&u=" + ui->editUsername->text() + "&p=" + ui->editPass->text());

                connect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(downloadFinishedCalaosFr(QNetworkReply*)));
                networkAccess.get(QNetworkRequest(url));
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

                if (s == "empty login" || s == "not found")
                {
                        QMessageBox::critical(this, tr("Calaos Installer"),
                                              QString::fromUtf8("Utilisateur inconnu !"));

                        this->setEnabled(true);
                        delete spinner;
                }
                else if (s == "failed")
                {
                        QMessageBox::critical(this, tr("Calaos Installer"),
                                              QString::fromUtf8("La connexion a échoué, peut-être que la connexion Internet n'est pas active."));

                        this->setEnabled(true);
                        delete spinner;
                }
                else
                {
                        uploadXmlFiles(s);
                }
        }
        else
        {
                QMessageBox::critical(this, tr("Calaos Installer"), "Erreur http: " + reply->errorString());

                this->setEnabled(true);
                delete spinner;
        }

        disconnect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(downloadFinishedCalaosFr(QNetworkReply*)));

        reply->deleteLater();
}

void DialogSaveOnline::uploadXmlFiles(QString ip)
{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Etes vous sûr de vouloir envoyer cette configuration sur la centrale domotique à l'adresse IP: %1?").arg(ip),
                              QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
                formPost.setUserAgent("Calaos/agent 1.0");

                if (QFile::exists(currentDir + "/io.xml") &&
                    QFile::exists(currentDir + "/rules.xml"))
                {
                        formPost.addField("upload", "CalaosUploader");
                        formPost.addFile("io", currentDir + "/io.xml", "text/xml");
                        formPost.addFile("rules", currentDir + "/rules.xml", "text/xml");

                        formPost.postData("https://" + ip + "/putFile.php?u=" + ui->editUsername->text() + "&p=" + ui->editPass->text());

                        currentIP = ip;
                }
        }
        else
                reject();
}

void DialogSaveOnline::uploadFinished(QByteArray &reply)
{
        QString s(reply);

        if (s == "Le fichier est introuvable")
        {
                QMessageBox::critical(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Le fichier envoyé n'existe pas !"));

                this->setEnabled(true);
                delete spinner;
        }
        else if (s == "Erreur de copie")
        {
                QMessageBox::critical(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Erreur lors de la copie des fichiers !"));

                this->setEnabled(true);
                delete spinner;
        }
        else if (s == "Pas d'upload")
        {
                QMessageBox::critical(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Aucun fichier n'a été envoyé !"));

                this->setEnabled(true);
                delete spinner;
        }
        else if (s == "<script>location.href = 'index.php';</script>")
        {
                QMessageBox::critical(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Identifiant/mot de passe invalide."));

                this->setEnabled(true);
                delete spinner;
        }
        else if (s == "OK")
        {
                //Restart calaosd
                QUrl url("https://" + currentIP + "/action.php?action=reboot&value=calaosd&u=" + ui->editUsername->text() + "&p=" + ui->editPass->text());

                connect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(downloadFinishedRestart(QNetworkReply*)));
                networkAccess.get(QNetworkRequest(url));
        }
        else
        {
                QMessageBox::critical(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Erreur inconnue !\n\n") + s);

                this->setEnabled(true);
                delete spinner;
        }
}

void DialogSaveOnline::downloadFinishedRestart(QNetworkReply *reply)
{
        if (reply->error() == QNetworkReply::NoError)
        {
                accept();
        }
        else
        {
                QMessageBox::critical(this, tr("Calaos Installer"), "Erreur http: " + reply->errorString());

                this->setEnabled(true);
                delete spinner;
        }

        disconnect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(downloadFinishedCalaosFr(QNetworkReply*)));

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
