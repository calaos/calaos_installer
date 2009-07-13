#ifndef DIALOGNEWWAGO_H
#define DIALOGNEWWAGO_H

#include <QtGui/QDialog>
#include <QtGui/QAbstractButton>
#include <QUdpSocket>
#include <iostream>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
        class DialogNewWago;
}

class DialogNewWago : public QDialog
{
                Q_OBJECT
                Q_DISABLE_COPY(DialogNewWago)
        public:
                explicit DialogNewWago(int type, Room *room, QWidget *parent = 0);
                virtual ~DialogNewWago();

                Input *getInput() { return dynamic_cast<Input *>(io); }
                Output *getOutput() { return dynamic_cast<Output *>(io); }

                bool wantAnother() { return another; }

        protected:
                virtual void changeEvent(QEvent *e);

        private:
                Ui::DialogNewWago *ui;
                string type;
                IOBase *io;
                Room *room;
                QUdpSocket *udp_server;
                bool another;
                QPushButton *moreButton;

                void setWagoOutput(bool enable);

        private slots:
                void on_buttonBox_clicked(QAbstractButton* button);
                void on_button_test_disable_clicked();
                void on_button_test_enable_clicked();
                void on_button_detect_clicked();
                void on_buttonBox_accepted();
                void processUDPRequest();
};

#endif // DIALOGNEWROOM_H
