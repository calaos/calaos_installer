#ifndef DIALOGNEWDALI_H
#define DIALOGNEWDALI_H

#include <QtWidgets>
#include <QDialog>
#include <wagoconnect.h>
#include <iostream>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
class DialogNewDali;
}

class DialogNewDali : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogNewDali)
public:
    explicit DialogNewDali(Room *room, QWidget *parent = 0);
    virtual ~DialogNewDali();

    IOBase *getOutput() { return dali; }

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogNewDali *ui;
    IOBase *dali;
    Room *room;

    void setWagoDali(bool enable);

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_buttonBox_accepted();
};

#endif // DIALOGNEWDALI_H
