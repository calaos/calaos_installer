#include "dialognewdali.h"
#include "ui_dialognewdali.h"
#include "mainwindow.h"

DialogNewDali::DialogNewDali(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewDali), dali(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();
}

DialogNewDali::~DialogNewDali()
{
        delete ui;
}

void DialogNewDali::changeEvent(QEvent *e)
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

void DialogNewDali::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toLocal8Bit().constData());
        p.Add("line", "1");
        p.Add("type", "WODali");
        p.Add("address", to_string(ui->spin_addr->value()));
        p.Add("fade_time", to_string(ui->spin_time->value()));

        if (ui->check_group->isChecked())
                p.Add("group", "1");
        else
                p.Add("group", "0");

        dali = ListeRoom::Instance().createOutput(p, room);

        accept();
}

void DialogNewDali::setWagoDali(bool enable)
{
        //send udp datagram to enable output
        QUdpSocket *udpSocket = new QUdpSocket(this);
        QByteArray datagram = "WAGO_DALI_SET 1 ";

        if (ui->check_group->isChecked())
                datagram += "1 ";
        else
                datagram += "0 ";

        datagram += QByteArray::number(ui->spin_addr->value());

        if (enable)
                datagram += " 100 ";
        else
                datagram += " 0 ";

        datagram += QByteArray::number(ui->spin_time->value());

        udpSocket->writeDatagram(datagram.data(), datagram.size(),
                              QHostAddress(QString(WAGO_HOST)), WAGO_LISTEN_PORT);

        delete udpSocket;
}

void DialogNewDali::on_pushButton_clicked()
{
        setWagoDali(true);
}

void DialogNewDali::on_pushButton_2_clicked()
{
        setWagoDali(false);
}
