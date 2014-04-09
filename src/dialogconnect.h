#ifndef DIALOGCONNECT_H
#define DIALOGCONNECT_H

#include <QObject>
#include <wagoconnect.h>
#include <QDialog>

namespace Ui
{
class DialogConnect;
}

class DialogConnect : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogConnect)
public:
    explicit DialogConnect(QWidget *parent = 0);
    virtual ~DialogConnect();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogConnect *ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // DIALOGCONNECT_H
