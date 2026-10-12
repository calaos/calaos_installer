#include <QtTest>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTimer>

#include "HomeTreeMove.h"
#include "qtreewidget_addition.h"
#include "projectmanager.h"

using namespace Calaos;

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

QByteArray ioSource(const QString &inner, const QString &homeAttrs = QString())
{
    return ("<?xml version=\"1.0\"?>\n"
            "<calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">\n<calaos:home" + homeAttrs + ">\n" +
            inner + "\n</calaos:home>\n</calaos:ioconfig>\n").toUtf8();
}

QByteArray rulesSource(const QString &inner)
{
    return ("<?xml version=\"1.0\"?>\n<calaos:rules xmlns:calaos=\"http://www.calaos.fr\">\n" +
            inner + "\n</calaos:rules>\n").toUtf8();
}

void resetModel()
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();
}

// One IO of every kind that lives in the model, in room A; B and C are empty.
const char *kProject = R"(
<calaos:room name="A" type="salon" hits="1">
  <calaos:input id="in_temp" type="WITemp" name="temp" host="h" port="502" var="0"/>
  <calaos:output id="out_dig" type="WODigital" name="light" host="h" port="502" var="1"/>
  <calaos:internal id="int_bool" type="InternalBool" name="ib" visible="false"/>
  <calaos:internal id="int_int" type="InternalInt" name="ii" visible="false"/>
  <calaos:internal id="int_str" type="InternalString" name="is" visible="false"/>
  <calaos:input id="scn" type="scenario" name="scn" visible="true"/>
  <calaos:input id="timer" type="InputTimer" name="timer" host="h" hour="0" min="0" msec="0" sec="6" port="502" var="0"/>
  <calaos:camera id="cam_i_cam_o" iid="cam_i" oid="cam_o" type="planet" name="cam" host="h" port="80" model="ICA-300"/>
  <calaos:audio id="aud" iid="aud_i" oid="aud_o" type="slim" name="aud" host="h" port="9090" pid="0"/>
  <calaos:avr id="avr" type="AVReceiver" name="avr" host="h" model="pioneer"/>
  <calaos:device id="dev" type="climate" impl="thermostat" name="dev">
    <calaos:internal id="dev_target" type="InternalInt" name="target" visible="false"/>
    <calaos:input id="dev_in" type="WITemp" name="in" host="h" port="502" var="1" visible="false"/>
    <calaos:output id="dev_out" type="WODigital" name="out" host="h" port="502" var="2" visible="false"/>
    <calaos:link name="current_temp" io="in_temp"/>
    <calaos:link name="output_heat" io="dev_out"/>
  </calaos:device>
</calaos:room>
<calaos:room name="B" type="salon" hits="1"/>
<calaos:room name="C" type="salon" hits="1"/>)";

const char *kRules = R"(
<calaos:rule name="r_internal" type="t">
  <calaos:condition><calaos:input id="int_bool" oper="==" val="true"/></calaos:condition>
  <calaos:action><calaos:output id="out_dig" val="true"/></calaos:action>
</calaos:rule>
<calaos:rule name="r_device" type="t">
  <calaos:condition><calaos:input id="in_temp" oper="&gt;" val="1"/></calaos:condition>
  <calaos:action><calaos:output id="dev" val="heat"/></calaos:action>
  <calaos:action><calaos:output id="dev_out" val="true"/></calaos:action>
</calaos:rule>
<calaos:rule name="r_scenario" type="t">
  <calaos:condition><calaos:input id="scn" oper="==" val="true"/></calaos:condition>
  <calaos:action><calaos:output id="cam_o" val="true"/></calaos:action>
</calaos:rule>)";

// Every room's lists, one line per (IO, list): the kind of an IO is the lists it is in.
QStringList dump()
{
    QStringList l;
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
    {
        Room *room = lr.get_room(r);
        for (int i = 0;i < room->get_size_in();i++)
            l << u(room->get_name()) + ":" + u(room->get_input(i)->get_param("id")) + ":in";
        for (int i = 0;i < room->get_size_out();i++)
            l << u(room->get_name()) + ":" + u(room->get_output(i)->get_param("id")) + ":out";
    }
    l.sort();
    return l;
}

// `base` with these IOs standing in `room` instead.
QStringList dumpWith(const QStringList &base, const QStringList &ioIds, const QString &room)
{
    QStringList l;
    for (const QString &line: base)
    {
        QStringList f = line.split(':');
        if (f.size() == 3 && ioIds.contains(f[1]))
            f[0] = room;
        l << f.join(':');
    }
    l.sort();
    return l;
}

