#include <QtTest>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTemporaryDir>

#include "../tst_devicetemplates/fixture.h"
#include "EsphomeDiscovery.h"
#ifdef QT_MQTT_AVAILABLE
#include "FakeMqttBroker.h"
#include "MqttListener.h"
#endif

using namespace Fixture;

namespace
{

const QString P = "clim-buanderie/climate/clim_mitsu_buanderie/";

//What the unit of the user's test publishes, retained, without discovery.
QList<QPair<QString, QByteArray>> buanderieStates()
{
    return { { "clim-buanderie/status", "online" },
             { P + "mode/state", "heat" },
             { P + "target_temperature_low/state", "19.0" },
             { P + "target_temperature_high/state", "24.0" },
             { P + "current_temperature/state", "21.5" },
             { P + "action/state", "heating" },
             { P + "fan_mode/state", "auto" },
             { P + "swing_mode/state", "off" } };
}

//ESPHome's Home Assistant config, abbreviated keys.
QByteArray buanderieConfig()
{
    return R"({"name":"Clim Mitsu Buanderie","uniq_id":"ESPclimateclim_mitsu_buanderie",
      "avty_t":"clim-buanderie/status",
      "mode_cmd_t":"clim-buanderie/climate/clim_mitsu_buanderie/mode/command",
      "mode_stat_t":"clim-buanderie/climate/clim_mitsu_buanderie/mode/state",
      "modes":["off","heat_cool","cool","heat","fan_only","dry","auto"],
      "temp_lo_cmd_t":"clim-buanderie/climate/clim_mitsu_buanderie/target_temperature_low/command",
      "temp_lo_stat_t":"clim-buanderie/climate/clim_mitsu_buanderie/target_temperature_low/state",
      "temp_hi_cmd_t":"clim-buanderie/climate/clim_mitsu_buanderie/target_temperature_high/command",
      "temp_hi_stat_t":"clim-buanderie/climate/clim_mitsu_buanderie/target_temperature_high/state",
      "curr_temp_t":"clim-buanderie/climate/clim_mitsu_buanderie/current_temperature/state",
      "min_temp":17,"max_temp":31,"temp_step":0.5,
      "act_t":"clim-buanderie/climate/clim_mitsu_buanderie/action/state",
      "fan_mode_cmd_t":"clim-buanderie/climate/clim_mitsu_buanderie/fan_mode/command",
      "fan_mode_stat_t":"clim-buanderie/climate/clim_mitsu_buanderie/fan_mode/state",
      "fan_modes":["auto","low","medium","high","Turbo"],
      "swing_mode_cmd_t":"clim-buanderie/climate/clim_mitsu_buanderie/swing_mode/command",
      "swing_mode_stat_t":"clim-buanderie/climate/clim_mitsu_buanderie/swing_mode/state",
      "swing_modes":["off","vertical"],
      "dev":{"ids":"a0b1c2","name":"clim-buanderie"}})";
}

EsphomeDiscovery::Climate only(const EsphomeDiscovery &d)
{
    const QList<EsphomeDiscovery::Climate> l = d.climates();
    return l.size() == 1 ? l.first() : EsphomeDiscovery::Climate();
}

QStringList sorted(QSet<QString> s)
{
    QStringList l(s.begin(), s.end());
    l.sort();
    return l;
}

}

class tst_EsphomeDiscovery: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_workDir;

private slots:
    void initTestCase();
    void cleanup();

    void theTopicsOfAClimateMakeOneEntry();
    void aPrefixMayHoldSlashes();
    void otherTopicsAreNotClimates();
    void anEmptyRetainedMessageIsNoState();
    void aClimateNeitherDiscoveredNorOnlineIsOld();
    void theDiscoveryConfigTellsModesAndRange();
    void aConfigBaseTopicIsExpanded();
    void theNodeGivesItsFriendlyName();

    void oneOrTwoSetpointsFollowTheTopics();
    void fanSwingAndActionFollowTheirTopics();
    void modesComeFromTheConfig();
    void withoutAConfigTheModeReadIsAdded();
    void aClimateNotReadInFullChangesOnlyItsTopics();
    void theNameIsOnlyFilledWhenEmpty();
    void theIrTemplateGetsItsSensorFlag();
    void theRangeStaysInTheFieldBounds();

    void theBrokerOfTheProjectIsTheMostUsedOne();
    void aLoopbackBrokerIsReachedThroughTheBox();

