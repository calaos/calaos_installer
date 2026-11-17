#include <QtTest>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTimer>
#include <QTreeWidget>

#include "ClimateProgram.h"
#include "HomeTree.h"
#include "qtreewidget_addition.h"
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

QByteArray rulesSource(const QString &inner)
{
    return ("<?xml version=\"1.0\"?>\n<calaos:rules xmlns:calaos=\"http://www.calaos.fr\">\n" +
            inner + "\n</calaos:rules>\n").toUtf8();
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

// Rules on a kept level (heat 1), on removed ones (heat 5 as a condition,
// cool 6 setpoint as an action) and on program_enabled.
const char *kRules = R"(
<calaos:rule name="r_kept" type="t">
  <calaos:condition><calaos:input id="clim_heat_level1" oper="==" val="true"/></calaos:condition>
  <calaos:action><calaos:output id="clim_heat_level1_setpoint" val="20"/></calaos:action>
</calaos:rule>
<calaos:rule name="r_heat5" type="t">
  <calaos:condition><calaos:input id="clim_heat_level5" oper="==" val="true"/></calaos:condition>
  <calaos:action><calaos:output id="free" val="true"/></calaos:action>
</calaos:rule>
<calaos:rule name="r_cool6" type="t">
  <calaos:condition><calaos:input id="temp" oper="&gt;" val="25"/></calaos:condition>
  <calaos:action><calaos:output id="clim_cool_level6_setpoint" val="24"/></calaos:action>
</calaos:rule>
<calaos:rule name="r_enabled" type="t">
  <calaos:condition><calaos:input id="clim_program_enabled" oper="==" val="false"/></calaos:condition>
  <calaos:action><calaos:output id="free" val="false"/></calaos:action>
</calaos:rule>)";

Device *theClimate()
{
    return dynamic_cast<Device *>(ListeRoom::Instance().get_output("clim"));
}

QStringList ruleNames()
{
    QStringList l;
    for (int i = 0;i < ListeRule::Instance().size();i++)
        l << u(ListeRule::Instance().get_rule(i)->get_name());
    l.sort();
    return l;
}

QStringList linkNames(Device *dev)
{
    QStringList l;
    for (const DeviceLink &link: dev->links())
        l << u(link.name);
    l.sort();
    return l;
}

QStringList programRoles(int heat, int cool)
{
    QStringList l;
    for (Set set: { Set::Heat, Set::Cool })
    {
        const int count = set == Set::Heat ? heat : cool;
        for (int n = 1;n <= count;n++)
            l << u(ClimateProgram::levelRole(set, n)) << u(ClimateProgram::setpointRole(set, n));
        if (count > 0)
            l << u(ClimateProgram::defaultRole(set));
    }
    if (cool > 0)
        l << "cool_default_off";
    if (heat + cool > 0)
        l << "program_enabled";
    l.sort();
    return l;
}

// Every level of both sets is there with its own range and setpoint.
void checkLevels(Device *dev, int heat, int cool)
{
    for (Set set: { Set::Heat, Set::Cool })
    {
        const int count = set == Set::Heat ? heat : cool;
        QCOMPARE(ClimateProgram::levels(dev, set), count);
        for (int n = 1;n <= ClimateProgram::kMaxLevels;n++)
        {
            IOBase *range = ClimateProgram::ioForRole(dev, ClimateProgram::levelRole(set, n));
            IOBase *sp = ClimateProgram::ioForRole(dev, ClimateProgram::setpointRole(set, n));
            const QByteArray tag = QByteArray(setName(set)) + QByteArray::number(n);
            QVERIFY2((range != nullptr) == (n <= count), tag.constData());
            QVERIFY2((sp != nullptr) == (n <= count), tag.constData());
            if (n > count)
                continue;
            QCOMPARE(range->get_param("type"), std::string("InPlageHoraire"));
            QCOMPARE(int(range->range_monday.size()), 1);
            QCOMPARE(u(range->range_monday[0].shour), QString::number(n));
            QCOMPARE(u(range->range_monday[0].emin), QString("30"));
            QCOMPARE(ClimateProgram::ioValue(sp, -1), double(setpointOf(set, n)));
            QCOMPARE(sp->get_param("save"), std::string("true"));
        }
    }
}

int itemCount(QTreeWidgetItem *parent)
{
    return parent ? parent->childCount() : -1;
}

