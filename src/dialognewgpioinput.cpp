#include "dialognewgpioinput.h"
#include "ui_dialognewgpioinput.h"

DialogNewGpioInput::DialogNewGpioInput(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogNewGpioInput)
{
        ui->setupUi(this);
}

DialogNewGpioInput::~DialogNewGpioInput()
{
        delete ui;
}
