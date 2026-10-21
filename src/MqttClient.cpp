#include "MqttClient.h"

#include <QRandomGenerator>
#include <QTcpSocket>

//A slot of ours may open a modal dialog: its event loop must not delete the
//socket whose signal is still being emitted below it. Sockets closed meanwhile
//are deleted once the outermost socket callback returns.
class SocketCallback
{
public:
    explicit SocketCallback(MqttClient *c): m_client(c) { m_client->m_socketCallbacks++; }
    ~SocketCallback()
    {
        if (--m_client->m_socketCallbacks > 0)
            return;
        for (QTcpSocket *s: std::as_const(m_client->m_retired))
            s->deleteLater();
        m_client->m_retired.clear();
    }

private:
    MqttClient *m_client;
};

MqttClient::~MqttClient()
{
    //No signal from a destructor: only the DISCONNECT and the socket's close.
    if (m_state == CONNECTED)
        send(MqttPacket::disconnect());
    closeSocket(true);
}

MqttClient::MqttClient(QObject *parent):
    QObject(parent)
{
    m_connectTimer.setSingleShot(true);
    connect(&m_connectTimer, &QTimer::timeout, this, [this]()
    {
        fail(CONNECT_TIMED_OUT);
    });

    m_keepAliveTimer.setSingleShot(true);
    connect(&m_keepAliveTimer, &QTimer::timeout, this, [this]()
    {
        if (m_state != CONNECTED)
            return;
        if (m_pingTimer.isActive())
        {
            m_keepAliveTimer.start(m_keepAlive * 1000);
            return;
        }
        send(MqttPacket::pingreq());
        m_pingTimer.start(m_keepAlive * 1000);
    });

    m_pingTimer.setSingleShot(true);
    connect(&m_pingTimer, &QTimer::timeout, this, [this]()
    {
        fail(PING_TIMED_OUT);
    });
}

void MqttClient::connectToHost()
{
    closeSocket(false);
    m_error = NO_CLIENT_ERROR;
    m_connackCode = 0;
    m_socketError = QAbstractSocket::UnknownSocketError;
    m_tcpUp = false;
    m_connackReceived = false;
    m_reader.clear();
    m_pendingSubscriptions.clear();
    m_qos2Received.clear();
    if (!m_fixedClientId)
        //23 letters and digits at most: the only client ids every broker must accept.
        m_clientId = QStringLiteral("calaosinst%1")
                     .arg(QRandomGenerator::global()->generate(), 8, 16, QLatin1Char('0'));

    setState(CONNECTING);
    if (m_connectTimeoutMs > 0)
        m_connectTimer.start(m_connectTimeoutMs);
    openTransport();
}

void MqttClient::openTransport()
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, [this]()
    {
        SocketCallback guard(this);
        onTransportReady();
    });
    connect(m_socket, &QTcpSocket::readyRead, this, [this]()
    {
        SocketCallback guard(this);
        onReadyRead();
    });
    connect(m_socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError e)
    {
        SocketCallback guard(this);
        onSocketError(e);
    });
    connect(m_socket, &QTcpSocket::disconnected, this, [this]()
    {
        SocketCallback guard(this);
        onSocketError(QAbstractSocket::RemoteHostClosedError);
    });
    m_socket->connectToHost(m_host, m_port);
}

void MqttClient::disconnectFromHost()
{
    if (m_state == UNCONNECTED)
        return;
    if (m_state == CONNECTED)
        send(MqttPacket::disconnect());
    closeSocket(true);
    setState(UNCONNECTED);
    emit disconnected();
}

quint16 MqttClient::subscribe(const QStringList &filters)
{
    if (m_state != CONNECTED)
        return 0;
    quint16 id = m_nextId;
    do
    {
        id++;
    }
    while (id == 0 || m_pendingSubscriptions.contains(id));
    const QByteArray packet = MqttPacket::subscribe(id, filters);
    if (packet.isEmpty())
        return 0;
    m_nextId = id;
    m_pendingSubscriptions.insert(id, filters.size());
    send(packet);
    return id;
}

void MqttClient::onTransportReady()
{
    m_tcpUp = true;
    MqttPacket::Connect c;
    c.clientId = m_clientId;
    c.user = m_user;
    c.password = m_password;
    c.keepAlive = m_keepAlive;
    send(MqttPacket::connect(c));
}

void MqttClient::send(const QByteArray &packet)
{
    if (!m_socket)
        return;
    m_socket->write(packet);
    if (m_keepAlive > 0)
        m_keepAliveTimer.start(m_keepAlive * 1000);
}

void MqttClient::armKeepAlive()
{
    if (m_keepAlive > 0 && !m_keepAliveTimer.isActive())
        m_keepAliveTimer.start(m_keepAlive * 1000);
}

void MqttClient::onReadyRead()
{
    if (!m_socket)
        return;
    const quint64 session = m_session;
    m_reader.append(m_socket->readAll());
    while (session == m_session)
    {
        if (!m_connackReceived)
        {
            //Anything but a CONNACK first is not a broker: do not wait for a length it never meant.
            const QByteArray head = m_reader.peek(2);
            if ((head.size() >= 1 && quint8(head.at(0)) != (MqttPacket::CONNACK << 4)) ||
                (head.size() == 2 && head.at(1) != 2))
            {
                fail(PROTOCOL_VIOLATION);
                return;
            }
        }

        MqttPacket p;
        switch (m_reader.next(p))
        {
        case MqttStreamReader::NEED_MORE:
            //A slot that ran a nested event loop got no readyRead for what came meanwhile.
            if (m_socket && m_socket->bytesAvailable() > 0)
            {
                m_reader.append(m_socket->readAll());
                break;
            }
            return;
        case MqttStreamReader::MALFORMED:
            fail(PROTOCOL_VIOLATION);
            return;
        case MqttStreamReader::TOO_LARGE:
            fail(PACKET_TOO_LARGE);
            return;
        case MqttStreamReader::PACKET:
            handle(p);
            break;
        }
    }
}

