#ifndef DIALOGNEWWOL_H
#define DIALOGNEWWOL_H

#include <QtWidgets>
#include <QDialog>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui {
class DialogNewWOL;
}

class DialogNewWOL : public QDialog
{
    Q_OBJECT

public:
    DialogNewWOL(Room *r, int it, QWidget *parent = 0);
    ~DialogNewWOL();

    IOBase *getOutput() { return io; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogNewWOL *ui;

    IOBase *io;
    Room *room;
    int itemType;
};

#endif // DIALOGNEWWOL_H
