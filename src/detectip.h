#ifndef DETECTIP_H
#define DETECTIP_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
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

                string getLocalIP() { return ip; }

        private:
                DetectIP();

                QTcpSocket *tcpSocket;
                QUdpSocket *udpSocket;

                string ip;

        private slots:
                void sock_connected();
                void sock_error(QAbstractSocket::SocketError error);
};

#endif // DETECTIP_H
