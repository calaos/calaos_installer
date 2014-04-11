#include "DialogEditTimeRange.h"
#include "ui_DialogEditTimeRange.h"
#include "TwoLineItemDelegate.h"

DialogEditTimeRange::DialogEditTimeRange(IOBase *in) :
    QDialog(nullptr),
    ui(new Ui::DialogEditTimeRange),
    inTimeRange(in)
{
    ui->setupUi(this);
    ui->treeRanges->setItemDelegate(new TwoLineItemDelegate());

    QStringList headers;
    headers << tr("Time range");
    ui->treeRanges->setHeaderLabels(headers);

    items_months.push_back(ui->checkBoxJanuary);
    items_months.push_back(ui->checkBoxFebruar);
    items_months.push_back(ui->checkBoxMarch);
    items_months.push_back(ui->checkBoxApril);
    items_months.push_back(ui->checkBoxMay);
    items_months.push_back(ui->checkBoxJune);
    items_months.push_back(ui->checkBoxJuly);
    items_months.push_back(ui->checkBoxAugust);
    items_months.push_back(ui->checkBoxSeptember);
    items_months.push_back(ui->checkBoxOctober);
    items_months.push_back(ui->checkBoxNovember);
    items_months.push_back(ui->checkBoxDecember);

    input = new IOBase(in->get_params(), "time_range", TBOOL, IOBase::IO_INPUT);

    input->range_monday = inTimeRange->range_monday;
    input->range_tuesday = inTimeRange->range_tuesday;
    input->range_wednesday = inTimeRange->range_wednesday;
    input->range_thursday = inTimeRange->range_thursday;
    input->range_friday = inTimeRange->range_friday;
    input->range_saturday = inTimeRange->range_saturday;
    input->range_sunday = inTimeRange->range_sunday;
    input->range_months = inTimeRange->range_months;

    reloadTimeRanges();
}

DialogEditTimeRange::~DialogEditTimeRange()
{
    delete ui;
    delete input;
}

void DialogEditTimeRange::save()
{
    //set months from checkboxes
    for (uint i = 0;i < items_months.size();i++)
        input->range_months.set(i, items_months[i]->isChecked());

    inTimeRange->range_monday = input->range_monday;
    inTimeRange->range_tuesday = input->range_tuesday;
    inTimeRange->range_wednesday = input->range_wednesday;
    inTimeRange->range_thursday = input->range_thursday;
    inTimeRange->range_friday = input->range_friday;
    inTimeRange->range_saturday = input->range_saturday;
    inTimeRange->range_sunday = input->range_sunday;
    inTimeRange->range_months = input->range_months;
}

void DialogEditTimeRange::reloadTimeRanges()
{
    ui->treeRanges->clear();

    vector<TimeRange> trange_sorted;
    auto sortTimeRange = [&trange_sorted](vector<TimeRange> &range, int day)
    {
        for (TimeRange &t: range)
        {
            auto it = std::find(trange_sorted.begin(), trange_sorted.end(), t);
            if (it != trange_sorted.end())
            {
                //timerange already in trange_sorted, add the day of week flag
                it->dayOfWeek.set(day);
            }
            else
            {
                TimeRange newtr = t;
                newtr.dayOfWeek.reset();
                newtr.dayOfWeek.set(day);
                trange_sorted.push_back(newtr);
            }
        }
    };

    sortTimeRange(input->range_monday, 0);
    sortTimeRange(input->range_tuesday, 1);
    sortTimeRange(input->range_wednesday, 2);
    sortTimeRange(input->range_thursday, 3);
    sortTimeRange(input->range_friday, 4);
    sortTimeRange(input->range_saturday, 5);
    sortTimeRange(input->range_sunday, 6);

    for (TimeRange &range: trange_sorted)
        addTreeItem(range);

    //reload months
    for (uint i = 0;i < items_months.size();i++)
        items_months[i]->setChecked(input->range_months.test(i));

    ui->groupBoxRange->setEnabled(false);
    clearRangeEdit();
}

