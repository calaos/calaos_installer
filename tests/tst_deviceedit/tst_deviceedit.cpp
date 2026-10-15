#include <QtTest>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTimer>
#include <QTranslator>

#include "../tst_devicetemplates/fixture.h"
#include "ConfigOptions.h"
#include "DeviceEdit.h"
#include "DialogEditDevice.h"
#include "house.h"

using namespace Calaos;
using namespace Fixture;

namespace
{



QStringList names(const QList<DeviceEdit::Role> &roles)
{
    QStringList l;
    for (const DeviceEdit::Role &r: roles)
        l << r.name;
    return l;
}

DeviceEdit::Role roleNamed(Device *dev, const QString &name)
{
    for (const DeviceEdit::Role &r: DeviceEdit::roles(dev))
        if (r.name == name)
            return r;
    return DeviceEdit::Role();
}

QMap<QString, QString> linksOf(Device *dev)
{
    QMap<QString, QString> m;
    for (const DeviceLink &l: dev->links())
        m[u(l.name)] = u(l.io);
    return m;
}

QStringList paramsOf(Device *dev)
{
    QStringList l;
    for (const auto &p: dev->deviceParams())
        l << u(p.first) + "=" + u(p.second);
    return l;
}

QStringList problemTexts(const QList<DeviceEdit::Problem> &problems)
{
    QStringList l;
    for (const DeviceEdit::Problem &p: problems)
        l << p.field + ": " + p.message;
    return l;
}

void clickOk(QDialog *d)
{
    QTest::mouseClick(d->findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok), Qt::LeftButton);
}

void choose(QComboBox *box, const QString &data)
{
    QVERIFY(box);
    const int i = box->findData(data);
    QVERIFY2(i >= 0, qPrintable(data));
    box->setCurrentIndex(i);
}

}

class tst_DeviceEdit: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;
    QTimer m_dialogKiller;
    QStringList m_dialogs;
    QString m_shots;

    QString work(const QString &f) const { return m_workDir.filePath(f); }
    Device *th() { return ListeRoom::Instance().get_device("th"); }
    QByteArray saveRules(const QString &file)
    {
        QString p = work(file);
        return ProjectManager::saveRulesToFile(p) ? readFile(p) : QByteArray();
    }
    void shoot(QWidget *w, const QString &name);

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void rolesAndParamsComeFromIODoc();
    void labelsComeFromIODocOrFromTheName();
    void theDialogLabelsRolesAndParamsAndKeepsTheirIds();
    void aRoleTakesItsValueTypeFromIODoc();
    void aRoleWithoutValueTypeOffersEveryWritableIo();
    void anExistingLinkOfTheWrongTypeDoesNotChangeTheFilter();
    void theEmitterIsAComboBoxOfItsChoices();
    void linksToTheDevicesOwnIosAreNotEditable();
    void aLinkAndAParamChangedSurviveASave();
    void rulesAndOwnedIosAreUntouched();
    void anotherIoDropsTheCommandIo();
    void theDialogSaysACommandIoGoes();
    void whatIODocDoesNotDocumentIsShown();
    void aLinkOrAParamClearedIsRemoved();
    void refusals_data();
    void refusals();
    void ioFieldsOfferTheIosThatFit();
    void whatTheFileAlreadyHeldIsNotRefused();
    void itsOwnOutputsMaySwap();
    void requiredRolesAreReported();
    void okWithoutAChangeChangesNothing();
    void theDialogChangesTheDevice();
    void theDialogRefusesAndSaysWhy();
    void theDialogMarksTheRequiredRoles();
    void screenshots();
};

void tst_DeviceEdit::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_deviceedit");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    //IODoc follows ui/lang, not the locale of the machine running the tests.
    ConfigOptions::Instance().setOption("ui/lang", qEnvironmentVariable("CALAOS_SHOTS_LANG", "en"));

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
    m_shots = qEnvironmentVariable("CALAOS_SHOTS_DIR");
}

void tst_DeviceEdit::init()
{
    m_dialogs.clear();
    QVERIFY(loadIo(work("house.xml"), kIo));
    QVERIFY(writeFile(work("rules.xml"), kRules));
    QString r = work("rules.xml");
    QVERIFY(ProjectManager::loadRulesFromFile(r));
    QCOMPARE(ListeRule::Instance().size(), 2);
    QVERIFY(th());
}

