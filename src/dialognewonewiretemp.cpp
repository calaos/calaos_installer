#include "dialognewonewiretemp.h"
#include "ui_DialogNewOneWireTemp.h"

DialogNewOneWireTemp::DialogNewOneWireTemp(Room *r, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogNewOneWireTemp), temperature(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();
}

DialogNewOneWireTemp::~DialogNewOneWireTemp()
{
        delete ui;
}

void DialogNewOneWireTemp::changeEvent(QEvent *e)
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

void DialogNewOneWireTemp::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty() ||
            ui->edit_sensor_id->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toUtf8().constData());
        p.Add("type", "OWTemp");
        p.Add("ow_id", to_string(ui->edit_sensor_id->text().toUtf8().constData()));
        p.Add("ow_args", to_string(ui->edit_owfs_args->text().toUtf8().constData()));

        temperature = ListeRoom::Instance().createInput(p, room);
        accept();
}
