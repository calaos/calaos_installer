#include <QtTest>
#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>

#include "../tst_devicetemplates/fixture.h"
#include "ConfigOptions.h"
#include "DeviceCommand.h"
#include "DeviceCommandForm.h"
#include "DeviceEdit.h"
#include "DialogEditDevice.h"
#include "IODoc.h"
#include "house.h"

using namespace Calaos;
using namespace Fixture;

namespace
{

Device *device(const char *id)
{
    return ListeRoom::Instance().get_device(id);
}

QJsonObject thermostatDoc()
{
    return IODoc::getDeviceObject("thermostat");
}

DeviceEdit::Role roleNamed(Device *dev, const QString &name)
{
    for (const DeviceEdit::Role &r: DeviceEdit::roles(dev))
        if (r.name == name)
            return r;
    return DeviceEdit::Role();
}

QStringList problemTexts(const QList<DeviceEdit::Problem> &problems)
{
    QStringList l;
    for (const DeviceEdit::Problem &p: problems)
        l << p.field + ": " + p.message;
    return l;
}

QMap<QString, QString> linksOf(Device *dev)
{
    QMap<QString, QString> m;
    for (const DeviceLink &l: dev->links())
        m[u(l.name)] = u(l.io);
    return m;
}

void choose(QComboBox *box, const QString &data)
{
    QVERIFY(box);
    const int i = box->findData(data);
    QVERIFY2(i >= 0, qPrintable(data));
    box->setCurrentIndex(i);
}

QStringList items(QComboBox *box)
{
    QStringList l;
    for (int i = 0;i < box->count();i++)
        l << box->itemData(i).toString();
    return l;
}

DeviceCommand::Field modeOf(const DeviceCommand::Offer &o)
{
    for (const DeviceCommand::Field &f: o.fields)
        if (f.name == "mode")
            return f;
    return DeviceCommand::Field();
}

}

class tst_ReplacedRoles: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;

    QString work(const QString &f) const { return m_workDir.filePath(f); }

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void theValveReplacesBothOutputs();
    void aRequiredRoleIsMetByTheRoleReplacingIt();
    void linkingTheValveWithAnOutputIsRefused_data();
    void linkingTheValveWithAnOutputIsRefused();
    void theOutputsMayGiveWayToTheValve();
    void bothLinksHeldByTheFileAreNotRefused();
    void theValveBelongsToOneThermostat();
    void theDialogMarksAndRefusesFromReplaces();
    void commandRequirementsAreMetByTheValve();
    void theFormOffersTheModesOfTheValve();
};

void tst_ReplacedRoles::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_replacedroles");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    ConfigOptions::Instance().setOption("ui/lang", "en");
}

void tst_ReplacedRoles::init()
{
    QVERIFY(loadIo(work("house.xml"), kIo));
    QVERIFY(device("zone"));
    QVERIFY(device("th"));
}

void tst_ReplacedRoles::cleanup()
{
    resetModel();
}

void tst_ReplacedRoles::theValveReplacesBothOutputs()
{
    QCOMPARE(roleNamed(device("zone"), "output").replaces, QStringList({ "output_heat", "output_cool" }));
    QVERIFY(roleNamed(device("zone"), "output_heat").replaces.isEmpty());
    QCOMPARE(IODoc::replacersOf(thermostatDoc(), "output_heat"), QStringList({ "output" }));
    QCOMPARE(IODoc::replacersOf(thermostatDoc(), "output_cool"), QStringList({ "output" }));
    QCOMPARE(IODoc::replacersOf(thermostatDoc(), "current_temp"), QStringList());
    QCOMPARE(IODoc::replacersOf(QJsonObject(), "output_heat"), QStringList());
}

void tst_ReplacedRoles::aRequiredRoleIsMetByTheRoleReplacingIt()
{
    Device *zone = device("zone");
    DeviceEdit::State s = DeviceEdit::current(zone);
    const DeviceEdit::Role heat = roleNamed(zone, "output_heat");
    QVERIFY(heat.required);
    QVERIFY(!DeviceEdit::isRequired(zone, heat, s));
    QCOMPARE(DeviceEdit::missingRequired(zone, s), QStringList());

    //An unresolved valve gives the server nothing: the output is missing again.
    s.links["output"] = "ghost";
    QVERIFY(DeviceEdit::isRequired(zone, heat, s));
    QCOMPARE(DeviceEdit::missingRequired(zone, s), QStringList({ "Heating output" }));
    s.links["output"] = QString();
    QCOMPARE(DeviceEdit::missingRequired(zone, s), QStringList({ "Heating output" }));
    s.links["output_heat"] = "relay_spare";
    QCOMPARE(DeviceEdit::missingRequired(zone, s), QStringList());
}