void tst_DeviceEdit::cleanup()
{
    QCOMPARE(m_dialogs, QStringList());
    resetModel();
}

void tst_DeviceEdit::shoot(QWidget *w, const QString &name)
{
    if (m_shots.isEmpty())
        return;
    w->show();
    QApplication::processEvents();
    QVERIFY(w->grab().save(QDir(m_shots).filePath(name + ".png")));
}

void tst_DeviceEdit::rolesAndParamsComeFromIODoc()
{
    QVERIFY(DeviceEdit::isDocumented(th()));
    QCOMPARE(names(DeviceEdit::roles(th())),
             QStringList({ "current_temp", "target_temp", "target_temp_low", "target_temp_high", "action",
                           "output_heat", "output_cool", "output", "humidity" }));

    //The filter of a role picked from the house comes from the templates of the impl.
    QCOMPARE(roleNamed(th(), "current_temp").valueType, QString("number"));
    QVERIFY(!roleNamed(th(), "current_temp").writable);
    QCOMPARE(roleNamed(th(), "output_heat").valueType, QString("bool"));
    QVERIFY(roleNamed(th(), "output_heat").writable);
    QVERIFY(roleNamed(th(), "output_heat").required);
    QVERIFY(roleNamed(th(), "humidity").requiredWhen.isEmpty());
    QCOMPARE(roleNamed(th(), "output").replaces, QStringList({ "output_heat", "output_cool" }));
    //Unlinked: only the templates tell its kind.
    QCOMPARE(roleNamed(th(), "humidity").valueType, QString("number"));

    QStringList params;
    for (const DeviceEdit::Param &p: DeviceEdit::params(th()))
        params << p.name + (p.deviceParam ? "*" : "");
    //Read-only ones (id, impl, gui_type, io_type) are not offered.
    QCOMPARE(params, QStringList({ "emitter*", "enabled", "humidity_max*", "hysteresis*", "log_history", "logged",
                                   "min_off_time*", "min_on_time*", "min_range*", "modes*", "name",
                                   "put_back_delay*", "put_back_delay_max*", "sensor_timeout*", "valid_max*",
                                   "valid_min*", "visible" }));

    Params p = { { "id", "x" }, { "type", "f" }, { "impl", "nope" } };
    Device unknown(p);
    QVERIFY(!DeviceEdit::isDocumented(&unknown));
    QVERIFY(DeviceEdit::roles(&unknown).isEmpty());
}

void tst_DeviceEdit::labelsComeFromIODocOrFromTheName()
{
    QCOMPARE(DeviceEdit::labelFor(QJsonObject { { "label", "Room sensor" } }, "current_temp"), QString("Room sensor"));
    QCOMPARE(DeviceEdit::labelFor(QJsonObject(), "put_back_delay_max"), QString("Put back delay max"));
    QCOMPARE(roleNamed(th(), "output_heat").label, QString("Heating output"));
}

void tst_DeviceEdit::theDialogLabelsRolesAndParamsAndKeepsTheirIds()
{
    DialogEditDevice d(th());
    auto labelOf = [&](const QString &editor)
    {
        QWidget *w = d.findChild<QWidget *>(editor);
        QFormLayout *form = qobject_cast<QFormLayout *>(w->parentWidget()->parentWidget()->layout());
        QLabel *l = qobject_cast<QLabel *>(form->labelForField(w->parentWidget()));
        return l;
    };
    QLabel *role = labelOf("link_output_heat");
    QVERIFY(role);
    QVERIFY2(role->text().startsWith("Heating output"), qPrintable(role->text()));
    QCOMPARE(role->toolTip(), QString("output_heat"));
    QCOMPARE(d.findChild<QWidget *>("link_output_heat")->toolTip(), QString("output_heat"));

    QLabel *param = labelOf("param_hysteresis");
    QVERIFY(param);
    QCOMPARE(param->text(), QString("Hysteresis:"));
    QCOMPARE(param->toolTip(), QString("hysteresis"));
    QCOMPARE(d.findChild<QWidget *>("param_hysteresis")->toolTip(), QString("hysteresis"));
}

