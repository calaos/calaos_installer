#include <QtTest>
#include <QApplication>
#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QJsonDocument>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTreeWidgetItem>
#include <memory>

#include "DeviceCommand.h"
#include "DeviceCommandForm.h"
#include "FormActionStd.h"
#include "IODoc.h"
#include "projectmanager.h"
#include <Device.h>
#include <ListeRoom.h>
#include <ListeRule.h>

using namespace Calaos;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

Device *device(const char *id)
{
    return ListeRoom::Instance().get_device(id);
}

QJsonObject docOf(Device *d)
{
    return IODoc::getDeviceObject(u(d->impl()));
}

DeviceCommand::Offer offerOf(const char *id)
{
    Device *d = device(id);
    return DeviceCommand::offer(docOf(d), DeviceCommand::Instance::of(d));
}

QStringList names(const DeviceCommand::Offer &o)
{
    QStringList l;
    for (const DeviceCommand::Field &f : o.fields)
        l << f.name;
    return l;
}

DeviceCommand::Field field(const DeviceCommand::Offer &o, const QString &name)
{
    for (const DeviceCommand::Field &f : o.fields)
    {
        if (f.name == name)
            return f;
    }
    return DeviceCommand::Field();
}

// The form of one device, filled as FormActionStd does.
void fill(DeviceCommandForm &form, const char *id)
{
    Device *d = device(id);
    form.setDevice(docOf(d), DeviceCommand::Instance::of(d));
}

QComboBox *combo(DeviceCommandForm &f, const char *name)
{
    return qobject_cast<QComboBox *>(f.editor(name));
}

QDoubleSpinBox *spin(DeviceCommandForm &f, const char *name)
{
    return qobject_cast<QDoubleSpinBox *>(f.editor(name));
}

void pick(QComboBox *c, const QString &value)
{
    QVERIFY(c);
    const int i = c->findData(value);
    QVERIFY2(i >= 0, qPrintable(value));
    c->setCurrentIndex(i);
}

QJsonObject json(const QString &s)
{
    return QJsonDocument::fromJson(s.toUtf8()).object();
}

struct ActionOn
{
    Params rp;
    std::unique_ptr<Rule> rule;
    std::unique_ptr<Action> action;
    QTreeWidgetItem item;

    ActionOn(const char *ioId, const QString &value, bool dynamic = false)
    {
        rp.Add("name", "rule");
        rp.Add("type", "std");
        rule.reset(new Rule(rp));
        action.reset(new Action(ACTION_STD));
        IOBase *io = ListeRoom::Instance().get_output(ioId);
        action->Add(io);
        if (dynamic)
            action->get_params_var().Add(io->get_param("id"), "other");
        else if (!value.isNull())
            action->get_params().Add(io->get_param("id"), value.toUtf8().constData());
    }

    QString stored() const { return u(action->get_params().get_param(action->get_output(0)->get_param("id"))); }
};

}

class tst_DeviceCommand: public QObject
{
    Q_OBJECT

    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();

    // Generation from IODoc
    void proxyFieldsFollowTheDocumentedOrder();
    void thermostatFieldsAreGeneratedFromTheDoc();
    void anEnumIsAListAndANumberHasBoundsStepAndUnit();
    void aFieldWithoutItsLinkIsHidden();
    void aLinkWithoutIoIsNotALink();
    void aLinkThatCannotBeWrittenIsNotOffered();
    void aCommandIoMustBeAnOutputToo();
    void theBoundsComeFromTheIoThatIsWritten();
    void anEmptyModesParamOffersNoMode();
    void aProxyWithoutModeLinkOffersNoMode();
    void aRangeNeedsBothBounds();
    void enumValuesAreFilteredByValueRequiresLinks();
    void theModesParamNarrowsTheModesOfAProxyOnly();
    void aValuesParamComesFromTheInstance();
    void theLinkedIoSetsTheBoundsAndTheUnit();
    void invertedBoundsOfTheLinkedIoAreIgnored();
    void aDeviceWithoutJsonCommandOffersNothing();
    void textKindsAreTold();
    void aTextWithALeadingSpaceIsNotAnObject();

    // JSON
    void theJsonIsCompactWithRealNumbers_data();
    void theJsonIsCompactWithRealNumbers();
    void theFieldsKeepTheirDeclarationOrder();

    // Form
    void nothingIsSentUntilAFieldIsChecked();
    void editingAFieldChecksIt();
    void onlyTheCheckedFieldsAreSent();
    void uncheckingAFieldRemovesIt();
    void uncheckingALoadedFieldRemovesIt();
    void aNumberFieldIsAnUnitSpinWithBoundsAndStep();
    void aStoredCommandFillsTheForm();
    void aStoredCommandComesBackUntouched_data();
    void aStoredCommandComesBackUntouched();
    void keysTheFormDoesNotOfferAreKept();
    void aValueOutsideTheOfferedListIsKeptAndFlagged();
    void aNumberOutsideTheBoundsIsKeptAndFlagged();
    void aNumberKeepsItsDecimals();
    void aNumberKeepsMoreThanSixDecimals();
    void choosingTheDisplayedValueChecksTheRow();
    void aValueOutsideTheListSaysWhy();
    void aRangeCloserThanMinRangeIsFlagged();
    void editingAFlaggedFieldClearsTheFlag();
    void uncheckingAFlaggedFieldDropsIt();
    void aPlainModeFillsTheModeField();
    void aDeviceWithoutLinksSaysSo();
    void hiddenFieldsAreCounted();

    // FormActionStd
    void aDeviceActionShowsTheForm();
    void anOrdinaryIoKeepsTheTextField();
    void showingTheFormWritesNothing();
    void editingTheFormStoresTheCompactJson();
    void aPlainModeOrUnknownTextStaysInTheTextField_data();
    void aPlainModeOrUnknownTextStaysInTheTextField();
    void theFormCanBeOpenedOnAPlainMode();
    void theTextCanBeEditedAsBefore();
    void aDynamicValueStaysInTheTextField();
    void aDynamicValueChosenInTheTextDisablesTheForm();
    void aTextWithALeadingSpaceStaysInTheTextField();
    void thePopupIsResetBetweenActions();
    void aDeviceWithoutJsonDocKeepsTheTextField();

    void anOrdinaryIoHasTheSameSizeWhateverCameBefore();
    void theTextModeOfADeviceHasTheSizeOfAnOrdinaryIo();
    void theTextModeShrinksBackAfterTheForm();
    void theFormGrowsAgainAfterTheText();
    void aNoticeThatAppearsGrowsThePopup();

