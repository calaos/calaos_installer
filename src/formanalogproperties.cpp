#include "formanalogproperties.h"
#include "ui_formanalogproperties.h"
#include <QDoubleValidator>
#include <QLineEdit>
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
        v->setNotation(QDoubleValidator::StandardNotation);
        QLocale c = QLocale::c();
        c.setNumberOptions(QLocale::RejectGroupSeparator);
        v->setLocale(c);
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

// Empty and hidden fields mean "no bound": switching output to input must not keep a stale one.
static QString boundText(QLineEdit *e, QWidget *form)
{
    return e->isVisibleTo(form) ? e->text().trimmed() : QString();
}

QString FormAnalogProperties::getValueMin()
{
    return boundText(ui->edit_value_min, this);
}

QString FormAnalogProperties::getValueMax()
{
    return boundText(ui->edit_value_max, this);
}

QString FormAnalogProperties::boundsError()
{
    for (QLineEdit *e: { ui->edit_value_min, ui->edit_value_max })
    {
        if (!boundText(e, this).isEmpty() && !e->hasAcceptableInput())
            return tr("The minimum and maximum values must be plain numbers.");
    }

    const QString lo = getValueMin(), hi = getValueMax();
    if (!lo.isEmpty() && !hi.isEmpty() && lo.toDouble() > hi.toDouble())
        return tr("The minimum value cannot be greater than the maximum value.");

    return QString();
}

void FormAnalogProperties::addBoundsTo(Params &p)
{
    if (!boundsError().isEmpty())
        return;

    if (!getValueMin().isEmpty())
        p.Add("value_min", getValueMin().toUtf8().constData());
    if (!getValueMax().isEmpty())
        p.Add("value_max", getValueMax().toUtf8().constData());
}
