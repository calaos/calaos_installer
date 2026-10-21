#include <QtTest>
#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QStandardPaths>
#include <QTcpServer>
#include <QTemporaryDir>
#include <QTreeWidget>

#include "../tst_devicetemplates/fixture.h"
#include "ConfigOptions.h"
#include "EsphomeDiscovery.h"
#include "HomeAssistantDiscovery.h"
#include "IODoc.h"
#include <IOFactory.h>
#include "DialogHomeAssistantMqtt.h"
#include "../tst_esphomediscovery/FakeMqttBroker.h"

using namespace Calaos;
using namespace Fixture;

namespace
{

using Entity = HomeAssistantDiscovery::Entity;

Entity one(const QString &topic, const QByteArray &payload)
{
    HomeAssistantDiscovery d;
    d.feed(topic, payload);
    const QList<Entity> l = d.entities();
    return l.size() == 1 ? l.first() : Entity();
}

MqttBroker localBroker()
{
    MqttBroker b;
    b.host = "127.0.0.1";
    return b;
}

Params paramsOf(const QString &topic, const QByteArray &payload, QString *reason = nullptr, QStringList *notes = nullptr)
{
    Params p;
    QString why;
    if (!HomeAssistantDiscovery::ioParams(one(topic, payload), localBroker(), p, &why, notes))
        p.clear();
    if (reason)
        *reason = why;
    return p;
}

QStringList namesOf(const QJsonArray &a)
{
    QStringList l;
    for (const QJsonValue &v: a)
        l << v.toObject()["name"].toString();
    return l;
}

//Every param of an IO is documented by IODoc for its type, and none is empty.
QStringList undocumented(Params p)
{
    const QStringList documented = namesOf(IODoc::getIOObject(u(p["type"]))["parameters"].toArray());
    QStringList res;
    if (documented.isEmpty())
        res << "no IODoc for " + u(p["type"]);
    for (int i = 0;i < p.size();i++)
    {
        std::string k, v;
        p.get_item(i, k, v);
        if (k != "type" && (!documented.contains(u(k)) || v.empty()))
            res << u(p["type"]) + "." + u(k);
    }
    return res;
}

QStringList undocumented(Device *dev)
{
    QStringList res;
    const QJsonObject doc = IODoc::getDeviceObject(u(dev->impl()));
    QStringList roles;
    for (const QJsonValue &r: doc["link_roles"].toArray())
        roles << r.toObject()["role"].toString();
    for (const DeviceLink &l: dev->links())
    {
        if (!roles.contains(u(l.name)))
            res << "role " + u(l.name);
        if (!DeviceTemplates::ioById(l.io))
            res << "unresolved " + u(l.name);
    }
    const QStringList params = namesOf(doc["parameters"].toArray());
    for (const auto &p: dev->deviceParams())
        if (!params.contains(u(p.first)) || p.second.empty())
            res << "param " + u(p.first);
    const std::vector<std::string> creatable = IOFactory::Instance().creatableTypes();
    for (IOBase *io: dev->ownedIos())
    {
        std::string type = io->get_param("type");
        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        if (std::find(creatable.begin(), creatable.end(), type) == creatable.end())
            res << "type " + u(io->get_param("type"));
        res += undocumented(io->get_params());
    }
    return res;
}

IOBase *linked(Device *dev, const QString &role)
{
    for (const DeviceLink &l: dev->links())
        if (u(l.name) == role)
            return DeviceTemplates::ioById(l.io);
    return nullptr;
}

QString param(IOBase *io, const char *name)
{
    return io ? u(io->get_param(name)) : QString("<no IO>");
}

QString deviceParam(Device *dev, const char *name)
{
    for (const auto &p: dev->deviceParams())
        if (p.first == name)
            return u(p.second);
    return QString();
}

//What calaos_server reads at path in payload (IO/JsonPath.h): booleans as true/false,
//strings as they are, numbers in its decimal form.
QString serverValue(const QByteArray &payload, const QString &path)
{
    QJsonValue v = QJsonDocument::fromJson(payload).object();
    for (const QString &k: path.split('/', Qt::SkipEmptyParts))
        v = v.toObject().value(k);
    if (v.isBool())
        return v.toBool() ? "true" : "false";
    if (v.isDouble())
        return QString::number(v.toDouble());
    return v.toString();
}

const QString P = "clim-buanderie/climate/clim_mitsu_buanderie/";

//What ESPHome publishes for a dual setpoint climate (see tst_esphomediscovery).
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
const QString BUANDERIE_TOPIC = "homeassistant/climate/clim-buanderie/clim_mitsu_buanderie/config";

//An ESPHome climate with one setpoint, its topics written with ~.
QByteArray salonConfig()
{
    return R"({"~":"salon/climate/clim_salon","name":"Clim salon","uniq_id":"ESPclimateclim_salon",
      "avty_t":"salon/status","mode_cmd_t":"~/mode/command","mode_stat_t":"~/mode/state",
      "modes":["off","heat_cool","cool","heat"],"curr_temp_t":"~/current_temperature/state",
      "temp_cmd_t":"~/target_temperature/command","temp_stat_t":"~/target_temperature/state",
      "min_temp":16,"max_temp":30,"temp_step":1,"dev":{"ids":"d1","name":"salon"}})";
}
const QString SALON_TOPIC = "homeassistant/climate/salon/clim_salon/config";

//A Zigbee2MQTT thermostatic valve: one JSON state topic, a command topic per value.
QByteArray trvConfig()
{
    return R"({"name":null,"object_id":"vanne_chambre","unique_id":"0x1234_climate_zigbee2mqtt",
      "availability":[{"topic":"zigbee2mqtt/bridge/state","value_template":"{{ value_json.state }}"},
                      {"topic":"zigbee2mqtt/Vanne chambre/availability","value_template":"{{ value_json.state }}"}],
      "availability_mode":"all",
      "current_temperature_topic":"zigbee2mqtt/Vanne chambre",
      "current_temperature_template":"{{ value_json.local_temperature }}",
      "temperature_command_topic":"zigbee2mqtt/Vanne chambre/set/current_heating_setpoint",
      "temperature_state_topic":"zigbee2mqtt/Vanne chambre",
      "temperature_state_template":"{{ value_json.current_heating_setpoint }}",
      "mode_command_topic":"zigbee2mqtt/Vanne chambre/set/system_mode",
      "mode_state_topic":"zigbee2mqtt/Vanne chambre",
      "mode_state_template":"{{ value_json.system_mode }}",
      "modes":["off","heat","auto"],
      "action_topic":"zigbee2mqtt/Vanne chambre",
      "action_template":"{% set values = {None:None,'idle':'idle','heat':'heating','cool':'cooling','fan_only':'fan'} %}{{ values[value_json.running_state] }}",
      "min_temp":"5","max_temp":"30","temp_step":0.5,
      "device":{"identifiers":["zigbee2mqtt_0x1234"],"name":"Vanne chambre","model":"TRV"}})";
}
const QString TRV_TOPIC = "homeassistant/climate/0x1234/climate/config";

}

