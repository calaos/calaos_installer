#include <QtTest>
#include <QApplication>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTimer>
#include <QTreeWidget>
#include <set>

#include "fixture.h"
#include "HomeTree.h"
#include "IODoc.h"

using namespace Calaos;
using namespace Fixture;

namespace
{

struct Variant
{
    QString name;
    QString tpl;
    QVariantMap values;
};

//What a user fills in: the defaults, a name, a value in every required text,
//and for each io field the first IO that fits and that no other field took.
QVariantMap filled(const QJsonObject &tpl)
{
    QVariantMap v = DeviceTemplates::defaults(tpl, roomNamed("Salon"));
    QStringList taken;
    for (const QJsonValue &e: tpl["fields"].toArray())
    {
        const QJsonObject f = e.toObject();
        const QString name = f["name"].toString();
        const QString type = f["type"].toString();
        if (type == "string" && f["required"].toBool() && v.value(name).toString().isEmpty())
            v[name] = name == "name" ? QString("Chauffage salon") : QString("tpl-%1").arg(name);
        if (type == "io")
        {
            for (IOBase *io: DeviceTemplates::candidates(f["io_filter"].toObject()))
            {
                const QString id = u(io->get_param("id"));
                if (!taken.contains(id))
                {
                    taken << id;
                    v[name] = id;
                    break;
                }
            }
        }
    }
    return v;
}

//Each enum option, each bool both ways, each optional multi, io and text empty
//and set: every `when` of the shipped templates is taken both ways (checked by
//everyWhenIsTakenBothWays).
std::vector<Variant> generatedVariants()
{
    std::vector<Variant> res;
    for (const QJsonValue &t: shippedTemplates())
    {
        const QJsonObject tpl = t.toObject();
        const QString id = tpl["id"].toString();
        const QVariantMap base = filled(tpl);
        res.push_back({ "defaults", id, base });

        for (const QJsonValue &e: tpl["fields"].toArray())
        {
            const QJsonObject f = e.toObject();
            const QString name = f["name"].toString();
            const QString type = f["type"].toString();
            auto with = [&](const QString &label, const QVariant &value)
            {
                QVariantMap v = base;
                v[name] = value;
                res.push_back({ QString("%1=%2").arg(name, label), id, v });
            };

            if (type == "enum")
                for (const QJsonValue &o: f["values"].toArray())
                    with(o.toObject()["value"].toString(), o.toObject()["value"].toString());
            else if (type == "bool")
            {
                with("true", true);
                with("false", false);
            }
            else if (type == "multi")
            {
                QStringList all;
                for (const QJsonValue &o: f["values"].toArray())
                    all << o.toObject()["value"].toString();
                with("all", all);
                with("first", QStringList { all.first() });
                if (!f["required"].toBool())
                    with("none", QStringList());
            }
            else if (!f["required"].toBool() && type == "io")
            {
                with("none", QString());
                with("temp_cave", QString("temp_cave"));
            }
            else if (!f["required"].toBool() && type == "string")
            {
                with("empty", QString());
                with("set", QString("value-%1").arg(name));
            }
        }
    }
    return res;
}

const Variant *generatedVariant(const std::vector<Variant> &all, const QString &tpl, const QString &name)
{
    for (const Variant &v: all)
        if (v.tpl == tpl && v.name == name)
            return &v;
    return nullptr;
}

QSet<QString> rolesOf(Device *dev)
{
    QSet<QString> s;
    for (const DeviceLink &l: dev->links())
        s.insert(u(l.name));
    return s;
}

QMap<QString, QString> paramsOf(Device *dev)
{
    QMap<QString, QString> m;
    for (const auto &p: dev->deviceParams())
        m[u(p.first)] = u(p.second);
    return m;
}

IOBase *ownedIo(Device *dev, const QString &suffix)
{
    for (IOBase *io: dev->ownedIos())
        if (u(io->get_param("id")).endsWith("_" + suffix))
            return io;
    return nullptr;
}

QStringList namesOf(const QJsonArray &a, const char *key)
{
    QStringList l;
    for (const QJsonValue &e: a)
        l << e.toObject()[key].toString();
    return l;
}

QJsonObject synthetic()
{
    //Every rule of the format on a template no family uses.
    return QJsonDocument::fromJson(R"({
      "id": "synthetic", "family": "fam", "impl": "imp", "name": "Synthetic", "description": "d",
      "attributes": { "name": "{name}", "extra": "{label} of {id}", "lbl": "{label}", "kept": "{nothing}" },
      "fields": [
        { "name": "name", "type": "string", "label": "Name", "required": true },
        { "name": "room", "type": "room", "label": "Room", "required": true },
        { "name": "label", "type": "string", "label": "Label", "required": false },
        { "name": "count", "type": "number", "label": "Count", "required": true, "min": 0, "max": 10, "unit": "s", "default": 2 },
        { "name": "flags", "type": "multi", "label": "Flags", "required": false,
          "values": [ { "value": "a", "label": "A" }, { "value": "b", "label": "B" }, { "value": "c", "label": "C" } ] },
        { "name": "advanced", "type": "bool", "label": "Advanced", "required": false, "default": false },
        { "name": "tuning", "type": "number", "label": "Tuning", "required": true, "min": 1, "max": 5, "default": 3 },
        { "name": "target", "type": "io", "label": "Target", "required": false, "io_filter": { "value": "string", "writable": true } },
        { "name": "kind", "type": "enum", "label": "Kind", "required": true, "default": "x",
          "values": [ { "value": "x", "label": "X", "params": { "p": "1" } }, { "value": "y", "label": "Y", "params": { "p": "2" } } ] }
      ],
      "ios": [
        { "key": "state", "element": "input", "type": "MqttInputString", "params": { "name": "{name} state", "user": "{label}", "topic_sub": "t/{id}/state" } },
        { "key": "cmd", "element": "output", "type": "MqttOutputString", "params": { "name": "{name} cmd", "topic_pub": "t/{id}/cmd" }, "when": "flags" },
        { "key": "var", "element": "internal", "type": "InternalInt", "params": { "name": "{name} var", "value": "{tuning}" }, "when": "!label" }
      ],
      "links": [
        { "role": "state", "io": { "owned": "state" }, "command_io": { "owned": "cmd" } },
        { "role": "var", "io": { "owned": "var" } },
        { "role": "target", "io": { "field": "target" } },
        { "role": "cond", "io": { "owned": "state" }, "when": "advanced" }
      ],
      "params": [
        { "name": "count", "value": "{count}" }, { "name": "flags", "value": "{flags}" },
        { "name": "advanced", "value": "{advanced}" }, { "name": "p", "value": "0" },
        { "name": "tuning", "value": "{tuning}" }, { "name": "kept", "value": "{unknown} {}" },
        { "name": "labelled", "value": "yes", "when": "label" }
      ],
      "notes": [ { "text": "Topic t/{id}/state of {name}" }, { "text": "only advanced", "when": "advanced" } ]
    })").object();
}

}

