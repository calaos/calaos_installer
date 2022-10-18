#ifndef DIALOGEDITTIMERANGE_H
#define DIALOGEDITTIMERANGE_H

#include <QDialog>
#include "IOBase.h"
#include <QTreeWidgetItem>
#include <QCheckBox>

using namespace Calaos;

namespace Ui {
class DialogEditTimeRange;
}

class DialogEditTimeRange : public QDialog
{
    Q_OBJECT

public:
    DialogEditTimeRange(IOBase *input);
    ~DialogEditTimeRange();

    void save();

private slots:
    void on_checkBoxEndTime_stateChanged(int);

    void on_comboTypeStart_currentIndexChanged(int);

    void on_comboTypeEnd_currentIndexChanged(int index);

    void on_addButton_clicked();

    void on_delButton_clicked();

    void on_treeRanges_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);

    void on_pushButtonApply_clicked();

    void on_comboSunStart_currentIndexChanged(int index);

    void on_comboSunEnd_currentIndexChanged(int index);

private:
    Ui::DialogEditTimeRange *ui;

    enum {
        TimeRangeRole = Qt::UserRole + 1
    };

    IOBase *input = nullptr;
    IOBase *inTimeRange = nullptr;

    vector<QCheckBox *> items_months;

    TimeRange currentEdit;

    void reloadTimeRanges();
    void clearRangeEdit();
    void deleteTimeRange(TimeRange &range);
    void editTimeRange();
    void addTreeItem(TimeRange &range);
};

#endif // DIALOGEDITTIMERANGE_H
