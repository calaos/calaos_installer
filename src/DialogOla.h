#ifndef DIALOGOLA_H
#define DIALOGOLA_H

#include <QtWidgets>
#include <QDialog>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui {
class DialogOla;
}

class DialogOla : public QDialog
{
    Q_OBJECT

public:
    DialogOla(Room *r, int it, QWidget *parent = 0);
    ~DialogOla();

    IOBase *getOutput() { return io; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogOla *ui;

    IOBase *io;
    Room *room;
    int itemType;
};

#endif // DIALOGOLA_H
