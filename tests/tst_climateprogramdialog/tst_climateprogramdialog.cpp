#include <QtTest>
#include <QApplication>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTimer>

#include "ClimateProgram.h"
#include "DialogClimateProgram.h"
#include "DialogDeleteImpact.h"
#include "projectmanager.h"

using namespace Calaos;
using ClimateProgram::Set;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

bool writeAll(const QString &path, const QByteArray &data)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
        return false;
    f.write(data);
    return true;
}

QByteArray readAll(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return QByteArray();
    return f.readAll();
}

QByteArray rulesSource(const QString &inner)
{
    return ("<?xml version=\"1.0\"?>\n<calaos:rules xmlns:calaos=\"http://www.calaos.fr\">\n" +
            inner + "\n</calaos:rules>\n").toUtf8();
}

QString ruleOn(const QString &name, const QString &inputId)
{
    return QStringLiteral(
        "<calaos:rule name=\"%1\" type=\"t\">\n"
        "  <calaos:condition><calaos:input id=\"%2\" oper=\"==\" val=\"true\"/></calaos:condition>\n"
        "  <calaos:action><calaos:output id=\"free\" val=\"true\"/></calaos:action>\n"
        "</calaos:rule>\n").arg(name, inputId);
}

QByteArray ioSource(const QString &inner)
{
    return ("<?xml version=\"1.0\"?>\n"
            "<calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">\n<calaos:home>\n" +
            inner + "\n</calaos:home>\n</calaos:ioconfig>\n").toUtf8();
}

const char *setName(Set set)
{
    return set == Set::Heat ? "heat" : "cool";
}

// The setpoint the fixture gives a level: distinct per set and level.
int setpointOf(Set set, int n)
{
    return (set == Set::Heat ? 14 : 22) + n;
}

// A climate as the server writes it back with these level counts: every
// program IO, linked under its role. Level n has one range on Monday at n:00.
QString climate(int heat, int cool, const QString &extraParams = QString())
{
    QString ios, links;
    for (Set set: { Set::Heat, Set::Cool })
    {
        const int count = set == Set::Heat ? heat : cool;
        const QString s = setName(set);
        for (int n = 1;n <= count;n++)
        {
            ios += QStringLiteral(
                "    <calaos:input id=\"clim_%1_level%2\" type=\"InPlageHoraire\" name=\"Salon %1 %2\" visible=\"false\" months=\"111111111111\">\n"
                "      <calaos:lundi><calaos:plage start_hour=\"%2\" start_min=\"0\" start_sec=\"0\" start_type=\"0\" start_offset=\"1\""
                " end_hour=\"%2\" end_min=\"30\" end_sec=\"0\" end_type=\"0\" end_offset=\"1\"/></calaos:lundi>\n"
                "    </calaos:input>\n"
                "    <calaos:internal id=\"clim_%1_level%2_setpoint\" type=\"InternalInt\" name=\"Salon %1 %2 setpoint\""
                " save=\"true\" value=\"%3\" value_min=\"7\" value_max=\"30\" step=\"0.5\" visible=\"false\"/>\n")
                   .arg(s).arg(n).arg(setpointOf(set, n));
            links += QStringLiteral("    <calaos:link name=\"%1_level%2\" io=\"clim_%1_level%2\"/>\n"
                                    "    <calaos:link name=\"%1_level%2_setpoint\" io=\"clim_%1_level%2_setpoint\"/>\n")
                         .arg(s).arg(n);
        }
        if (count > 0)
        {
            ios += QStringLiteral("    <calaos:internal id=\"clim_%1_default\" type=\"InternalInt\" name=\"Salon %1 default\""
                                  " save=\"true\" value=\"%2\" value_min=\"7\" value_max=\"30\" step=\"0.5\" visible=\"false\"/>\n")
                       .arg(s).arg(set == Set::Heat ? 17 : 26);
            links += QStringLiteral("    <calaos:link name=\"%1_default\" io=\"clim_%1_default\"/>\n").arg(s);
        }
    }
    if (cool > 0)
    {
        ios += "    <calaos:internal id=\"clim_cool_default_off\" type=\"InternalBool\" name=\"Salon cool off\" save=\"true\" value=\"false\" visible=\"false\"/>\n";
        links += "    <calaos:link name=\"cool_default_off\" io=\"clim_cool_default_off\"/>\n";
    }
    if (heat + cool > 0)
    {
        ios += "    <calaos:internal id=\"clim_program_enabled\" type=\"InternalBool\" name=\"Salon program\" save=\"true\" value=\"true\" visible=\"false\"/>\n";
        links += "    <calaos:link name=\"program_enabled\" io=\"clim_program_enabled\"/>\n";
    }

    return QStringLiteral(
        "<calaos:room name=\"A\" type=\"salon\" hits=\"1\">\n"
        "  <calaos:input id=\"temp\" type=\"WITemp\" name=\"temp\" host=\"h\" port=\"502\" var=\"0\"/>\n"
        "  <calaos:internal id=\"free\" type=\"InternalBool\" name=\"free\" visible=\"false\"/>\n"
        "  <calaos:device id=\"clim\" type=\"climate\" impl=\"thermostat\" name=\"Salon\">\n"
        "    <calaos:param name=\"heat_levels\" value=\"%1\"/>\n"
        "    <calaos:param name=\"cool_levels\" value=\"%2\"/>\n"
        "%3"
        "    <calaos:internal id=\"clim_target\" type=\"InternalInt\" name=\"target\" visible=\"false\"/>\n"
        "%4"
        "    <calaos:link name=\"current_temp\" io=\"temp\"/>\n"
        "    <calaos:link name=\"target_temp\" io=\"clim_target\"/>\n"
        "%5"
        "  </calaos:device>\n"
        "</calaos:room>")
        .arg(heat).arg(cool).arg(extraParams, ios, links);
}

