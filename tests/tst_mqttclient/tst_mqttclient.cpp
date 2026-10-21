#include <QtTest>
#include <QProcess>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QTcpServer>
#include <QTcpSocket>

#include "FakeMqttBroker.h"
#include "MqttClient.h"
#include "MqttPacket.h"

namespace
{

QByteArray bytes(std::initializer_list<int> l)
{
    QByteArray b;
    for (int c: l)
        b.append(char(c));
    return b;
}

QByteArray publishPacket(const QString &topic, const QByteArray &payload, quint8 flags = 0, quint16 id = 0)
{
    const QByteArray t = topic.toUtf8();
    QByteArray body = bytes({ int(t.size() >> 8), int(t.size() & 0xFF) }) + t;
    if ((flags >> 1) & 0x03)
        body += bytes({ id >> 8, id & 0xFF });
    return MqttPacket::encode(0x30 | flags, body + payload);
}

MqttPacket packetOf(quint8 header, const QByteArray &body)
{
    MqttPacket p;
    p.header = header;
    p.body = body;
    return p;
}

//What a client saw during a test.
struct Seen
{
    QList<MqttClient::State> states;
    QList<MqttClient::ClientError> errors;
    QList<QPair<quint16, QList<bool>>> subacks;
    struct Message { QString topic; QByteArray payload; bool retained; };
    QList<Message> messages;
    int connected = 0;
    int disconnected = 0;

    void watch(MqttClient &c)
    {
        QObject::connect(&c, &MqttClient::stateChanged, [this](MqttClient::State s) { states << s; });
        QObject::connect(&c, &MqttClient::errorOccurred, [this](MqttClient::ClientError e) { errors << e; });
        QObject::connect(&c, &MqttClient::connected, [this]() { connected++; });
        QObject::connect(&c, &MqttClient::disconnected, [this]() { disconnected++; });
        QObject::connect(&c, &MqttClient::subscribed, [this](quint16 id, const QList<bool> &g) { subacks << qMakePair(id, g); });
        QObject::connect(&c, &MqttClient::messageReceived, [this](const QString &t, const QByteArray &p, bool r)
        {
            messages << Message{ t, p, r };
        });
    }
};

void aim(MqttClient &c, quint16 port, const QString &user = QString(), const QString &password = QString())
{
    c.setHost("127.0.0.1");
    c.setPort(port);
    c.setUser(user);
    c.setPassword(password);
}

//A broker that answers anything with fixed bytes, in one write.
class ScriptedBroker: public QTcpServer
{
public:
    QByteArray answer;
    bool closeAfterAnswer = false;
    QByteArray received;

    quint16 start()
    {
        connect(this, &QTcpServer::newConnection, this, [this]()
        {
            QTcpSocket *s = nextPendingConnection();
            connect(s, &QTcpSocket::readyRead, s, [this, s]()
            {
                received += s->readAll();
                if (answer.isEmpty())
                    return;
                s->write(answer);
                answer.clear();
                if (closeAfterAnswer)
                    s->disconnectFromHost();
            });
        });
        listen(QHostAddress::LocalHost, 0);
        return serverPort();
    }
};

}

class tst_MqttClient: public QObject
{
    Q_OBJECT

private slots:
    void remainingLengthIsEncoded_data();
    void remainingLengthIsEncoded();
    void aFifthLengthByteIsMalformed();
    void theConnectPacketIsExact();
    void aPasswordGoesOnlyWithAUser();
    void theSubscribePacketIsExact();
    void invalidFiltersAreRefused_data();
    void invalidFiltersAreRefused();
    void acksCarryTheirReservedFlags();
    void packetsAreReadWhateverTheSplit();
    void aLargePayloadIsRead();
    void aPacketOverTheLimitIsRefusedBeforeItsBody();
    void publishIsRead();
    void malformedPublishIsRefused();
    void subackCodesAreRead();
    void connackIsRead();
    void flagsFromTheBrokerAreChecked();

    void connectsSubscribesAndReceives();
    void retainedMessagesAreFlagged();
    void bytesOneByOneAreRead();
    void packetsInOneWriteAreRead();
    void aLargeMessageArrivesWhole();
    void aMessageOverTheLimitEndsTheConnection();
    void qos1IsAcknowledged();
    void qos2IsAcknowledgedOnce();
    void connackRefusalsAreReported_data();
    void connackRefusalsAreReported();
    void aRefusalFollowedByACloseIsARefusal();
    void subackSaysWhichFiltersAreRefused();
    void pingsKeepTheConnection();
    void aBrokerThatStopsAnsweringPingsIsLeft();
    void aSilentBrokerTimesOut();
    void aCloseBeforeConnackIsSaid();
    void somethingElseThanMqttIsSaidAtOnce();
    void aSecondConnackIsAViolation();
    void disconnectSendsDisconnect();
    void subscribingNeedsAConnection();
    void theClientConnectsAgain();
    void anUnreachableBrokerIsASocketError();
    void stoppingWhileConnectingLeavesNoSocket();
    void aDestroyedClientSaysDisconnect();
    void aSlotMayRunANestedEventLoop();

    void aRealMosquittoIsRead();
};

void tst_MqttClient::remainingLengthIsEncoded_data()
{
    QTest::addColumn<int>("length");
    QTest::addColumn<QByteArray>("encoded");
    QTest::newRow("0") << 0 << bytes({ 0x00 });
    QTest::newRow("127") << 127 << bytes({ 0x7F });
    QTest::newRow("128") << 128 << bytes({ 0x80, 0x01 });
    QTest::newRow("16383") << 16383 << bytes({ 0xFF, 0x7F });
    QTest::newRow("16384") << 16384 << bytes({ 0x80, 0x80, 0x01 });
    QTest::newRow("2097151") << 2097151 << bytes({ 0xFF, 0xFF, 0x7F });
    QTest::newRow("2097152") << 2097152 << bytes({ 0x80, 0x80, 0x80, 0x01 });
    QTest::newRow("max") << MqttPacket::MAX_REMAINING_LENGTH << bytes({ 0xFF, 0xFF, 0xFF, 0x7F });
    QTest::newRow("max + 1") << MqttPacket::MAX_REMAINING_LENGTH + 1 << QByteArray();
    QTest::newRow("negative") << -1 << QByteArray();
}

