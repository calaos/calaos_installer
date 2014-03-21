#ifndef FORMACTIONSTD_H
#define FORMACTIONSTD_H

#include <QtWidgets>
#include <ListeRule.h>
#include <ListeRoom.h>
#include <dialogiolist.h>

#include "RuleActionMenu.h"

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

        private:
                Ui::FormActionStd *ui;

        protected:
                void changeEvent(QEvent *e);

                Rule *rule;
                Action *action;
                QTreeWidgetItem *qitem;

                bool onStart;
                QMenu *actionMenu;

                void addActionMenu(QString action, QString help, QString cmd);


        private slots:

                void menuAction(RuleActionMenu *);

                void on_btMore_clicked();
                void on_buttonMore_clicked();
                void on_editValue_textChanged(const QString &arg1);
};

#endif // FORMACTIONSTD_H
