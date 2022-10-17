#include "dialogconnect.h"
#include "ui_DialogConnect.h"

#include "ConfigOptions.h"

DialogConnect::DialogConnect(bool ipOnly, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogConnect),
    iponly(ipOnly)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ui->ip_address->setText(ConfigOptions::Instance().getWagoHost());
    //        ui->checkBoxCalaosFr->setChecked(ConfigOptions::Instance().useCalaosFr());
    //        ui->groupProxy->setChecked(ConfigOptions::Instance().useCalaosServer());
    //        ui->username->setText(ConfigOptions::Instance().getUsername());
    //ui->password->setText(ConfigOptions::Instance().getPassword());
}

DialogConnect::~DialogConnect()
{
}

void DialogConnect::changeEvent(QEvent *e)
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

void DialogConnect::on_buttonBox_accepted()
{
    ConfigOptions::Instance().setWagoHost(ui->ip_address->text());
    //        ConfigOptions::Instance().setUseCalaosFr(ui->checkBoxCalaosFr->isChecked());
    //        ConfigOptions::Instance().setUseCalaosServer(ui->groupProxy->isChecked());
    //        ConfigOptions::Instance().setUsername(ui->username->text());
    //        ConfigOptions::Instance().setPassword(ui->password->text());

    //        WagoConnect::Instance().setUsername(ui->username->text());
    //        WagoConnect::Instance().setPassword(ui->password->text());
    if (!iponly)
        WagoConnect::Instance().Connect(ui->ip_address->text(), false);
}
