#ifndef DIALOGNEWAVRECEIVER_H
#define DIALOGNEWAVRECEIVER_H

#include <QtWidgets>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
class DialogNewAVReceiver;
}

class DialogNewAVReceiver : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewAVReceiver(Calaos::Room *r, QWidget *parent = 0);
    ~DialogNewAVReceiver();

    IOBase *getOutput() { return output; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogNewAVReceiver *ui;

    IOBase *output;
    Room *room;
};

#endif // DIALOGNEWAVRECEIVER_H
