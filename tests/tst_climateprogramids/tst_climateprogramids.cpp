#include <QtTest>
#include <QApplication>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSettings>
#include <QSpinBox>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTimer>
#include <QTreeWidget>
#include <functional>
#include <set>

#include "ClimateProgram.h"
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

// Every program role of a climate with these level counts.
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
    return l;
}

// The id the server gives the owned IO of a role: a taken id gets a suffix.
using IdFn = std::function<QString(const QString &dev, const QString &role)>;

QString plainId(const QString &dev, const QString &role)
{
    return dev + "_" + role;
}

IdFn suffixed(const QString &suffix)
{
    return [suffix](const QString &dev, const QString &role) { return plainId(dev, role) + suffix; };
}

struct Fixture
{
    int heat = 0;
    int cool = 0;
    QString impl = "thermostat";
    QString dev = "clim";
    QString name = "Salon";
    IdFn id = plainId;
    // Proxy climates also own the device-facing IOs and link a command IO.
    bool proxy = false;
    // Raw XML of IOs of the room that are not the climate's.
    QString houseIos;
};

QString ownedIo(const QString &id, const QString &role, const QString &name, int seed)
{
    if (role.endsWith("_default_off") || role == "program_enabled")
        return QStringLiteral("    <calaos:internal id=\"%1\" type=\"InternalBool\" name=\"%2\" save=\"true\" value=\"%3\" visible=\"false\"/>\n")
            .arg(id, name, seed % 2 ? "true" : "false");
    if (role.contains("_level") && !role.endsWith("_setpoint"))
        return QStringLiteral(
            "    <calaos:input id=\"%1\" type=\"InPlageHoraire\" name=\"%2\" visible=\"false\" months=\"111111111111\">\n"
            "      <calaos:lundi><calaos:plage start_hour=\"%3\" start_min=\"0\" start_sec=\"0\" start_type=\"0\" start_offset=\"1\""
            " end_hour=\"%3\" end_min=\"30\" end_sec=\"0\" end_type=\"0\" end_offset=\"1\"/></calaos:lundi>\n"
            "    </calaos:input>\n").arg(id, name).arg(seed % 20);
    return QStringLiteral("    <calaos:internal id=\"%1\" type=\"InternalInt\" name=\"%2\" save=\"true\" value=\"%3\""
                          " value_min=\"7\" value_max=\"30\" step=\"0.5\" visible=\"false\"/>\n")
        .arg(id, name).arg(10 + seed % 15);
}

QString climate(const Fixture &f)
{
    QString ios, links;
    const QStringList roles = programRoles(f.heat, f.cool);
    for (int i = 0;i < roles.size();i++)
    {
        const QString id = f.id(f.dev, roles[i]);
        ios += ownedIo(id, roles[i], f.name + " " + roles[i], i + 1);
        links += QStringLiteral("    <calaos:link name=\"%1\" io=\"%2\"/>\n").arg(roles[i], id);
    }

    QString params;
    if (f.proxy)
    {
        params = "    <calaos:param name=\"topic_prefix\" value=\"esphome/salon\"/>\n"
                 "    <calaos:param name=\"object_id\" value=\"salon_clim\"/>\n";
        ios += QStringLiteral("    <calaos:internal id=\"%1_mode\" type=\"InternalInt\" name=\"mode\" visible=\"false\"/>\n"
                              "    <calaos:internal id=\"%1_mode_cmd\" type=\"InternalInt\" name=\"mode cmd\" visible=\"false\"/>\n")
                   .arg(f.dev);
        links += QStringLiteral("    <calaos:link name=\"mode\" io=\"%1_mode\" command_io=\"%1_mode_cmd\"/>\n"
                                "    <calaos:link name=\"remote_temp\" io=\"temp\"/>\n")
                     .arg(f.dev);
    }

    return QStringLiteral(
        "<calaos:room name=\"A\" type=\"salon\" hits=\"1\">\n"
        "  <calaos:input id=\"temp\" type=\"WITemp\" name=\"temp\" host=\"h\" port=\"502\" var=\"0\"/>\n"
        "  <calaos:internal id=\"free\" type=\"InternalBool\" name=\"free\" visible=\"false\"/>\n"
        "%1"
        "  <calaos:device id=\"%2\" type=\"climate\" impl=\"%3\" name=\"%4\">\n"
        "    <calaos:param name=\"heat_levels\" value=\"%5\"/>\n"
        "    <calaos:param name=\"cool_levels\" value=\"%6\"/>\n"
        "%7"
        "    <calaos:internal id=\"%2_target\" type=\"InternalInt\" name=\"target\" visible=\"false\"/>\n"
        "%8"
        "    <calaos:link name=\"current_temp\" io=\"temp\"/>\n"
        "    <calaos:link name=\"target_temp\" io=\"%2_target\"/>\n"
        "%9"
        "  </calaos:device>\n"
        "</calaos:room>")
        .arg(f.houseIos, f.dev, f.impl, f.name).arg(f.heat).arg(f.cool).arg(params, ios, links);
}