#ifdef QT_MQTT_AVAILABLE
    void theListenerReadsRetainedMessages();
    void packetsSplitAnywhereAreRead();
    void aQos1DeliveryIsAcknowledged();
    void aPasswordWithoutUserIsNotSent();
    void aBadPasswordIsSaid();
    void aBrokerThatIsNotThereIsSaid();
    void aSilentBrokerTimesOut();
    void aRefusedSubscriptionIsSaid();
    void aBrokerClosingTheConnectionIsSaid();
    void somethingElseThanMqttIsSaid();
#else
    void theListenerNeedsQtMqtt();
#endif
};

void tst_EsphomeDiscovery::initTestCase()
{
    QVERIFY(m_workDir.isValid());
}

void tst_EsphomeDiscovery::cleanup()
{
    resetModel();
}

void tst_EsphomeDiscovery::theTopicsOfAClimateMakeOneEntry()
{
    EsphomeDiscovery d;
    for (const auto &m: buanderieStates())
        d.feed(m.first, m.second);
    d.feed(P + "mode/command", "cool");

    const EsphomeDiscovery::Climate c = only(d);
    QCOMPARE(c.prefix, QString("clim-buanderie"));
    QCOMPARE(c.objectId, QString("clim_mitsu_buanderie"));
    QCOMPARE(sorted(c.topics), QStringList({ "action", "current_temperature", "fan_mode", "mode", "swing_mode",
                                             "target_temperature_high", "target_temperature_low" }));
    //A command is not a state.
    QCOMPARE(c.states.value("mode"), QString("heat"));
    QCOMPARE(c.states.value("target_temperature_low"), QString("19.0"));
    QVERIFY(c.complete());
    QVERIFY(!c.configured);
    QCOMPARE(c.displayName(), QString("clim_mitsu_buanderie"));
}

void tst_EsphomeDiscovery::aPrefixMayHoldSlashes()
{
    EsphomeDiscovery d;
    d.feed("maison/rdc/climate/salon/mode/state", "off");
    QCOMPARE(only(d).prefix, QString("maison/rdc"));
    QCOMPARE(only(d).objectId, QString("salon"));
    QVERIFY(EsphomeDiscovery::filters().contains("+/+/climate/+/#"));
    QVERIFY(EsphomeDiscovery::filters().contains("+/climate/+/#"));
    QVERIFY(EsphomeDiscovery::filters().contains("esphome/discover/#"));
}

void tst_EsphomeDiscovery::otherTopicsAreNotClimates()
{
    EsphomeDiscovery d;
    d.feed("clim-buanderie/status", "online");
    d.feed("clim-buanderie/sensor/temp/state", "21");
    d.feed("climate/x/mode/state", "heat"); //no prefix
    d.feed("a/climate/x/mode/other", "heat");
    d.feed("homeassistant/climate/node/x/config", "not json");
    d.feed("homeassistant/sensor/node/x/config", R"({"stat_t":"node/sensor/x/state"})");
    QVERIFY(d.climates().isEmpty());
}

void tst_EsphomeDiscovery::anEmptyRetainedMessageIsNoState()
{
    //An empty retained message deletes the topic: a setpoint removed from the YAML
    //must not make the climate look dual.
    EsphomeDiscovery d;
    for (const auto &m: buanderieStates())
        d.feed(m.first, m.second);
    d.feed(P + "target_temperature/state", "21");
    d.feed(P + "target_temperature_low/state", "");
    d.feed(P + "target_temperature_high/state", "");
    QVERIFY(!only(d).topics.contains("target_temperature_low"));
    QVERIFY(!only(d).states.contains("target_temperature_low"));
    const QJsonObject tpl = shippedTemplate("esphome_climate");
    QVariantMap start = DeviceTemplates::defaults(tpl, nullptr);
    start["two_point"] = true;
    QCOMPARE(EsphomeDiscovery::prefill(tpl, only(d), start)["two_point"].toBool(), false);

    d.feed(P + "mode/state", "");
    QVERIFY(!only(d).topics.contains("mode"));
    QVERIFY(!only(d).complete());

    //Nothing left but deletions: no climate at all.
    EsphomeDiscovery gone;
    gone.feed("old/climate/clim/mode/state", "off");
    gone.feed("old/climate/clim/mode/state", "");
    gone.feed("never/climate/clim/mode/state", "");
    QVERIFY(gone.climates().isEmpty());
}

