#include <QtTest>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTextBrowser>
#include <QTimer>
#include <QTranslator>

#include "../tst_devicetemplates/fixture.h"
#include "ConfigOptions.h"
#include "DialogNewDevice.h"
#include "DialogDeviceInfo.h"
#include "DialogNewRemoteUIRelay.h"
#include "ConfigOptions.h"
#include "WidgetEsphomeDiscovery.h"
#include "../tst_esphomediscovery/FakeMqttBroker.h"

#include <QTcpServer>
#include <QTreeWidget>

using namespace Calaos;
using namespace Fixture;

namespace
{

template<typename T>
T *field(QDialog *d, const QString &name)
{
    return d->findChild<T *>("field_" + name);
}

void choose(QComboBox *box, const QString &data)
{
    const int i = box->findData(data);
    QVERIFY2(i >= 0, qPrintable(data));
    box->setCurrentIndex(i);
}

QStringList itemData(QComboBox *box)
{
    QStringList l;
    for (int i = 0;i < box->count();i++)
        l << box->itemData(i).toString();
    return l;
}

QStringList itemTexts(QComboBox *box)
{
    QStringList l;
    for (int i = 0;i < box->count();i++)
        l << box->itemText(i);
    return l;
}

//On its box: a check box stretched by the form does not take a click on its blank part.
void tick(QCheckBox *cb)
{
    QTest::mouseClick(cb, Qt::LeftButton, Qt::NoModifier, QPoint(6, cb->height() / 2));
}

void clickCreate(QDialog *d)
{
    QPushButton *create = d->findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok);
    QTest::mouseClick(create, Qt::LeftButton);
}

QLabel *errorLabel(QDialog *d)
{
    return d->findChild<QLabel *>("labelError");
}

}

class tst_DialogNewDevice: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;
    QTimer m_dialogKiller;
    QStringList m_dialogs;
    QString m_shots;

    QString work(const QString &f) const { return m_workDir.filePath(f); }
    void shoot(QWidget *w, const QString &name);

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void fillingTheDialogCreatesTheDevice();
    void createIsRefusedUntilTheFieldsAreValid();
    void aRequiredMultiNeedsOneBoxChecked();
    void ioFieldsOfferTheIosThatFit();
    void fieldsNameTheLinkRoleAndTheParamTheyFeed();
    void anIoFieldDoesNotOfferWhatItsRoleRefuses();
    void editorsFollowTheFieldTypes();
    void aSmallStepGetsItsDecimals();
    void switchingTemplatesKeepsTheCommonValues();
    void switchingTemplatesKeepsOnlyWhatTheUserChanged();
    void theNotesOfTheTemplateAreShownAfterCreation();
    void noTemplateNoCreate();
    void theDiscoveryIsOfferedForEsphomeTemplates();
    void theBrokerOfTheProjectFillsTheForm();
    void aLoopbackBrokerIsSearchedThroughTheBox();
    void searchingListsTheClimatesAndChoosingOneFillsTheForm();
    void anUnreachableBrokerLeavesTheFormToFillByHand();
    void anOldEntityIsGreyedAtTheBottom();
    void theProjectCredentialsOnlyGoToItsBroker();
    void enterInTheBrokerAddressSearches();
    void screenshots();
};

void tst_DialogNewDevice::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_dialognewdevice");
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

    //Set to a directory to write the dialogs as images.
    m_shots = qEnvironmentVariable("CALAOS_SHOTS_DIR");
}

void tst_DialogNewDevice::init()
{
    m_dialogs.clear();
    QVERIFY(loadIo(work("house.xml"), houseXml()));
}

void tst_DialogNewDevice::cleanup()
{
    QCOMPARE(m_dialogs, QStringList());
    resetModel();
}

void tst_DialogNewDevice::shoot(QWidget *w, const QString &name)
{
    if (m_shots.isEmpty())
        return;
    w->show();
    QApplication::processEvents();
    QVERIFY(w->grab().save(QDir(m_shots).filePath(name + ".png")));
}

