#ifndef DIALOGNEWZIBASE_H
#define DIALOGNEWZIBASE_H

#include <QtWidgets>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
class DialogNewZibase;
}

class DialogNewZibase : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewZibase(Room *r, int item, QWidget *parent = 0);
    ~DialogNewZibase();

    IOBase *getInput() { return io; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogNewZibase *ui;
    IOBase *io = nullptr;
    Room *room;
    int item;
};

#endif // DIALOGNEWZIBASE_H
