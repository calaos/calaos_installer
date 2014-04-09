#ifndef DIALOGNEWAUDIO_H
#define DIALOGNEWAUDIO_H

#include <QtWidgets>
#include <QDialog>
#include <iostream>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
class DialogNewAudio;
}

class DialogNewAudio : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogNewAudio)
public:
    explicit DialogNewAudio(Room *room, QWidget *parent = 0);
    virtual ~DialogNewAudio();

    IOBase *getOutput() { return output; }

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogNewAudio *ui;
    IOBase *output;
    Room *room;

private slots:
    void on_pushButton_clicked();
    void on_buttonBox_accepted();
};

#endif // DIALOGNEWAUDIO_H
