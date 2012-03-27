#ifndef DIALOGIOLIST_H
#define DIALOGIOLIST_H

#include <QDialog>
#include <IOBase.h>
#include <ListeRoom.h>
#include <qtreewidget_addition.h>

using namespace Calaos;

namespace Ui
{
        class DialogIOList;
}

class DialogIOList : public QDialog
{
        Q_OBJECT

        public:
                DialogIOList(Input *input = NULL, Output *output = NULL, QWidget *parent = NULL);
                ~DialogIOList();

                Input *getInput();
                Output *getOutput();

        protected:
                void changeEvent(QEvent *e);

        private slots:
                void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

        private:
                Ui::DialogIOList *ui;

                Input *input;
                Output *output;

                QTreeWidgetItem *item_current;
};

#endif // DIALOGIOLIST_H