void tst_MqttClient::remainingLengthIsEncoded()
{
    QFETCH(int, length);
    QFETCH(QByteArray, encoded);
    QCOMPARE(MqttPacket::encodeRemainingLength(length), encoded);
    if (encoded.isEmpty())
        return;

    //The reader decodes the same length: it waits for exactly that many bytes.
    MqttStreamReader r(MqttPacket::MAX_REMAINING_LENGTH);
    r.append(QByteArray(1, char(0x30)) + encoded);
    MqttPacket p;
    if (length == 0)
    {
        QCOMPARE(r.next(p), MqttStreamReader::PACKET);
        QVERIFY(p.body.isEmpty());
        return;
    }
    QCOMPARE(r.next(p), MqttStreamReader::NEED_MORE);
    if (length <= 16384)
    {
        r.append(QByteArray(length - 1, 'x'));
        QCOMPARE(r.next(p), MqttStreamReader::NEED_MORE);
        r.append("y");
        QCOMPARE(r.next(p), MqttStreamReader::PACKET);
        QCOMPARE(p.body.size(), length);
        QVERIFY(p.body.endsWith('y'));
    }
    MqttStreamReader small(length - 1);
    small.append(QByteArray(1, char(0x30)) + encoded);
    QCOMPARE(small.next(p), MqttStreamReader::TOO_LARGE);
}

void tst_MqttClient::aFifthLengthByteIsMalformed()
{
    MqttStreamReader r;
    MqttPacket p;
    r.append(bytes({ 0x30, 0xFF, 0xFF, 0xFF }));
    QCOMPARE(r.next(p), MqttStreamReader::NEED_MORE);
    r.append(bytes({ 0xFF }));
    QCOMPARE(r.next(p), MqttStreamReader::MALFORMED);
}

void tst_MqttClient::theConnectPacketIsExact()
{
    MqttPacket::Connect c;
    c.clientId = "id";
    c.user = "u";
    c.password = "pw";
    c.keepAlive = 300;
    const QByteArray expected = bytes({ 0x10, 21,
                                        0, 4, 'M', 'Q', 'T', 'T', 4,
                                        0xC2, //user, password, clean session
                                        0x01, 0x2C,
                                        0, 2, 'i', 'd',
                                        0, 1, 'u',
                                        0, 2, 'p', 'w' });
    QCOMPARE(MqttPacket::connect(c), expected);
}

void tst_MqttClient::aPasswordGoesOnlyWithAUser()
{
    MqttPacket::Connect c;
    c.clientId = "id";
    c.password = "orphan";
    QByteArray p = MqttPacket::connect(c);
    QCOMPARE(quint8(p.at(9)), quint8(0x02));
    QVERIFY(!p.contains("orphan"));

    c.user = "u";
    c.password.clear();
    p = MqttPacket::connect(c);
    QCOMPARE(quint8(p.at(9)), quint8(0x82));
    QVERIFY(p.endsWith(bytes({ 0, 1, 'u' })));
}

void tst_MqttClient::theSubscribePacketIsExact()
{
    QCOMPARE(MqttPacket::subscribe(0x0102, { "a/+", "#" }),
             bytes({ 0x82, 12, 0x01, 0x02, 0, 3, 'a', '/', '+', 0, 0, 1, '#', 0 }));
    QCOMPARE(MqttPacket::subscribe(0, { "a" }), QByteArray());
    QCOMPARE(MqttPacket::subscribe(1, {}), QByteArray());
}

void tst_MqttClient::invalidFiltersAreRefused_data()
{
    QTest::addColumn<QString>("filter");
    QTest::addColumn<bool>("valid");
    QTest::newRow("plain") << "a/b" << true;
    QTest::newRow("hash") << "#" << true;
    QTest::newRow("plus everywhere") << "+/+/#" << true;
    QTest::newRow("empty levels") << "/a//b/" << true;
    QTest::newRow("empty") << "" << false;
    QTest::newRow("hash not last") << "a/#/b" << false;
    QTest::newRow("hash in a level") << "a/b#" << false;
    QTest::newRow("plus in a level") << "a/b+/c" << false;
    QTest::newRow("nul") << QString("a") + QChar(0) << false;
}

void tst_MqttClient::invalidFiltersAreRefused()
{
    QFETCH(QString, filter);
    QFETCH(bool, valid);
    QCOMPARE(MqttPacket::isValidFilter(filter), valid);
    QCOMPARE(MqttPacket::subscribe(1, { "ok", filter }).isEmpty(), !valid);
}

void tst_MqttClient::acksCarryTheirReservedFlags()
{
    QCOMPARE(MqttPacket::ack(MqttPacket::PUBACK, 0x1234), bytes({ 0x40, 2, 0x12, 0x34 }));
    QCOMPARE(MqttPacket::ack(MqttPacket::PUBREC, 7), bytes({ 0x50, 2, 0, 7 }));
    QCOMPARE(MqttPacket::ack(MqttPacket::PUBREL, 7), bytes({ 0x62, 2, 0, 7 }));
    QCOMPARE(MqttPacket::ack(MqttPacket::PUBCOMP, 7), bytes({ 0x70, 2, 0, 7 }));
    QCOMPARE(MqttPacket::pingreq(), bytes({ 0xC0, 0 }));
    QCOMPARE(MqttPacket::disconnect(), bytes({ 0xE0, 0 }));
}