Device *theClimate()
{
    return dynamic_cast<Device *>(ListeRoom::Instance().get_output("clim"));
}

QGroupBox *boxOf(QWidget *d, const QString &title)
{
    for (QGroupBox *g: d->findChildren<QGroupBox *>())
        if (g->title() == title)
            return g;
    return nullptr;
}

QGroupBox *setBox(QWidget *d, Set set)
{
    return boxOf(d, set == Set::Heat ? "Heating" : "Cooling");
}

// The setpoint spinboxes of a set, level 1 first (the first spinbox is the default setpoint).
QList<QDoubleSpinBox *> setpoints(QWidget *d, Set set)
{
    QList<QDoubleSpinBox *> l = setBox(d, set)->findChildren<QDoubleSpinBox *>();
    l.removeFirst();
    return l;
}

QDoubleSpinBox *defaultSpin(QWidget *d, Set set)
{
    return setBox(d, set)->findChildren<QDoubleSpinBox *>().first();
}

QList<QPushButton *> rangeButtons(QWidget *d, Set set)
{
    return setBox(d, set)->findChildren<QPushButton *>();
}

QDoubleSpinBox *gapSpin(QWidget *d)
{
    return boxOf(d, "Program")->findChild<QDoubleSpinBox *>();
}

QCheckBox *enabledBox(QWidget *d)
{
    return boxOf(d, "Program")->findChild<QCheckBox *>();
}

QCheckBox *coolOffBox(QWidget *d)
{
    for (QCheckBox *c: setBox(d, Set::Cool)->findChildren<QCheckBox *>())
        if (c->objectName() != "coolProgram")
            return c;
    return nullptr;
}

QStringList roles(Device *dev)
{
    QStringList l;
    for (IOBase *io: ClimateProgram::programIos(dev))
        l << u(ClimateProgram::roleOf(io));
    l.sort();
    return l;
}

// Level n of a set is in the model with a range and a setpoint.
bool hasLevel(Device *dev, Set set, int n)
{
    return ClimateProgram::ioForRole(dev, ClimateProgram::levelRole(set, n)) &&
           ClimateProgram::ioForRole(dev, ClimateProgram::setpointRole(set, n));
}

}

class tst_ClimateProgramDialog: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;
    QStringList m_dialogs;
    QTimer m_dialogKiller;
    // What the delete-impact confirmation gets, and how many were shown.
    bool m_confirmAnswer = true;
    int m_confirmations = 0;

    QString work(const QString &file) const { return m_workDir.filePath(file); }

    bool load(const QByteArray &io, const QByteArray &rules = QByteArray())
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
        writeAll(work("src_io.xml"), io);
        QString p = work("src_io.xml");
        if (!ProjectManager::loadIOsFromFile(p))
            return false;
        if (rules.isEmpty())
            return true;
        writeAll(work("src_rules.xml"), rules);
        p = work("src_rules.xml");
        return ProjectManager::loadRulesFromFile(p);
    }
    QByteArray saveIOs()
    {
        QString p = work("dst_io.xml");
        if (!ProjectManager::saveIOsToFile(p))
            return QByteArray();
        return readAll(p);
    }

