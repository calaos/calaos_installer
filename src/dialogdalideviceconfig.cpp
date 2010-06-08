#include "dialogdalideviceconfig.h"
#include "ui_dialogdalideviceconfig.h"

DialogDaliDeviceConfig::DialogDaliDeviceConfig(QWidget *parent) :
                QDialog(parent),
                ui(new Ui::DialogDaliDeviceConfig)
{
        ui->setupUi(this);
}

DialogDaliDeviceConfig::~DialogDaliDeviceConfig()
{
        delete ui;
}

void DialogDaliDeviceConfig::changeEvent(QEvent *e)
{
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}

void DialogDaliDeviceConfig::setFadeRate(int value)
{
        ui->spinFadeRate->setValue(value);
}

int DialogDaliDeviceConfig::getFadeRate()
{
        return ui->spinFadeRate->value();
}

void DialogDaliDeviceConfig::setFadeTime(int value)
{
        ui->spinFadeTime->setValue(value);
}

int DialogDaliDeviceConfig::getFadeTime()
{
        return ui->spinFadeTime->value();
}

void DialogDaliDeviceConfig::setMaxValue(int value)
{
        ui->spinMax->setValue(value);
}

int DialogDaliDeviceConfig::getMaxValue()
{
        return ui->spinMax->value();
}

void DialogDaliDeviceConfig::setMinValue(int value)
{
        ui->spinMin->setValue(value);
}

int DialogDaliDeviceConfig::getMinValue()
{
        return ui->spinMin->value();
}

void DialogDaliDeviceConfig::setSystemFailureLevel(int value)
{
        ui->spinSysFail->setValue(value);
}

int DialogDaliDeviceConfig::getSystemFailureLevel()
{
        return ui->spinSysFail->value();
}

void DialogDaliDeviceConfig::setPowerOnLevel(int value)
{
        ui->spinPowerOn->setValue(value);
}

int DialogDaliDeviceConfig::getPowerOnLevel()
{
        return ui->spinPowerOn->value();
}