Device *climateOf(const QString &id = "clim")
{
    return dynamic_cast<Device *>(ListeRoom::Instance().get_output(id.toStdString()));
}

// Each program role of the device is its own IO and carries the id the file gave it.
void checkProgram(Device *dev, int heat, int cool, const IdFn &id, const QString &devId = "clim")
{
    QCOMPARE(ClimateProgram::levels(dev, Set::Heat), heat);
    QCOMPARE(ClimateProgram::levels(dev, Set::Cool), cool);
    const QStringList present = programRoles(heat, cool);
    std::set<IOBase *> seen;
    for (Set set: { Set::Heat, Set::Cool })
    {
        QStringList all = { u(ClimateProgram::defaultRole(set)) };
        for (int n = 1;n <= ClimateProgram::kMaxLevels;n++)
            all << u(ClimateProgram::levelRole(set, n)) << u(ClimateProgram::setpointRole(set, n));
        all << "cool_default_off" << "program_enabled";
        for (const QString &role: all)
        {
            IOBase *io = ClimateProgram::ioForRole(dev, role.toStdString());
            const QByteArray tag = role.toUtf8();
            QCOMPARE_EQ(io != nullptr, present.contains(role));
            if (!io)
                continue;
            QVERIFY2(!seen.count(io) || role == "program_enabled" || role == "cool_default_off", tag.constData());
            seen.insert(io);
            QCOMPARE(u(io->get_param("id")), id(devId, role));
            QCOMPARE(ClimateProgram::roleOf(io), role.toStdString());
        }
    }
}

std::set<std::string> allIds()
{
    std::set<std::string> ids;
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
    {
        Room *room = lr.get_room(r);
        for (int i = 0;i < room->get_size_in();i++)
            ids.insert(room->get_input(i)->get_param("id"));
        for (int i = 0;i < room->get_size_out();i++)
            ids.insert(room->get_output(i)->get_param("id"));
    }
    return ids;
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

QString ruleOn(const QString &name, const QString &inputId)
{
    return QStringLiteral(
        "<calaos:rule name=\"%1\" type=\"t\">\n"
        "  <calaos:condition><calaos:input id=\"%2\" oper=\"==\" val=\"true\"/></calaos:condition>\n"
        "  <calaos:action><calaos:output id=\"free\" val=\"true\"/></calaos:action>\n"
        "</calaos:rule>\n").arg(name, inputId);
}

}

class tst_ClimateProgramIds: public QObject
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

    // Ids of the program IOs
    void suffixedIdsAreFoundThroughTheirLink();
    void loweringSparesAHouseIoHoldingTheFreedId();
    void rolesNotIdPatternsDecideWhatIsRemoved();
    void linkToAHouseIoIsNotAProgramIo();
    void aGapInTheLevelsIsNotRenumbered();
    void twoClimatesKeepTheirOwnIds();
    void aDuplicatedIdIsReported();
    void randomSuffixesSurviveARoundTrip();
    void loweringNeverTouchesTheKeptIds();

    // Proxy climates
    void supportsFollowsFamilyAndImpl_data();
    void supportsFollowsFamilyAndImpl();
    void proxyRoundTripKeepsTheDeviceAndItsProgram();
    void proxyLoweringKeepsTheProxyIos();
};

void tst_ClimateProgramIds::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());

    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_climateprogramids");
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