void tst_EsphomeDiscovery::aClimateNeitherDiscoveredNorOnlineIsOld()
{
    QVERIFY(EsphomeDiscovery::filters().contains("+/status"));
    QVERIFY(EsphomeDiscovery::filters().contains("+/+/status"));

    //The user's broker: the states of the former object id are still retained.
    EsphomeDiscovery d;
    d.feed("clim-buanderie/climate/clim_mitsu/mode/state", "cool");
    d.feed("clim-buanderie/climate/clim_mitsu/target_temperature/state", "22");
    for (const auto &m: buanderieStates())
        d.feed(m.first, m.second);
    d.feed("homeassistant/climate/clim-buanderie/clim_mitsu_buanderie/config", buanderieConfig());
    d.feed("garage/climate/clim/mode/state", "off");
    d.feed("garage/status", "offline");
    d.feed("bureau/climate/clim/mode/state", "heat");
    d.feed("bureau/status", "online");

    QStringList order, states;
    for (const EsphomeDiscovery::Climate &c: d.climates())
    {
        order << c.key() + (c.stale ? " old" : "");
        states << c.status;
    }
    QCOMPARE(order, QStringList({ "bureau/climate/clim", "clim-buanderie/climate/clim_mitsu_buanderie",
                                  "clim-buanderie/climate/clim_mitsu old", "garage/climate/clim old" }));
    QCOMPARE(states, QStringList({ "online", "online", "online", "offline" }));
}

void tst_EsphomeDiscovery::theDiscoveryConfigTellsModesAndRange()
{
    EsphomeDiscovery d;
    d.feed("homeassistant/climate/clim-buanderie/clim_mitsu_buanderie/config", buanderieConfig());

    const EsphomeDiscovery::Climate c = only(d);
    QCOMPARE(c.key(), QString("clim-buanderie/climate/clim_mitsu_buanderie"));
    QVERIFY(c.configured);
    QVERIFY(c.complete());
    QCOMPARE(c.name, QString("Clim Mitsu Buanderie"));
    QCOMPARE(c.modes, QStringList({ "off", "heat_cool", "cool", "heat", "fan_only", "dry", "auto" }));
    QCOMPARE(c.fanModes, QStringList({ "auto", "low", "medium", "high", "turbo" }));
    QCOMPARE(c.swingModes, QStringList({ "off", "vertical" }));
    QCOMPARE(c.tempMin, 17.0);
    QCOMPARE(c.tempMax, 31.0);
    QCOMPARE(c.tempStep, 0.5);
    QCOMPARE(sorted(c.topics), QStringList({ "action", "current_temperature", "fan_mode", "mode", "swing_mode",
                                             "target_temperature_high", "target_temperature_low" }));

    //States then merge into the same entry.
    for (const auto &m: buanderieStates())
        d.feed(m.first, m.second);
    QCOMPARE(only(d).states.value("fan_mode"), QString("auto"));
    QCOMPARE(only(d).name, QString("Clim Mitsu Buanderie"));
}

void tst_EsphomeDiscovery::aConfigBaseTopicIsExpanded()
{
    EsphomeDiscovery d;
    d.feed("homeassistant/climate/n/c/config",
           R"({"~":"salon/climate/clim","mode_cmd_t":"~/mode/command","temp_cmd_t":"~/target_temperature/command",
               "modes":["off","cool"],"dev":{"name":"Salon node"}})");
    const EsphomeDiscovery::Climate c = only(d);
    QCOMPARE(c.prefix, QString("salon"));
    QCOMPARE(c.objectId, QString("clim"));
    QCOMPARE(sorted(c.topics), QStringList({ "mode", "target_temperature" }));
    QCOMPARE(c.name, QString("Salon node"));
}

void tst_EsphomeDiscovery::theNodeGivesItsFriendlyName()
{
    EsphomeDiscovery d;
    d.feed("esphome/discover/clim-buanderie",
           R"({"name":"clim-buanderie","friendly_name":"Clim buanderie","ip":"192.168.1.42"})");
    d.feed(P + "mode/state", "off");
    QCOMPARE(only(d).node, QString("Clim buanderie"));
    QCOMPARE(only(d).ip, QString("192.168.1.42"));
    QCOMPARE(only(d).displayName(), QString("Clim buanderie"));
}

