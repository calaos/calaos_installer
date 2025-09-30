#ifndef DIALOGREMOTEUI_H
#define DIALOGREMOTEUI_H

#include <QDialog>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
class DialogRemoteUI;
}

class DialogRemoteUI : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRemoteUI(Calaos::Room *r, QWidget *parent = nullptr);
    ~DialogRemoteUI();

    IOBase *getOutput() { return output; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogRemoteUI *ui;

    IOBase *output;
    Room *room;
};

#endif // DIALOGREMOTEUI_H