class tst_DeviceTemplates: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;
    QTimer m_dialogKiller;
    QStringList m_dialogs;

    QString work(const QString &f) const { return m_workDir.filePath(f); }

    void checkDocumentedDevice(Device *dev, const QJsonObject &tpl);
    void checkRoundTrip(Device *dev);

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void theShippedCatalogueIsRead();
    void theFrenchCatalogueIsReadWhenTheLanguageIsFrench();
    void anotherFormatIsNotRead();

    void everyVariantBuildsAValidDevice_data();
    void everyVariantBuildsAValidDevice();
    void everyWhenIsTakenBothWays();
    void theNamedVariantsHaveTheirLinksAndParams_data();
    void theNamedVariantsHaveTheirLinksAndParams();
    void theOwnedIosCarryTheFieldValues();
    void theDocumentedExampleIsWrittenAsDocumented();

    void substitutionFollowsTheRules();
    void anEnumOptionSetsItsParams();

    void aTakenIdGetsTheFirstFreeSuffix();
    void idsAssignedByTheWizardAreTakenToo();
    void aSecondDeviceFromTheSameTemplateGetsItsOwnIds();
    void theDeviceIdComesFromItsName_data();
    void theDeviceIdComesFromItsName();

    void ioFieldsListTheIosThatFit_data();
    void ioFieldsListTheIosThatFit();
    void linksTargetTheChosenIosInAnyRoom();
    void anIoTheServerDroppedIsNotOffered();

    void validation_data();
    void validation();
    void anOutputOfAnotherThermostatIsRefused();
    void aTemplateWithoutRoomOrWithAnUnknownKindIsRefused();
    void anInvalidSetCreatesNothing();

    void theDeviceIsInsertedInTheTreeAsAFreshTreeShowsIt();
};

void tst_DeviceTemplates::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_devicetemplates");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());

    //The reader opens a modal box on a duplicate id: it must never show.
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

void tst_DeviceTemplates::init()
{
    m_dialogs.clear();
    QVERIFY(loadIo(work("house.xml"), houseXml()));
}

void tst_DeviceTemplates::cleanup()
{
    QCOMPARE(m_dialogs, QStringList());
    resetModel();
}

void tst_DeviceTemplates::theShippedCatalogueIsRead()
{
    const QJsonArray all = shippedTemplates();
    QCOMPARE(namesOf(all, "id"), QStringList({ "esphome_climate", "esphome_climate_ir", "thermostat_heat",
                                               "thermostat_heat_cool", "thermostat_floor_heat_cool" }));
    for (const QJsonValue &t: all)
        QVERIFY(!t.toObject()["name"].toString().isEmpty());
}

void tst_DeviceTemplates::theFrenchCatalogueIsReadWhenTheLanguageIsFrench()
{
    //fr/ is a copy of en/ until calaos_base translates the templates: a marked
    //resource tells which file was read.
    QCOMPARE(DeviceTemplates::load("fr").first().toObject()["description"].toString(), QString("fr-marker"));
    QCOMPARE(namesOf(DeviceTemplates::load("de"), "id"), namesOf(shippedTemplates("en"), "id"));
    QVERIFY(DeviceTemplates::load("de").first().toObject()["description"].toString() != "fr-marker");
}

void tst_DeviceTemplates::anotherFormatIsNotRead()
{
    QVERIFY(DeviceTemplates::parse(R"({"format": 2, "templates": [{"id": "x"}]})").isEmpty());
    QVERIFY(DeviceTemplates::parse("not json").isEmpty());
    QCOMPARE(DeviceTemplates::parse(R"({"format": 1, "templates": [{"id": "x"}]})").size(), 1);
}

void tst_DeviceTemplates::checkDocumentedDevice(Device *dev, const QJsonObject &tpl)
{
    const QJsonObject doc = IODoc::getDeviceObject(tpl["impl"].toString());
    QVERIFY(!doc.isEmpty());

    QMap<QString, QJsonObject> roles;
    for (const QJsonValue &r: doc["link_roles"].toArray())
        roles[r.toObject()["role"].toString()] = r.toObject();
    for (const QString &role: roles.keys())
        if (roles[role]["required"].toBool())
            QVERIFY2(rolesOf(dev).contains(role), qPrintable("required role missing: " + role));

    for (const DeviceLink &l: dev->links())
    {
        QVERIFY2(roles.contains(u(l.name)), qPrintable("unknown role " + u(l.name)));
        IOBase *target = DeviceTemplates::ioById(l.io);
        QVERIFY2(target, qPrintable("unresolved link " + u(l.name)));
        if (roles[u(l.name)]["writable"].toBool() && l.commandIo.empty())
            QVERIFY2(target->is_output(), qPrintable("read-only target for " + u(l.name)));
        if (!l.commandIo.empty())
            QVERIFY(DeviceTemplates::ioById(l.commandIo));
    }

    const QStringList params = namesOf(doc["parameters"].toArray(), "name");
    for (const auto &p: dev->deviceParams())
    {
        QVERIFY2(params.contains(u(p.first)), qPrintable("undocumented param " + u(p.first)));
        QVERIFY(!p.second.empty());
    }

    const std::vector<std::string> creatable = IOFactory::Instance().creatableTypes();
    for (IOBase *io: dev->ownedIos())
    {
        std::string type = io->get_param("type");
        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        QVERIFY2(std::find(creatable.begin(), creatable.end(), type) != creatable.end(), type.c_str());
        QCOMPARE(ListeRoom::Instance().searchIO(io), ListeRoom::Instance().searchIO(dev));

        const QStringList documented = namesOf(IODoc::getIOObject(u(io->get_param("type")))["parameters"].toArray(), "name");
        for (int i = 0;i < io->get_params().size();i++)
        {
            std::string k, v;
            io->get_params().get_item(i, k, v);
            if (k == "type")
                continue;
            QVERIFY2(documented.contains(u(k)), qPrintable(u(io->get_param("type")) + " has no param " + u(k)));
            QVERIFY(!v.empty());
            QVERIFY2(!QRegularExpression("\\{[a-z_]+\\}").match(u(v)).hasMatch(), v.c_str());
        }
    }
}