// What FormRules::PopulateRoomsTree builds, minus icons and labels.
void populate(QTreeWidget *tree)
{
    tree->clear();
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
    {
        Room *room = lr.get_room(r);
        QTreeWidgetItemRoom *iroom = new QTreeWidgetItemRoom(room, tree);
        for (int i = 0;i < room->get_size_in();i++)
            if (inputHasTreeItem(room->get_input(i)))
                new QTreeWidgetItemInput(room->get_input(i), iroom);
        for (int i = 0;i < room->get_size_out();i++)
            new QTreeWidgetItemOutput(room->get_output(i), iroom);
    }
    tree->expandAll();
}

QStringList treeDump(QTreeWidget *tree)
{
    QStringList l;
    for (QTreeWidgetItemIterator it(tree);*it;++it)
    {
        QTreeWidgetItem *parent = (*it)->parent();
        QTreeWidgetItemRoom *room = dynamic_cast<QTreeWidgetItemRoom *>(parent);
        if (!room)
            continue;

        if (QTreeWidgetItemInput *in = dynamic_cast<QTreeWidgetItemInput *>(*it))
            l << u(room->getRoom()->get_name()) + ":" + u(in->getInput()->get_param("id")) + ":in";
        else if (QTreeWidgetItemOutput *out = dynamic_cast<QTreeWidgetItemOutput *>(*it))
            l << u(room->getRoom()->get_name()) + ":" + u(out->getOutput()->get_param("id")) + ":out";
    }
    l.sort();
    return l;
}

IOBase *findIo(const QString &id)
{
    ListeRoom &lr = ListeRoom::Instance();
    if (IOBase *io = lr.get_output(id.toStdString()))
        return io;
    return lr.get_input(id.toStdString());
}

struct ProbeIo: public IOBase
{
    explicit ProbeIo(Params &p): IOBase(p, "probe", TBOOL, IOBase::IO_INPUT) { }
};

QSet<IOBase *> ruleIos()
{
    QSet<IOBase *> ios;
    for (int i = 0;i < ListeRule::Instance().size();i++)
    {
        Rule *r = ListeRule::Instance().get_rule(i);
        for (int c = 0;c < r->get_size_conds();c++)
        {
            Condition *cd = r->get_condition(c);
            for (int k = 0;k < cd->get_size();k++)
                ios << cd->get_input(k);
        }
        for (int a = 0;a < r->get_size_actions();a++)
        {
            Action *ac = r->get_action(a);
            for (int k = 0;k < ac->get_size();k++)
                ios << ac->get_output(k);
        }
    }
    return ios;
}

}

class tst_IoMove: public QObject
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
        resetModel();
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
    QByteArray saveRules()
    {
        QString p = work("dst_rules.xml");
        if (!ProjectManager::saveRulesToFile(p))
            return QByteArray();
        return readAll(p);
    }

    bool move(IOBase *io, Room *to);

private slots:
    void initTestCase();
    void init() { resetModel(); m_dialogs.clear(); }
    void cleanup() { QVERIFY2(m_dialogs.isEmpty(), qPrintable(m_dialogs.join('|'))); }

    void movedIoIsKeptOnceInItsNewRoom_data();
    void movedIoIsKeptOnceInItsNewRoom();
    void movingThereAndBackChangesNothing_data();
    void movingThereAndBackChangesNothing();
    void rulesKeepPointingAtMovedIos();
    void movingTwiceKeepsTheLastRoom();
    void refusedMovesChangeNothing();
    void treeFollowsTheModel_data();
    void treeFollowsTheModel();
    void treeKeepsTheCurrentItem();
    void refusedTreeMoveChangesNothing();
    void nestedItemsTravelWithTheirParent();
};

bool tst_IoMove::move(IOBase *io, Room *to)
{
    return ListeRoom::Instance().moveIO(io, to);
}

void tst_IoMove::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());

    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_iomove");
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

static void kinds()
{
    QTest::addColumn<QString>("id");
    QTest::addColumn<QStringList>("moved");

    QTest::newRow("input only")      << "in_temp"  << QStringList({ "in_temp" });
    QTest::newRow("output only")     << "out_dig"  << QStringList({ "out_dig" });
    QTest::newRow("InternalBool")    << "int_bool" << QStringList({ "int_bool" });
    QTest::newRow("InternalInt")     << "int_int"  << QStringList({ "int_int" });
    QTest::newRow("InternalString")  << "int_str"  << QStringList({ "int_str" });
    QTest::newRow("scenario")        << "scn"      << QStringList({ "scn" });
    QTest::newRow("InputTimer")      << "timer"    << QStringList({ "timer" });
    QTest::newRow("camera")          << "cam_i_cam_o" << QStringList({ "cam_i_cam_o" });
    QTest::newRow("audio")           << "aud"      << QStringList({ "aud" });
    QTest::newRow("avr")             << "avr"      << QStringList({ "avr" });
    QTest::newRow("device")          << "dev"      << QStringList({ "dev", "dev_target", "dev_in", "dev_out" });
}