private slots:
    void initTestCase();
    void init()
    {
        m_dialogs.clear();
        m_confirmAnswer = true;
        m_confirmations = 0;
    }
    void cleanup()
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
        QVERIFY2(m_dialogs.isEmpty(), qPrintable(m_dialogs.join('|')));
    }

    void dialogShowsTheProgramBoxes();
    void rowsFollowTheProgramBox();
    void checkingAProgramWritesTheMaximum();
    void aCountBelowTheMaximumReadsAsProgramAndStaysUntouched();
    void uncheckedWritesZero();
    void unchangedAcceptTouchesNothing();
    void checkingAProgramCreatesNoIo();
    void loweringToZeroDeletesEveryLevel();
    void loweringOneSetToZeroDeletesItsDefaults();
    void untouchedValuesAreNotRewrittenRounded();
    void aModeTheClimateDoesNotOfferIsDisabled();
    void aModeWithoutItsOutputLinkIsNotOffered();
    void refusedConfirmationKeepsEverything();
    void cancelKeepsEverything();
    void editedValuesSurviveARoundTrip();
    void coolOffDisablesTheCoolDefault();
    void levelsTheServerHasNotCreatedAreNotEditable();
    void readOnlyDialogEditsNothing();
    void rejectedRangesDialogChangesNothing();
    void programIosAreNotDeletableAlone();
    void screenshots();
};

void tst_ClimateProgramDialog::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());

    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_climateprogramdialog");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());

    // A modal box (reader duplicate id) would hang a headless run.
    connect(&m_dialogKiller, &QTimer::timeout, this, [this]()
    {
        if (QWidget *w = QApplication::activeModalWidget())
        {
            if (DialogDeleteImpact *c = qobject_cast<DialogDeleteImpact *>(w))
            {
                m_confirmations++;
                m_confirmAnswer ? c->accept() : c->reject();
                return;
            }
            if (QMessageBox *mb = qobject_cast<QMessageBox *>(w))
                m_dialogs << mb->text();
            w->close();
        }
    });
    m_dialogKiller.start(20);
}

void tst_ClimateProgramDialog::dialogShowsTheProgramBoxes()
{
    QVERIFY(load(ioSource(climate(2, 1))));
    Device *dev = theClimate();
    QVERIFY(dev);

    DialogClimateProgram d(dev, false);
    QVERIFY(d.programBox(Set::Heat)->isChecked());
    QVERIFY(d.programBox(Set::Cool)->isChecked());
    QVERIFY(!d.changed());
}

void tst_ClimateProgramDialog::rowsFollowTheProgramBox()
{
    QVERIFY(load(ioSource(climate(2, 1))));
    DialogClimateProgram d(theClimate(), false);

    for (Set set: { Set::Heat, Set::Cool })
    {
        const int count = set == Set::Heat ? 2 : 1;
        const QList<QDoubleSpinBox *> sp = setpoints(&d, set);
        const QList<QPushButton *> btn = rangeButtons(&d, set);
        QCOMPARE(sp.size(), ClimateProgram::kMaxLevels);
        QCOMPARE(btn.size(), ClimateProgram::kMaxLevels);
        for (int n = 1;n <= ClimateProgram::kMaxLevels;n++)
        {
            const QByteArray tag = QByteArray(setName(set)) + QByteArray::number(n);
            QCOMPARE_NE(sp[n - 1], nullptr);
            QVERIFY2(sp[n - 1]->isVisibleTo(&d) == (n <= count), tag.constData());
            QVERIFY2(btn[n - 1]->isVisibleTo(&d) == (n <= count), tag.constData());
            if (n <= count)
            {
                QVERIFY2(sp[n - 1]->isEnabled(), tag.constData());
                QVERIFY2(btn[n - 1]->isEnabled(), tag.constData());
                QCOMPARE(sp[n - 1]->value(), double(setpointOf(set, n)));
            }
        }
    }

    // Unchecking hides the rows' controls at once; their labels and a deletion
    // note stay until the dialog is accepted.
    d.programBox(Set::Heat)->setChecked(false);
    for (int n = 0;n < 2;n++)
    {
        QVERIFY(!setpoints(&d, Set::Heat)[n]->isVisibleTo(&d));
        QVERIFY(!rangeButtons(&d, Set::Heat)[n]->isVisibleTo(&d));
    }
    QVERIFY(rangeButtons(&d, Set::Cool)[0]->isVisibleTo(&d));
}

