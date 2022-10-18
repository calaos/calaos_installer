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
#include "WagoModbus.h"

WagoModbus::WagoModbus(QObject *parent):
    QObject(parent),
    modbusSocket(new QTcpSocket(this))
{
    connect(modbusSocket, &QTcpSocket::readyRead, this, &WagoModbus::modbusReadTCPPacket);
    connect(modbusSocket, &QTcpSocket::connected, this, &WagoModbus::modbusTcpConnected);
    connect(modbusSocket, &QTcpSocket::disconnected, this, &WagoModbus::modbusTcpDisconnected);
    connect(modbusSocket, &QTcpSocket::errorOccurred, this, &WagoModbus::modbusTcpError);
}

WagoModbus::~WagoModbus()
{
    if (modbusSocket)
    {
        modbusSocket->deleteLater();
    }
}

void WagoModbus::sendRequest(Modbus &query)
{
    request = query;

    modbusSocket->connectToHost(ConfigOptions::Instance().getWagoHost(), 502);
}

void WagoModbus::modbusReadTCPPacket()
{
    QByteArray res = modbusSocket->readAll();

    modbusSocket->close();

    /* Do some check on modbus result */
    if (request[0] != res[0] || request[1] != res[1] ||
        res.size() <= 10)
    {
        emit requestDone(false, res);

        return;
    }

    emit requestDone(true, res);
}

void WagoModbus::modbusTcpError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    qDebug() << "modbusTcpError: error: " << modbusSocket->errorString();

    Modbus data;
    emit requestDone(false, data);
}

void WagoModbus::modbusTcpConnected()
{
    quint64 dataSent = 0;
    while(dataSent < sizeof(request.size()))
    {
        qint64 sentNow = modbusSocket->write(request.constData() + dataSent, request.size());
        if(sentNow >= 0)
            dataSent += sentNow;
        else
            return;
    }
}

void WagoModbus::modbusTcpDisconnected()
{
    qDebug() << "modbusTcpDisconnected: disconnected !";
}

void WagoModbus::createModbusRequest(Modbus &req, int function, int addr, int data)
{
    req.clear();
    req.resize(MODBUS_LENGTH_MIN);

    /* Extract from MODBUS Messaging on TCP/IP Implementation Guide V1.0b
               (page 23/46):
               The transaction identifier is used to associate the future response
               with the request. So, at a time, on a TCP connection, this identifier
               must be unique. */
    static quint16 transac_id = 0;

    if (transac_id < 65535)
        transac_id++;
    else
        transac_id = 0;
    req[0] = transac_id >> 8;
    req[1] = transac_id & 0x00ff;

    /* Protocol Modbus */
    req[2] = 0;
    req[3] = 0;

    /* Length */
    int mbap_length = req.length() - 6;
    req[4] = mbap_length >> 8;
    req[5] = mbap_length & 0x00FF;

    req[6] = 1;
    req[7] = function;
    req[8] = addr >> 8;
    req[9] = addr & 0x00ff;
    req[10] = data >> 8;
    req[11] = data & 0x00ff;
}