void tst_DialogNewDevice::fillingTheDialogCreatesTheDevice()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Cave"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("thermostat_heat_cool"));

    QTest::keyClicks(field<QLineEdit>(&d, "name"), "  Chauffage salon ");
    field<QComboBox>(&d, "room")->setCurrentIndex(0);
    choose(field<QComboBox>(&d, "current_temp"), "temp_cave");
    choose(field<QComboBox>(&d, "output_heat"), "relay_heat");
    choose(field<QComboBox>(&d, "output_cool"), "flag");
    choose(field<QComboBox>(&d, "emitter"), "fan_coil");
    clickCreate(&d);

    QCOMPARE(d.result(), int(QDialog::Accepted));
    Device *dev = d.device();
    QVERIFY(dev);
    QCOMPARE(d.room(), roomNamed("Salon"));
    QCOMPARE(ListeRoom::Instance().get_device("chauffage_salon"), dev);
    QCOMPARE(u(dev->get_param("name")), QString("Chauffage salon"));
    QCOMPARE(int(dev->ownedIos().size()), 4);
    QVERIFY(roomNamed("Salon")->hasOutput(dev));

    QMap<QString, QString> links, params;
    for (const DeviceLink &l: dev->links())
        links[u(l.name)] = u(l.io);
    for (const auto &p: dev->deviceParams())
        params[u(p.first)] = u(p.second);
    QCOMPARE(links["current_temp"], QString("temp_cave"));
    QCOMPARE(links["output_cool"], QString("flag"));
    QCOMPARE(links["target_temp_low"], QString("chauffage_salon_target_temp_low"));
    QCOMPARE(params["emitter"], QString("fan_coil"));
    QCOMPARE(params["min_range"], QString("1"));
    QCOMPARE(params["min_on_time"], QString("120"));

    const QByteArray saved = saveIo(work("dialog.xml"));
    QVERIFY(deviceElement(saved, "chauffage_salon").contains("  link io=relay_heat name=output_heat"));
}

void tst_DialogNewDevice::createIsRefusedUntilTheFieldsAreValid()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("thermostat_heat"));
    QVERIFY(errorLabel(&d)->isHidden());

    clickCreate(&d);
    QVERIFY(d.isVisible());
    QVERIFY(d.result() != QDialog::Accepted);
    QVERIFY(!d.device());
    QVERIFY(!errorLabel(&d)->isHidden());
    QCOMPARE(errorLabel(&d)->text(), QString("Name is required.\nTemperature sensor: choose an IO.\n"
                                             "Heating output: choose an IO."));
    QVERIFY(roomNamed("Salon")->get_devices().empty());
    QCOMPARE(d.focusWidget(), field<QLineEdit>(&d, "name"));

    QTest::keyClicks(field<QLineEdit>(&d, "name"), "Heat");
    choose(field<QComboBox>(&d, "current_temp"), "temp_salon");
    clickCreate(&d);
    QCOMPARE(errorLabel(&d)->text(), QString("Heating output: choose an IO."));
    QCOMPARE(d.focusWidget(), field<QComboBox>(&d, "output_heat"));

    choose(field<QComboBox>(&d, "output_heat"), "relay_heat");
    clickCreate(&d);
    QCOMPARE(d.result(), int(QDialog::Accepted));
    QVERIFY(d.device());
}

void tst_DialogNewDevice::aRequiredMultiNeedsOneBoxChecked()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("esphome_climate"));
    //A form built while the dialog is shown is shown too.
    QVERIFY(field<QLineEdit>(&d, "topic_prefix")->isVisible());
    QTest::keyClicks(field<QLineEdit>(&d, "name"), "Clim");
    QTest::keyClicks(field<QLineEdit>(&d, "topic_prefix"), "clim");
    QTest::keyClicks(field<QLineEdit>(&d, "object_id"), "clim");

    const QList<QCheckBox *> modes = d.findChildren<QCheckBox *>(QRegularExpression("^field_modes_"));
    QCOMPARE(modes.size(), 6);
    for (QCheckBox *cb: modes)
    {
        QVERIFY(cb->isChecked());
        cb->setChecked(false);
    }
    QCOMPARE(d.findChild<QCheckBox *>("field_modes_fan_only")->text(), QString("Fan only"));

    clickCreate(&d);
    QVERIFY(!d.device());
    QCOMPARE(errorLabel(&d)->text(), QString("Modes: choose one at least."));
    QCOMPARE(d.focusWidget(), d.findChild<QCheckBox *>("field_modes_off"));

    //Picked in any order, written in the template's.
    tick(d.findChild<QCheckBox *>("field_modes_cool"));
    tick(d.findChild<QCheckBox *>("field_modes_heat"));
    QCOMPARE(d.values()["modes"].toStringList(), QStringList({ "heat", "cool" }));
    QVERIFY(d.findChild<QCheckBox *>("field_fan_modes_low"));
    clickCreate(&d);
    QVERIFY(d.device());
    QCOMPARE(u(d.device()->deviceParams()[0].second), QString("heat,cool"));
}

void tst_DialogNewDevice::ioFieldsOfferTheIosThatFit()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    QVERIFY(d.selectTemplate("thermostat_heat"));

    QComboBox *out = field<QComboBox>(&d, "output_heat");
    QCOMPARE(itemData(out), QStringList({ "", "flag", "relay_heat", "relay_cool" }));
    QCOMPARE(itemTexts(out), QStringList({ "Choose an IO", "Flag (flag) - Salon", "Heating relay (relay_heat) - Salon",
                                           "Cooling relay (relay_cool) - Salon" }));
    QCOMPARE(itemData(field<QComboBox>(&d, "current_temp")),
             QStringList({ "", "temp_salon", "hum_salon", "temp_cave", "other_temp" }));

    QVERIFY(d.selectTemplate("esphome_climate"));
    QComboBox *source = field<QComboBox>(&d, "remote_temp_source");
    QCOMPARE(source->itemText(0), QString("None"));
    QCOMPARE(source->currentIndex(), 0);
}

