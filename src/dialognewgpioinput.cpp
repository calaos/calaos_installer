#include "dialognewgpioinput.h"
#include "ui_DialogNewGpioInput.h"

DialogNewGpioInput::DialogNewGpioInput(Room *r, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogNewGpioInput), input(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();
}

DialogNewGpioInput::~DialogNewGpioInput()
{
        delete ui;
}

void DialogNewGpioInput::changeEvent(QEvent *e)
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

void DialogNewGpioInput::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toUtf8().constData());
        p.Add("type", "GpioInputSwitch");
        p.Add("gpio_nb", ui->edit_gpio_nb->text().toUtf8().constData());

        input = ListeRoom::Instance().createInput(p, room);
        accept();
}
