#include "dialognewwago.h"
#include "ui_dialognewwago.h"
#include "mainwindow.h"

DialogNewWago::DialogNewWago(int t, Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewWago), io(NULL), room(r), udp_server(NULL)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();

        if (t == ITEM_INTER)
        {
                ui->pager->setCurrentIndex(1);
                type = "WIDigital";
        }
        else if (t == ITEM_LIGHT)
        {
                ui->pager->setCurrentIndex(0);
                type = "WODigital";
                ui->check_triple->hide();
        }
}

DialogNewWago::~DialogNewWago()
{
        delete ui;
}

void DialogNewWago::changeEvent(QEvent *e)
{
        QDialog::changeEvent(e);
        switch (e->type())
        {
           case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
           default:
                break;
        }
}

void DialogNewWago::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        if (type == "WIDigital")
        {
                if (ui->check_triple->isChecked())
                        type = "WIDigitalTriple";
                else
                        type = "WIDigitalBP";
        }

        Params p;
        p.Add("name", ui->edit_name->text().toLocal8Bit().data());
        p.Add("type", type);
        p.Add("var", to_string(ui->spin_var->value()));

        if (type == "WODigital")
        {
                p.Add("gtype", "light");
                io = ListeRoom::Instance().createOutput(p, room);
        }
        else
        {
                io = ListeRoom::Instance().createInput(p, room);
        }

        accept();
}

void DialogNewWago::on_button_detect_clicked()
{
        //Start an udp socket to listen for wago inputs
        if (!udp_server)
        {
                udp_server = new QUdpSocket(this);
                udp_server->bind(WAGO_LISTEN_PORT);

                connect(udp_server, SIGNAL(readyRead()), this, SLOT(processUDPRequest()));

                ui->button_detect->setText(QString::fromUtf8("Arrêter..."));

                QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        }
        else
        {
               delete udp_server;
               udp_server = NULL;

               ui->button_detect->setText(QString::fromUtf8("Détection"));

               QApplication::restoreOverrideCursor();
        }
}

void DialogNewWago::processUDPRequest()
{
        if (!udp_server) return;

        while (udp_server->hasPendingDatagrams())
        {
                QByteArray datagram;
                datagram.resize(udp_server->pendingDatagramSize());
                udp_server->readDatagram(datagram.data(), datagram.size());

                string req = datagram.data();

                vector<string> tok;
                Utils::split(req, tok, " ", 4);

                if (tok[0] == "WAGO" && tok[1] == "INT")
                {
                        int var;
                        Utils::from_string(tok[2], var);

                        ui->spin_var->setValue(var);

                        on_button_detect_clicked();

                        return;
                }
        }
}

void DialogNewWago::setWagoOutput(bool enable)
{
        //send udp datagram to enable output
        QUdpSocket *udpSocket = new QUdpSocket(this);
        QByteArray datagram = "WAGO_SET_OUTPUT " + QByteArray::number(ui->spin_var->value());

        if (enable)
                datagram += " 1";
        else
                datagram += " 0";

        udpSocket->writeDatagram(datagram.data(), datagram.size(),
                              QHostAddress(QString(WAGO_HOST)), WAGO_LISTEN_PORT);

        delete udpSocket;
}

void DialogNewWago::on_button_test_enable_clicked()
{
        setWagoOutput(true);
}

void DialogNewWago::on_button_test_disable_clicked()
{
        setWagoOutput(false);
}
