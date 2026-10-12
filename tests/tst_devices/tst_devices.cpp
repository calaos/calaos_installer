#include <QtTest>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTimer>

#include "projectmanager.h"

using namespace Calaos;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

QByteArray readAll(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return QByteArray();
    return f.readAll();
}

bool writeAll(const QString &path, const QByteArray &data)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
        return false;
    f.write(data);
    return true;
}

QByteArray ioSource(const QString &inner, const QString &homeAttrs = QString())
{
    return ("<?xml version=\"1.0\"?>\n"
            "<calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">\n<calaos:home" + homeAttrs + ">\n" +
            inner + "\n</calaos:home>\n</calaos:ioconfig>\n").toUtf8();
}

QString ioDoc(const QString &inner, const QString &homeAttrs = QString())
{
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">\n"
           "    <calaos:home" + homeAttrs + ">\n" + inner +
           "    </calaos:home>\n</calaos:ioconfig>\n";
}

QByteArray rulesSource(const QString &inner)
{
    return ("<?xml version=\"1.0\"?>\n<calaos:rules xmlns:calaos=\"http://www.calaos.fr\">\n" +
            inner + "\n</calaos:rules>\n").toUtf8();
}

void resetModel()
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();
}

QStringList ids(const std::vector<IOBase *> &ios)
{
    QStringList l;
    for (IOBase *io: ios)
        l << u(io->get_param("id"));
    return l;
}

QStringList ruleNames()
{
    QStringList l;
    for (int i = 0;i < ListeRule::Instance().size();i++)
        l << u(ListeRule::Instance().get_rule(i)->get_name());
    return l;
}

// Spec §3.1, with the elided attributes filled in.
const char *kSpecExample = R"(
<calaos:room name="Salon" type="livingroom" hits="0">

  <calaos:input id="salon_temp" type="MqttInputTemp" name="Température salon" topic_sub="salon/temp"/>

  <!-- proxy: Mitsubishi CN105 via ESPHome, regulating against the room sensor -->
  <calaos:device id="salon_ac" type="climate" impl="proxy" name="Clim salon">
    <calaos:input  id="salon_ac_mode"       type="MqttInputString"  visible="false" name="mode"/>
    <calaos:output id="salon_ac_mode_cmd"   type="MqttOutputString" visible="false" name="mode_cmd"/>
    <calaos:input  id="salon_ac_current"    type="MqttInputTemp"    visible="false" name="current"/>
    <calaos:output id="salon_ac_target"     type="MqttOutputAnalog" visible="false" value_min="16" value_max="30" step="0.5" name="target"/>
    <calaos:input  id="salon_ac_action"     type="MqttInputString"  visible="false" name="action"/>
    <calaos:output id="salon_ac_remotetemp" type="MqttOutputAnalog" visible="false" name="remotetemp"/>

    <calaos:link name="mode"         io="salon_ac_mode" command_io="salon_ac_mode_cmd"/>
    <calaos:link name="current_temp" io="salon_ac_current"/>
    <calaos:link name="target_temp"  io="salon_ac_target"/>
    <calaos:link name="action"       io="salon_ac_action"/>
    <calaos:link name="remote_temp"  io="salon_ac_remotetemp"/>
    <calaos:link name="remote_temp_source" io="salon_temp"/>   <!-- external -->
    <calaos:param name="remote_temp_keepalive" value="120"/>
  </calaos:device>

  <!-- thermostat: reversible floor heat pump on two relays -->
  <calaos:device id="salon_floor" type="climate" impl="thermostat" name="Plancher salon">
    <calaos:internal id="salon_floor_target"      type="InternalInt"    save="true" visible="false"/>
    <calaos:internal id="salon_floor_target_low"  type="InternalInt"    save="true" visible="false"/>
    <calaos:internal id="salon_floor_target_high" type="InternalInt"    save="true" visible="false"/>
    <calaos:internal id="salon_floor_action"      type="InternalString" visible="false"/>
    <calaos:output   id="salon_floor_heat" type="WODigital" visible="false" host="10.0.0.5" var="1"/>
    <calaos:output   id="salon_floor_cool" type="WODigital" visible="false" host="10.0.0.5" var="2"/>

    <calaos:link name="current_temp"     io="salon_temp"/>      <!-- external -->
    <calaos:link name="humidity"         io="salon_humidity"/>  <!-- external -->
    <calaos:link name="target_temp"      io="salon_floor_target"/>
    <calaos:link name="target_temp_low"  io="salon_floor_target_low"/>
    <calaos:link name="target_temp_high" io="salon_floor_target_high"/>
    <calaos:link name="action"           io="salon_floor_action"/>
    <calaos:link name="output_heat"      io="salon_floor_heat"/>
    <calaos:link name="output_cool"      io="salon_floor_cool"/>
    <calaos:param name="emitter"        value="floor"/>
    <calaos:param name="hysteresis"     value="0.5"/>
    <calaos:param name="min_on_time"    value="900"/>
    <calaos:param name="min_off_time"   value="900"/>
    <calaos:param name="min_range"      value="2"/>
    <calaos:param name="sensor_timeout" value="1800"/>
    <calaos:param name="humidity_max"   value="65"/>
  </calaos:device>

