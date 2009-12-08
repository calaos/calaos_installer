#include "dialogconnect.h"
#include "ui_dialogconnect.h"

DialogConnect::DialogConnect(QWidget *parent): QDialog(parent),
        ui(new Ui::DialogConnect)
{
        ui->setupUi(this);
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
        WagoConnect::Instance().setUsername(ui->username->text());
        WagoConnect::Instance().setPassword(ui->password->text());
        WagoConnect::Instance().Connect(ui->ip_address->text(), ui->groupProxy->isChecked());
}
