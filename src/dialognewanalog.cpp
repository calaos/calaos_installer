#include "dialognewanalog.h"
#include "ui_dialognewanalog.h"
#include "mainwindow.h"

DialogNewAnalog::DialogNewAnalog(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewAnalog), io(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();
}

DialogNewAnalog::~DialogNewAnalog()
{
        delete ui;
}

void DialogNewAnalog::changeEvent(QEvent *e)
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

void DialogNewAnalog::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        if (ui->radio_input->isChecked())
                type = "WIAnalog";
        else
                type = "WOAnalog";

        Params p;
        p.Add("name", ui->edit_name->text().toUtf8().data());
        p.Add("type", type);
        p.Add("var", to_string(ui->spin_var->value()));
        p.Add("wago_max", Utils::to_string(ui->spin_WagoMax->text().toUtf8().data()));
        p.Add("real_max", Utils::to_string(ui->spin_UsrMax->text().toUtf8().data()));
        p.Add("unit", ui->line_unit->text().toUtf8().data());
        p.Add("frequency", "15.0");

        if (isInputType())
        {
                io = ListeRoom::Instance().createInput(p, room);
        }
        else
        {
                io = ListeRoom::Instance().createOutput(p, room);
        }

        accept();
}