QTreeWidgetItemProgram *programItemOf(QTreeWidget *tree)
{
    for (QTreeWidgetItemIterator it(tree);*it;++it)
        if (QTreeWidgetItemProgram *p = dynamic_cast<QTreeWidgetItemProgram *>(*it))
            return p;
    return nullptr;
}

QTreeWidgetItemDevice *deviceItemOf(QTreeWidget *tree)
{
    for (QTreeWidgetItemIterator it(tree);*it;++it)
        if (QTreeWidgetItemDevice *d = dynamic_cast<QTreeWidgetItemDevice *>(*it))
            return d;
    return nullptr;
}

void populate(QTreeWidget *tree)
{
    tree->clear();
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
        populateRoomItems(new QTreeWidgetItemRoom(lr.get_room(r), tree));
    tree->expandAll();
    collapseProgramItems(tree);
}

}

class tst_ClimateProgram: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;
    QStringList m_dialogs;
    QTimer m_dialogKiller;

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
    void init() { m_dialogs.clear(); }
    void cleanup()
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
        QVERIFY2(m_dialogs.isEmpty(), qPrintable(m_dialogs.join('|')));
    }

    void sixLevelsPerSetSurviveARoundTrip();
    void lowerCountsDeleteTheLevelsAbove();
    void lowerCountsSurviveARoundTrip();
    void linksOfMissingLevelsGoToo();
    void levelCountsAsTheServerReadsThem_data();
    void levelCountsAsTheServerReadsThem();
    void programIosAreNotDeletableAlone();
    void onlyProgramRolesOfAClimateCount();
    void treeGroupsTheProgramUnderItsClimate();
    void deletedLevelsLeaveTheTree();
    void rulesNameProgramIosAfterTheirClimate();
};

void tst_ClimateProgram::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());

    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_climateprogram");
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

void tst_ClimateProgram::sixLevelsPerSetSurviveARoundTrip()
{
    QVERIFY(load(ioSource(climate(6, 6))));
    Device *dev = theClimate();
    QVERIFY(dev);
    checkLevels(dev, 6, 6);
    QCOMPARE(int(ClimateProgram::programIos(dev).size()), 6 * 2 * 2 + 2 + 1 + 1);

    const QByteArray written = saveIOs();
    QVERIFY(!written.isEmpty());

    QVERIFY(load(written));
    dev = theClimate();
    QVERIFY(dev);
    checkLevels(dev, 6, 6);
    QStringList expected = programRoles(6, 6) << "current_temp" << "target_temp";
    expected.sort();
    QCOMPARE(linkNames(dev), expected);
    QCOMPARE(ClimateProgram::ioBool(ClimateProgram::ioForRole(dev, "program_enabled"), false), true);
    QCOMPARE(ClimateProgram::ioValue(ClimateProgram::ioForRole(dev, "cool_default"), -1), 26.0);

    QCOMPARE(saveIOs(), written);
}

void tst_ClimateProgram::lowerCountsDeleteTheLevelsAbove()
{
    QVERIFY(load(ioSource(climate(6, 6)), rulesSource(kRules)));
    Device *dev = theClimate();
    QVERIFY(dev);
    IOBase *heat3 = ClimateProgram::ioForRole(dev, "heat_level3");

    const std::vector<IOBase *> expected = ClimateProgram::iosRemovedBy(dev, 2, 4);
    QCOMPARE(int(expected.size()), (4 + 2) * 2);
    QVERIFY(std::find(expected.begin(), expected.end(), heat3) != expected.end());

    const std::vector<IOBase *> gone = ClimateProgram::applyLevels(dev, 2, 4);
    QCOMPARE(gone, expected);

    checkLevels(dev, 2, 4);
    QCOMPARE(u(ClimateProgram::param(dev, "heat_levels", "")), QString("2"));
    QCOMPARE(u(ClimateProgram::param(dev, "cool_levels", "")), QString("4"));
    QStringList links = programRoles(2, 4) << "current_temp" << "target_temp";
    links.sort();
    QCOMPARE(linkNames(dev), links);

    QVERIFY(!ListeRoom::Instance().get_input("clim_heat_level3"));
    QVERIFY(!ListeRoom::Instance().get_output("clim_heat_level3_setpoint"));
    QVERIFY(!ListeRoom::Instance().get_output("clim_cool_level5_setpoint"));
    QVERIFY(ListeRoom::Instance().get_output("clim_cool_level4_setpoint"));
    QVERIFY(ListeRoom::Instance().get_output("clim_heat_default"));
    QCOMPARE(ruleNames(), QStringList({ "r_enabled", "r_kept" }));
}

