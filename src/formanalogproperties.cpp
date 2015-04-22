#include "formanalogproperties.h"
#include "ui_formanalogproperties.h"

FormAnalogProperties::FormAnalogProperties(QWidget *parent, bool isOutput) :
    QWidget(parent),
    ui(new Ui::FormAnalogProperties)
{
    ui->setupUi(this);
    updateUI(isOutput);
}

FormAnalogProperties::~FormAnalogProperties()
{
    delete ui;
}

void FormAnalogProperties::updateUI(bool isOutput)
{
    if (!isOutput)
    {
        ui->spin_step->hide();
        ui->label_step->hide();
    }
    else
    {
        ui->spin_step->show();
        ui->label_step->show();
    }

    //always hide min/max, it's not used in calaos for now
    ui->spin_max->hide();
    ui->spin_min->hide();
    ui->label_max->hide();
    ui->label_min->hide();
}

QString FormAnalogProperties::getUnit()
{
    return ui->edit_unit->text().toUtf8().constData();
}

double FormAnalogProperties::getStep()
{
    return ui->spin_step->value();
}

double FormAnalogProperties::getCoeff()
{
    return ui->spin_coeff->value();
}

double FormAnalogProperties::getOffset()
{
    return ui->spin_offset->value();
}

double FormAnalogProperties::getMin()
{
    return ui->spin_min->value();
}

double FormAnalogProperties::getMax()
{
    return ui->spin_max->value();
}
