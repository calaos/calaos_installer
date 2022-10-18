#include "dialogdetectsqueezebox.h"
#include "ui_DialogDetectSqueezebox.h"

#include <iostream>

using namespace std;

SqueezeServer::SqueezeServer(QObject *parent):
    QObject(parent)
{
    cliSocket = new QTcpSocket(this);
    connect(cliSocket, &QTcpSocket::readyRead, this, &SqueezeServer::readTCPPacket);
    connect(cliSocket, &QTcpSocket::connected, this, &SqueezeServer::tcpConnected);
    connect(cliSocket, &QTcpSocket::errorOccurred,
            this, &SqueezeServer::tcpError);
}

SqueezeServer::~SqueezeServer()
{
}

void SqueezeServer::Connect(QHostAddress &addr)
{
    cliSocket->connectToHost(addr, 9090);
}

void SqueezeServer::SendCommand(QString c, QObject *obj, QString slot)
{
    cmd.cmd = c;
    cmd.res = "";
    cmd.slot = slot;
    cmd.obj = obj;

    c += "\n";

    cliSocket->write(c.toLocal8Bit());
}

void SqueezeServer::readTCPPacket()
{
    cmd.res = cliSocket->readLine().trimmed();
    if (cmd.obj)
    {
        QMetaObject::invokeMethod(cmd.obj,
                                  cmd.slot.toUtf8().constData(),
                                  Qt::DirectConnection,
                                  Q_ARG(QString, cmd.cmd),
                                  Q_ARG(QString, cmd.res));
    }
}

void SqueezeServer::tcpError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    emit error();
}

void SqueezeServer::tcpConnected()
{
    emit connected();
}

DialogDetectSqueezebox::DialogDetectSqueezebox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDetectSqueezebox)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

DialogDetectSqueezebox::~DialogDetectSqueezebox()
{
    delete ui;
}

void DialogDetectSqueezebox::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

Squeezebox *DialogDetectSqueezebox::getSelectedDevice()
{
    int i = ui->treeSqueezebox->currentIndex().row();

    if (i < 0 || i >= squeezebox.size())
        return NULL;

    return &squeezebox[i];
}

void DialogDetectSqueezebox::on_buttonRefresh_clicked()
{
    SearchDevices();
}

void DialogDetectSqueezebox::SearchDevices()
{
    if (timer) return;

    ui->treeSqueezebox->setEnabled(false);
    ui->buttonBox->setEnabled(false);
    ui->buttonRefresh->setEnabled(false);

    ui->treeSqueezebox->clear();
    squeezebox.clear();
    squeezeserver.clear();

    if (spinnerLayout) delete spinnerLayout;
    if (spinnerLayoutWidget) delete spinnerLayoutWidget;

    spinnerLayoutWidget = new QWidget(this);
    spinnerLayoutWidget->setGeometry(ui->treeSqueezebox->geometry());
    spinnerLayout = new QHBoxLayout(spinnerLayoutWidget);
    spinnerLayout->setContentsMargins(0, 0, 0, 0);
    spinnerLayoutWidget->show();

    if (spinner) delete spinner;
    spinner = new QAnimationLabel(":/img/loader.gif", this);
    spinnerLayout->addWidget(spinner, 0, Qt::AlignCenter);
    spinner->start();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DialogDetectSqueezebox::timerDoneAndDiscoverPlayers);
    timer->start(2000);

    socket = new QUdpSocket(this);
    socket->bind(3483);
    connect(socket, &QUdpSocket::readyRead, this, &DialogDetectSqueezebox::readUDPDatagrams);
    socket->writeDatagram((char *)&discoverPacket, sizeof(discoverPacket), QHostAddress(QHostAddress::Broadcast), 3483);
}

