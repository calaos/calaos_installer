#include "formanalogproperties.h"
#include "ui_formanalogproperties.h"

FormAnalogProperties::FormAnalogProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAnalogProperties)
{
    ui->setupUi(this);
}

FormAnalogProperties::~FormAnalogProperties()
{
    delete ui;
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