class tst_HaDiscovery: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;
    QString work(const QString &name) const { return m_workDir.filePath(name); }
    Device *importClimate(const QString &topic, const QByteArray &config, QStringList *notes = nullptr);
    void checkRoundTrip(const QString &tag);

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void aCoverIsWrittenWithTheParamsOfAShutter();
    void aNumberKeepsItsRange();
    void aDimmerIsScaledToItsBrightness();
    void numericPayloadsAreKept();
    void aZigbeeContactIsReadAsTheServerReadsIt();
    void entitiesWithoutUniqueIdOfTwoNodesStayApart();
    void aPrefixMayHoldSlashes();
    void theBaseTopicIsExpandedInTheAvailabilityList();

    void abbreviationsAreExpanded();
    void anEmptyRetainedConfigDeletesTheEntity();
    void aConfigThatIsNotJsonIsCounted();
    void availabilityIsReadFromItsTopic();
    void aDeviceConfigHoldsSeveralEntities();
    void templatesCalaosCanFollow_data();
    void templatesCalaosCanFollow();
    void everyKindIsADocumentedIo_data();
    void everyKindIsADocumentedIo();
    void whatCannotBeImportedSaysWhy_data();
    void whatCannotBeImportedSaysWhy();
    void theProjectCredentialsAreWritten();

    void twoSetpointsAreImported();
    void anEsphomeClimateIsTheDeviceOfTheEsphomePanel();
    void aClimateWithOneSetpoint();
    void aZigbeeThermostatKeepsItsTopicsAndPaths();
    void aClimateWithoutSensorTakesTheIrTemplate();
    void aClimateCalaosCannotDriveIsRefused_data();
    void aClimateCalaosCannotDriveIsRefused();
    void whatTheProjectHasIsSkipped();
    void importedIosAndDevicesSurviveASaveAndReload();

    void theDialogStartsOnTheChosenRoom();
    void searchingListsTheEntitiesByDevice();
    void theProjectCredentialsOnlyGoToItsBroker();
    void brokerErrorsAreShown();
    void anEmptyBrokerSaysSo();
    void importingCreatesTheCheckedEntities();
    void enterInTheAddressSearches();
};

void tst_HaDiscovery::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_hadiscovery");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    ConfigOptions::Instance().setOption("ui/lang", "en");
}

void tst_HaDiscovery::init()
{
    QVERIFY(loadIo(work("house.xml"), houseXml()));
}

void tst_HaDiscovery::cleanup()
{
    resetModel();
}

Device *tst_HaDiscovery::importClimate(const QString &topic, const QByteArray &config, QStringList *notes)
{
    HomeAssistantDiscovery d;
    d.feed(topic, config);
    const HomeAssistantDiscovery::Imported imported =
        HomeAssistantDiscovery::import(d.entities(), shippedTemplates(), localBroker(), roomNamed("Salon"));
    if (notes)
        *notes = imported.notes;
    return imported.devices.size() == 1 && imported.ios.empty() ? imported.devices.front() : nullptr;
}

void tst_HaDiscovery::checkRoundTrip(const QString &tag)
{
    const QByteArray first = saveIo(work("first.xml"));
    QVERIFY(!first.isEmpty());
    //For a load by the real server: tests/server_load/run.sh.
    const QString dump = qEnvironmentVariable("CALAOS_DEVICE_XML_DIR");
    if (!dump.isEmpty())
        QVERIFY(writeFile(QDir(dump).filePath("ha_" + tag + ".xml"), first));
    QVERIFY(loadIo(work("reload.xml"), first));
    QCOMPARE(saveIo(work("second.xml")), first);
}

void tst_HaDiscovery::aCoverIsWrittenWithTheParamsOfAShutter()
{
    Params p = paramsOf("homeassistant/cover/node/volet/config",
                        R"({"name":"Volet","uniq_id":"v1","cmd_t":"node/volet/set","stat_t":"node/volet/state",
                            "pl_open":"UP","pl_cls":"DOWN","pl_stop":"HALT","stat_clsd":"down"})");
    QCOMPARE(u(p["type"]), QString("MqttOutputShutter"));
    QCOMPARE(undocumented(p), QStringList());
    QCOMPARE(u(p["payload_open"]), QString("UP"));
    QCOMPARE(u(p["payload_close"]), QString("DOWN"));
    QCOMPARE(u(p["payload_stop"]), QString("HALT"));
    QCOMPARE(u(p["topic_sub"]), QString("node/volet/state"));
    QCOMPARE(u(p["state_open"]), QString("open"));
    QCOMPARE(u(p["state_close"]), QString("down"));

    //Without a state, Calaos times the travel: no topic_sub.
    QStringList notes;
    p = paramsOf("homeassistant/cover/node/v2/config", R"({"name":"V2","cmd_t":"node/v2/set"})", nullptr, &notes);
    QCOMPARE(undocumented(p), QStringList());
    QVERIFY(!p.Exists("topic_sub"));
    QCOMPARE(u(p["payload_open"]), QString("OPEN"));
    //Mandatory: without it the server stops the shutter as soon as it starts.
    QCOMPARE(u(p["time"]), QString("60"));
    QCOMPARE(notes, QStringList({ "It reports no state: Calaos times its travel, 60 s until set in its properties." }));
}

void tst_HaDiscovery::aNumberKeepsItsRange()
{
    Params p = paramsOf("homeassistant/number/node/n/config",
                        R"({"name":"N","uniq_id":"n1","cmd_t":"node/n/set","stat_t":"node/n/state","min":5,"max":40,"step":0.5,
                            "cmd_tpl":"{\"value\": {{ value }}}","unit_of_meas":"%"})");
    QCOMPARE(u(p["type"]), QString("MqttOutputAnalog"));
    QCOMPARE(undocumented(p), QStringList());
    QCOMPARE(u(p["value_min"]), QString("5"));
    QCOMPARE(u(p["value_max"]), QString("40"));
    QCOMPARE(u(p["step"]), QString("0.5"));
    QCOMPARE(u(p["data"]), QString(R"({"value": __##VALUE##__})"));
    QCOMPARE(u(p["unit"]), QString("%"));

    //Home Assistant's range when none is given.
    p = paramsOf("homeassistant/number/node/m/config", R"({"name":"M","cmd_t":"node/m/set","stat_t":"node/m"})");
    QCOMPARE(u(p["value_min"]), QString("1"));
    QCOMPARE(u(p["value_max"]), QString("100"));
    QCOMPARE(u(p["data"]), QString("__##VALUE##__"));
}

void tst_HaDiscovery::aDimmerIsScaledToItsBrightness()
{
    //Calaos dims from 0 to 100, Home Assistant brightness defaults to 0..255.
    Params p = paramsOf("homeassistant/light/node/l/config",
                        R"({"name":"L","uniq_id":"l1","schema":"json","brightness":true,
                            "cmd_t":"node/l/set","stat_t":"node/l/state"})");
    QCOMPARE(u(p["type"]), QString("MqttOutputLightDimmer"));
    QCOMPARE(undocumented(p), QStringList());
    QCOMPARE(u(p["out_expr"]), QString("x * 2.55"));
    QCOMPARE(u(p["in_expr"]), QString("x / 2.55"));
    QCOMPARE(u(p["path"]), QString("brightness"));
    QStringList notes;
    paramsOf("homeassistant/light/node/l/config", R"({"name":"L","schema":"json","brightness":true,"cmd_t":"l/set","stat_t":"l"})",
             nullptr, &notes);
    QCOMPARE(notes, QStringList({ "Calaos turns it off with brightness 0: switched off elsewhere with its brightness kept, "
                                  "it still shows on." }));

    p = paramsOf("homeassistant/light/node/l/config",
                 R"({"name":"L","schema":"json","sup_clrm":["brightness"],"bri_scl":100,"cmd_t":"l/set","stat_t":"l"})");
    QCOMPARE(u(p["out_expr"]), QString("x * 1"));
}

void tst_HaDiscovery::numericPayloadsAreKept()
{
    Params p = paramsOf("homeassistant/binary_sensor/node/b/config",
                        R"({"name":"B","uniq_id":"b1","stat_t":"node/b","pl_on":1,"pl_off":0})");
    QCOMPARE(u(p["on_value"]), QString("1"));
    QCOMPARE(u(p["off_value"]), QString("0"));
    p = paramsOf("homeassistant/binary_sensor/node/c/config", R"({"name":"C","stat_t":"node/c","pl_on":true,"pl_off":false})");
    QCOMPARE(u(p["on_value"]), QString("true"));
    QCOMPARE(u(p["off_value"]), QString("false"));
}

