#include "wagoconnect.h"
#include <DialogWagoFirmwareUpdate.h>
#include "ConfigOptions.h"

WagoConnect::WagoConnect(QObject *parent):
    QObject(parent),
    connect_status(WAGO_DISCONNECTED),
    use_proxy(false),
    udpSocket(NULL),
    mainTimer(NULL),
    heartbeatTimer(NULL),
    modbus(new WagoModbus(this))
{
}

WagoConnect::~WagoConnect()
{
}

void WagoConnect::Connect(QString ip, bool proxy)
{
    if (connect_status != WAGO_DISCONNECTED)
    {
        qDebug() << "WagoConnect::Connect(): Already connected.";
        return;
    }

    commands.clear();

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(WAGO_LISTEN_PORT);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    wago_ip = ip;
    use_proxy = proxy;
    current_cmd = WagoCommand();

    heartbeatTimer = new QTimer(this);
    mainTimer = new QTimer(this);

    connect(heartbeatTimer, SIGNAL(timeout()), this, SLOT(heartbeatTick()));
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(mainTick()));

    heartbeatTimer->start(4000);
    heartbeatTick();

    mainTimer->start(50);
}

void WagoConnect::Disconnect()
{
    if (mainTimer) delete mainTimer;
    if (heartbeatTimer) delete heartbeatTimer;
    if (udpSocket) delete udpSocket;

    mainTimer = NULL;
    heartbeatTimer = NULL;
    udpSocket = NULL;

    wago_ip = "";
    wago_type = "";
    wago_fwversion = "";

    if (connect_status != WAGO_CONNECTED)
    {
        qDebug() << "WagoConnect::Disconnect(): Not connected.";
        return;
    }

    connect_status = WAGO_DISCONNECTED;

    emit disconnected();
}

void WagoConnect::SendCommand(QString cmd, QObject *object, QString slotName)
{
    if (connect_status != WAGO_CONNECTED && cmd != "WAGO_GET_VERSION")
    {
        emit error(WERROR_NOTCONNECTED);
        qDebug() << "WagoConnect::SendCommand(): Not connected.";

        return;
    }

    WagoCommand wc(cmd, object, slotName);
    commands.enqueue(wc);
}

void WagoConnect::heartbeatTick()
{
    SendCommand("WAGO_GET_VERSION", this, "heartbeat_cb");
}

void WagoConnect::heartbeat_cb(QString command, QString response)
{
    if (command != "WAGO_GET_VERSION")
    {
        emit error(WERROR_CONNECT_FAILED);
        Disconnect();

        return;
    }

    QStringList list = response.split(" ");
    if (list.size() >= 2)
    {
        if (list[0] == command)
        {
            wago_fwversion = list[1];
            if (list.size() == 3) wago_type = list[2];
            if (connect_status != WAGO_CONNECTED)
            {
                connect_status = WAGO_CONNECTED;
                emit connected(wago_ip, use_proxy);

                /* Start the IP detection mechanism and send the
                                 * WAGO_SET_SERVER_IP command to catch INT datagram from wago
                                 */
                DetectIP::Instance().startDetectIP(wago_ip);

                if (wago_type == "")
                {
                    getWagoTypeModbus();
                }
                else
                {
                    if (wago_type == "750-842" && wago_fwversion < WAGO_FW_VERSION_842)
                        emit updateNeeded(wago_fwversion, QString::fromLatin1(WAGO_FW_VERSION_842));
                    if (wago_type == "750-841" && wago_fwversion < WAGO_FW_VERSION_841)
                        emit updateNeeded(wago_fwversion, QString::fromLatin1(WAGO_FW_VERSION_841));
                    if (wago_type == "750-849" && wago_fwversion < WAGO_FW_VERSION_849)
                        emit updateNeeded(wago_fwversion, QString::fromLatin1(WAGO_FW_VERSION_849));
                    if (wago_type == "750-881" && wago_fwversion < WAGO_FW_VERSION_881)
                        emit updateNeeded(wago_fwversion, QString::fromLatin1(WAGO_FW_VERSION_881));
                }
            }
        }
    }
    //Hack for emulating connection when running on localhost, for dev only
    else if (wago_ip == "127.0.0.1" ||
             wago_ip == "localhost")
    {
        if (connect_status != WAGO_CONNECTED)
        {
            connect_status = WAGO_CONNECTED;
            wago_type = "750-999";
            wago_fwversion = "9.9";
            emit connected(wago_ip, use_proxy);
        }
    }
}