    void captureTheForms();
    void captureThePopupSizes();
};

void tst_DeviceCommand::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_devicecommand");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());
}

void tst_DeviceCommand::init()
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();
    QString path = m_workDir.filePath("io.xml");
    QFile f(path);
    QVERIFY(f.open(QIODevice::WriteOnly));
    QFile src(QFINDTESTDATA("project.xml"));
    QVERIFY(src.open(QIODevice::ReadOnly));
    f.write(src.readAll());
    f.close();
    QVERIFY(ProjectManager::loadIOsFromFile(path));
}

void tst_DeviceCommand::cleanupTestCase()
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();
}

void tst_DeviceCommand::proxyFieldsFollowTheDocumentedOrder()
{
    const DeviceCommand::Offer o = offerOf("ac");
    QCOMPARE(names(o), QStringList({"mode", "target_temp", "fan_mode"}));
    QCOMPARE(o.hidden, QStringList({"target_temp_low", "target_temp_high", "swing_mode"}));
}

void tst_DeviceCommand::thermostatFieldsAreGeneratedFromTheDoc()
{
    const DeviceCommand::Offer o = offerOf("floor");
    QCOMPARE(names(o), QStringList({"mode", "target_temp", "target_temp_low", "target_temp_high"}));
    QVERIFY(o.hidden.isEmpty());
    QCOMPARE(field(o, "mode").values, QStringList({"off", "heat", "cool", "auto"}));
}

void tst_DeviceCommand::anEnumIsAListAndANumberHasBoundsStepAndUnit()
{
    const DeviceCommand::Offer o = offerOf("floor");

    const DeviceCommand::Field mode = field(o, "mode");
    QCOMPARE(mode.type, DeviceCommand::Enum);

    const DeviceCommand::Field t = field(o, "target_temp");
    QCOMPARE(t.type, DeviceCommand::Number);
    QCOMPARE(t.min, 7.0);
    QCOMPARE(t.max, 35.0);
    QCOMPARE(t.step, 0.5);
    QCOMPARE(t.decimals, 1);
    QCOMPARE(t.unit, QString::fromUtf8("°C"));
}

void tst_DeviceCommand::aFieldWithoutItsLinkIsHidden()
{
    const DeviceCommand::Offer o = offerOf("bare");
    QCOMPARE(names(o), QStringList({"mode"}));
    QCOMPARE(o.hidden, QStringList({"target_temp", "target_temp_low", "target_temp_high", "fan_mode", "swing_mode"}));
}

void tst_DeviceCommand::aLinkWithoutIoIsNotALink()
{
    for (DeviceLink &l : device("ac")->links())
    {
        if (l.name == "mode")
            l.io.clear();
    }
    const DeviceCommand::Offer o = offerOf("ac");
    QVERIFY(!names(o).contains("mode"));
    QVERIFY(o.hidden.contains("mode"));
}

void tst_DeviceCommand::aLinkThatCannotBeWrittenIsNotOffered()
{
    // target_temp has no command_io and its io is an input; fan_mode has a
    // command_io that is an input: the server refuses both.
    const DeviceCommand::Offer o = offerOf("ro");
    QCOMPARE(names(o), QStringList({"mode"}));
    QVERIFY(o.hidden.contains("target_temp"));
    QVERIFY(o.hidden.contains("fan_mode"));

    DeviceCommandForm form;
    fill(form, "ro");
    QVERIFY(form.notice()->isVisibleTo(&form));
    QVERIFY(form.notice()->toolTip().contains("not writable"));
    QVERIFY(form.notice()->toolTip().contains("Target temperature"));
}

void tst_DeviceCommand::aCommandIoMustBeAnOutputToo()
{
    // an input io is fine when its command_io is an output (ac: fan_mode)
    QVERIFY(names(offerOf("ac")).contains("fan_mode"));
    QVERIFY(names(offerOf("cmdb")).contains("target_temp"));
}

void tst_DeviceCommand::theBoundsComeFromTheIoThatIsWritten()
{
    // io says 1 to 2 by 0.1 in K, the command_io 10 to 20 by 1 in C
    const DeviceCommand::Field t = field(offerOf("cmdb"), "target_temp");
    QCOMPARE(t.min, 10.0);
    QCOMPARE(t.max, 20.0);
    QCOMPARE(t.step, 1.0);
    QCOMPARE(t.decimals, 0);
    QCOMPARE(t.unit, QString("C"));
}

void tst_DeviceCommand::anEmptyModesParamOffersNoMode()
{
    // The server offers the modes the param lists: none when it lists none.
    Device *d = device("narrow");
    for (auto &p : d->deviceParams())
    {
        if (p.first == "modes")
            p.second = "";
    }
    const DeviceCommand::Offer o = offerOf("narrow");
    QVERIFY(!names(o).contains("mode"));
    QVERIFY(o.hidden.contains("mode"));
}

void tst_DeviceCommand::aProxyWithoutModeLinkOffersNoMode()
{
    const DeviceCommand::Offer o = offerOf("nomode");
    QCOMPARE(names(o), QStringList({"target_temp"}));
    QVERIFY(o.hidden.contains("mode"));
}

void tst_DeviceCommand::aRangeNeedsBothBounds()
{
    const DeviceCommand::Offer o = offerOf("half");
    QVERIFY(!names(o).contains("target_temp_low"));
    QVERIFY(!names(o).contains("target_temp_high"));
    QVERIFY(o.hidden.contains("target_temp_low"));
    QVERIFY(o.hidden.contains("target_temp_high"));
    QVERIFY(names(o).contains("target_temp"));
}

void tst_DeviceCommand::enumValuesAreFilteredByValueRequiresLinks()
{
    QCOMPARE(field(offerOf("heat"), "mode").values, QStringList({"off", "heat"}));
    QCOMPARE(field(offerOf("floor"), "mode").values, QStringList({"off", "heat", "cool", "auto"}));
    QCOMPARE(field(offerOf("half"), "mode").values, QStringList({"off", "heat", "cool", "auto"}));
    // auto needs both outputs, heat needs its own
    QCOMPARE(field(offerOf("cool_only"), "mode").values, QStringList({"off", "cool"}));
}

