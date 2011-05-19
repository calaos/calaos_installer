#ifndef FORMCONDITIONSTD_H
#define FORMCONDITIONSTD_H

#include <QtGui>
#include <ListeRule.h>
#include <ListeRoom.h>
#include <dialogiolist.h>

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

                bool setDone;


        private slots:
                void on_comboValue_currentIndexChanged(int index);
                void on_comboValue_editTextChanged(QString );
                void on_comboOp_currentIndexChanged(int index);
                void on_btMore_clicked();
};

#endif // FORMCONDITIONSTD_H
