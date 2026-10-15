#ifndef FORMACTIONSTD_H
#define FORMACTIONSTD_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <ListeRule.h>
#include <ListeRoom.h>
#include <dialogiolist.h>

#include "RuleActionMenu.h"

class DeviceCommandForm;
class QPushButton;

using namespace Calaos;

namespace Ui
{
class FormActionStd;
}

class FormActionStd : public QWidget
{
    Q_OBJECT

public:
    explicit FormActionStd(QWidget *parent = 0);
    ~FormActionStd();

    void setAction(QTreeWidgetItem *item, Rule *rule, Action *action);

    // The action takes the value of another IO instead of its own text.
    void setDynamicValue(const std::string &ioId);

signals:
    // The value of the action was edited: the rule tree has to redraw its line.
    void actionChanged(QTreeWidgetItem *item, Calaos::Action *action);

private:
    Ui::FormActionStd *ui;

protected:
    void changeEvent(QEvent *e);

    Rule *rule;
    Action *action;
    QTreeWidgetItem *qitem;

    bool onStart;
    QMenu *actionMenu;

    RuleActionMenu *addActionMenu(QString action, QString help, QString cmd);

    DeviceCommandForm *cmdForm;
    QPushButton *btMode;
    bool jsonDevice = false;

    void storeValue(const QString &value);
    void showCommandForm(bool form);
    void updateModeButton();


private slots:

    void menuAction(RuleActionMenu *);

    void on_btMore_clicked();
    void on_buttonMore_clicked();
    void on_editValue_textChanged(const QString &arg1);
    void onFormChanged(const QString &value);
    void onModeButtonClicked();
};

#endif // FORMACTIONSTD_H
