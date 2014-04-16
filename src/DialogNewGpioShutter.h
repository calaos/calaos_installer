#ifndef DIALOGNEWGPIOSHUTTER_H
#define DIALOGNEWGPIOSHUTTER_H

#include <QtWidgets>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
class DialogNewGpioShutter;
}

class DialogNewGpioShutter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewGpioShutter(Room *room, QWidget *parent = 0);
    ~DialogNewGpioShutter();

    IOBase *getOutput() { return io; }

private slots:
    void on_check_smart_stateChanged(int arg1);
    void on_buttonBox_accepted();

private:
    Ui::DialogNewGpioShutter *ui;
    IOBase *io = nullptr;
    Room *room;
};

#endif // DIALOGNEWGPIOSHUTTER_H
