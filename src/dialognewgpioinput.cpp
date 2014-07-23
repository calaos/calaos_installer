#include "dialognewgpioinput.h"
#include "ui_DialogNewGpioInput.h"

DialogNewGpioInput::DialogNewGpioInput(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewGpioInput), input(NULL), room(r)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
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
    if (ui->edit_name->text().isEmpty() ||
        ui->edit_gpio_nb->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;

    string type;

    if (ui->switch_type_combo->currentIndex() == 1)
        type = "GpioInputSwitchTriple";
    else if (ui->switch_type_combo->currentIndex() == 2)
        type = "GpioInputSwitchLongPress";
    else
        type = "GpioInputSwitch";


    p.Add("name", ui->edit_name->text().toUtf8().constData());
    p.Add("type", type);
    p.Add("gpio", ui->edit_gpio_nb->text().toUtf8().constData());

    input = ListeRoom::Instance().createInput(p, room);
    accept();
}