void tst_DeviceEdit::aRoleTakesItsValueTypeFromIODoc()
{
    //The proxy's low setpoint has no template field and no link in the file: only
    //IODoc says it is a number, and a relay must not be offered for it.
    Device *px = ListeRoom::Instance().get_device("px");
    const DeviceEdit::Role low = roleNamed(px, "target_temp_low");
    QCOMPARE(low.valueType, QString("number"));
    QVERIFY(low.writable);

    QStringList ids;
    for (IOBase *io: DeviceEdit::candidates(low, px))
        ids << u(io->get_param("id"));
    for (const char *notNumber: { "relay_heat", "relay_cool", "relay_spare", "relay_cave", "flag", "mode_cmd" })
        QVERIFY2(!ids.contains(notNumber), notNumber);
    QVERIFY(ids.contains("th_target_temp"));

    QCOMPARE(roleNamed(px, "mode").valueType, QString("string"));
    QCOMPARE(roleNamed(th(), "output_cool").valueType, QString("bool"));
}

void tst_DeviceEdit::aRoleWithoutValueTypeOffersEveryWritableIo()
{
    //A document older than value_type, or a type this installer does not know.
    DeviceEdit::Role legacy;
    legacy.name = "x";
    legacy.writable = true;
    QStringList ids;
    for (IOBase *io: DeviceEdit::candidates(legacy, th()))
        ids << u(io->get_param("id"));
    for (const char *id: { "relay_heat", "flag", "mode_cmd", "th2_target_temp" })
        QVERIFY2(ids.contains(id), id);
    QVERIFY(!ids.contains("temp_salon"));
}

void tst_DeviceEdit::anExistingLinkOfTheWrongTypeDoesNotChangeTheFilter()
{
    //The file links target_temp_low to a relay: IODoc still says number, so no relay is
    //offered, and the link is shown as it is and survives an OK.
    const QByteArray house = QByteArray(kIo).replace(
        "<calaos:link name=\"mode\" io=\"mode_state\" command_io=\"mode_cmd\"/>",
        "<calaos:link name=\"mode\" io=\"mode_state\" command_io=\"mode_cmd\"/>\n"
        "    <calaos:link name=\"target_temp_low\" io=\"relay_cave\"/>");
    QVERIFY(loadIo(work("wrongtype.xml"), house));
    Device *px = ListeRoom::Instance().get_device("px");
    QVERIFY(px);

    const DeviceEdit::Role low = roleNamed(px, "target_temp_low");
    QCOMPARE(low.valueType, QString("number"));
    for (IOBase *io: DeviceEdit::candidates(low, px))
        QVERIFY2(u(io->get_param("id")) != "relay_cave", "relay offered");

    DialogEditDevice d(px);
    QComboBox *box = d.findChild<QComboBox *>("link_target_temp_low");
    QVERIFY(box);
    QCOMPARE(box->currentData().toString(), QString("relay_cave"));
    QVERIFY2(box->currentText().contains("of another kind"), qPrintable(box->currentText()));
    for (int i = 0;i < box->count();i++)
        if (box->itemData(i).toString() != "relay_cave")
            QVERIFY2(!box->itemText(i).contains("relay", Qt::CaseInsensitive), qPrintable(box->itemText(i)));

    clickOk(&d);
    QCOMPARE(linksOf(px)["target_temp_low"], QString("relay_cave"));
}

void tst_DeviceEdit::theEmitterIsAComboBoxOfItsChoices()
{
    QStringList keys;
    for (const DeviceEdit::Param &p: DeviceEdit::params(th()))
        if (p.name == "emitter")
        {
            QCOMPARE(p.type, QString("list"));
            keys = p.listValues.keys();
        }
    QCOMPARE(keys, QStringList({ "electric", "fan_coil", "floor", "heat_pump", "water_radiator" }));

    DialogEditDevice d(th());
    QComboBox *emitter = d.findChild<QComboBox *>("param_emitter");
    QVERIFY(emitter);
    QCOMPARE(emitter->currentData().toString(), QString("heat_pump"));
    QVERIFY(emitter->findData("floor") >= 0);

    choose(emitter, "floor");
    clickOk(&d);
    QVERIFY(paramsOf(th()).contains("emitter=floor"));
}