</calaos:room>)";

// What the installer writes for it: free IOs first, then the devices with
// their owned IOs in file order, links, then params, attributes sorted.
const char *kSpecWritten = R"(        <calaos:room name="Salon" type="livingroom" hits="0">
            <calaos:input id="salon_temp" name="Température salon" topic_sub="salon/temp" type="MqttInputTemp"/>
            <calaos:device id="salon_ac" impl="proxy" name="Clim salon" type="climate">
                <calaos:input id="salon_ac_mode" name="mode" type="MqttInputString" visible="false"/>
                <calaos:output id="salon_ac_mode_cmd" name="mode_cmd" type="MqttOutputString" visible="false"/>
                <calaos:input id="salon_ac_current" name="current" type="MqttInputTemp" visible="false"/>
                <calaos:output id="salon_ac_target" name="target" step="0.5" type="MqttOutputAnalog" value_max="30" value_min="16" visible="false"/>
                <calaos:input id="salon_ac_action" name="action" type="MqttInputString" visible="false"/>
                <calaos:output id="salon_ac_remotetemp" name="remotetemp" type="MqttOutputAnalog" visible="false"/>
                <calaos:link name="mode" io="salon_ac_mode" command_io="salon_ac_mode_cmd"/>
                <calaos:link name="current_temp" io="salon_ac_current"/>
                <calaos:link name="target_temp" io="salon_ac_target"/>
                <calaos:link name="action" io="salon_ac_action"/>
                <calaos:link name="remote_temp" io="salon_ac_remotetemp"/>
                <calaos:link name="remote_temp_source" io="salon_temp"/>
                <calaos:param name="remote_temp_keepalive" value="120"/>
            </calaos:device>
            <calaos:device id="salon_floor" impl="thermostat" name="Plancher salon" type="climate">
                <calaos:internal id="salon_floor_target" name="Input" save="true" type="InternalInt" visible="false"/>
                <calaos:internal id="salon_floor_target_low" name="Input" save="true" type="InternalInt" visible="false"/>
                <calaos:internal id="salon_floor_target_high" name="Input" save="true" type="InternalInt" visible="false"/>
                <calaos:internal id="salon_floor_action" name="Input" type="InternalString" visible="false"/>
                <calaos:output host="10.0.0.5" id="salon_floor_heat" name="Output" port="502" type="WODigital" var="1" visible="false"/>
                <calaos:output host="10.0.0.5" id="salon_floor_cool" name="Output" port="502" type="WODigital" var="2" visible="false"/>
                <calaos:link name="current_temp" io="salon_temp"/>
                <calaos:link name="humidity" io="salon_humidity"/>
                <calaos:link name="target_temp" io="salon_floor_target"/>
                <calaos:link name="target_temp_low" io="salon_floor_target_low"/>
                <calaos:link name="target_temp_high" io="salon_floor_target_high"/>
                <calaos:link name="action" io="salon_floor_action"/>
                <calaos:link name="output_heat" io="salon_floor_heat"/>
                <calaos:link name="output_cool" io="salon_floor_cool"/>
                <calaos:param name="emitter" value="floor"/>
                <calaos:param name="hysteresis" value="0.5"/>
                <calaos:param name="min_on_time" value="900"/>
                <calaos:param name="min_off_time" value="900"/>
                <calaos:param name="min_range" value="2"/>
                <calaos:param name="sensor_timeout" value="1800"/>
                <calaos:param name="humidity_max" value="65"/>
            </calaos:device>
        </calaos:room>
)";

