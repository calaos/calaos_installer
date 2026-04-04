#ifndef DIALOGNEWREMOTEUIRELAY_H
#define DIALOGNEWREMOTEUIRELAY_H

#include <QDialog>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
class DialogNewRemoteUIRelay;
}

class DialogNewRemoteUIRelay : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewRemoteUIRelay(Calaos::Room *r, QWidget *parent = nullptr);
    ~DialogNewRemoteUIRelay();

    IOBase *getOutput() { return output; }

private slots:
    void on_buttonBox_accepted();

private:
    void populateRemoteUICombo();

    Ui::DialogNewRemoteUIRelay *ui;
    IOBase *output = nullptr;
    Room *room;
};

#endif // DIALOGNEWREMOTEUIRELAY_H