void tst_DeviceEdit::linksToTheDevicesOwnIosAreNotEditable()
{
    for (const char *role: { "target_temp", "target_temp_low", "target_temp_high", "action" })
        QVERIFY2(DeviceEdit::isOwnedRole(th(), role), role);
    QVERIFY(!DeviceEdit::isOwnedRole(th(), "current_temp"));
    QVERIFY(!DeviceEdit::isOwnedRole(th(), "humidity"));

    const DeviceEdit::State s = DeviceEdit::current(th());
    QCOMPARE(s.links.keys(), QStringList({ "current_temp", "humidity", "output", "output_cool", "output_heat" }));
    QCOMPARE(s.links["humidity"], QString());
    QCOMPARE(s.links["output"], QString());
    QCOMPARE(s.params["hysteresis"], QString("0.5"));
    QCOMPARE(s.params["name"], QString("Chauffage salon"));
    QVERIFY(!s.params.contains("humidity_max"));

    //An owned role slipped into a state is left alone.
    DeviceEdit::State t = s;
    t.links["target_temp"] = "temp_cave";
    QVERIFY(DeviceEdit::apply(th(), t));
    QCOMPARE(linksOf(th())["target_temp"], QString("th_target_temp"));
}

void tst_DeviceEdit::aLinkAndAParamChangedSurviveASave()
{
    const QStringList ownedBefore = deviceElement(saveIo(work("before.xml")), "th").filter(QRegularExpression("^  internal "));
    QCOMPARE(ownedBefore.size(), 4);

    DeviceEdit::State s = DeviceEdit::current(th());
    s.links["current_temp"] = "temp_cave";
    s.links["humidity"] = "hum_salon";
    s.params["hysteresis"] = "0.7";
    s.params["humidity_max"] = "60";
    s.params["name"] = "Salon";
    QCOMPARE(problemTexts(DeviceEdit::validate(th(), s)), QStringList());
    QVERIFY(DeviceEdit::apply(th(), s));

    const QByteArray saved = saveIo(work("edited.xml"));
    QVERIFY(saved.contains("<calaos:home format=\"2\">"));
    QVERIFY(loadIo(work("reload.xml"), saved));
    Device *dev = th();
    QVERIFY(dev);
    QCOMPARE(u(dev->get_param("name")), QString("Salon"));
    QCOMPARE(linksOf(dev)["current_temp"], QString("temp_cave"));
    QCOMPARE(linksOf(dev)["humidity"], QString("hum_salon"));
    QCOMPARE(linksOf(dev)["target_temp"], QString("th_target_temp"));
    //In place, the new one last.
    QCOMPARE(paramsOf(dev), QStringList({ "emitter=heat_pump", "hysteresis=0.7", "min_on_time=600", "min_off_time=300",
                                          "min_range=2", "sensor_timeout=9000", "humidity_max=60" }));
    QCOMPARE(deviceElement(saved, "th").filter(QRegularExpression("^  internal ")), ownedBefore);
    QCOMPARE(saveIo(work("again.xml")), saved);

    //For a load by the real server (tests/server_load).
    const QString dump = qEnvironmentVariable("CALAOS_DEVICE_XML_DIR");
    if (!dump.isEmpty())
        QVERIFY(writeFile(QDir(dump).filePath("edited_thermostat.xml"), saved));
}

void tst_DeviceEdit::rulesAndOwnedIosAreUntouched()
{
    const QByteArray rulesBefore = saveRules("rules_before.xml");
    IOBase *target = DeviceTemplates::ioById("th_target_temp");
    const std::vector<IOBase *> owned = th()->ownedIos();

    DeviceEdit::State s = DeviceEdit::current(th());
    s.links["output_heat"] = "relay_spare";
    s.params["min_range"] = "3";
    QVERIFY(DeviceEdit::apply(th(), s));

    QCOMPARE(saveRules("rules_after.xml"), rulesBefore);
    QCOMPARE(ListeRule::Instance().size(), 2);
    QCOMPARE(DeviceTemplates::ioById("th_target_temp"), target);
    QVERIFY(th()->ownedIos() == owned);
    QCOMPARE(u(th()->get_param("id")), QString("th"));
    for (IOBase *io: owned)
        QCOMPARE(io->getOwnerDevice(), th());
}

void tst_DeviceEdit::anotherIoDropsTheCommandIo()
{
    //The server would go on writing the commands to the old IO.
    Device *px = ListeRoom::Instance().get_device("px");
    DeviceEdit::State s = DeviceEdit::current(px);
    QCOMPARE(s.links["mode"], QString("mode_state"));

    //Unchanged, it stays.
    s.params["sensor_timeout"] = "600";
    QVERIFY(DeviceEdit::apply(px, s));
    QCOMPARE(u(px->links()[0].commandIo), QString("mode_cmd"));

    s.links["mode"] = "mode_other";
    QVERIFY(DeviceEdit::apply(px, s));
    QCOMPARE(u(px->links()[0].io), QString("mode_other"));
    QCOMPARE(u(px->links()[0].commandIo), QString());
    QVERIFY(!deviceElement(saveIo(work("command.xml")), "px").join('\n').contains("command_io"));
}