// The server never rebuilds an id from the device id: a taken one gets `_N`.
// A house IO holds the plain ids here.
void tst_ClimateProgramIds::suffixedIdsAreFoundThroughTheirLink()
{
    Fixture f;
    f.heat = 2;
    f.id = suffixed("_0");
    f.houseIos =
        "  <calaos:internal id=\"clim_heat_level1\" type=\"InternalBool\" name=\"decoy1\"/>\n"
        "  <calaos:internal id=\"clim_heat_level2_setpoint\" type=\"InternalInt\" name=\"decoy2\"/>\n"
        "  <calaos:internal id=\"clim_program_enabled\" type=\"InternalBool\" name=\"decoy3\"/>\n";
    QVERIFY(load(ioSource(climate(f))));
    Device *dev = climateOf();
    QVERIFY(dev);
    checkProgram(dev, 2, 0, f.id);
    QCOMPARE(int(ClimateProgram::programIos(dev).size()), 2 * 2 + 1 + 1);

    for (const char *id: { "clim_heat_level1", "clim_heat_level2_setpoint", "clim_program_enabled" })
    {
        IOBase *decoy = ListeRoom::Instance().get_output(id);
        if (!decoy)
            decoy = ListeRoom::Instance().get_input(id);
        QVERIFY2(decoy, id);
        QVERIFY2(!ClimateProgram::isProgramIO(decoy), id);
        QVERIFY2(ClimateProgram::canDeleteAlone(decoy), id);
        QVERIFY2(ClimateProgram::roleOf(decoy).empty(), id);
        // Rules name it as any house IO, not after the climate.
        QVERIFY2(!ClimateProgram::ruleLabel(decoy).contains(QString::fromUtf8("›")), id);
    }
    QCOMPARE(ClimateProgram::ruleLabel(ClimateProgram::ioForRole(dev, "program_enabled")),
             QString::fromUtf8("Salon › Program enabled"));
}

void tst_ClimateProgramIds::loweringSparesAHouseIoHoldingTheFreedId()
{
    Fixture f;
    f.heat = 3;
    f.id = suffixed("_1");
    f.houseIos =
        "  <calaos:internal id=\"clim_heat_level2\" type=\"InternalBool\" name=\"decoy\"/>\n"
        "  <calaos:internal id=\"clim_heat_level3_setpoint\" type=\"InternalInt\" name=\"decoy sp\"/>\n";
    const QString rules = ruleOn("r_decoy", "clim_heat_level2") +
                          ruleOn("r_owned2", "clim_heat_level2_1") +
                          ruleOn("r_owned1", "clim_heat_level1_1") +
                          ruleOn("r_decoy_sp", "clim_heat_level3_setpoint");
    QVERIFY(load(ioSource(climate(f)), rulesSource(rules)));
    Device *dev = climateOf();
    QVERIFY(dev);

    ClimateProgram::applyLevels(dev, 1, 0);

    checkProgram(dev, 1, 0, f.id);
    QVERIFY(ListeRoom::Instance().get_input("clim_heat_level2"));
    QVERIFY(ListeRoom::Instance().get_output("clim_heat_level3_setpoint"));
    QVERIFY(!ListeRoom::Instance().get_input("clim_heat_level2_1"));
    QVERIFY(!ListeRoom::Instance().get_input("clim_heat_level3_1"));
    QCOMPARE(ruleNames(), QStringList({ "r_decoy", "r_decoy_sp", "r_owned1" }));
    // No dangling reference left to hold the freed ids.
    QVERIFY(!ListeRoom::isPendingIOId("clim_heat_level2_1"));
    QVERIFY(!ListeRoom::isPendingIOId("clim_heat_level3_1"));

    // The decoys come out of the file untouched.
    const QByteArray written = saveIOs();
    QVERIFY(written.contains("name=\"decoy\""));
    QVERIFY(written.contains("name=\"decoy sp\""));
    QVERIFY(!written.contains("clim_heat_level2_1"));
}

// Ids are labels: level 2's IO took the id of level 5 and the other way round.
void tst_ClimateProgramIds::rolesNotIdPatternsDecideWhatIsRemoved()
{
    Fixture f;
    f.heat = 5;
    f.id = [](const QString &dev, const QString &role)
    {
        if (role == "heat_level2")
            return dev + "_heat_level5";
        if (role == "heat_level5")
            return dev + "_heat_level2";
        return plainId(dev, role);
    };
    QVERIFY(load(ioSource(climate(f)),
                 rulesSource(ruleOn("r_id2", "clim_heat_level2") + ruleOn("r_id5", "clim_heat_level5"))));
    Device *dev = climateOf();
    QVERIFY(dev);
    IOBase *role2 = ClimateProgram::ioForRole(dev, "heat_level2");
    QCOMPARE(role2->get_param("id"), std::string("clim_heat_level5"));

    ClimateProgram::applyLevels(dev, 3, 0);

    QCOMPARE(ClimateProgram::ioForRole(dev, "heat_level2"), role2);
    QVERIFY(ListeRoom::Instance().get_input("clim_heat_level5"));
    // role 5 held the id of level 2: its rule goes, the rule on the kept IO stays.
    QVERIFY(!ListeRoom::Instance().get_input("clim_heat_level2"));
    QCOMPARE(ruleNames(), QStringList({ "r_id5" }));
    QVERIFY(!ClimateProgram::ioForRole(dev, "heat_level5"));
}