void DialogEditTimeRange::addTreeItem(TimeRange &range)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeRanges);

    bool same = range.isSameStartEnd();
    string starttxt, subtxt;
    if (same)
        starttxt = tr("Execute at ").toUtf8().constData();
    else
        starttxt = tr("Start at ").toUtf8().constData();

    auto offsetString = [=](bool isstart)
    {
        if (isstart)
        {
            int h, m, s;
            from_string(range.shour, h);
            from_string(range.smin, m);
            from_string(range.ssec, s);
            int v = h * 3600 + m * 60 + s;

            if (range.start_offset == 1)
                return string(" +") + Utils::time2string_digit(v);
            else if (range.start_offset == -1)
                return string(" -") + Utils::time2string_digit(v);

        }
        else
        {
            int h, m, s;
            from_string(range.ehour, h);
            from_string(range.emin, m);
            from_string(range.esec, s);
            int v = h * 3600 + m * 60 + s;

            if (range.end_offset == 1)
                return string(" +") + Utils::time2string_digit(v);
            else
                if (range.end_offset == -1)
                    return string(" -") + Utils::time2string_digit(v);
        }

        return string();
    };

    if (range.start_type == TimeRange::HTYPE_NORMAL)
        starttxt = starttxt + Utils::time2string_digit(range.getStartTimeSec());
    else if (range.start_type == TimeRange::HTYPE_SUNRISE)
        starttxt = starttxt + tr("Sunrise").toUtf8().constData() + offsetString(true) + " (" + Utils::time2string_digit(range.getStartTimeSec()) + ")";
    else if (range.start_type == TimeRange::HTYPE_SUNSET)
        starttxt = starttxt + tr("Sunset").toUtf8().constData() + offsetString(true) + " (" + Utils::time2string_digit(range.getStartTimeSec()) + ")";
    else if (range.start_type == TimeRange::HTYPE_NOON)
        starttxt = starttxt + tr("Noon").toUtf8().constData() + offsetString(true) + " (" + Utils::time2string_digit(range.getStartTimeSec()) + ")";

    item->setData(0, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(starttxt.c_str()));

    if (!same)
    {
        subtxt = tr("Stop at ").toUtf8().constData();

        if (range.end_type == TimeRange::HTYPE_NORMAL)
            subtxt += Utils::time2string_digit(range.getEndTimeSec());
        else if (range.end_type == TimeRange::HTYPE_SUNRISE)
            subtxt += tr("Sunrise").toUtf8().constData() + offsetString(false) + " (" + Utils::time2string_digit(range.getEndTimeSec()) + ")";
        else if (range.end_type == TimeRange::HTYPE_SUNSET)
            subtxt += tr("Sunset").toUtf8().constData() + offsetString(false) + " (" + Utils::time2string_digit(range.getEndTimeSec()) + ")";
        else if (range.end_type == TimeRange::HTYPE_NOON)
            subtxt += tr("Noon").toUtf8().constData() + offsetString(false) + " (" + Utils::time2string_digit(range.getEndTimeSec()) + ")";

        item->setData(0, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(subtxt.c_str()));
    }

    if (range.isNewRange)
    {
        item->setData(0, TwoLineItemDelegate::headerTextRole, tr("New empty range..."));
        item->setData(0, TwoLineItemDelegate::subHeaderTextRole, tr("Click to edit"));
    }

    QVariant uservalue = QVariant::fromValue<TimeRange>(range);
    item->setData(0, TimeRangeRole, uservalue);

    item->setIcon(0, QIcon(":/img/icon_clock.png"));
}

void DialogEditTimeRange::clearRangeEdit()
{
    ui->comboTypeStart->setCurrentIndex(0);
    ui->timeEditFixStart->setTime(QTime());
    ui->timeEditFixEnd->setTime(QTime());
    ui->timeEditOffsetStart->setTime(QTime());
    ui->timeEditOffsetEnd->setTime(QTime());

    ui->checkBoxEndTime->setChecked(false);
    ui->comboTypeEnd->setCurrentIndex(0);
    ui->comboSunStart->setCurrentIndex(0);
    ui->comboSunEnd->setCurrentIndex(0);

    ui->checkBoxMonday->setChecked(false);
    ui->checkBoxTuesday->setChecked(false);
    ui->checkBoxWednesday->setChecked(false);
    ui->checkBoxThursday->setChecked(false);
    ui->checkBoxFriday->setChecked(false);
    ui->checkBoxSaturday->setChecked(false);
    ui->checkBoxSunday->setChecked(false);
}

void DialogEditTimeRange::on_checkBoxEndTime_stateChanged(int)
{
    ui->frameEnd->setEnabled(ui->checkBoxEndTime->isChecked());
}

