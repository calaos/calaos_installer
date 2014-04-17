#ifndef DIALOGNEWX10_H
#define DIALOGNEWX10_H

#include <QtWidgets>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
class DialogNewX10;
}

class DialogNewX10 : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewX10(Room *r, QWidget *parent = 0);
    ~DialogNewX10();

    IOBase *getOutput() { return io; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogNewX10 *ui;
    IOBase *io = nullptr;
    Room *room;
};

#endif // DIALOGNEWX10_H