void tst_DeviceCommand::theModesParamNarrowsTheModesOfAProxyOnly()
{
    QCOMPARE(field(offerOf("bare"), "mode").values,
             QStringList({"off", "heat", "cool", "auto", "dry", "fan_only"}));
    QCOMPARE(field(offerOf("narrow"), "mode").values, QStringList({"off", "heat", "cool"}));

    // The thermostat does not document modes: a stray param does not narrow it.
    Device *d = device("floor");
    d->deviceParams().push_back({"modes", "off"});
    QCOMPARE(field(offerOf("floor"), "mode").values, QStringList({"off", "heat", "cool", "auto"}));
}

void tst_DeviceCommand::aValuesParamComesFromTheInstance()
{
    // "auto, low ,high," : trimmed, no empty item
    QCOMPARE(field(offerOf("ac"), "fan_mode").values, QStringList({"auto", "low", "high"}));

    // The link exists but the appliance accepts no fan mode: nothing to choose from.
    const DeviceCommand::Offer o = offerOf("nofan");
    QVERIFY(!names(o).contains("fan_mode"));
    QVERIFY(o.hidden.contains("fan_mode"));
}

void tst_DeviceCommand::theLinkedIoSetsTheBoundsAndTheUnit()
{
    const DeviceCommand::Field t = field(offerOf("ac"), "target_temp");
    QCOMPARE(t.min, 16.0);
    QCOMPARE(t.max, 30.0);
    QCOMPARE(t.step, 0.25);
    QCOMPARE(t.decimals, 2);
    QCOMPARE(t.unit, QString::fromUtf8("°C")); // not set on the IO: the documented one
}

void tst_DeviceCommand::invertedBoundsOfTheLinkedIoAreIgnored()
{
    const DeviceCommand::Field t = field(offerOf("nomode"), "target_temp");
    QCOMPARE(t.min, 7.0);
    QCOMPARE(t.max, 35.0);
    QCOMPARE(t.unit, QString("F"));
}

void tst_DeviceCommand::aDeviceWithoutJsonCommandOffersNothing()
{
    Device *d = device("mystery");
    QVERIFY(docOf(d).isEmpty());
    QVERIFY(!DeviceCommand::isJson(docOf(d)));
    QVERIFY(DeviceCommand::offer(docOf(d), DeviceCommand::Instance::of(d)).fields.isEmpty());

    QJsonObject doc = docOf(device("ac"));
    doc["command_format"] = "text";
    QVERIFY(!DeviceCommand::isJson(doc));
    QVERIFY(DeviceCommand::offer(doc, DeviceCommand::Instance::of(device("ac"))).fields.isEmpty());

    // an ordinary IO type is not a device
    QVERIFY(!DeviceCommand::isJson(IODoc::getIOObject("WODigital")));
}

void tst_DeviceCommand::textKindsAreTold()
{
    const QJsonObject doc = docOf(device("ac"));
    QCOMPARE(DeviceCommand::classify("", doc), DeviceCommand::Empty);
    QCOMPARE(DeviceCommand::classify("{}", doc), DeviceCommand::Object);
    QCOMPARE(DeviceCommand::classify("{\"mode\":\"off\"}", doc), DeviceCommand::Object);
    QCOMPARE(DeviceCommand::classify("off", doc), DeviceCommand::PlainMode);
    QCOMPARE(DeviceCommand::classify("fan_only", doc), DeviceCommand::PlainMode);
    QCOMPARE(DeviceCommand::classify("sleep", doc), DeviceCommand::Other);
    QCOMPARE(DeviceCommand::classify("{\"mode\":", doc), DeviceCommand::Other);
    QCOMPARE(DeviceCommand::classify("[\"off\"]", doc), DeviceCommand::Other);
    QCOMPARE(DeviceCommand::classify("\"off\"", doc), DeviceCommand::Other);
    QCOMPARE(DeviceCommand::classify("12", doc), DeviceCommand::Other);
}

void tst_DeviceCommand::aTextWithALeadingSpaceIsNotAnObject()
{
    // The server takes a JSON command only when the text starts with {.
    const QJsonObject doc = docOf(device("ac"));
    QCOMPARE(DeviceCommand::classify(" {\"mode\":\"off\"}", doc), DeviceCommand::Other);
    QCOMPARE(DeviceCommand::classify("\n{}", doc), DeviceCommand::Other);
    QCOMPARE(DeviceCommand::classify(" off", doc), DeviceCommand::Other);
    QCOMPARE(DeviceCommand::classify("  ", doc), DeviceCommand::Other);
    QCOMPARE(DeviceCommand::classify("{\"mode\":\"off\"} ", doc), DeviceCommand::Object);

    QJsonObject o;
    QVERIFY(!DeviceCommand::parse(" {}", o));
}

void tst_DeviceCommand::theJsonIsCompactWithRealNumbers_data()
{
    QTest::addColumn<QString>("mode");
    QTest::addColumn<double>("target");
    QTest::addColumn<QString>("expected");

    QTest::newRow("fraction") << "heat" << 21.5 << "{\"mode\":\"heat\",\"target_temp\":21.5}";
    QTest::newRow("whole") << "cool" << 22.0 << "{\"mode\":\"cool\",\"target_temp\":22}";
    QTest::newRow("tenths") << "auto" << 19.3 << "{\"mode\":\"auto\",\"target_temp\":19.3}";
}

void tst_DeviceCommand::theJsonIsCompactWithRealNumbers()
{
    QFETCH(QString, mode);
    QFETCH(double, target);
    QFETCH(QString, expected);

    const DeviceCommand::Offer o = offerOf("floor");
    QHash<QString, QJsonValue> values;
    values.insert("mode", mode);
    values.insert("target_temp", target);

    const QString s = DeviceCommand::compose(o.fields, values, QJsonObject());
    QCOMPARE(s, expected);
    QVERIFY(json(s)["target_temp"].isDouble());
    QVERIFY(!s.contains(' '));
}

void tst_DeviceCommand::theFieldsKeepTheirDeclarationOrder()
{
    const DeviceCommand::Offer o = offerOf("ac");
    QHash<QString, QJsonValue> values;
    values.insert("fan_mode", "low");
    values.insert("target_temp", 20.5);
    values.insert("mode", "heat");
    QCOMPARE(DeviceCommand::compose(o.fields, values, QJsonObject()),
             QString("{\"mode\":\"heat\",\"target_temp\":20.5,\"fan_mode\":\"low\"}"));
    QCOMPARE(DeviceCommand::compose(o.fields, QHash<QString, QJsonValue>(), QJsonObject()), QString());
}