void MqttClient::handle(MqttPacket &p)
{
    if (!p.isValidFromBroker())
    {
        fail(PROTOCOL_VIOLATION);
        return;
    }

    if (!m_connackReceived)
    {
        bool sessionPresent = false;
        int code = 0;
        if (!p.readConnack(sessionPresent, code))
        {
            fail(PROTOCOL_VIOLATION);
            return;
        }
        m_connackReceived = true;
        m_connectTimer.stop();
        if (code != 0)
        {
            m_connackCode = code;
            fail(CONNECTION_REFUSED);
            return;
        }
        setState(CONNECTED);
        armKeepAlive();
        emit connected();
        return;
    }

    switch (p.type())
    {
    case MqttPacket::SUBACK:
    {
        quint16 id = 0;
        QList<int> codes;
        if (!p.readSuback(id, codes))
        {
            fail(PROTOCOL_VIOLATION);
            return;
        }
        auto it = m_pendingSubscriptions.find(id);
        if (it == m_pendingSubscriptions.end())
            return;
        if (codes.size() != it.value())
        {
            fail(PROTOCOL_VIOLATION);
            return;
        }
        m_pendingSubscriptions.erase(it);
        QList<bool> granted;
        for (int c: std::as_const(codes))
            granted << (c != 0x80);
        emit subscribed(id, granted);
        return;
    }
    case MqttPacket::PUBLISH:
    {
        MqttPacket::Publish m;
        if (!p.takePublish(m))
        {
            fail(PROTOCOL_VIOLATION);
            return;
        }
        //Above QoS 0 although we asked for 0: a broker resuming an old session may.
        if (m.qos == 1)
            send(MqttPacket::ack(MqttPacket::PUBACK, m.id));
        else if (m.qos == 2)
        {
            send(MqttPacket::ack(MqttPacket::PUBREC, m.id));
            //A QoS 2 resend before PUBREL is the same message.
            if (m_qos2Received.contains(m.id))
                return;
            m_qos2Received.insert(m.id);
        }
        emit messageReceived(m.topic, m.payload, m.retain);
        return;
    }
    case MqttPacket::PUBREL:
    {
        quint16 id = 0;
        if (!p.readAck(id))
        {
            fail(PROTOCOL_VIOLATION);
            return;
        }
        m_qos2Received.remove(id);
        send(MqttPacket::ack(MqttPacket::PUBCOMP, id));
        return;
    }
    case MqttPacket::PINGRESP:
        if (!p.body.isEmpty())
        {
            fail(PROTOCOL_VIOLATION);
            return;
        }
        m_pingTimer.stop();
        armKeepAlive();
        return;
    case MqttPacket::CONNACK:
        fail(PROTOCOL_VIOLATION);
        return;
    default:
        //PUBACK, PUBREC, PUBCOMP, UNSUBACK: answers to packets this client never sends.
        return;
    }
}

void MqttClient::onSocketError(QAbstractSocket::SocketError e)
{
    if (m_state == UNCONNECTED || !m_socket)
        return;
    //A broker that answers and closes at once (a refused CONNACK) is read before its close.
    if (m_socket->bytesAvailable() > 0)
    {
        const quint64 session = m_session;
        onReadyRead();
        if (session != m_session)
            return;
    }
    m_socketError = e;
    fail(SOCKET_FAILED);
}

void MqttClient::setState(State s)
{
    if (m_state == s)
        return;
    m_state = s;
    emit stateChanged(s);
}

void MqttClient::fail(ClientError e)
{
    if (m_state == UNCONNECTED)
        return;
    m_error = e;
    closeSocket(false);
    const quint64 session = m_session;
    m_state = UNCONNECTED;
    emit errorOccurred(e);
    //A slot may have connected again.
    if (session != m_session)
        return;
    emit stateChanged(UNCONNECTED);
    emit disconnected();
}

void MqttClient::closeSocket(bool graceful)
{
    m_connectTimer.stop();
    m_keepAliveTimer.stop();
    m_pingTimer.stop();
    m_session++;
    if (!m_socket)
        return;
    QTcpSocket *s = m_socket;
    m_socket = nullptr;
    s->disconnect(this);
    if (!graceful)
    {
        s->abort();
        retire(s);
        return;
    }
    //Still looking up or connecting, it would never emit disconnected(): a failed
    //connect only emits errorOccurred().
    if (s->state() != QAbstractSocket::ConnectedState)
    {
        s->abort();
        retire(s);
        return;
    }
    //Flushed now: deleting the client right after must not lose the DISCONNECT.
    s->flush();
    s->disconnectFromHost();
    if (s->state() == QAbstractSocket::UnconnectedState)
    {
        retire(s);
        return;
    }
    connect(s, &QTcpSocket::disconnected, this, [this, s]() { retire(s); });
    connect(s, &QTcpSocket::errorOccurred, this, [this, s]() { retire(s); });
}

void MqttClient::retire(QTcpSocket *s)
{
    if (m_socketCallbacks > 0)
    {
        if (!m_retired.contains(s))
            m_retired << s;
        return;
    }
    s->deleteLater();
}
