#include "dialognewanalog.h"
#include "ui_DialogNewAnalog.h"
#include "mainwindow.h"

DialogNewAnalog::DialogNewAnalog(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewAnalog), io(NULL), room(r)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    // Insert the generic Analog form
    analogWidget = new FormAnalogProperties(this, false);
    ui->verticalLayout_3->insertWidget(2, analogWidget);

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
    QLocale locale;
    double value;

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
    value = locale.toDouble(ui->spin_WagoMax->text(), NULL);
    p.Add("wago_max", QString::number(value).toStdString());
    value = locale.toDouble(ui->spin_UsrMax->text(), NULL);
    p.Add("real_max",QString::number(value).toStdString());
    p.Add("frequency", "15.0");

    p.Add("unit", analogWidget->getUnit().toUtf8().constData());
    p.Add("coeff_a",  QString::number(analogWidget->getCoeff()).toUtf8().constData());
    p.Add("coeff_b",  QString::number(analogWidget->getOffset()).toUtf8().constData());
    p.Add("step",  QString::number(analogWidget->getStep()).toUtf8().constData());
    p.Add("min",  QString::number(analogWidget->getMin()).toUtf8().constData());
    p.Add("max",  QString::number(analogWidget->getMax()).toUtf8().constData());

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

void DialogNewAnalog::on_radio_input_toggled(bool)
{
    analogWidget->updateUI(ui->radio_output->isChecked());
}
