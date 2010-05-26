#ifndef DIALOGPLAGEINPUT_H
#define DIALOGPLAGEINPUT_H

#include <QtGui/QDialog>

namespace Ui
{
    class DialogPlageInput;
}

class DialogPlageInput : public QDialog
{
        Q_OBJECT
        Q_DISABLE_COPY(DialogPlageInput)

        public:
                explicit DialogPlageInput(QWidget *parent = 0);
                ~DialogPlageInput();

                QTime getTimeBegin();
                QTime getTimeEnd();

                void setTimeBegin(QTime t);
                void setTimeEnd(QTime t);

        protected:
                void changeEvent(QEvent *e);

        private:
                Ui::DialogPlageInput *ui;
};

#endif // DIALOGPLAGEINPUT_H
