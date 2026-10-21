#include "MqttPacket.h"

namespace
{

void appendU16(QByteArray &out, quint16 v)
{
    out.append(char(v >> 8));
    out.append(char(v & 0xFF));
}

quint16 readU16(const QByteArray &b, int at)
{
    return quint16((quint8(b.at(at)) << 8) | quint8(b.at(at + 1)));
}

void appendString(QByteArray &out, const QByteArray &s)
{
    appendU16(out, quint16(s.size()));
    out += s;
}

}

QByteArray MqttPacket::encodeRemainingLength(int length)
{
    if (length < 0 || length > MAX_REMAINING_LENGTH)
        return QByteArray();
    QByteArray out;
    do
    {
        quint8 digit = length % 128;
        length /= 128;
        if (length > 0)
            digit |= 0x80;
        out.append(char(digit));
    }
    while (length > 0);
    return out;
}

QByteArray MqttPacket::encode(quint8 header, const QByteArray &body)
{
    const QByteArray len = encodeRemainingLength(body.size());
    if (len.isEmpty())
        return QByteArray();
    return QByteArray(1, char(header)) + len + body;
}

QByteArray MqttPacket::connect(const Connect &c)
{
    const QByteArray id = c.clientId.toUtf8(), user = c.user.toUtf8(), password = c.password.toUtf8();
    if (id.size() > 0xFFFF || user.size() > 0xFFFF || password.size() > 0xFFFF)
        return QByteArray();

    QByteArray body;
    appendString(body, "MQTT");
    body.append(char(4)); //protocol level of 3.1.1
    quint8 flags = 0x02;
    //3.1.1 allows a password only with a user name.
    const bool withUser = !c.user.isEmpty();
    if (withUser)
        flags |= 0x80;
    if (withUser && !c.password.isEmpty())
        flags |= 0x40;
    body.append(char(flags));
    appendU16(body, c.keepAlive);
    appendString(body, id);
    if (flags & 0x80)
        appendString(body, user);
    if (flags & 0x40)
        appendString(body, password);
    return encode(CONNECT << 4, body);
}

QByteArray MqttPacket::subscribe(quint16 id, const QStringList &filters)
{
    if (id == 0 || filters.isEmpty())
        return QByteArray();
    QByteArray body;
    appendU16(body, id);
    for (const QString &f: filters)
    {
        if (!isValidFilter(f))
            return QByteArray();
        appendString(body, f.toUtf8());
        body.append(char(0)); //QoS 0
    }
    return encode((SUBSCRIBE << 4) | 0x02, body);
}

QByteArray MqttPacket::ack(Type type, quint16 id)
{
    QByteArray body;
    appendU16(body, id);
    return encode((type << 4) | (type == PUBREL ? 0x02 : 0x00), body);
}

QByteArray MqttPacket::pingreq()
{
    return encode(PINGREQ << 4, QByteArray());
}

QByteArray MqttPacket::disconnect()
{
    return encode(DISCONNECT << 4, QByteArray());
}

bool MqttPacket::isValidFilter(const QString &filter)
{
    if (filter.isEmpty() || filter.contains(QChar(0)) || filter.toUtf8().size() > 0xFFFF)
        return false;
    const QStringList levels = filter.split('/');
    for (int i = 0;i < levels.size();i++)
    {
        const QString &l = levels.at(i);
        if (l.contains('#') && (l != "#" || i != levels.size() - 1))
            return false;
        if (l.contains('+') && l != "+")
            return false;
    }
    return true;
}

bool MqttPacket::isValidFromBroker() const
{
    switch (type())
    {
    case PUBLISH:
        return ((flags() >> 1) & 0x03) != 3;
    case PUBREL:
        return flags() == 0x02;
    case CONNACK:
    case PUBACK:
    case PUBREC:
    case PUBCOMP:
    case SUBACK:
    case UNSUBACK:
    case PINGRESP:
        return flags() == 0;
    default:
        return false;
    }
}

bool MqttPacket::readConnack(bool &sessionPresent, int &code) const
{
    if (type() != CONNACK || body.size() != 2 || (quint8(body.at(0)) & 0xFE))
        return false;
    sessionPresent = body.at(0) & 0x01;
    code = quint8(body.at(1));
    return true;
}

bool MqttPacket::readSuback(quint16 &id, QList<int> &codes) const
{
    if (type() != SUBACK || body.size() < 3)
        return false;
    id = readU16(body, 0);
    codes.clear();
    for (int i = 2;i < body.size();i++)
    {
        const int c = quint8(body.at(i));
        if (c > 2 && c != 0x80)
            return false;
        codes << c;
    }
    return true;
}

bool MqttPacket::readPublish(Publish &p) const
{
    if (type() != PUBLISH || body.size() < 2)
        return false;
    p.qos = (flags() >> 1) & 0x03;
    p.retain = flags() & 0x01;
    p.dup = flags() & 0x08;
    if (p.qos == 3)
        return false;
    const int topicLen = readU16(body, 0);
    int pos = 2 + topicLen;
    if (body.size() < pos + (p.qos > 0 ? 2 : 0))
        return false;
    p.topic = QString::fromUtf8(body.mid(2, topicLen));
    //3.1.1 forbids wildcards and empty names in a published topic.
    if (p.topic.isEmpty() || p.topic.contains('+') || p.topic.contains('#'))
        return false;
    p.id = 0;
    if (p.qos > 0)
    {
        p.id = readU16(body, pos);
        pos += 2;
        if (p.id == 0)
            return false;
    }
    p.payload = body.mid(pos);
    return true;
}

bool MqttPacket::readAck(quint16 &id) const
{
    if (body.size() != 2)
        return false;
    id = readU16(body, 0);
    return true;
}

MqttStreamReader::MqttStreamReader(int maxPacket):
    m_maxPacket(maxPacket)
{
}

void MqttStreamReader::append(const QByteArray &data)
{
    //Dropping what was read only now and then keeps many small packets in one read linear.
    if (m_pos > 0 && m_pos >= m_buffer.size() / 2)
    {
        m_buffer.remove(0, m_pos);
        m_pos = 0;
    }
    m_buffer += data;
}

void MqttStreamReader::clear()
{
    m_buffer.clear();
    m_pos = 0;
}

MqttStreamReader::Status MqttStreamReader::next(MqttPacket &packet)
{
    const int avail = m_buffer.size() - m_pos;
    if (avail < 2)
        return NEED_MORE;

    int len = 0, multiplier = 1, used = 0;
    while (true)
    {
        if (used == 4)
            return MALFORMED;
        if (1 + used >= avail)
            return NEED_MORE;
        const quint8 digit = quint8(m_buffer.at(m_pos + 1 + used));
        used++;
        len += (digit & 0x7F) * multiplier;
        multiplier *= 128;
        if (!(digit & 0x80))
            break;
    }
    if (len > m_maxPacket)
        return TOO_LARGE;
    if (avail < 1 + used + len)
        return NEED_MORE;

    packet.header = quint8(m_buffer.at(m_pos));
    packet.body = m_buffer.mid(m_pos + 1 + used, len);
    m_pos += 1 + used + len;
    if (m_pos == m_buffer.size())
        clear();
    return PACKET;
}
