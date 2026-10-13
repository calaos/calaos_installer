#include <QtTest>
#include <QJsonDocument>
#include <QJsonObject>

#include "OnlineConfig.h"

using namespace OnlineConfig;

namespace
{
QJsonObject obj(const QByteArray &b) { return QJsonDocument::fromJson(b).object(); }

QByteArray getAnswer(const QString &formatJson)
{
    QString fmt = formatJson.isEmpty() ? QString() : QStringLiteral(",\"io_format\":") + formatJson;
    return QStringLiteral("{\"success\":\"true\",\"config_files\":{\"io.xml\":\"<io/>\",\"rules.xml\":\"<r/>\"}%1}")
        .arg(fmt).toUtf8();
}
}

class TstOnlineConfig : public QObject
{
    Q_OBJECT

private slots:
    void putDeclaresFormat2();
    void putCarriesFilesAndAuth();
    void getRequestShape();

    void getFormat_data();
    void getFormat();
    void getWithoutFilesIsOkButFlagged();
    void getFailureIsNotOk();

    void putAccepted();
    void putRefusedKeepsServerError();
    void putRefusedWithoutErrorText();
    void putGarbage();

    void formatsArePerHost();
    void devicesGate();
    void probeDecision();
};

void TstOnlineConfig::putDeclaresFormat2()
{
    const QJsonObject o = obj(buildPutRequest("u", "p", "a", "b"));
    QVERIFY(o.value("io_format").isDouble());
    QCOMPARE(o.value("io_format").toInt(), 2);
}

void TstOnlineConfig::putCarriesFilesAndAuth()
{
    const QJsonObject o = obj(buildPutRequest("user", "pw", "<io>é</io>", "<rules/>"));
    QCOMPARE(o["cn_user"].toString(), QString("user"));
    QCOMPARE(o["cn_pass"].toString(), QString("pw"));
    QCOMPARE(o["action"].toString(), QString("config"));
    QCOMPARE(o["type"].toString(), QString("put"));
    QCOMPARE(o["config_files"].toObject()["io.xml"].toString(), QString("<io>é</io>"));
    QCOMPARE(o["config_files"].toObject()["rules.xml"].toString(), QString("<rules/>"));
}

void TstOnlineConfig::getRequestShape()
{
    const QJsonObject o = obj(buildGetRequest("u", "p"));
    QCOMPARE(o["type"].toString(), QString("get"));
    QCOMPARE(o["action"].toString(), QString("config"));
    QVERIFY(!o.contains("io_format"));
    QVERIFY(!o.contains("config_files"));
}

void TstOnlineConfig::getFormat_data()
{
    QTest::addColumn<QString>("json");
    QTest::addColumn<int>("expected");

    QTest::newRow("absent") << QString() << 1;
    QTest::newRow("2") << QString("2") << 2;
    QTest::newRow("1") << QString("1") << 1;
    QTest::newRow("3") << QString("3") << 3;
    QTest::newRow("string") << QString("\"2\"") << 1;
    QTest::newRow("fractional") << QString("2.5") << 1;
    QTest::newRow("null") << QString("null") << 1;
    QTest::newRow("zero") << QString("0") << 1;
    QTest::newRow("negative") << QString("-4") << 1;
    QTest::newRow("huge") << QString("4294967298") << 1;
}

void TstOnlineConfig::getFormat()
{
    QFETCH(QString, json);
    QFETCH(int, expected);

    const GetResult r = parseGetResponse(getAnswer(json));
    QVERIFY(r.ok);
    QVERIFY(r.hasFiles);
    QCOMPARE(r.ioFormat, expected);
    QCOMPARE(r.ioXml, QString("<io/>"));
    QCOMPARE(r.rulesXml, QString("<r/>"));
}

void TstOnlineConfig::getWithoutFilesIsOkButFlagged()
{
    const GetResult r = parseGetResponse("{\"success\":\"true\",\"io_format\":2,\"config_files\":{\"io.xml\":\"x\"}}");
    QVERIFY(r.ok);
    QVERIFY(!r.hasFiles);
    QCOMPARE(r.ioFormat, 2);
}

void TstOnlineConfig::getFailureIsNotOk()
{
    QVERIFY(!parseGetResponse("{\"success\":\"false\",\"io_format\":2}").ok);
    QVERIFY(!parseGetResponse("not json").ok);
    QVERIFY(!parseGetResponse("").ok);
}

void TstOnlineConfig::putAccepted()
{
    QVERIFY(parsePutResponse("{\"success\":\"true\"}").ok);
}

void TstOnlineConfig::putRefusedKeepsServerError()
{
    const PutResult r = parsePutResponse(
        "{\"success\":\"false\",\"error\":\"update calaos_installer: this io.xml would drop 2 device(s)\"}");
    QVERIFY(!r.ok);
    QCOMPARE(r.error, QString("update calaos_installer: this io.xml would drop 2 device(s)"));
}

void TstOnlineConfig::putRefusedWithoutErrorText()
{
    const PutResult r = parsePutResponse("{\"success\":\"false\"}");
    QVERIFY(!r.ok);
    QVERIFY(r.error.isEmpty());
}

void TstOnlineConfig::putGarbage()
{
    QVERIFY(!parsePutResponse("<html>502</html>").ok);
    QVERIFY(!parsePutResponse("{}").ok);
}

void TstOnlineConfig::formatsArePerHost()
{
    QCOMPARE(serverFormat("a.local"), kFormatUnknown);
    rememberServerFormat("a.local", 2);
    rememberServerFormat("b.local", 1);
    QCOMPARE(serverFormat("a.local"), 2);
    QCOMPARE(serverFormat("b.local"), 1);
    QCOMPARE(serverFormat("c.local"), kFormatUnknown);
    rememberServerFormat("a.local", 1);
    QCOMPARE(serverFormat("a.local"), 1);
}

void TstOnlineConfig::devicesGate()
{
    QVERIFY(!serverAcceptsDevices(kFormatUnknown));
    QVERIFY(!serverAcceptsDevices(1));
    QVERIFY(serverAcceptsDevices(2));
    QVERIFY(serverAcceptsDevices(3));
}

void TstOnlineConfig::probeDecision()
{
    QVERIFY(!mustProbeBeforePut(false, "p.unknown"));
    QVERIFY(mustProbeBeforePut(true, "p.unknown"));

    rememberServerFormat("p.old", 1);
    QVERIFY(mustProbeBeforePut(true, "p.old"));
    QVERIFY(!mustProbeBeforePut(false, "p.old"));

    rememberServerFormat("p.new", 2);
    QVERIFY(!mustProbeBeforePut(true, "p.new"));
}

QTEST_APPLESS_MAIN(TstOnlineConfig)
#include "tst_onlineconfig.moc"
