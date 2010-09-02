#include "dialognewvolet.h"
#include "ui_dialognewvolet.h"
#include "mainwindow.h"

DialogNewVolet::DialogNewVolet(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewVolet), volet(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();

        on_check_smart_stateChanged(0);
}

DialogNewVolet::~DialogNewVolet()
{
        delete ui;
}

void DialogNewVolet::changeEvent(QEvent *e)
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

void DialogNewVolet::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toUtf8().data());
        p.Add("var_up", to_string(ui->spin_var_up->value()));
        p.Add("var_down", to_string(ui->spin_var_down->value()));

        if (ui->check_smart->isChecked())
        {
                p.Add("type", "WOVoletSmart");
                p.Add("time_up", to_string(ui->spin_time_up->value()));
                p.Add("time_down", to_string(ui->spin_time_down->value()));
        }
        else
        {
                p.Add("type", "WOVolet");
                p.Add("time", to_string(ui->spin_time_up->value()));
        }

        if (ui->check_impulse->isChecked())
        {
                p.Add("impulse_time", "200");
        }

        volet = ListeRoom::Instance().createOutput(p, room);

        accept();
}

void DialogNewVolet::on_check_smart_stateChanged(int )
{
        if (!ui->check_smart->isChecked())
        {
                ui->label_time_down->hide();
                ui->spin_time_down->hide();
                ui->label_time_up->setText(QString::fromUtf8("Durée (sec.) :"));
        }
        else
        {
                ui->label_time_down->show();
                ui->spin_time_down->show();
                ui->label_time_up->setText(QString::fromUtf8("Durée de la montée (sec.) :"));
        }
}