void tst_ClimateProgramIds::linkToAHouseIoIsNotAProgramIo()
{
    Fixture f;
    f.heat = 2;
    QString xml = climate(f);
    // Level 2 is linked to an IO of the room instead of an owned one; its own IO is gone.
    xml.replace("<calaos:link name=\"heat_level2\" io=\"clim_heat_level2\"/>",
                "<calaos:link name=\"heat_level2\" io=\"free\"/>");
    QVERIFY(load(ioSource(xml)));
    Device *dev = climateOf();
    QVERIFY(dev);
    IOBase *house = ListeRoom::Instance().get_output("free");
    QVERIFY(house);

    QVERIFY(!ClimateProgram::ioForRole(dev, "heat_level2"));
    QVERIFY(ClimateProgram::canDeleteAlone(house));
    QVERIFY(ClimateProgram::roleOf(house).empty());

    ClimateProgram::applyLevels(dev, 1, 0);
    QVERIFY(ListeRoom::Instance().get_output("free"));
    QVERIFY(!linkNames(dev).contains("heat_level2"));
    QVERIFY(!linkNames(dev).contains("heat_level2_setpoint"));
}

// The server counts levels, it does not compact them: a missing level 2 of 3
// leaves level 3 where it is, and lowering to 2 only removes level 3.
void tst_ClimateProgramIds::aGapInTheLevelsIsNotRenumbered()
{
    Fixture f;
    f.heat = 3;
    QString xml = climate(f);
    xml.remove(QRegularExpression("    <calaos:input id=\"clim_heat_level2\".*?</calaos:input>\n",
                                  QRegularExpression::DotMatchesEverythingOption));
    xml.remove("    <calaos:link name=\"heat_level2\" io=\"clim_heat_level2\"/>\n");
    QVERIFY(load(ioSource(xml)));
    Device *dev = climateOf();
    QVERIFY(dev);
    IOBase *level3 = ClimateProgram::ioForRole(dev, "heat_level3");
    IOBase *level1 = ClimateProgram::ioForRole(dev, "heat_level1");
    QVERIFY(level3 && level1);
    QVERIFY(!ClimateProgram::ioForRole(dev, "heat_level2"));

    QCOMPARE(int(ClimateProgram::iosRemovedBy(dev, 2, 0).size()), 2);
    ClimateProgram::applyLevels(dev, 2, 0);

    QCOMPARE(ClimateProgram::ioForRole(dev, "heat_level1"), level1);
    QVERIFY(!ClimateProgram::ioForRole(dev, "heat_level2"));
    QVERIFY(!ClimateProgram::ioForRole(dev, "heat_level3"));
    QVERIFY(ClimateProgram::ioForRole(dev, "heat_level2_setpoint"));
    QCOMPARE(level1->get_param("id"), std::string("clim_heat_level1"));
}

