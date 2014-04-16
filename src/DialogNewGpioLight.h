#ifndef DIALOGNEWGPIOLIGHT_H
#define DIALOGNEWGPIOLIGHT_H

#include <QtWidgets>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
class DialogNewGpioLight;
}

class DialogNewGpioLight : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewGpioLight(Room *room, QWidget *parent = 0);
    ~DialogNewGpioLight();

    IOBase *getOutput() { return io; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogNewGpioLight *ui;
    IOBase *io = nullptr;
    Room *room;
};

#endif // DIALOGNEWGPIOLIGHT_H