void tst_DeviceTemplates::checkRoundTrip(Device *dev)
{
    const QString id = u(dev->get_param("id"));
    const QByteArray first = saveIo(work("first.xml"));
    const QStringList written = deviceElement(first, id);
    QVERIFY(!written.isEmpty());

    //For a load by the real server (see the INST-4 fiche): one io.xml per row.
    const QString dump = qEnvironmentVariable("CALAOS_DEVICE_XML_DIR");
    if (!dump.isEmpty() && QTest::currentDataTag())
    {
        const QString tag = QString(QTest::currentDataTag()).replace(QRegularExpression("[^A-Za-z0-9_=-]"), "_");
        QVERIFY(writeFile(QDir(dump).filePath(tag + ".xml"), first));
    }

    QVERIFY(loadIo(work("reload.xml"), first));
    Device *again = ListeRoom::Instance().get_device(id.toStdString());
    QVERIFY(again);
    QCOMPARE(int(again->ownedIos().size()), written.filter(QRegularExpression("^  (input|output|internal) ")).size());
    QCOMPARE(saveIo(work("second.xml")), first);
}

void tst_DeviceTemplates::everyVariantBuildsAValidDevice_data()
{
    //The rows pick IOs of the house: init() has not loaded it yet.
    QVERIFY(loadIo(work("data.xml"), houseXml()));
    QTest::addColumn<QString>("tpl");
    QTest::addColumn<QVariantMap>("values");
    for (const Variant &v: generatedVariants())
        QTest::newRow(qPrintable(v.tpl + "/" + v.name)) << v.tpl << v.values;
}

void tst_DeviceTemplates::everyVariantBuildsAValidDevice()
{
    QFETCH(QString, tpl);
    QFETCH(QVariantMap, values);
    const QJsonObject t = shippedTemplate(tpl);
    values["room"] = QVariant::fromValue(roomNamed("Salon"));

    const QList<DeviceTemplates::Problem> problems = DeviceTemplates::validate(t, values);
    QVERIFY2(problems.isEmpty(), qPrintable(problems.isEmpty() ? QString() : problems.first().message));

    QStringList notes;
    Device *dev = DeviceTemplates::create(t, values, &notes);
    QVERIFY(dev);
    QCOMPARE(u(dev->family()), t["family"].toString());
    QCOMPARE(u(dev->impl()), t["impl"].toString());
    QCOMPARE(u(dev->get_param("name")), values["name"].toString());
    QCOMPARE(DeviceTemplates::roomOf(t, values), roomNamed("Salon"));
    QVERIFY(roomNamed("Salon")->get_devices().back() == dev);

    //Each entry is there exactly when its `when` holds.
    int ios = 0, notesExpected = 0;
    for (const QJsonValue &e: t["ios"].toArray())
        ios += DeviceTemplates::holds(e.toObject(), values);
    for (const QJsonValue &e: t["notes"].toArray())
        notesExpected += DeviceTemplates::holds(e.toObject(), values);
    QCOMPARE(int(dev->ownedIos().size()), ios);
    QCOMPARE(notes.size(), notesExpected);

    checkDocumentedDevice(dev, t);
    if (QTest::currentTestFailed())
        return;
    checkRoundTrip(dev);
}

void tst_DeviceTemplates::everyWhenIsTakenBothWays()
{
    const std::vector<Variant> all = generatedVariants();
    for (const QJsonValue &t: shippedTemplates())
    {
        const QJsonObject tpl = t.toObject();
        for (const char *section: { "ios", "links", "params", "notes" })
        {
            for (const QJsonValue &e: tpl[section].toArray())
            {
                if (!e.toObject().contains("when"))
                    continue;
                bool yes = false, no = false;
                for (const Variant &v: all)
                {
                    if (v.tpl != tpl["id"].toString())
                        continue;
                    (DeviceTemplates::holds(e.toObject(), v.values) ? yes : no) = true;
                }
                QVERIFY2(yes && no, qPrintable(tpl["id"].toString() + " " + e.toObject()["when"].toString()));
            }
        }
    }
}

