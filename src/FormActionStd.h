#ifndef FORMACTIONSTD_H
#define FORMACTIONSTD_H

#include <QtGui>
#include <ListeRule.h>
#include <ListeRoom.h>
#include <dialogiolist.h>

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

                bool setDone;


        private slots:
                void on_comboValue_currentIndexChanged(int index);
                void on_comboValue_editTextChanged(QString );
                void on_btMore_clicked();
};

#endif // FORMACTIONSTD_H
