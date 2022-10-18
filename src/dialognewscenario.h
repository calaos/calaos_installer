#ifndef DIALOGNEWSCENARIO_H
#define DIALOGNEWSCENARIO_H

#include <QDialog>
#include <iostream>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
class DialogNewScenario;
}

class DialogNewScenario : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogNewScenario)
public:
    explicit DialogNewScenario(Room *room, QWidget *parent = 0);
    virtual ~DialogNewScenario();

    IOBase *getOutput() { return output; }

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogNewScenario *ui;
    IOBase *output;
    Room *room;

private slots:
    void on_buttonBox_accepted();
};

#endif // DIALOGNEWSCENARIO_H