void tst_HaDiscovery::aZigbeeContactIsReadAsTheServerReadsIt()
{
    //Zigbee2MQTT: {"contact":false} is an open door, payload_on false.
    Params p = paramsOf("homeassistant/binary_sensor/0x1/contact/config",
                        R"({"name":"Porte","stat_t":"zigbee2mqtt/Porte","val_tpl":"{{ value_json.contact }}",
                            "pl_on":false,"pl_off":true,"dev_cla":"door"})");
    QCOMPARE(u(p["on_value"]), serverValue(R"({"contact":false,"battery":90})", u(p["path"])));
    QCOMPARE(u(p["off_value"]), serverValue(R"({"contact":true,"battery":90})", u(p["path"])));
    //A switch whose payload is a boolean publishes what the device takes.
    p = paramsOf("homeassistant/switch/n/s/config", R"({"name":"S","stat_t":"s","cmd_t":"s/set","pl_on":true,"pl_off":false})");
    QCOMPARE(u(p["on_value"]), QString("true"));
}

void tst_HaDiscovery::entitiesWithoutUniqueIdOfTwoNodesStayApart()
{
    HomeAssistantDiscovery d;
    d.feed("homeassistant/sensor/node_a/temperature/config", R"({"name":"T","stat_t":"a/t"})");
    d.feed("homeassistant/sensor/node_b/temperature/config", R"({"name":"T","stat_t":"b/t"})");
    QCOMPARE(d.entities().size(), 2);
    //The same config again replaces it.
    d.feed("homeassistant/sensor/node_b/temperature/config", R"({"name":"T2","stat_t":"b/t"})");
    QCOMPARE(d.entities().size(), 2);
}

void tst_HaDiscovery::aPrefixMayHoldSlashes()
{
    HomeAssistantDiscovery d("ha/discovery/");
    QCOMPARE(d.prefix(), QString("ha/discovery"));
    QCOMPARE(d.filters(), QStringList({ "ha/discovery/#", "#" }));
    d.feed("ha/discovery/sensor/x/config", R"({"name":"X","stat_t":"x"})");
    //Not under the prefix: not a config.
    d.feed("homeassistant/sensor/y/config", R"({"name":"Y","stat_t":"y"})");
    QCOMPARE(d.entities().size(), 1);
    QCOMPARE(d.entities().first().component, QString("sensor"));
    QCOMPARE(d.entities().first().objectId, QString("x"));
    QCOMPARE(HomeAssistantDiscovery::normalizedPrefix("  "), QString("homeassistant"));
}

void tst_HaDiscovery::theBaseTopicIsExpandedInTheAvailabilityList()
{
    const Entity e = one("homeassistant/sensor/node/t/config",
                         R"({"~":"node","name":"T","stat_t":"~/t","avty":[{"t":"~/status"}]})");
    QCOMPARE(e.config["state_topic"].toString(), QString("node/t"));
    QCOMPARE(e.config["availability"].toArray().first().toObject()["topic"].toString(), QString("node/status"));
    Params p;
    QVERIFY(HomeAssistantDiscovery::ioParams(e, localBroker(), p, nullptr));
    QCOMPARE(u(p["connected_status_topic"]), QString("node/status"));
    QCOMPARE(u(p["connected_status_expr"]), QString("value == 'online'"));
}

void tst_HaDiscovery::abbreviationsAreExpanded()
{
    const Entity e = one("homeassistant/switch/plug_1/relay/config",
                         R"({"name":"Relay","uniq_id":"r1","stat_t":"plug/relay/state","cmd_t":"plug/relay/set",
                             "avty_t":"plug/status","pl_avail":"up","dev":{"ids":["plug-aa"],"name":"Plug","mf":"Acme"}})");
    QCOMPARE(e.component, QString("switch"));
    QCOMPARE(e.nodeId, QString("plug_1"));
    QCOMPARE(e.objectId, QString("relay"));
    QCOMPARE(e.deviceKey, QString("plug-aa"));
    QCOMPARE(e.deviceName, QString("Plug"));
    QCOMPARE(e.displayName(), QString("Plug Relay"));
    QCOMPARE(e.config["state_topic"].toString(), QString("plug/relay/state"));
    QCOMPARE(e.config["unique_id"].toString(), QString("r1"));
    QCOMPARE(e.config["device"].toObject()["manufacturer"].toString(), QString("Acme"));
    Params p;
    QVERIFY(HomeAssistantDiscovery::ioParams(e, localBroker(), p, nullptr));
    QCOMPARE(u(p["connected_status_expr"]), QString("value == 'up'"));

    //A null name is the device's; a name that starts with the device name is kept.
    QCOMPARE(one("homeassistant/light/n/l/config", R"({"name":null,"dev":{"name":"Lampe"}})").displayName(), QString("Lampe"));
    QCOMPARE(one("homeassistant/light/n/l/config", R"({"name":"Lampe salon","dev":{"name":"Lampe"}})").displayName(),
             QString("Lampe salon"));
    QCOMPARE(one("homeassistant/light/n/l/config", R"({"stat_t":"x"})").displayName(), QString("l"));
    //No identifier: the device name groups.
    QCOMPARE(one("homeassistant/light/n/l/config", R"({"name":"L","dev":{"ids":[],"name":"Lampe"}})").deviceKey, QString("Lampe"));
}

void tst_HaDiscovery::anEmptyRetainedConfigDeletesTheEntity()
{
    HomeAssistantDiscovery d;
    d.feed("homeassistant/sensor/n/a/config", R"({"name":"A","stat_t":"a"})");
    d.feed("homeassistant/sensor/n/b/config", R"({"name":"B","stat_t":"b"})");
    d.feed("homeassistant/sensor/n/a/config", "");
    QCOMPARE(d.entities().size(), 1);
    QCOMPARE(d.entities().first().name, QString("B"));
    //A deletion of what was never there changes nothing.
    d.feed("homeassistant/sensor/n/c/config", "");
    QCOMPARE(d.entities().size(), 1);
    //A deletion is not an unreadable config.
    QVERIFY(d.unreadable().isEmpty());
}

void tst_HaDiscovery::aConfigThatIsNotJsonIsCounted()
{
    HomeAssistantDiscovery d;
    d.feed("homeassistant/sensor/n/a/config", "not json");
    d.feed("homeassistant/sensor/n/b/config", "[1,2]");
    d.feed("homeassistant/sensor/n/c/config", R"({"name":"C","stat_t":"c"})");
    QCOMPARE(d.entities().size(), 1);
    QCOMPARE(d.unreadable().size(), 2);
    //Fixed since: read.
    d.feed("homeassistant/sensor/n/a/config", R"({"name":"A","stat_t":"a"})");
    QCOMPARE(d.unreadable(), QStringList({ "homeassistant/sensor/n/b/config" }));
    QCOMPARE(d.entities().size(), 2);
}

void tst_HaDiscovery::availabilityIsReadFromItsTopic()
{
    HomeAssistantDiscovery d;
    d.feed("homeassistant/sensor/n/a/config", R"({"name":"A","stat_t":"a","avty_t":"n/status"})");
    d.feed("homeassistant/sensor/n/b/config", R"({"name":"B","stat_t":"b","avty_t":"m/lwt","pl_avail":"Online","pl_not_avail":"Offline"})");
    d.feed("homeassistant/sensor/n/c/config",
           R"({"name":"C","stat_t":"c","avty":[{"t":"z/bridge","val_tpl":"{{ value_json.state }}"},{"t":"z/c/availability","val_tpl":"{{ value_json.state }}"}]})");
    d.feed("homeassistant/sensor/n/e/config", R"({"name":"E","stat_t":"e"})");
    auto status = [&]()
    {
        QStringList l;
        for (const Entity &e: d.entities())
            l << e.name + "=" + QString::number(e.availability);
        return l.join(' ');
    };
    QCOMPARE(status(), QString("A=0 B=0 C=0 E=0"));
    d.feed("n/status", "online");
    d.feed("m/lwt", "Offline");
    d.feed("z/bridge", R"({"state":"online"})");
    d.feed("z/c/availability", R"({"state":"offline"})");
    QCOMPARE(status(), QString("A=1 B=2 C=2 E=0"));
    //An empty retained message deletes the retained one: unknown again.
    d.feed("m/lwt", "");
    d.feed("z/c/availability", R"({"state":"online"})");
    QCOMPARE(status(), QString("A=1 B=0 C=1 E=0"));

    //The connected status of the IO follows the device's own topic.
    const Entity c = d.entities().at(2);
    Params p;
    QVERIFY(HomeAssistantDiscovery::ioParams(c, localBroker(), p, nullptr));
    QCOMPARE(u(p["connected_status_topic"]), QString("z/c/availability"));
    QCOMPARE(u(p["connected_status_path"]), QString("state"));
    QCOMPARE(u(p["connected_status_expr"]), QString("value == 'online'"));
}