void tst_MqttClient::packetsAreReadWhateverTheSplit()
{
    const QByteArray big(300, 'b');
    const QByteArray stream = bytes({ 0x20, 2, 0, 0 }) + bytes({ 0x90, 3, 0, 1, 0 })
                              + publishPacket("t/1", "one") + publishPacket("t/2", big) + bytes({ 0xD0, 0 });
    const QList<int> types = { MqttPacket::CONNACK, MqttPacket::SUBACK, MqttPacket::PUBLISH,
                               MqttPacket::PUBLISH, MqttPacket::PINGRESP };

    for (int chunk: { 1, 2, 3, 5, 7, 64, int(stream.size()) })
    {
        MqttStreamReader r;
        QList<MqttPacket> got;
        for (int i = 0;i < stream.size();i += chunk)
        {
            r.append(stream.mid(i, chunk));
            MqttPacket p;
            MqttStreamReader::Status s;
            while ((s = r.next(p)) == MqttStreamReader::PACKET)
                got << p;
            QCOMPARE(s, MqttStreamReader::NEED_MORE);
        }
        QCOMPARE(got.size(), types.size());
        for (int i = 0;i < got.size();i++)
            QCOMPARE(got.at(i).type(), types.at(i));
        MqttPacket::Publish m;
        QVERIFY(got[3].takePublish(m));
        QCOMPARE(m.topic, QString("t/2"));
        QCOMPARE(m.payload, big);
        QCOMPARE(r.buffered(), 0);
    }
}

void tst_MqttClient::aLargePayloadIsRead()
{
    //Over 2 MiB: a four-byte remaining length.
    QByteArray payload(3 * 1024 * 1024, Qt::Uninitialized);
    for (int i = 0;i < payload.size();i++)
        payload[i] = char(i * 7);
    const QByteArray packet = publishPacket("zigbee2mqtt/bridge/devices", payload, 0x01);
    QCOMPARE(quint8(packet.at(4)) & 0x80, 0);
    QCOMPARE(quint8(packet.at(3)) & 0x80, 0x80);

    MqttStreamReader r;
    MqttPacket p;
    for (int i = 0;i < packet.size();i += 65536)
    {
        QCOMPARE(r.next(p), MqttStreamReader::NEED_MORE);
        r.append(packet.mid(i, 65536));
    }
    QCOMPARE(r.next(p), MqttStreamReader::PACKET);
    MqttPacket::Publish m;
    QVERIFY(p.takePublish(m));
    QVERIFY(m.retain);
    QCOMPARE(m.payload.size(), payload.size());
    QVERIFY(m.payload == payload);
}

void tst_MqttClient::aPacketOverTheLimitIsRefusedBeforeItsBody()
{
    MqttStreamReader r(1000);
    MqttPacket p;
    r.append(MqttPacket::encode(0x30, QByteArray(1000, 'x')));
    QCOMPARE(r.next(p), MqttStreamReader::PACKET);
    r.append(bytes({ 0x30 }) + MqttPacket::encodeRemainingLength(1001));
    QCOMPARE(r.next(p), MqttStreamReader::TOO_LARGE);
}

void tst_MqttClient::publishIsRead()
{
    MqttPacket::Publish m;
    MqttStreamReader r;
    MqttPacket p;

    r.append(publishPacket("a/b", "x", 0x01));
    QCOMPARE(r.next(p), MqttStreamReader::PACKET);
    QVERIFY(p.takePublish(m));
    QCOMPARE(m.qos, 0);
    QVERIFY(m.retain);
    QCOMPARE(m.id, quint16(0));

    r.append(publishPacket("a/b", "", 0x02, 0x0A0B));
    QCOMPARE(r.next(p), MqttStreamReader::PACKET);
    QVERIFY(p.takePublish(m));
    QCOMPARE(m.qos, 1);
    QVERIFY(!m.retain);
    QCOMPARE(m.id, quint16(0x0A0B));
    QVERIFY(m.payload.isEmpty());

    r.append(publishPacket("é/ü", "y", 0x0C, 9));
    QCOMPARE(r.next(p), MqttStreamReader::PACKET);
    QVERIFY(p.takePublish(m));
    QCOMPARE(m.qos, 2);
    QVERIFY(m.dup);
    QCOMPARE(m.topic, QString("é/ü"));
    QCOMPARE(m.payload, QByteArray("y"));
}

void tst_MqttClient::malformedPublishIsRefused()
{
    MqttPacket::Publish m;
    QVERIFY(!packetOf(0x30, bytes({ 0 })).takePublish(m));
    QVERIFY(!packetOf(0x30, bytes({ 0, 5, 'a' })).takePublish(m));
    QVERIFY(!packetOf(0x30, bytes({ 0, 0, 'x' })).takePublish(m));
    QVERIFY(!packetOf(0x30, bytes({ 0, 3, 'a', '/', '#' })).takePublish(m));
    QVERIFY(!packetOf(0x30, bytes({ 0, 1, '+' })).takePublish(m));
    QVERIFY(!packetOf(0x36, bytes({ 0, 1, 'a', 0, 1 })).takePublish(m));
    //QoS 1 without packet id, or with id 0.
    QVERIFY(!packetOf(0x32, bytes({ 0, 1, 'a', 0 })).takePublish(m));
    QVERIFY(!packetOf(0x32, bytes({ 0, 1, 'a', 0, 0 })).takePublish(m));
}

