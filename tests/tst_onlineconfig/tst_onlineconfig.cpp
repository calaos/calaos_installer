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
    void deviceIdsParsing();
    void lostDevices();
    void getRefusalKeepsError();
    void lostIdlessDevices();

    void sessionOnlyInBodyWhenGiven();
    void unlockRequestShape();
    void unlockResponse();
    void refusalTexts();
    void unknownRefusalKeepsServerText();
    void serverUrls();
    void httpsRetry();
};

void TstOnlineConfig::sessionOnlyInBodyWhenGiven()
{
    QVERIFY(!obj(buildGetRequest("u", "p")).contains("admin_session"));
    QVERIFY(!obj(buildPutRequest("u", "p", "a", "b")).contains("admin_session"));
    QCOMPARE(obj(buildGetRequest("u", "p", "s1"))["admin_session"].toString(), QString("s1"));
    const QJsonObject put = obj(buildPutRequest("u", "p", "a", "b", "s2"));
    QCOMPARE(put["admin_session"].toString(), QString("s2"));
    QVERIFY(!put.contains("admin_pass"));
}

void TstOnlineConfig::unlockRequestShape()
{
    const QJsonObject o = obj(buildUnlockRequest("user", "pw", "admin secret"));
    QCOMPARE(o["action"].toString(), QString("admin_unlock"));
    QCOMPARE(o["cn_user"].toString(), QString("user"));
    QCOMPARE(o["cn_pass"].toString(), QString("pw"));
    QCOMPARE(o["admin_pass"].toString(), QString("admin secret"));
}

void TstOnlineConfig::unlockResponse()
{
    QCOMPARE(parseUnlockResponse("{\"success\":\"true\",\"admin_session\":\"abc\",\"expires_in\":\"900\"}"),
             QString("abc"));
    QVERIFY(parseUnlockResponse("{\"success\":\"false\",\"error\":\"bad_password\"}").isEmpty());
    QVERIFY(parseUnlockResponse("garbage").isEmpty());
    QCOMPARE(refusalCode("{\"success\":\"false\",\"error\":\"bad_password\"}"), QString("bad_password"));
    QVERIFY(refusalCode("{\"success\":\"true\",\"error\":\"x\"}").isEmpty());
    QVERIFY(refusalCode("<html/>").isEmpty());
}

void TstOnlineConfig::refusalTexts()
{
    auto text = [](const QString &json) { return parseGetResponse(json.toUtf8()).error; };

    QCOMPARE(text("{\"success\":\"false\",\"error\":\"bad_password\",\"msg\":\"Wrong admin password\"}"),
             QString("Wrong admin password."));
    QCOMPARE(text("{\"success\":\"false\",\"error\":\"rate_limited\",\"retry_after\":\"42\"}"),
             QString("Too many attempts, try again in 42 s."));
    QCOMPARE(text("{\"success\":\"false\",\"error\":\"rate_limited\"}"),
             QString("Too many attempts, try again later."));
    QCOMPARE(text("{\"success\":\"false\",\"error\":\"password_too_short\",\"min_length\":\"10\"}"),
             QString("The admin password must have at least 10 characters."));
    QCOMPARE(text("{\"success\":\"false\",\"error\":\"invalid_request\",\"msg\":\"admin_pass missing\"}"),
             QString("admin_pass missing"));
    QCOMPARE(text("{\"success\":\"false\",\"error\":\"not_allowed\"}"), QString("not_allowed"));

    const QStringList codes = { "bad_session", "https_required", "admin_not_set", "admin_unavailable",
                                "daemon_unreachable", "daemon_too_old", "upgrade_in_progress",
                                "household_password_required", "admin_already_set", "internal" };
    for (const QString &c : codes)
    {
        const QString t = text("{\"success\":\"false\",\"error\":\"" + c + "\",\"msg\":\"server text\"}");
        QVERIFY2(!t.isEmpty() && t != c && !t.contains("server text"), qPrintable(c + ": " + t));
    }
    QCOMPARE(parsePutResponse("{\"success\":\"false\",\"error\":\"bad_password\"}").error,
             QString("Wrong admin password."));
}

void TstOnlineConfig::unknownRefusalKeepsServerText()
{
    QCOMPARE(refusalText("{\"success\":\"false\",\"error\":\"unknown action\"}"), QString("unknown action"));
    QVERIFY(refusalText("{\"success\":\"false\"}").isEmpty());
    QVERIFY(refusalText("not json").isEmpty());
}