void tst_DeviceTemplates::theNamedVariantsHaveTheirLinksAndParams_data()
{
    QTest::addColumn<QString>("tpl");
    QTest::addColumn<QVariantMap>("extra");
    QTest::addColumn<QStringList>("roles");
    QTest::addColumn<QVariantMap>("params");

    const QStringList allModes = { "off", "heat", "cool", "auto", "dry", "fan_only" };
    const QStringList heat = { "current_temp", "target_temp", "action", "output_heat" };
    const QStringList range = { "current_temp", "target_temp", "target_temp_low", "target_temp_high", "action",
                                "output_heat", "output_cool" };
    auto thermostat = [](const QString &emitter, const QString &hyst, const QString &on, const QString &off)
    {
        return QVariantMap { { "emitter", emitter }, { "hysteresis", hyst }, { "min_on_time", on },
                             { "min_off_time", off }, { "sensor_timeout", "9000" }, { "put_back_delay", "1" },
                             { "put_back_delay_max", "60" } };
    };
    auto plus = [](QVariantMap m, const QVariantMap &extra)
    {
        for (auto it = extra.constBegin();it != extra.constEnd();++it)
            m[it.key()] = it.value();
        return m;
    };

    QTest::newRow("esphome_full") << "esphome_climate"
        << QVariantMap { { "mqtt_user", "calaos" }, { "mqtt_password", "s3cret" }, { "modes", allModes },
                         { "two_point", true }, { "with_action", true },
                         { "fan_modes", QStringList { "high", "auto", "low" } },
                         { "swing_modes", QStringList { "vertical", "off" } }, { "remote_temp_source", "temp_cave" } }
        << QStringList { "mode", "current_temp", "target_temp_low", "target_temp_high", "action", "fan_mode",
                         "swing_mode", "remote_temp", "remote_temp_source" }
        << QVariantMap { { "modes", allModes.join(',') }, { "auto_payload", "heat_cool" },
                         { "fan_modes", "auto,low,high" }, { "swing_modes", "off,vertical" },
                         { "remote_temp_keepalive", "120" }, { "sensor_timeout", "9000" } };
    QTest::newRow("esphome_minimal") << "esphome_climate"
        << QVariantMap { { "modes", QStringList { "cool", "off", "heat" } }, { "two_point", false },
                         { "with_action", false }, { "remote_temp_source", "" } }
        << QStringList { "mode", "current_temp", "target_temp" }
        << QVariantMap { { "modes", "off,heat,cool" }, { "auto_payload", "heat_cool" } };
    QTest::newRow("esphome_ir_full") << "esphome_climate_ir"
        << QVariantMap { { "modes", allModes }, { "with_current_temp", true }, { "temp_step", 1.0 },
                         { "fan_modes", QStringList { "auto", "high" } }, { "swing_modes", QStringList { "off", "both" } } }
        << QStringList { "mode", "current_temp", "target_temp", "fan_mode", "swing_mode" }
        << QVariantMap { { "modes", allModes.join(',') }, { "auto_payload", "heat_cool" },
                         { "fan_modes", "auto,high" }, { "swing_modes", "off,both" } };
    QTest::newRow("esphome_ir_minimal") << "esphome_climate_ir"
        << QVariantMap { { "modes", QStringList { "off", "cool" } }, { "with_current_temp", false } }
        << QStringList { "mode", "target_temp" }
        << QVariantMap { { "modes", "off,cool" }, { "auto_payload", "heat_cool" } };
    QTest::newRow("heat_electric") << "thermostat_heat" << QVariantMap { { "emitter", "electric" } } << heat
                                   << thermostat("electric", "0.3", "120", "120");
    QTest::newRow("heat_water_radiator") << "thermostat_heat" << QVariantMap { { "emitter", "water_radiator" } } << heat
                                         << thermostat("water_radiator", "0.5", "300", "300");
    QTest::newRow("heat_floor") << "thermostat_heat" << QVariantMap { { "emitter", "floor" } } << heat
                                << thermostat("floor", "0.5", "900", "900");
    QTest::newRow("heat_cool_heat_pump") << "thermostat_heat_cool" << QVariantMap { { "emitter", "heat_pump" } } << range
                                         << plus(thermostat("heat_pump", "0.5", "600", "300"), { { "min_range", "2" } });
    QTest::newRow("heat_cool_fan_coil") << "thermostat_heat_cool" << QVariantMap { { "emitter", "fan_coil" } } << range
                                        << plus(thermostat("fan_coil", "0.5", "120", "120"), { { "min_range", "1" } });
    QTest::newRow("floor_heat_cool") << "thermostat_floor_heat_cool" << QVariantMap { { "humidity_max", 55.0 } }
                                     << QStringList(range) + QStringList { "humidity" }
                                     << plus(thermostat("floor", "0.5", "900", "900"),
                                             { { "min_range", "2" }, { "humidity_max", "55" } });
}

void tst_DeviceTemplates::theNamedVariantsHaveTheirLinksAndParams()
{
    QFETCH(QString, tpl);
    QFETCH(QVariantMap, extra);
    QFETCH(QStringList, roles);
    QFETCH(QVariantMap, params);

    const QJsonObject t = shippedTemplate(tpl);
    QVariantMap values = filled(t);
    for (auto it = extra.constBegin();it != extra.constEnd();++it)
        values[it.key()] = it.value();

    Device *dev = DeviceTemplates::create(t, values);
    QVERIFY(dev);
    QCOMPARE(rolesOf(dev), QSet<QString>(roles.begin(), roles.end()));

    QMap<QString, QString> expected;
    for (auto it = params.constBegin();it != params.constEnd();++it)
        expected[it.key()] = it.value().toString();
    QCOMPARE(paramsOf(dev), expected);
}

void tst_DeviceTemplates::theOwnedIosCarryTheFieldValues()
{
    const QJsonObject t = shippedTemplate("esphome_climate");
    QVariantMap v = filled(t);
    v["name"] = "Clim salon";
    v["topic_prefix"] = "salon-clim";
    v["object_id"] = "clim_salon";
    v["mqtt_port"] = 1891.0;
    v["temp_step"] = 0.5;
    v["two_point"] = true;

    Device *dev = DeviceTemplates::create(t, v);
    QVERIFY(dev);
    QCOMPARE(u(dev->get_param("id")), QString("clim_salon"));

    IOBase *low = ownedIo(dev, "target_temp_low");
    QVERIFY(low);
    QCOMPARE(u(low->get_param("id")), QString("clim_salon_target_temp_low"));
    QCOMPARE(u(low->get_param("topic_pub")), QString("salon-clim/climate/clim_salon/target_temperature_low/command"));
    QCOMPARE(u(low->get_param("name")), QString("Clim salon low setpoint"));
    QCOMPARE(u(low->get_param("port")), QString("1891"));
    QCOMPARE(u(low->get_param("step")), QString("0.5"));
    QCOMPARE(u(low->get_param("value_min")), QString("16"));
    QCOMPARE(u(low->get_param("connected_status_topic")), QString("salon-clim/status"));
    //A user and a password left empty are not written.
    QVERIFY(!low->get_params().Exists("user"));
    QVERIFY(!low->get_params().Exists("password"));
    QVERIFY(!ownedIo(dev, "target_temp"));

    QVERIFY(dynamic_cast<Device *>(dev));
    for (IOBase *io: dev->ownedIos())
        QCOMPARE(io->getOwnerDevice(), dev);
}

