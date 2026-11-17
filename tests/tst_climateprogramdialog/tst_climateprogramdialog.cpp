#include <QtTest>
#include <QApplication>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
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

}

class tst_ClimateProgramDialog: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;
    QStringList m_dialogs;
    QTimer m_dialogKiller;

    QString work(const QString &file) const { return m_workDir.filePath(file); }

    bool load(const QByteArray &io)
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
        writeAll(work("src_io.xml"), io);
        QString p = work("src_io.xml");
        return ProjectManager::loadIOsFromFile(p);
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
    void init() { m_dialogs.clear(); }
    void cleanup()
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
        QVERIFY2(m_dialogs.isEmpty(), qPrintable(m_dialogs.join('|')));
    }

    void dialogShowsTheLevelCounts();
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
            if (QMessageBox *mb = qobject_cast<QMessageBox *>(w))
                m_dialogs << mb->text();
            w->close();
        }
    });
    m_dialogKiller.start(20);
}

void tst_ClimateProgramDialog::dialogShowsTheLevelCounts()
{
    QVERIFY(load(ioSource(climate(2, 1))));
    Device *dev = theClimate();
    QVERIFY(dev);

    DialogClimateProgram d(dev, false);
    QCOMPARE(d.levelCount(Set::Heat)->value(), 2);
    QCOMPARE(d.levelCount(Set::Cool)->value(), 1);
    QVERIFY(!d.changed());
}

QTEST_MAIN(tst_ClimateProgramDialog)
#include "tst_climateprogramdialog.moc"
