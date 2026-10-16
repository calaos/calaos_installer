#include <QtTest>
#include <QJsonArray>

#include "../tst_devicetemplates/fixture.h"
#include "HomeAssistantDiscovery.h"
#include "IODoc.h"

using namespace Fixture;

namespace
{

QList<HomeAssistantDiscovery::Entity> entities(const HomeAssistantDiscovery &d)
{
    QList<HomeAssistantDiscovery::Entity> l;
    const auto byDevice = d.entitiesByDevice();
    for (const auto &list: byDevice)
        l += list;
    return l;
}

HomeAssistantDiscovery::Entity one(const QString &topic, const QByteArray &payload)
{
    HomeAssistantDiscovery d;
    d.handleMessage(topic, payload);
    const auto l = entities(d);
    return l.size() == 1 ? l.first() : HomeAssistantDiscovery::Entity();
}

QList<Params> paramsOf(const QString &topic, const QByteArray &payload)
{
    QList<Params> out;
    QString reason;
    HomeAssistantDiscovery::BrokerCreds b;
    b.host = "127.0.0.1";
    HomeAssistantDiscovery::toCalaosParams(one(topic, payload), b, out, reason);
    return out;
}

QStringList undocumented(Params p)
{
    QStringList documented;
    for (const QJsonValue &v: IODoc::getIOObject(u(p["type"]))["parameters"].toArray())
        documented << v.toObject()["name"].toString();
    QStringList res;
    for (int i = 0;i < p.size();i++)
    {
        std::string k, v;
        p.get_item(i, k, v);
        if (!documented.contains(u(k)))
            res << u(p["type"]) + "." + u(k);
    }
    return res;
}

}

class tst_HaDiscovery: public QObject
{
    Q_OBJECT

private slots:
    void aCoverIsWrittenWithTheParamsOfAShutter();
    void aNumberKeepsItsRange();
    void aDimmerIsScaledToItsBrightness();
    void numericPayloadsAreKept();
    void twoSetpointsAreImported();
    void entitiesWithoutUniqueIdOfTwoNodesStayApart();
    void aPrefixMayHoldSlashes();
    void theBaseTopicIsExpandedInTheAvailabilityList();
};

void tst_HaDiscovery::aCoverIsWrittenWithTheParamsOfAShutter()
{
    QList<Params> l = paramsOf("homeassistant/cover/node/volet/config",
                                R"({"name":"Volet","uniq_id":"v1","cmd_t":"node/volet/set","stat_t":"node/volet/state",
                                    "pl_open":"UP","pl_cls":"DOWN","pl_stop":"HALT"})");
    QCOMPARE(l.size(), 1);
    QCOMPARE(undocumented(l.first()), QStringList());
    QCOMPARE(u(l.first()["payload_open"]), QString("UP"));
    QCOMPARE(u(l.first()["payload_close"]), QString("DOWN"));
    QCOMPARE(u(l.first()["payload_stop"]), QString("HALT"));
}

void tst_HaDiscovery::aNumberKeepsItsRange()
{
    QList<Params> l = paramsOf("homeassistant/number/node/n/config",
                                R"({"name":"N","uniq_id":"n1","cmd_t":"node/n/set","stat_t":"node/n/state","min":5,"max":40,"step":0.5})");
    QCOMPARE(l.size(), 1);
    QCOMPARE(undocumented(l.first()), QStringList());
    QCOMPARE(u(l.first()["value_min"]), QString("5"));
    QCOMPARE(u(l.first()["value_max"]), QString("40"));
    QCOMPARE(u(l.first()["step"]), QString("0.5"));
}

void tst_HaDiscovery::aDimmerIsScaledToItsBrightness()
{
    //Calaos dims from 0 to 100, Home Assistant brightness defaults to 0..255.
    QList<Params> l = paramsOf("homeassistant/light/node/l/config",
                                R"({"name":"L","uniq_id":"l1","schema":"json","brightness":true,
                                    "cmd_t":"node/l/set","stat_t":"node/l/state"})");
    QCOMPARE(l.size(), 1);
    QCOMPARE(u(l.first()["type"]), QString("MqttOutputLightDimmer"));
    QCOMPARE(undocumented(l.first()), QStringList());
    QCOMPARE(u(l.first()["out_expr"]), QString("x * 2.55"));
    QCOMPARE(u(l.first()["in_expr"]), QString("x / 2.55"));
}

void tst_HaDiscovery::numericPayloadsAreKept()
{
    QList<Params> l = paramsOf("homeassistant/binary_sensor/node/b/config",
                                R"({"name":"B","uniq_id":"b1","stat_t":"node/b","pl_on":1,"pl_off":0})");
    QCOMPARE(l.size(), 1);
    QCOMPARE(u(l.first()["on_value"]), QString("1"));
    QCOMPARE(u(l.first()["off_value"]), QString("0"));
}

void tst_HaDiscovery::twoSetpointsAreImported()
{
    const QString p = "clim-buanderie/climate/clim_mitsu_buanderie/";
    const QByteArray config = QString(R"({"name":"Clim","uniq_id":"c1","mode_cmd_t":"%1mode/command","mode_stat_t":"%1mode/state",
        "modes":["off","heat_cool","cool","heat"],"curr_temp_t":"%1current_temperature/state",
        "temp_lo_cmd_t":"%1target_temperature_low/command","temp_lo_stat_t":"%1target_temperature_low/state",
        "temp_hi_cmd_t":"%1target_temperature_high/command","temp_hi_stat_t":"%1target_temperature_high/state"})").arg(p).toUtf8();
    QStringList published;
    for (Params io: paramsOf("homeassistant/climate/clim-buanderie/clim_mitsu_buanderie/config", config))
        published << u(io["topic_pub"]);
    QVERIFY2(published.contains(p + "target_temperature_low/command"), qPrintable(published.join(' ')));
    QVERIFY2(published.contains(p + "target_temperature_high/command"), qPrintable(published.join(' ')));
}

void tst_HaDiscovery::entitiesWithoutUniqueIdOfTwoNodesStayApart()
{
    HomeAssistantDiscovery d;
    d.handleMessage("homeassistant/sensor/node_a/temperature/config", R"({"name":"T","stat_t":"a/t"})");
    d.handleMessage("homeassistant/sensor/node_b/temperature/config", R"({"name":"T","stat_t":"b/t"})");
    QCOMPARE(entities(d).size(), 2);
}

void tst_HaDiscovery::aPrefixMayHoldSlashes()
{
    QVERIFY(HomeAssistantDiscovery::isDiscoveryTopic("ha/discovery/sensor/x/config", "ha/discovery"));
    QCOMPARE(one("ha/discovery/sensor/x/config", R"({"name":"X","stat_t":"x"})").component, QString("sensor"));
}

void tst_HaDiscovery::theBaseTopicIsExpandedInTheAvailabilityList()
{
    const HomeAssistantDiscovery::Entity e = one("homeassistant/sensor/node/t/config",
                                                  R"({"~":"node","name":"T","stat_t":"~/t","avty":[{"t":"~/status"}]})");
    QCOMPARE(e.payload["state_topic"].toString(), QString("node/t"));
    QCOMPARE(e.payload["availability"].toArray().first().toObject()["topic"].toString(), QString("node/status"));
}

QTEST_MAIN(tst_HaDiscovery)

#include "tst_hadiscovery.moc"
