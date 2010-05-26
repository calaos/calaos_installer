#include "dialogplageinput.h"
#include "ui_dialogplageinput.h"

DialogPlageInput::DialogPlageInput(QWidget *parent) :
                QDialog(parent),
                ui(new Ui::DialogPlageInput)
{
        ui->setupUi(this);
}

DialogPlageInput::~DialogPlageInput()
{
        delete ui;
}

void DialogPlageInput::changeEvent(QEvent *e)
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

void DialogPlageInput::setTimeBegin(QTime t)
{
        ui->timeEditStart->setTime(t);
}

void DialogPlageInput::setTimeEnd(QTime t)
{
        ui->timeEditEnd->setTime(t);
}

QTime DialogPlageInput::getTimeBegin()
{
        return ui->timeEditStart->time();
}

QTime DialogPlageInput::getTimeEnd()
{
        return ui->timeEditEnd->time();
}
