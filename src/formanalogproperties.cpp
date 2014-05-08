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