void tst_MqttClient::subackCodesAreRead()
{
    quint16 id = 0;
    QList<int> codes;
    QVERIFY(packetOf(0x90, bytes({ 0, 9, 0x00, 0x01, 0x02, 0x80 })).readSuback(id, codes));
    QCOMPARE(id, quint16(9));
    QCOMPARE(codes, QList<int>({ 0, 1, 2, 0x80 }));
    QVERIFY(!packetOf(0x90, bytes({ 0, 9, 0x03 })).readSuback(id, codes));
    QVERIFY(!packetOf(0x90, bytes({ 0, 9 })).readSuback(id, codes));
}

void tst_MqttClient::connackIsRead()
{
    bool sp = true;
    int code = -1;
    QVERIFY(packetOf(0x20, bytes({ 0, 0 })).readConnack(sp, code));
    QVERIFY(!sp);
    QCOMPARE(code, 0);
    QVERIFY(packetOf(0x20, bytes({ 1, 5 })).readConnack(sp, code));
    QVERIFY(sp);
    QCOMPARE(code, 5);
    QVERIFY(!packetOf(0x20, bytes({ 2, 0 })).readConnack(sp, code));
    QVERIFY(!packetOf(0x20, bytes({ 0 })).readConnack(sp, code));
    QVERIFY(!packetOf(0x20, bytes({ 0, 0, 0 })).readConnack(sp, code));
}

void tst_MqttClient::flagsFromTheBrokerAreChecked()
{
    QVERIFY(packetOf(0x20, {}).isValidFromBroker());
    QVERIFY(!packetOf(0x21, {}).isValidFromBroker());
    QVERIFY(packetOf(0x3D, {}).isValidFromBroker());
    QVERIFY(!packetOf(0x36, {}).isValidFromBroker());
    QVERIFY(packetOf(0x62, {}).isValidFromBroker());
    QVERIFY(!packetOf(0x60, {}).isValidFromBroker());
    QVERIFY(!packetOf(0x92, {}).isValidFromBroker());
    QVERIFY(packetOf(0xD0, {}).isValidFromBroker());
    //Packets only a client sends.
    for (int type: { 0, 1, 8, 10, 12, 14, 15 })
        QVERIFY2(!packetOf(type << 4, {}).isValidFromBroker(), qPrintable(QString::number(type)));
    QVERIFY(!packetOf(0x82, {}).isValidFromBroker());
}

void tst_MqttClient::connectsSubscribesAndReceives()
{
    FakeMqttBroker broker;
    broker.requiredUser = "calaos";
    broker.requiredPassword = "s3cret";
    broker.retained << qMakePair(QString("esphome/discover/node"), QByteArray("hello"));
    broker.retained << qMakePair(QString("other/topic"), QByteArray("no"));

    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start(), "calaos", "s3cret");
    quint16 id = 0;
    connect(&c, &MqttClient::connected, this, [&]() { id = c.subscribe({ "esphome/#", "homeassistant/+/+/config" }); });
    c.connectToHost();
    QCOMPARE(c.state(), MqttClient::CONNECTING);
    QTRY_COMPARE(seen.subacks.size(), 1);

    QCOMPARE(seen.connected, 1);
    QCOMPARE(c.state(), MqttClient::CONNECTED);
    QCOMPARE(seen.states, QList<MqttClient::State>({ MqttClient::CONNECTING, MqttClient::CONNECTED }));
    QVERIFY(id != 0);
    QCOMPARE(seen.subacks.first().first, id);
    QCOMPARE(seen.subacks.first().second, QList<bool>({ true, true }));
    QCOMPARE(broker.user, QString("calaos"));
    QCOMPARE(broker.password, QString("s3cret"));
    QVERIFY(broker.cleanSession);
    QCOMPARE(broker.keepAlive, 60);
    QVERIFY(broker.clientId.startsWith("calaosinst"));
    QVERIFY(broker.clientId.size() <= 23);
    for (const QChar ch: std::as_const(broker.clientId))
        QVERIFY(ch.isLetterOrNumber() && ch.unicode() < 128);
    QCOMPARE(c.clientId(), broker.clientId);
    QCOMPARE(broker.filters, QStringList({ "esphome/#", "homeassistant/+/+/config" }));

    QTRY_COMPARE(seen.messages.size(), 1);
    QCOMPARE(seen.messages.first().topic, QString("esphome/discover/node"));
    QCOMPARE(seen.messages.first().payload, QByteArray("hello"));

    broker.publish("homeassistant/climate/x/config", "{}");
    QTRY_COMPARE(seen.messages.size(), 2);
    QCOMPARE(seen.messages.last().topic, QString("homeassistant/climate/x/config"));
    QVERIFY(seen.errors.isEmpty());
}

void tst_MqttClient::retainedMessagesAreFlagged()
{
    FakeMqttBroker broker;
    broker.retained << qMakePair(QString("a/b"), QByteArray("kept"));
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    connect(&c, &MqttClient::connected, this, [&]() { c.subscribe({ "a/#" }); });
    c.connectToHost();
    QTRY_COMPARE(seen.messages.size(), 1);
    QVERIFY(seen.messages.first().retained);
    broker.publish("a/c", "live");
    QTRY_COMPARE(seen.messages.size(), 2);
    QVERIFY(!seen.messages.last().retained);
}

void tst_MqttClient::bytesOneByOneAreRead()
{
    FakeMqttBroker broker;
    broker.byteByByte = true;
    broker.retained << qMakePair(QString("a/1"), QByteArray(200, 'x'));
    broker.retained << qMakePair(QString("a/2"), QByteArray("y"));
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    connect(&c, &MqttClient::connected, this, [&]() { c.subscribe({ "a/+" }); });
    c.connectToHost();
    QTRY_COMPARE(seen.messages.size(), 2);
    QCOMPARE(seen.messages.at(0).payload, QByteArray(200, 'x'));
    QCOMPARE(seen.messages.at(1).payload, QByteArray("y"));
    QCOMPARE(seen.subacks.size(), 1);
    QVERIFY(seen.errors.isEmpty());
}

