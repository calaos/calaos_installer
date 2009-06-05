#include "detectip.h"

DetectIP::DetectIP(): QObject(NULL), tcpSocket(NULL), udpSocket(NULL)
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

        //Send a WAGO_SET_SERVER_IP to the wago, so we can receive WAGO INT datagrams
        QUdpSocket *udpSocket = new QUdpSocket();

        QByteArray datagram = "WAGO_SET_SERVER_IP ";
        datagram += myHost.toString().toLocal8Bit().data();

        ip = myHost.toString().toLocal8Bit().data();

        udpSocket->writeDatagram(datagram.data(), datagram.size(),
                              QHostAddress(QString(WAGO_HOST)), WAGO_LISTEN_PORT);

        delete udpSocket;
}

void DetectIP::sock_error(QAbstractSocket::SocketError)
{
        cout << "socket error" << endl;
}