const char *kFormat2 = " format=\"2\"";

const char *kCascadeIo = R"(
<calaos:room name="A" type="salon" hits="1">
  <calaos:input id="ext" type="WITemp" name="ext" host="h" port="502" var="0"/>
  <calaos:device id="dev" type="climate" impl="thermostat" name="dev">
    <calaos:internal id="dev_target" type="InternalInt" name="target"/>
    <calaos:input id="dev_in" type="WITemp" name="in" host="h" port="502" var="1"/>
    <calaos:output id="dev_out" type="WODigital" name="out" host="h" port="502" var="2"/>
    <calaos:link name="current_temp" io="ext"/>
    <calaos:link name="output_heat" io="dev_out"/>
  </calaos:device>
</calaos:room>)";

const char *kCascadeRules = R"(
<calaos:rule name="on_target" type="t">
  <calaos:condition><calaos:input id="dev_target" oper="==" val="20"/></calaos:condition>
</calaos:rule>
<calaos:rule name="on_in" type="t">
  <calaos:condition><calaos:input id="dev_in" oper="&gt;" val="0"/></calaos:condition>
</calaos:rule>
<calaos:rule name="drives_out" type="t">
  <calaos:condition><calaos:input id="ext" oper="&gt;" val="1"/></calaos:condition>
  <calaos:action><calaos:output id="dev_out" val="true"/></calaos:action>
</calaos:rule>
<calaos:rule name="on_device" type="t">
  <calaos:condition><calaos:input id="dev" oper="==" val="off"/></calaos:condition>
</calaos:rule>
<calaos:rule name="drives_device" type="t">
  <calaos:condition><calaos:input id="ext" oper="&gt;" val="2"/></calaos:condition>
  <calaos:action><calaos:output id="dev" val="heat"/></calaos:action>
</calaos:rule>
<calaos:rule name="external_only" type="t">
  <calaos:condition><calaos:input id="ext" oper="&gt;" val="3"/></calaos:condition>
</calaos:rule>)";

// Every IO the rules point at, dereferenced: under ASan a freed one aborts here.
QSet<IOBase *> ruleIos()
{
    QSet<IOBase *> ios;
    for (int i = 0;i < ListeRule::Instance().size();i++)
    {
        Rule *r = ListeRule::Instance().get_rule(i);
        for (int c = 0;c < r->get_size_conds();c++)
        {
            Condition *cd = r->get_condition(c);
            for (int k = 0;k < cd->get_size();k++)
                ios << cd->get_input(k);
            for (int k = 0;k < cd->getScriptInputSize();k++)
                ios << cd->getScriptInput(k);
            if (cd->getType() == COND_OUTPUT && cd->getOutput())
                ios << cd->getOutput();
        }
        for (int a = 0;a < r->get_size_actions();a++)
        {
            Action *ac = r->get_action(a);
            for (int k = 0;k < ac->get_size();k++)
                ios << ac->get_output(k);
        }
    }
    for (IOBase *io: ios)
        (void)io->get_param("id");
    return ios;
}

}

class tst_Devices: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;
    QStringList m_dialogs;
    QStringList m_expectedDialogs;
    QTimer m_dialogKiller;

    QString work(const QString &file) const { return m_workDir.filePath(file); }

    bool loadIOs(const QString &path)
    {
        QString p = path;
        return ProjectManager::loadIOsFromFile(p);
    }
    bool loadRules(const QString &path)
    {
        QString p = path;
        return ProjectManager::loadRulesFromFile(p);
    }
    QByteArray saveIOs(const QString &file)
    {
        QString p = work(file);
        if (!ProjectManager::saveIOsToFile(p))
            return QByteArray();
        return readAll(p);
    }
    bool load(const QByteArray &io, const QByteArray &rules = QByteArray())
    {
        resetModel();
        writeAll(work("src_io.xml"), io);
        if (!loadIOs(work("src_io.xml")))
            return false;
        if (rules.isEmpty())
            return true;
        writeAll(work("src_rules.xml"), rules);
        return loadRules(work("src_rules.xml"));
    }
    QString ioThroughModel(const QByteArray &xml)
    {
        load(xml);
        return QString::fromUtf8(saveIOs("dst.xml"));
    }

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void specExampleRoundTrip();
    void specExampleModel();
    void rulesResolveDevicesAndOwnedIos();
    void unknownImplAndAttributesAreKept();
    void unknownDeviceChildrenAreKept();
    void formatAttributeFollowsDevices();
    void formatSupport_data();
    void formatSupport();
    void newerFormatIsReadOnly();
    void deletingDeviceDeletesOwnedIosAndTheirRules_data();
    void deletingDeviceDeletesOwnedIosAndTheirRules();
    void deletingDeviceLeavesNoRuleOnItsIos();
    void deletingOwnedIoKeepsDeviceAndLink();
    void renamingDeviceKeepsItsIos();
    void ownedIoMovedToAnotherRoomStaysInItsDevice();
    void ownerWithoutDeviceIsWrittenInItsRoom();
    void duplicateDeviceIdWritesIosOnce();
    void adoptRefusesDevicesAndNull();
    void createDeviceGeneratesUniqueIds();
};

