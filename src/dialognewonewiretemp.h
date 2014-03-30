#ifndef DIALOGNEWONEWIRETEMP_H
#define DIALOGNEWONEWIRETEMP_H

#include <QDialog>

namespace Ui {
class DialogNewOneWireTemp;
}

class DialogNewOneWireTemp : public QDialog
{
        Q_OBJECT

public:
        explicit DialogNewOneWireTemp(QWidget *parent = 0);
        ~DialogNewOneWireTemp();

private:
        Ui::DialogNewOneWireTemp *ui;
};

#endif // DIALOGNEWONEWIRETEMP_H