void tst_HaDiscovery::aDeviceConfigHoldsSeveralEntities()
{
    HomeAssistantDiscovery d;
    d.feed("homeassistant/device/plug/config",
           R"({"dev":{"ids":"plug-1","name":"Plug"},"~":"plug","avty_t":"~/status",
               "cmps":{"relay":{"p":"switch","name":"Relay","stat_t":"~/relay","cmd_t":"~/relay/set","avty":[{"t":"~/relay/up"}]},
                       "power":{"p":"sensor","name":"Power","stat_t":"~/power","unit_of_meas":"W","dev_cla":"power"}}})");
    const QList<Entity> l = d.entities();
    QCOMPARE(l.size(), 2);
    QCOMPARE(l.at(0).displayName(), QString("Plug Power"));
    QCOMPARE(l.at(0).component, QString("sensor"));
    QCOMPARE(l.at(1).config["command_topic"].toString(), QString("plug/relay/set"));
    QCOMPARE(l.at(1).config["availability"].toArray().first().toObject()["topic"].toString(), QString("plug/relay/up"));
    Params p;
    QVERIFY(HomeAssistantDiscovery::ioParams(l.at(0), localBroker(), p, nullptr));
    QCOMPARE(u(p["io_style"]), QString("watt"));
    QCOMPARE(u(p["connected_status_topic"]), QString("plug/status"));
    d.feed("homeassistant/device/plug/config", "");
    QVERIFY(d.entities().isEmpty());
}

void tst_HaDiscovery::templatesCalaosCanFollow_data()
{
    QTest::addColumn<QString>("tpl");
    QTest::addColumn<bool>("ok");
    QTest::addColumn<QString>("path");
    QTest::newRow("none") << "" << true << "";
    QTest::newRow("value") << "{{ value }}" << true << "";
    QTest::newRow("key") << "{{ value_json.temperature }}" << true << "temperature";
    QTest::newRow("nested") << "{{value_json.a.b}}" << true << "a/b";
    QTest::newRow("brackets") << "{{ value_json['DS18B20'].Temperature }}" << true << "DS18B20/Temperature";
    QTest::newRow("index") << "{{ value_json.weather[0].description }}" << true << "weather/[0]/description";
    QTest::newRow("filters") << "{{ value_json.t | float(0) | round(1) }}" << true << "t";
    QTest::newRow("if") << "{% if value == '1' %}ON{% endif %}" << false << "";
    QTest::newRow("mapping") << "{{ 'ON' if value_json.s else 'OFF' }}" << false << "";
    QTest::newRow("lower") << "{{ value | lower }}" << false << "";
}

void tst_HaDiscovery::templatesCalaosCanFollow()
{
    QFETCH(QString, tpl);
    QFETCH(bool, ok);
    QFETCH(QString, path);
    bool done = !ok;
    QCOMPARE(HomeAssistantDiscovery::pathOf(tpl, &done), path);
    QCOMPARE(done, ok);

    bool dataOk = false;
    QCOMPARE(HomeAssistantDiscovery::dataOf("{\"mode\": \"{{ value }}\"}", &dataOk), QString("{\"mode\": \"__##VALUE##__\"}"));
    QVERIFY(dataOk);
    QCOMPARE(HomeAssistantDiscovery::dataOf("{{ value | int }}", &dataOk), QString("__##VALUE##__"));
    QVERIFY(dataOk);
    HomeAssistantDiscovery::dataOf("{{ (value * 10) | int }}", &dataOk);
    QVERIFY(!dataOk);
    HomeAssistantDiscovery::dataOf("{% if value %}1{% endif %}", &dataOk);
    QVERIFY(!dataOk);
}

void tst_HaDiscovery::everyKindIsADocumentedIo_data()
{
    QTest::addColumn<QString>("component");
    QTest::addColumn<QByteArray>("config");
    QTest::addColumn<QString>("type");
    QTest::addColumn<QString>("expected"); //param=value, space separated

    QTest::newRow("binary_sensor") << "binary_sensor"
        << QByteArray(R"({"name":"Porte","stat_t":"z/porte","val_tpl":"{{ value_json.contact }}","pl_on":false,"pl_off":true,"dev_cla":"door"})")
        << "MqttInputSwitch" << "topic_sub=z/porte path=contact on_value=false off_value=true io_style=door";
    QTest::newRow("binary_sensor window") << "binary_sensor"
        << QByteArray(R"({"name":"F","stat_t":"f","dev_cla":"window"})") << "MqttInputSwitch" << "io_style=door on_value=ON";
    QTest::newRow("binary_sensor other class") << "binary_sensor"
        << QByteArray(R"({"name":"F","stat_t":"f","dev_cla":"battery"})") << "MqttInputSwitch" << "io_style=";
    QTest::newRow("temperature") << "sensor"
        << QByteArray(R"({"name":"T","stat_t":"n/t","dev_cla":"temperature","unit_of_meas":"°C"})")
        << "MqttInputTemp" << "topic_sub=n/t unit=";
    QTest::newRow("humidity") << "sensor"
        << QByteArray(R"({"name":"H","stat_t":"z/s","val_tpl":"{{ value_json.humidity }}","dev_cla":"humidity","unit_of_meas":"%","sug_dsp_prc":1})")
        << "MqttInputAnalog" << "path=humidity unit=% io_style=humidity precision=1";
    QTest::newRow("energy") << "sensor"
        << QByteArray(R"({"name":"E","stat_t":"e","dev_cla":"energy","unit_of_meas":"kWh","stat_cla":"total_increasing"})")
        << "MqttInputAnalog" << "unit=kWh io_style=";
    QTest::newRow("text") << "sensor" << QByteArray(R"({"name":"V","stat_t":"n/version"})") << "MqttInputString" << "topic_sub=n/version";
    QTest::newRow("enum") << "sensor" << QByteArray(R"({"name":"S","stat_t":"n/s","dev_cla":"enum"})") << "MqttInputString" << "";
    QTest::newRow("switch") << "switch"
        << QByteArray(R"({"name":"Prise","stat_t":"p/state","cmd_t":"p/set","dev_cla":"outlet"})")
        << "MqttOutputLight" << "topic_sub=p/state topic_pub=p/set on_value=ON off_value=OFF data=__##VALUE##__ io_style=outlet";
    QTest::newRow("light") << "light"
        << QByteArray(R"({"name":"L","stat_t":"l/state","cmd_t":"l/set","pl_on":"1","pl_off":"0"})")
        << "MqttOutputLight" << "on_value=1 off_value=0 io_style=";
    QTest::newRow("light json onoff") << "light"
        << QByteArray(R"({"name":"L","schema":"json","sup_clrm":["onoff"],"stat_t":"l","cmd_t":"l/set"})")
        << "MqttOutputLight" << R"(data={"state":"__##VALUE##__"} path=state on_value=ON)";
    QTest::newRow("light json xy") << "light"
        << QByteArray(R"({"name":"L","schema":"json","sup_clrm":["xy","color_temp"],"stat_t":"l","cmd_t":"l/set"})")
        << "MqttOutputLightRGB" << "path_x=color/x path_y=color/y path_brightness=brightness topic_sub=l";
    QTest::newRow("light brightness topic") << "light"
        << QByteArray(R"({"name":"L","stat_t":"l/s","cmd_t":"l/set","bri_cmd_t":"l/bri/set","bri_stat_t":"l/bri","bri_scl":254})")
        << "MqttOutputLightDimmer" << "topic_pub=l/bri/set topic_sub=l/bri out_expr=x~*~2.54";
    QTest::newRow("cover") << "cover"
        << QByteArray(R"({"name":"V","cmd_t":"v/set","stat_t":"v","val_tpl":"{{ value_json.state }}"})")
        << "MqttOutputShutter" << "path=state payload_stop=STOP";
    QTest::newRow("number") << "number"
        << QByteArray(R"({"name":"N","cmd_t":"n/set","stat_t":"n","min":0,"max":1.5,"step":0.1})")
        << "MqttOutputAnalog" << "value_max=1.5 step=0.1";
}

