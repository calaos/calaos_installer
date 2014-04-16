#include "DialogNewGpioShutter.h"
#include "ui_DialogNewGpioShutter.h"

DialogNewGpioShutter::DialogNewGpioShutter(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewGpioShutter),
    room(r)
{
    ui->setupUi(this);

    ui->label_error_empty->hide();

    on_check_smart_stateChanged(0);
}

DialogNewGpioShutter::~DialogNewGpioShutter()
{
    delete ui;
}

void DialogNewGpioShutter::on_buttonBox_accepted()
{
    if (ui->edit_name->text().isEmpty() ||
        ui->edit_gpio_up->text().isEmpty() ||
        ui->edit_gpio_down->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;
    p.Add("name", ui->edit_name->text().toUtf8().data());
    p.Add("gpio_up", ui->edit_gpio_up->text().toUtf8().data());
    p.Add("gpio_down", ui->edit_gpio_down->text().toUtf8().data());

    if (ui->check_smart->isChecked())
    {
        p.Add("type", "GpioOutputShutterSmart");
        p.Add("time_up", to_string(ui->spin_time_up->value()));
        p.Add("time_down", to_string(ui->spin_time_down->value()));
    }
    else
    {
        p.Add("type", "GpioOutputShutter");
        p.Add("time", to_string(ui->spin_time_up->value()));
    }

    if (ui->check_impulse->isChecked())
    {
        p.Add("impulse_time", "200");
    }

    io = ListeRoom::Instance().createOutput(p, room);

    accept();
}

void DialogNewGpioShutter::on_check_smart_stateChanged(int)
{
    if (!ui->check_smart->isChecked())
    {
        ui->label_time_down->hide();
        ui->spin_time_down->hide();
        ui->label_time_up->setText(tr("Duration (sec.) :"));
    }
    else
    {
        ui->label_time_down->show();
        ui->spin_time_down->show();
        ui->label_time_up->setText(tr("Up duration (sec.) :"));
    }
}
