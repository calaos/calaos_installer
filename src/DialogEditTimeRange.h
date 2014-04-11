#ifndef DIALOGEDITTIMERANGE_H
#define DIALOGEDITTIMERANGE_H

#include <QDialog>
#include "IOBase.h"

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

private:
    Ui::DialogEditTimeRange *ui;

    IOBase *input = nullptr;
    IOBase *inTimeRange = nullptr;
};

#endif // DIALOGEDITTIMERANGE_H
