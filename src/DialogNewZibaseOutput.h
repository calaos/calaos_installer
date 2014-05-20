#ifndef DIALOGNEWZIBASEOUTPUT_H
#define DIALOGNEWZIBASEOUTPUT_H

#include <QtWidgets>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
class DialogNewZibaseOutput;
}

class DialogNewZibaseOutput : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewZibaseOutput(Room *r, int item, QWidget *parent = 0);
    ~DialogNewZibaseOutput();

    IOBase *getOutput() { return io; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogNewZibaseOutput *ui;
    IOBase *io = nullptr;
    Room *room;
    int item;
};

#endif // DIALOGNEWZIBASEOUTPUT_H
