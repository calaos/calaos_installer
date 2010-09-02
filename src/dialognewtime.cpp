#include "dialognewtime.h"
#include "ui_dialognewtime.h"
#include "mainwindow.h"

DialogNewTime::DialogNewTime(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewTime), input(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();
}

DialogNewTime::~DialogNewTime()
{
        delete ui;
}

void DialogNewTime::changeEvent(QEvent *e)
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

void DialogNewTime::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toUtf8().constData());

        if (ui->tab_model->currentIndex() == 0) //Time/DateTime
        {
                p.Add("type", "InputTime");

                if (ui->check_time->isChecked())
                {
                        p.Add("hour", to_string(ui->heure->time().hour()));
                        p.Add("min", to_string(ui->heure->time().minute()));
                        p.Add("sec", to_string(ui->heure->time().second()));
                }
                else if (ui->check_datetime->isChecked())
                {
                        p.Add("hour", to_string(ui->date_heure->time().hour()));
                        p.Add("min", to_string(ui->date_heure->time().minute()));
                        p.Add("sec", to_string(ui->date_heure->time().second()));
                        p.Add("day", to_string(ui->date_heure->date().day()));
                        p.Add("month", to_string(ui->date_heure->date().month()));
                        p.Add("year", to_string(ui->date_heure->date().year()));
                }
        }
        else if (ui->tab_model->currentIndex() == 1) //Timer
        {
                p.Add("type", "InputTimer");
                p.Add("hour", to_string(ui->timer->time().hour()));
                p.Add("min", to_string(ui->timer->time().minute()));
                p.Add("sec", to_string(ui->timer->time().second()));
                p.Add("msec", to_string(ui->timer->time().msec()));
        }
        else if (ui->tab_model->currentIndex() == 2) //Plage
        {
                p.Add("type", "InPlageHoraire");
        }
        else
        {
                QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Type inconnu!"));

                return;
        }

        input = ListeRoom::Instance().createInput(p, room);

        accept();
}

void DialogNewTime::on_check_time_clicked()
{
        if (ui->check_time->isChecked())
        {
                ui->heure->setEnabled(true);
                ui->date_heure->setEnabled(false);
        }
        else if (ui->check_datetime->isChecked())
        {
                ui->heure->setEnabled(false);
                ui->date_heure->setEnabled(true);
        }
}

void DialogNewTime::on_check_datetime_clicked()
{
        on_check_time_clicked();
}
