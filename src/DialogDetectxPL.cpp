#include "DialogDetectxPL.h"
#include "ui_DialogDetectxPL.h"

using namespace std;

DialogDetectxPL::DialogDetectxPL(const Params &params, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDetectxPL)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    SearchDevices();
}

DialogDetectxPL::~DialogDetectxPL()
{
    SendxPLHeartBeatEnd();
    if(udpSocket!=nullptr) delete udpSocket;
    delete ui;
}

void DialogDetectxPL::changeEvent(QEvent *e)
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

void DialogDetectxPL::on_buttonRefresh_clicked()
{
    SearchDevices();
}

Params& DialogDetectxPL::getParams()
{
    QList<QTreeWidgetItem *> qList = ui->treexPLDevices->selectedItems();

    if(qList.count() > 0)
    {
        params.Add("source", qList[0]->text(0).toStdString());
        params.Add("sensor", qList[0]->text(1).toStdString());
    }
    return params;
}

void DialogDetectxPL::SearchDevices()
{
    if (timer) return;

    ui->treexPLDevices->setEnabled(false);
    ui->buttonBox->setEnabled(false);
    ui->buttonRefresh->setEnabled(false);

    ui->treexPLDevices->clear();

    if (spinnerLayout) delete spinnerLayout;
    if (spinnerLayoutWidget) delete spinnerLayoutWidget;

    spinnerLayoutWidget = new QWidget(this);
    spinnerLayoutWidget->setGeometry(ui->treexPLDevices->geometry());
    spinnerLayout = new QHBoxLayout(spinnerLayoutWidget);
    spinnerLayout->setContentsMargins(0, 0, 0, 0);
    spinnerLayoutWidget->show();

    if (spinner) delete spinner;
    spinner = new QAnimationLabel(":/img/loader.gif", this);
    spinnerLayout->addWidget(spinner, 0, Qt::AlignCenter);
    spinner->start();

    udpSocket = new QUdpSocket(this);
    BindxPLPort();
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    SendxPLSensorRequest();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerDone()));
    timer->start(2000);
}

void DialogDetectxPL::BindxPLPort()
{
    bool found = false;
    bool error = false;

    if(udpSocket==nullptr) return;

    xPLPort = 3865;
    if(udpSocket->bind(QHostAddress::Any, xPLPort, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint))
        return;

    if(udpSocket->error() != QAbstractSocket::SocketError::AddressInUseError)
        return;

    for(xPLPort=49152; xPLPort <= 65535; xPLPort++)
    {
        if(udpSocket->bind(QHostAddress::Any, xPLPort, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint))
        {
            found = true;
            break;
        }

        if(udpSocket->error() != QAbstractSocket::SocketError::AddressInUseError)
        {
            error = true;
            break;
        }
    }
    if(error == true) return;
    if(found == false) return;

    SendxPLHeartBeat();
}

void DialogDetectxPL::SendxPLHeartBeat()
{
    QString localAdr;
    QString body("hbeat.app\n"
                "{\n"
                "interval=5\n"
                "port=%1\n"
                "remote-ip=%2\n"
                "}");

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if(address.isNull()) continue;
        if(address.isLoopback()) continue;
        if(address.protocol() != QAbstractSocket::IPv4Protocol) continue;
        localAdr = address.toString();
        break;
    }

    SendxPLMessage(xPLTypeMessage::stat, "*", body.arg(QString::number(xPLPort), localAdr));
}

void DialogDetectxPL::SendxPLHeartBeatEnd()
{
    QString body("hbeat.end\n"
                "{\n"
                "}");
    SendxPLMessage(xPLTypeMessage::stat, "*", body);
}

void DialogDetectxPL::SendxPLSensorRequest()
{
    QString body("sensor.request\n"
                "{\n"
                "request=current\n"
                "}");
    SendxPLMessage(xPLTypeMessage::cmnd, "*", body);
}

void DialogDetectxPL::SendxPLMessage(xPLTypeMessage type, QString target, QString body)
{
    if(udpSocket==nullptr) return;

    QString message("%1\n"
                    "{\n"
                    "hop=1\n"
                    "source=fragxpl-calaos.installer\n"
                    "target=%2\n"
                    "}\n"
                    "%3");
    QString typeStr;
    QByteArray data;

    if(type==xPLTypeMessage::cmnd)
        typeStr = "xpl-cmnd";
    else
        typeStr = "xpl-stat";

    data.append(message.arg(typeStr, target, body));
    udpSocket->writeDatagram(data, QHostAddress::Broadcast, 3865);
}

void DialogDetectxPL::onReadyRead()
{
    while (udpSocket->hasPendingDatagrams())
    {
        qint64 dataSize = udpSocket->pendingDatagramSize();
        QByteArray data(dataSize , '\0');

        udpSocket->readDatagram(data.data(), dataSize);

        QString str(data);
        QStringList lines = str.split(QRegExp("\n|\r\n|\r"));
        QStringList values;
        QString source;
        QString device;
        for(int i=0; i<lines.count(); i++)
        {
            values = lines[i].split("=");
            if(values[0] == "source") source = values[1];
            if(values[0] == "device") device = values[1];
        }
        if((source!="")&&(device!=""))
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treexPLDevices);
            item->setData(0, Qt::DisplayRole, source);
            item->setData(1, Qt::DisplayRole, device);
        }
    }
}

void DialogDetectxPL::timerDone()
{
    delete timer;
    delete spinner;
    delete spinnerLayout;
    delete spinnerLayoutWidget;
    delete udpSocket;

    ui->treexPLDevices->setEnabled(true);
    ui->buttonBox->setEnabled(true);
    ui->buttonRefresh->setEnabled(true);

    ui->treexPLDevices->resizeColumnToContents(0);
    ui->treexPLDevices->resizeColumnToContents(1);
}