void DialogDetectSqueezebox::searchDone()
{
    delete timer;
    delete spinner;
    delete spinnerLayout;
    delete spinnerLayoutWidget;
    delete socket;
    if (squeeze) delete squeeze;

    ui->treeSqueezebox->setEnabled(true);
    ui->buttonBox->setEnabled(true);
    ui->buttonRefresh->setEnabled(true);

    for (int i = 0;i < squeezebox.size();i++)
    {
        Squeezebox &sq = squeezebox[i];

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeSqueezebox);
        item->setData(0, Qt::DisplayRole, sq.name);
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_sound.png"));
        item->setData(1, Qt::DisplayRole, sq.id);
        item->setData(2, Qt::DisplayRole, sq.server);
    }

    ui->treeSqueezebox->resizeColumnToContents(0);
    ui->treeSqueezebox->resizeColumnToContents(1);
}

void DialogDetectSqueezebox::timerDoneAndDiscoverPlayers()
{
    delete socket;
    delete timer;

    if (squeezeserver.isEmpty())
    {
        searchDone();
    }
    else
    {
        cpt = 0;
        scanSqueezeserver();
    }
}

void DialogDetectSqueezebox::readUDPDatagrams()
{
    while (socket && socket->hasPendingDatagrams())
    {
        SLIMProto packet;
        memset(&packet, '\0', sizeof(SLIMProto));

        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram((char *)&packet, sizeof(SLIMProto),
                             &sender, &senderPort);

        squeezeserver.push_back(sender);
    }
}

void DialogDetectSqueezebox::scanSqueezeserver()
{
    if (cpt < 0 || cpt >= squeezeserver.size())
    {
        if (squeeze)
            delete squeeze;
        searchDone();
        return;
    }

    QHostAddress &addr = squeezeserver[cpt];

    if (squeeze)
        delete squeeze;
    squeeze = new SqueezeServer(this);
    connect(squeeze, &SqueezeServer::connected, this, &DialogDetectSqueezebox::squeezeConnected);
    connect(squeeze, &SqueezeServer::error, this, &DialogDetectSqueezebox::squeezeError);
    connect(squeeze, &SqueezeServer::connectError, this, &DialogDetectSqueezebox::squeezeError);
    squeeze->Connect(addr);

    sq.id = "";
    sq.name = "";
    sq.server = addr.toString();
}

void DialogDetectSqueezebox::squeezeError()
{
    cpt++;
    scanSqueezeserver();
}

void DialogDetectSqueezebox::squeezeConnected()
{
    if (squeeze)
        squeeze->SendCommand("player count ?", this, "squeezePlayerCount");
}

void DialogDetectSqueezebox::squeezePlayerCount(QString cmd, QString res)
{
    Q_UNUSED(cmd);
    QStringList l = res.split(" ");

    numberSqueeze = 0;

    if (l.size() >= 3)
    {
        numberSqueeze = l[2].toInt();
    }

    cptSqueeze = 0;
    getSqueezeId();
}

void DialogDetectSqueezebox::getSqueezeId()
{
    if (cptSqueeze < 0 || cptSqueeze >= numberSqueeze)
    {
        cpt++;
        scanSqueezeserver();
        return;
    }

    if (squeeze)
        squeeze->SendCommand(QString("player id %1 ?").arg(cptSqueeze), this, "squeezePlayerId");
}

void DialogDetectSqueezebox::squeezePlayerId(QString cmd, QString res)
{
    Q_UNUSED(cmd);
    QStringList l = res.split(" ");

    if (l.size() >= 4)
    {
        sq.id = QUrl::fromPercentEncoding(l[3].toLocal8Bit());

        getSqueezeName();
    }
    else
    {
        cptSqueeze++;
        getSqueezeId();
    }
}

void DialogDetectSqueezebox::getSqueezeName()
{
    if (squeeze)
        squeeze->SendCommand(QString("player name %1 ?").arg(cptSqueeze), this, "squeezePlayerName");
}

void DialogDetectSqueezebox::squeezePlayerName(QString cmd, QString res)
{
    Q_UNUSED(cmd);
    QStringList l = res.split(" ");

    if (l.size() >= 4)
    {
        sq.name = QUrl::fromPercentEncoding(l[3].toLocal8Bit());

        squeezebox.push_back(sq);
    }

    cptSqueeze++;
    getSqueezeId();
}
