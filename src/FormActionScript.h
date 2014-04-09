#ifndef FORMACTIONSCRIPT_H
#define FORMACTIONSCRIPT_H

#include <QtWidgets>
#include <ListeRule.h>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui
{
class FormActionScript;
}

class FormActionScript : public QWidget
{
    Q_OBJECT

public:
    explicit FormActionScript(QWidget *parent = 0);
    ~FormActionScript();

    void setAction(QTreeWidgetItem *item, Rule *rule, Action *action);

private:
    Ui::FormActionScript *ui;

protected:
    void changeEvent(QEvent *e);

    Rule *rule;
    Action *action;
    QTreeWidgetItem *qitem;

private slots:
    void on_btEditScript_clicked();
};

#endif // FORMACTIONSCRIPT_H
