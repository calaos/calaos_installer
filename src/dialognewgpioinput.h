#ifndef DIALOGNEWGPIOINPUT_H
#define DIALOGNEWGPIOINPUT_H

#include <QtWidgets>
#include <QDialog>
#include <iostream>

#include <ListeRoom.h>

using namespace std;
using namespace Calaos;

namespace Ui {
class DialogNewGpioInput;
}

class DialogNewGpioInput : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogNewGpioInput)

public:
    explicit DialogNewGpioInput(Room *r, QWidget *parent = 0);
    virtual ~DialogNewGpioInput();

    IOBase *getInput() { return input; }

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogNewGpioInput *ui;
    IOBase *input;
    Room *room;

private slots:
    void on_buttonBox_accepted();
};

#endif // DIALOGNEWGPIOINPUT_H
