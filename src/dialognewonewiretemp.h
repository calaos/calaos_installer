#ifndef DIALOGNEWONEWIRETEMP_H
#define DIALOGNEWONEWIRETEMP_H

#include <QtWidgets>
#include <QDialog>
#include <iostream>

#include <ListeRoom.h>

using namespace std;
using namespace Calaos;

namespace Ui {
class DialogNewOneWireTemp;
}

class DialogNewOneWireTemp : public QDialog
{
        Q_OBJECT
        Q_DISABLE_COPY(DialogNewOneWireTemp)
public:
        explicit DialogNewOneWireTemp(Room *r, QWidget *parent = 0);
        virtual ~DialogNewOneWireTemp();

        IOBase *getInput() { return temperature; }

protected:
        virtual void changeEvent(QEvent *e);

private:
        Ui::DialogNewOneWireTemp *ui;
        IOBase *temperature;
        Room *room;

private slots:
        void on_buttonBox_accepted();
};

#endif // DIALOGNEWONEWIRETEMP_H