void tst_DialogNewDevice::fieldsNameTheLinkRoleAndTheParamTheyFeed()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    QVERIFY(d.selectTemplate("thermostat_heat"));
    auto hint = [&](const QString &name)
    {
        QLabel *l = d.findChild<QLabel *>("hint_" + name);
        return l ? l->text() : QString();
    };
    QVERIFY2(hint("current_temp").contains("Link: Current temperature (current_temp)"), qPrintable(hint("current_temp")));
    QVERIFY2(hint("output_heat").contains("Link: Heating output (output_heat)"), qPrintable(hint("output_heat")));
    QVERIFY2(hint("emitter").contains("Parameter: Emitter (emitter)"), qPrintable(hint("emitter")));
    QVERIFY(!hint("name").contains("Link:"));
}

void tst_DialogNewDevice::anIoFieldDoesNotOfferWhatItsRoleRefuses()
{
    //A template whose filter alone would offer the relays for the room sensor (a number).
    QJsonObject tpl = shippedTemplate("thermostat_heat");
    QJsonArray fields = tpl["fields"].toArray();
    for (int i = 0;i < fields.size();i++)
        if (fields[i].toObject()["name"].toString() == "current_temp")
        {
            QJsonObject f = fields[i].toObject();
            f["io_filter"] = QJsonObject { { "value", "bool" }, { "writable", true } };
            fields[i] = f;
        }
    tpl["fields"] = fields;

    DialogNewDevice d(QJsonArray { tpl }, roomNamed("Salon"));
    QCOMPARE(itemData(field<QComboBox>(&d, "current_temp")), QStringList({ "" }));
    QCOMPARE(itemData(field<QComboBox>(&d, "output_heat")), QStringList({ "", "flag", "relay_heat", "relay_cool" }));
}

void tst_DialogNewDevice::editorsFollowTheFieldTypes()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Cave"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("esphome_climate"));

    QCOMPARE(field<QLineEdit>(&d, "mqtt_password")->echoMode(), QLineEdit::Password);
    QCOMPARE(field<QLineEdit>(&d, "mqtt_user")->echoMode(), QLineEdit::Normal);
    //The broker of the MQTT IOs of the house.
    QCOMPARE(field<QLineEdit>(&d, "mqtt_host")->text(), QString("h"));

    QDoubleSpinBox *port = field<QDoubleSpinBox>(&d, "mqtt_port");
    QCOMPARE(port->minimum(), 1.0);
    QCOMPARE(port->maximum(), 65535.0);
    QCOMPARE(port->decimals(), 0);
    QCOMPARE(port->value(), 1883.0);
    QDoubleSpinBox *step = field<QDoubleSpinBox>(&d, "temp_step");
    QCOMPARE(step->decimals(), 1);
    QCOMPARE(step->singleStep(), 0.1);
    QCOMPARE(step->suffix(), QString(" °C"));
    QCOMPARE(step->value(), 0.5);

    QCheckBox *twoPoint = field<QCheckBox>(&d, "two_point");
    QCOMPARE(twoPoint->text(), QString("Two setpoints"));
    QVERIFY(!twoPoint->isChecked());
    QVERIFY(field<QCheckBox>(&d, "with_action")->isChecked());

    QComboBox *room = field<QComboBox>(&d, "room");
    QCOMPARE(itemTexts(room), QStringList({ "Salon", "Cave" }));
    QCOMPARE(room->currentText(), QString("Cave"));

    //The description is said next to the editor and in its tooltip.
    QCOMPARE(field<QLineEdit>(&d, "object_id")->toolTip(), DeviceTemplates::field(shippedTemplate("esphome_climate"), "object_id")["description"].toString());
    bool hinted = false;
    for (QLabel *l: d.findChild<QWidget *>("row_object_id")->findChildren<QLabel *>())
        hinted |= l->text().startsWith("Object id of the climate entity");
    QVERIFY(hinted);
    QCOMPARE(d.findChild<QLabel *>("templateDescription")->text(), shippedTemplate("esphome_climate")["description"].toString());

    QVERIFY(d.selectTemplate("thermostat_heat"));
    QComboBox *emitter = field<QComboBox>(&d, "emitter");
    QCOMPARE(itemTexts(emitter), QStringList({ "Electric heaters", "Water radiators (boiler, zone valve)", "Underfloor heating" }));
    QCOMPARE(emitter->currentData().toString(), QString("electric"));
}

