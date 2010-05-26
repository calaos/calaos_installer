#include "wagoconnect.h"

WagoConnect::WagoConnect(QObject *parent):
                QObject(parent), connect_status(WAGO_DISCONNECTED),
                use_proxy(false), udpSocket(NULL), httpProxy(NULL),
                mainTimer(NULL), heartbeatTimer(NULL)
{
}

WagoConnect::~WagoConnect()
{
}

void WagoConnect::Connect(QString ip, bool proxy)
{
        if (connect_status != WAGO_DISCONNECTED)
        {
                cout << "WagoConnect::Connect(): Already connected." << endl;
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

        if (connect_status != WAGO_CONNECTED)
        {
                cout << "WagoConnect::Disconnect(): Not connected." << endl;
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
                cout << "WagoConnect::SendCommand(): Not connected." << endl;

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
        if (list.size() == 2)
        {
                if (list[0] == command)
                {
                        wago_fwversion = list[1];
                        if (connect_status != WAGO_CONNECTED)
                        {
                                connect_status = WAGO_CONNECTED;
                                emit connected(wago_ip, use_proxy);

                                /* Start the IP detection mechanism and send the
                                 * WAGO_SET_SERVER_IP command to catch INT datagram from wago
                                 */
                                DetectIP::Instance().startDetectIP();

                                if (wago_fwversion < WAGO_FW_VESION)
                                        emit updateNeeded(wago_fwversion);
                        }
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

                cout << "mainTick(): timeout=" << current_cmd.timeout << endl;

                // More than 5s is a timeout
                if (current_cmd.timeout >= 100)
                {
                        emit error(WERROR_TIMEOUT);

                        cout << "WagoConnect: Error, timeout waiting reply for command: \"" <<
                                        current_cmd.command.toLocal8Bit().data() << "\"" << endl;

                        Disconnect();
                }

                return;
        }

        if (commands.isEmpty()) return;

        current_cmd = commands.dequeue();

        cout << "mainTick(): send: " << current_cmd.command.toLocal8Bit().data() << endl;

        if (wago_ip.isEmpty())
        {
                cout << "mainTick(): wago_ip is empty !" << endl;
                return;
        }

        QByteArray datagram(current_cmd.command.toLocal8Bit().data());
        udpSocket->writeDatagram(datagram.data(), datagram.size(),
                              QHostAddress(wago_ip), WAGO_LISTEN_PORT);
}

void WagoConnect::readPendingDatagrams()
{
        cout << "mainTick(): reading..." << endl;

        while (udpSocket && udpSocket->hasPendingDatagrams())
        {
                QByteArray datagram;
                datagram.resize(udpSocket->pendingDatagramSize());

                QHostAddress sender;
                quint16 senderPort;

                udpSocket->readDatagram(datagram.data(), datagram.size(),
                                        &sender, &senderPort);

                if (sender.toString() != wago_ip) continue;

                current_cmd.response = datagram.data();
                current_cmd.emitCallback();

                emit responseReceived(current_cmd.command, current_cmd.response);

                cout << "mainTick(): received: " << current_cmd.response.toLocal8Bit().data() << endl;

                //clear current command
                current_cmd = WagoCommand();
        }
}