void tst_ClimateProgramDialog::unchangedAcceptTouchesNothing()
{
    QVERIFY(load(ioSource(climate(3, 2))));
    Device *dev = theClimate();
    const QByteArray before = saveIOs();
    QVERIFY(!before.isEmpty());

    {
        DialogClimateProgram d(dev, false);
        d.accept();
        QVERIFY(!d.changed());
        QVERIFY(d.removedIos().empty());
    }
    QCOMPARE(m_confirmations, 0);
    QCOMPARE(saveIOs(), before);
}

void tst_ClimateProgramDialog::checkingAProgramCreatesNoIo()
{
    QVERIFY(load(ioSource(climate(2, 0))));
    Device *dev = theClimate();
    const QStringList rolesBefore = roles(dev);

    DialogClimateProgram d(dev, false);
    QVERIFY(!d.programBox(Set::Cool)->isChecked());
    d.programBox(Set::Cool)->setChecked(true);
    // The server creates the IOs at its next start: a note stands for them.
    QVERIFY(!rangeButtons(&d, Set::Cool)[2]->isVisibleTo(&d));
    d.accept();

    QVERIFY(d.changed());
    QVERIFY(d.removedIos().empty());
    QCOMPARE(m_confirmations, 0);
    QCOMPARE(ClimateProgram::levels(dev, Set::Cool), ClimateProgram::kMaxLevels);
    QCOMPARE(ClimateProgram::levels(dev, Set::Heat), 2);
    QCOMPARE(roles(dev), rolesBefore);
    QVERIFY(!hasLevel(dev, Set::Cool, 1));

    const QByteArray xml = saveIOs();
    QVERIFY(xml.contains("name=\"cool_levels\" value=\"10\""));
    QVERIFY(xml.contains("name=\"heat_levels\" value=\"2\""));
    QVERIFY(!xml.contains("clim_cool_level1"));

    // And back from the file: the param is the maximum, read as checked.
    QVERIFY(load(xml));
    DialogClimateProgram again(theClimate(), false);
    QVERIFY(again.programBox(Set::Cool)->isChecked());
    QCOMPARE(roles(theClimate()), rolesBefore);
}

void tst_ClimateProgramDialog::checkingAProgramWritesTheMaximum()
{
    QVERIFY(load(ioSource(climate(0, 0))));
    Device *dev = theClimate();
    DialogClimateProgram d(dev, false);
    QVERIFY(!d.programBox(Set::Heat)->isChecked());
    QVERIFY(!d.programBox(Set::Cool)->isChecked());
    d.programBox(Set::Heat)->setChecked(true);
    d.accept();

    QCOMPARE(ClimateProgram::param(dev, "heat_levels", ""), std::string("10"));
    QCOMPARE(ClimateProgram::param(dev, "cool_levels", ""), std::string("0"));
}

void tst_ClimateProgramDialog::aCountBelowTheMaximumReadsAsProgramAndStaysUntouched()
{
    QVERIFY(load(ioSource(climate(3, 1))));
    Device *dev = theClimate();
    const QByteArray before = saveIOs();

    DialogClimateProgram d(dev, false);
    QVERIFY(d.programBox(Set::Heat)->isChecked());
    QVERIFY(d.programBox(Set::Cool)->isChecked());
    // Toggling back and forth is no change either.
    d.programBox(Set::Heat)->setChecked(false);
    d.programBox(Set::Heat)->setChecked(true);
    d.accept();

    QVERIFY(!d.changed());
    QCOMPARE(m_confirmations, 0);
    QCOMPARE(ClimateProgram::param(dev, "heat_levels", ""), std::string("3"));
    QCOMPARE(ClimateProgram::param(dev, "cool_levels", ""), std::string("1"));
    QCOMPARE(saveIOs(), before);
}

