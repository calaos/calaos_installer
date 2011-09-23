/******************************************************************************
**  Copyright (c) 2006-2011, Calaos. All Rights Reserved.
**
**  This file is part of Calaos Home.
**
**  Calaos Home is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 3 of the License, or
**  (at your option) any later version.
**
**  Calaos Home is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Calaos; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
******************************************************************************/

#ifndef WAGOMODBUS_H
#define WAGOMODBUS_H

#include <ConfigOptions.h>
#include <QtNetwork>
#include <QtGlobal>

typedef QByteArray Modbus;
#define MODBUS_LENGTH_MIN 12

class WagoModbus : public QObject
{
        Q_OBJECT

        private:
                QTcpSocket *modbusSocket;
                Modbus request;

        public:
                WagoModbus(QObject *parent = NULL);
                ~WagoModbus();

                static void createModbusRequest(Modbus &req, int function, int addr, int data);
                void sendRequest(Modbus &query);

        signals:
                void requestDone(bool success, Modbus &res);

        private slots:
                void modbusReadTCPPacket();
                void modbusTcpError(QAbstractSocket::SocketError socketError);
                void modbusTcpConnected();
                void modbusTcpDisconnected();
};

#endif // WAGOMODBUS_H