void tst_HaDiscovery::everyKindIsADocumentedIo()
{
    QFETCH(QString, component);
    QFETCH(QByteArray, config);
    QFETCH(QString, type);
    QFETCH(QString, expected);

    QString reason;
    Params p = paramsOf(QStringLiteral("homeassistant/%1/node/obj/config").arg(component), config, &reason);
    QVERIFY2(p.size() > 0, qPrintable(reason));
    QCOMPARE(u(p["type"]), type);
    QCOMPARE(undocumented(p), QStringList());
    QCOMPARE(u(p["host"]), QString("127.0.0.1"));
    QCOMPARE(u(p["port"]), QString("1883"));
    QVERIFY(!p.Exists("user") && !p.Exists("password"));
    QCOMPARE(u(p["io_type"]), QString(type.startsWith("MqttInput") ? "input" : "output"));
    //~ for a space in a value.
    for (const QString &kv: expected.split(' ', Qt::SkipEmptyParts))
    {
        const QString k = kv.section('=', 0, 0);
        QCOMPARE(k + "=" + u(p[k.toStdString()]), k + "=" + kv.section('=', 1).replace('~', ' '));
    }

    //Mapping says the same type.
    const HomeAssistantDiscovery::Mapping m =
        HomeAssistantDiscovery::mappingOf(one(QStringLiteral("homeassistant/%1/node/obj/config").arg(component), config), {});
    QVERIFY(m.ok());
    QVERIFY(!m.device);
    QCOMPARE(m.what, type);
}

void tst_HaDiscovery::whatCannotBeImportedSaysWhy_data()
{
    QTest::addColumn<QString>("component");
    QTest::addColumn<QByteArray>("config");
    QTest::addColumn<QString>("reason");
    QTest::newRow("select") << "select" << QByteArray(R"({"name":"S","cmd_t":"s"})") << "Calaos does not import select entities";
    QTest::newRow("no state") << "sensor" << QByteArray(R"({"name":"S"})") << "it has no state topic";
    QTest::newRow("optimistic switch") << "switch" << QByteArray(R"({"name":"S","cmd_t":"s"})") << "it does not report its state";
    QTest::newRow("state values") << "switch" << QByteArray(R"({"name":"S","cmd_t":"s/set","stat_t":"s","stat_on":"on"})")
                                  << "it reports other values than the ones it takes";
    QTest::newRow("template light") << "light" << QByteArray(R"({"name":"L","schema":"template","cmd_t":"l"})")
                                    << "lights of the template schema are not imported";
    QTest::newRow("template value") << "sensor" << QByteArray(R"({"name":"S","stat_t":"s","val_tpl":"{{ value | lower }}"})")
                                    << "its value goes through a template Calaos cannot follow";
    QTest::newRow("number template") << "number"
        << QByteArray(R"({"name":"N","stat_t":"n","cmd_t":"n/set","cmd_tpl":"{{ value * 10 }}"})")
        << "its value goes through a template Calaos cannot follow";
}

void tst_HaDiscovery::whatCannotBeImportedSaysWhy()
{
    QFETCH(QString, component);
    QFETCH(QByteArray, config);
    QFETCH(QString, reason);
    QString why;
    Params p = paramsOf(QStringLiteral("homeassistant/%1/node/obj/config").arg(component), config, &why);
    QCOMPARE(p.size(), 0);
    QCOMPARE(why, reason);
    QCOMPARE(HomeAssistantDiscovery::mappingOf(one(QStringLiteral("homeassistant/%1/node/obj/config").arg(component), config),
                                               shippedTemplates()).reason, reason);
}

void tst_HaDiscovery::theProjectCredentialsAreWritten()
{
    MqttBroker b;
    b.host = "192.168.1.10";
    b.port = 1884;
    b.user = "calaos";
    b.password = "s3cret";
    Params p;
    QVERIFY(HomeAssistantDiscovery::ioParams(one("homeassistant/sensor/n/t/config", R"({"name":"T","stat_t":"t"})"), b, p, nullptr));
    QCOMPARE(u(p["host"]), QString("192.168.1.10"));
    QCOMPARE(u(p["port"]), QString("1884"));
    QCOMPARE(u(p["user"]), QString("calaos"));
    QCOMPARE(u(p["password"]), QString("s3cret"));
}

void tst_HaDiscovery::twoSetpointsAreImported()
{
    Device *dev = importClimate(BUANDERIE_TOPIC, buanderieConfig());
    QVERIFY(dev);
    QCOMPARE(u(dev->family()), QString("climate"));
    QCOMPARE(u(dev->impl()), QString("proxy"));
    QCOMPARE(undocumented(dev), QStringList());
    QVERIFY(!linked(dev, "target_temp"));
    QCOMPARE(param(linked(dev, "target_temp_low"), "topic_pub"), P + "target_temperature_low/command");
    QCOMPARE(param(linked(dev, "target_temp_high"), "topic_sub"), P + "target_temperature_high/state");
    QCOMPARE(param(linked(dev, "target_temp_low"), "value_min"), QString("17"));
    QCOMPARE(param(linked(dev, "target_temp_low"), "step"), QString("0.5"));
    //No loose IO: everything belongs to the device.
    QCOMPARE(int(dev->ownedIos().size()), 7);
}

void tst_HaDiscovery::anEsphomeClimateIsTheDeviceOfTheEsphomePanel()
{
    //The same climate through the ESPHome panel of the template dialog.
    EsphomeDiscovery esphome;
    esphome.feed(BUANDERIE_TOPIC, buanderieConfig());
    QCOMPARE(esphome.climates().size(), 1);
    const QJsonObject tpl = shippedTemplate("esphome_climate");
    QVariantMap v = DeviceTemplates::defaults(tpl, roomNamed("Salon"));
    v["mqtt_host"] = "127.0.0.1";
    v = EsphomeDiscovery::prefill(tpl, esphome.climates().first(), v);
    QVERIFY(DeviceTemplates::create(tpl, v));
    const QStringList panel = deviceElement(saveIo(work("panel.xml")), "clim_mitsu_buanderie");
    QVERIFY(!panel.isEmpty());

    QVERIFY(loadIo(work("house.xml"), houseXml()));
    QStringList notes;
    QVERIFY(importClimate(BUANDERIE_TOPIC, buanderieConfig(), &notes));
    QCOMPARE(deviceElement(saveIo(work("ha.xml")), "clim_mitsu_buanderie"), panel);
    QVERIFY2(notes.join(' ').contains("Two setpoints (target_temperature_low and _high)."), qPrintable(notes.join(' ')));
    QVERIFY(!notes.join(' ').contains("room sensor"));
    checkRoundTrip("esphome_dual");
}