void tst_ClimateProgramDialog::uncheckedWritesZero()
{
    QVERIFY(load(ioSource(climate(3, 1))));
    Device *dev = theClimate();
    DialogClimateProgram d(dev, false);
    d.programBox(Set::Heat)->setChecked(false);
    d.accept();

    QCOMPARE(ClimateProgram::param(dev, "heat_levels", ""), std::string("0"));
    QCOMPARE(ClimateProgram::param(dev, "cool_levels", ""), std::string("1"));
    QCOMPARE(int(d.removedIos().size()), 3 * 2 + 1);
}

void tst_ClimateProgramDialog::loweringToZeroDeletesEveryLevel()
{
    QVERIFY(load(ioSource(climate(2, 2)), rulesSource(ruleOn("r_flag", "clim_program_enabled"))));
    Device *dev = theClimate();
    QCOMPARE(ListeRule::Instance().size(), 1);

    DialogClimateProgram d(dev, false);
    d.programBox(Set::Heat)->setChecked(false);
    d.programBox(Set::Cool)->setChecked(false);
    d.accept();

    QCOMPARE(m_confirmations, 1);
    // 8 levels and setpoints, 2 defaults, cool_default_off, program_enabled.
    QCOMPARE(int(d.removedIos().size()), 2 * 2 * 2 + 4);
    for (Set set: { Set::Heat, Set::Cool })
    {
        QCOMPARE(ClimateProgram::levels(dev, set), 0);
        for (int n = 1;n <= ClimateProgram::kMaxLevels;n++)
            QVERIFY(!ClimateProgram::ioForRole(dev, ClimateProgram::levelRole(set, n)));
        QVERIFY(!ClimateProgram::ioForRole(dev, ClimateProgram::defaultRole(set)));
    }
    QVERIFY(!ClimateProgram::ioForRole(dev, "cool_default_off"));
    QVERIFY(!ClimateProgram::ioForRole(dev, "program_enabled"));
    QVERIFY(ClimateProgram::programIos(dev).empty());
    QVERIFY(!saveIOs().contains("clim_program_enabled"));
    QCOMPARE(ListeRule::Instance().size(), 0);
    // The device's own IOs are not program levels.
    QVERIFY(ListeRoom::Instance().get_output("clim_target"));
    QVERIFY(ListeRoom::Instance().get_input("temp"));
    QVERIFY(ListeRoom::Instance().get_output("free"));
}

void tst_ClimateProgramDialog::loweringOneSetToZeroDeletesItsDefaults()
{
    QVERIFY(load(ioSource(climate(2, 2))));
    Device *dev = theClimate();

    DialogClimateProgram d(dev, false);
    d.programBox(Set::Cool)->setChecked(false);
    d.accept();

    // 4 cool levels and setpoints, cool_default, cool_default_off.
    QCOMPARE(int(d.removedIos().size()), 2 * 2 + 2);
    QVERIFY(!ClimateProgram::ioForRole(dev, ClimateProgram::defaultRole(Set::Cool)));
    QVERIFY(!ClimateProgram::ioForRole(dev, "cool_default_off"));
    QVERIFY(ClimateProgram::ioForRole(dev, ClimateProgram::defaultRole(Set::Heat)));
    QVERIFY(ClimateProgram::ioForRole(dev, "program_enabled"));
    QCOMPARE(ClimateProgram::levels(dev, Set::Heat), 2);
}

void tst_ClimateProgramDialog::untouchedValuesAreNotRewrittenRounded()
{
    QVERIFY(load(ioSource(climate(1, 1, "    <calaos:param name=\"program_min_gap\" value=\"1.25\"/>\n"))));
    Device *dev = theClimate();
    // set_state stores values as given: the contract does not round them.
    ClimateProgram::setIoValue(ClimateProgram::ioForRole(dev, "heat_level1_setpoint"), 21.25);
    ClimateProgram::setIoValue(ClimateProgram::ioForRole(dev, "heat_default"), 17.25);

    DialogClimateProgram d(dev, false);
    d.accept();

    QVERIFY(!d.changed());
    QCOMPARE(ClimateProgram::ioValue(ClimateProgram::ioForRole(dev, "heat_level1_setpoint"), 0), 21.25);
    QCOMPARE(ClimateProgram::ioValue(ClimateProgram::ioForRole(dev, "heat_default"), 0), 17.25);
    QCOMPARE(u(ClimateProgram::param(dev, "program_min_gap", "")), QString("1.25"));
}

