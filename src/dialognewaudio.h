#ifndef DIALOGNEWAUDIO_H
#define DIALOGNEWAUDIO_H

#include <QtGui/QDialog>
#include <iostream>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
        class DialogNewAudio;
}

class DialogNewAudio : public QDialog
{
                Q_OBJECT
                Q_DISABLE_COPY(DialogNewAudio)
        public:
                explicit DialogNewAudio(Room *room, QWidget *parent = 0);
                virtual ~DialogNewAudio();

                Output *getOutput() { return output; }

        protected:
                virtual void changeEvent(QEvent *e);

        private:
                Ui::DialogNewAudio *ui;
                Output *output;
                Room *room;

        private slots:
                void on_buttonBox_accepted();
};

#endif // DIALOGNEWAUDIO_H