void tst_MqttClient::packetsInOneWriteAreRead()
{
    //CONNACK, the SUBACK of the subscription made on connected(), messages and a
    //PINGRESP, all in the broker's first answer.
    ScriptedBroker broker;
    broker.answer = bytes({ 0x20, 2, 0, 0 }) + bytes({ 0x90, 3, 0, 1, 0 })
                    + publishPacket("a/1", "one", 0x01) + publishPacket("a/2", QByteArray(5000, 'z'))
                    + bytes({ 0xD0, 0 });
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    connect(&c, &MqttClient::connected, this, [&]() { QCOMPARE(c.subscribe({ "a/+" }), quint16(1)); });
    c.connectToHost();
    QTRY_COMPARE(seen.messages.size(), 2);
    QCOMPARE(seen.subacks.size(), 1);
    QCOMPARE(seen.messages.at(0).topic, QString("a/1"));
    QVERIFY(seen.messages.at(0).retained);
    QCOMPARE(seen.messages.at(1).payload.size(), 5000);
    QVERIFY(seen.errors.isEmpty());
    QCOMPARE(c.state(), MqttClient::CONNECTED);
}

void tst_MqttClient::aLargeMessageArrivesWhole()
{
    FakeMqttBroker broker;
    QByteArray devices(3 * 1024 * 1024, Qt::Uninitialized);
    for (int i = 0;i < devices.size();i++)
        devices[i] = char('a' + i % 26);
    broker.retained << qMakePair(QString("zigbee2mqtt/bridge/devices"), devices);
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    connect(&c, &MqttClient::connected, this, [&]() { c.subscribe({ "zigbee2mqtt/bridge/devices" }); });
    c.connectToHost();
    QTRY_COMPARE_WITH_TIMEOUT(seen.messages.size(), 1, 10000);
    QVERIFY(seen.messages.first().payload == devices);
}

void tst_MqttClient::aMessageOverTheLimitEndsTheConnection()
{
    FakeMqttBroker broker;
    broker.retained << qMakePair(QString("a"), QByteArray(2000, 'x'));
    MqttClient c;
    Seen seen;
    seen.watch(c);
    c.setMaximumPacketSize(1000);
    aim(c, broker.start());
    connect(&c, &MqttClient::connected, this, [&]() { c.subscribe({ "#" }); });
    c.connectToHost();
    QTRY_COMPARE(seen.errors.size(), 1);
    QCOMPARE(seen.errors.first(), MqttClient::PACKET_TOO_LARGE);
    QCOMPARE(c.error(), MqttClient::PACKET_TOO_LARGE);
    QCOMPARE(seen.disconnected, 1);
    QVERIFY(seen.messages.isEmpty());
}

void tst_MqttClient::qos1IsAcknowledged()
{
    FakeMqttBroker broker;
    broker.deliverAtQos1 = true;
    broker.retained << qMakePair(QString("a/1"), QByteArray("1")) << qMakePair(QString("a/2"), QByteArray("2"));
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    connect(&c, &MqttClient::connected, this, [&]() { c.subscribe({ "a/+" }); });
    c.connectToHost();
    QTRY_COMPARE(seen.messages.size(), 2);
    QTRY_COMPARE(broker.pubacks, 2);
}

void tst_MqttClient::qos2IsAcknowledgedOnce()
{
    FakeMqttBroker broker;
    broker.deliverAtQos2 = true;
    broker.duplicateQos2 = true;
    broker.retained << qMakePair(QString("a/1"), QByteArray("1")) << qMakePair(QString("a/2"), QByteArray("2"));
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    connect(&c, &MqttClient::connected, this, [&]() { c.subscribe({ "a/+" }); });
    c.connectToHost();
    QTRY_COMPARE(broker.pubcomps, 2);
    //Each delivery answered, each message seen once.
    QCOMPARE(broker.pubrecs, 4);
    QCOMPARE(seen.messages.size(), 2);
    QVERIFY(seen.errors.isEmpty());
}

void tst_MqttClient::connackRefusalsAreReported_data()
{
    QTest::addColumn<int>("code");
    for (int code: { 1, 2, 3, 4, 5, 0x42 })
        QTest::newRow(qPrintable(QString::number(code))) << code;
}

void tst_MqttClient::connackRefusalsAreReported()
{
    QFETCH(int, code);
    FakeMqttBroker broker;
    broker.connackCode = code;
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    c.connectToHost();
    QTRY_COMPARE(seen.disconnected, 1);
    QCOMPARE(seen.errors, QList<MqttClient::ClientError>({ MqttClient::CONNECTION_REFUSED }));
    QCOMPARE(c.connackCode(), code);
    QCOMPARE(seen.connected, 0);
    QCOMPARE(c.state(), MqttClient::UNCONNECTED);
    QVERIFY(!c.failedBeforeConnack());
}

void tst_MqttClient::aRefusalFollowedByACloseIsARefusal()
{
    FakeMqttBroker broker;
    broker.requiredUser = "calaos";
    broker.requiredPassword = "s3cret";
    broker.closeAfterConnack = true;
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start(), "calaos", "wrong");
    c.connectToHost();
    QTRY_COMPARE(seen.disconnected, 1);
    QCOMPARE(seen.errors, QList<MqttClient::ClientError>({ MqttClient::CONNECTION_REFUSED }));
    QCOMPARE(c.connackCode(), 4);
}