void tst_ClimateProgramDialog::aModeTheClimateDoesNotOfferIsDisabled()
{
    QVERIFY(load(ioSource(climate(1, 1))));
    Device *dev = theClimate();
    {
        DialogClimateProgram d(dev, false);
        QVERIFY(boxOf(&d, "Heating")->isEnabled());
        QVERIFY(boxOf(&d, "Cooling")->isEnabled());
    }

    ClimateProgram::setParam(dev, "modes", "off,heat");
    DialogClimateProgram d(dev, false);
    QVERIFY(d.programBox(Set::Heat)->isEnabled());
    QVERIFY(d.programBox(Set::Cool)->isEnabled());
    QVERIFY(boxOf(&d, "Heating")->findChild<QDoubleSpinBox *>()->isEnabled());
    QVERIFY(!boxOf(&d, "Cooling")->findChild<QDoubleSpinBox *>()->isEnabled());
    QVERIFY(ClimateProgram::offers(dev, Set::Heat));
    QVERIFY(!ClimateProgram::offers(dev, Set::Cool));

    // auto needs heat and cool offered: the server then offers off only.
    ClimateProgram::setParam(dev, "modes", "off,auto");
    QVERIFY(!ClimateProgram::offers(dev, Set::Heat));
    QVERIFY(!ClimateProgram::offers(dev, Set::Cool));

    ClimateProgram::setParam(dev, "modes", "off,heat,cool");
    QVERIFY(ClimateProgram::offers(dev, Set::Heat));
    QVERIFY(ClimateProgram::offers(dev, Set::Cool));
}

void tst_ClimateProgramDialog::aModeWithoutItsOutputLinkIsNotOffered()
{
    // Nothing linked yet: both modes stay offered.
    QVERIFY(load(ioSource(climate(1, 1))));
    QVERIFY(ClimateProgram::offers(theClimate(), Set::Heat));
    QVERIFY(ClimateProgram::offers(theClimate(), Set::Cool));

    QString xml = climate(1, 1);
    xml.replace("  </calaos:device>", "    <calaos:link name=\"output_heat\" io=\"free\"/>\n  </calaos:device>");
    QVERIFY(load(ioSource(xml)));
    QVERIFY(ClimateProgram::offers(theClimate(), Set::Heat));
    QVERIFY(!ClimateProgram::offers(theClimate(), Set::Cool));
}

void tst_ClimateProgramDialog::refusedConfirmationKeepsEverything()
{
    QVERIFY(load(ioSource(climate(3, 3))));
    Device *dev = theClimate();
    const QByteArray before = saveIOs();
    m_confirmAnswer = false;

    DialogClimateProgram d(dev, false);
    d.programBox(Set::Heat)->setChecked(false);
    setpoints(&d, Set::Cool)[0]->setValue(29);
    d.accept();

    QCOMPARE(m_confirmations, 1);
    QVERIFY(!d.changed());
    QVERIFY(d.removedIos().empty());
    QCOMPARE(ClimateProgram::levels(dev, Set::Heat), 3);
    QCOMPARE(ClimateProgram::ioValue(ClimateProgram::ioForRole(dev, "cool_level1_setpoint"), -1),
             double(setpointOf(Set::Cool, 1)));
    QCOMPARE(saveIOs(), before);
}

void tst_ClimateProgramDialog::cancelKeepsEverything()
{
    QVERIFY(load(ioSource(climate(2, 2))));
    Device *dev = theClimate();
    const QByteArray before = saveIOs();

    DialogClimateProgram d(dev, false);
    d.programBox(Set::Heat)->setChecked(false);
    defaultSpin(&d, Set::Heat)->setValue(20);
    gapSpin(&d)->setValue(5);
    enabledBox(&d)->setChecked(false);
    d.reject();

    QCOMPARE(m_confirmations, 0);
    QVERIFY(!d.changed());
    QCOMPARE(saveIOs(), before);
}