void tst_ReplacedRoles::linkingTheValveWithAnOutputIsRefused_data()
{
    QTest::addColumn<QString>("dev");
    QTest::addColumn<QString>("role");
    QTest::addColumn<QString>("io");
    QTest::addColumn<QStringList>("problems");

    const QString heat = "output: Zone valve and Heating output cannot both be linked: Zone valve replaces Heating output.";
    const QString cool = "output: Zone valve and Cooling output cannot both be linked: Zone valve replaces Cooling output.";
    QTest::newRow("heat on the zone") << "zone" << "output_heat" << "relay_spare" << QStringList { heat };
    QTest::newRow("cool on the zone") << "zone" << "output_cool" << "relay_spare" << QStringList { cool };
    QTest::newRow("valve on the outputs") << "th" << "output" << "relay_spare" << QStringList { heat, cool };
}

void tst_ReplacedRoles::linkingTheValveWithAnOutputIsRefused()
{
    QFETCH(QString, dev);
    QFETCH(QString, role);
    QFETCH(QString, io);
    QFETCH(QStringList, problems);

    Device *d = device(qPrintable(dev));
    const QMap<QString, QString> before = linksOf(d);
    DeviceEdit::State s = DeviceEdit::current(d);
    s.links[role] = io;
    QCOMPARE(problemTexts(DeviceEdit::validate(d, s)), problems);
    QVERIFY(!DeviceEdit::apply(d, s));
    QCOMPARE(linksOf(d), before);
}

void tst_ReplacedRoles::theOutputsMayGiveWayToTheValve()
{
    Device *th = device("th");
    DeviceEdit::State s = DeviceEdit::current(th);
    s.links["output"] = "relay_spare";
    s.links["output_heat"] = QString();
    s.links["output_cool"] = QString();
    QCOMPARE(problemTexts(DeviceEdit::validate(th, s)), QStringList());
    QCOMPARE(DeviceEdit::missingRequired(th, s), QStringList());
    QVERIFY(DeviceEdit::apply(th, s));
    QCOMPARE(linksOf(th)["output"], QString("relay_spare"));
    QVERIFY(!linksOf(th).contains("output_heat"));
}

void tst_ReplacedRoles::bothLinksHeldByTheFileAreNotRefused()
{
    QVERIFY(loadIo(work("both.xml"), QByteArray(kIo).replace(
        "<calaos:link name=\"output\" io=\"valve_salon\"/>",
        "<calaos:link name=\"output\" io=\"valve_salon\"/><calaos:link name=\"output_heat\" io=\"relay_spare\"/>")));
    Device *zone = device("zone");
    DeviceEdit::State s = DeviceEdit::current(zone);
    s.params["hysteresis"] = "0.5";
    QCOMPARE(problemTexts(DeviceEdit::validate(zone, s)), QStringList());

    //Touching either link is a change: it has to be right.
    DeviceEdit::State moved = DeviceEdit::current(zone);
    moved.links["output_heat"] = "relay_cool";
    moved.links["output"] = QString();
    QCOMPARE(problemTexts(DeviceEdit::validate(zone, moved)),
             QStringList({ "output_heat: Heating output: \"Cooling relay\" is already an output of thermostat \"Salon heat pump\"." }));
    moved = DeviceEdit::current(zone);
    moved.links["output"] = "relay_cool";
    QCOMPARE(problemTexts(DeviceEdit::validate(zone, moved)).size(), 2);
}