void tst_DialogNewDevice::aSmallStepGetsItsDecimals()
{
    const QJsonArray templates = QJsonDocument::fromJson(R"([{
      "id": "fine", "family": "fam", "impl": "imp", "name": "Fine", "description": "d",
      "attributes": { "name": "{name}" }, "ios": [], "links": [], "params": [],
      "fields": [ { "name": "room", "type": "room", "label": "Room", "required": true },
                  { "name": "gain", "type": "number", "label": "Gain", "required": true,
                    "min": 0, "max": 1, "step": 0.00001, "default": 0.00002 } ] }])").array();
    DialogNewDevice d(templates, roomNamed("Salon"));
    QDoubleSpinBox *gain = field<QDoubleSpinBox>(&d, "gain");
    QCOMPARE(gain->decimals(), 5);
    QCOMPARE(gain->value(), 0.00002);
}

void tst_DialogNewDevice::switchingTemplatesKeepsTheCommonValues()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    QVERIFY(d.selectTemplate("thermostat_heat"));
    QTest::keyClicks(field<QLineEdit>(&d, "name"), "Bureau");
    choose(field<QComboBox>(&d, "current_temp"), "hum_salon");
    choose(field<QComboBox>(&d, "emitter"), "floor");

    QVERIFY(d.selectTemplate("thermostat_heat_cool"));
    QCOMPARE(field<QLineEdit>(&d, "name")->text(), QString("Bureau"));
    QCOMPARE(field<QComboBox>(&d, "current_temp")->currentData().toString(), QString("hum_salon"));
    //Not one of this template's options: its default.
    QCOMPARE(field<QComboBox>(&d, "emitter")->currentData().toString(), QString("heat_pump"));
    QCOMPARE(d.findChildren<QWidget *>("field_emitter").size(), 1);
}

void tst_DialogNewDevice::switchingTemplatesKeepsOnlyWhatTheUserChanged()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    QVERIFY(d.selectTemplate("esphome_climate"));
    QLineEdit *host = field<QLineEdit>(&d, "mqtt_host");
    host->clear();
    QTest::keyClicks(host, "10.0.0.2");
    field<QDoubleSpinBox>(&d, "temp_max")->setValue(28);

    QVERIFY(d.selectTemplate("esphome_climate_ir"));
    QCOMPARE(field<QLineEdit>(&d, "mqtt_host")->text(), QString("10.0.0.2"));
    QCOMPARE(field<QDoubleSpinBox>(&d, "temp_max")->value(), 28.0);
    //Defaults the user left alone are this template's.
    QCOMPARE(field<QDoubleSpinBox>(&d, "temp_step")->value(), 1.0);
    QCOMPARE(d.values()["modes"].toStringList(), QStringList({ "off", "heat", "cool", "auto" }));

    //A value carried over stays the user's for the next switch.
    QVERIFY(d.selectTemplate("esphome_climate"));
    QCOMPARE(field<QLineEdit>(&d, "mqtt_host")->text(), QString("10.0.0.2"));
    QCOMPARE(field<QDoubleSpinBox>(&d, "temp_step")->value(), 0.5);
}

void tst_DialogNewDevice::theNotesOfTheTemplateAreShownAfterCreation()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("esphome_climate"));
    QTest::keyClicks(field<QLineEdit>(&d, "name"), "Clim salon");
    QTest::keyClicks(field<QLineEdit>(&d, "topic_prefix"), "salon-clim");
    QTest::keyClicks(field<QLineEdit>(&d, "object_id"), "clim_salon");
    choose(field<QComboBox>(&d, "remote_temp_source"), "temp_salon");
    clickCreate(&d);
    QVERIFY(d.device());
    QCOMPARE(d.notes().size(), 1);
    QVERIFY(d.notes().first().contains("topic: salon-clim/calaos/remote_temp"));

    std::unique_ptr<QDialog> notes(DialogNewDevice::createNotesDialog("Clim salon", d.notes(), nullptr));
    QTextBrowser *text = notes->findChild<QTextBrowser *>("notesText");
    QVERIFY(text->toPlainText().contains("salon-clim/calaos/remote_temp"));
    QVERIFY(!text->toPlainText().contains("```"));
    shoot(notes.get(), "notes");
}

void tst_DialogNewDevice::noTemplateNoCreate()
{
    DialogNewDevice d(QJsonArray(), roomNamed("Salon"));
    QVERIFY(!d.findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok)->isEnabled());
    QCOMPARE(d.findChild<QLabel *>("templateDescription")->text(), QString("No device template is available in this installer."));
}

namespace
{

const char *const NO_MQTT_HOUSE = R"(<?xml version="1.0"?>
<calaos:ioconfig xmlns:calaos="http://www.calaos.fr"><calaos:home format="2">
<calaos:room name="Salon" type="salon" hits="0">
  <calaos:input id="temp_salon" type="WITemp" name="Salon temperature" host="10.0.0.5" port="502" var="1"/>
</calaos:room></calaos:home></calaos:ioconfig>)";