void tst_DeviceTemplates::theDocumentedExampleIsWrittenAsDocumented()
{
    //docs/17_device_templates.md of calaos_base, with the id this installer gives it.
    const QString documented = R"(
<calaos:device id="chauffage_salon" type="climate" impl="thermostat" name="Chauffage salon">
  <calaos:internal id="chauffage_salon_target_temp" type="InternalInt" name="Chauffage salon setpoint"
                   save="true" step="0.5" unit="°C" value="20" value_max="30" value_min="7" visible="false"/>
  <calaos:internal id="chauffage_salon_action" type="InternalString" name="Chauffage salon action" visible="false"/>
  <calaos:link name="current_temp" io="temp_salon"/>
  <calaos:link name="target_temp" io="chauffage_salon_target_temp"/>
  <calaos:link name="action" io="chauffage_salon_action"/>
  <calaos:link name="output_heat" io="relay_heat"/>
  <calaos:param name="emitter" value="electric"/>
  <calaos:param name="hysteresis" value="0.3"/>
  <calaos:param name="min_on_time" value="120"/>
  <calaos:param name="min_off_time" value="120"/>
  <calaos:param name="sensor_timeout" value="9000"/>
  <calaos:param name="put_back_delay" value="1"/>
  <calaos:param name="put_back_delay_max" value="60"/>
</calaos:device>)";
    const QByteArray wrapped = "<calaos:x xmlns:calaos=\"http://www.calaos.fr\">" + documented.toUtf8() + "</calaos:x>";

    const QJsonObject t = shippedTemplate("thermostat_heat");
    QVariantMap v = DeviceTemplates::defaults(t, roomNamed("Salon"));
    v["name"] = "Chauffage salon";
    v["current_temp"] = "temp_salon";
    v["output_heat"] = "relay_heat";
    v["emitter"] = "electric";
    QVERIFY(DeviceTemplates::create(t, v));

    QCOMPARE(deviceElement(saveIo(work("doc.xml")), "chauffage_salon"), deviceElement(wrapped, "chauffage_salon"));
}

void tst_DeviceTemplates::substitutionFollowsTheRules()
{
    const QJsonObject t = synthetic();
    QVariantMap v = DeviceTemplates::defaults(t, roomNamed("Cave"));
    v["name"] = "Été 2";
    v["label"] = "lbl";
    v["count"] = 7.5;
    v["flags"] = QStringList { "c", "a" };
    v["advanced"] = true;
    v["target"] = "text_out";

    QCOMPARE(DeviceTemplates::substitute("{flags}|{advanced}|{count}|{room}|{id}|{nope}|{label}", t, v, "dev"),
             QString("a,c|true|7.5|Cave|dev|{nope}|lbl"));
    QCOMPARE(DeviceTemplates::substitute("{ count}{}{", t, v, "dev"), QString("{ count}{}{"));

    DeviceTemplates::Plan p = DeviceTemplates::plan(t, v, &DeviceTemplates::isTakenInHouse);
    QCOMPARE(u(p.device["id"]), QString("ete_2"));
    QCOMPARE(u(p.device["extra"]), QString("lbl of ete_2"));
    QCOMPARE(u(p.device["lbl"]), QString("lbl"));
    QCOMPARE(u(p.device["kept"]), QString("{nothing}"));
    QCOMPARE(u(p.device["type"]), QString("fam"));
    QCOMPARE(u(p.device["impl"]), QString("imp"));

    //The `!label` IO is left out, so is its link; the command link resolves both owned IOs.
    QCOMPARE(int(p.ios.size()), 2);
    QCOMPARE(u(p.ios[0].params["topic_sub"]), QString("t/ete_2/state"));
    QCOMPARE(u(p.ios[0].params["user"]), QString("lbl"));
    QCOMPARE(int(p.links.size()), 3);
    QCOMPARE(u(p.links[0].name), QString("state"));
    QCOMPARE(u(p.links[0].io), QString("ete_2_state"));
    QCOMPARE(u(p.links[0].commandIo), QString("ete_2_cmd"));
    QCOMPARE(u(p.links[1].io), QString("text_out"));
    QCOMPARE(u(p.links[2].name), QString("cond"));

    QMap<QString, QString> params;
    for (const auto &e: p.params)
        params[u(e.first)] = u(e.second);
    QCOMPARE(params, (QMap<QString, QString> { { "count", "7.5" }, { "flags", "a,c" }, { "advanced", "true" },
                                                { "p", "1" }, { "tuning", "3" }, { "kept", "{unknown} {}" },
                                                { "labelled", "yes" } }));
    QCOMPARE(p.notes, QStringList({ "Topic t/ete_2/state of Été 2", "only advanced" }));

    //The other way round: no flags, so no command IO and no state link (its command is missing);
    //no label, so the var IO; empty values are not written.
    v["flags"] = QStringList();
    v["label"] = QString();
    v["advanced"] = false;
    v["target"] = QString();
    DeviceTemplates::Plan q = DeviceTemplates::plan(t, v, &DeviceTemplates::isTakenInHouse);
    QCOMPARE(int(q.ios.size()), 2);
    QCOMPARE(u(q.ios[1].key), QString("var"));
    QVERIFY(!q.ios[0].params.Exists("user"));
    QCOMPARE(u(q.ios[1].params["value"]), QString("3"));
    QCOMPARE(int(q.links.size()), 1);
    QCOMPARE(u(q.links[0].name), QString("var"));
    QStringList names;
    for (const auto &e: q.params)
        names << u(e.first);
    QCOMPARE(names, QStringList({ "count", "advanced", "p", "tuning", "kept" }));
    QCOMPARE(q.notes, QStringList({ "Topic t/ete_2/state of Été 2" }));
    QVERIFY(!q.device.Exists("lbl"));
}

void tst_DeviceTemplates::anEnumOptionSetsItsParams()
{
    const QJsonObject t = synthetic();
    QVariantMap v = DeviceTemplates::defaults(t, roomNamed("Salon"));
    v["name"] = "n";
    auto pOf = [&]()
    {
        for (const auto &e: DeviceTemplates::plan(t, v, &DeviceTemplates::isTakenInHouse).params)
            if (e.first == "p")
                return u(e.second);
        return QString();
    };
    QCOMPARE(pOf(), QString("1"));
    v["kind"] = "y";
    QCOMPARE(pOf(), QString("2"));
}

