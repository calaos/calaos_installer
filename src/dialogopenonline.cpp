#include "dialogopenonline.h"
#include "ui_DialogOpenOnline.h"

#include "ConfigOptions.h"
#include "OnlineConfig.h"
#include "OnlineSession.h"
#include <Utils.h>
#include <QJsonDocument>
#include <QMessageBox>

DialogOpenOnline::DialogOpenOnline(QString temp, QWidget *parent):
    QDialog(parent), ui(new Ui::DialogOpenOnline),
    tempDir(temp)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    online = new OnlineSession(this);
    connect(online, &OnlineSession::getFinished, this, &DialogOpenOnline::downloadFinished);

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
    online->setServer(ip);
    online->setCredentials(ui->editUsername->text(), ui->editPass->text(), ui->editAdminPass->text());
    online->getConfig();
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

void DialogOpenOnline::downloadFinished(bool reached, const QByteArray &body, const QString &networkError)
{
    if (reached)
    {
        const OnlineConfig::GetResult res = OnlineConfig::parseGetResponse(body);

        if (res.ok)
        {
            OnlineConfig::rememberServerFormat(currentIP, res.ioFormat);

            if (res.hasFiles)
            {
                saveXMLFile("io.xml", res.ioXml);
                saveXMLFile("rules.xml", res.rulesXml);

                accept();
            }
            else
            {
                QMessageBox::critical(this, tr("Calaos Installer"),
                                      tr("The server answer has no io.xml or rules.xml."));

                this->setEnabled(true);
                delete spinner;
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Calaos Installer"),
                                  res.error.isEmpty() ? tr("Failed to load config files!")
                                                      : tr("The server refused the request:\n%1").arg(res.error));

            this->setEnabled(true);
            delete spinner;
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Calaos Installer"), tr("Failed to connect!\nPlease check your credentials.\nHTTP error: ") + networkError);

        this->setEnabled(true);
        delete spinner;
    }
}

void DialogOpenOnline::on_buttonBox_rejected()
{
    reject();
}

void DialogOpenOnline::on_checkShowPass_toggled(bool checked)
{
    const QLineEdit::EchoMode mode = checked ? QLineEdit::Normal : QLineEdit::Password;
    ui->editPass->setEchoMode(mode);
    ui->editAdminPass->setEchoMode(mode);
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
    // Another box has its own admin password.
    ui->editAdminPass->clear();
}