void tst_HaDiscovery::aClimateWithOneSetpoint()
{
    Device *dev = importClimate(SALON_TOPIC, salonConfig());
    QVERIFY(dev);
    QCOMPARE(undocumented(dev), QStringList());
    QCOMPARE(u(dev->get_param("name")), QString("Clim salon"));
    QVERIFY(!linked(dev, "target_temp_low"));
    QCOMPARE(param(linked(dev, "target_temp"), "topic_pub"), QString("salon/climate/clim_salon/target_temperature/command"));
    QCOMPARE(param(linked(dev, "mode"), "topic_sub"), QString("salon/climate/clim_salon/mode/state"));
    QCOMPARE(param(linked(dev, "mode"), "connected_status_topic"), QString("salon/status"));
    QCOMPARE(param(linked(dev, "current_temp"), "topic_sub"), QString("salon/climate/clim_salon/current_temperature/state"));
    //Home Assistant's heat_cool is Calaos' auto, sent as heat_cool.
    QCOMPARE(deviceParam(dev, "modes"), QString("off,heat,cool,auto"));
    QCOMPARE(deviceParam(dev, "auto_payload"), QString("heat_cool"));
    QVERIFY(!linked(dev, "action"));
    QVERIFY(!linked(dev, "fan_mode"));
    checkRoundTrip("esphome_single");
}

void tst_HaDiscovery::aZigbeeThermostatKeepsItsTopicsAndPaths()
{
    QStringList notes;
    Device *dev = importClimate(TRV_TOPIC, trvConfig(), &notes);
    QVERIFY2(dev, qPrintable(notes.join('\n')));
    QCOMPARE(undocumented(dev), QStringList());
    QCOMPARE(u(dev->get_param("name")), QString("Vanne chambre"));
    IOBase *mode = linked(dev, "mode");
    QCOMPARE(param(mode, "topic_pub"), QString("zigbee2mqtt/Vanne chambre/set/system_mode"));
    QCOMPARE(param(mode, "topic_sub"), QString("zigbee2mqtt/Vanne chambre"));
    QCOMPARE(param(mode, "path"), QString("system_mode"));
    QCOMPARE(param(mode, "data"), QString("__##VALUE##__"));
    QCOMPARE(param(mode, "connected_status_topic"), QString("zigbee2mqtt/Vanne chambre/availability"));
    QCOMPARE(param(mode, "connected_status_path"), QString("state"));
    IOBase *setpoint = linked(dev, "target_temp");
    QCOMPARE(param(setpoint, "topic_pub"), QString("zigbee2mqtt/Vanne chambre/set/current_heating_setpoint"));
    QCOMPARE(param(setpoint, "path"), QString("current_heating_setpoint"));
    QCOMPARE(param(setpoint, "value_min"), QString("5"));
    QCOMPARE(param(linked(dev, "current_temp"), "path"), QString("local_temperature"));
    //Its action maps the valve's values in a template: left out, said.
    QVERIFY(!linked(dev, "action"));
    QVERIFY2(notes.join(' ').contains("Not imported: action (its action goes through a template Calaos cannot follow)."),
             qPrintable(notes.join(' ')));
    //The relay of a room sensor publishes on the ESPHome layout only.
    QVERIFY2(notes.join(' ').contains("No room sensor relay: Calaos publishes it on the topics of an ESPHome node only."),
             qPrintable(notes.join(' ')));
    //Home Assistant's auto is a schedule of the valve: not a Calaos mode.
    QCOMPARE(deviceParam(dev, "modes"), QString("off,heat"));
    QVERIFY2(notes.join(' ').contains("Not offered by this template: auto."), qPrintable(notes.join(' ')));
    checkRoundTrip("zigbee_trv");
}

void tst_HaDiscovery::aClimateWithoutSensorTakesTheIrTemplate()
{
    QByteArray ir = salonConfig();
    ir.replace(R"("curr_temp_t":"~/current_temperature/state",)", "");
    ir.replace(R"("avty_t":"salon/status",)", R"("act_t":"~/action/state",)");
    QStringList notes;
    Device *dev = importClimate(SALON_TOPIC, ir, &notes);
    QVERIFY(dev);
    QCOMPARE(undocumented(dev), QStringList());
    QVERIFY(!linked(dev, "current_temp"));
    QVERIFY(!linked(dev, "action"));
    QVERIFY(linked(dev, "target_temp"));
    //No availability: no connected status.
    QCOMPARE(param(linked(dev, "mode"), "connected_status_topic"), QString());
    QVERIFY(notes.join(' ').contains("Not imported: action"));
    const HomeAssistantDiscovery::Mapping m = HomeAssistantDiscovery::mappingOf(one(SALON_TOPIC, ir), shippedTemplates());
    QVERIFY(m.device);
    QCOMPARE(m.what, QString("ESPHome IR climate (MQTT)"));
}

void tst_HaDiscovery::aClimateCalaosCannotDriveIsRefused_data()
{
    QTest::addColumn<QByteArray>("config");
    QTest::addColumn<QString>("reason");
    QTest::newRow("fahrenheit") << QByteArray(salonConfig()).replace(R"("min_temp")", R"("temp_unit":"F","min_temp")")
                                << "its temperatures are in °F, Calaos climates in °C";
    QTest::newRow("no mode") << QByteArray(salonConfig()).replace(R"("mode_cmd_t":"~/mode/command",)", "")
                             << "it has no mode topics";
    QTest::newRow("optimistic mode") << QByteArray(salonConfig()).replace(R"("mode_stat_t":"~/mode/state",)", "")
                                     << "it does not report its mode";
    QTest::newRow("no setpoint") << QByteArray(salonConfig()).replace(R"("temp_cmd_t":"~/target_temperature/command",)", "")
                                 << "it has no setpoint topics";
    QTest::newRow("mode template") << QByteArray(salonConfig()).replace(R"("modes")", R"("mode_stat_tpl":"{{ value | lower }}","modes")")
                                   << "its mode goes through a template Calaos cannot follow";
    QByteArray dualIr = buanderieConfig();
    dualIr.replace(R"("curr_temp_t":"clim-buanderie/climate/clim_mitsu_buanderie/current_temperature/state",)", "");
    QTest::newRow("dual without sensor") << dualIr << "two setpoints without a current temperature: no template takes that";
}

void tst_HaDiscovery::aClimateCalaosCannotDriveIsRefused()
{
    QFETCH(QByteArray, config);
    QFETCH(QString, reason);
    QStringList notes;
    QVERIFY(!importClimate(SALON_TOPIC, config, &notes));
    QCOMPARE(notes, QStringList({ QStringLiteral("%1: not imported, %2.").arg(one(SALON_TOPIC, config).displayName(), reason) }));
    QCOMPARE(HomeAssistantDiscovery::mappingOf(one(SALON_TOPIC, config), shippedTemplates()).reason, reason);
    //Without the ESPHome templates, nothing to make the device with.
    if (QTest::currentDataTag() == QLatin1String("fahrenheit"))
        QCOMPARE(HomeAssistantDiscovery::mappingOf(one(SALON_TOPIC, salonConfig()), QJsonArray()).reason,
                 QString("the device template esphome_climate is missing from this installer"));
}

void tst_HaDiscovery::whatTheProjectHasIsSkipped()
{
    HomeAssistantDiscovery d;
    d.feed(SALON_TOPIC, salonConfig());
    d.feed("homeassistant/sensor/salon/t/config", R"({"name":"T","stat_t":"z/salon","val_tpl":"{{ value_json.temperature }}","dev_cla":"temperature"})");
    d.feed("homeassistant/sensor/salon/h/config", R"({"name":"H","stat_t":"z/salon","val_tpl":"{{ value_json.humidity }}","unit_of_meas":"%"})");
    //The fixture house reads s/t already.
    d.feed("homeassistant/sensor/salon/old/config", R"({"name":"Old","stat_t":"s/t"})");
    const HomeAssistantDiscovery::Imported first = HomeAssistantDiscovery::import(d.entities(), shippedTemplates(), localBroker(),
                                                                                 roomNamed("Cave"));
    QCOMPARE(int(first.devices.size()), 1);
    QCOMPARE(int(first.ios.size()), 2);
    QCOMPARE(first.notes.filter("already in the project").size(), 1);
    QCOMPARE(roomNamed("Cave")->get_size_in() + roomNamed("Cave")->get_size_out() > 0, true);

    const HomeAssistantDiscovery::Imported again = HomeAssistantDiscovery::import(d.entities(), shippedTemplates(), localBroker(),
                                                                                 roomNamed("Salon"));
    QVERIFY(again.devices.empty());
    QVERIFY(again.ios.empty());
    QCOMPARE(again.notes.filter("already in the project").size(), 4);
}

