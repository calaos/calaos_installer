#ifndef DIALOGIOLIST_H
#define DIALOGIOLIST_H

#include <QDialog>
#include <IOBase.h>
#include <ListeRoom.h>

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

        protected:
                void changeEvent(QEvent *e);

        private:
                Ui::DialogIOList *ui;

                Input *input;
                Output *output;
};

#endif // DIALOGIOLIST_H