void tst_EsphomeDiscovery::oneOrTwoSetpointsFollowTheTopics()
{
    const QJsonObject tpl = shippedTemplate("esphome_climate");
    QVariantMap start = DeviceTemplates::defaults(tpl, nullptr);

    EsphomeDiscovery dual;
    for (const auto &m: buanderieStates())
        dual.feed(m.first, m.second);
    start["two_point"] = false;
    QStringList summary;
    QVariantMap v = EsphomeDiscovery::prefill(tpl, only(dual), start, &summary);
    QCOMPARE(v["topic_prefix"].toString(), QString("clim-buanderie"));
    QCOMPARE(v["object_id"].toString(), QString("clim_mitsu_buanderie"));
    QCOMPARE(v["two_point"].toBool(), true);
    QVERIFY(summary.contains("Two setpoints (target_temperature_low and _high)."));

    EsphomeDiscovery single;
    single.feed("salon/climate/clim/mode/state", "cool");
    single.feed("salon/climate/clim/target_temperature/state", "22");
    start["two_point"] = true;
    v = EsphomeDiscovery::prefill(tpl, only(single), start, &summary);
    QCOMPARE(v["two_point"].toBool(), false);
    QVERIFY(summary.contains("One setpoint (target_temperature)."));

    //And the device built from it uses the topics the unit publishes.
    QVERIFY(loadIo(m_workDir.filePath("h.xml"), houseXml()));
    v["name"] = "Clim";
    v["room"] = QVariant::fromValue(roomNamed("Salon"));
    const DeviceTemplates::Plan plan = DeviceTemplates::plan(tpl, v, [](const std::string &) { return false; });
    QStringList subs;
    for (const DeviceTemplates::OwnedIo &io: plan.ios)
        subs << u(io.params.get_param_const("topic_sub"));
    QVERIFY(subs.contains("salon/climate/clim/target_temperature/state"));
    QVERIFY(!subs.contains("salon/climate/clim/target_temperature_low/state"));
}

void tst_EsphomeDiscovery::fanSwingAndActionFollowTheirTopics()
{
    const QJsonObject tpl = shippedTemplate("esphome_climate");
    const QVariantMap start = DeviceTemplates::defaults(tpl, nullptr);

    EsphomeDiscovery d;
    for (const auto &m: buanderieStates())
        d.feed(m.first, m.second);
    QVariantMap v = EsphomeDiscovery::prefill(tpl, only(d), start);
    QCOMPARE(v["fan_modes"].toStringList(), QStringList({ "auto" }));
    QCOMPARE(v["swing_modes"].toStringList(), QStringList({ "off" }));
    QCOMPARE(v["with_action"].toBool(), true);

    //A unit without fan, swing nor action: none of them, whatever the form had.
    EsphomeDiscovery bare;
    bare.feed("salon/climate/clim/mode/state", "cool");
    bare.feed("salon/climate/clim/target_temperature/state", "22");
    QVariantMap had = start;
    had["fan_modes"] = QStringList({ "low", "high" });
    had["swing_modes"] = QStringList({ "both" });
    v = EsphomeDiscovery::prefill(tpl, only(bare), had);
    QCOMPARE(v["fan_modes"].toStringList(), QStringList());
    QCOMPARE(v["swing_modes"].toStringList(), QStringList());
    QCOMPARE(v["with_action"].toBool(), false);
}

void tst_EsphomeDiscovery::modesComeFromTheConfig()
{
    const QJsonObject tpl = shippedTemplate("esphome_climate");
    QVariantMap start = DeviceTemplates::defaults(tpl, nullptr);
    start["temp_min"] = 10.0;

    EsphomeDiscovery d;
    d.feed("homeassistant/climate/clim-buanderie/clim_mitsu_buanderie/config", buanderieConfig());
    QStringList summary;
    const QVariantMap v = EsphomeDiscovery::prefill(tpl, only(d), start, &summary);
    //heat_cool is Calaos' auto; ESPHome's auto (a schedule) is dropped, as is a custom fan mode.
    QCOMPARE(v["modes"].toStringList(), QStringList({ "off", "heat", "cool", "auto", "dry", "fan_only" }));
    QCOMPARE(v["fan_modes"].toStringList(), QStringList({ "auto", "low", "medium", "high" }));
    QCOMPARE(v["swing_modes"].toStringList(), QStringList({ "off", "vertical" }));
    QCOMPARE(v["two_point"].toBool(), true);
    QCOMPARE(v["temp_min"].toDouble(), 17.0);
    QCOMPARE(v["temp_max"].toDouble(), 31.0);
    QCOMPARE(v["temp_step"].toDouble(), 0.5);
    QVERIFY2(summary.contains("Not offered by this template: auto, turbo."), qPrintable(summary.join('|')));
    QVERIFY(summary.contains("Setpoint from 17 to 31 °C."));

    //Without heat_cool, no Calaos auto.
    EsphomeDiscovery noRange;
    noRange.feed("homeassistant/climate/n/c/config",
                 R"({"mode_cmd_t":"n/climate/c/mode/command","modes":["off","heat","auto"]})");
    QCOMPARE(EsphomeDiscovery::prefill(tpl, only(noRange), start)["modes"].toStringList(),
             QStringList({ "off", "heat" }));
}