void tst_IoMove::movedIoIsKeptOnceInItsNewRoom_data()
{
    kinds();
}

void tst_IoMove::movedIoIsKeptOnceInItsNewRoom()
{
    QFETCH(QString, id);
    QFETCH(QStringList, moved);

    QVERIFY(load(ioSource(kProject, " format=\"2\"")));
    ListeRoom &lr = ListeRoom::Instance();
    const QStringList expected = dumpWith(dump(), moved, "B");

    IOBase *io = findIo(id);
    QVERIFY(io);
    QVERIFY(move(io, lr.get_room(1)));

    QByteArray written = saveIOs();
    QVERIFY(!written.isEmpty());
    for (const QString &m: moved)
    {
        // Leading space: the camera's "iid" must not count as its "id".
        QCOMPARE(written.count(QString(" id=\"%1\"").arg(m).toUtf8()), 1);
    }
    QCOMPARE(dump(), expected);

    QVERIFY(load(written));
    QCOMPARE(dump(), expected);
    QCOMPARE(saveIOs(), written);
}

void tst_IoMove::movingThereAndBackChangesNothing_data()
{
    kinds();
}

void tst_IoMove::movingThereAndBackChangesNothing()
{
    QFETCH(QString, id);

    QVERIFY(load(ioSource(kProject, " format=\"2\"")));
    ListeRoom &lr = ListeRoom::Instance();
    const QStringList before = dump();

    IOBase *io = findIo(id);
    QVERIFY(move(io, lr.get_room(1)));
    QVERIFY(move(io, lr.get_room(0)));
    QCOMPARE(dump(), before);

    QByteArray written = saveIOs();
    QVERIFY(load(written));
    QCOMPARE(dump(), before);
}

void tst_IoMove::rulesKeepPointingAtMovedIos()
{
    QVERIFY(load(ioSource(kProject, " format=\"2\""), rulesSource(kRules)));
    ListeRoom &lr = ListeRoom::Instance();
    const QByteArray rulesBefore = saveRules();
    const QSet<IOBase *> ios = ruleIos();
    QCOMPARE(ListeRule::Instance().size(), 3);

    const QStringList movedIds = { "int_bool", "out_dig", "dev", "dev_target", "dev_in", "dev_out", "scn", "cam_i_cam_o" };
    const QStringList expected = dumpWith(dump(), movedIds, "C");
    for (const char *id: { "int_bool", "out_dig", "dev", "scn", "cam_i_cam_o" })
        QVERIFY(move(findIo(id), lr.get_room(2)));
    QCOMPARE(dump(), expected);

    QCOMPARE(ListeRule::Instance().size(), 3);
    QCOMPARE(ruleIos(), ios);
    QCOMPARE(saveRules(), rulesBefore);

    QByteArray written = saveIOs();
    QVERIFY(load(written, rulesBefore));
    QCOMPARE(dump(), expected);
    QCOMPARE(ListeRule::Instance().size(), 3);
    QCOMPARE(saveRules(), rulesBefore);
}

void tst_IoMove::movingTwiceKeepsTheLastRoom()
{
    QVERIFY(load(ioSource(kProject, " format=\"2\"")));
    ListeRoom &lr = ListeRoom::Instance();

    const QStringList expected = dumpWith(dump(), { "int_str" }, "C");
    IOBase *io = findIo("int_str");
    QVERIFY(move(io, lr.get_room(1)));
    QVERIFY(move(io, lr.get_room(2)));

    QCOMPARE(dump(), expected);
    QVERIFY(load(saveIOs()));
    QCOMPARE(dump(), expected);
}

