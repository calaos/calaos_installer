#ifndef DIALOGNEWWEBOUTPUTSTRING_H
#define DIALOGNEWWEBOUTPUTSTRING_H


#include <QtWidgets>
#include <QDialog>
#include <iostream>

#include <ListeRoom.h>

using namespace std;
using namespace Calaos;

namespace Ui {
class DialogNewWebOutputString;
}

class DialogNewWebOutputString : public QDialog
{
        Q_OBJECT
        Q_DISABLE_COPY(DialogNewWebOutputString)
public:
        explicit DialogNewWebOutputString(Room *r, QWidget *parent = 0);
        ~DialogNewWebOutputString();
        Output *getOutput() { return output; }

private:
        Ui::DialogNewWebOutputString *ui;
        Room *room;
        Output *output;

private slots:
        void on_buttonBox_accepted();
};

#endif // DIALOGNEWWEBOUTPUTSTRING_H