void tst_DeviceCommand::nothingIsSentUntilAFieldIsChecked()
{
    DeviceCommandForm form;
    fill(form, "floor");
    QCOMPARE(form.fieldCount(), 4);
    QCOMPARE(form.value(), QString());
    for (const char *f : {"mode", "target_temp", "target_temp_low", "target_temp_high"})
        QVERIFY(!form.checkBox(f)->isChecked());
}

void tst_DeviceCommand::editingAFieldChecksIt()
{
    DeviceCommandForm form;
    fill(form, "floor");
    QSignalSpy spy(&form, &DeviceCommandForm::valueChanged);

    pick(combo(form, "mode"), "heat");
    QVERIFY(form.checkBox("mode")->isChecked());
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toString(), QString("{\"mode\":\"heat\"}"));

    spin(form, "target_temp")->setValue(21.5);
    QVERIFY(form.checkBox("target_temp")->isChecked());
    QCOMPARE(spy.last().at(0).toString(), QString("{\"mode\":\"heat\",\"target_temp\":21.5}"));
}

void tst_DeviceCommand::onlyTheCheckedFieldsAreSent()
{
    DeviceCommandForm form;
    fill(form, "floor");
    form.checkBox("target_temp_low")->setChecked(true);
    const double low = spin(form, "target_temp_low")->value();
    QCOMPARE(json(form.value()).keys(), QStringList({"target_temp_low"}));
    QCOMPARE(json(form.value())["target_temp_low"].toDouble(), low);
}

void tst_DeviceCommand::uncheckingAFieldRemovesIt()
{
    DeviceCommandForm form;
    fill(form, "floor");
    pick(combo(form, "mode"), "cool");
    spin(form, "target_temp")->setValue(24);

    QSignalSpy spy(&form, &DeviceCommandForm::valueChanged);
    form.checkBox("target_temp")->setChecked(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toString(), QString("{\"mode\":\"cool\"}"));
    form.checkBox("mode")->setChecked(false);
    QCOMPARE(spy.last().at(0).toString(), QString());
}

void tst_DeviceCommand::uncheckingALoadedFieldRemovesIt()
{
    DeviceCommandForm form;
    fill(form, "floor");
    form.setValue("{\"mode\":\"heat\",\"target_temp\":21}");
    form.checkBox("target_temp")->setChecked(false);
    QCOMPARE(form.value(), QString("{\"mode\":\"heat\"}"));
}

void tst_DeviceCommand::aNumberFieldIsAnUnitSpinWithBoundsAndStep()
{
    DeviceCommandForm form;
    fill(form, "ac");
    QDoubleSpinBox *s = spin(form, "target_temp");
    QVERIFY(s);
    QCOMPARE(s->minimum(), 16.0);
    QCOMPARE(s->maximum(), 30.0);
    QCOMPARE(s->singleStep(), 0.25);
    QCOMPARE(s->decimals(), 2);
    QVERIFY(s->suffix().contains(QString::fromUtf8("°C")));

    QVERIFY(combo(form, "mode"));
    QCOMPARE(combo(form, "mode")->count(), 6);
    QVERIFY(combo(form, "fan_mode"));
}

void tst_DeviceCommand::aStoredCommandFillsTheForm()
{
    DeviceCommandForm form;
    fill(form, "floor");
    QSignalSpy spy(&form, &DeviceCommandForm::valueChanged);
    form.setValue("{\"mode\":\"auto\",\"target_temp_low\":19,\"target_temp_high\":23.5}");

    QCOMPARE(spy.count(), 0);
    QVERIFY(form.checkBox("mode")->isChecked());
    QCOMPARE(combo(form, "mode")->currentData().toString(), QString("auto"));
    QVERIFY(!form.checkBox("target_temp")->isChecked());
    QVERIFY(form.checkBox("target_temp_low")->isChecked());
    QCOMPARE(spin(form, "target_temp_low")->value(), 19.0);
    QCOMPARE(spin(form, "target_temp_high")->value(), 23.5);
    QVERIFY(!form.kept()->isVisibleTo(&form));
}

void tst_DeviceCommand::aStoredCommandComesBackUntouched_data()
{
    QTest::addColumn<QString>("io");
    QTest::addColumn<QString>("stored");

    // "heat" offers no range, "floor" does. Kept keys come back sorted by name.
    QTest::newRow("empty") << "heat" << "";
    QTest::newRow("mode") << "heat" << "{\"mode\":\"heat\"}";
    QTest::newRow("both") << "heat" << "{\"mode\":\"cool\",\"target_temp\":21.5}";
    QTest::newRow("range") << "floor" << "{\"mode\":\"auto\",\"target_temp_low\":18,\"target_temp_high\":24.5}";
    QTest::newRow("unknown key") << "heat" << "{\"mode\":\"heat\",\"boost\":true}";
    QTest::newRow("nested") << "heat" << "{\"mode\":\"off\",\"extra\":{\"a\":[1,2]}}";
    QTest::newRow("hidden field") << "heat" << "{\"mode\":\"heat\",\"target_temp_low\":18}";
    QTest::newRow("wrong type") << "heat" << "{\"mode\":\"heat\",\"target_temp\":\"21\"}";
    QTest::newRow("null") << "heat" << "{\"mode\":\"heat\",\"target_temp\":null}";
}

void tst_DeviceCommand::aStoredCommandComesBackUntouched()
{
    QFETCH(QString, io);
    QFETCH(QString, stored);

    DeviceCommandForm form;
    fill(form, io.toUtf8().constData());
    form.setValue(stored);
    QCOMPARE(form.value(), stored);

    DeviceCommandForm again;
    fill(again, io.toUtf8().constData());
    again.setValue(form.value());
    QCOMPARE(again.value(), form.value());
}

void tst_DeviceCommand::keysTheFormDoesNotOfferAreKept()
{
    DeviceCommandForm form;
    fill(form, "heat");
    form.setValue("{\"mode\":\"heat\",\"boost\":true,\"swing_mode\":\"up\",\"target_temp\":\"hot\"}");

    QVERIFY(form.kept()->isVisibleTo(&form));
    QVERIFY(form.kept()->text().contains("boost"));
    QVERIFY(form.kept()->text().contains("swing_mode"));
    QVERIFY(form.kept()->text().contains("target_temp")); // wrong type: not an editable field

    // an edit keeps them
    pick(combo(form, "mode"), "off");
    const QJsonObject o = json(form.value());
    QCOMPARE(o["mode"].toString(), QString("off"));
    QCOMPARE(o["boost"].toBool(), true);
    QCOMPARE(o["swing_mode"].toString(), QString("up"));
    QCOMPARE(o["target_temp"].toString(), QString("hot"));

    // checking the field takes over the key
    form.checkBox("target_temp")->setChecked(true);
    QVERIFY(json(form.value())["target_temp"].isDouble());
}