void tst_EsphomeDiscovery::withoutAConfigTheModeReadIsAdded()
{
    const QJsonObject tpl = shippedTemplate("esphome_climate");
    QVariantMap start = DeviceTemplates::defaults(tpl, nullptr);
    start["modes"] = QStringList({ "off", "cool" });

    EsphomeDiscovery d;
    d.feed("salon/climate/clim/mode/state", "heat_cool");
    QCOMPARE(EsphomeDiscovery::prefill(tpl, only(d), start)["modes"].toStringList(),
             QStringList({ "off", "cool", "auto" }));

    EsphomeDiscovery dry;
    dry.feed("salon/climate/clim/mode/state", "dry");
    QCOMPARE(EsphomeDiscovery::prefill(tpl, only(dry), start)["modes"].toStringList(),
             QStringList({ "off", "cool", "dry" }));
}

void tst_EsphomeDiscovery::aClimateNotReadInFullChangesOnlyItsTopics()
{
    const QJsonObject tpl = shippedTemplate("esphome_climate");
    QVariantMap start = DeviceTemplates::defaults(tpl, nullptr);
    start["fan_modes"] = QStringList({ "low" });

    //Only a setpoint was read, not the mode: what is absent may just not be published yet.
    EsphomeDiscovery d;
    d.feed("salon/climate/clim/target_temperature_low/state", "19");
    QStringList summary;
    const QVariantMap v = EsphomeDiscovery::prefill(tpl, only(d), start, &summary);
    QCOMPARE(v["two_point"].toBool(), true);
    QCOMPARE(v["fan_modes"].toStringList(), QStringList({ "low" }));
    QCOMPARE(v["with_action"].toBool(), true);
    QCOMPARE(v["modes"], start["modes"]);
    QVERIFY(summary.contains("Its mode was not read: modes, fan and swing are left as they are."));
}

void tst_EsphomeDiscovery::theNameIsOnlyFilledWhenEmpty()
{
    const QJsonObject tpl = shippedTemplate("esphome_climate");
    QVariantMap start = DeviceTemplates::defaults(tpl, nullptr);
    EsphomeDiscovery d;
    d.feed("homeassistant/climate/clim-buanderie/clim_mitsu_buanderie/config", buanderieConfig());

    start["name"] = "  ";
    QCOMPARE(EsphomeDiscovery::prefill(tpl, only(d), start)["name"].toString(), QString("Clim Mitsu Buanderie"));
    start["name"] = "Clim";
    QCOMPARE(EsphomeDiscovery::prefill(tpl, only(d), start)["name"].toString(), QString("Clim"));
}

void tst_EsphomeDiscovery::theIrTemplateGetsItsSensorFlag()
{
    const QJsonObject tpl = shippedTemplate("esphome_climate_ir");
    const QVariantMap start = DeviceTemplates::defaults(tpl, nullptr);
    EsphomeDiscovery d;
    for (const auto &m: buanderieStates())
        d.feed(m.first, m.second);
    const QVariantMap v = EsphomeDiscovery::prefill(tpl, only(d), start);
    QCOMPARE(v["with_current_temp"].toBool(), true);
    QVERIFY(!v.contains("two_point"));
    QVERIFY(!v.contains("with_action"));

    EsphomeDiscovery blind;
    blind.feed("ir/climate/clim/mode/state", "off");
    QCOMPARE(EsphomeDiscovery::prefill(tpl, only(blind), start)["with_current_temp"].toBool(), false);
}

