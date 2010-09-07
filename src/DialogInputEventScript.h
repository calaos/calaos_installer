#ifndef DIALOGINPUTEVENTSCRIPT_H
#define DIALOGINPUTEVENTSCRIPT_H

#include <QDialog>

#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
        class DialogInputEventScript;
}

class DialogInputEventScript : public QDialog
{
        Q_OBJECT

        public:
                explicit DialogInputEventScript(Condition *condition, QWidget *parent = 0);
                ~DialogInputEventScript();

        protected:
                void changeEvent(QEvent *e);

        private:
                Ui::DialogInputEventScript *ui;

                Condition *condition;

        private slots:
                void on_btRemove_clicked();
                void on_btAdd_clicked();
                void on_buttonBox_accepted();
};

#endif // DIALOGINPUTEVENTSCRIPT_H