void tst_DeviceTemplates::aTakenIdGetsTheFirstFreeSuffix()
{
    //Device id held by an IO, owned ids held by an IO, a rule's dangling reference and a dangling link.
    const QByteArray house = QByteArray(houseXml()).replace(
        "<calaos:room name=\"Cave\" type=\"cave\" hits=\"0\">",
        "<calaos:room name=\"Cave\" type=\"cave\" hits=\"0\">\n"
        "  <calaos:internal id=\"heat\" type=\"InternalBool\" name=\"x\"/>\n"
        "  <calaos:internal id=\"heat_0_target_temp\" type=\"InternalBool\" name=\"x\"/>\n"
        "  <calaos:device id=\"d2\" type=\"climate\" impl=\"proxy\" name=\"d2\">\n"
        "    <calaos:link name=\"mode\" io=\"heat_0_action\"/>\n"
        "  </calaos:device>\n");
    QVERIFY(loadIo(work("taken.xml"), house));
    ListeRoom::reservePendingIOId("heat_0_target_temp_0");

    const QJsonObject t = shippedTemplate("thermostat_heat");
    QVariantMap v = filled(t);
    v["name"] = "Heat";
    Device *dev = DeviceTemplates::create(t, v);
    QVERIFY(dev);
    QCOMPARE(u(dev->get_param("id")), QString("heat_0"));
    QStringList owned;
    for (IOBase *io: dev->ownedIos())
        owned << u(io->get_param("id"));
    QCOMPARE(owned, QStringList({ "heat_0_target_temp_1", "heat_0_action_0" }));
    QCOMPARE(u(dev->links()[1].io), QString("heat_0_target_temp_1"));
    QCOMPARE(u(dev->links()[2].io), QString("heat_0_action_0"));
    ListeRoom::clearPendingIOIds();

    QCOMPARE(DeviceTemplates::freeId("a", [](const std::string &) { return false; }), std::string("a"));
    std::set<std::string> held = { "a", "a_0", "a_1" };
    QCOMPARE(DeviceTemplates::freeId("a", [&](const std::string &id) { return held.count(id) > 0; }), std::string("a_2"));
}

void tst_DeviceTemplates::idsAssignedByTheWizardAreTakenToo()
{
    //Key b_0 wants the id that key b fell back to.
    QJsonObject t = synthetic();
    t["ios"] = QJsonDocument::fromJson(R"([
        { "key": "b", "element": "internal", "type": "InternalBool", "params": { "name": "b" } },
        { "key": "b_0", "element": "internal", "type": "InternalBool", "params": { "name": "b0" } } ])").array();
    t["links"] = QJsonArray();
    QVariantMap v = DeviceTemplates::defaults(t, roomNamed("Salon"));
    v["name"] = "Flag";

    //"flag" is an IO of the house: the device is flag_0, then flag_0_b is free.
    std::set<std::string> held = { "flag", "flag_0_b" };
    DeviceTemplates::Plan p = DeviceTemplates::plan(t, v, [&](const std::string &id) { return held.count(id) > 0; });
    QCOMPARE(u(p.device["id"]), QString("flag_0"));
    QCOMPARE(u(p.ios[0].params["id"]), QString("flag_0_b_0"));
    QCOMPARE(u(p.ios[1].params["id"]), QString("flag_0_b_0_0"));
}

void tst_DeviceTemplates::aSecondDeviceFromTheSameTemplateGetsItsOwnIds()
{
    //Same sensor; each its own output (decision 17).
    const QJsonObject t = shippedTemplate("thermostat_heat");
    QVariantMap v = filled(t);
    Device *a = DeviceTemplates::create(t, v);
    v["output_heat"] = "relay_cool";
    Device *b = DeviceTemplates::create(t, v);
    QVERIFY(a && b);
    QCOMPARE(u(a->get_param("id")), QString("chauffage_salon"));
    QCOMPARE(u(b->get_param("id")), QString("chauffage_salon_0"));
    QCOMPARE(u(b->ownedIos()[0]->get_param("id")), QString("chauffage_salon_0_target_temp"));

    std::set<std::string> ids;
    for (Device *d: { a, b })
    {
        ids.insert(d->get_param("id"));
        for (IOBase *io: d->ownedIos())
            QVERIFY(ids.insert(io->get_param("id")).second);
    }

    const QByteArray saved = saveIo(work("two.xml"));
    QVERIFY(loadIo(work("two_reload.xml"), saved));
    QCOMPARE(m_dialogs, QStringList());
    QCOMPARE(int(ListeRoom::Instance().get_device("chauffage_salon_0")->ownedIos().size()), 2);
}

void tst_DeviceTemplates::theDeviceIdComesFromItsName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("id");
    QTest::newRow("plain") << "Chauffage salon" << "chauffage_salon";
    QTest::newRow("accents") << "Clim chambre d'Élise" << "clim_chambre_d_elise";
    QTest::newRow("punctuation") << "  --Salon (RDC) #2-- " << "salon_rdc_2";
    QTest::newRow("nothing left") << "° — °" << "device";
}

void tst_DeviceTemplates::theDeviceIdComesFromItsName()
{
    QFETCH(QString, name);
    QFETCH(QString, id);
    QCOMPARE(u(DeviceTemplates::idBaseFor(name)), id);
}

void tst_DeviceTemplates::ioFieldsListTheIosThatFit_data()
{
    QTest::addColumn<QString>("filter");
    QTest::addColumn<QStringList>("ids");
    //Room order, inputs before outputs; never a device, owned IOs of other devices included.
    QTest::newRow("number") << R"({"value": "number", "writable": false})"
                            << QStringList { "temp_salon", "hum_salon", "temp_cave", "other_temp" };
    QTest::newRow("bool") << R"({"value": "bool", "writable": false})"
                          << QStringList { "switch_salon", "flag", "relay_heat", "relay_cool" };
    QTest::newRow("writable bool") << R"({"value": "bool", "writable": true})"
                                   << QStringList { "flag", "relay_heat", "relay_cool" };
    QTest::newRow("writable string") << R"({"value": "string", "writable": true})" << QStringList { "text_out" };
    QTest::newRow("writable number") << R"({"value": "number", "writable": true})" << QStringList();
    QTest::newRow("unknown kind") << R"({"value": "color", "writable": false})" << QStringList();
}

