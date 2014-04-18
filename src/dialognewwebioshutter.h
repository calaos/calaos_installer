#ifndef DIALOGNEWWEBIOSHUTTER_H
#define DIALOGNEWWEBIOSHUTTER_H

#include <QDialog>

#include <QtWidgets>
#include <QDialog>
#include <iostream>

#include <ListeRoom.h>


using namespace std;
using namespace Calaos;

namespace Ui {
class DialogNewWebIOShutter;
}

class DialogNewWebIOShutter : public QDialog
{
        Q_OBJECT
        Q_DISABLE_COPY(DialogNewWebIOShutter)

public:
        explicit DialogNewWebIOShutter(Room *r, QWidget *parent = 0);
        ~DialogNewWebIOShutter();
        IOBase *getOutput() { return io; }

private:
        Ui::DialogNewWebIOShutter *ui;
        IOBase *io;
        Room *room;

private slots:
        void on_buttonBox_accepted();
};

#endif // DIALOGNEWWEBIOSHUTTER_H
