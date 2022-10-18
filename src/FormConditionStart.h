#ifndef FORMCONDITIONSTART_H
#define FORMCONDITIONSTART_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <ListeRule.h>

using namespace Calaos;

namespace Ui
{
class FormConditionStart;
}

class FormConditionStart : public QWidget
{
    Q_OBJECT

public:
    explicit FormConditionStart(QWidget *parent = 0);
    ~FormConditionStart();

    void setCondition(QTreeWidgetItem *item, Rule *rule, Condition *condition);

private:
    Ui::FormConditionStart *ui;

protected:
    void changeEvent(QEvent *e);

    Rule *rule;
    Condition *condition;
    QTreeWidgetItem *qitem;
};

#endif // FORMCONDITIONSTART_H
