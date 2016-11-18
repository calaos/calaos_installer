#include "dialogautodetect.h"
#include "ui_dialogautodetect.h"

#define BCAST_UDP_PORT 4545

DialogAutoDetect::DialogAutoDetect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAutoDetect)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    udpSocket = new QUdpSocket(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    calaosDiscover();
    timer->start(10000);
    connect(udpSocket, SIGNAL(readyRead()),
               this, SLOT(readPendingDatagrams()));

}

DialogAutoDetect::~DialogAutoDetect()
{
    delete timer;
    delete udpSocket;
    delete ui;
}


QString DialogAutoDetect::getHost()
{
    QListWidgetItem *it = ui->list->currentItem();
    return it->text();
}

void DialogAutoDetect::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress sender;
        quint16 senderPort;
        QList<QListWidgetItem *> items;

        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        QString msg(datagram.left(9));
        QString ip(datagram.mid(9));

        if (msg != QString("CALAOS_IP"))
            return;

        items = ui->list->findItems(ip, Qt::MatchExactly);
        if (!items.count())
        {
            ui->list->addItem(ip);
            if (ui->list->count() == 1)
                ui->list->setCurrentRow(0);
        }
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void DialogAutoDetect::timeout()
{
    calaosDiscover();
}

void DialogAutoDetect::calaosDiscover()
{
    QByteArray datagram = "CALAOS_DISCOVER";
    QHostAddress broadcastAddress = QHostAddress("255.255.255.255");
    udpSocket->writeDatagram(datagram.data(),datagram.size(), broadcastAddress , BCAST_UDP_PORT);
}

void DialogAutoDetect::on_list_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    accept();
}