void tst_HaDiscovery::importedIosAndDevicesSurviveASaveAndReload()
{
    HomeAssistantDiscovery d;
    d.feed(TRV_TOPIC, trvConfig());
    d.feed("homeassistant/binary_sensor/n/porte/config", R"({"name":"Porte","stat_t":"z/porte","val_tpl":"{{ value_json.contact }}","pl_on":false,"pl_off":true})");
    d.feed("homeassistant/light/n/l/config", R"({"name":"Lampe","schema":"json","sup_clrm":["xy"],"stat_t":"z/l","cmd_t":"z/l/set"})");
    d.feed("homeassistant/cover/n/v/config", R"({"name":"Volet","cmd_t":"v/set","stat_t":"v"})");
    d.feed("homeassistant/number/n/n/config", R"({"name":"Consigne","cmd_t":"n/set","stat_t":"n","min":0,"max":10})");
    d.feed("homeassistant/switch/n/s/config", R"({"name":"Prise","cmd_t":"s/set","stat_t":"s","avty_t":"n/status"})");
    MqttBroker b = localBroker();
    b.user = "calaos";
    b.password = "s3cret";
    const HomeAssistantDiscovery::Imported imported = HomeAssistantDiscovery::import(d.entities(), shippedTemplates(), b,
                                                                                    roomNamed("Salon"));
    QCOMPARE(int(imported.devices.size()), 1);
    QCOMPARE(int(imported.ios.size()), 5);
    QStringList ids;
    for (IOBase *io: imported.ios)
    {
        QCOMPARE(undocumented(io->get_params()), QStringList());
        QCOMPARE(param(io, "password"), QString("s3cret"));
        ids << param(io, "id");
    }
    QCOMPARE(ids, QStringList({ "consigne", "lampe", "porte", "prise", "volet" }));
    QCOMPARE(undocumented(imported.devices.front()), QStringList());

    const QByteArray xml = saveIo(work("all.xml"));
    QVERIFY(xml.contains("connected_status_topic=\"n/status\""));
    checkRoundTrip("all_kinds");
    QVERIFY(ListeRoom::Instance().get_device("vanne_chambre"));
    QVERIFY(DeviceTemplates::ioById("prise"));
}

void tst_HaDiscovery::theDialogStartsOnTheChosenRoom()
{
    DialogHomeAssistantMqtt d(shippedTemplates(), roomNamed("Cave"));
    QCOMPARE(d.room(), roomNamed("Cave"));
    QCOMPARE(d.findChild<QComboBox *>("room")->count(), 2);
    QCOMPARE(d.findChild<QLineEdit *>("prefix")->text(), QString("homeassistant"));
}

namespace
{

QList<QPair<QString, QByteArray>> retainedOfAHouse()
{
    return { { SALON_TOPIC, salonConfig() },
             { "salon/status", "online" },
             { "homeassistant/sensor/salon/temperature/config",
               R"({"name":"Temperature","stat_t":"salon/sensor/temperature/state","dev_cla":"temperature","avty_t":"salon/status","dev":{"ids":"d1","name":"salon"}})" },
             { "homeassistant/switch/garage/relay/config",
               R"({"name":"Relay","stat_t":"garage/relay","cmd_t":"garage/relay/set","avty_t":"garage/status","dev":{"ids":"g1","name":"garage"}})" },
             { "garage/status", "offline" },
             { "homeassistant/select/garage/mode/config", R"({"name":"Mode","cmd_t":"garage/mode","dev":{"ids":"g1","name":"garage"}})" },
             { "homeassistant/sensor/salon/broken/config", "{" },
             { "homeassistant/sensor/salon/old/config", R"({"name":"Old","stat_t":"s/t","dev":{"ids":"d1","name":"salon"}})" } };
}

void searchOn(DialogHomeAssistantMqtt &d, quint16 port, const QString &host = "127.0.0.1")
{
    d.findChild<QSpinBox *>("serverPort")->setValue(port);
    QLineEdit *probe = d.findChild<QLineEdit *>("probeHost");
    probe->clear();
    QTest::keyClicks(probe, host);
    d.setTimings(300, 2000);
    QSignalSpy done(&d, &DialogHomeAssistantMqtt::searchFinished);
    QTest::mouseClick(d.findChild<QPushButton *>("searchButton"), Qt::LeftButton);
    QVERIFY(!d.findChild<QPushButton *>("searchButton")->isEnabled());
    QTRY_COMPARE_WITH_TIMEOUT(done.count(), 1, 10000);
}

QTreeWidgetItem *entityItem(DialogHomeAssistantMqtt &d, const QString &name)
{
    QTreeWidget *list = d.findChild<QTreeWidget *>("entityList");
    for (int i = 0;i < list->topLevelItemCount();i++)
        for (int c = 0;c < list->topLevelItem(i)->childCount();c++)
            if (list->topLevelItem(i)->child(c)->text(0) == name)
                return list->topLevelItem(i)->child(c);
    return nullptr;
}

QString statusText(DialogHomeAssistantMqtt &d)
{
    return d.findChild<QLabel *>("status")->text();
}

}

void tst_HaDiscovery::searchingListsTheEntitiesByDevice()
{
    FakeMqttBroker broker;
    broker.retained = retainedOfAHouse();
    const quint16 port = broker.start();

    DialogHomeAssistantMqtt d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(!d.findChild<QTreeWidget *>("entityList")->isVisible());
    searchOn(d, port);
    QCOMPARE(broker.filters, QStringList({ "homeassistant/#", "#" }));

    QCOMPARE(statusText(d), QString("Entities found: 5. Check the ones to import. Discovery configs that could not be read: 1."));
    QTreeWidget *list = d.findChild<QTreeWidget *>("entityList");
    QVERIFY(list->isVisible());
    QCOMPARE(list->topLevelItemCount(), 2);
    QCOMPARE(list->topLevelItem(0)->text(0), QString("garage"));
    QCOMPARE(list->topLevelItem(1)->text(0), QString("salon"));
    QCOMPARE(list->topLevelItem(1)->childCount(), 3);

    QTreeWidgetItem *clim = entityItem(d, "Clim salon");
    QVERIFY(clim);
    QCOMPARE(clim->text(1), QString("climate"));
    QCOMPARE(clim->text(2), QString("ESPHome climate (MQTT)"));
    QCOMPARE(clim->text(3), QString("online"));
    QVERIFY(clim->flags() & Qt::ItemIsUserCheckable);
    QCOMPARE(clim->foreground(0).color() == d.palette().color(QPalette::Disabled, QPalette::Text), false);

    QTreeWidgetItem *relay = entityItem(d, "Relay");
    QCOMPARE(relay->text(2), QString("MqttOutputLight"));
    QCOMPARE(relay->text(3), QString("offline"));
    QCOMPARE(relay->foreground(0).color(), d.palette().color(QPalette::Disabled, QPalette::Text));
    QVERIFY(relay->flags() & Qt::ItemIsUserCheckable);

    QTreeWidgetItem *select = entityItem(d, "Mode");
    QCOMPARE(select->text(3), QString("not imported: Calaos does not import select entities"));
    QVERIFY(!(select->flags() & Qt::ItemIsUserCheckable));

    QTreeWidgetItem *old = entityItem(d, "Old");
    QCOMPARE(old->text(3), QString("already in the project"));
    QVERIFY(!(old->flags() & Qt::ItemIsUserCheckable));

    //Nothing checked: nothing to import; checking the device checks its entities.
    QPushButton *import = d.findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok);
    QVERIFY(!import->isEnabled());
    list->topLevelItem(1)->setCheckState(0, Qt::Checked);
    QCOMPARE(clim->checkState(0), Qt::Checked);
    QVERIFY(import->isEnabled());
}