void tst_Devices::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());

    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_devices");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());

    // The reader opens a modal box on a duplicate id: a headless run would hang.
    connect(&m_dialogKiller, &QTimer::timeout, this, [this]()
    {
        if (QWidget *w = QApplication::activeModalWidget())
        {
            if (QMessageBox *mb = qobject_cast<QMessageBox *>(w))
                m_dialogs << mb->text();
            w->close();
        }
    });
    m_dialogKiller.start(20);
}

void tst_Devices::init()
{
    resetModel();
    m_dialogs.clear();
    m_expectedDialogs.clear();
}

void tst_Devices::cleanup()
{
    QCOMPARE(m_dialogs, m_expectedDialogs);
}

void tst_Devices::specExampleRoundTrip()
{
    QString expected = ioDoc(kSpecWritten, kFormat2);
    QCOMPARE(ioThroughModel(ioSource(kSpecExample, kFormat2)), expected);
    QCOMPARE(ioThroughModel(expected.toUtf8()), expected);
}

void tst_Devices::specExampleModel()
{
    QVERIFY(load(ioSource(kSpecExample, kFormat2)));
    ListeRoom &lr = ListeRoom::Instance();
    QCOMPARE(lr.getLoadedFormat(), QString("2"));
    QVERIFY(!lr.isReadOnly());

    Room *room = lr.get_room(0);
    std::vector<Device *> devs = room->get_devices();
    QCOMPARE(int(devs.size()), 2);

    Device *ac = lr.get_device("salon_ac");
    QCOMPARE(ac, devs[0]);
    QCOMPARE(u(ac->family()), QString("climate"));
    QCOMPARE(u(ac->impl()), QString("proxy"));
    QCOMPARE(u(ac->get_param("name")), QString("Clim salon"));
    QCOMPARE(u(ac->get_gui_type()), QString("climate"));
    QVERIFY(ac->is_input() && ac->is_output());
    QCOMPARE(ids(ac->ownedIos()), QStringList({ "salon_ac_mode", "salon_ac_mode_cmd", "salon_ac_current",
                                                "salon_ac_target", "salon_ac_action", "salon_ac_remotetemp" }));
    for (IOBase *io: ac->ownedIos())
        QCOMPARE(u(io->getOwnerDevice()), QString("salon_ac"));

    QCOMPARE(int(ac->links().size()), 6);
    QCOMPARE(u(ac->links()[0].name), QString("mode"));
    QCOMPARE(u(ac->links()[0].io), QString("salon_ac_mode"));
    QCOMPARE(u(ac->links()[0].commandIo), QString("salon_ac_mode_cmd"));
    QCOMPARE(u(ac->links()[5].io), QString("salon_temp"));
    QCOMPARE(u(ac->links()[5].commandIo), QString());
    QCOMPARE(int(ac->deviceParams().size()), 1);
    QCOMPARE(u(ac->deviceParams()[0].first), QString("remote_temp_keepalive"));
    QCOMPARE(u(ac->deviceParams()[0].second), QString("120"));

    Device *floor = lr.get_device("salon_floor");
    QCOMPARE(floor, devs[1]);
    QCOMPARE(u(floor->impl()), QString("thermostat"));
    QCOMPARE(int(floor->ownedIos().size()), 6);
    QStringList params;
    for (const auto &p: floor->deviceParams())
        params << u(p.first);
    QCOMPARE(params, QStringList({ "emitter", "hysteresis", "min_on_time", "min_off_time",
                                   "min_range", "sensor_timeout", "humidity_max" }));

    // The external sensor stays a free IO, linked by both devices.
    QCOMPARE(u(lr.get_input("salon_temp")->getOwnerDevice()), QString());
    QVERIFY(lr.get_device("salon_temp") == nullptr);
    QVERIFY(lr.get_device("") == nullptr);
}

