#ifndef DIALOGNEWGPIOINPUT_H
#define DIALOGNEWGPIOINPUT_H

#include <QDialog>

namespace Ui {
class DialogNewGpioInput;
}

class DialogNewGpioInput : public QDialog
{
        Q_OBJECT

public:
        explicit DialogNewGpioInput(QWidget *parent = 0);
        ~DialogNewGpioInput();

private:
        Ui::DialogNewGpioInput *ui;
};

#endif // DIALOGNEWGPIOINPUT_H
