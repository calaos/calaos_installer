#ifndef DIALOGNEWINTERNAL_H
#define DIALOGNEWINTERNAL_H

#include <QtWidgets>
#include <QDialog>
#include <iostream>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
class DialogNewInternal;
}

class DialogNewInternal : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogNewInternal)
public:
    explicit DialogNewInternal(Room *room, QWidget *parent = 0);
    virtual ~DialogNewInternal();

    IOBase *getOutput() { return output; }

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogNewInternal *ui;
    IOBase *output;
    Room *room;

private slots:
    void on_radioButton_text_clicked();
    void on_radioButton_int_clicked();
    void on_radioButton_bool_clicked();
    void on_buttonBox_accepted();
};

#endif // DIALOGNEWAUDIO_H