void tst_MqttClient::subackSaysWhichFiltersAreRefused()
{
    FakeMqttBroker broker;
    broker.refusedFilters << "secret/#";
    broker.retained << qMakePair(QString("secret/x"), QByteArray("no")) << qMakePair(QString("open/x"), QByteArray("yes"));
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    connect(&c, &MqttClient::connected, this, [&]() { c.subscribe({ "open/#", "secret/#" }); });
    c.connectToHost();
    QTRY_COMPARE(seen.subacks.size(), 1);
    QCOMPARE(seen.subacks.first().second, QList<bool>({ true, false }));
    QTRY_COMPARE(seen.messages.size(), 1);
    QCOMPARE(seen.messages.first().topic, QString("open/x"));

    //All refused: still a SUBACK, not an error.
    broker.refuseSubscriptions = true;
    c.subscribe({ "a", "b" });
    QTRY_COMPARE(seen.subacks.size(), 2);
    QCOMPARE(seen.subacks.last().second, QList<bool>({ false, false }));
    QCOMPARE(c.state(), MqttClient::CONNECTED);
}

void tst_MqttClient::pingsKeepTheConnection()
{
    FakeMqttBroker broker;
    MqttClient c;
    Seen seen;
    seen.watch(c);
    c.setKeepAlive(2);
    aim(c, broker.start());
    c.connectToHost();
    QTRY_COMPARE(seen.connected, 1);
    QCOMPARE(broker.keepAlive, 2);
    QTest::qWait(5000);
    QVERIFY2(broker.pings >= 2, qPrintable(QString::number(broker.pings)));
    QCOMPARE(c.state(), MqttClient::CONNECTED);
    QVERIFY(seen.errors.isEmpty());

    //No keep alive, no ping.
    FakeMqttBroker quiet;
    MqttClient q;
    q.setKeepAlive(0);
    aim(q, quiet.start());
    q.connectToHost();
    QTRY_COMPARE(q.state(), MqttClient::CONNECTED);
    QTest::qWait(1500);
    QCOMPARE(quiet.pings, 0);
    QCOMPARE(quiet.keepAlive, 0);
}

void tst_MqttClient::aBrokerThatStopsAnsweringPingsIsLeft()
{
    FakeMqttBroker broker;
    broker.answerPings = false;
    MqttClient c;
    Seen seen;
    seen.watch(c);
    c.setKeepAlive(1);
    aim(c, broker.start());
    c.connectToHost();
    QTRY_COMPARE(seen.connected, 1);
    QElapsedTimer t;
    t.start();
    QTRY_COMPARE_WITH_TIMEOUT(seen.errors.size(), 1, 5000);
    QCOMPARE(seen.errors.first(), MqttClient::PING_TIMED_OUT);
    QVERIFY(t.elapsed() >= 1500);
    QCOMPARE(broker.pings, 1);
    QCOMPARE(seen.disconnected, 1);
}

void tst_MqttClient::aSilentBrokerTimesOut()
{
    FakeMqttBroker broker;
    broker.answerConnect = false;
    MqttClient c;
    Seen seen;
    seen.watch(c);
    c.setConnectTimeout(300);
    aim(c, broker.start());
    QElapsedTimer t;
    t.start();
    c.connectToHost();
    QTRY_COMPARE(seen.errors.size(), 1);
    QCOMPARE(seen.errors.first(), MqttClient::CONNECT_TIMED_OUT);
    QVERIFY(t.elapsed() >= 250);
    QCOMPARE(broker.connects, 1);
    QVERIFY(c.failedBeforeConnack());
}

void tst_MqttClient::aCloseBeforeConnackIsSaid()
{
    FakeMqttBroker broker;
    broker.closeAfterConnect = true;
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    c.connectToHost();
    QTRY_COMPARE(seen.errors.size(), 1);
    QCOMPARE(seen.errors.first(), MqttClient::SOCKET_FAILED);
    QCOMPARE(c.socketError(), QAbstractSocket::RemoteHostClosedError);
    QVERIFY(c.failedBeforeConnack());
    QCOMPARE(seen.disconnected, 1);
}

void tst_MqttClient::somethingElseThanMqttIsSaidAtOnce()
{
    //An HTTP server: its answer is not waited for as if 'H' started a packet.
    ScriptedBroker http;
    http.answer = "HTTP/1.1 400 Bad Request\r\n\r\n";
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, http.start());
    c.connectToHost();
    QTRY_COMPARE_WITH_TIMEOUT(seen.errors.size(), 1, 1000);
    QCOMPARE(seen.errors.first(), MqttClient::PROTOCOL_VIOLATION);

    //A CONNACK of the wrong length is not waited for either.
    ScriptedBroker longer;
    longer.answer = bytes({ 0x20, 3 });
    MqttClient l;
    Seen seenLonger;
    seenLonger.watch(l);
    aim(l, longer.start());
    l.connectToHost();
    QTRY_COMPARE_WITH_TIMEOUT(seenLonger.errors.size(), 1, 1000);
    QCOMPARE(seenLonger.errors.first(), MqttClient::PROTOCOL_VIOLATION);
}

void tst_MqttClient::aSecondConnackIsAViolation()
{
    ScriptedBroker broker;
    broker.answer = bytes({ 0x20, 2, 0, 0, 0x20, 2, 0, 0 });
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    c.connectToHost();
    QTRY_COMPARE(seen.errors.size(), 1);
    QCOMPARE(seen.connected, 1);
    QCOMPARE(seen.errors.first(), MqttClient::PROTOCOL_VIOLATION);
}

void tst_MqttClient::disconnectSendsDisconnect()
{
    FakeMqttBroker broker;
    MqttClient *c = new MqttClient;
    Seen seen;
    seen.watch(*c);
    aim(*c, broker.start());
    c->connectToHost();
    QTRY_COMPARE(seen.connected, 1);
    c->disconnectFromHost();
    QCOMPARE(c->state(), MqttClient::UNCONNECTED);
    QCOMPARE(seen.disconnected, 1);
    QVERIFY(seen.errors.isEmpty());
    //Deleted right away, as MqttListener does: the DISCONNECT still goes out.
    delete c;
    QTRY_COMPARE(broker.disconnects, 1);
}

