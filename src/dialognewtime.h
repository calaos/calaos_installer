#ifndef DIALOGNEWTIME_H
#define DIALOGNEWTIME_H

#include <QDialog>
#include <iostream>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
class DialogNewTime;
}

class DialogNewTime : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogNewTime)
public:
    explicit DialogNewTime(Room *room, QWidget *parent = 0);
    virtual ~DialogNewTime();

    IOBase *getInput() { return input; }

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogNewTime *ui;
    IOBase *input;
    Room *room;

private slots:
    void on_check_datetime_clicked();
    void on_check_time_clicked();
    void on_buttonBox_accepted();
};

#endif // DIALOGNEWTIME_H
