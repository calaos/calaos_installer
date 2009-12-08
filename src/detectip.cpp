#include "detectip.h"
#include "wagoconnect.h"

DetectIP::DetectIP(): QObject(NULL), tcpSocket(NULL)
{
}

void DetectIP::startDetectIP()
{
        //Detect our own ip address
        tcpSocket = new QTcpSocket();
        connect(tcpSocket, SIGNAL(connected()), SLOT(sock_connected()));
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(sock_error(QAbstractSocket::SocketError)));

        tcpSocket->connectToHost(QString(WAGO_HOST), 502);
}

void DetectIP::sock_connected()
{
        if (!tcpSocket) return;

        QHostAddress myHost = tcpSocket->localAddress();
        delete tcpSocket;

        QString cmd = "WAGO_SET_SERVER_IP " + myHost.toString();

        //Send a WAGO_SET_SERVER_IP to the wago, so we can receive WAGO INT datagrams
        WagoConnect::Instance().SendCommand(cmd);
}

void DetectIP::sock_error(QAbstractSocket::SocketError)
{
        cout << "DetectIP: socket error" << endl;
}