void tst_DeviceCommand::aValueOutsideTheOfferedListIsKeptAndFlagged()
{
    DeviceCommandForm form;
    fill(form, "heat"); // no output_cool
    form.setValue("{\"mode\":\"cool\"}");

    QComboBox *c = combo(form, "mode");
    QVERIFY(form.checkBox("mode")->isChecked());
    QCOMPARE(c->currentData().toString(), QString("cool"));
    QCOMPARE(c->count(), 3); // off, heat and the stored one
    QVERIFY(c->styleSheet().contains("#ffe8b3"));
    QVERIFY(!c->toolTip().isEmpty());
    QCOMPARE(form.value(), QString("{\"mode\":\"cool\"}"));

    // loading another command takes the extra item away
    form.setValue("{\"mode\":\"off\"}");
    QCOMPARE(c->count(), 2);
    QVERIFY(!c->styleSheet().contains("#ffe8b3"));
}

void tst_DeviceCommand::aNumberOutsideTheBoundsIsKeptAndFlagged()
{
    DeviceCommandForm form;
    fill(form, "ac"); // 16 to 30
    form.setValue("{\"target_temp\":12}");

    QDoubleSpinBox *s = spin(form, "target_temp");
    QCOMPARE(s->value(), 12.0);
    QVERIFY(s->minimum() <= 12.0);
    QVERIFY(s->styleSheet().contains("#ffe8b3"));
    QCOMPARE(form.value(), QString("{\"target_temp\":12}"));

    form.setValue("{\"target_temp\":99}");
    QCOMPARE(form.value(), QString("{\"target_temp\":99}"));
}

void tst_DeviceCommand::aNumberKeepsItsDecimals()
{
    DeviceCommandForm form;
    fill(form, "ac"); // step 0.25, two decimals
    form.setValue("{\"target_temp\":21.125}");
    QCOMPARE(form.value(), QString("{\"target_temp\":21.125}"));

    // an unrelated edit does not round it
    pick(combo(form, "mode"), "heat");
    QCOMPARE(json(form.value())["target_temp"].toDouble(), 21.125);
}

void tst_DeviceCommand::aNumberKeepsMoreThanSixDecimals()
{
    DeviceCommandForm form;
    fill(form, "ac");
    form.setValue("{\"target_temp\":21.1234567}");
    QCOMPARE(form.value(), QString("{\"target_temp\":21.1234567}"));

    // a number nobody touched comes back as it was, whatever else is edited
    pick(combo(form, "mode"), "heat");
    QCOMPARE(json(form.value())["target_temp"].toDouble(), 21.1234567);

    // once edited, the number is the one of the spin
    spin(form, "target_temp")->setValue(22);
    QCOMPARE(json(form.value())["target_temp"].toDouble(), 22.0);

    // dropped and sent again, it is what the spin shows
    form.setValue("{\"target_temp\":21.1234567}");
    form.checkBox("target_temp")->setChecked(false);
    form.checkBox("target_temp")->setChecked(true);
    QCOMPARE(json(form.value())["target_temp"].toDouble(), spin(form, "target_temp")->value());
    QVERIFY(json(form.value())["target_temp"].toDouble() != 21.1234567);
}

void tst_DeviceCommand::choosingTheDisplayedValueChecksTheRow()
{
    DeviceCommandForm form;
    fill(form, "floor");
    QSignalSpy spy(&form, &DeviceCommandForm::valueChanged);

    // The list shows its first item: choosing it again changes nothing for the
    // combo, and still means "send this".
    QVERIFY(!form.checkBox("mode")->isChecked());
    QComboBox *c = combo(form, "mode");
    QMetaObject::invokeMethod(c, "activated", Q_ARG(int, c->currentIndex()));
    QVERIFY(form.checkBox("mode")->isChecked());
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toString(), QString("{\"mode\":\"off\"}"));

    // already sent: nothing more to say
    QMetaObject::invokeMethod(c, "activated", Q_ARG(int, c->currentIndex()));
    QCOMPARE(spy.count(), 1);
}

void tst_DeviceCommand::aValueOutsideTheListSaysWhy()
{
    // a list that comes from a param
    DeviceCommandForm fan;
    fill(fan, "ac");
    fan.setValue("{\"fan_mode\":\"turbo\"}");
    QVERIFY(combo(fan, "fan_mode")->toolTip().contains("fan_modes"));
    QVERIFY(!combo(fan, "fan_mode")->toolTip().contains("link"));

    // a mode the modes param leaves out
    DeviceCommandForm narrow;
    fill(narrow, "narrow");
    narrow.setValue("{\"mode\":\"auto\"}");
    QVERIFY(combo(narrow, "mode")->toolTip().contains("modes"));
    QVERIFY(!combo(narrow, "mode")->toolTip().contains("link"));

    // a mode that needs a link the device lacks
    DeviceCommandForm heat;
    fill(heat, "heat");
    heat.setValue("{\"mode\":\"cool\"}");
    QVERIFY(combo(heat, "mode")->toolTip().contains("output_cool"));
    QVERIFY(combo(heat, "mode")->toolTip().contains("link"));

    // a word the documentation does not know
    heat.setValue("{\"mode\":\"banana\"}");
    QVERIFY(!combo(heat, "mode")->toolTip().contains("link"));
    QVERIFY(!combo(heat, "mode")->toolTip().contains("param"));
    QCOMPARE(heat.value(), QString("{\"mode\":\"banana\"}"));
}

