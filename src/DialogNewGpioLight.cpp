#include "DialogNewGpioLight.h"
#include "ui_DialogNewGpioLight.h"

DialogNewGpioLight::DialogNewGpioLight(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewGpioLight),
    room(r)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ui->label_error_empty->hide();
}

DialogNewGpioLight::~DialogNewGpioLight()
{
    delete ui;
}

void DialogNewGpioLight::on_buttonBox_accepted()
{
    if (ui->edit_name->text().isEmpty() ||
        ui->edit_gpio_nb->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;

    p.Add("name", ui->edit_name->text().toUtf8().constData());
    p.Add("type", "GpioOutputSwitch");
    p.Add("gpio_nb", ui->edit_gpio_nb->text().toUtf8().constData());

    io = ListeRoom::Instance().createOutput(p, room);

    accept();
}
