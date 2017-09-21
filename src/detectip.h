#ifndef DETECTIP_H
#define DETECTIP_H

#include <QObject>
#include <QTcpSocket>
#include <iostream>
#include <Utils.h>

using namespace std;

class DetectIP: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DetectIP)
public:
    static DetectIP &Instance()
    {
        static DetectIP dip;
        return dip;
    }

    void startDetectIP(QString wagohost);
    string getLocalIP() { return ip; }

private:
    DetectIP();

    QTcpSocket *tcpSocket;

    string ip;

private slots:
    void sock_connected();
    void sock_error(QAbstractSocket::SocketError error);
};

#endif // DETECTIP_H
