#ifndef FORMACTIONTOUCHSCREEN_H
#define FORMACTIONTOUCHSCREEN_H

#include <QtGui>
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

        private slots:
                void on_editAction_textChanged(QString );
};

#endif // FORMACTIONTOUCHSCREEN_H
