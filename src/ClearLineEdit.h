#ifndef CLEARLINEEDIT_H
#define CLEARLINEEDIT_H

#include <QLineEdit>
#include <QToolButton>
#include <QLabel>

class ClearLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit ClearLineEdit(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void updateCloseButton(const QString &text);

private:
    QToolButton *clearButton;
    QLabel *label;
};

#endif // CLEARLINEEDIT_H
