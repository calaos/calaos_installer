#ifndef FORMACTIONMAIL_H
#define FORMACTIONMAIL_H

#include <QtGui>
#include <ListeRule.h>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui
{
        class FormActionMail;
}

class FormActionMail : public QWidget
{
        Q_OBJECT

        public:
                explicit FormActionMail(QWidget *parent = 0);
                ~FormActionMail();

                void setAction(QTreeWidgetItem *item, Rule *rule, Action *action);

        private:
                Ui::FormActionMail *ui;

        protected:
                void changeEvent(QEvent *e);

                Rule *rule;
                Action *action;
                QTreeWidgetItem *qitem;

                bool setDone;

        private slots:
                void on_btEditMsg_clicked();
                void on_comboCamera_currentIndexChanged(int index);
                void on_editSender_textChanged(QString );
                void on_editRecipient_textChanged(QString );
                void on_editSubject_textChanged(QString );
};

#endif // FORMACTIONMAIL_H
