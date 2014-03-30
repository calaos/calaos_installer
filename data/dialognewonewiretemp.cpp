#include "dialognewonewiretemp.h"
#include "ui_dialognewonewiretemp.h"

DialogNewOneWireTemp::DialogNewOneWireTemp(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogNewOneWireTemp)
{
        ui->setupUi(this);
}

DialogNewOneWireTemp::~DialogNewOneWireTemp()
{
        delete ui;
}
