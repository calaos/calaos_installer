#include "dialognewdalirgb.h"
#include "ui_DialogNewDaliRGB.h"
#include "mainwindow.h"

DialogNewDaliRGB::DialogNewDaliRGB(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewDaliRGB), dali(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();
}

DialogNewDaliRGB::~DialogNewDaliRGB()
{
        delete ui;
}

void DialogNewDaliRGB::changeEvent(QEvent *e)
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

void DialogNewDaliRGB::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toUtf8().constData());
        p.Add("type", "WODaliRVB");

        p.Add("rline", "1");
        if (ui->radioDALI->isChecked())
                p.Add("raddress", to_string(ui->spin_addr_red->value()));
        else
                p.Add("raddress", to_string(ui->spin_addr_red->value() + 100));
        p.Add("rfade_time", to_string(ui->spin_time->value()));

        if (ui->check_group_red->isChecked())
                p.Add("rgroup", "1");
        else
                p.Add("rgroup", "0");

        p.Add("gline", "1");
        if (ui->radioDALI->isChecked())
                p.Add("gaddress", to_string(ui->spin_addr_green->value()));
        else
                p.Add("gaddress", to_string(ui->spin_addr_green->value() + 100));
        p.Add("gfade_time", to_string(ui->spin_time->value()));

        if (ui->check_group_green->isChecked())
                p.Add("ggroup", "1");
        else
                p.Add("ggroup", "0");

        p.Add("bline", "1");
        if (ui->radioDALI->isChecked())
                p.Add("baddress", to_string(ui->spin_addr_blue->value()));
        else
                p.Add("baddress", to_string(ui->spin_addr_blue->value() + 100));
        p.Add("bfade_time", to_string(ui->spin_time->value()));

        if (ui->check_group_blue->isChecked())
                p.Add("bgroup", "1");
        else
                p.Add("bgroup", "0");

        dali = ListeRoom::Instance().createOutput(p, room);

        accept();
}

void DialogNewDaliRGB::setWagoDali(int address, bool group, bool enable)
{
        QString cmd = "WAGO_DALI_SET 1 ";

        if (group)
                cmd += "1 ";
        else
                cmd += "0 ";

        cmd += QString::number(address);

        if (enable)
                cmd += " 100 ";
        else
                cmd += " 0 ";

        cmd += QString::number(address);

        //send udp datagram to enable output
        WagoConnect::Instance().SendCommand(cmd);
}

void DialogNewDaliRGB::on_pushButton_red_on_clicked()
{
        setWagoDali(ui->spin_addr_red->value(), ui->check_group_red->isChecked(), true);
}

void DialogNewDaliRGB::on_pushButton_red_off_clicked()
{
        setWagoDali(ui->spin_addr_red->value(), ui->check_group_red->isChecked(), false);
}

void DialogNewDaliRGB::on_pushButton_green_on_clicked()
{
        setWagoDali(ui->spin_addr_green->value(), ui->check_group_green->isChecked(), true);
}

void DialogNewDaliRGB::on_pushButton_greenoff_clicked()
{
        setWagoDali(ui->spin_addr_green->value(), ui->check_group_green->isChecked(), false);
}

void DialogNewDaliRGB::on_pushButton_blue_on_clicked()
{
        setWagoDali(ui->spin_addr_blue->value(), ui->check_group_blue->isChecked(), true);
}

void DialogNewDaliRGB::on_pushButton_blue_off_clicked()
{
        setWagoDali(ui->spin_addr_blue->value(), ui->check_group_blue->isChecked(), false);
}