void tst_ClimateProgramIds::twoClimatesKeepTheirOwnIds()
{
    Fixture a;
    a.heat = 3;
    a.cool = 1;
    Fixture b;
    b.dev = "clim2";
    b.name = "Chambre";
    b.heat = 3;
    b.cool = 1;
    // B's ids are suffixed as a second climate of the house gets them.
    b.id = suffixed("_0");
    const QString both = climate(a) + "\n" + QString(climate(b)).replace("<calaos:room name=\"A\"", "<calaos:room name=\"B\"")
                                                         .replace("id=\"temp\"", "id=\"temp2\"")
                                                         .replace("io=\"temp\"", "io=\"temp2\"")
                                                         .replace("id=\"free\"", "id=\"free2\"");
    QVERIFY(load(ioSource(both)));
    Device *devA = climateOf("clim");
    Device *devB = climateOf("clim2");
    QVERIFY(devA && devB);
    checkProgram(devA, 3, 1, plainId, "clim");
    checkProgram(devB, 3, 1, b.id, "clim2");
    QVERIFY(ClimateProgram::ioForRole(devA, "heat_level1") != ClimateProgram::ioForRole(devB, "heat_level1"));
    const size_t programB = ClimateProgram::programIos(devB).size();

    ClimateProgram::applyLevels(devA, 0, 0);

    // No level left: the defaults and the flags go with them.
    QCOMPARE(int(ClimateProgram::programIos(devA).size()), 0);
    QCOMPARE(ClimateProgram::programIos(devB).size(), programB);
    checkProgram(devB, 3, 1, b.id, "clim2");
    QVERIFY(ListeRoom::Instance().get_input("clim2_heat_level1_0"));
    QVERIFY(!ListeRoom::Instance().get_input("clim_heat_level1"));
}

// An owned IO taking the id of a house IO is the one broken case: the reader says so.
void tst_ClimateProgramIds::aDuplicatedIdIsReported()
{
    Fixture f;
    f.heat = 1;
    f.houseIos = "  <calaos:internal id=\"clim_heat_level1_setpoint\" type=\"InternalInt\" name=\"house\"/>\n";
    QVERIFY(load(ioSource(climate(f))));
    QCOMPARE(m_dialogs.size(), 1);
    QVERIFY2(m_dialogs.first().contains("clim_heat_level1_setpoint"), qPrintable(m_dialogs.first()));
    QVERIFY(m_dialogs.first().contains("already exists"));
    m_dialogs.clear();

    // The program still resolves to the owned IO of the device, never to the house one.
    Device *dev = climateOf();
    QVERIFY(dev);
    IOBase *sp = ClimateProgram::ioForRole(dev, "heat_level1_setpoint");
    QVERIFY(sp);
    QVERIFY(sp->getOwnerDevice() == dev);
}

namespace
{

// Random suffix per role, from a fixed seed: none, or `_N` with N below 12.
QHash<QString, QString> randomIds(QRandomGenerator &rng, int heat, int cool)
{
    QHash<QString, QString> ids;
    for (const QString &role: programRoles(heat, cool))
    {
        const quint32 n = rng.bounded(14u);
        ids[role] = "clim_" + role + (n < 2 ? QString() : "_" + QString::number(n - 2));
    }
    return ids;
}

}

void tst_ClimateProgramIds::randomSuffixesSurviveARoundTrip()
{
    for (quint32 seed = 1;seed <= 24;seed++)
    {
        QRandomGenerator rng(seed);
        const int heat = rng.bounded(11), cool = rng.bounded(7);
        const QHash<QString, QString> ids = randomIds(rng, heat, cool);
        Fixture f;
        f.heat = heat;
        f.cool = cool;
        f.id = [ids](const QString &, const QString &role) { return ids.value(role); };
        const QByteArray tag = "seed " + QByteArray::number(seed);

        QVERIFY2(load(ioSource(climate(f))), tag.constData());
        QVERIFY2(m_dialogs.isEmpty(), tag.constData());
        checkProgram(climateOf(), heat, cool, f.id);
        if (QTest::currentTestFailed())
            return;

        const QByteArray written = saveIOs();
        QVERIFY2(!written.isEmpty(), tag.constData());
        QVERIFY2(load(written), tag.constData());
        checkProgram(climateOf(), heat, cool, f.id);
        if (QTest::currentTestFailed())
            return;
        QCOMPARE(saveIOs(), written);
    }
}