void tst_DeviceCommand::aRangeCloserThanMinRangeIsFlagged()
{
    DeviceCommandForm form; // min_range is 2
    fill(form, "floor");

    form.setValue("{\"target_temp_low\":19,\"target_temp_high\":20}");
    QVERIFY(form.rangeNotice()->isVisibleTo(&form));
    QVERIFY(spin(form, "target_temp_low")->styleSheet().contains("#ffe8b3"));
    QVERIFY(spin(form, "target_temp_high")->styleSheet().contains("#ffe8b3"));
    QCOMPARE(form.value(), QString("{\"target_temp_low\":19,\"target_temp_high\":20}")); // flagged, not changed

    spin(form, "target_temp_high")->setValue(21);
    QVERIFY(!form.rangeNotice()->isVisibleTo(&form));
    QVERIFY(!spin(form, "target_temp_low")->styleSheet().contains("#ffe8b3"));

    // low above high
    spin(form, "target_temp_low")->setValue(25);
    QVERIFY(form.rangeNotice()->isVisibleTo(&form));

    // one bound alone: the other one is not known here
    form.setValue("{\"target_temp_low\":19}");
    QVERIFY(!form.rangeNotice()->isVisibleTo(&form));

    // unchecking one bound clears it
    form.setValue("{\"target_temp_low\":19,\"target_temp_high\":20}");
    form.checkBox("target_temp_high")->setChecked(false);
    QVERIFY(!form.rangeNotice()->isVisibleTo(&form));
    QVERIFY(!spin(form, "target_temp_low")->styleSheet().contains("#ffe8b3"));
}

void tst_DeviceCommand::editingAFlaggedFieldClearsTheFlag()
{
    DeviceCommandForm form;
    fill(form, "ac");
    form.setValue("{\"target_temp\":12}");
    QVERIFY(spin(form, "target_temp")->styleSheet().contains("#ffe8b3"));

    spin(form, "target_temp")->setValue(18);
    QVERIFY(!spin(form, "target_temp")->styleSheet().contains("#ffe8b3"));
    QCOMPARE(form.value(), QString("{\"target_temp\":18}"));
}

void tst_DeviceCommand::uncheckingAFlaggedFieldDropsIt()
{
    DeviceCommandForm form;
    fill(form, "heat");
    form.setValue("{\"mode\":\"cool\"}");
    form.checkBox("mode")->setChecked(false);
    QCOMPARE(form.value(), QString());
    QVERIFY(!combo(form, "mode")->styleSheet().contains("#ffe8b3"));
}

void tst_DeviceCommand::aPlainModeFillsTheModeField()
{
    DeviceCommandForm form;
    fill(form, "heat");
    form.setValue("heat");
    QVERIFY(form.checkBox("mode")->isChecked());
    QCOMPARE(combo(form, "mode")->currentData().toString(), QString("heat"));
    QCOMPARE(form.value(), QString("{\"mode\":\"heat\"}"));
}

void tst_DeviceCommand::aDeviceWithoutLinksSaysSo()
{
    // a thermostat with nothing linked still offers off, so use a bare doc
    DeviceCommandForm form;
    form.setDevice(docOf(device("ac")), DeviceCommand::Instance());
    QCOMPARE(form.fieldCount(), 0);
    QVERIFY(form.notice()->isVisibleTo(&form));
    form.setValue("{\"mode\":\"off\"}");
    QCOMPARE(form.value(), QString("{\"mode\":\"off\"}"));
}

void tst_DeviceCommand::hiddenFieldsAreCounted()
{
    DeviceCommandForm form;
    fill(form, "bare");
    QVERIFY(form.notice()->isVisibleTo(&form));
    QVERIFY(form.notice()->text().contains("5"));

    fill(form, "floor");
    QVERIFY(!form.notice()->isVisibleTo(&form));
}

namespace
{
QLineEdit *textOf(FormActionStd &w) { return w.findChild<QLineEdit *>("editValue"); }
DeviceCommandForm *formOf(FormActionStd &w) { return w.findChild<DeviceCommandForm *>("cmdForm"); }
QPushButton *modeButton(FormActionStd &w) { return w.findChild<QPushButton *>("btMode"); }
}

void tst_DeviceCommand::aDeviceActionShowsTheForm()
{
    ActionOn a("floor", "{\"mode\":\"heat\",\"target_temp\":21.5}");
    FormActionStd w;
    w.setAction(&a.item, a.rule.get(), a.action.get());

    QVERIFY(formOf(w)->isVisibleTo(&w));
    QVERIFY(!textOf(w)->isVisibleTo(&w));
    QCOMPARE(combo(*formOf(w), "mode")->currentData().toString(), QString("heat"));
    QCOMPARE(spin(*formOf(w), "target_temp")->value(), 21.5);
    QVERIFY(modeButton(w)->isVisibleTo(&w));
    QCOMPARE(modeButton(w)->text(), QString("Edit as text"));
    // a dynamic value cannot go inside the JSON
    QVERIFY(!w.findChild<QPushButton *>("btMore")->isVisibleTo(&w));
    QVERIFY(!w.findChild<QPushButton *>("buttonMore")->isVisibleTo(&w));
}

void tst_DeviceCommand::anOrdinaryIoKeepsTheTextField()
{
    ActionOn a("lamp", "toggle");
    FormActionStd w;
    w.setAction(&a.item, a.rule.get(), a.action.get());

    QVERIFY(!formOf(w)->isVisibleTo(&w));
    QVERIFY(textOf(w)->isVisibleTo(&w));
    QVERIFY(!modeButton(w)->isVisibleTo(&w));
    QCOMPARE(textOf(w)->text(), QString("toggle"));
}

void tst_DeviceCommand::showingTheFormWritesNothing()
{
    // Odd spelling and an unknown key: reading must not rewrite the rule.
    const QString stored = "{ \"target_temp\" : 21.50 , \"boost\":true, \"mode\":\"heat\" }";
    ActionOn a("floor", stored);
    FormActionStd w;
    int changes = 0;
    connect(&w, &FormActionStd::actionChanged, [&]() { changes++; });
    w.setAction(&a.item, a.rule.get(), a.action.get());

    QCOMPARE(changes, 0);
    QCOMPARE(a.stored(), stored);
}

void tst_DeviceCommand::editingTheFormStoresTheCompactJson()
{
    ActionOn a("floor", QString(""));
    FormActionStd w;
    int changes = 0;
    QTreeWidgetItem *seen = nullptr;
    connect(&w, &FormActionStd::actionChanged, [&](QTreeWidgetItem *item, Action *) { changes++; seen = item; });
    w.setAction(&a.item, a.rule.get(), a.action.get());

    pick(combo(*formOf(w), "mode"), "heat");
    spin(*formOf(w), "target_temp")->setValue(21.5);

    QCOMPARE(a.stored(), QString("{\"mode\":\"heat\",\"target_temp\":21.5}"));
    QCOMPARE(changes, 2);
    QCOMPARE(seen, &a.item);
    QVERIFY(!a.action->get_params_var().Exists("floor"));
    // the text field follows
    QCOMPARE(textOf(w)->text(), a.stored());
}

