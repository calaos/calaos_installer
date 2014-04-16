#ifndef DialogNewWebIO_H
#define DialogNewWebIO_H

#include <QDialog>

#include <QtWidgets>
#include <QDialog>
#include <iostream>

#include <ListeRoom.h>

using namespace std;
using namespace Calaos;

namespace Ui {
class DialogNewWebIO;
}

class DialogNewWebIO : public QDialog
{
        Q_OBJECT
        Q_DISABLE_COPY(DialogNewWebIO)
public:
        explicit DialogNewWebIO(Room *r, int item,  QWidget *parent = 0);
        virtual ~DialogNewWebIO();

        bool isInputType()
        {
                if (type.find("Input") != string::npos)
                        return true; return false;
        }
        IOBase *getInput() { return io; }
        IOBase *getOutput() { return io; }

protected:
        virtual void changeEvent(QEvent *e);

private:
        Ui::DialogNewWebIO *ui;
        IOBase *io;
        Room *room;
        string type;
        int item;

private slots:
        void on_buttonBox_accepted();
};

#endif // DialogNewWebIO_H