void tst_DeviceTemplates::ioFieldsListTheIosThatFit()
{
    QFETCH(QString, filter);
    QFETCH(QStringList, ids);
    QStringList got;
    for (IOBase *io: DeviceTemplates::candidates(QJsonDocument::fromJson(filter.toUtf8()).object()))
        got << u(io->get_param("id"));
    QCOMPARE(got, ids);
}

void tst_DeviceTemplates::linksTargetTheChosenIosInAnyRoom()
{
    const QJsonObject t = shippedTemplate("thermostat_floor_heat_cool");
    QVariantMap v = filled(t);
    v["current_temp"] = "temp_cave";
    v["humidity"] = "other_temp";
    v["output_heat"] = "flag";
    v["output_cool"] = "relay_cool";
    Device *dev = DeviceTemplates::create(t, v);
    QVERIFY(dev);

    QMap<QString, QString> links;
    for (const DeviceLink &l: dev->links())
        links[u(l.name)] = u(l.io);
    QCOMPARE(links["current_temp"], QString("temp_cave"));
    QCOMPARE(links["humidity"], QString("other_temp"));
    QCOMPARE(links["output_heat"], QString("flag"));
    QCOMPARE(links["output_cool"], QString("relay_cool"));

    //Linked, not adopted: they stay where they are.
    QCOMPARE(DeviceTemplates::ioById("other_temp")->getOwnerDevice(), ListeRoom::Instance().get_device("other"));
    QCOMPARE(ListeRoom::Instance().searchRoomByInput(DeviceTemplates::ioById("temp_cave")), roomNamed("Cave"));

    const QByteArray saved = saveIo(work("links.xml"));
    QVERIFY(deviceElement(saved, "chauffage_salon").contains("  link io=temp_cave name=current_temp"));
}

void tst_DeviceTemplates::anIoTheServerDroppedIsNotOffered()
{
    //The server would not create it: a link to it would stay unresolved.
    const QByteArray house = QByteArray(houseXml()).replace(
        "<calaos:room name=\"Cave\" type=\"cave\" hits=\"0\">",
        "<calaos:room name=\"Cave\" type=\"cave\" hits=\"0\">\n"
        "  <calaos:input id=\"zibase_temp\" type=\"ZibaseTemp\" name=\"Old sensor\"/>\n");
    QVERIFY(loadIo(work("legacy.xml"), house));
    QVERIFY(!DeviceTemplates::ioById("zibase_temp")->isSupportedByServer());

    QStringList ids;
    for (IOBase *io: DeviceTemplates::candidates(QJsonObject { { "value", "number" }, { "writable", false } }))
        ids << u(io->get_param("id"));
    QCOMPARE(ids, QStringList({ "temp_salon", "hum_salon", "temp_cave", "other_temp" }));

    const QJsonObject t = shippedTemplate("thermostat_heat");
    QVariantMap v = filled(t);
    v["current_temp"] = "zibase_temp";
    const QList<DeviceTemplates::Problem> problems = DeviceTemplates::validate(t, v);
    QCOMPARE(problems.size(), 1);
    QCOMPARE(problems.first().message, QString("Temperature sensor: \"Old sensor\" is not an IO of the expected kind."));
}

void tst_DeviceTemplates::validation_data()
{
    QTest::addColumn<QString>("tpl");
    QTest::addColumn<QVariantMap>("changes");
    QTest::addColumn<QString>("field");
    QTest::addColumn<QString>("message");

    QTest::newRow("required text") << "thermostat_heat" << QVariantMap { { "name", "" } } << "name" << "Name is required.";
    QTest::newRow("blank text") << "thermostat_heat" << QVariantMap { { "name", "   " } } << "name" << "Name is required.";
    QTest::newRow("empty multi") << "esphome_climate" << QVariantMap { { "modes", QStringList() } } << "modes"
                                 << "Modes: choose one at least.";
    QTest::newRow("unknown choice") << "esphome_climate" << QVariantMap { { "modes", QStringList { "heat", "boost" } } }
                                    << "modes" << "Modes: \"boost\" is not one of the choices.";
    QTest::newRow("below min") << "esphome_climate" << QVariantMap { { "temp_min", -1.0 } } << "temp_min"
                               << "Lowest setpoint must be between 0 °C and 50 °C.";
    QTest::newRow("above max") << "esphome_climate" << QVariantMap { { "mqtt_port", 70000.0 } } << "mqtt_port"
                               << "MQTT port must be between 1 and 65535.";
    QTest::newRow("not a number") << "esphome_climate" << QVariantMap { { "mqtt_port", "abc" } } << "mqtt_port"
                                  << "MQTT port must be a number.";
    QTest::newRow("unknown option") << "thermostat_heat" << QVariantMap { { "emitter", "nuclear" } } << "emitter"
                                    << "Emitter: \"nuclear\" is not one of the choices.";
    QTest::newRow("required io") << "thermostat_heat" << QVariantMap { { "current_temp", "" } } << "current_temp"
                                 << "Temperature sensor: choose an IO.";
    QTest::newRow("unknown io") << "thermostat_heat" << QVariantMap { { "current_temp", "ghost" } } << "current_temp"
                                << "Temperature sensor: no IO has the id \"ghost\".";
    QTest::newRow("wrong kind") << "thermostat_heat" << QVariantMap { { "output_heat", "switch_salon" } } << "output_heat"
                                << "Heating output: \"Salon switch\" is not an IO of the expected kind.";
    QTest::newRow("same output twice") << "thermostat_heat_cool"
                                       << QVariantMap { { "output_heat", "relay_heat" }, { "output_cool", "relay_heat" } }
                                       << "output_cool"
                                       << "Cooling output: \"Heating relay\" is already chosen for Heating output.";
    QTest::newRow("no room") << "thermostat_heat" << QVariantMap { { "room", QVariant::fromValue<Room *>(nullptr) } }
                             << "room" << "Room: choose a room.";
}