void tst_ReplacedRoles::theValveBelongsToOneThermostat()
{
    QVERIFY(DeviceTemplates::isThermostatOutput("output"));
    QCOMPARE(DeviceTemplates::thermostatOwning("valve_salon"), device("zone"));
    QCOMPARE(DeviceTemplates::thermostatOwning("valve_salon", device("zone")), nullptr);

    //The valve of a zone is no heating output of another thermostat, nor the reverse.
    DeviceEdit::State s = DeviceEdit::current(device("th"));
    s.links["output_heat"] = "valve_salon";
    QCOMPARE(problemTexts(DeviceEdit::validate(device("th"), s)),
             QStringList({ "output_heat: Heating output: \"Salon valve\" is already an output of thermostat \"Salon floor\"." }));

    s = DeviceEdit::current(device("zone"));
    s.links["output"] = "relay_heat";
    QCOMPARE(problemTexts(DeviceEdit::validate(device("zone"), s)),
             QStringList({ "output: Zone valve: \"Heating relay\" is already an output of thermostat \"Salon heat pump\"." }));
}

void tst_ReplacedRoles::theDialogMarksAndRefusesFromReplaces()
{
    DialogEditDevice d(device("zone"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QLabel *warning = d.findChild<QLabel *>("labelWarning");
    auto labelOf = [&](const QString &role)
    {
        QWidget *editor = d.findChild<QWidget *>("link_" + role);
        QFormLayout *form = qobject_cast<QFormLayout *>(editor->parentWidget()->parentWidget()->layout());
        return qobject_cast<QLabel *>(form->labelForField(editor->parentWidget()))->text();
    };
    QCOMPARE(labelOf("output_heat"), QString("Heating output:"));
    QVERIFY(warning->isHidden());

    choose(d.findChild<QComboBox *>("link_output"), QString());
    QCOMPARE(labelOf("output_heat"), QString("Heating output (required):"));
    QVERIFY(!warning->isHidden());

    choose(d.findChild<QComboBox *>("link_output"), "valve_salon");
    choose(d.findChild<QComboBox *>("link_output_cool"), "relay_spare");
    QTest::mouseClick(d.findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok), Qt::LeftButton);
    QVERIFY(d.result() != QDialog::Accepted);
    QLabel *error = d.findChild<QLabel *>("labelError");
    QVERIFY(!error->isHidden());
    QCOMPARE(error->text(), QString("Zone valve and Cooling output cannot both be linked: Zone valve replaces Cooling output."));
    QVERIFY(!linksOf(device("zone")).contains("output_cool"));
}

void tst_ReplacedRoles::commandRequirementsAreMetByTheValve()
{
    const QJsonObject doc = thermostatDoc();
    DeviceCommand::Instance zone = DeviceCommand::Instance::of(device("zone"));
    QVERIFY(zone.writable.contains("output"));
    zone.params["modes"] = "off,heat,cool,auto";
    QCOMPARE(modeOf(DeviceCommand::offer(doc, zone)).values, QStringList({ "off", "heat", "cool", "auto" }));

    //A valve the server cannot write runs nothing.
    zone.writable.remove("output");
    const DeviceCommand::Field unwritable = modeOf(DeviceCommand::offer(doc, zone));
    QCOMPARE(unwritable.values, QStringList({ "off" }));
    QCOMPARE(unwritable.excludedByLinks.value("heat"), QStringList({ "output" }));
    QCOMPARE(unwritable.excludedByLinks.value("auto"), QStringList({ "output", "output" }));

    zone.links.remove("output");
    const DeviceCommand::Field none = modeOf(DeviceCommand::offer(doc, zone));
    QCOMPARE(none.values, QStringList({ "off" }));
    QCOMPARE(none.excludedByLinks.value("cool"), QStringList({ "output_cool" }));

    //Without a valve, the two outputs still decide.
    const DeviceCommand::Instance th = DeviceCommand::Instance::of(device("th"));
    QCOMPARE(modeOf(DeviceCommand::offer(doc, th)).values, QStringList({ "off", "heat", "cool", "auto" }));
}

void tst_ReplacedRoles::theFormOffersTheModesOfTheValve()
{
    DeviceCommandForm form;
    form.setDevice(thermostatDoc(), DeviceCommand::Instance::of(device("zone")));
    QComboBox *mode = qobject_cast<QComboBox *>(form.editor("mode"));
    QVERIFY(mode);
    QCOMPARE(items(mode), QStringList({ "off", "heat", "cool" }));
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    tst_ReplacedRoles tc;
    QTEST_SET_MAIN_SOURCE_PATH
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_replacedroles.moc"