// Owned IOs are ordinary IOs and the device is an IO: rules.xml resolves both,
// none of them becomes a "missing IO" placeholder.
void tst_Devices::rulesResolveDevicesAndOwnedIos()
{
    QVERIFY(load(ioSource(kCascadeIo, kFormat2), rulesSource(kCascadeRules)));
    QVERIFY(ProjectManager::missingIOReport().isEmpty());
    QCOMPARE(ListeRule::Instance().size(), 6);
}

// The installer does not know impls: an unknown one, like any attribute it
// does not use, is written back untouched.
void tst_Devices::unknownImplAndAttributesAreKept()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:device id="d" type="cover" impl="future_impl" name="d" extra="1">
    <calaos:link name="x" io="nowhere"/>
  </calaos:device>
</calaos:room>)", kFormat2);

    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:device extra="1" id="d" impl="future_impl" name="d" type="cover">
                <calaos:link name="x" io="nowhere"/>
            </calaos:device>
        </calaos:room>
)", kFormat2));
}

// A nested device is not built (devices do not nest) and an unknown child
// element is not dropped: both come back verbatim, after the known content.
void tst_Devices::unknownDeviceChildrenAreKept()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:device id="d" type="climate" impl="proxy" name="d">
    <calaos:schedule a="1"><calaos:slot/></calaos:schedule>
    <calaos:device id="inner" type="climate" impl="proxy"><calaos:input id="inner_in" type="WITemp"/></calaos:device>
    <calaos:input id="d_in" type="WITemp" name="in" host="h" port="502" var="0"/>
    <calaos:link name="current_temp" io="d_in"><calaos:junk/></calaos:link>
    <calaos:param name="p" value="v"><calaos:junk/></calaos:param>
  </calaos:device>
  <calaos:input id="after" type="WITemp" name="after" host="h" port="502" var="0"/>
</calaos:room>)", kFormat2);

    QString expected = ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input host="h" id="after" name="after" port="502" type="WITemp" var="0"/>
            <calaos:device id="d" impl="proxy" name="d" type="climate">
                <calaos:input host="h" id="d_in" name="in" port="502" type="WITemp" var="0"/>
                <calaos:link name="current_temp" io="d_in"/>
                <calaos:param name="p" value="v"/>
                <calaos:schedule a="1">
                    <calaos:slot/>
                </calaos:schedule>
                <calaos:device id="inner" type="climate" impl="proxy">
                    <calaos:input id="inner_in" type="WITemp"/>
                </calaos:device>
            </calaos:device>
        </calaos:room>
)", kFormat2);
    QCOMPARE(ioThroughModel(src), expected);
    QVERIFY(ListeRoom::Instance().get_device("inner") == nullptr);
    QVERIFY(ListeRoom::Instance().get_input("inner_in") == nullptr);
    QCOMPARE(ioThroughModel(expected.toUtf8()), expected);
}

// Same rule as the server: format="2" exactly when there is a device to lose.
// A file without devices is written as before, format or not in the source.
void tst_Devices::formatAttributeFollowsDevices()
{
    const char *plain = R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="i" type="WITemp" name="i" host="h" port="502" var="0"/>
</calaos:room>)";
    QString plainDoc = ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input host="h" id="i" name="i" port="502" type="WITemp" var="0"/>
        </calaos:room>
)");

    QCOMPARE(ioThroughModel(ioSource(plain)), plainDoc);
    QCOMPARE(ioThroughModel(ioSource(plain, kFormat2)), plainDoc);
    QVERIFY(!ListeRoom::Instance().isReadOnly());

    // A format 1 file that gains a device is upgraded on save.
    load(ioSource(plain));
    Params p;
    p.Add("id", "dev");
    p.Add("type", "climate");
    p.Add("impl", "proxy");
    ListeRoom::Instance().createDevice(p, ListeRoom::Instance().get_room(0));
    QVERIFY(QString::fromUtf8(saveIOs("dst.xml")).contains("<calaos:home format=\"2\">"));
}

