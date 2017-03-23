#ifndef FORMACTIONTOUCHSCREEN_H
#define FORMACTIONTOUCHSCREEN_H

#include <QtWidgets>
#include <ListeRule.h>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui
{
class FormActionTouchscreen;
}

class FormActionTouchscreen : public QWidget
{
    Q_OBJECT

public:
    explicit FormActionTouchscreen(QWidget *parent = 0);
    ~FormActionTouchscreen();

    void setAction(QTreeWidgetItem *item, Rule *rule, Action *action);

private:
    Ui::FormActionTouchscreen *ui;

protected:
    void changeEvent(QEvent *e);

    Rule *rule;
    Action *action;
    QTreeWidgetItem *qitem;

    bool setDone;

    void updateAction();

private slots:
    void on_comboBoxAction_currentIndexChanged(int);
    void on_comboBoxCamera_currentIndexChanged(int);
};

#endif // FORMACTIONTOUCHSCREEN_H
