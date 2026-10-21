#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <QAbstractSocket>
#include <QHash>
#include <QObject>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QTimer>

#include "MqttPacket.h"

class QTcpSocket;

// MQTT 3.1.1 client over plain TCP, reading only: connect, subscribe at QoS 0,
// receive, ping, disconnect. Messages a broker still sends at QoS 1 or 2 are
// acknowledged as the protocol requires. Not to be deleted from a slot of its
// signals that runs a nested event loop.
class MqttClient: public QObject
{
    Q_OBJECT

public:
    enum State { UNCONNECTED, CONNECTING, CONNECTED };
    enum ClientError
    {
        NO_CLIENT_ERROR,
        SOCKET_FAILED,      //socketError() says why
        CONNECTION_REFUSED, //connackCode() says why
        PROTOCOL_VIOLATION,
        PACKET_TOO_LARGE,
        CONNECT_TIMED_OUT,
        PING_TIMED_OUT,
    };

    explicit MqttClient(QObject *parent = nullptr);

    void setHost(const QString &host) { m_host = host; }
    void setPort(quint16 port) { m_port = port; }
    void setUser(const QString &user) { m_user = user; }
    void setPassword(const QString &password) { m_password = password; }
    //Random by default, so that two installers on one broker do not kick each other out.
    void setClientId(const QString &id) { m_clientId = id; m_fixedClientId = !id.isEmpty(); }
    //Seconds; 0 turns the pings off.
    void setKeepAlive(quint16 seconds) { m_keepAlive = seconds; }
    //Until CONNACK; 0 waits as long as the socket does.
    void setConnectTimeout(int ms) { m_connectTimeoutMs = ms; }
    void setMaximumPacketSize(int bytes) { m_reader.setMaximumPacketSize(bytes); }

    QString host() const { return m_host; }
    quint16 port() const { return m_port; }
    QString clientId() const { return m_clientId; }
    State state() const { return m_state; }
    ClientError error() const { return m_error; }
    int connackCode() const { return m_connackCode; }
    QAbstractSocket::SocketError socketError() const { return m_socketError; }
    //The TCP connection was up when the error came, the broker had not answered CONNECT yet.
    bool failedBeforeConnack() const { return m_tcpUp && !m_connackReceived; }

    void connectToHost();
    //Sends DISCONNECT when connected. Emits disconnected() unless already unconnected.
    void disconnectFromHost();
    //Packet id of the SUBSCRIBE, 0 when not connected or a filter is invalid.
    quint16 subscribe(const QStringList &filters);

signals:
    void stateChanged(MqttClient::State state);
    void connected();
    //After errorOccurred() too.
    void disconnected();
    //granted[i] for filters[i] of that subscribe().
    void subscribed(quint16 id, const QList<bool> &granted);
    void messageReceived(const QString &topic, const QByteArray &payload, bool retained);
    void errorOccurred(MqttClient::ClientError error);

private:
    QTcpSocket *m_socket = nullptr;
    QString m_host;
    quint16 m_port = 1883;
    QString m_user, m_password, m_clientId;
    bool m_fixedClientId = false;
    quint16 m_keepAlive = 60;
    int m_connectTimeoutMs = 0;

    State m_state = UNCONNECTED;
    ClientError m_error = NO_CLIENT_ERROR;
    int m_connackCode = 0;
    QAbstractSocket::SocketError m_socketError = QAbstractSocket::UnknownSocketError;
    bool m_tcpUp = false;
    bool m_connackReceived = false;

    MqttStreamReader m_reader;
    QTimer m_connectTimer;
    QTimer m_keepAliveTimer;
    QTimer m_pingTimer;
    quint16 m_nextId = 0;
    QHash<quint16, int> m_pendingSubscriptions; //id -> number of filters
    QSet<quint16> m_qos2Received;
    quint64 m_session = 0;
    int m_socketCallbacks = 0;
    QList<QTcpSocket *> m_retired;

    void openTransport();
    void onTransportReady();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError e);
    void handle(const MqttPacket &p);
    void send(const QByteArray &packet);
    void armKeepAlive();
    void setState(State s);
    void fail(ClientError e);
    void closeSocket(bool graceful);
    void retire(QTcpSocket *s);

    friend class SocketCallback;
};

#endif // MQTTCLIENT_H