void tst_DeviceEdit::theDialogSaysACommandIoGoes()
{
    DialogEditDevice d(ListeRoom::Instance().get_device("px"));
    bool said = false;
    for (QLabel *l: d.findChild<QWidget *>("link_mode")->parentWidget()->findChildren<QLabel *>())
        said |= l->text().contains("Commands go to AC mode command (mode_cmd) - Cave. Picking another IO drops it");
    QVERIFY(said);
}

void tst_DeviceEdit::whatIODocDoesNotDocumentIsShown()
{
    const QByteArray house = QByteArray(kIo)
        .replace("<calaos:link name=\"action\" io=\"th_action\"/>",
                 "<calaos:link name=\"action\" io=\"th_action\" command_io=\"flag\"/>\n"
                 "    <calaos:link name=\"extra_role\" io=\"relay_spare\"/>")
        .replace("<calaos:param name=\"emitter\" value=\"heat_pump\"/>",
                 "<calaos:param name=\"emitter\" value=\"heat_pump\"/>\n    <calaos:param name=\"old_knob\" value=\"7\"/>");
    QVERIFY(loadIo(work("undocumented.xml"), house));

    DialogEditDevice d(th());
    QStringList texts;
    for (QLabel *l: d.findChildren<QLabel *>())
        texts << l->text();
    const QString all = texts.join('\n');
    QVERIFY2(all.contains("extra_role: Spare relay (relay_spare) - Salon"), qPrintable(all));
    QVERIFY(all.contains("old_knob = 7"));
    //The command IO of a link the editor does not change is shown too.
    QVERIFY(all.contains("Commands go to Flag (flag) - Salon."));

    //And kept as they are.
    clickOk(&d);
    QVERIFY(linksOf(th()).contains("extra_role"));
    QVERIFY(paramsOf(th()).contains("old_knob=7"));
}

void tst_DeviceEdit::aLinkOrAParamClearedIsRemoved()
{
    DeviceEdit::State s = DeviceEdit::current(th());
    s.links["output_cool"] = QString();
    s.params.remove("min_range");
    s.params.remove("visible");
    QVERIFY(DeviceEdit::apply(th(), s));
    QVERIFY(!linksOf(th()).contains("output_cool"));
    QVERIFY(!paramsOf(th()).join(' ').contains("min_range"));
    QVERIFY(deviceElement(saveIo(work("cleared.xml")), "th").filter("link").filter("output_cool").isEmpty());

    s.params["visible"] = "false";
    QVERIFY(DeviceEdit::apply(th(), s));
    QCOMPARE(u(th()->get_param("visible")), QString("false"));
}

void tst_DeviceEdit::refusals_data()
{
    QTest::addColumn<QString>("key");
    QTest::addColumn<QString>("value");
    QTest::addColumn<QString>("message");

    QTest::newRow("decision 17") << "output_heat" << "relay_cave"
                                 << "output_heat: Heating output: \"Cave relay\" is already an output of thermostat \"Cave thermostat\".";
    QTest::newRow("legacy") << "current_temp" << "zibase"
                            << "current_temp: Current temperature: \"Old sensor\" is not an IO of the expected kind.";
    QTest::newRow("not writable") << "output_cool" << "switch_salon"
                                  << "output_cool: Cooling output: \"Salon switch\" is not an IO of the expected kind.";
    QTest::newRow("value type") << "current_temp" << "relay_spare"
                                << "current_temp: Current temperature: \"Spare relay\" is not an IO of the expected kind.";
    QTest::newRow("heat and cool") << "output_cool" << "relay_heat"
                                   << "output_cool: Cooling output: \"Heating relay\" is already linked as Heating output.";
    QTest::newRow("own IO") << "current_temp" << "th_target_temp"
                            << "current_temp: Current temperature: \"Chauffage salon setpoint\" is one of this device's own IOs.";
    QTest::newRow("unknown id") << "humidity" << "ghost" << "humidity: Humidity sensor: no IO has the id \"ghost\".";
    QTest::newRow("float") << "param:hysteresis" << "abc" << "hysteresis: Hysteresis must be a number.";
    QTest::newRow("bool") << "param:visible" << "yes" << "visible: Visible must be true or false.";
    QTest::newRow("name") << "param:name" << " " << "name: Name is required.";
}