void tst_DeviceCommand::aPlainModeOrUnknownTextStaysInTheTextField_data()
{
    QTest::addColumn<QString>("stored");
    QTest::addColumn<bool>("canUseTheForm");

    QTest::newRow("plain mode") << "heat" << true;
    QTest::newRow("another word") << "sleep" << false;
    QTest::newRow("broken json") << "{\"mode\":" << false;
    QTest::newRow("json array") << "[1,2]" << false;
    QTest::newRow("json string") << "\"heat\"" << false;
}

void tst_DeviceCommand::aPlainModeOrUnknownTextStaysInTheTextField()
{
    QFETCH(QString, stored);
    QFETCH(bool, canUseTheForm);

    ActionOn a("floor", stored);
    FormActionStd w;
    int changes = 0;
    connect(&w, &FormActionStd::actionChanged, [&]() { changes++; });
    w.setAction(&a.item, a.rule.get(), a.action.get());

    QVERIFY(!formOf(w)->isVisibleTo(&w));
    QVERIFY(textOf(w)->isVisibleTo(&w));
    QCOMPARE(textOf(w)->text(), stored);
    QCOMPARE(a.stored(), stored);
    QCOMPARE(changes, 0);
    QCOMPARE(modeButton(w)->text(), QString("Use the form"));
    QCOMPARE(modeButton(w)->isEnabled(), canUseTheForm);
}

void tst_DeviceCommand::theFormCanBeOpenedOnAPlainMode()
{
    ActionOn a("floor", QString("heat"));
    FormActionStd w;
    int changes = 0;
    connect(&w, &FormActionStd::actionChanged, [&]() { changes++; });
    w.setAction(&a.item, a.rule.get(), a.action.get());

    modeButton(w)->click();
    QVERIFY(formOf(w)->isVisibleTo(&w));
    QCOMPARE(combo(*formOf(w), "mode")->currentData().toString(), QString("heat"));
    QCOMPARE(a.stored(), QString("heat")); // looking is not editing
    QCOMPARE(changes, 0);

    // back to the text: still the plain mode, no loss
    modeButton(w)->click();
    QVERIFY(textOf(w)->isVisibleTo(&w));
    QCOMPARE(textOf(w)->text(), QString("heat"));

    // an edit in the form turns it into the JSON form of the same command
    modeButton(w)->click();
    spin(*formOf(w), "target_temp")->setValue(20);
    QCOMPARE(a.stored(), QString("{\"mode\":\"heat\",\"target_temp\":20}"));
}

void tst_DeviceCommand::theTextCanBeEditedAsBefore()
{
    ActionOn a("floor", QString("off"));
    FormActionStd w;
    w.setAction(&a.item, a.rule.get(), a.action.get());

    textOf(w)->setText("{\"mode\":\"cool\"}");
    QCOMPARE(a.stored(), QString("{\"mode\":\"cool\"}"));

    // the button follows what the text is
    QVERIFY(modeButton(w)->isEnabled());
    textOf(w)->setText("{\"mode\":");
    QVERIFY(!modeButton(w)->isEnabled());
    textOf(w)->setText("{\"mode\":\"heat\",\"target_temp\":20}");
    modeButton(w)->click();
    QCOMPARE(spin(*formOf(w), "target_temp")->value(), 20.0);
}

void tst_DeviceCommand::aDynamicValueStaysInTheTextField()
{
    ActionOn a("floor", QString(), true);
    FormActionStd w;
    w.setAction(&a.item, a.rule.get(), a.action.get());

    QVERIFY(!formOf(w)->isVisibleTo(&w));
    QVERIFY(!modeButton(w)->isEnabled());
    QVERIFY(a.action->get_params_var().Exists("floor"));
}

void tst_DeviceCommand::aDynamicValueChosenInTheTextDisablesTheForm()
{
    ActionOn a("floor", QString("{\"mode\":\"heat\"}"));
    FormActionStd w;
    w.setAction(&a.item, a.rule.get(), a.action.get());

    modeButton(w)->click(); // Edit as text
    QVERIFY(textOf(w)->isVisibleTo(&w));
    QVERIFY(modeButton(w)->isEnabled());

    w.setDynamicValue("floor_heat");
    QVERIFY(a.action->get_params_var().Exists("floor"));
    QVERIFY(!modeButton(w)->isEnabled());

    // the disabled button does nothing: the dynamic value is still there
    modeButton(w)->click();
    QVERIFY(!formOf(w)->isVisibleTo(&w));
    QVERIFY(a.action->get_params_var().Exists("floor"));
}

void tst_DeviceCommand::aTextWithALeadingSpaceStaysInTheTextField()
{
    const QString stored = " {\"mode\":\"heat\"}";
    ActionOn a("floor", stored);
    FormActionStd w;
    w.setAction(&a.item, a.rule.get(), a.action.get());

    QVERIFY(!formOf(w)->isVisibleTo(&w));
    QCOMPARE(textOf(w)->text(), stored);
    QVERIFY(!modeButton(w)->isEnabled());
    QCOMPARE(a.stored(), stored);
}

void tst_DeviceCommand::thePopupIsResetBetweenActions()
{
    ActionOn dev("floor", QString("{\"mode\":\"heat\"}"));
    ActionOn lamp("lamp", QString("toggle"));
    ActionOn other("ac", QString("{\"fan_mode\":\"low\"}"));
    FormActionStd w;

    w.setAction(&dev.item, dev.rule.get(), dev.action.get());
    QVERIFY(formOf(w)->isVisibleTo(&w));

    w.setAction(&lamp.item, lamp.rule.get(), lamp.action.get());
    QVERIFY(!formOf(w)->isVisibleTo(&w));
    QVERIFY(textOf(w)->isVisibleTo(&w));
    QVERIFY(!modeButton(w)->isVisibleTo(&w));

    // the form belongs to the device of the action, not to the previous one
    w.setAction(&other.item, other.rule.get(), other.action.get());
    QVERIFY(formOf(w)->isVisibleTo(&w));
    QVERIFY(formOf(w)->editor("fan_mode"));
    QVERIFY(!formOf(w)->editor("target_temp_low"));
    QCOMPARE(combo(*formOf(w), "fan_mode")->currentData().toString(), QString("low"));
    QVERIFY(!formOf(w)->checkBox("mode")->isChecked());
}

