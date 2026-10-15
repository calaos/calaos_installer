#include <QtTest>
#include <QApplication>
#include <QDir>
#include <QIcon>
#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QSettings>
#include <QSet>
#include <QStandardPaths>
#include <QTemporaryDir>

#include "HomeTree.h"
#include "projectmanager.h"
#include <IOFactory.h>
#include <ListeRoom.h>
#include <ListeRule.h>

using namespace Calaos;

namespace
{

// Types the server no longer registers; the installer must keep reading them.
const char *kLegacy[] = {
    "Gadspot", "WebInputSwitch", "X10Output",
    "MySensorsInputAnalog", "MySensorsInputString", "MySensorsInputSwitch",
    "MySensorsInputSwitchLongPress", "MySensorsInputSwitchTriple", "MySensorsInputTemp",
    "MySensorsOutputAnalog", "MySensorsOutputDimmer", "MySensorsOutputLight",
    "MySensorsOutputLightRGB", "MySensorsOutputShutter", "MySensorsOutputShutterSmart",
    "MySensorsOutputString",
    "xPLInputAnalog", "xPLInputString", "xPLInputSwitch", "xPLInputTemp",
    "xPLOutputAnalog", "xPLOutputString", "xPLOutputSwitch",
    "ZibaseAnalogIn", "ZibaseDigitalIn", "ZibaseDigitalOut", "ZibaseTemp",
};

QSet<QString> serverTypes()
{
    QFile f(QStringLiteral(SUITE_DIR "/server_io_types.txt"));
    QSet<QString> types;
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return types;
    while (!f.atEnd())
    {
        const QString line = QString::fromUtf8(f.readLine()).trimmed();
        if (!line.isEmpty() && !line.startsWith('#'))
            types.insert(line.toLower());
    }
    return types;
}

QString readFile(const QString &path)
{
    QFile f(path);
    return f.open(QIODevice::ReadOnly) ? QString::fromUtf8(f.readAll()) : QString();
}

}

class tst_ServerTypes: public QObject
{
    Q_OBJECT

    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;

    bool load(const QByteArray &rooms)
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
        QString path = m_workDir.filePath("io.xml");
        QFile f(path);
        if (!f.open(QIODevice::WriteOnly))
            return false;
        f.write("<?xml version=\"1.0\"?>\n<calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">\n"
                "<calaos:home format=\"2\">" + rooms + "</calaos:home>\n</calaos:ioconfig>\n");
        f.close();
        return ProjectManager::loadIOsFromFile(path);
    }