void tst_DeviceEdit::refusals()
{
    QFETCH(QString, key);
    QFETCH(QString, value);
    QFETCH(QString, message);

    const QByteArray house = QByteArray(kIo).replace(
        "<calaos:room name=\"Cave\" type=\"cave\" hits=\"0\">",
        "<calaos:room name=\"Cave\" type=\"cave\" hits=\"0\">\n"
        "  <calaos:input id=\"zibase\" type=\"ZibaseTemp\" name=\"Old sensor\"/>\n");
    QVERIFY(loadIo(work("refusals.xml"), house));
    const QString before = QString::fromUtf8(saveIo(work("refusals_before.xml")));

    DeviceEdit::State s = DeviceEdit::current(th());
    if (key.startsWith("param:"))
        s.params[key.mid(6)] = value;
    else
        s.links[key] = value;

    QCOMPARE(problemTexts(DeviceEdit::validate(th(), s)), QStringList({ message }));
    QVERIFY(!DeviceEdit::apply(th(), s));
    QCOMPARE(QString::fromUtf8(saveIo(work("refusals_after.xml"))), before);
}

void tst_DeviceEdit::ioFieldsOfferTheIosThatFit()
{
    const QByteArray house = QByteArray(kIo).replace(
        "<calaos:room name=\"Cave\" type=\"cave\" hits=\"0\">",
        "<calaos:room name=\"Cave\" type=\"cave\" hits=\"0\">\n"
        "  <calaos:input id=\"zibase\" type=\"ZibaseTemp\" name=\"Old sensor\"/>\n");
    QVERIFY(loadIo(work("candidates.xml"), house));

    auto ids = [this](const DeviceEdit::Role &r)
    {
        QStringList l;
        for (IOBase *io: DeviceEdit::candidates(r, th()))
            l << u(io->get_param("id"));
        return l;
    };
    //Never a device, nor a type the server dropped, nor one of its own IOs: a thermostat
    //regulating on its own setpoint. An owned IO of another device is an IO like another.
    QCOMPARE(ids(roleNamed(th(), "current_temp")),
             QStringList({ "temp_salon", "hum_salon", "temp_cave", "th2_target_temp" }));
    QCOMPARE(ids(roleNamed(th(), "output_heat")),
             QStringList({ "flag", "relay_heat", "relay_cool", "relay_spare", "relay_cave" }));
}

void tst_DeviceEdit::whatTheFileAlreadyHeldIsNotRefused()
{
    //th3 already shares th2's output in the file: changing one of its params is not refused.
    const QByteArray house = QByteArray(kIo).replace(
        "</calaos:room>\n</calaos:home>",
        "  <calaos:device id=\"th3\" type=\"climate\" impl=\"thermostat\" name=\"Third\">\n"
        "    <calaos:link name=\"output_heat\" io=\"relay_cave\"/>\n"
        "    <calaos:link name=\"output_cool\" io=\"relay_cave\"/>\n"
        "    <calaos:param name=\"hysteresis\" value=\"oops\"/>\n"
        "  </calaos:device>\n</calaos:room>\n</calaos:home>");
    QVERIFY(loadIo(work("held.xml"), house));
    Device *th3 = ListeRoom::Instance().get_device("th3");
    QVERIFY(th3);

    DeviceEdit::State s = DeviceEdit::current(th3);
    s.params["min_on_time"] = "60";
    QCOMPARE(problemTexts(DeviceEdit::validate(th3, s)), QStringList());
    QVERIFY(DeviceEdit::apply(th3, s));
    QVERIFY(paramsOf(th3).contains("min_on_time=60"));
}

void tst_DeviceEdit::itsOwnOutputsMaySwap()
{
    //Decision 17 is between thermostats: this one already owning both is no conflict.
    DeviceEdit::State s = DeviceEdit::current(th());
    s.links["output_heat"] = "relay_cool";
    s.links["output_cool"] = "relay_heat";
    QCOMPARE(problemTexts(DeviceEdit::validate(th(), s)), QStringList());
    QVERIFY(DeviceEdit::apply(th(), s));
    QCOMPARE(linksOf(th())["output_heat"], QString("relay_cool"));
    QCOMPARE(linksOf(th())["output_cool"], QString("relay_heat"));
}