void tst_EsphomeDiscovery::theRangeStaysInTheFieldBounds()
{
    const QJsonObject tpl = shippedTemplate("esphome_climate");
    EsphomeDiscovery d;
    d.feed("homeassistant/climate/n/c/config",
           R"({"mode_cmd_t":"n/climate/c/mode/command","min_temp":-5,"max_temp":60,"temp_step":0.01})");
    const QVariantMap v = EsphomeDiscovery::prefill(tpl, only(d), DeviceTemplates::defaults(tpl, nullptr));
    QCOMPARE(v["temp_min"].toDouble(), 0.0);
    QCOMPARE(v["temp_max"].toDouble(), 50.0);
    QCOMPARE(v["temp_step"].toDouble(), 0.1);
    QVERIFY(DeviceTemplates::validate(tpl, v).size() <= 2); //name and room only
}

void tst_EsphomeDiscovery::theBrokerOfTheProjectIsTheMostUsedOne()
{
    const QByteArray xml1 = R"(<?xml version="1.0"?>
<calaos:ioconfig xmlns:calaos="http://www.calaos.fr"><calaos:home>
<calaos:room name="Salon" type="salon" hits="0">
  <calaos:input id="t" type="WITemp" name="T" host="10.0.0.5" port="502" var="1"/>
</calaos:room></calaos:home></calaos:ioconfig>)";
    QVERIFY(loadIo(m_workDir.filePath("none.xml"), xml1));
    QVERIFY(!EsphomeDiscovery::projectBroker().found);

    const QByteArray xml2 = R"(<?xml version="1.0"?>
<calaos:ioconfig xmlns:calaos="http://www.calaos.fr"><calaos:home>
<calaos:room name="Salon" type="salon" hits="0">
  <calaos:input id="a" type="MqttInputTemp" name="A" host="10.0.0.9" port="1884" topic_sub="a"/>
  <calaos:input id="b" type="MqttInputTemp" name="B" host="127.0.0.1" user="calaos" password="s3cret" topic_sub="b"/>
  <calaos:input id="w" type="WITemp" name="W" host="127.0.0.1" port="502" var="1"/>
</calaos:room>
<calaos:room name="Cave" type="cave" hits="0">
  <calaos:output id="c" type="MqttOutputAnalog" name="C" host="127.0.0.1" user="calaos" password="s3cret" topic_sub="c" topic_pub="c"/>
</calaos:room></calaos:home></calaos:ioconfig>)";
    QVERIFY(loadIo(m_workDir.filePath("mqtt.xml"), xml2));
    const EsphomeDiscovery::Broker b = EsphomeDiscovery::projectBroker();
    QVERIFY(b.found);
    QCOMPARE(b.host, QString("127.0.0.1"));
    QCOMPARE(b.port, 1883);
    QCOMPARE(b.user, QString("calaos"));
    QCOMPARE(b.password, QString("s3cret"));
}

void tst_EsphomeDiscovery::aLoopbackBrokerIsReachedThroughTheBox()
{
    QCOMPARE(EsphomeDiscovery::probeHost("127.0.0.1", "", "192.168.1.10"), QString("192.168.1.10"));
    QCOMPARE(EsphomeDiscovery::probeHost(" localhost", "", "calaos.local:5454"), QString("calaos.local"));
    QCOMPARE(EsphomeDiscovery::probeHost("::1", "", "https://box.lan:443/api"), QString("box.lan"));
    QCOMPARE(EsphomeDiscovery::probeHost("127.0.0.1", "", ""), QString());
    QCOMPARE(EsphomeDiscovery::probeHost("10.0.0.9 ", "10.0.0.7", "192.168.1.10"), QString("10.0.0.9"));
    //The broker the Zigbee2MQTT dialog reached from here comes before the box.
    QCOMPARE(EsphomeDiscovery::probeHost("127.0.0.1", "10.0.0.7", "192.168.1.10"), QString("10.0.0.7"));
    QCOMPARE(EsphomeDiscovery::probeHost("127.0.0.1", "localhost", "192.168.1.10"), QString("192.168.1.10"));
    QCOMPARE(EsphomeDiscovery::hostOf("fe80::1"), QString("fe80::1"));
}

#ifdef QT_MQTT_AVAILABLE