void tst_ClimateProgramIds::loweringNeverTouchesTheKeptIds()
{
    for (quint32 seed = 100;seed <= 124;seed++)
    {
        QRandomGenerator rng(seed);
        const int heat = 1 + rng.bounded(10), cool = rng.bounded(11);
        const QHash<QString, QString> ids = randomIds(rng, heat, cool);
        const int heat2 = rng.bounded(heat + 1), cool2 = rng.bounded(cool + 1);
        Fixture f;
        f.heat = heat;
        f.cool = cool;
        f.id = [ids](const QString &, const QString &role) { return ids.value(role); };
        const QByteArray tag = "seed " + QByteArray::number(seed);

        QVERIFY2(load(ioSource(climate(f))), tag.constData());
        Device *dev = climateOf();
        const std::set<std::string> before = allIds();
        const size_t programBefore = ClimateProgram::programIos(dev).size();
        const std::vector<IOBase *> gone = ClimateProgram::applyLevels(dev, heat2, cool2);

        const std::set<std::string> after = allIds();
        QVERIFY2(before.size() - after.size() == gone.size(), tag.constData());
        QVERIFY2(std::includes(before.begin(), before.end(), after.begin(), after.end()), tag.constData());
        for (const QString &role: programRoles(heat, cool))
        {
            const std::string id = ids.value(role).toStdString();
            const QRegularExpressionMatch m = QRegularExpression("^(heat|cool)_level(10|[1-9])").match(role);
            const bool removed =
                (m.hasMatch() && m.captured(2).toInt() > (m.captured(1) == "heat" ? heat2 : cool2)) ||
                (role == "heat_default" && heat2 == 0) || (role == "cool_default" && cool2 == 0) ||
                (role == "cool_default_off" && cool2 == 0) ||
                (role == "program_enabled" && heat2 + cool2 == 0);
            QVERIFY2(after.count(id) == (removed ? 0u : 1u), qPrintable(tag + " " + role));
        }
        QVERIFY2(ClimateProgram::programIos(dev).size() == programBefore - gone.size(), tag.constData());

        const QByteArray written = saveIOs();
        QVERIFY2(load(written), tag.constData());
        dev = climateOf();
        QCOMPARE(ClimateProgram::levels(dev, Set::Heat), heat2);
        QCOMPARE(ClimateProgram::levels(dev, Set::Cool), cool2);
        QVERIFY2(ClimateProgram::programIos(dev).size() == programBefore - gone.size(), tag.constData());
        for (const QString &role: programRoles(heat2, cool2))
        {
            IOBase *io = ClimateProgram::ioForRole(dev, role.toStdString());
            QVERIFY2(io, qPrintable(tag + " " + role));
            QCOMPARE(u(io->get_param("id")), ids.value(role));
        }
    }
}

void tst_ClimateProgramIds::supportsFollowsFamilyAndImpl_data()
{
    QTest::addColumn<QString>("family");
    QTest::addColumn<QString>("impl");
    QTest::addColumn<bool>("supported");

    QTest::newRow("thermostat") << "climate" << "thermostat" << true;
    QTest::newRow("thermostat upper") << "climate" << "THERMOSTAT" << true;
    QTest::newRow("proxy") << "climate" << "proxy" << true;
    QTest::newRow("proxy mixed case") << "climate" << "Proxy" << true;
    QTest::newRow("pid") << "climate" << "pid" << false;
    QTest::newRow("no impl") << "climate" << "" << false;
    QTest::newRow("proxy with a trailing blank") << "climate" << "proxy " << false;
    QTest::newRow("proxy prefix only") << "climate" << "prox" << false;
    QTest::newRow("proxy suffix") << "climate" << "proxy2" << false;
    QTest::newRow("template name is not an impl") << "climate" << "esphome_climate" << false;
    QTest::newRow("other family, proxy impl") << "light" << "proxy" << false;
    QTest::newRow("other family, thermostat impl") << "cover" << "thermostat" << false;
    QTest::newRow("no family") << "" << "proxy" << false;
}

void tst_ClimateProgramIds::supportsFollowsFamilyAndImpl()
{
    QFETCH(QString, family);
    QFETCH(QString, impl);
    QFETCH(bool, supported);

    Params p;
    p.Add("id", "c");
    p.Add("name", "c");
    p.Add("type", family.toStdString());
    p.Add("impl", impl.toStdString());
    Device dev(p);
    QCOMPARE(ClimateProgram::supports(&dev), supported);
    QVERIFY(!ClimateProgram::supports(nullptr));

    // The same from a file: what is read decides, not what the tree shows.
    Fixture f;
    f.heat = 1;
    f.impl = impl;
    QString xml = climate(f);
    xml.replace("type=\"climate\"", "type=\"" + family + "\"");
    QVERIFY(load(ioSource(xml)));
    Device *dev2 = climateOf();
    QVERIFY(dev2);
    QCOMPARE(ClimateProgram::supports(dev2), supported);
    QCOMPARE(int(ClimateProgram::programIos(dev2).size()), supported ? 4 : 0);
}