QList<QPair<QString, QByteArray>> twoClimates()
{
    const QString p = "clim-buanderie/climate/clim_mitsu_buanderie/";
    return { { "esphome/discover/clim-buanderie",
               R"({"name":"clim-buanderie","friendly_name":"Clim buanderie","ip":"192.168.1.42"})" },
             { p + "mode/state", "heat" },
             { p + "target_temperature_low/state", "19" },
             { p + "target_temperature_high/state", "24" },
             { p + "current_temperature/state", "21.5" },
             { p + "action/state", "heating" },
             { p + "fan_mode/state", "auto" },
             { p + "swing_mode/state", "off" },
             { "clim-buanderie/status", "online" },
             { "salon-ir/climate/clim_salon/mode/state", "cool" },
             { "salon-ir/climate/clim_salon/target_temperature/state", "23" },
             { "salon-ir/status", "online" } };
}

WidgetEsphomeDiscovery *panel(DialogNewDevice &d)
{
    return d.findChild<WidgetEsphomeDiscovery *>("discoveryBox");
}

void searchOn(DialogNewDevice &d, quint16 port, const QString &host = "127.0.0.1")
{
    QDoubleSpinBox *portBox = field<QDoubleSpinBox>(&d, "mqtt_port");
    portBox->setValue(port);
    QLineEdit *hostEdit = d.findChild<QLineEdit *>("discoveryHost");
    hostEdit->clear();
    QTest::keyClicks(hostEdit, host);
    panel(d)->setTimings(300, 2000);
    QSignalSpy done(panel(d), &WidgetEsphomeDiscovery::searchFinished);
    QTest::mouseClick(d.findChild<QPushButton *>("discoveryButton"), Qt::LeftButton);
    QVERIFY(!d.findChild<QPushButton *>("discoveryButton")->isEnabled());
    QTRY_COMPARE_WITH_TIMEOUT(done.count(), 1, 10000);
}

}

void tst_DialogNewDevice::theDiscoveryIsOfferedForEsphomeTemplates()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("thermostat_heat"));
    QVERIFY(!panel(d)->isVisible());
    QVERIFY(d.selectTemplate("esphome_climate"));
    QVERIFY(panel(d)->isVisible());
    QVERIFY(d.selectTemplate("esphome_climate_ir"));
    QVERIFY(panel(d)->isVisible());
    QVERIFY(!d.findChild<QTreeWidget *>("discoveryList")->isVisible());
}

void tst_DialogNewDevice::theBrokerOfTheProjectFillsTheForm()
{
    const QByteArray xml1 = R"(<?xml version="1.0"?>
<calaos:ioconfig xmlns:calaos="http://www.calaos.fr"><calaos:home format="2">
<calaos:room name="Salon" type="salon" hits="0">
  <calaos:input id="t" type="MqttInputTemp" name="T" host="192.168.1.10" port="1884" user="calaos" password="s3cret" topic_sub="t"/>
</calaos:room></calaos:home></calaos:ioconfig>)";
    QVERIFY(loadIo(work("mqtt.xml"), xml1));
    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    QVERIFY(d.selectTemplate("esphome_climate"));
    QCOMPARE(field<QLineEdit>(&d, "mqtt_host")->text(), QString("192.168.1.10"));
    QCOMPARE(field<QDoubleSpinBox>(&d, "mqtt_port")->value(), 1884.0);
    QCOMPARE(field<QLineEdit>(&d, "mqtt_user")->text(), QString("calaos"));
    QCOMPARE(field<QLineEdit>(&d, "mqtt_password")->text(), QString("s3cret"));
    QCOMPARE(d.findChild<QLineEdit *>("discoveryHost")->text(), QString("192.168.1.10"));

    //What the user typed wins over the project's broker on a template switch.
    QLineEdit *host = field<QLineEdit>(&d, "mqtt_host");
    host->clear();
    QTest::keyClicks(host, "10.0.0.2");
    QCOMPARE(d.findChild<QLineEdit *>("discoveryHost")->text(), QString("10.0.0.2"));
    QVERIFY(d.selectTemplate("esphome_climate_ir"));
    QCOMPARE(field<QLineEdit>(&d, "mqtt_host")->text(), QString("10.0.0.2"));
    QCOMPARE(field<QLineEdit>(&d, "mqtt_user")->text(), QString("calaos"));

    //No MQTT IO: the template's default, to be completed by hand.
    QVERIFY(loadIo(work("nomqtt.xml"), NO_MQTT_HOUSE));
    DialogNewDevice bare(shippedTemplates(), roomNamed("Salon"));
    QVERIFY(bare.selectTemplate("esphome_climate"));
    QCOMPARE(field<QLineEdit>(&bare, "mqtt_host")->text(), QString("127.0.0.1"));
    QCOMPARE(field<QLineEdit>(&bare, "mqtt_password")->text(), QString());
}