namespace
{

struct Run
{
    QString error;
    QList<QPair<QString, QByteArray>> messages;
    bool listened = false;
    bool done = false;
};

Run probe(FakeMqttBroker &broker, const MqttListener::Settings &s, int listenMs = 300, int timeoutMs = 2000)
{
    Run r;
    MqttListener p;
    QObject::connect(&p, &MqttListener::message, [&](const QString &t, const QByteArray &m) { r.messages << qMakePair(t, m); });
    QObject::connect(&p, &MqttListener::listening, [&]() { r.listened = true; });
    QObject::connect(&p, &MqttListener::finished, [&](const QString &e) { r.error = e; r.done = true; });
    p.start(s, EsphomeDiscovery::filters(), listenMs, timeoutMs);
    if (!QTest::qWaitFor([&]() { return r.done; }, 10000))
        r.error = "the probe never finished";
    else if (p.isRunning())
        r.error = "the probe still runs after finished()";
    //Let the broker read what was sent last.
    QTest::qWait(50);
    Q_UNUSED(broker);
    return r;
}

MqttListener::Settings local(quint16 port, const QString &user = QString(), const QString &password = QString())
{
    MqttListener::Settings s;
    s.host = "127.0.0.1";
    s.port = port;
    s.user = user;
    s.password = password;
    return s;
}

}

void tst_EsphomeDiscovery::theListenerReadsRetainedMessages()
{
    FakeMqttBroker broker;
    broker.retained = buanderieStates();
    broker.retained << qMakePair(QString("homeassistant/climate/clim-buanderie/clim_mitsu_buanderie/config"), buanderieConfig());
    broker.retained << qMakePair(QString("zigbee2mqtt/bridge/devices"), QByteArray("[]"));
    broker.requiredUser = "calaos";
    broker.requiredPassword = "s3cret";
    const quint16 port = broker.start();

    const Run r = probe(broker, local(port, "calaos", "s3cret"));
    QCOMPARE(r.error, QString());
    QVERIFY(r.listened);
    QCOMPARE(broker.connects, 1);
    QCOMPARE(broker.user, QString("calaos"));
    QCOMPARE(broker.password, QString("s3cret"));
    QVERIFY(broker.cleanSession);
    QVERIFY(broker.clientId.startsWith("calaos-installer-"));
    QCOMPARE(broker.filters, EsphomeDiscovery::filters());
    QCOMPARE(broker.disconnects, 1);
    //zigbee2mqtt: not subscribed.
    QCOMPARE(r.messages.size(), 9);
    //Two installers on one broker must not kick each other out.
    const QString firstId = broker.clientId;
    probe(broker, local(port, "calaos", "s3cret"));
    QVERIFY(broker.clientId != firstId);
    QCOMPARE(broker.connects, 2);

    EsphomeDiscovery d;
    for (const auto &m: r.messages)
        d.feed(m.first, m.second);
    QCOMPARE(only(d).key(), QString("clim-buanderie/climate/clim_mitsu_buanderie"));
    QVERIFY(only(d).configured);
    QCOMPARE(only(d).states.value("mode"), QString("heat"));

    //Live messages during the listening time count too.
    FakeMqttBroker live;
    const quint16 livePort = live.start();
    MqttListener p;
    QStringList topics;
    bool done = false;
    connect(&p, &MqttListener::message, this, [&](const QString &t) { topics << t; });
    connect(&p, &MqttListener::listening, this, [&]() { live.publish("x/climate/y/mode/state", "off"); });
    connect(&p, &MqttListener::finished, this, [&]() { done = true; });
    p.start(local(livePort), EsphomeDiscovery::filters(), 300, 2000);
    QTRY_VERIFY(done);
    QCOMPARE(topics, QStringList({ "x/climate/y/mode/state" }));
}

void tst_EsphomeDiscovery::packetsSplitAnywhereAreRead()
{
    FakeMqttBroker broker;
    broker.byteByByte = true;
    broker.retained = buanderieStates();
    //Over 127 bytes: a two-byte remaining length.
    broker.retained << qMakePair(QString("homeassistant/climate/clim-buanderie/clim_mitsu_buanderie/config"), buanderieConfig());
    const Run r = probe(broker, local(broker.start()));
    QCOMPARE(r.error, QString());
    QCOMPARE(r.messages.size(), 9);
    //One SUBSCRIBE per filter: the order follows the filters.
    QVERIFY(r.messages.contains(qMakePair(QString("homeassistant/climate/clim-buanderie/clim_mitsu_buanderie/config"),
                                          buanderieConfig())));
}

void tst_EsphomeDiscovery::aQos1DeliveryIsAcknowledged()
{
    FakeMqttBroker broker;
    broker.deliverAtQos1 = true;
    broker.retained = buanderieStates();
    const Run r = probe(broker, local(broker.start()));
    QCOMPARE(r.error, QString());
    QCOMPARE(r.messages.size(), 8);
    QVERIFY(r.messages.contains(qMakePair(P + "mode/state", QByteArray("heat"))));
    QCOMPARE(broker.pubacks, 8);
}

