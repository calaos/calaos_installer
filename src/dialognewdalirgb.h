#ifndef DIALOGNEWDALIRGB_H
#define DIALOGNEWDALIRGB_H

#include <QDialog>
#include <wagoconnect.h>
#include <iostream>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
class DialogNewDaliRGB;
}

class DialogNewDaliRGB : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogNewDaliRGB)
public:
    explicit DialogNewDaliRGB(Room *room, QWidget *parent = 0);
    virtual ~DialogNewDaliRGB();

    IOBase *getOutput() { return dali; }

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogNewDaliRGB *ui;
    IOBase *dali;
    Room *room;

    void setWagoDali(int address, bool group, bool enable);

private slots:
    void on_pushButton_blue_off_clicked();
    void on_pushButton_blue_on_clicked();
    void on_pushButton_greenoff_clicked();
    void on_pushButton_green_on_clicked();
    void on_pushButton_red_off_clicked();
    void on_pushButton_red_on_clicked();
    void on_buttonBox_accepted();
};

#endif // DIALOGNEWDALIRGB_H