void tst_DeviceEdit::requiredRolesAreReported()
{
    DeviceEdit::State s = DeviceEdit::current(th());
    QCOMPARE(DeviceEdit::missingRequired(th(), s), QStringList());

    //Humidity is optional since decision 22, emitter=floor included.
    s.params["emitter"] = "floor";
    QCOMPARE(DeviceEdit::missingRequired(th(), s), QStringList());

    //No shipped role has a required_when any more: the rule on a role built here,
    //the shape humidity had (emitter=floor and output_cool linked).
    DeviceEdit::Role when = roleNamed(th(), "humidity");
    when.requiredWhen = QJsonObject { { "param", "emitter" }, { "value", "floor" }, { "link", "output_cool" } };
    QVERIFY(DeviceEdit::isRequired(th(), when, s));
    s.params["emitter"] = "electric";
    QVERIFY(!DeviceEdit::isRequired(th(), when, s));
    s.params.remove("emitter");
    QVERIFY(!DeviceEdit::isRequired(th(), when, s));
    s.params["emitter"] = "floor";
    s.links["output_cool"] = QString();
    QVERIFY(!DeviceEdit::isRequired(th(), when, s));
    s.links["output_cool"] = "ghost";
    QVERIFY(!DeviceEdit::isRequired(th(), when, s));
    when.requiredWhen.remove("link");
    QVERIFY(DeviceEdit::isRequired(th(), when, s));
    s.links["output_cool"] = QString();

    s.links["current_temp"] = QString();
    QCOMPARE(DeviceEdit::missingRequired(th(), s), QStringList({ "Current temperature" }));
    //Reported, not refused: the server loads the device without that capability.
    QVERIFY(DeviceEdit::validate(th(), s).isEmpty());

    //A link to an IO the house no longer has gives nothing either.
    QVERIFY(loadIo(work("ghost.xml"), QByteArray(kIo).replace("<calaos:link name=\"current_temp\" io=\"temp_salon\"/>",
                                                                "<calaos:link name=\"current_temp\" io=\"ghost\"/>")));
    QCOMPARE(DeviceEdit::missingRequired(th(), DeviceEdit::current(th())),
             QStringList({ "Current temperature (\"ghost\" not found)" }));
}

void tst_DeviceEdit::okWithoutAChangeChangesNothing()
{
    //Raw values a form would not write back as they are.
    const QByteArray house = QByteArray(kIo)
        .replace("<calaos:param name=\"hysteresis\" value=\"0.5\"/>", "<calaos:param name=\"hysteresis\" value=\" 0.5\"/>")
        .replace("<calaos:param name=\"min_range\" value=\"2\"/>", "<calaos:param name=\"min_range\" value=\"\"/>")
        .replace("impl=\"thermostat\" name=\"Chauffage salon\">", "impl=\"thermostat\" name=\"Chauffage salon\" visible=\"1\">");
    QVERIFY(loadIo(work("raw.xml"), house));
    const QByteArray before = saveIo(work("raw_before.xml"));

    DialogEditDevice d(th());
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    clickOk(&d);
    QCOMPARE(d.result(), int(QDialog::Accepted));
    QVERIFY(!d.changed());
    QCOMPARE(QString::fromUtf8(saveIo(work("raw_after.xml"))), QString::fromUtf8(before));
}

void tst_DeviceEdit::theDialogChangesTheDevice()
{
    DialogEditDevice d(th());
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));

    QVERIFY(qobject_cast<QLabel *>(d.findChild<QWidget *>("link_target_temp")));
    QVERIFY(d.findChild<QLabel *>("link_target_temp")->text().contains("th_target_temp"));
    QComboBox *sensor = d.findChild<QComboBox *>("link_current_temp");
    QCOMPARE(sensor->currentData().toString(), QString("temp_salon"));
    QCOMPARE(sensor->itemText(0), QString("None"));
    choose(sensor, "temp_cave");

    QLineEdit *hyst = d.findChild<QLineEdit *>("param_hysteresis");
    QCOMPARE(hyst->text(), QString("0.5"));
    hyst->selectAll();
    QTest::keyClicks(hyst, "0.8");
    QCOMPARE(d.findChild<QLineEdit *>("param_humidity_max")->placeholderText(), QString("Default: 65"));
    //Emptied: back to the server's default, not written.
    d.findChild<QLineEdit *>("param_min_range")->clear();
    //A bool the file does not hold is not written unless it is clicked.
    QVERIFY(d.findChild<QCheckBox *>("param_visible")->isChecked());
    QVERIFY(!d.state().params.contains("visible"));

    clickOk(&d);
    QCOMPARE(d.result(), int(QDialog::Accepted));
    QVERIFY(d.changed());
    QCOMPARE(linksOf(th())["current_temp"], QString("temp_cave"));
    QVERIFY(paramsOf(th()).contains("hysteresis=0.8"));
    QVERIFY(!paramsOf(th()).join(' ').contains("min_range"));
    QVERIFY(!th()->get_params().Exists("visible"));
}