void tst_DeviceTemplates::validation()
{
    QFETCH(QString, tpl);
    QFETCH(QVariantMap, changes);
    QFETCH(QString, field);
    QFETCH(QString, message);

    const QJsonObject t = shippedTemplate(tpl);
    QVariantMap v = filled(t);
    QVERIFY(DeviceTemplates::validate(t, v).isEmpty());
    for (auto it = changes.constBegin();it != changes.constEnd();++it)
        v[it.key()] = it.value();

    const QList<DeviceTemplates::Problem> problems = DeviceTemplates::validate(t, v);
    QCOMPARE(problems.size(), 1);
    QCOMPARE(problems.first().field, field);
    QCOMPARE(problems.first().message, message);
}

void tst_DeviceTemplates::anOutputOfAnotherThermostatIsRefused()
{
    //Decision 17: the server ignores the output link of the second thermostat.
    const QByteArray house = QByteArray(houseXml()).replace(
        "<calaos:room name=\"Cave\" type=\"cave\" hits=\"0\">",
        "<calaos:room name=\"Cave\" type=\"cave\" hits=\"0\">\n"
        "  <calaos:device id=\"t1\" type=\"climate\" impl=\"thermostat\" name=\"Old thermostat\">\n"
        "    <calaos:link name=\"output_cool\" io=\"relay_cool\"/>\n"
        "  </calaos:device>\n"
        "  <calaos:device id=\"p1\" type=\"climate\" impl=\"proxy\" name=\"Proxy\">\n"
        "    <calaos:link name=\"mode\" io=\"relay_heat\"/>\n"
        "  </calaos:device>\n");
    QVERIFY(loadIo(work("claimed.xml"), house));

    const QJsonObject t = shippedTemplate("thermostat_heat_cool");
    QVariantMap v = filled(t);
    v["output_heat"] = "relay_cool";
    v["output_cool"] = "flag";
    const QList<DeviceTemplates::Problem> problems = DeviceTemplates::validate(t, v);
    QCOMPARE(problems.size(), 1);
    QCOMPARE(problems.first().field, QString("output_heat"));
    QCOMPARE(problems.first().message,
             QString("Heating output: \"Cooling relay\" is already an output of thermostat \"Old thermostat\"."));

    //Another kind of device linking it does not own it.
    v["output_heat"] = "relay_heat";
    QVERIFY(DeviceTemplates::validate(t, v).isEmpty());
}

void tst_DeviceTemplates::aTemplateWithoutRoomOrWithAnUnknownKindIsRefused()
{
    QJsonObject t = synthetic();
    QJsonArray fields = t["fields"].toArray();
    fields.removeAt(1);
    fields.append(QJsonObject { { "name", "colour" }, { "type", "color" }, { "label", "Colour" } });
    t["fields"] = fields;
    QJsonArray ios = t["ios"].toArray();
    ios.append(QJsonObject { { "key", "k" }, { "element", "widget" }, { "type", "InternalBool" } });
    t["ios"] = ios;

    QVariantMap v = DeviceTemplates::defaults(t, roomNamed("Salon"));
    v["name"] = "n";
    QStringList messages;
    for (const DeviceTemplates::Problem &p: DeviceTemplates::validate(t, v))
        messages << p.message;
    QCOMPARE(messages, QStringList({ "Colour: this installer does not know the field type \"color\".",
                                     "The template has no room field.",
                                     "The template declares an IO of unknown kind \"widget\"." }));
    QVERIFY(!DeviceTemplates::create(t, v));
}

void tst_DeviceTemplates::anInvalidSetCreatesNothing()
{
    const QJsonObject t = shippedTemplate("thermostat_heat");
    QVariantMap v = filled(t);
    v["output_heat"] = QString();
    const int before = roomNamed("Salon")->get_size_out();
    QVERIFY(!DeviceTemplates::create(t, v));
    QCOMPARE(roomNamed("Salon")->get_size_out(), before);
    QVERIFY(!ListeRoom::Instance().get_device("chauffage_salon"));
}

void tst_DeviceTemplates::theDeviceIsInsertedInTheTreeAsAFreshTreeShowsIt()
{
    auto shape = [](QTreeWidget *tree)
    {
        QStringList l;
        for (QTreeWidgetItemIterator it(tree);*it;++it)
        {
            int depth = 0;
            for (QTreeWidgetItem *p = (*it)->parent();p;p = p->parent())
                depth++;
            IOBase *io = ioOfItem(*it);
            QString what = io ? u(io->get_param("id")) : (*it)->text(0);
            if (dynamic_cast<QTreeWidgetItemDevice *>(*it))
                what = "device " + what;
            else if (dynamic_cast<QTreeWidgetItemInput *>(*it))
                what = "in " + what;
            else if (dynamic_cast<QTreeWidgetItemOutput *>(*it))
                what = "out " + what;
            l << QString(depth * 2, ' ') + what;
        }
        return l;
    };
    auto populate = [](QTreeWidget *tree)
    {
        tree->clear();
        for (int r = 0;r < ListeRoom::Instance().size();r++)
            populateRoomItems(new QTreeWidgetItemRoom(ListeRoom::Instance().get_room(r), tree));
    };

    QTreeWidget live, fresh;
    populate(&live);

    QStringList created;
    for (const char *id: { "esphome_climate", "thermostat_heat_cool" })
    {
        const QJsonObject t = shippedTemplate(id);
        QVariantMap v = filled(t);
        v["room"] = QVariant::fromValue(roomNamed("Cave"));
        Device *dev = DeviceTemplates::create(t, v);
        QVERIFY(dev);

        QTreeWidgetItemRoom *roomItem = nullptr;
        for (QTreeWidgetItemIterator it(&live);*it;++it)
            if (QTreeWidgetItemRoom *r = dynamic_cast<QTreeWidgetItemRoom *>(*it))
                if (r->getRoom() == roomNamed("Cave"))
                    roomItem = r;
        QVERIFY(roomItem);
        QTreeWidgetItemDevice *item = addDeviceItem(roomItem, dev, [&](QTreeWidgetItem *i) { created << ioOfItem(i)->get_param("id").c_str(); });
        QCOMPARE(item->getDevice(), dev);
        QCOMPARE(item->childCount(), int(dev->ownedIos().size()));
    }
    QCOMPARE(created.first(), QString("chauffage_salon"));
    populate(&fresh);
    QCOMPARE(shape(&live), shape(&fresh));
}

QTEST_MAIN(tst_DeviceTemplates)
#include "tst_devicetemplates.moc"
