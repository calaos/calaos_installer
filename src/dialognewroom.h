#ifndef DIALOGNEWROOM_H
#define DIALOGNEWROOM_H

#include <iostream>
#include <QDialog>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
class DialogNewRoom;
}

class DialogNewRoom : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogNewRoom)
public:
    explicit DialogNewRoom(QWidget *parent = 0);
    virtual ~DialogNewRoom();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogNewRoom *m_ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // DIALOGNEWROOM_H