void tst_EsphomeDiscovery::aPasswordWithoutUserIsNotSent()
{
    FakeMqttBroker broker;
    probe(broker, local(broker.start(), QString(), "orphan"));
    QCOMPARE(broker.connects, 1);
    QVERIFY(!broker.hadPassword);
    QCOMPARE(broker.user, QString());
}

void tst_EsphomeDiscovery::aBadPasswordIsSaid()
{
    FakeMqttBroker broker;
    broker.requiredUser = "calaos";
    broker.requiredPassword = "s3cret";
    const quint16 port = broker.start();
    const Run r = probe(broker, local(port, "calaos", "wrong"));
    QCOMPARE(r.error, QString("The broker at 127.0.0.1:%1 refused the user name or the password.").arg(port));
    QVERIFY(!r.listened);
    QVERIFY(broker.filters.isEmpty());

    broker.connackCode = 5;
    QCOMPARE(probe(broker, local(port, "calaos", "s3cret")).error,
             QString("The broker at 127.0.0.1:%1 does not let this user in: check the user and the password.").arg(port));
    broker.connackCode = 3;
    QCOMPARE(probe(broker, local(port)).error, QString("The broker at 127.0.0.1:%1 refused the connection (code 3).").arg(port));
}

void tst_EsphomeDiscovery::aBrokerThatIsNotThereIsSaid()
{
    QTcpServer closed;
    QVERIFY(closed.listen(QHostAddress::LocalHost, 0));
    const quint16 port = closed.serverPort();
    closed.close();

    FakeMqttBroker unused;
    const Run r = probe(unused, local(port));
    QCOMPARE(r.error, QString("Nothing answers on 127.0.0.1:%1: check that the broker runs and listens on this port.").arg(port));
}

void tst_EsphomeDiscovery::aSilentBrokerTimesOut()
{
    FakeMqttBroker broker;
    broker.answerConnect = false;
    const quint16 port = broker.start();
    QElapsedTimer t;
    t.start();
    const Run r = probe(broker, local(port), 300, 500);
    QCOMPARE(r.error, QString("No answer from the broker at 127.0.0.1:%1 within 0.5 s.").arg(port));
    QVERIFY(t.elapsed() >= 450);
    QCOMPARE(broker.connects, 1);
}

void tst_EsphomeDiscovery::aRefusedSubscriptionIsSaid()
{
    FakeMqttBroker broker;
    broker.refuseSubscriptions = true;
    const quint16 port = broker.start();
    const Run r = probe(broker, local(port));
    QCOMPARE(r.error, QString("The broker at 127.0.0.1:%1 does not let this user read those topics.").arg(port));
    QVERIFY(!r.listened);
}

void tst_EsphomeDiscovery::aBrokerClosingTheConnectionIsSaid()
{
    FakeMqttBroker broker;
    broker.closeAfterConnect = true;
    const quint16 port = broker.start();
    QCOMPARE(probe(broker, local(port)).error,
             QString("The broker at 127.0.0.1:%1 closed the connection: check the user and the password.").arg(port));
}

void tst_EsphomeDiscovery::somethingElseThanMqttIsSaid()
{
    //An HTTP server, say.
    QTcpServer http;
    QVERIFY(http.listen(QHostAddress::LocalHost, 0));
    connect(&http, &QTcpServer::newConnection, this, [&]()
    {
        QTcpSocket *s = http.nextPendingConnection();
        connect(s, &QTcpSocket::readyRead, s, [s]() { s->write("HTTP/1.1 400 Bad Request\r\n\r\n"); });
    });
    FakeMqttBroker unused;
    const Run r = probe(unused, local(http.serverPort()));
    QCOMPARE(r.error, QString("The broker at 127.0.0.1:%1 sent data that is not MQTT.").arg(http.serverPort()));
}

#else

void tst_EsphomeDiscovery::theListenerNeedsQtMqtt()
{
    //CI builds with Qt MQTT: there, a build without it is a broken setup, not a skip.
    if (qEnvironmentVariableIsSet("CALAOS_REQUIRE_MQTT"))
        QFAIL("Built without Qt MQTT while CALAOS_REQUIRE_MQTT is set");
    QSKIP("Built without Qt MQTT: the broker discovery is not part of this build");
}

#endif

QTEST_GUILESS_MAIN(tst_EsphomeDiscovery)

#include "tst_esphomediscovery.moc"