void tst_ClimateProgram::lowerCountsSurviveARoundTrip()
{
    QVERIFY(load(ioSource(climate(6, 6))));
    ClimateProgram::applyLevels(theClimate(), 1, 0);

    const QByteArray written = saveIOs();
    QVERIFY(!written.isEmpty());
    QVERIFY(!written.contains("clim_heat_level2"));
    QVERIFY(!written.contains("clim_cool_level1"));
    QVERIFY(!QString::fromUtf8(written).contains(QRegularExpression("\"cool_level[1-6]")));

    QVERIFY(load(written));
    Device *dev = theClimate();
    QVERIFY(dev);
    checkLevels(dev, 1, 0);
}

// A count lowered by hand in io.xml leaves links the server drops with it.
void tst_ClimateProgram::linksOfMissingLevelsGoToo()
{
    QVERIFY(load(ioSource(climate(3, 0))));
    Device *dev = theClimate();
    dev->links().push_back({ "heat_level5", "clim_gone", "" });

    QVERIFY(ClimateProgram::applyLevels(dev, 2, 0).size() == 2);
    QStringList links = programRoles(2, 0) << "current_temp" << "target_temp";
    links.sort();
    QCOMPARE(linkNames(dev), links);
}

void tst_ClimateProgram::levelCountsAsTheServerReadsThem_data()
{
    QTest::addColumn<QString>("value");
    QTest::addColumn<int>("levels");

    QTest::newRow("absent") << QString() << 0;
    QTest::newRow("zero") << "0" << 0;
    QTest::newRow("three") << "3" << 3;
    QTest::newRow("six") << "6" << 6;
    QTest::newRow("above six") << "8" << 6;
    QTest::newRow("negative") << "-1" << 0;
    QTest::newRow("not whole") << "2.5" << 0;
    QTest::newRow("not a number") << "abc" << 0;
    QTest::newRow("empty") << "" << 0;
}

void tst_ClimateProgram::levelCountsAsTheServerReadsThem()
{
    QFETCH(QString, value);
    QFETCH(int, levels);

    Params p;
    p.Add("id", "c");
    p.Add("name", "c");
    p.Add("type", "climate");
    p.Add("impl", "thermostat");
    Device dev(p);
    if (!value.isNull())
        ClimateProgram::setParam(&dev, "heat_levels", value.toStdString());
    QCOMPARE(ClimateProgram::levels(&dev, Set::Heat), levels);

    ClimateProgram::setLevels(&dev, Set::Cool, 9);
    QCOMPARE(u(ClimateProgram::param(&dev, "cool_levels", "")), QString("6"));
    ClimateProgram::setLevels(&dev, Set::Cool, -2);
    QCOMPARE(u(ClimateProgram::param(&dev, "cool_levels", "")), QString("0"));
}

void tst_ClimateProgram::programIosAreNotDeletableAlone()
{
    QVERIFY(load(ioSource(climate(2, 1))));
    Device *dev = theClimate();
    QVERIFY(dev);

    for (IOBase *io: ClimateProgram::programIos(dev))
        QVERIFY2(!ClimateProgram::canDeleteAlone(io), io->get_param("id").c_str());
    QCOMPARE(int(ClimateProgram::programIos(dev).size()), (2 + 1) * 2 + 2 + 1 + 1);

    QVERIFY(ClimateProgram::canDeleteAlone(dev));
    QVERIFY(ClimateProgram::canDeleteAlone(ClimateProgram::ioForRole(dev, "target_temp")));
    QVERIFY(ClimateProgram::canDeleteAlone(ListeRoom::Instance().get_output("free")));
    QVERIFY(ClimateProgram::canDeleteAlone(ListeRoom::Instance().get_input("temp")));
}