void tst_DialogNewDevice::aLoopbackBrokerIsSearchedThroughTheBox()
{
    QVERIFY(loadIo(work("nomqtt.xml"), NO_MQTT_HOUSE));
    ConfigOptions::Instance().addAuth({ "user", "pass", "192.168.1.10:5454" });
    ConfigOptions::Instance().setCurrentAuthIndex(0);

    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    QVERIFY(d.selectTemplate("esphome_climate"));
    QLineEdit *probeHost = d.findChild<QLineEdit *>("discoveryHost");
    QCOMPARE(probeHost->text(), QString("192.168.1.10"));

    //The broker the Zigbee2MQTT dialog reached from this computer comes first.
    ConfigOptions::Instance().setMqttBrokerHost("10.0.0.7");
    {
        DialogNewDevice z(shippedTemplates(), roomNamed("Salon"));
        QVERIFY(z.selectTemplate("esphome_climate"));
        QCOMPARE(z.findChild<QLineEdit *>("discoveryHost")->text(), QString("10.0.0.7"));
    }
    QSettings().remove("calaos/mqtt_broker_host");

    //Once typed, the address stays the user's.
    probeHost->clear();
    QTest::keyClicks(probeHost, "box.lan");
    QTest::keyClicks(field<QLineEdit>(&d, "mqtt_host"), "x");
    QCOMPARE(probeHost->text(), QString("box.lan"));
    ConfigOptions::Instance().deleteAuth("192.168.1.10:5454");
}

void tst_DialogNewDevice::searchingListsTheClimatesAndChoosingOneFillsTheForm()
{
    FakeMqttBroker broker;
    broker.retained = twoClimates();
    broker.requiredUser = "calaos";
    broker.requiredPassword = "s3cret";
    const quint16 port = broker.start();

    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("esphome_climate"));
    QTest::keyClicks(field<QLineEdit>(&d, "mqtt_user"), "calaos");
    QTest::keyClicks(field<QLineEdit>(&d, "mqtt_password"), "s3cret");
    field<QLineEdit>(&d, "mqtt_host")->setText("127.0.0.1");
    searchOn(d, port);
    QCOMPARE(broker.password, QString("s3cret"));

    QTreeWidget *list = d.findChild<QTreeWidget *>("discoveryList");
    QVERIFY(list->isVisible());
    QCOMPARE(list->topLevelItemCount(), 2);
    QCOMPARE(d.findChild<QLabel *>("discoveryStatus")->text(), QString("2 climates found: choose the one to fill in the form with."));
    QTreeWidgetItem *buanderie = list->topLevelItem(0);
    QCOMPARE(buanderie->text(0), QString("Clim buanderie"));
    QCOMPARE(buanderie->text(1), QString("clim-buanderie"));
    QCOMPARE(buanderie->text(2), QString("clim_mitsu_buanderie"));
    QCOMPARE(buanderie->text(3), QString("two setpoints, fan, swing, action, now heat"));
    QCOMPARE(list->topLevelItem(1)->text(3), QString("one setpoint, now cool"));
    //Nothing chosen yet: the form is untouched.
    QCOMPARE(field<QLineEdit>(&d, "object_id")->text(), QString());

    list->setCurrentItem(buanderie);
    QCOMPARE(field<QLineEdit>(&d, "topic_prefix")->text(), QString("clim-buanderie"));
    QCOMPARE(field<QLineEdit>(&d, "object_id")->text(), QString("clim_mitsu_buanderie"));
    QCOMPARE(field<QLineEdit>(&d, "name")->text(), QString("Clim buanderie"));
    QVERIFY(field<QCheckBox>(&d, "two_point")->isChecked());
    QVERIFY(d.findChild<QCheckBox *>("field_fan_modes_auto")->isChecked());
    QVERIFY(d.findChild<QCheckBox *>("field_swing_modes_off")->isChecked());
    QVERIFY(d.findChild<QLabel *>("discoveryApplied")->text().startsWith(
        "Form filled in from clim-buanderie/climate/clim_mitsu_buanderie."));

    //The other one: one setpoint, no fan; the name typed meanwhile stays.
    field<QLineEdit>(&d, "name")->setText("Clim salon");
    list->setCurrentItem(list->topLevelItem(1));
    QCOMPARE(field<QLineEdit>(&d, "object_id")->text(), QString("clim_salon"));
    QVERIFY(!field<QCheckBox>(&d, "two_point")->isChecked());
    QVERIFY(!d.findChild<QCheckBox *>("field_fan_modes_auto")->isChecked());
    QCOMPARE(field<QLineEdit>(&d, "name")->text(), QString("Clim salon"));

    list->setCurrentItem(buanderie);
    field<QLineEdit>(&d, "name")->setText("Clim buanderie");
    field<QLineEdit>(&d, "mqtt_host")->setText("127.0.0.1");
    clickCreate(&d);
    QVERIFY2(d.device(), qPrintable(errorLabel(&d)->text()));
    const QStringList dev = deviceElement(saveIo(work("found.xml")), "clim_buanderie");
    const QString all = dev.join('\n');
    QVERIFY2(all.contains("topic_sub=clim-buanderie/climate/clim_mitsu_buanderie/target_temperature_low/state"), qPrintable(all));
    QVERIFY(!all.contains("target_temperature/state"));
    QVERIFY(all.contains("password=s3cret"));
    QVERIFY(all.contains("param name=fan_modes value=auto"));
}