void tst_ClimateProgramDialog::editedValuesSurviveARoundTrip()
{
    QVERIFY(load(ioSource(climate(2, 2))));
    Device *dev = theClimate();

    {
        DialogClimateProgram d(dev, false);
        QCOMPARE(defaultSpin(&d, Set::Heat)->value(), 17.0);
        QCOMPARE(defaultSpin(&d, Set::Cool)->value(), 26.0);
        QCOMPARE(gapSpin(&d)->value(), 2.0);
        QVERIFY(enabledBox(&d)->isChecked());
        QVERIFY(!coolOffBox(&d)->isChecked());

        defaultSpin(&d, Set::Heat)->setValue(18.5);
        defaultSpin(&d, Set::Cool)->setValue(27);
        setpoints(&d, Set::Heat)[1]->setValue(19.5);
        setpoints(&d, Set::Cool)[0]->setValue(23);
        gapSpin(&d)->setValue(3.5);
        enabledBox(&d)->setChecked(false);
        d.accept();
        QVERIFY(d.changed());
        QCOMPARE(m_confirmations, 0);
    }

    QCOMPARE(ClimateProgram::ioValue(ClimateProgram::ioForRole(dev, "heat_default"), -1), 18.5);
    QCOMPARE(ClimateProgram::param(dev, "heat_default_setpoint", ""), std::string("18.5"));
    QCOMPARE(ClimateProgram::param(dev, "program_min_gap", ""), std::string("3.5"));
    QVERIFY(!ClimateProgram::ioBool(ClimateProgram::ioForRole(dev, "program_enabled"), true));

    QVERIFY(load(saveIOs()));
    dev = theClimate();
    QVERIFY(dev);
    DialogClimateProgram d(dev, false);
    QCOMPARE(defaultSpin(&d, Set::Heat)->value(), 18.5);
    QCOMPARE(defaultSpin(&d, Set::Cool)->value(), 27.0);
    QCOMPARE(setpoints(&d, Set::Heat)[0]->value(), double(setpointOf(Set::Heat, 1)));
    QCOMPARE(setpoints(&d, Set::Heat)[1]->value(), 19.5);
    QCOMPARE(setpoints(&d, Set::Cool)[0]->value(), 23.0);
    QCOMPARE(setpoints(&d, Set::Cool)[1]->value(), double(setpointOf(Set::Cool, 2)));
    QCOMPARE(gapSpin(&d)->value(), 3.5);
    QVERIFY(!enabledBox(&d)->isChecked());
    // The level ranges came through untouched.
    QCOMPARE(int(ClimateProgram::ioForRole(dev, "heat_level2")->range_monday.size()), 1);
    QCOMPARE(u(ClimateProgram::ioForRole(dev, "heat_level2")->range_monday[0].shour), QString("2"));
}

void tst_ClimateProgramDialog::coolOffDisablesTheCoolDefault()
{
    QVERIFY(load(ioSource(climate(1, 1))));
    Device *dev = theClimate();

    {
        DialogClimateProgram d(dev, false);
        QVERIFY(defaultSpin(&d, Set::Cool)->isEnabled());
        coolOffBox(&d)->setChecked(true);
        QVERIFY(!defaultSpin(&d, Set::Cool)->isEnabled());
        QVERIFY(defaultSpin(&d, Set::Heat)->isEnabled());
        d.accept();
        QVERIFY(d.changed());
    }
    QVERIFY(ClimateProgram::ioBool(ClimateProgram::ioForRole(dev, "cool_default_off"), false));
    QCOMPARE(ClimateProgram::param(dev, "cool_default_off", ""), std::string("true"));

    QVERIFY(load(saveIOs()));
    DialogClimateProgram d(theClimate(), false);
    QVERIFY(coolOffBox(&d)->isChecked());
    QVERIFY(!defaultSpin(&d, Set::Cool)->isEnabled());
}

