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
#include "DialogNewDevice.h"
#include "DialogDeviceInfo.h"
#include "DialogNewRemoteUIRelay.h"

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
    void editorsFollowTheFieldTypes();
    void aSmallStepGetsItsDecimals();
    void switchingTemplatesKeepsTheCommonValues();
    void switchingTemplatesKeepsOnlyWhatTheUserChanged();
    void theNotesOfTheTemplateAreShownAfterCreation();
    void noTemplateNoCreate();
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

void tst_DialogNewDevice::editorsFollowTheFieldTypes()
{
    DialogNewDevice d(shippedTemplates(), roomNamed("Cave"));
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.selectTemplate("esphome_climate"));

    QCOMPARE(field<QLineEdit>(&d, "mqtt_password")->echoMode(), QLineEdit::Password);
    QCOMPARE(field<QLineEdit>(&d, "mqtt_user")->echoMode(), QLineEdit::Normal);
    QCOMPARE(field<QLineEdit>(&d, "mqtt_host")->text(), QString("127.0.0.1"));

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
