#include "DialogEditTimeRange.h"
#include "ui_DialogEditTimeRange.h"

DialogEditTimeRange::DialogEditTimeRange(IOBase *in) :
    QDialog(nullptr),
    ui(new Ui::DialogEditTimeRange),
    inTimeRange(in)
{
    ui->setupUi(this);

    input = new IOBase(in->get_params(), "time_range", TBOOL, IOBase::IO_INPUT);

    input->range_monday = inTimeRange->range_monday;
    input->range_tuesday = inTimeRange->range_tuesday;
    input->range_wednesday = inTimeRange->range_wednesday;
    input->range_thursday = inTimeRange->range_thursday;
    input->range_friday = inTimeRange->range_friday;
    input->range_saturday = inTimeRange->range_saturday;
    input->range_sunday = inTimeRange->range_sunday;
    input->range_months = inTimeRange->range_months;
}

DialogEditTimeRange::~DialogEditTimeRange()
{
    delete ui;
    delete inTimeRange;
}

void DialogEditTimeRange::save()
{
    inTimeRange->range_monday = input->range_monday;
    inTimeRange->range_tuesday = input->range_tuesday;
    inTimeRange->range_wednesday = input->range_wednesday;
    inTimeRange->range_thursday = input->range_thursday;
    inTimeRange->range_friday = input->range_friday;
    inTimeRange->range_saturday = input->range_saturday;
    inTimeRange->range_sunday = input->range_sunday;
    inTimeRange->range_months = input->range_months;
}
