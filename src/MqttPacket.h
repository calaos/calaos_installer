#ifndef MQTTPACKET_H
#define MQTTPACKET_H

#include <QByteArray>
#include <QList>
#include <QString>
#include <QStringList>

// MQTT 3.1.1 packets, built and read without any socket: what MqttClient sends
// and the checks it applies to what a broker sends.
class MqttPacket
{
public:
    enum Type
    {
        CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL, PUBCOMP,
        SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK, PINGREQ, PINGRESP, DISCONNECT
    };

    static constexpr int MAX_REMAINING_LENGTH = 268435455; //4 bytes of 7 bits

    quint8 header = 0;
    QByteArray body;

    int type() const { return header >> 4; }
    int flags() const { return header & 0x0F; }

    //Empty when the length does not fit in 4 bytes.
    static QByteArray encodeRemainingLength(int length);
    static QByteArray encode(quint8 header, const QByteArray &body);

    struct Connect
    {
        QString clientId;
        QString user;
        QString password;
        quint16 keepAlive = 60;
    };
    //Always a clean session: nothing stays on the broker after the installer.
    static QByteArray connect(const Connect &c);
    static QByteArray subscribe(quint16 id, const QStringList &filters);
    //PUBACK, PUBREC, PUBREL or PUBCOMP.
    static QByteArray ack(Type type, quint16 id);
    static QByteArray pingreq();
    static QByteArray disconnect();

    static bool isValidFilter(const QString &filter);

    //What a broker may send to a client, with the reserved flags 3.1.1 requires.
    bool isValidFromBroker() const;

    struct Publish
    {
        QString topic;
        QByteArray payload;
        int qos = 0;
        bool retain = false;
        bool dup = false;
        quint16 id = 0;
    };
    bool readConnack(bool &sessionPresent, int &code) const;
    //codes: 0, 1 or 2 is the granted QoS, 0x80 a refusal.
    bool readSuback(quint16 &id, QList<int> &codes) const;
    bool readPublish(Publish &p) const;
    bool readAck(quint16 &id) const;
};

// Cuts the byte stream of a connection into packets, whatever TCP did to it.
class MqttStreamReader
{
public:
    enum Status { NEED_MORE, PACKET, MALFORMED, TOO_LARGE };

    explicit MqttStreamReader(int maxPacket = 64 * 1024 * 1024);

    void setMaximumPacketSize(int bytes) { m_maxPacket = bytes; }
    void append(const QByteArray &data);
    Status next(MqttPacket &packet);
    void clear();
    int buffered() const { return m_buffer.size() - m_pos; }
    //The unread bytes, before they make a whole packet.
    QByteArray peek(int n) const { return m_buffer.mid(m_pos, n); }

private:
    QByteArray m_buffer;
    int m_pos = 0;
    int m_maxPacket;
};

#endif // MQTTPACKET_H