void tst_Devices::formatSupport_data()
{
    QTest::addColumn<QString>("format");
    QTest::addColumn<bool>("supported");

    QTest::newRow("absent") << QString() << true;
    QTest::newRow("1") << QString("1") << true;
    QTest::newRow("2") << QString("2") << true;
    QTest::newRow("3") << QString("3") << false;
    QTest::newRow("42") << QString("42") << false;
    // Not understood is not known to be safe to rewrite.
    QTest::newRow("0") << QString("0") << false;
    QTest::newRow("-1") << QString("-1") << false;
    QTest::newRow("garbage") << QString("2b") << false;
}

void tst_Devices::formatSupport()
{
    QFETCH(QString, format);
    QFETCH(bool, supported);

    QCOMPARE(ListeRoom::isFormatSupported(format), supported);

    QString attrs = format.isNull() ? QString() : QString(" format=\"%1\"").arg(format);
    QVERIFY(load(ioSource(R"(<calaos:room name="R" type="salon" hits="1"/>)", attrs)));
    QCOMPARE(ListeRoom::Instance().getLoadedFormat(), format.isNull() ? QString("") : format);
    QCOMPARE(ListeRoom::Instance().isReadOnly(), !supported);
}

// Saving rebuilds the files from the model: for a format it does not know,
// the installer would drop whatever it cannot represent. It refuses instead,
// and leaves the files on disk as they were.
void tst_Devices::newerFormatIsReadOnly()
{
    QVERIFY(load(ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:device id="d" type="climate" impl="proxy" name="d"/>
</calaos:room>)", " format=\"3\"")));
    QVERIFY(ListeRoom::Instance().isReadOnly());
    QVERIFY(ListeRoom::Instance().get_device("d"));
    QString reason = ProjectManager::readOnlyReason();
    QVERIFY2(reason.contains("format \"3\"") && reason.contains("up to format 2"), qPrintable(reason));

    QString io = work("ro_io.xml");
    QString rules = work("ro_rules.xml");
    writeAll(io, "original io");
    writeAll(rules, "original rules");
    QVERIFY(!ProjectManager::saveIOsToFile(io));
    QVERIFY(!ProjectManager::saveRulesToFile(rules));
    QCOMPARE(readAll(io), QByteArray("original io"));
    QCOMPARE(readAll(rules), QByteArray("original rules"));

    // A new project is writable again.
    resetModel();
    QVERIFY(!ListeRoom::Instance().isReadOnly());
    QVERIFY(ProjectManager::readOnlyReason().isEmpty());
    QVERIFY(ProjectManager::saveIOsToFile(io));

    // So is the next file loaded, even without going through clear().
    QVERIFY(load(ioSource(R"(<calaos:room name="R" type="salon" hits="1"/>)", " format=\"3\"")));
    writeAll(work("plain.xml"), ioSource(R"(<calaos:room name="R" type="salon" hits="1"/>)"));
    QVERIFY(loadIOs(work("plain.xml")));
    QVERIFY(!ListeRoom::Instance().isReadOnly());
}

void tst_Devices::deletingDeviceDeletesOwnedIosAndTheirRules_data()
{
    QTest::addColumn<bool>("asInput");
    QTest::newRow("deleteIOInput") << true;
    QTest::newRow("deleteIOOutput") << false;
}

