#ifndef FORMCONDITIONSCRIPT_H
#define FORMCONDITIONSCRIPT_H

#include <QtWidgets>
#include <ListeRule.h>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui
{
        class FormConditionScript;
}

class FormConditionScript : public QWidget
{
        Q_OBJECT

        public:
                explicit FormConditionScript(QWidget *parent = 0);
                ~FormConditionScript();

                void setCondition(QTreeWidgetItem *item, Rule *rule, Condition *condition);

        private:
                Ui::FormConditionScript *ui;

        protected:
                void changeEvent(QEvent *e);

                Rule *rule;
                Condition *condition;
                QTreeWidgetItem *qitem;

        private slots:
                void on_btEditScript_clicked();
                void on_btEditInput_clicked();
};

#endif // FORMCONDITIONSCRIPT_H