void DialogEditTimeRange::on_comboTypeStart_currentIndexChanged(int index)
{
    if (index == 0)
        ui->stackedWidgetStart->setCurrentIndex(0);
    else
        ui->stackedWidgetStart->setCurrentIndex(1);
}

void DialogEditTimeRange::on_comboTypeEnd_currentIndexChanged(int index)
{
    if (index == 0)
        ui->stackedWidgetEnd->setCurrentIndex(0);
    else
        ui->stackedWidgetEnd->setCurrentIndex(1);
}

void DialogEditTimeRange::on_comboSunStart_currentIndexChanged(int index)
{
    if (index == 0)
        ui->timeEditOffsetStart->setEnabled(false);
    else
        ui->timeEditOffsetStart->setEnabled(true);
}

void DialogEditTimeRange::on_comboSunEnd_currentIndexChanged(int index)
{
    if (index == 0)
        ui->timeEditOffsetEnd->setEnabled(false);
    else
        ui->timeEditOffsetEnd->setEnabled(true);
}

void DialogEditTimeRange::on_addButton_clicked()
{
    TimeRange t;
    t.isNewRange = true;
    t.dayOfWeek.set();

    addTreeItem(t);
}

void DialogEditTimeRange::on_delButton_clicked()
{
    if (!ui->treeRanges->currentItem())
        return;

    QTreeWidgetItem *item = ui->treeRanges->currentItem();
    TimeRange range = item->data(0, TimeRangeRole).value<TimeRange>();

    deleteTimeRange(range);
    reloadTimeRanges();
}

void DialogEditTimeRange::on_treeRanges_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)
{
    if (!ui->treeRanges->currentItem())
        return;

    QTreeWidgetItem *item = ui->treeRanges->currentItem();
    TimeRange range = item->data(0, TimeRangeRole).value<TimeRange>();

    currentEdit = range;

    //load from timerange
    editTimeRange();
}

void DialogEditTimeRange::editTimeRange()
{
    clearRangeEdit();
    ui->groupBoxRange->setEnabled(true);

    auto setDay = [=](QCheckBox *chk, int active)
    {
        if (active == 1)
            chk->setChecked(true);
        else
            chk->setChecked(false);
    };

    setDay(ui->checkBoxMonday, currentEdit.dayOfWeek[0]);
    setDay(ui->checkBoxTuesday, currentEdit.dayOfWeek[1]);
    setDay(ui->checkBoxWednesday, currentEdit.dayOfWeek[2]);
    setDay(ui->checkBoxThursday, currentEdit.dayOfWeek[3]);
    setDay(ui->checkBoxFriday, currentEdit.dayOfWeek[4]);
    setDay(ui->checkBoxSaturday, currentEdit.dayOfWeek[5]);
    setDay(ui->checkBoxSunday, currentEdit.dayOfWeek[6]);

    bool same = currentEdit.isSameStartEnd();

    ui->comboTypeStart->setCurrentIndex(currentEdit.start_type);

    int h, m, s;
    Utils::from_string(currentEdit.shour, h);
    Utils::from_string(currentEdit.smin, m);
    Utils::from_string(currentEdit.ssec, s);

    if (currentEdit.start_type == 0)
        ui->timeEditFixStart->setTime(QTime(h, m, s));
    else
    {
        if (currentEdit.start_offset == -1)
            ui->comboSunStart->setCurrentIndex(2);
        else if (currentEdit.start_offset == 1)
            ui->comboSunStart->setCurrentIndex(1);
        else
            ui->comboSunStart->setCurrentIndex(0);

        if (currentEdit.start_offset != 0)
            ui->timeEditOffsetStart->setTime(QTime(h, m, s));
    }

    ui->checkBoxEndTime->setChecked(!same);
    if (!same)
    {
        ui->comboTypeEnd->setCurrentIndex(currentEdit.end_type);
        if (currentEdit.end_type == 0)
            ui->timeEditFixEnd->setTime(QTime(h, m, s));
        else
        {
            if (currentEdit.end_offset == -1)
                ui->comboSunEnd->setCurrentIndex(2);
            else if (currentEdit.end_offset == 1)
                ui->comboSunEnd->setCurrentIndex(1);
            else
                ui->comboSunEnd->setCurrentIndex(0);

            if (currentEdit.end_offset != 0)
                ui->timeEditOffsetEnd->setTime(QTime(h, m, s));
        }
    }

    //force UI update
    on_checkBoxEndTime_stateChanged(0);
    on_comboTypeStart_currentIndexChanged(ui->comboTypeStart->currentIndex());
    on_comboTypeEnd_currentIndexChanged(ui->comboTypeEnd->currentIndex());
    on_comboSunStart_currentIndexChanged(ui->comboSunStart->currentIndex());
    on_comboSunEnd_currentIndexChanged(ui->comboSunEnd->currentIndex());
}