void tst_ClimateProgram::onlyProgramRolesOfAClimateCount()
{
    QString src = climate(1, 0);
    QVERIFY(load(ioSource(src.replace("impl=\"thermostat\"", "impl=\"pid\""))));
    Device *dev = theClimate();
    QVERIFY(dev);
    QVERIFY(!ClimateProgram::supports(dev));
    QVERIFY(ClimateProgram::programIos(dev).empty());
    QVERIFY(ClimateProgram::canDeleteAlone(ListeRoom::Instance().get_input("clim_heat_level1")));

    QVERIFY(load(ioSource(climate(1, 0).replace("impl=\"thermostat\"", "impl=\"Proxy\""))));
    QVERIFY(ClimateProgram::supports(theClimate()));
    QCOMPARE(int(ClimateProgram::programIos(theClimate()).size()), 4);

    QVERIFY(ClimateProgram::isProgramRole("heat_level6_setpoint"));
    QVERIFY(!ClimateProgram::isProgramRole("heat_level7"));
    QVERIFY(!ClimateProgram::isProgramRole("heat_level0"));
    QVERIFY(!ClimateProgram::isProgramRole("target_temp"));
}

void tst_ClimateProgram::treeGroupsTheProgramUnderItsClimate()
{
    QVERIFY(load(ioSource(climate(6, 6))));
    QTreeWidget tree;
    populate(&tree);

    QTreeWidgetItemDevice *devItem = deviceItemOf(&tree);
    QTreeWidgetItemProgram *program = programItemOf(&tree);
    QVERIFY(devItem);
    QVERIFY(program);
    QCOMPARE(program->parent(), static_cast<QTreeWidgetItem *>(devItem));
    QCOMPARE(program->getDevice(), devItem->getDevice());
    QVERIFY(!program->isExpanded());
    QVERIFY(devItem->isExpanded());
    QCOMPARE(itemCount(program), 28);
    // The climate's other IO stays next to the program item.
    QCOMPARE(itemCount(devItem), 2);
    QVERIFY(!ioOfItem(program));

    for (int i = 0;i < program->childCount();i++)
        QVERIFY(ClimateProgram::isProgramIO(ioOfItem(program->child(i))));

    // Rebuilding the device item alone gives the same shape.
    QTreeWidgetItemRoom *room = dynamic_cast<QTreeWidgetItemRoom *>(devItem->parent());
    Device *dev = devItem->getDevice();
    delete devItem;
    QTreeWidgetItemDevice *rebuilt = addDeviceItem(room, dev);
    QCOMPARE(itemCount(rebuilt), 2);
    QCOMPARE(itemCount(programItemOf(&tree)), 28);
}

void tst_ClimateProgram::deletedLevelsLeaveTheTree()
{
    QVERIFY(load(ioSource(climate(1, 0))));
    QTreeWidget tree;
    populate(&tree);
    Device *dev = theClimate();

    // Level, setpoint, default and flag: the program item goes with its last IO.
    removeIOItems(&tree, ClimateProgram::applyLevels(dev, 0, 0));
    QVERIFY(ClimateProgram::programIos(dev).empty());
    QVERIFY(!programItemOf(&tree));
    QCOMPARE(itemCount(deviceItemOf(&tree)), 1);
}

void tst_ClimateProgram::rulesNameProgramIosAfterTheirClimate()
{
    QVERIFY(load(ioSource(climate(2, 1))));
    Device *dev = theClimate();

    QCOMPARE(ClimateProgram::ruleLabel(ClimateProgram::ioForRole(dev, "program_enabled")),
             QString::fromUtf8("Salon › Program enabled"));
    QCOMPARE(ClimateProgram::ruleLabel(ClimateProgram::ioForRole(dev, "heat_level2")),
             QString::fromUtf8("Salon › Heating level 2 active"));
    QCOMPARE(ClimateProgram::ruleLabel(ClimateProgram::ioForRole(dev, "cool_level1_setpoint")),
             QString::fromUtf8("Salon › Cooling level 1 setpoint"));
    QCOMPARE(ClimateProgram::ruleLabel(ClimateProgram::ioForRole(dev, "cool_default_off")),
             QString::fromUtf8("Salon › Cooling off by default"));
    QCOMPARE(ClimateProgram::ruleLabel(ClimateProgram::ioForRole(dev, "target_temp")), QString("target"));
    QCOMPARE(ClimateProgram::ruleLabel(ListeRoom::Instance().get_output("free")), QString("free"));
}

QTEST_MAIN(tst_ClimateProgram)
#include "tst_climateprogram.moc"