void TstOnlineConfig::serverUrls()
{
    QCOMPARE(serverUrl("192.168.1.10"), QUrl("https://192.168.1.10/api.php"));
    QCOMPARE(serverUrl("http://192.168.1.10:5454/api.php"), QUrl("http://192.168.1.10:5454/api.php"));
}

void TstOnlineConfig::httpsRetry()
{
    const QUrl clear("http://192.168.1.10:5454/api.php");
    QCOMPARE(httpsRetryUrl(clear), QUrl("https://192.168.1.10/api.php"));
    QCOMPARE(httpsRetryUrl(clear, 8443), QUrl("https://192.168.1.10:8443/api.php"));
    QVERIFY(!httpsRetryUrl(QUrl("https://192.168.1.10/api.php")).isValid());
    QCOMPARE(httpsRetryUrl(QUrl("https://box.local/other.php")), QUrl("https://box.local/api.php"));

    QVERIFY(shouldRetryOverHttps(QUrl("https://box/api.php"), "https_required"));
    QVERIFY(shouldRetryOverHttps(clear, "bad_session"));
    QVERIFY(!shouldRetryOverHttps(QUrl("http://192.168.1.10:8080/api.php"), "bad_session"));
    QVERIFY(!shouldRetryOverHttps(clear, "bad_password"));
    // An old server's own refusal: retrying elsewhere would hide it.
    QVERIFY(!shouldRetryOverHttps(clear, "update calaos_installer: this io.xml would drop 2 device(s)"));
}

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

namespace
{
QString io(const QStringList &ids)
{
    QString x = "<?xml version=\"1.0\"?><calaos:home xmlns:calaos=\"http://www.calaos.fr\" format=\"2\"><calaos:room name=\"r\" type=\"salon\">";
    for (const QString &i : ids)
        x += "<calaos:device id=\"" + i + "\" type=\"climate\"><calaos:input id=\"i_" + i + "\"/></calaos:device>";
    return x + "</calaos:room></calaos:home>";
}
}

void TstOnlineConfig::deviceIdsParsing()
{
    QCOMPARE(deviceIds(io({"a", "b"})), QStringList({"a", "b"}));
    QVERIFY(deviceIds(io({})).isEmpty());
    QVERIFY(deviceIds("not xml").isEmpty());
    QVERIFY(deviceIds("<calaos:home xmlns:calaos=\"http://www.calaos.fr\"><calaos:device type=\"x\"/></calaos:home>").isEmpty());
}

void TstOnlineConfig::lostDevices()
{
    QCOMPARE(devicesLostByPut(io({"a", "b"}), io({"a"})), QStringList({"b"}));
    QCOMPARE(devicesLostByPut(io({"a", "b"}), io({})), QStringList({"a", "b"}));
    QVERIFY(devicesLostByPut(io({"a"}), io({"a", "c"})).isEmpty());
    QVERIFY(devicesLostByPut(io({}), io({"a"})).isEmpty());
    QVERIFY(devicesLostByPut(io({}), io({})).isEmpty());
}

void TstOnlineConfig::getRefusalKeepsError()
{
    const GetResult r = parseGetResponse("{\"success\":\"false\",\"error\":\"bad credentials\"}");
    QVERIFY(!r.ok);
    QCOMPARE(r.error, QString("bad credentials"));
}

void TstOnlineConfig::lostIdlessDevices()
{
    const QString one = "<calaos:home xmlns:calaos=\"http://www.calaos.fr\"><calaos:device type=\"x\"/></calaos:home>";
    const QString two = "<calaos:home xmlns:calaos=\"http://www.calaos.fr\"><calaos:device type=\"x\"/><calaos:device type=\"y\"/></calaos:home>";
    const QString none = "<calaos:home xmlns:calaos=\"http://www.calaos.fr\"/>";

    QCOMPARE(devicesLostByPut(one, none).size(), 1);
    QCOMPARE(devicesLostByPut(two, one).size(), 1);
    QCOMPARE(devicesLostByPut(two, none).size(), 2);
    QVERIFY(devicesLostByPut(one, one).isEmpty());
    QVERIFY(devicesLostByPut(one, two).isEmpty());
    QVERIFY(devicesLostByPut(none, one).isEmpty());
    QCOMPARE(devicesLostByPut(one, io({"a"})).size(), 1);
}

QTEST_APPLESS_MAIN(TstOnlineConfig)
#include "tst_onlineconfig.moc"
