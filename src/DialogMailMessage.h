#ifndef DIALOGMAILMESSAGE_H
#define DIALOGMAILMESSAGE_H

#include <QDialog>

namespace Ui
{
class DialogMailMessage;
}

class DialogMailMessage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMailMessage(QString msg, QWidget *parent = 0);
    ~DialogMailMessage();

    QString getMessage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogMailMessage *ui;
};

#endif // DIALOGMAILMESSAGE_H