private slots:
    void initTestCase()
    {
        QVERIFY(m_settingsDir.isValid() && m_workDir.isValid());
        QStandardPaths::setTestModeEnabled(true);
        QCoreApplication::setOrganizationName("calaos-installer-tests");
        QCoreApplication::setApplicationName("tst_servertypes");
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
        QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());
    }

    void cleanupTestCase()
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
    }

    // The fixture is only trustworthy while the catalogue the server generated agrees with it.
    void fixtureCoversTheShippedCatalogue()
    {
        const QSet<QString> server = serverTypes();
        QVERIFY(server.size() > 50);

        const QJsonObject doc = QJsonDocument::fromJson(
            readFile(QStringLiteral(REPO_ROOT "/data/doc/en/io_doc.json")).toUtf8()).object();
        QVERIFY(!doc.isEmpty());

        QStringList missing;
        for (const QString &type: doc.keys())
        {
            // Reserved key holding the devices by impl; they are not IO types.
            if (type == "devices")
                continue;
            if (!server.contains(type.toLower()))
                missing << type;
        }
        QVERIFY2(missing.isEmpty(), qPrintable(missing.join(", ")));
    }

    void everyCreatableTypeIsRegisteredByTheServer()
    {
        const QSet<QString> server = serverTypes();
        const std::vector<std::string> creatable = IOFactory::Instance().creatableTypes();
        QVERIFY(creatable.size() > 50);

        QStringList dead;
        for (const std::string &t: creatable)
            if (!server.contains(QString::fromStdString(t)))
                dead << QString::fromStdString(t);
        QVERIFY2(dead.isEmpty(), qPrintable("not known to the server: " + dead.join(", ")));
    }

    void legacyTypesAreNotServerTypesAndNotCreatable_data()
    {
        QTest::addColumn<QString>("type");
        for (const char *t: kLegacy)
            QTest::newRow(t) << t;
    }

    void legacyTypesAreNotServerTypesAndNotCreatable()
    {
        QFETCH(QString, type);
        QVERIFY(!serverTypes().contains(type.toLower()));
        QVERIFY(IOFactory::Instance().isLegacyType(type.toStdString()));

        const auto creatable = IOFactory::Instance().creatableTypes();
        QVERIFY(std::find(creatable.begin(), creatable.end(), type.toLower().toStdString()) == creatable.end());

        // Still known to the factory, otherwise loading would degrade it to a bare "unknown" IO.
        Params p;
        p.Add("type", type.toStdString());
        p.Add("name", "x");
        p.Add("id", "io_x");
        IOBase *io = IOFactory::Instance().CreateIO(type.toStdString(), p, IOBase::IO_INPUT);
        QVERIFY(io->get_gui_type() != "unknown");
        QVERIFY(!io->isSupportedByServer());
        delete io;
    }

    void aServerTypeIsSupported()
    {
        Params p;
        p.Add("type", "MqttInputTemp");
        p.Add("id", "io_x");
        IOBase *io = IOFactory::Instance().CreateIO("MqttInputTemp", p, IOBase::IO_INPUT);
        QVERIFY(io->isSupportedByServer());
        delete io;
    }

    // Menus and dialogs spell the type they create as a string literal: scan them, so a type
    // reintroduced there cannot slip past the factory check above.
    void noSourceOffersATypeTheServerLacks()
    {
        const QSet<QString> server = serverTypes();
        // Rule actions and widget attributes also say "type"; only IO creation sites are of interest.
        static const QSet<QString> notIoFiles = { "projectmanager.cpp", "WidgetModel.cpp", "PageModel.cpp", "textedit.cpp" };
        const QRegularExpression literal(QStringLiteral("(?:\"type\"\\s*,\\s*|\\btype\\s*=\\s*)\"(\\w+)\""));

        int scanned = 0;
        QStringList dead;
        for (const QString &dir: { QStringLiteral(REPO_ROOT "/src"), QStringLiteral(REPO_ROOT "/Calaos") })
        {
            QDirIterator it(dir, { "*.cpp" }, QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                const QString path = it.next();
                const QString name = QFileInfo(path).fileName();
                if (notIoFiles.contains(name) || name == "IOBase.cpp")
                    continue;
                scanned++;

                auto m = literal.globalMatch(readFile(path));
                while (m.hasNext())
                {
                    const QString t = m.next().captured(1);
                    if (!server.contains(t.toLower()))
                        dead << name + ": " + t;
                }
            }
        }
        QVERIFY(scanned > 30);
        QVERIFY2(dead.isEmpty(), qPrintable(dead.join("\n")));
    }

    void existingLegacyIoIsKeptInTheModelAndTheFile()
    {
        QVERIFY(load(R"(
<calaos:room name="A" type="salon" hits="1">
  <calaos:input id="in_ms" type="MySensorsInputTemp" name="ms temp" node_id="3" sensor_id="1" gateway="serial"/>
  <calaos:camera id="out_g" type="Gadspot" name="cam" host="10.0.0.2"/>
  <calaos:input id="in_ok" type="MqttInputTemp" name="mqtt temp" topic_sub="a/b"/>
</calaos:room>)"));

        Room *room = ListeRoom::Instance().get_room(0);
        QVERIFY(room);
        QVERIFY(ListeRoom::Instance().get_input("in_ms"));
        QVERIFY(ListeRoom::Instance().get_output("out_g"));
        QVERIFY(ListeRoom::Instance().get_input("in_ok"));

        QString file = m_workDir.filePath("saved.xml");
        QVERIFY(ProjectManager::saveIOsToFile(file));
        const QString saved = readFile(file);
        QVERIFY(saved.contains("type=\"MySensorsInputTemp\""));
        QVERIFY(saved.contains("node_id=\"3\""));
        QVERIFY(saved.contains("gateway=\"serial\""));
        QVERIFY(saved.contains("type=\"Gadspot\""));
        QVERIFY(saved.contains("host=\"10.0.0.2\""));
    }

    void treeMarksUnsupportedIosAndLeavesTheOthers()
    {
        QVERIFY(load(R"(
<calaos:room name="A" type="salon" hits="1">
  <calaos:input id="in_ms" type="MySensorsInputTemp" name="ms temp"/>
  <calaos:input id="in_ok" type="MqttInputTemp" name="mqtt temp"/>
</calaos:room>)"));

        QTreeWidget tree;
        auto *roomItem = new QTreeWidgetItemRoom(ListeRoom::Instance().get_room(0), &tree);
        populateRoomItems(roomItem);
        QCOMPARE(roomItem->childCount(), 2);

        int marked = 0;
        for (int i = 0;i < roomItem->childCount();i++)
        {
            QTreeWidgetItem *item = roomItem->child(i);
            item->setData(0, Qt::ToolTipRole, QStringLiteral("tip"));
            markUnsupportedByServer(item);

            const bool legacy = ioOfItem(item)->get_param("type") == "MySensorsInputTemp";
            const QString tip = item->data(0, Qt::ToolTipRole).toString();
            QCOMPARE(tip.startsWith("tip"), true);
            QCOMPARE(tip.contains("Not supported by the server"), legacy);
            QCOMPARE(item->data(0, Qt::StatusTipRole).toString() == tip, legacy);
            QCOMPARE(item->data(0, Qt::ForegroundRole).isValid(), legacy);
            // The resource must exist, or the icon silently vanishes.
            if (legacy)
                QVERIFY(!item->data(0, Qt::DecorationRole).value<QIcon>().isNull());
            marked += legacy;
        }
        QCOMPARE(marked, 1);
    }
};

QTEST_MAIN(tst_ServerTypes)
#include "tst_servertypes.moc"
