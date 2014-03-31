#include "dialognewdali.h"
#include "ui_DialogNewDali.h"
#include "mainwindow.h"

DialogNewDali::DialogNewDali(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewDali), dali(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();
}

DialogNewDali::~DialogNewDali()
{
        delete ui;
}

void DialogNewDali::changeEvent(QEvent *e)
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

void DialogNewDali::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toUtf8().constData());
        p.Add("line", "1");
        p.Add("type", "WODali");
        if (ui->radioDALI->isChecked())
                p.Add("address", to_string(ui->spin_addr->value()));
        else
                p.Add("address", to_string(ui->spin_addr->value() + 100));
        p.Add("fade_time", to_string(ui->spin_time->value()));

        if (ui->check_group->isChecked())
                p.Add("group", "1");
        else
                p.Add("group", "0");

        dali = ListeRoom::Instance().createOutput(p, room);

        accept();
}

void DialogNewDali::setWagoDali(bool enable)
{
        QString cmd = "WAGO_DALI_SET 1 ";

        if (ui->check_group->isChecked())
                cmd += "1 ";
        else
                cmd += "0 ";

        if (ui->radioDALI->isChecked())
                cmd += QString::number(ui->spin_addr->value());
        else
                cmd += QString::number(ui->spin_addr->value() + 100);

        if (enable)
                cmd += " 100 ";
        else
                cmd += " 0 ";

        cmd += QString::number(ui->spin_time->value());

        //send udp datagram to enable output
        WagoConnect::Instance().SendCommand(cmd);
}

void DialogNewDali::on_pushButton_clicked()
{
        setWagoDali(true);
}

void DialogNewDali::on_pushButton_2_clicked()
{
        setWagoDali(false);
}