void tst_MqttClient::subscribingNeedsAConnection()
{
    FakeMqttBroker broker;
    MqttClient c;
    QCOMPARE(c.subscribe({ "a" }), quint16(0));
    aim(c, broker.start());
    c.connectToHost();
    QCOMPARE(c.subscribe({ "a" }), quint16(0));
    QTRY_COMPARE(c.state(), MqttClient::CONNECTED);
    QCOMPARE(c.subscribe({ "a/#/b" }), quint16(0));
    const quint16 first = c.subscribe({ "a" });
    const quint16 second = c.subscribe({ "b" });
    QVERIFY(first != 0 && second != 0 && first != second);
    QTRY_COMPARE(broker.filters, QStringList({ "a", "b" }));
}

void tst_MqttClient::theClientConnectsAgain()
{
    FakeMqttBroker broker;
    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, broker.start());
    c.connectToHost();
    QTRY_COMPARE(seen.connected, 1);
    const QString firstId = c.clientId();
    c.disconnectFromHost();
    QTRY_COMPARE(broker.disconnects, 1);
    c.connectToHost();
    QTRY_COMPARE(seen.connected, 2);
    QCOMPARE(broker.connects, 2);
    QVERIFY(c.clientId() != firstId);

    c.setClientId("fixed");
    c.connectToHost();
    QTRY_COMPARE(seen.connected, 3);
    QCOMPARE(broker.clientId, QString("fixed"));
}

void tst_MqttClient::anUnreachableBrokerIsASocketError()
{
    QTcpServer closed;
    QVERIFY(closed.listen(QHostAddress::LocalHost, 0));
    const quint16 port = closed.serverPort();
    closed.close();

    MqttClient c;
    Seen seen;
    seen.watch(c);
    aim(c, port);
    c.connectToHost();
    QTRY_COMPARE(seen.errors.size(), 1);
    QCOMPARE(seen.errors.first(), MqttClient::SOCKET_FAILED);
    QCOMPARE(c.socketError(), QAbstractSocket::ConnectionRefusedError);
    QVERIFY(!c.failedBeforeConnack());
    QCOMPARE(seen.disconnected, 1);
    QCOMPARE(seen.states, QList<MqttClient::State>({ MqttClient::CONNECTING, MqttClient::UNCONNECTED }));
}

namespace
{

int sockets(const MqttClient &c)
{
    //What deleteLater() left is deleted here, as by the event loop.
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    return int(c.findChildren<QTcpSocket *>(QString(), Qt::FindDirectChildrenOnly).size());
}

}

void tst_MqttClient::stoppingWhileConnectingLeavesNoSocket()
{
    QTcpServer closed;
    QVERIFY(closed.listen(QHostAddress::LocalHost, 0));
    const quint16 refused = closed.serverPort();
    closed.close();

    //A refused port, and TEST-NET-1 where nothing answers (or the route is refused at once).
    for (const QString &host: { QString("127.0.0.1"), QString("192.0.2.1") })
    {
        MqttClient c;
        c.setHost(host);
        c.setPort(host == "127.0.0.1" ? refused : 1883);
        for (int i = 0;i < 5;i++)
        {
            c.connectToHost();
            QTest::qWait(i * 10);
            c.disconnectFromHost();
        }
        QTest::qWait(200);
        QCOMPARE(sockets(c), 0);

        //The same after a connect timeout.
        c.setConnectTimeout(100);
        c.connectToHost();
        QTRY_COMPARE(c.state(), MqttClient::UNCONNECTED);
        QTest::qWait(100);
        QCOMPARE(sockets(c), 0);
    }
}

void tst_MqttClient::aDestroyedClientSaysDisconnect()
{
    FakeMqttBroker broker;
    auto *c = new MqttClient;
    aim(*c, broker.start());
    c->connectToHost();
    QTRY_COMPARE(c->state(), MqttClient::CONNECTED);
    delete c;
    QTRY_COMPARE(broker.disconnects, 1);
}

void tst_MqttClient::aSlotMayRunANestedEventLoop()
{
    //A modal error box in a slot: its event loop runs the deferred deletes while
    //the socket that emitted is still below on the stack.
    auto nested = []()
    {
        QEventLoop loop;
        QTimer::singleShot(100, &loop, &QEventLoop::quit);
        loop.exec();
    };

    FakeMqttBroker refusing;
    refusing.connackCode = 5;
    MqttClient c;
    aim(c, refusing.start());
    int errors = 0;
    connect(&c, &MqttClient::errorOccurred, this, [&]() { errors++; nested(); });
    c.connectToHost();
    QTRY_COMPARE(errors, 1);
    QTest::qWait(50);
    QCOMPARE(sockets(c), 0);

    //The same from a message, with the session closed by the slot itself.
    FakeMqttBroker broker;
    broker.retained << qMakePair(QString("a/1"), QByteArray("1")) << qMakePair(QString("a/2"), QByteArray("2"));
    MqttClient m;
    aim(m, broker.start());
    int messages = 0;
    connect(&m, &MqttClient::connected, this, [&]() { m.subscribe({ "a/+" }); });
    connect(&m, &MqttClient::messageReceived, this, [&]()
    {
        messages++;
        m.disconnectFromHost();
        nested();
    });
    m.connectToHost();
    QTRY_COMPARE(messages, 1);
    QTRY_COMPARE(broker.disconnects, 1);
    QTest::qWait(50);
    QCOMPARE(messages, 1);
    QCOMPARE(sockets(m), 0);
}

namespace
{

//A Mosquitto container of its own, removed when the test ends.
class Mosquitto
{
public:
    static constexpr const char *IMAGE = "eclipse-mosquitto:2.0.22";