void tst_DialogNewDevice::anUnreachableBrokerLeavesTheFormToFillByHand()
{
    QTcpServer closed;
    QVERIFY(closed.listen(QHostAddress::LocalHost, 0));
    const quint16 port = closed.serverPort();
    closed.close();

    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("esphome_climate"));
    searchOn(d, port);
    QCOMPARE(d.findChild<QLabel *>("discoveryStatus")->text(),
             QString("Nothing answers on 127.0.0.1:%1: check that the broker runs and listens on this port.").arg(port));
    QVERIFY(!d.findChild<QTreeWidget *>("discoveryList")->isVisible());
    QVERIFY(d.findChild<QPushButton *>("discoveryButton")->isEnabled());

    //An empty broker says so.
    FakeMqttBroker empty;
    searchOn(d, empty.start());
    QVERIFY(d.findChild<QLabel *>("discoveryStatus")->text().startsWith("No ESPHome climate on this broker."));

    //No address: asked for, nothing sent.
    d.findChild<QLineEdit *>("discoveryHost")->clear();
    QTest::mouseClick(d.findChild<QPushButton *>("discoveryButton"), Qt::LeftButton);
    QCOMPARE(d.findChild<QLabel *>("discoveryStatus")->text(), QString("Type the address of the broker as this computer reaches it."));
    QCOMPARE(empty.connects, 1);

    QTest::keyClicks(field<QLineEdit>(&d, "name"), "Clim");
    QTest::keyClicks(field<QLineEdit>(&d, "topic_prefix"), "clim");
    QTest::keyClicks(field<QLineEdit>(&d, "object_id"), "clim_salon");
    clickCreate(&d);
    QVERIFY(d.device());
}

void tst_DialogNewDevice::anOldEntityIsGreyedAtTheBottom()
{
    FakeMqttBroker broker;
    //Retained states of a former object id, on a node that no longer says it is online.
    broker.retained = { { "old-clim/climate/clim_mitsu/mode/state", "cool" },
                        { "old-clim/climate/clim_mitsu/target_temperature/state", "22" } };
    broker.retained += twoClimates();
    const quint16 port = broker.start();

    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("esphome_climate"));
    searchOn(d, port);

    QTreeWidget *list = d.findChild<QTreeWidget *>("discoveryList");
    QCOMPARE(list->topLevelItemCount(), 3);
    QCOMPARE(list->headerItem()->text(4), QString("Status"));
    QCOMPARE(list->topLevelItem(0)->text(4), QString("online"));
    QTreeWidgetItem *old = list->topLevelItem(2);
    QCOMPARE(old->text(1), QString("old-clim"));
    QCOMPARE(old->text(4), QString("old or offline"));
    QCOMPARE(old->foreground(0).color(), d.palette().color(QPalette::Disabled, QPalette::Text));
    QVERIFY(list->topLevelItem(0)->foreground(0).color() != old->foreground(0).color());
}

void tst_DialogNewDevice::theProjectCredentialsOnlyGoToItsBroker()
{
    const QByteArray xml = R"(<?xml version="1.0"?>
<calaos:ioconfig xmlns:calaos="http://www.calaos.fr"><calaos:home format="2">
<calaos:room name="Salon" type="salon" hits="0">
  <calaos:input id="t" type="MqttInputTemp" name="T" host="192.168.1.10" user="calaos" password="s3cret" topic_sub="t"/>
</calaos:room></calaos:home></calaos:ioconfig>)";
    QVERIFY(loadIo(work("creds.xml"), xml));
    FakeMqttBroker broker;
    const quint16 port = broker.start();

    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("esphome_climate"));
    QCOMPARE(field<QLineEdit>(&d, "mqtt_password")->text(), QString("s3cret"));
    //Another host than the project's broker: its password stays home.
    searchOn(d, port, "127.0.0.1");
    QCOMPARE(broker.connects, 1);
    QCOMPARE(broker.user, QString());
    QVERIFY(!broker.hadPassword);

    //Typed by the user for this search: sent.
    QLineEdit *user = field<QLineEdit>(&d, "mqtt_user");
    user->clear();
    QTest::keyClicks(user, "probe");
    searchOn(d, port, "127.0.0.1");
    QCOMPARE(broker.user, QString("probe"));
    QCOMPARE(broker.password, QString("s3cret"));

    //The project's broker itself, reached as the box: its credentials go.
    const QByteArray local = R"(<?xml version="1.0"?>
<calaos:ioconfig xmlns:calaos="http://www.calaos.fr"><calaos:home format="2">
<calaos:room name="Salon" type="salon" hits="0">
  <calaos:input id="t" type="MqttInputTemp" name="T" host="127.0.0.1" user="calaos" password="s3cret" topic_sub="t"/>
