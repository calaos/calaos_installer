#include "dialogopenonline.h"
#include "ui_dialogopenonline.h"

DialogOpenOnline::DialogOpenOnline(QString temp, QWidget *parent):
                QDialog(parent), ui(new Ui::DialogOpenOnline),
                tempDir(temp)
{
        ui->setupUi(this);

        connect(&networkAccess, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)),
                this, SLOT(sslErrors(QNetworkReply*, const QList<QSslError> &)));
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
                //Search IP address from home.calaos.fr
                QUrl url("https://www.calaos.fr/cms/calaos_home/utils/connect_home.php?h=1&u=" + ui->editUsername->text() + "&p=" + ui->editPass->text());

                connect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(downloadFinishedCalaosFr(QNetworkReply*)));
                networkAccess.get(QNetworkRequest(url));
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
                        loadXmlFiles(s);
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

void DialogOpenOnline::loadXmlFiles(QString ip)
{
        //Get io.xml
        QUrl url("https://" + ip + "/getFile.php?file=io.xml&u=" + ui->editUsername->text() + "&p=" + ui->editPass->text());

        connect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(downloadFinishedFiles(QNetworkReply*)));
        currentIP = ip;

        reply_io = networkAccess.get(QNetworkRequest(url));
}

void DialogOpenOnline::downloadFinishedFiles(QNetworkReply *reply)
{
        if (reply->error() == QNetworkReply::NoError)
        {
                QByteArray bytes = reply->readAll();
                QString s(bytes);

                if (s == "No filename given!" || s == "Can't open file")
                {
                        QMessageBox::critical(this, tr("Calaos Installer"),
                                              QString::fromUtf8("Fichier non trouvé !"));

                        this->setEnabled(true);
                        delete spinner;
                }
                else if (s == "<script>location.href = 'index.php';</script>")
                {
                        QMessageBox::critical(this, tr("Calaos Installer"),
                                              QString::fromUtf8("Identifiant/mot de passe invalide."));

                        this->setEnabled(true);
                        delete spinner;
                        reply->deleteLater();

                        disconnect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                                this, SLOT(downloadFinishedFiles(QNetworkReply*)));

                        return;
                }
                else
                {
                        QFile xml;

                        if (reply == reply_io) xml.setFileName(tempDir + "/io.xml");
                        if (reply == reply_rules) xml.setFileName(tempDir + "/rules.xml");

                        if (xml.open(QIODevice::WriteOnly | QIODevice::Text))
                        {
                                QTextStream flux(&xml);
                                flux.setCodec("UTF-8");

                                flux << s;
                        }
                        xml.close();
                }
        }
        else
        {
                QMessageBox::critical(this, tr("Calaos Installer"), "Erreur http: " + reply->errorString());

                this->setEnabled(true);
                delete spinner;

                disconnect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(downloadFinishedFiles(QNetworkReply*)));

                return;
        }

        if (reply == reply_rules)
        {
                disconnect(&networkAccess, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(downloadFinishedFiles(QNetworkReply*)));

                accept();
        }
        else
        {
                QUrl url("https://" + currentIP + "/getFile.php?file=rules.xml&u=" + ui->editUsername->text() + "&p=" + ui->editPass->text());

                reply_rules = networkAccess.get(QNetworkRequest(url));
        }

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
