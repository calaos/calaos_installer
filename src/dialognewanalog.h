#ifndef DIALOGNEWANALOG_H
#define DIALOGNEWANALOG_H

#include <QDialog>
#include <iostream>
#include <ListeRoom.h>

#include "formanalogproperties.h"

using namespace std;
using namespace Calaos;

namespace Ui
{
class DialogNewAnalog;
}

class DialogNewAnalog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogNewAnalog)
public:
    explicit DialogNewAnalog(Room *room, QWidget *parent = 0);
    virtual ~DialogNewAnalog();

    IOBase *getInput() { return io->is_input()?io:nullptr; }
    IOBase *getOutput() { return io->is_output()?io:nullptr; }

    bool isInputType() { if (type == "WIAnalog") return true; return false; }

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogNewAnalog *ui;
    string type;
    IOBase *io;
    Room *room;
    FormAnalogProperties *analogWidget;

private slots:
    void on_buttonBox_accepted();
    void on_radio_input_toggled(bool checked);
};

#endif