void WagoConnect::mainTick()
{
    /* Wait for response if needed */
    if (!current_cmd.isNull() &&
        current_cmd.needResponse() &&
        current_cmd.response == "")
    {
        current_cmd.timeout++;

        qDebug() << "mainTick(): timeout=" << current_cmd.timeout;

        // More than 5s is a timeout
        if (current_cmd.timeout >= 100)
        {
            emit error(WERROR_TIMEOUT);

            qDebug() << "WagoConnect: Error, timeout waiting reply for command: \"" <<
                        current_cmd.command.toUtf8().data() << "\"";

            Disconnect();
        }

        return;
    }

    if (commands.isEmpty()) return;

    current_cmd = commands.dequeue();

    qDebug() << "mainTick(): send: " << current_cmd.command.toUtf8().data();

    if (wago_ip.isEmpty())
    {
        qDebug() << "mainTick(): wago_ip is empty !";
        return;
    }

    QByteArray datagram(current_cmd.command.toUtf8().data());
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress(wago_ip), WAGO_LISTEN_PORT);
}

void WagoConnect::readPendingDatagrams()
{
    qDebug() << "mainTick(): reading...";

    while (udpSocket && udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        qDebug() << "Received from: " << QHostAddress(sender.toIPv4Address()).toString();
        if (QHostAddress(sender.toIPv4Address()).toString() != wago_ip) continue;

        current_cmd.response = datagram.data();
        current_cmd.emitCallback();

        emit responseReceived(current_cmd.command, current_cmd.response);

        qDebug() << "mainTick(): received: " << current_cmd.response.toUtf8().data();

        //clear current command
        current_cmd = WagoCommand();
    }
}

void WagoConnect::ResetWago()
{
    if (connect_status != WAGO_DISCONNECTED)
        Disconnect();

    Modbus cmd;
    WagoModbus::createModbusRequest(cmd,
                                    0x06, /* write single register */
                                    0x2040,
                                    0x55AA);

    modbus->connect(modbus, SIGNAL(requestDone(bool,Modbus&)), this, SLOT(modbusResetDone(bool,Modbus&)));

    modbus->sendRequest(cmd);
}

void WagoConnect::modbusResetDone(bool success, Modbus &result)
{
    Q_UNUSED(result);
    Q_UNUSED(success);
    modbus->disconnect(modbus, SIGNAL(requestDone(bool,Modbus&)), this, SLOT(modbusResetDone(bool,Modbus&)));
}

void WagoConnect::modbusTypeDone(bool success, Modbus &result)
{
    modbus->disconnect(modbus, SIGNAL(requestDone(bool,Modbus&)), this, SLOT(modbusTypeDone(bool,Modbus&)));

    if (!success) return;

    int data = (result[9] << 8) | result[10];

    if (data == 841)
    {
        wago_type = "750-841";
        if (wago_fwversion < WAGO_FW_VERSION_841)
            emit updateNeeded(wago_fwversion, QString::fromLatin1(WAGO_FW_VERSION_841));
    }
    else if (data == 849)
    {
        wago_type = "750-849";
        if (wago_fwversion < WAGO_FW_VERSION_849)
            emit updateNeeded(wago_fwversion, QString::fromLatin1(WAGO_FW_VERSION_849));
    }
    else if (data == 842)
    {
        wago_type = "750-842";
        if (wago_fwversion < WAGO_FW_VERSION_842)
            emit updateNeeded(wago_fwversion, QString::fromLatin1(WAGO_FW_VERSION_842));
    }
    else if (data == 881)
    {
        wago_type = "750-881";
        if (wago_fwversion < WAGO_FW_VERSION_881)
            emit updateNeeded(wago_fwversion, QString::fromLatin1(WAGO_FW_VERSION_881));
    }
}

void WagoConnect::getWagoTypeModbus()
{
    Modbus cmd;
    WagoModbus::createModbusRequest(cmd,
                                    0x03, /* read multiple registers */
                                    0x2012,
                                    0x01);

    modbus->connect(modbus, SIGNAL(requestDone(bool,Modbus&)), this, SLOT(modbusTypeDone(bool,Modbus&)));

    modbus->sendRequest(cmd);
}

void WagoConnect::updateWago()
{
    QString host = wago_ip, type = wago_type, version = wago_fwversion;

    Disconnect();

    dialogUpdate = new DialogWagoFirmwareUpdate(host, type, version);
    connect(dialogUpdate, SIGNAL(updateFirmwareDone()), this, SLOT(updateWagoDone()));
    dialogUpdate->exec();

    delete dialogUpdate;

    this->Connect(ConfigOptions::Instance().getWagoHost());
}

void WagoConnect::updateWagoDone()
{
    ResetWago();
}
