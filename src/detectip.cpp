#include "detectip.h"
#include "wagoconnect.h"

DetectIP::DetectIP(): QObject(NULL), tcpSocket(NULL)
{
}

void DetectIP::startDetectIP(QString wagohost)
{
    //Detect our own ip address
    tcpSocket = new QTcpSocket();
    connect(tcpSocket, &QTcpSocket::connected, this, &DetectIP::sock_connected);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &DetectIP::sock_error);

    tcpSocket->connectToHost(wagohost, 502);
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

