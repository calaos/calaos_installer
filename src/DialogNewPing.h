#ifndef DIALOGNEWPING_H
#define DIALOGNEWPING_H

#include <QtWidgets>
#include <QDialog>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui {
class DialogNewPing;
}

class DialogNewPing : public QDialog
{
    Q_OBJECT

public:
    DialogNewPing(Room *r, int it, QWidget *parent = 0);
    ~DialogNewPing();

    IOBase *getInput() { return io; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogNewPing *ui;

    IOBase *io;
    Room *room;
    int itemType;
};

#endif // DIALOGNEWPING_H