// An owned IO is written inside its device: moving it alone would be undone by
// the next reload, so it is refused rather than half done.
void tst_IoMove::refusedMovesChangeNothing()
{
    QVERIFY(load(ioSource(kProject, " format=\"2\"")));
    ListeRoom &lr = ListeRoom::Instance();
    Room *a = lr.get_room(0);
    Room *b = lr.get_room(1);
    const QStringList before = dump();

    QVERIFY(!lr.moveIO(findIo("in_temp"), a));        // already there
    QVERIFY(!lr.moveIO(findIo("dev_in"), b));         // owned, input
    QVERIFY(!lr.moveIO(findIo("dev_out"), b));        // owned, output
    QVERIFY(!lr.moveIO(findIo("dev_target"), b));     // owned, input+output
    QVERIFY(!lr.moveIO(nullptr, b));
    QVERIFY(!lr.moveIO(findIo("in_temp"), nullptr));

    Room stranger("stranger", "salon");
    QVERIFY(!lr.moveIO(findIo("in_temp"), &stranger));

    Params p;
    p.Add("id", "orphan");
    ProbeIo orphan(p);
    QVERIFY(!lr.moveIO(&orphan, b));

    QCOMPARE(dump(), before);
    QCOMPARE(findIo("dev_in")->getOwnerDevice(), lr.get_device("dev"));
}

void tst_IoMove::treeFollowsTheModel_data()
{
    kinds();
}

void tst_IoMove::treeFollowsTheModel()
{
    QFETCH(QString, id);

    QVERIFY(load(ioSource(kProject, " format=\"2\"")));
    ListeRoom &lr = ListeRoom::Instance();
    QTreeWidget tree;
    populate(&tree);

    QVERIFY(moveIOInHome(&tree, findIo(id), lr.get_room(1)));
    const QStringList moved = treeDump(&tree);

    QTreeWidget fresh;
    populate(&fresh);
    QCOMPARE(moved, treeDump(&fresh));

    // Back again, through a third room.
    QVERIFY(moveIOInHome(&tree, findIo(id), lr.get_room(2)));
    QVERIFY(moveIOInHome(&tree, findIo(id), lr.get_room(0)));
    populate(&fresh);
    QCOMPARE(treeDump(&tree), treeDump(&fresh));
}

void tst_IoMove::treeKeepsTheCurrentItem()
{
    QVERIFY(load(ioSource(kProject, " format=\"2\"")));
    ListeRoom &lr = ListeRoom::Instance();
    QTreeWidget tree;
    populate(&tree);

    QTreeWidgetItem *grabbed = nullptr;
    for (QTreeWidgetItemIterator it(&tree);*it && !grabbed;++it)
        if (QTreeWidgetItemOutput *out = dynamic_cast<QTreeWidgetItemOutput *>(*it))
            if (out->getOutput() == findIo("int_bool"))
                grabbed = out;
    QVERIFY(grabbed);
    tree.setCurrentItem(grabbed);

    QVERIFY(moveIOInHome(&tree, findIo("int_bool"), lr.get_room(1)));
    QCOMPARE(tree.currentItem(), grabbed);
    QCOMPARE(dynamic_cast<QTreeWidgetItemRoom *>(grabbed->parent())->getRoom(), lr.get_room(1));
}

void tst_IoMove::refusedTreeMoveChangesNothing()
{
    QVERIFY(load(ioSource(kProject, " format=\"2\"")));
    ListeRoom &lr = ListeRoom::Instance();
    QTreeWidget tree;
    populate(&tree);
    const QStringList before = treeDump(&tree);

    QVERIFY(!moveIOInHome(&tree, findIo("dev_out"), lr.get_room(1)));
    QVERIFY(!moveIOInHome(&tree, findIo("in_temp"), lr.get_room(0)));
    QCOMPARE(treeDump(&tree), before);
}

// Once owned IOs are shown under their device, only the device item is a room child.
void tst_IoMove::nestedItemsTravelWithTheirParent()
{
    QVERIFY(load(ioSource(kProject, " format=\"2\"")));
    ListeRoom &lr = ListeRoom::Instance();
    QTreeWidget tree;
    populate(&tree);

    QTreeWidgetItem *deviceItem = nullptr;
    for (QTreeWidgetItemIterator it(&tree);*it && !deviceItem;++it)
        if (QTreeWidgetItemOutput *out = dynamic_cast<QTreeWidgetItemOutput *>(*it))
            if (out->getOutput() == findIo("dev"))
                deviceItem = out;
    QVERIFY(deviceItem);
    QTreeWidgetItem *child = new QTreeWidgetItemOutput(findIo("dev_out"), deviceItem);

    QVERIFY(moveIOInHome(&tree, findIo("dev"), lr.get_room(1)));
    QCOMPARE(child->parent(), deviceItem);
    QCOMPARE(dynamic_cast<QTreeWidgetItemRoom *>(deviceItem->parent())->getRoom(), lr.get_room(1));
}

QTEST_MAIN(tst_IoMove)
#include "tst_iomove.moc"
