#ifndef DIALOGNEWMYSENSORS_H
#define DIALOGNEWMYSENSORS_H

#include <QtWidgets>
#include <QDialog>

#include "formanalogproperties.h"

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui {
class DialogNewMySensors;
}

class DialogNewMySensors : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewMySensors(Room *room, int it, QWidget *parent = 0);
    ~DialogNewMySensors();

    IOBase *getOutput() { return io; }
    IOBase *getInput() { return io; }

private slots:
    void on_buttonBox_accepted();

    void on_check_smart_stateChanged(int);

    void on_comboBoxType_currentIndexChanged(int index);

private:
    Ui::DialogNewMySensors *ui;

    IOBase *io;
    Room *room;
    int itemType;
    FormAnalogProperties *analogWidget;
};

#endif // DIALOGNEWMYSENSORS_H