void tst_Devices::deletingDeviceDeletesOwnedIosAndTheirRules()
{
    QFETCH(bool, asInput);

    QVERIFY(load(ioSource(kCascadeIo, kFormat2), rulesSource(kCascadeRules)));
    ListeRoom &lr = ListeRoom::Instance();
    Device *dev = lr.get_device("dev");
    QVERIFY(dev);

    QVERIFY(asInput ? lr.deleteIOInput(dev) : lr.deleteIOOutput(dev));

    QVERIFY(lr.get_device("dev") == nullptr);
    QVERIFY(!lr.hasDevices());
    for (const char *id: { "dev", "dev_target", "dev_in", "dev_out" })
    {
        QVERIFY2(!lr.get_input(id), id);
        QVERIFY2(!lr.get_output(id), id);
    }
    QVERIFY(lr.get_input("ext"));
    QCOMPARE(lr.get_room(0)->get_size_in(), 1);
    QCOMPARE(lr.get_room(0)->get_size_out(), 0);
    QCOMPARE(ruleNames(), QStringList({ "external_only" }));

    // No device left: back to a format 1 file.
    QCOMPARE(QString::fromUtf8(saveIOs("dst.xml")), ioDoc(
R"(        <calaos:room name="A" type="salon" hits="1">
            <calaos:input host="h" id="ext" name="ext" port="502" type="WITemp" var="0"/>
        </calaos:room>
)"));
}

// Script inputs and output-state conditions hold the IO too: once the device
// and its IOs are gone, no rule may still point at any of them.
void tst_Devices::deletingDeviceLeavesNoRuleOnItsIos()
{
    QVERIFY(load(ioSource(kCascadeIo, kFormat2), rulesSource(R"(
<calaos:rule name="script_on_owned" type="t">
  <calaos:condition type="script"><calaos:input id="ext"/><calaos:input id="dev_in"/>
    <calaos:script type="lua"><![CDATA[return true]]></calaos:script></calaos:condition>
  <calaos:action><calaos:output id="dev_target" val="1"/></calaos:action>
</calaos:rule>
<calaos:rule name="state_of_owned" type="t">
  <calaos:condition type="output"><calaos:output id="dev_out" oper="==" val="true"/></calaos:condition>
</calaos:rule>
<calaos:rule name="state_of_device" type="t">
  <calaos:condition type="output"><calaos:output id="dev" oper="==" val="off"/></calaos:condition>
</calaos:rule>
<calaos:rule name="external_only" type="t">
  <calaos:condition><calaos:input id="ext" oper="&gt;" val="3"/></calaos:condition>
</calaos:rule>)")));
    QVERIFY(ProjectManager::missingIOReport().isEmpty());

    ListeRoom &lr = ListeRoom::Instance();
    Device *dev = lr.get_device("dev");
    QSet<IOBase *> gone = { dev };
    for (IOBase *io: dev->ownedIos())
        gone << io;
    QCOMPARE(gone.size(), 4);
    QVERIFY(ruleIos().intersects(gone));

    QVERIFY(lr.deleteIOOutput(dev));

    QVERIFY(!ruleIos().intersects(gone));
    QVERIFY(!ruleNames().contains("state_of_owned"));
    QVERIFY(!ruleNames().contains("state_of_device"));
    QVERIFY(ruleNames().contains("external_only"));

    QString rules = work("rules_after.xml");
    QVERIFY(ProjectManager::saveRulesToFile(rules));
    QVERIFY(!QString::fromUtf8(readAll(rules)).contains("dev"));
}

// A link is only an id: deleting its target leaves it unresolved, as the
// server would load it, rather than silently editing the device.
void tst_Devices::deletingOwnedIoKeepsDeviceAndLink()
{
    QVERIFY(load(ioSource(kCascadeIo, kFormat2), rulesSource(kCascadeRules)));
    ListeRoom &lr = ListeRoom::Instance();

    QVERIFY(lr.deleteIOOutput(lr.get_output("dev_out")));

    Device *dev = lr.get_device("dev");
    QVERIFY(dev);
    QCOMPARE(ids(dev->ownedIos()), QStringList({ "dev_target", "dev_in" }));
    QCOMPARE(int(dev->links().size()), 2);
    QCOMPARE(u(dev->links()[1].io), QString("dev_out"));
    QVERIFY(!ruleNames().contains("drives_out"));
    QVERIFY(ruleNames().contains("on_device"));
}

void tst_Devices::renamingDeviceKeepsItsIos()
{
    QVERIFY(load(ioSource(kCascadeIo, kFormat2)));
    ListeRoom &lr = ListeRoom::Instance();
    Device *dev = lr.get_device("dev");

    dev->set_param("name", "renamed");
    QCOMPARE(int(dev->ownedIos().size()), 3);

    dev->set_param("id", "dev2");
    QVERIFY(lr.get_device("dev") == nullptr);
    QCOMPARE(lr.get_device("dev2"), dev);
    QCOMPARE(ids(dev->ownedIos()), QStringList({ "dev_target", "dev_in", "dev_out" }));
    QCOMPARE(u(lr.get_input("dev_in")->getOwnerDevice()), QString("dev2"));

    QString written = QString::fromUtf8(saveIOs("dst.xml"));
    QVERIFY(written.contains(R"(<calaos:device id="dev2" impl="thermostat" name="renamed" type="climate">
                <calaos:internal id="dev_target")"));
}

// Ownership is the tag, not the room: the IO is still written inside its
// device, so a reload brings it back next to it.
void tst_Devices::ownedIoMovedToAnotherRoomStaysInItsDevice()
{
    QVERIFY(load(ioSource(QString(kCascadeIo) + R"(<calaos:room name="B" type="salon" hits="1"/>)", kFormat2)));
    ListeRoom &lr = ListeRoom::Instance();
    Room *a = lr.get_room(0);
    Room *b = lr.get_room(1);

    IOBase *out = lr.get_output("dev_out");
    a->RemoveOutput(out);
    b->AddOutput(out);

    QCOMPARE(ids(lr.get_device("dev")->ownedIos()), QStringList({ "dev_target", "dev_in", "dev_out" }));
    QString written = QString::fromUtf8(saveIOs("dst.xml"));
    QCOMPARE(written.count("id=\"dev_out\""), 1);
    QVERIFY(written.contains(R"(<calaos:room name="B" type="salon" hits="1"/>)"));
}

// The server writes an IO whose device is gone at room level: it must not
// vanish from the file.
void tst_Devices::ownerWithoutDeviceIsWrittenInItsRoom()
{
    QVERIFY(load(ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="i" type="WITemp" name="i" host="h" port="502" var="0"/>
</calaos:room>)")));
    ListeRoom::Instance().get_input("i")->setOwnerDevice("ghost");

    QCOMPARE(QString::fromUtf8(saveIOs("dst.xml")), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input host="h" id="i" name="i" port="502" type="WITemp" var="0"/>
        </calaos:room>
)"));
}

void tst_Devices::duplicateDeviceIdWritesIosOnce()
{
    m_expectedDialogs = { "Device with id \"d\" already exists!" };
    QString written = ioThroughModel(ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:device id="d" type="climate" impl="proxy" name="first">
    <calaos:input id="a" type="WITemp" name="a" host="h" port="502" var="0"/>
  </calaos:device>
  <calaos:device id="d" type="climate" impl="proxy" name="second">
    <calaos:input id="b" type="WITemp" name="b" host="h" port="502" var="0"/>
  </calaos:device>
</calaos:room>)", kFormat2));

    QCOMPARE(written, ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:device id="d" impl="proxy" name="first" type="climate">
                <calaos:input host="h" id="a" name="a" port="502" type="WITemp" var="0"/>
                <calaos:input host="h" id="b" name="b" port="502" type="WITemp" var="0"/>
            </calaos:device>
            <calaos:device id="d" impl="proxy" name="second" type="climate"/>
        </calaos:room>
)", kFormat2));
}

void tst_Devices::adoptRefusesDevicesAndNull()
{
    QVERIFY(load(ioSource(kCascadeIo, kFormat2)));
    ListeRoom &lr = ListeRoom::Instance();
    Room *room = lr.get_room(0);

    Params p;
    p.Add("id", "other");
    Device *other = lr.createDevice(p, room);
    Device *dev = lr.get_device("dev");

    QVERIFY(!dev->adoptIO(nullptr));
    QVERIFY(!dev->adoptIO(other));
    QVERIFY(!dev->adoptIO(dev));
    QCOMPARE(u(other->getOwnerDevice()), QString());

    QVERIFY(other->adoptIO(lr.get_input("ext")));
    QCOMPARE(ids(other->ownedIos()), QStringList({ "ext" }));
    QVERIFY(other->adoptIO(lr.get_input("ext")));
    QCOMPARE(ids(other->ownedIos()), QStringList({ "ext" }));
}

void tst_Devices::createDeviceGeneratesUniqueIds()
{
    QVERIFY(load(ioSource(R"(<calaos:room name="R" type="salon" hits="1"/>)")));
    ListeRoom &lr = ListeRoom::Instance();
    Room *room = lr.get_room(0);

    Device *d0 = lr.createDevice(Params(), room);
    Device *d1 = lr.createDevice(Params(), room);
    QCOMPARE(u(d0->get_param("id")), QString("device_0"));
    QCOMPARE(u(d1->get_param("id")), QString("device_1"));

    std::vector<Device *> devs = room->get_devices();
    QCOMPARE(int(devs.size()), 2);
    QCOMPARE(devs[0], d0);
    QCOMPARE(devs[1], d1);
    QVERIFY(lr.hasDevices());
}

QTEST_MAIN(tst_Devices)
#include "tst_devices.moc"
