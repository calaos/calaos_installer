#include "formanalogproperties.h"
#include "ui_formanalogproperties.h"
#include <QDoubleValidator>
#include <QLocale>
#include "Params.h"

FormAnalogProperties::FormAnalogProperties(QWidget *parent, bool isOutput) :
    QWidget(parent),
    ui(new Ui::FormAnalogProperties)
{
    ui->setupUi(this);

    // C locale: the value is written as is to io.xml.
    for (QLineEdit *e: { ui->edit_value_min, ui->edit_value_max })
    {
        QDoubleValidator *v = new QDoubleValidator(e);
        v->setLocale(QLocale::c());
        v->setNotation(QDoubleValidator::StandardNotation);
        e->setValidator(v);
    }
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
        ui->edit_value_min->hide();
        ui->edit_value_max->hide();
        ui->label_min->hide();
        ui->label_max->hide();
    }
    else
    {
        ui->spin_step->show();
        ui->label_step->show();
        ui->edit_value_min->show();
        ui->edit_value_max->show();
        ui->label_min->show();
        ui->label_max->show();
    }
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

QString FormAnalogProperties::getValueMin()
{
    return ui->edit_value_min->text().trimmed();
}

QString FormAnalogProperties::getValueMax()
{
    return ui->edit_value_max->text().trimmed();
}

void FormAnalogProperties::addBoundsTo(Params &p)
{
    // Absent means unbounded for the server.
    if (!getValueMin().isEmpty())
        p.Add("value_min", getValueMin().toUtf8().constData());
    if (!getValueMax().isEmpty())
        p.Add("value_max", getValueMax().toUtf8().constData());
}