</calaos:room></calaos:home></calaos:ioconfig>)";
    QVERIFY(loadIo(work("creds2.xml"), local));
    ConfigOptions::Instance().addAuth({ "u", "p", "127.0.0.1" });
    ConfigOptions::Instance().setCurrentAuthIndex(0);
    DialogNewDevice box(shippedTemplates(), roomNamed("Salon"));
    box.show();
    QVERIFY(QTest::qWaitForWindowExposed(&box));
    QVERIFY(box.selectTemplate("esphome_climate"));
    searchOn(box, port, "127.0.0.1");
    QCOMPARE(broker.user, QString("calaos"));
    QCOMPARE(broker.password, QString("s3cret"));
    ConfigOptions::Instance().deleteAuth("127.0.0.1");
}

void tst_DialogNewDevice::enterInTheBrokerAddressSearches()
{
    FakeMqttBroker broker;
    const quint16 port = broker.start();
    DialogNewDevice d(shippedTemplates(), roomNamed("Salon"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("esphome_climate"));
    field<QDoubleSpinBox>(&d, "mqtt_port")->setValue(port);
    panel(d)->setTimings(300, 2000);
    QSignalSpy done(panel(d), &WidgetEsphomeDiscovery::searchFinished);
    QLineEdit *host = d.findChild<QLineEdit *>("discoveryHost");
    host->setText("127.0.0.1");
    host->setFocus();
    QTest::keyClick(host, Qt::Key_Return);
    //Not Create: the dialog stays, without the form's errors.
    QVERIFY(d.isVisible());
    QVERIFY(errorLabel(&d)->isHidden());
    QTRY_COMPARE_WITH_TIMEOUT(done.count(), 1, 10000);
    QCOMPARE(broker.connects, 1);
}

void tst_DialogNewDevice::screenshots()
{
    if (m_shots.isEmpty())
        QSKIP("CALAOS_SHOTS_DIR is not set");

    const QString lang = qEnvironmentVariable("CALAOS_SHOTS_LANG", "en");
    for (const QJsonValue &t: shippedTemplates(lang))
    {
        DialogNewDevice d(shippedTemplates(lang), roomNamed("Salon"));
        d.selectTemplate(t.toObject()["id"].toString());
        shoot(&d, lang + "_" + t.toObject()["id"].toString());
    }

    DialogNewDevice d(shippedTemplates(lang), roomNamed("Salon"));
    d.selectTemplate("thermostat_heat");
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    clickCreate(&d);
    shoot(&d, lang + "_refused");

    QTest::keyClicks(field<QLineEdit>(&d, "name"), "Chauffage salon");
    choose(field<QComboBox>(&d, "current_temp"), "temp_salon");
    choose(field<QComboBox>(&d, "output_heat"), "relay_heat");
    clickCreate(&d);
    QVERIFY(d.device());
    DialogDeviceInfo info(d.device());
    shoot(&info, lang + "_created_info");

    DialogNewRemoteUIRelay existing(roomNamed("Salon"));
    shoot(&existing, lang + "_existing_relay_dialog");

    FakeMqttBroker broker;
    broker.retained = twoClimates();
    //The former object id of the user's unit, next to its discovered successor.
    broker.retained << qMakePair(QString("clim-buanderie/climate/clim_mitsu/mode/state"), QByteArray("cool"));
    broker.retained << qMakePair(QString("homeassistant/climate/clim-buanderie/clim_mitsu_buanderie/config"),
                                 QByteArray(R"({"name":"Clim buanderie","mode_cmd_t":"clim-buanderie/climate/clim_mitsu_buanderie/mode/command","modes":["off","heat","cool","heat_cool"]})"));
    const quint16 port = broker.start();
    DialogNewDevice found(shippedTemplates(lang), roomNamed("Salon"));
    found.resize(720, 820);
    found.selectTemplate("esphome_climate");
    found.show();
    QVERIFY(QTest::qWaitForWindowExposed(&found));
    searchOn(found, port);
    shoot(&found, lang + "_discovery_found");
    QTreeWidget *list = found.findChild<QTreeWidget *>("discoveryList");
    list->setCurrentItem(list->topLevelItem(0));
    shoot(&found, lang + "_discovery_filled");

    QTcpServer closed;
    QVERIFY(closed.listen(QHostAddress::LocalHost, 0));
    const quint16 none = closed.serverPort();
    closed.close();
    DialogNewDevice failed(shippedTemplates(lang), roomNamed("Salon"));
    failed.selectTemplate("esphome_climate");
    failed.show();
    QVERIFY(QTest::qWaitForWindowExposed(&failed));
    searchOn(failed, none);
    shoot(&failed, lang + "_discovery_unreachable");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    //A compiled lang/calaos_installer_<lang>.qm for the French screenshots.
    const QString qm = qEnvironmentVariable("CALAOS_SHOTS_QM");
    if (!qm.isEmpty() && translator.load(qm))
        app.installTranslator(&translator);
    tst_DialogNewDevice tc;
    QTEST_SET_MAIN_SOURCE_PATH
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_dialognewdevice.moc"