void tst_DeviceEdit::theDialogRefusesAndSaysWhy()
{
    DialogEditDevice d(th());
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));

    //Legacy and device IOs are not even offered.
    QComboBox *heat = d.findChild<QComboBox *>("link_output_heat");
    choose(heat, "relay_cave");
    QLineEdit *hyst = d.findChild<QLineEdit *>("param_hysteresis");
    hyst->selectAll();
    QTest::keyClicks(hyst, "warm");
    clickOk(&d);

    QVERIFY(d.result() != QDialog::Accepted);
    QLabel *error = d.findChild<QLabel *>("labelError");
    QVERIFY(!error->isHidden());
    QCOMPARE(error->text(), QString("Heating output: \"Cave relay\" is already an output of thermostat \"Cave thermostat\".\n"
                                    "Hysteresis must be a number."));
    QCOMPARE(d.focusWidget(), heat);
    QCOMPARE(linksOf(th())["output_heat"], QString("relay_heat"));
    QVERIFY(paramsOf(th()).contains("hysteresis=0.5"));
}

void tst_DeviceEdit::theDialogMarksTheRequiredRoles()
{
    DialogEditDevice d(th());
    QLabel *warning = d.findChild<QLabel *>("labelWarning");
    QVERIFY(warning->isHidden());

    auto labelOf = [&](const QString &role)
    {
        QWidget *editor = d.findChild<QWidget *>(role.startsWith("param_") ? role : "link_" + role);
        QFormLayout *form = qobject_cast<QFormLayout *>(editor->parentWidget()->parentWidget()->layout());
        return qobject_cast<QLabel *>(form->labelForField(editor->parentWidget()))->text();
    };
    QCOMPARE(labelOf("current_temp"), QString("Current temperature (required):"));
    QCOMPARE(labelOf("humidity"), QString("Humidity sensor:"));

    //A floor no longer needs humidity (decision 22).
    choose(d.findChild<QComboBox *>("param_emitter"), "floor");
    QCOMPARE(labelOf("humidity"), QString("Humidity sensor:"));
    QVERIFY(warning->isHidden());
    QCOMPARE(labelOf("param_put_back_delay_max"), QString("Maximum put-back delay:"));

    choose(d.findChild<QComboBox *>("link_current_temp"), QString());
    QVERIFY(!warning->isHidden());
    QCOMPARE(warning->text(), QString("No IO for Current temperature: the device loads without what this link gives."));
    choose(d.findChild<QComboBox *>("link_current_temp"), "temp_salon");
    QVERIFY(warning->isHidden());
}

void tst_DeviceEdit::screenshots()
{
    if (m_shots.isEmpty())
        QSKIP("CALAOS_SHOTS_DIR is not set");

    const QString lang = qEnvironmentVariable("CALAOS_SHOTS_LANG", "en");
    DialogEditDevice d(th());
    shoot(&d, lang + "_edit_device");
    choose(d.findChild<QComboBox *>("link_output_heat"), "relay_cave");
    choose(d.findChild<QComboBox *>("param_emitter"), "floor");
    clickOk(&d);
    shoot(&d, lang + "_edit_device_refused");

    Device *th2 = ListeRoom::Instance().get_device("th2");
    DialogEditDevice other(th2);
    shoot(&other, lang + "_edit_device_without_options");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    //A compiled lang/calaos_installer_<lang>.qm for the French screenshots.
    const QString qm = qEnvironmentVariable("CALAOS_SHOTS_QM");
    if (!qm.isEmpty() && translator.load(qm))
        app.installTranslator(&translator);
    tst_DeviceEdit tc;
    QTEST_SET_MAIN_SOURCE_PATH
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_deviceedit.moc"
