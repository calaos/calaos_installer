#ifndef DIALOGEDITTIMERANGE_H
#define DIALOGEDITTIMERANGE_H

#include <QDialog>

namespace Ui {
class DialogEditTimeRange;
}

class DialogEditTimeRange : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditTimeRange(QWidget *parent = 0);
    ~DialogEditTimeRange();

private:
    Ui::DialogEditTimeRange *ui;
};

#endif // DIALOGEDITTIMERANGE_H
