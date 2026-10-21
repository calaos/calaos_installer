#include "FakeMqttBroker.h"

#include <QTcpSocket>

namespace
{

QString takeString(const QByteArray &b, int &pos)
{
    const int len = (quint8(b.at(pos)) << 8) | quint8(b.at(pos + 1));
    const QString s = QString::fromUtf8(b.mid(pos + 2, len));
    pos += 2 + len;
    return s;
}

QByteArray lengthPrefixed(const QByteArray &s)
{
    return QByteArray(1, char(s.size() / 256)) + QByteArray(1, char(s.size() % 256)) + s;
}

}

FakeMqttBroker::FakeMqttBroker(QObject *parent):
    QTcpServer(parent)
{
    connect(this, &QTcpServer::newConnection, this, [this]()
    {
        if (m_client)
            m_client->disconnect(this);
        m_client = nextPendingConnection();
        m_in.clear();
        connect(m_client, &QTcpSocket::readyRead, this, &FakeMqttBroker::onData);
    });
}

quint16 FakeMqttBroker::start()
{
    listen(QHostAddress::LocalHost, 0);
    return serverPort();
}

bool FakeMqttBroker::matches(const QString &filter, const QString &topic)
{
    const QStringList f = filter.split('/'), t = topic.split('/');
    for (int i = 0;i < f.size();i++)
    {
        if (f[i] == "#")
            return true;
        if (i >= t.size() || (f[i] != "+" && f[i] != t[i]))
            return false;
    }
    return f.size() == t.size();
}

void FakeMqttBroker::sendRaw(const QByteArray &bytes)
{
    if (!m_client)
        return;
    if (!byteByByte)
    {
        m_client->write(bytes);
        return;
    }
    for (char c: bytes)
    {
        m_client->write(&c, 1);
        m_client->flush();
    }
}

void FakeMqttBroker::send(quint8 header, const QByteArray &body)
{
    QByteArray out(1, char(header));
    int n = body.size();
    while (true)
    {
        const int digit = n % 128;
        n /= 128;
        out.append(char(n ? digit | 0x80 : digit));
        if (!n)
            break;
    }
    sendRaw(out + body);
}

void FakeMqttBroker::publish(const QString &topic, const QByteArray &payload, bool retain)
{
    static quint16 nextId = 1;
    QByteArray body = lengthPrefixed(topic.toUtf8());
    quint8 header = 0x30;
    if (retain)
        header |= 0x01;
    if (deliverAtQos1 || deliverAtQos2)
    {
        header |= deliverAtQos2 ? 0x04 : 0x02;
        const quint16 id = nextId++;
        if (!nextId)
            nextId = 1;
        body.append(char(id >> 8));
        body.append(char(id & 0xFF));
    }
    send(header, body + payload);
    if (deliverAtQos2 && duplicateQos2)
        send(header | 0x08, body + payload);
}

void FakeMqttBroker::onData()
{
    m_in += m_client->readAll();
    while (m_in.size() >= 2)
    {
        int len = 0, mul = 1, pos = 1;
        while (true)
        {
            if (pos >= m_in.size())
                return;
            const quint8 d = quint8(m_in.at(pos++));
            len += (d & 0x7F) * mul;
            mul *= 128;
            if (!(d & 0x80))
                break;
        }
        if (m_in.size() < pos + len)
            return;
        const quint8 header = quint8(m_in.at(0));
        const QByteArray body = m_in.mid(pos, len);
        m_in.remove(0, pos + len);
        handle(header, body);
        if (!m_client)
            return;
    }
}

void FakeMqttBroker::handle(quint8 header, const QByteArray &body)
{
    switch (header >> 4)
    {
    case 1: //CONNECT
    {
        connects++;
        int pos = 0;
        if (takeString(body, pos) != "MQTT" || body.at(pos) != 4)
            qWarning() << "not a 3.1.1 CONNECT";
        const quint8 flags = quint8(body.at(pos + 1));
        keepAlive = (quint8(body.at(pos + 2)) << 8) | quint8(body.at(pos + 3));
        pos += 4;
        cleanSession = flags & 0x02;
        clientId = takeString(body, pos);
        user.clear();
        password.clear();
        if (flags & 0x80)
            user = takeString(body, pos);
        hadPassword = flags & 0x40;
        if (hadPassword)
            password = takeString(body, pos);

        if (!answerConnect)
            return;
        if (closeAfterConnect)
        {
            connect(m_client, &QTcpSocket::disconnected, m_client, &QObject::deleteLater);
            m_client->disconnectFromHost();
            m_client = nullptr;
            return;
        }
        int code = connackCode;
        if (code == 0 && !requiredUser.isEmpty() && (user != requiredUser || password != requiredPassword))
            code = 4;
        send(0x20, QByteArray("\x00", 1) + char(code));
        if (closeAfterConnack)
        {
            connect(m_client, &QTcpSocket::disconnected, m_client, &QObject::deleteLater);
            m_client->disconnectFromHost();
            m_client = nullptr;
        }
        return;
    }
    case 8: //SUBSCRIBE
    {
        if ((header & 0x0F) != 0x02)
            qWarning() << "SUBSCRIBE with wrong flags";
        int pos = 2;
        QByteArray codes;
        QStringList added;
        while (pos < body.size())
        {
            const QString f = takeString(body, pos);
            pos++; //requested QoS
            const bool refused = refuseSubscriptions || refusedFilters.contains(f);
            codes.append(char(refused ? 0x80 : 0x00));
            if (!refused)
                added << f;
            filters << f;
        }
        send(0x90, body.left(2) + codes);
        for (const auto &m: std::as_const(retained))
            for (const QString &f: std::as_const(added))
                if (matches(f, m.first))
                {
                    publish(m.first, m.second, true);
                    break;
                }
        return;
    }
    case 4: //PUBACK
        pubacks++;
        return;
    case 5: //PUBREC
        pubrecs++;
        //The duplicate's PUBREC comes second: one PUBREL for both.
        if (!duplicateQos2 || pubrecs % 2 == 0)
            send(0x62, body);
        return;
    case 7: //PUBCOMP
        pubcomps++;
        return;
    case 12: //PINGREQ
        pings++;
        if (answerPings)
            send(0xD0, QByteArray());
        return;
    case 14: //DISCONNECT
        disconnects++;
        return;
    default:
        return;
    }
}