void DialogEditTimeRange::deleteTimeRange(TimeRange &range)
{
    auto delRange = [=](vector<TimeRange> &vrange)
    {
        auto it = std::find(vrange.begin(), vrange.end(), range);
        if (it != vrange.end())
            vrange.erase(it);
    };

    delRange(input->range_monday);
    delRange(input->range_tuesday);
    delRange(input->range_wednesday);
    delRange(input->range_thursday);
    delRange(input->range_friday);
    delRange(input->range_saturday);
    delRange(input->range_sunday);
}

void DialogEditTimeRange::on_pushButtonApply_clicked()
{
    deleteTimeRange(currentEdit);

    currentEdit = TimeRange(); //clear

    currentEdit.start_type = ui->comboTypeStart->currentIndex();
    if (currentEdit.start_type == 0)
    {
        currentEdit.shour = Utils::to_string(ui->timeEditFixStart->time().hour());
        currentEdit.smin = Utils::to_string(ui->timeEditFixStart->time().minute());
        currentEdit.ssec = Utils::to_string(ui->timeEditFixStart->time().second());
    }
    else
    {
        if (ui->comboSunStart->currentIndex() != 0)
        {
            currentEdit.shour = Utils::to_string(ui->timeEditOffsetStart->time().hour());
            currentEdit.smin = Utils::to_string(ui->timeEditOffsetStart->time().minute());
            currentEdit.ssec = Utils::to_string(ui->timeEditOffsetStart->time().second());
        }

        if (ui->comboSunStart->currentIndex() == 0) currentEdit.start_offset = 0;
        if (ui->comboSunStart->currentIndex() == 1) currentEdit.start_offset = 1;
        if (ui->comboSunStart->currentIndex() == 2) currentEdit.start_offset = -1;
    }

    if (ui->checkBoxEndTime->isChecked())
    {
        currentEdit.end_type = ui->comboTypeEnd->currentIndex();
        if (currentEdit.end_type == 0)
        {
            currentEdit.ehour = Utils::to_string(ui->timeEditFixEnd->time().hour());
            currentEdit.emin = Utils::to_string(ui->timeEditFixEnd->time().minute());
            currentEdit.esec = Utils::to_string(ui->timeEditFixEnd->time().second());
        }
        else
        {
            if (ui->comboSunEnd->currentIndex() != 0)
            {
                currentEdit.ehour = Utils::to_string(ui->timeEditOffsetEnd->time().hour());
                currentEdit.emin = Utils::to_string(ui->timeEditOffsetEnd->time().minute());
                currentEdit.esec = Utils::to_string(ui->timeEditOffsetEnd->time().second());
            }

            if (ui->comboSunEnd->currentIndex() == 0) currentEdit.end_offset = 0;
            if (ui->comboSunEnd->currentIndex() == 1) currentEdit.end_offset = 1;
            if (ui->comboSunEnd->currentIndex() == 2) currentEdit.end_offset = -1;
        }
    }
    else
    {
        currentEdit.end_type = currentEdit.start_type;
        currentEdit.end_offset = currentEdit.start_offset;
        currentEdit.ehour = currentEdit.shour;
        currentEdit.emin = currentEdit.smin;
        currentEdit.esec = currentEdit.ssec;
    }

    if (ui->checkBoxMonday->isChecked())
        input->range_monday.push_back(currentEdit);
    if (ui->checkBoxTuesday->isChecked())
        input->range_tuesday.push_back(currentEdit);
    if (ui->checkBoxWednesday->isChecked())
        input->range_wednesday.push_back(currentEdit);
    if (ui->checkBoxThursday->isChecked())
        input->range_thursday.push_back(currentEdit);
    if (ui->checkBoxFriday->isChecked())
        input->range_friday.push_back(currentEdit);
    if (ui->checkBoxSaturday->isChecked())
        input->range_saturday.push_back(currentEdit);
    if (ui->checkBoxSunday->isChecked())
        input->range_sunday.push_back(currentEdit);

    reloadTimeRanges();
}
