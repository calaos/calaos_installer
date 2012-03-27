#ifndef FORMCONDITIONSTD_H
#define FORMCONDITIONSTD_H

#include <QtGui>
#include <ListeRule.h>
#include <ListeRoom.h>
#include <dialogiolist.h>

#include "RuleActionMenu.h"

using namespace Calaos;

namespace Ui
{
        class FormConditionStd;
}

class FormConditionStd : public QWidget
{
        Q_OBJECT

        public:
                explicit FormConditionStd(QWidget *parent = 0);
                ~FormConditionStd();

                void setCondition(QTreeWidgetItem *item, Rule *rule, Condition *condition);

        private:
                Ui::FormConditionStd *ui;

                string getRoomIcon(string room_type);

        protected:
                void changeEvent(QEvent *e);

                Rule *rule;
                Condition *condition;
                QTreeWidgetItem *qitem;

                bool onStart;

                QMenu *actionMenu;

                void addActionMenu(QString action, QString help, QString cmd);

        private slots:
                void menuAction(RuleActionMenu *);

                void on_btMore_clicked();
                void on_editValue_textChanged(const QString &arg1);
                void on_buttonMore_clicked();
                void on_comboOp_currentIndexChanged(int);
};

#endif // FORMCONDITIONSTD_H