void tst_HaDiscovery::theProjectCredentialsOnlyGoToItsBroker()
{
    const QByteArray xml = R"(<?xml version="1.0"?>
<calaos:ioconfig xmlns:calaos="http://www.calaos.fr"><calaos:home format="2">
<calaos:room name="Salon" type="salon" hits="0">
  <calaos:input id="t" type="MqttInputTemp" name="T" host="192.168.1.10" user="calaos" password="s3cret" topic_sub="t"/>
</calaos:room></calaos:home></calaos:ioconfig>)";
    QVERIFY(loadIo(work("creds.xml"), xml));
    FakeMqttBroker broker;
    const quint16 port = broker.start();

    DialogHomeAssistantMqtt d(shippedTemplates(), roomNamed("Salon"));
    QCOMPARE(d.findChild<QLineEdit *>("serverHost")->text(), QString("192.168.1.10"));
    QCOMPARE(d.findChild<QLineEdit *>("serverPassword")->text(), QString("s3cret"));
    QCOMPARE(d.findChild<QLineEdit *>("probeHost")->text(), QString("192.168.1.10"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));

    //Another host than the project's broker: its password stays home.
    searchOn(d, port);
    QCOMPARE(broker.connects, 1);
    QCOMPARE(broker.user, QString());
    QVERIFY(!broker.hadPassword);

    //Typed by the user for this search: sent.
    QLineEdit *user = d.findChild<QLineEdit *>("serverUser");
    user->clear();
    QTest::keyClicks(user, "probe");
    searchOn(d, port);
    QCOMPARE(broker.user, QString("probe"));
    QCOMPARE(broker.password, QString("s3cret"));

    //The project's broker on the box, reached as the box: its credentials go.
    const QByteArray local = QByteArray(xml).replace("192.168.1.10", "127.0.0.1");
    QVERIFY(loadIo(work("creds2.xml"), local));
    ConfigOptions::Instance().addAuth({ "u", "p", "127.0.0.1" });
    ConfigOptions::Instance().setCurrentAuthIndex(0);
    DialogHomeAssistantMqtt box(shippedTemplates(), roomNamed("Salon"));
    QCOMPARE(box.findChild<QLineEdit *>("probeHost")->text(), QString("127.0.0.1"));
    box.show();
    QVERIFY(QTest::qWaitForWindowExposed(&box));
    searchOn(box, port);
    QCOMPARE(broker.user, QString("calaos"));
    QCOMPARE(broker.password, QString("s3cret"));
    ConfigOptions::Instance().deleteAuth("127.0.0.1");
}

void tst_HaDiscovery::brokerErrorsAreShown()
{
    QTcpServer closed;
    QVERIFY(closed.listen(QHostAddress::LocalHost, 0));
    const quint16 port = closed.serverPort();
    closed.close();

    DialogHomeAssistantMqtt d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    searchOn(d, port);
    QCOMPARE(statusText(d), QString("Nothing answers on 127.0.0.1:%1: check that the broker runs and listens on this port.").arg(port));
    QVERIFY(d.findChild<QLabel *>("status")->styleSheet().contains("color"));
    QVERIFY(d.findChild<QPushButton *>("searchButton")->isEnabled());

    FakeMqttBroker refusing;
    refusing.requiredUser = "calaos";
    refusing.requiredPassword = "s3cret";
    searchOn(d, refusing.start());
    QCOMPARE(statusText(d), QString("The broker at 127.0.0.1:%1 refused the user name or the password.").arg(refusing.serverPort()));

    FakeMqttBroker silent;
    silent.answerConnect = false;
    searchOn(d, silent.start());
    QCOMPARE(statusText(d), QString("No answer from the broker at 127.0.0.1:%1 within 2 s.").arg(silent.serverPort()));

    //No address: asked for, nothing sent.
    d.findChild<QLineEdit *>("probeHost")->clear();
    QTest::mouseClick(d.findChild<QPushButton *>("searchButton"), Qt::LeftButton);
    QCOMPARE(statusText(d), QString("Type the address of the broker as this computer reaches it."));
    QCOMPARE(silent.connects, 1);
}

void tst_HaDiscovery::anEmptyBrokerSaysSo()
{
    FakeMqttBroker broker;
    broker.retained = { { "zigbee2mqtt/bridge/state", "online" } };
    DialogHomeAssistantMqtt d(shippedTemplates(), roomNamed("Salon"));
    d.findChild<QLineEdit *>("prefix")->setText("ha/");
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    searchOn(d, broker.start());
    QCOMPARE(statusText(d), QString("No Home Assistant discovery config under ha/ on this broker. Check the discovery prefix, "
                                    "and that the devices have discovery on."));
    QVERIFY(!d.findChild<QTreeWidget *>("entityList")->isVisible());
    QCOMPARE(broker.filters.first(), QString("ha/#"));
}

void tst_HaDiscovery::importingCreatesTheCheckedEntities()
{
    FakeMqttBroker broker;
    broker.retained = retainedOfAHouse();
    const quint16 port = broker.start();

    DialogHomeAssistantMqtt d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    searchOn(d, port);
    d.findChild<QComboBox *>("room")->setCurrentIndex(1);
    entityItem(d, "Clim salon")->setCheckState(0, Qt::Checked);
    entityItem(d, "Relay")->setCheckState(0, Qt::Checked);
    d.findChild<QLineEdit *>("prefix")->setText("changed");
    QTest::mouseClick(d.findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok), Qt::LeftButton);

    QCOMPARE(d.result(), int(QDialog::Accepted));
    QCOMPARE(d.room(), roomNamed("Cave"));
    QCOMPARE(int(d.imported().devices.size()), 1);
    QCOMPARE(int(d.imported().ios.size()), 1);
    Device *dev = d.imported().devices.front();
    //The project's broker as the server reaches it, with the port searched.
    QCOMPARE(param(linked(dev, "mode"), "host"), QString("h"));
    QCOMPARE(param(linked(dev, "mode"), "port"), QString::number(port));
    QCOMPARE(param(d.imported().ios.front(), "topic_pub"), QString("garage/relay/set"));
    QCOMPARE(ListeRoom::Instance().searchRoomByOutput(d.imported().ios.front()), roomNamed("Cave"));
    //The prefix searched is remembered, not the one typed after.
    QCOMPARE(ConfigOptions::Instance().getHADiscoveryPrefix(), QString("homeassistant"));
}

void tst_HaDiscovery::enterInTheAddressSearches()
{
    FakeMqttBroker broker;
    const quint16 port = broker.start();
    DialogHomeAssistantMqtt d(shippedTemplates(), roomNamed("Salon"));
    d.setTimings(300, 2000);
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    d.findChild<QSpinBox *>("serverPort")->setValue(port);
    QLineEdit *probe = d.findChild<QLineEdit *>("probeHost");
    probe->setText("127.0.0.1");
    probe->setFocus();
    QSignalSpy done(&d, &DialogHomeAssistantMqtt::searchFinished);
    QTest::keyClick(probe, Qt::Key_Return);
    QTRY_COMPARE_WITH_TIMEOUT(done.count(), 1, 10000);
    QVERIFY(d.isVisible());
    QCOMPARE(broker.connects, 1);
}


QTEST_MAIN(tst_HaDiscovery)

#include "tst_hadiscovery.moc"
