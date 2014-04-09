#include "DialogEditTimeRange.h"
#include "ui_DialogEditTimeRange.h"

DialogEditTimeRange::DialogEditTimeRange(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditTimeRange)
{
    ui->setupUi(this);
}

DialogEditTimeRange::~DialogEditTimeRange()
{
    delete ui;
}