void tst_ClimateProgramIds::proxyRoundTripKeepsTheDeviceAndItsProgram()
{
    Fixture f;
    f.heat = 3;
    f.cool = 2;
    f.impl = "proxy";
    f.proxy = true;
    f.id = suffixed("_2");
    QVERIFY(load(ioSource(climate(f))));
    QVERIFY(m_dialogs.isEmpty());
    Device *dev = climateOf();
    QVERIFY(dev);
    QVERIFY(ClimateProgram::supports(dev));
    checkProgram(dev, 3, 2, f.id);
    QCOMPARE(int(ClimateProgram::programIos(dev).size()), (3 + 2) * 2 + 2 + 1 + 1);
    // The proxy's own IOs are not program IOs and can be deleted alone.
    QVERIFY(ClimateProgram::canDeleteAlone(ListeRoom::Instance().get_output("clim_mode")));
    QVERIFY(ClimateProgram::canDeleteAlone(ListeRoom::Instance().get_output("clim_mode_cmd")));
    QVERIFY(ClimateProgram::roleOf(ListeRoom::Instance().get_output("clim_mode")).empty());

    const QByteArray written = saveIOs();
    QVERIFY(written.contains("impl=\"proxy\""));
    QVERIFY(written.contains("command_io=\"clim_mode_cmd\""));
    QVERIFY(written.contains("name=\"topic_prefix\" value=\"esphome/salon\""));

    QVERIFY(load(written));
    dev = climateOf();
    QVERIFY(dev);
    checkProgram(dev, 3, 2, f.id);
    QCOMPARE(u(ClimateProgram::param(dev, "object_id", "")), QString("salon_clim"));
    QStringList expected = programRoles(3, 2) << "current_temp" << "target_temp" << "mode" << "remote_temp";
    expected.sort();
    QCOMPARE(linkNames(dev), expected);
    QCOMPARE(saveIOs(), written);
}

// Same program, same outcome on a thermostat and on a proxy; the proxy's own
// IOs, links and command IO stay.
void tst_ClimateProgramIds::proxyLoweringKeepsTheProxyIos()
{
    Fixture f;
    f.heat = 4;
    f.cool = 4;
    f.impl = "proxy";
    f.proxy = true;
    const QString rules = ruleOn("r_gone", "clim_cool_level4") + ruleOn("r_mode", "clim_mode") +
                          ruleOn("r_kept", "clim_heat_level2");

    Fixture t = f;
    t.impl = "thermostat";
    t.proxy = false;
    QVERIFY(load(ioSource(climate(t))));
    const size_t thermostatRemoved = ClimateProgram::iosRemovedBy(climateOf(), 2, 0).size();
    // heat levels 3..4 and cool levels 1..4, each with its setpoint, then the
    // cool set's default and off flag (cool is now empty)
    QCOMPARE(int(thermostatRemoved), 2 * 2 + 4 * 2 + 2);

    QVERIFY(load(ioSource(climate(f)), rulesSource(rules)));
    Device *dev = climateOf();
    QVERIFY(dev);
    QCOMPARE(ClimateProgram::iosRemovedBy(dev, 2, 0).size(), thermostatRemoved);
    IOBase *mode = ListeRoom::Instance().get_output("clim_mode");
    QVERIFY(mode);

    ClimateProgram::applyLevels(dev, 2, 0);

    QCOMPARE(ClimateProgram::levels(dev, Set::Heat), 2);
    QCOMPARE(ClimateProgram::levels(dev, Set::Cool), 0);
    QCOMPARE(int(ClimateProgram::programIos(dev).size()), 2 * 2 + 1 + 1);
    QCOMPARE(ListeRoom::Instance().get_output("clim_mode"), mode);
    QVERIFY(ListeRoom::Instance().get_output("clim_mode_cmd"));
    QCOMPARE(ruleNames(), QStringList({ "r_kept", "r_mode" }));
    QStringList expected = programRoles(2, 0);
    expected << "current_temp" << "target_temp" << "mode" << "remote_temp";
    expected.sort();
    QCOMPARE(linkNames(dev), expected);
    for (const DeviceLink &l: dev->links())
        if (l.name == "mode")
            QCOMPARE(l.commandIo, std::string("clim_mode_cmd"));
    QCOMPARE(u(ClimateProgram::param(dev, "topic_prefix", "")), QString("esphome/salon"));
}

QTEST_MAIN(tst_ClimateProgramIds)
#include "tst_climateprogramids.moc"