namespace
{

// What FormRules does with the popup before it moves it by its width.
QSize shown(FormActionStd &w, ActionOn &a)
{
    w.setAction(&a.item, a.rule.get(), a.action.get());
    w.layout()->update();
    w.show();
    QCoreApplication::processEvents();
    return w.size();
}

QSize ordinarySize()
{
    ActionOn lamp("lamp", QString("toggle"));
    FormActionStd fresh;
    return shown(fresh, lamp);
}

}

void tst_DeviceCommand::anOrdinaryIoHasTheSameSizeWhateverCameBefore()
{
    const QSize reference = ordinarySize();

    ActionOn dev("ac", QString("{\"mode\":\"heat\",\"target_temp\":21.5}"));
    ActionOn lamp("lamp", QString("toggle"));
    FormActionStd w;
    QVERIFY(shown(w, dev).height() > reference.height());
    QCOMPARE(shown(w, lamp), reference);

    // and an ordinary IO is never taller than the popup was before the form existed
    QVERIFY(reference.height() <= 192);
}

void tst_DeviceCommand::theTextModeOfADeviceHasTheSizeOfAnOrdinaryIo()
{
    const QSize reference = ordinarySize();

    ActionOn plain("floor", QString("heat"));
    FormActionStd w;
    QCOMPARE(shown(w, plain), reference);
    QVERIFY(modeButton(w)->isVisibleTo(&w));
}

void tst_DeviceCommand::theTextModeShrinksBackAfterTheForm()
{
    const QSize reference = ordinarySize();

    ActionOn a("floor", QString("{\"mode\":\"heat\",\"target_temp\":21.5}"));
    FormActionStd w;
    const QSize form = shown(w, a);
    QVERIFY(form.height() > reference.height());

    // the user flips to the text: the popup is what a text popup is
    modeButton(w)->click();
    QVERIFY(textOf(w)->isVisibleTo(&w));
    QCoreApplication::processEvents();
    QVERIFY(w.size().height() < form.height());
    QCOMPARE(w.size(), reference);
}

void tst_DeviceCommand::theFormGrowsAgainAfterTheText()
{
    ActionOn a("floor", QString("{\"mode\":\"heat\",\"target_temp\":21.5}"));
    FormActionStd w;
    const QSize form = shown(w, a);

    modeButton(w)->click();
    QVERIFY(w.height() < form.height());
    modeButton(w)->click();
    QCoreApplication::processEvents();
    QVERIFY(formOf(w)->isVisibleTo(&w));
    QCOMPARE(w.size(), form);
}

void tst_DeviceCommand::aNoticeThatAppearsGrowsThePopup()
{
    ActionOn a("floor", QString(""));
    FormActionStd w;
    const QSize before = shown(w, a);

    // two bounds closer than min_range show a notice under the form
    spin(*formOf(w), "target_temp_low")->setValue(19);
    spin(*formOf(w), "target_temp_high")->setValue(20);
    QCoreApplication::processEvents();
    QVERIFY(w.height() > before.height());
    QVERIFY(w.height() >= w.sizeHint().height());
}

void tst_DeviceCommand::aDeviceWithoutJsonDocKeepsTheTextField()
{
    ActionOn a("mystery", QString("anything"));
    FormActionStd w;
    w.setAction(&a.item, a.rule.get(), a.action.get());

    QVERIFY(textOf(w)->isVisibleTo(&w));
    QVERIFY(!formOf(w)->isVisibleTo(&w));
    QCOMPARE(a.stored(), QString("anything"));
}

// Offscreen captures for the review, when INST5_SHOTS names a folder.
void tst_DeviceCommand::captureTheForms()
{
    const QString dir = qEnvironmentVariable("INST5_SHOTS");
    if (dir.isEmpty())
        QSKIP("INST5_SHOTS not set");

    struct Shot { const char *file; const char *io; QString stored; };
    const QList<Shot> shots = {
        {"proxy-empty", "ac", ""},
        {"proxy-partial", "ac", "{\"mode\":\"heat\",\"target_temp\":21.5}"},
        {"thermostat-heat-cool", "floor", "{\"mode\":\"auto\",\"target_temp_low\":19,\"target_temp_high\":23.5}"},
        {"thermostat-heat-only", "heat", "{\"mode\":\"heat\",\"target_temp\":20}"},
        {"flagged-and-kept", "heat", "{\"mode\":\"cool\",\"target_temp\":40,\"boost\":true}"},
        {"proxy-bare", "bare", "{\"mode\":\"cool\"}"},
        {"text-plain-mode", "floor", "heat"},
        {"ordinary-io", "lamp", "toggle"},
        {"thermostat-range-too-close", "floor", "{\"target_temp_low\":19,\"target_temp_high\":20}"},
        {"proxy-read-only-links", "ro", "{\"mode\":\"heat\"}"},
    };

    for (const Shot &s : shots)
    {
        ActionOn a(s.io, s.stored);
        FormActionStd w;
        w.setAction(&a.item, a.rule.get(), a.action.get());
        w.setWindowFlags(Qt::Widget);
        w.resize(w.sizeHint().expandedTo(QSize(460, 0)));
        w.show();
        QVERIFY(QTest::qWaitForWindowExposed(&w));
        QVERIFY(w.grab().save(QDir(dir).filePath(QString("%1.png").arg(s.file))));
    }
}

// Real size of the popup, as the user sees it, when INST8_SHOTS names a folder.
void tst_DeviceCommand::captureThePopupSizes()
{
    const QString dir = qEnvironmentVariable("INST8_SHOTS");
    if (dir.isEmpty())
        QSKIP("INST8_SHOTS not set");

    ActionOn form("floor", QString("{\"mode\":\"auto\",\"target_temp_low\":19,\"target_temp_high\":23.5}"));
    ActionOn plain("floor", QString("heat"));
    ActionOn lamp("lamp", QString("toggle"));
    FormActionStd w;

    auto snap = [&](const char *name)
    {
        QCoreApplication::processEvents();
        QVERIFY(w.grab().save(QDir(dir).filePath(QString("%1.png").arg(name))));
        qInfo() << name << w.size();
    };

    shown(w, lamp);
    snap("1-ordinary-fresh");
    shown(w, form);
    snap("2-form");
    modeButton(w)->click();
    snap("3-text-after-the-form");
    shown(w, lamp);
    snap("4-ordinary-after-the-form");
    shown(w, plain);
    snap("5-plain-mode-after-the-form");
}

QTEST_MAIN(tst_DeviceCommand)
#include "tst_devicecommand.moc"