void tst_ClimateProgramDialog::levelsTheServerHasNotCreatedAreNotEditable()
{
    // The params ask for 4 heat levels, the file only has the first two.
    QString io = climate(2, 0);
    io.replace("name=\"heat_levels\" value=\"2\"", "name=\"heat_levels\" value=\"4\"");
    QVERIFY(load(ioSource(io)));
    Device *dev = theClimate();
    QCOMPARE(ClimateProgram::levels(dev, Set::Heat), 4);

    DialogClimateProgram d(dev, false);
    QVERIFY(d.programBox(Set::Heat)->isChecked());
    const QList<QPushButton *> btn = rangeButtons(&d, Set::Heat);
    QVERIFY(btn[0]->isVisibleTo(&d) && btn[0]->isEnabled());
    QVERIFY(btn[1]->isVisibleTo(&d) && btn[1]->isEnabled());
    QVERIFY(!btn[2]->isVisibleTo(&d));
    QVERIFY(!btn[3]->isVisibleTo(&d));
    QVERIFY(!setpoints(&d, Set::Heat)[2]->isVisibleTo(&d));

    // Accepted as is: nothing is rewritten, created or deleted.
    d.accept();
    QCOMPARE(m_confirmations, 0);
    QVERIFY(!d.changed());
    QVERIFY(d.removedIos().empty());
    QCOMPARE(ClimateProgram::levels(dev, Set::Heat), 4);
    QVERIFY(hasLevel(dev, Set::Heat, 2));
}

void tst_ClimateProgramDialog::readOnlyDialogEditsNothing()
{
    QVERIFY(load(ioSource(climate(2, 2))));
    Device *dev = theClimate();
    const QByteArray before = saveIOs();

    DialogClimateProgram d(dev, true);
    QVERIFY(!d.programBox(Set::Heat)->isEnabled());
    QVERIFY(!d.programBox(Set::Cool)->isEnabled());
    QVERIFY(!gapSpin(&d)->isEnabled());
    QVERIFY(!enabledBox(&d)->isEnabled());
    QVERIFY(!coolOffBox(&d)->isEnabled());
    QVERIFY(!defaultSpin(&d, Set::Heat)->isEnabled());
    for (Set set: { Set::Heat, Set::Cool })
        for (int n = 0;n < 2;n++)
        {
            QVERIFY(!setpoints(&d, set)[n]->isEnabled());
            QVERIFY(!rangeButtons(&d, set)[n]->isEnabled());
        }

    // Even a value forced from code does not reach the model.
    d.programBox(Set::Heat)->setChecked(false);
    d.accept();
    QCOMPARE(m_confirmations, 0);
    QVERIFY(!d.changed());
    QVERIFY(d.removedIos().empty());
    QCOMPARE(saveIOs(), before);
}

void tst_ClimateProgramDialog::rejectedRangesDialogChangesNothing()
{
    QVERIFY(load(ioSource(climate(1, 0))));
    Device *dev = theClimate();
    const QByteArray before = saveIOs();

    // The time range editor is modal: the killer timer closes it (reject).
    DialogClimateProgram d(dev, false);
    QPushButton *btn = rangeButtons(&d, Set::Heat)[0];
    QVERIFY(btn->isEnabled());
    btn->click();

    QVERIFY(!d.changed());
    QCOMPARE(saveIOs(), before);
}

void tst_ClimateProgramDialog::programIosAreNotDeletableAlone()
{
    QVERIFY(load(ioSource(climate(6, 6))));
    Device *dev = theClimate();

    const std::vector<IOBase *> program = ClimateProgram::programIos(dev);
    QCOMPARE(int(program.size()), 6 * 2 * 2 + 2 + 1 + 1);
    for (IOBase *io: program)
        QVERIFY2(!ClimateProgram::canDeleteAlone(io), io->get_param("id").c_str());

    // The climate's own non-program IO and free IOs stay deletable.
    QVERIFY(ClimateProgram::canDeleteAlone(ListeRoom::Instance().get_output("clim_target")));
    QVERIFY(ClimateProgram::canDeleteAlone(ListeRoom::Instance().get_output("free")));
    QVERIFY(ClimateProgram::canDeleteAlone(ListeRoom::Instance().get_input("temp")));
}

void tst_ClimateProgramDialog::screenshots()
{
    const QString dir = qEnvironmentVariable("CALAOS_SHOTS_DIR");
    if (dir.isEmpty())
        QSKIP("CALAOS_SHOTS_DIR not set");
    QVERIFY(load(ioSource(climate(3, 2))));
    DialogClimateProgram d(theClimate(), false);
    d.show();
    QVERIFY(QTest::qWaitForWindowExposed(&d));
    QVERIFY(d.grab().save(QDir(dir).filePath(qEnvironmentVariable("CALAOS_SHOTS_NAME", "dialog") + ".png")));
}

QTEST_MAIN(tst_ClimateProgramDialog)
#include "tst_climateprogramdialog.moc"