    ~Mosquitto()
    {
        if (!m_name.isEmpty())
            docker({ "rm", "-f", m_name }, 30000);
    }

    QString start()
    {
        if (QStandardPaths::findExecutable("docker").isEmpty())
            return "docker is not installed";
        if (docker({ "info" }, 20000) != 0)
            return "docker does not answer";

        const QString name = QStringLiteral("calaos-tst-mqttclient-%1-%2")
                             .arg(QCoreApplication::applicationPid())
                             .arg(QRandomGenerator::global()->generate(), 8, 16, QLatin1Char('0'));
        const QString script =
            "printf 'listener 1883\\nallow_anonymous false\\npassword_file /tmp/pw\\n' > /tmp/test.conf"
            " && mosquitto_passwd -b -c /tmp/pw calaos s3cret && chown mosquitto:mosquitto /tmp/pw"
            " && chmod 0700 /tmp/pw && exec mosquitto -c /tmp/test.conf";
        //Named before the run: a run that times out may still leave the container.
        m_name = name;
        if (docker({ "run", "-d", "--rm", "--name", name, "-p", "127.0.0.1::1883", IMAGE, "sh", "-c", script }, 300000) != 0)
            return "could not start " + QString(IMAGE) + ": " + QString::fromUtf8(m_err);
        if (docker({ "port", m_name, "1883/tcp" }, 20000) != 0)
            return "no port for the container";
        const QString mapped = QString::fromUtf8(m_out).trimmed().split('\n').first();
        port = mapped.mid(mapped.lastIndexOf(':') + 1).toUShort();
        return port ? QString() : "no port in " + mapped;
    }

    int publish(const QString &topic, const QByteArray &payload, bool retain)
    {
        QStringList args = { "exec", "-i", m_name, "mosquitto_pub", "-u", "calaos", "-P", "s3cret", "-t", topic, "-s" };
        if (retain)
            args << "-r";
        return docker(args, 30000, payload);
    }

    quint16 port = 0;

private:
    QString m_name;
    QByteArray m_out, m_err;

    int docker(const QStringList &args, int timeoutMs, const QByteArray &input = QByteArray())
    {
        QProcess p;
        p.start("docker", args);
        if (!p.waitForStarted(10000))
            return -1;
        if (!input.isNull())
        {
            p.write(input);
            p.closeWriteChannel();
        }
        if (!p.waitForFinished(timeoutMs))
        {
            p.kill();
            p.waitForFinished();
            return -1;
        }
        m_out = p.readAllStandardOutput();
        m_err = p.readAllStandardError();
        return p.exitStatus() == QProcess::NormalExit ? p.exitCode() : -1;
    }
};

}

void tst_MqttClient::aRealMosquittoIsRead()
{
    Mosquitto broker;
    const QString why = broker.start();
    if (!why.isEmpty())
        QSKIP(qPrintable("No Mosquitto to test against: " + why));

    //The broker takes a moment to listen after the container starts.
    MqttClient c;
    aim(c, broker.port, "calaos", "wrong");
    QElapsedTimer t;
    t.start();
    do
    {
        c.connectToHost();
        QTRY_VERIFY_WITH_TIMEOUT(c.state() == MqttClient::UNCONNECTED, 5000);
        if (c.error() == MqttClient::CONNECTION_REFUSED)
            break;
        QTest::qWait(200);
    }
    while (t.elapsed() < 20000);
    QCOMPARE(c.error(), MqttClient::CONNECTION_REFUSED);
    //3.1.1 lets the broker choose between "bad user name or password" and "not authorized".
    QVERIFY2(c.connackCode() == 4 || c.connackCode() == 5, qPrintable(QString::number(c.connackCode())));

    QByteArray devices(1024 * 1024, Qt::Uninitialized);
    for (int i = 0;i < devices.size();i++)
        devices[i] = char('A' + i % 26);
    QCOMPARE(broker.publish("zigbee2mqtt/bridge/devices", devices, true), 0);
    QCOMPARE(broker.publish("esphome/discover/clim", "{\"name\":\"clim\"}", true), 0);

    MqttClient r;
    Seen got;
    got.watch(r);
    r.setKeepAlive(2);
    aim(r, broker.port, "calaos", "s3cret");
    connect(&r, &MqttClient::connected, this, [&]() { r.subscribe({ "zigbee2mqtt/bridge/+", "esphome/#", "nothing/here" }); });
    r.connectToHost();
    QTRY_COMPARE_WITH_TIMEOUT(got.messages.size(), 2, 10000);
    QCOMPARE(got.subacks.size(), 1);
    QCOMPARE(got.subacks.first().second, QList<bool>({ true, true, true }));
    for (const auto &m: std::as_const(got.messages))
    {
        QVERIFY(m.retained);
        if (m.topic == "zigbee2mqtt/bridge/devices")
            QVERIFY(m.payload == devices);
        else
            QCOMPARE(m.payload, QByteArray("{\"name\":\"clim\"}"));
    }

    QCOMPARE(broker.publish("esphome/clim/mode/state", "heat", false), 0);
    QTRY_COMPARE_WITH_TIMEOUT(got.messages.size(), 3, 10000);
    QVERIFY(!got.messages.last().retained);
    QCOMPARE(got.messages.last().payload, QByteArray("heat"));

    //Mosquitto drops a client silent for 1.5 keep alive periods: pings must go out.
    QTest::qWait(7000);
    QCOMPARE(r.state(), MqttClient::CONNECTED);
    QVERIFY(got.errors.isEmpty());
    r.disconnectFromHost();
    QCOMPARE(got.disconnected, 1);
}

QTEST_GUILESS_MAIN(tst_MqttClient)

#include "tst_mqttclient.moc"
