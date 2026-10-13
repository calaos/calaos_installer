#include <QtTest>
#include <QApplication>
#include <QDialogButtonBox>
#include <QLabel>
#include <algorithm>
#include <memory>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTemporaryDir>

#include "HomeTree.h"
#include "HomeTreeMove.h"
#include "hometreewidget.h"
#include "DialogDeleteImpact.h"
#include "DialogDeviceInfo.h"
#include "projectmanager.h"
#include <DeletionImpact.h>
#include <ListeRoom.h>
#include <ListeRule.h>

using namespace Calaos;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

const char *kRooms = R"(
<calaos:room name="A" type="salon" hits="1">
  <calaos:input id="in_a" type="WITemp" name="temp a" host="h" port="502" var="0"/>
  <calaos:device id="dev" type="climate" impl="thermostat" name="Salon AC">
    <calaos:internal id="dev_target" type="InternalInt" name="target" visible="false"/>
    <calaos:input id="dev_in" type="WITemp" name="measure" host="h" port="502" var="3" visible="false"/>
    <calaos:output id="dev_out" type="WODigital" name="relay" host="h" port="502" var="4" visible="false"/>
    <calaos:link name="current_temp" io="in_a"/>
    <calaos:link name="output_heat" io="dev_out"/>
    <calaos:link name="ghost" io="nowhere"/>
    <calaos:param name="min_range" value="2"/>
  </calaos:device>
  <calaos:output id="out_light" type="WODigital" name="light" host="h" port="502" var="2"/>
</calaos:room>
<calaos:room name="B" type="salon" hits="1">
  <calaos:device id="other" type="climate" impl="proxy" name="Bedroom AC"/>
  <calaos:output id="out_b" type="WODigital" name="lamp b" host="h" port="502" var="7"/>
</calaos:room>
<calaos:room name="C" type="salon" hits="1"/>
</calaos:home>)";

QByteArray projectXml()
{
    return QByteArray("<?xml version=\"1.0\"?>\n<calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">\n<calaos:home format=\"2\">") +
           kRooms + "\n</calaos:ioconfig>\n";
}

IOBase *io(const char *id)
{
    ListeRoom &lr = ListeRoom::Instance();
    if (IOBase *o = lr.get_output(id))
        return o;
    return lr.get_input(id);
}

Room *room(int i)
{
    return ListeRoom::Instance().get_room(i);
}

void populate(QTreeWidget *tree)
{
    tree->clear();
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
        populateRoomItems(new QTreeWidgetItemRoom(lr.get_room(r), tree));
    tree->expandAll();
}

// One line per item, indented by depth: the shape of the tree, nothing else.
QStringList shape(QTreeWidget *tree)
{
    QStringList l;
    for (QTreeWidgetItemIterator it(tree);*it;++it)
    {
        int depth = 0;
        for (QTreeWidgetItem *p = (*it)->parent();p;p = p->parent())
            depth++;

        QString kind, id;
        if (QTreeWidgetItemRoom *r = dynamic_cast<QTreeWidgetItemRoom *>(*it))
        {
            kind = "room";
            id = u(r->getRoom()->get_name());
        }
        else if (QTreeWidgetItemDevice *d = dynamic_cast<QTreeWidgetItemDevice *>(*it))
        {
            kind = "device";
            id = u(d->getDevice()->get_param("id"));
        }
        else if (QTreeWidgetItemInput *in = dynamic_cast<QTreeWidgetItemInput *>(*it))
        {
            kind = "in";
            id = u(in->getInput()->get_param("id"));
        }
        else if (QTreeWidgetItemOutput *out = dynamic_cast<QTreeWidgetItemOutput *>(*it))
        {
            kind = "out";
            id = u(out->getOutput()->get_param("id"));
        }
        l << QString(depth * 2, ' ') + kind + " " + id;
    }
    return l;
}

QTreeWidgetItem *itemOf(QTreeWidget *tree, IOBase *target, bool device = false)
{
    for (QTreeWidgetItemIterator it(tree);*it;++it)
        if (ioOfItem(*it) == target && (!device || dynamic_cast<QTreeWidgetItemDevice *>(*it)))
            return *it;
    return nullptr;
}

Rule *ruleOn(const std::string &name, IOBase *in)
{
    Params rp;
    rp.Add("name", name);
    rp.Add("type", "t");
    Rule *r = new Rule(rp);
    Condition *c = new Condition(COND_STD);
    c->Add(in);
    r->AddCondition(c);
    ListeRule::Instance().Add(r);
    return r;
}

Rule *scriptOn(const std::string &name, IOBase *trigger)
{
    Params rp;
    rp.Add("name", name);
    rp.Add("type", "t");
    Rule *r = new Rule(rp);
    Condition *c = new Condition(COND_SCRIPT);
    c->addScriptInput(trigger);
    c->setScript("return true");
    r->AddCondition(c);
    ListeRule::Instance().Add(r);
    return r;
}

struct ProbeTree: public HomeTreeWidget
{
    using HomeTreeWidget::dragEnterEvent;
    using HomeTreeWidget::dragMoveEvent;
    using HomeTreeWidget::dropEvent;
    using HomeTreeWidget::dragCandidate;
    QSet<int> seen;   // timers that ticked while only the hover was going on
    int ticks = 0;    // ticks of those timers once `seen` is frozen
    bool frozen = false;
    void timerEvent(QTimerEvent *e) override
    {
        if (!frozen)
            seen << e->timerId();
        else if (seen.contains(e->timerId()))
            ticks++;
        HomeTreeWidget::timerEvent(e);
    }
    void setDragged(IOBase *io) { m_dragged = io; }
};

QStringList sectionTitles(QTreeWidget *tree)
{
    QStringList l;
    for (int i = 0;i < tree->topLevelItemCount();i++)
        l << tree->topLevelItem(i)->text(0);
    return l;
}

QStringList entries(QTreeWidgetItem *section)
{
    // The first child is the explanation, not an entry.
    QStringList l;
    for (int i = 1;i < section->childCount();i++)
        l << section->child(i)->text(0);
    return l;
}

}

class tst_HomeTree: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;

    bool load()
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
        QString path = m_workDir.filePath("io.xml");
        QFile f(path);
        if (!f.open(QIODevice::WriteOnly))
            return false;
        f.write(projectXml());
        f.close();
        return ProjectManager::loadIOsFromFile(path);
    }

private slots:
    void initTestCase();
    void init() { QVERIFY(load()); }
    void cleanupTestCase()
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
    }

    void devicesStandUnderTheirRoomWithOwnedIosBelow();
    void ownedIoOfADeviceInAnotherRoomStaysUnderItsRoom();
    void everyItemIsCreatedOnceAndAnnounced();
    void roomItemOfWalksUpFromANestedItem();

    void deletingADeviceDropsItsWholeSubtreeOnce();
    void deletingAnOwnedIoLeavesTheDeviceItem();
    void removingIosAndTheirParentTogetherDeletesOnce_data();
    void removingIosAndTheirParentTogetherDeletesOnce();
    void deletingAFreeIoLeavesTheRestAlone();
    void deletedCurrentItemMovesTheSelection();

    void movingADeviceCarriesItsSubtree();
    void ownedIoCannotBeMovedAlone();

    void dropOnAnyItemOfARoomMovesTheIo();
    void dropRefusedWhereNothingMoves();
    void aDragFromOutsideIsIgnored();
    void everyIoIsDraggedButNoRoom();
    void dragCarriesTheIdTheRuleTreesRead();
    void ownedIoIsNotDroppedOnARoom();

    void dialogForADeviceListsWhatItDoes();
    void dialogForARoomListsEverythingItDestroys();
    void dialogForAnIoNothingUses();
    void dialogWarnsAboutLinksAndScripts();
void dialogListsAnAttachedCamera();
void dropStopsTheAutoScroll();
    void dialogCancelIsTheDefault();
    void deviceInfoShowsParamsLinksAndOwnedIos();
};

void tst_HomeTree::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_hometree");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());
}

void tst_HomeTree::devicesStandUnderTheirRoomWithOwnedIosBelow()
{
    QTreeWidget tree;
    populate(&tree);

    QCOMPARE(shape(&tree), QStringList({
        "room A",
        "  in in_a",
        "  device dev",
        "    in dev_in",
        "    out dev_target",
        "    out dev_out",
        "  out out_light",
        "room B",
        "  device other",
        "  out out_b",
        "room C",
    }));
}

// A device's items are one: it is both an input and an output of its room.
void tst_HomeTree::ownedIoOfADeviceInAnotherRoomStaysUnderItsRoom()
{
    // dev_out is owned by "dev" (room A) but listed in room B by a hand-built model.
    IOBase *out = io("dev_out");
    room(0)->detach(out);
    room(1)->AddOutput(out);

    QTreeWidget tree;
    populate(&tree);

    QTreeWidgetItem *item = itemOf(&tree, out);
    QVERIFY(item);
    QCOMPARE(dynamic_cast<QTreeWidgetItemRoom *>(item->parent())->getRoom(), room(1));
}

void tst_HomeTree::everyItemIsCreatedOnceAndAnnounced()
{
    QTreeWidget tree;
    QList<QTreeWidgetItem *> announced;
    for (int r = 0;r < ListeRoom::Instance().size();r++)
    {
        QTreeWidgetItemRoom *iroom = new QTreeWidgetItemRoom(room(r), &tree);
        populateRoomItems(iroom, [&](QTreeWidgetItem *item)
        {
            // A parent is announced before its children.
            if (item->parent() != iroom)
                QVERIFY(announced.contains(item->parent()));
            announced.append(item);
        });
    }

    QCOMPARE(announced.size(), 8);
    QCOMPARE(QSet<QTreeWidgetItem *>(announced.begin(), announced.end()).size(), 8);
    for (QTreeWidgetItem *item: announced)
        QVERIFY(item->treeWidget() == &tree);
}

void tst_HomeTree::roomItemOfWalksUpFromANestedItem()
{
    QTreeWidget tree;
    populate(&tree);

    QTreeWidgetItem *nested = itemOf(&tree, io("dev_in"));
    QVERIFY(nested);
    QCOMPARE(roomItemOf(nested)->getRoom(), room(0));
    QCOMPARE(roomItemOf(roomItemOf(nested))->getRoom(), room(0));
    QVERIFY(roomItemOf(nullptr) == nullptr);
    QVERIFY(ioOfItem(roomItemOf(nested)) == nullptr);
}

// The children are deleted with the device's item: a second delete would be a double free.
void tst_HomeTree::deletingADeviceDropsItsWholeSubtreeOnce()
{
    QTreeWidget tree;
    populate(&tree);
    ruleOn("on owned", io("dev_in"));
    ruleOn("free", io("in_a"));

    deleteIOInHome(&tree, io("dev"));

    QCOMPARE(shape(&tree), QStringList({
        "room A", "  in in_a", "  out out_light",
        "room B", "  device other", "  out out_b",
        "room C",
    }));
    QVERIFY(!ListeRoom::Instance().get_device("dev"));
    QVERIFY(!io("dev_in"));
    QCOMPARE(ListeRule::Instance().size(), 1);

    QTreeWidget fresh;
    populate(&fresh);
    QCOMPARE(shape(&tree), shape(&fresh));
}

void tst_HomeTree::deletingAnOwnedIoLeavesTheDeviceItem()
{
    QTreeWidget tree;
    populate(&tree);

    deleteIOInHome(&tree, io("dev_in"));

    QVERIFY(ListeRoom::Instance().get_device("dev"));
    QCOMPARE(shape(&tree).mid(0, 6), QStringList({
        "room A", "  in in_a", "  device dev", "    out dev_target", "    out dev_out", "  out out_light" }));
    QCOMPARE(ListeRoom::Instance().get_device("dev")->ownedIos().size(), size_t(2));

    QTreeWidget fresh;
    populate(&fresh);
    QCOMPARE(shape(&tree), shape(&fresh));
}

void tst_HomeTree::removingIosAndTheirParentTogetherDeletesOnce_data()
{
    QTest::addColumn<bool>("childrenFirst");
    QTest::newRow("device then children") << false;
    QTest::newRow("children then device") << true;
}

// removeIOItems must not care in which order the items are met.
void tst_HomeTree::removingIosAndTheirParentTogetherDeletesOnce()
{
    QFETCH(bool, childrenFirst);

    QTreeWidget tree;
    populate(&tree);

    std::vector<IOBase *> ios = ListeRoom::iosDeletedWith(io("dev"));
    if (childrenFirst)
        std::reverse(ios.begin(), ios.end());

    removeIOItems(&tree, ios);

    QCOMPARE(shape(&tree), QStringList({
        "room A", "  in in_a", "  out out_light",
        "room B", "  device other", "  out out_b",
        "room C",
    }));
}

void tst_HomeTree::deletingAFreeIoLeavesTheRestAlone()
{
    QTreeWidget tree;
    populate(&tree);
    const QStringList before = shape(&tree);

    deleteIOInHome(&tree, io("out_light"));

    QStringList expected = before;
    expected.removeOne("  out out_light");
    QCOMPARE(shape(&tree), expected);
}

void tst_HomeTree::deletedCurrentItemMovesTheSelection()
{
    QTreeWidget tree;
    populate(&tree);
    tree.setCurrentItem(itemOf(&tree, io("dev_out")));

    deleteIOInHome(&tree, io("dev"));

    QTreeWidgetItem *current = tree.currentItem();
    QVERIFY(current == nullptr || ioOfItem(current) != io("dev_out"));
    QVERIFY(current == nullptr || tree.indexFromItem(current).isValid());
}

void tst_HomeTree::movingADeviceCarriesItsSubtree()
{
    QTreeWidget tree;
    populate(&tree);
    QTreeWidgetItem *deviceItem = itemOf(&tree, io("dev"), true);
    QTreeWidgetItem *child = itemOf(&tree, io("dev_in"));

    QVERIFY(moveIOInHome(&tree, io("dev"), room(2)));

    QCOMPARE(deviceItem->parent(), static_cast<QTreeWidgetItem *>(tree.topLevelItem(2)));
    QCOMPARE(child->parent(), deviceItem);
    QCOMPARE(room(2)->get_size_out(), 3);

    QTreeWidget fresh;
    populate(&fresh);
    auto sorted = [](QStringList l) { l.sort(); return l; };
    QCOMPARE(sorted(shape(&tree)), sorted(shape(&fresh)));
}

void tst_HomeTree::ownedIoCannotBeMovedAlone()
{
    QTreeWidget tree;
    populate(&tree);
    const QStringList before = shape(&tree);

    QVERIFY(!moveIOInHome(&tree, io("dev_in"), room(2)));
    QCOMPARE(shape(&tree), before);
}

void tst_HomeTree::dropOnAnyItemOfARoomMovesTheIo()
{
    ProbeTree tree;
    populate(&tree);
    tree.resize(400, 400);
    tree.show();
    QVERIFY(QTest::qWaitForWindowExposed(&tree));
    QSignalSpy moved(&tree, &HomeTreeWidget::ioMoved);

    // On the room itself, then on a nested item of the destination (a device's child).
    QVERIFY(tree.dropIO(io("out_light"), tree.visualItemRect(tree.topLevelItem(2)).center()));
    QCOMPARE(moved.count(), 1);
    QCOMPARE(room(2)->get_size_out(), 1);

    QVERIFY(tree.dropIO(io("out_b"), tree.visualItemRect(itemOf(&tree, io("dev_in"))).center()));
    QCOMPARE(moved.count(), 2);
    QVERIFY(ListeRoom::Instance().searchRoomByOutput(io("out_b")) == room(0));

    // The same through the drop event, with a drag in progress.
    tree.setDragged(io("out_b"));
    const QPoint onC = tree.visualItemRect(tree.topLevelItem(2)).center();
    QMimeData mime;
    QDropEvent drop(onC, Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    tree.dropEvent(&drop);
    QVERIFY(drop.isAccepted());
    QVERIFY(ListeRoom::Instance().searchRoomByOutput(io("out_b")) == room(2));
    QCOMPARE(moved.count(), 3);
}

void tst_HomeTree::dropRefusedWhereNothingMoves()
{
    ProbeTree tree;
    populate(&tree);
    tree.resize(400, 400);
    tree.show();
    QVERIFY(QTest::qWaitForWindowExposed(&tree));
    QSignalSpy moved(&tree, &HomeTreeWidget::ioMoved);
    const QStringList before = shape(&tree);

    QVERIFY(!tree.dropIO(io("out_light"), tree.visualItemRect(tree.topLevelItem(0)).center())); // same room
    QVERIFY(!tree.dropIO(io("out_light"), QPoint(-5, -5)));                                       // nowhere
    QVERIFY(!tree.dropIO(nullptr, tree.visualItemRect(tree.topLevelItem(2)).center()));
    QVERIFY(!tree.dropIO(io("dev_in"), tree.visualItemRect(tree.topLevelItem(2)).center()));     // owned

    QCOMPARE(moved.count(), 0);
    QCOMPARE(shape(&tree), before);
}

// Qt's file drops and other widgets' drags carry no IO: nothing is moved.
void tst_HomeTree::aDragFromOutsideIsIgnored()
{
    ProbeTree tree;
    populate(&tree);
    tree.resize(400, 400);
    tree.show();
    QVERIFY(QTest::qWaitForWindowExposed(&tree));
    const QStringList before = shape(&tree);

    QMimeData mime;
    mime.setText("out_light");
    const QPoint onC = tree.visualItemRect(tree.topLevelItem(2)).center();

    QDropEvent drop(onC, Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    tree.dropEvent(&drop);
    QVERIFY(!drop.isAccepted());

    QDragEnterEvent enter(onC, Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    tree.dragEnterEvent(&enter);
    QVERIFY(!enter.isAccepted());

    QDragMoveEvent hover(onC, Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    tree.dragMoveEvent(&hover);
    QVERIFY(!hover.isAccepted());

    QCOMPARE(shape(&tree), before);

    // With a drag of ours in progress, hovering another room is accepted, its own room is not.
    tree.setDragged(io("out_light"));
    QDragEnterEvent enter2(onC, Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    tree.dragEnterEvent(&enter2);
    QVERIFY(enter2.isAccepted());
    QDragMoveEvent over(onC, Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    tree.dragMoveEvent(&over);
    QVERIFY(over.isAccepted());
    QDragMoveEvent ownRoom(tree.visualItemRect(tree.topLevelItem(0)).center(), Qt::MoveAction, &mime,
                           Qt::LeftButton, Qt::NoModifier);
    tree.dragMoveEvent(&ownRoom);
    QVERIFY(!ownRoom.isAccepted());
}

// Any IO may be dragged into a rule, an owned one included; rooms are not dragged.
void tst_HomeTree::everyIoIsDraggedButNoRoom()
{
    ProbeTree tree;
    populate(&tree);

    auto candidate = [&](QTreeWidgetItem *item)
    {
        tree.setCurrentItem(item);
        return tree.dragCandidate();
    };

    QVERIFY(candidate(itemOf(&tree, io("out_light"))) == io("out_light"));
    QVERIFY(candidate(itemOf(&tree, io("dev"), true)) == io("dev"));
    QVERIFY(candidate(itemOf(&tree, io("dev_in"))) == io("dev_in"));
    QVERIFY(candidate(itemOf(&tree, io("dev_out"))) == io("dev_out"));
    QVERIFY(candidate(tree.topLevelItem(0)) == nullptr);
}

// The condition and action trees read text/uri-list and take the url as an IO id.
void tst_HomeTree::dragCarriesTheIdTheRuleTreesRead()
{
    for (const char *id: { "out_light", "dev_in", "dev_out", "dev" })
    {
        std::unique_ptr<QMimeData> mime(HomeTreeWidget::ioMimeData(io(id)));

        QVERIFY(mime->hasFormat("text/uri-list"));
        QCOMPARE(mime->urls().size(), 1);
        const std::string read = mime->urls().first().toString().toUtf8().data();
        QCOMPARE(read, std::string(id));
        QVERIFY(ListeRoom::Instance().get_input(read) || ListeRoom::Instance().get_output(read));
    }
}

// Dropping on a room would split the IO from its device, which writes it inside itself.
void tst_HomeTree::ownedIoIsNotDroppedOnARoom()
{
    ProbeTree tree;
    populate(&tree);
    tree.resize(400, 400);
    tree.show();
    QVERIFY(QTest::qWaitForWindowExposed(&tree));
    const QStringList before = shape(&tree);

    tree.setDragged(io("dev_in"));
    QMimeData mime;
    const QPoint onC = tree.visualItemRect(tree.topLevelItem(2)).center();

    QDragMoveEvent over(onC, Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    tree.dragMoveEvent(&over);
    QVERIFY(!over.isAccepted());

    QDropEvent drop(onC, Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    tree.dropEvent(&drop);
    QVERIFY(!drop.isAccepted());
    QCOMPARE(shape(&tree), before);
}

void tst_HomeTree::dialogForADeviceListsWhatItDoes()
{
    ruleOn("on relay", io("dev_out"));
    ruleOn("on other", io("out_b"));
    scriptOn("script on target", io("dev_target"));
    Rule *var = ruleOn("names the relay", io("out_b"));
    var->get_condition(0)->get_params_var().Add("v", "dev_in");

    DeletionImpact imp = deletionImpact(io("dev"));
    DialogDeleteImpact dlg("device \"Salon AC\"", imp, DialogDeleteImpact::ListOwned);

    QCOMPARE(dlg.findChild<QLabel *>("title")->text(), QString("Delete device \"Salon AC\"?"));
    QTreeWidget *tree = dlg.findChild<QTreeWidget *>("impactTree");
    QCOMPARE(sectionTitles(tree), QStringList({
        "Deleted with it (3)",
        "Rules deleted (1)",
        "Script rules that will never run (1)",
        "Rules still naming a deleted variable (1)",
    }));
    QCOMPARE(entries(tree->topLevelItem(0)),
             QStringList({ "target (dev_target)", "measure (dev_in)", "relay (dev_out)" }));
    QCOMPARE(entries(tree->topLevelItem(1)), QStringList({ "on relay (t)" }));
    QCOMPARE(entries(tree->topLevelItem(2)), QStringList({ "script on target (t)" }));
    QCOMPARE(entries(tree->topLevelItem(3)), QStringList({ "names the relay (t), variable \"dev_in\"" }));
}

void tst_HomeTree::dialogForARoomListsEverythingItDestroys()
{
    // A full room with no rule at all must still say what goes.
    DeletionImpact imp = deletionImpactOfRoom(room(0));
    DialogDeleteImpact dlg("room \"A\"", imp, DialogDeleteImpact::ListAll);

    QTreeWidget *tree = dlg.findChild<QTreeWidget *>("impactTree");
    QVERIFY(!tree->isHidden());
    QCOMPARE(sectionTitles(tree).first(), QString("Deleted with it (%1)").arg(imp.ios.size()));
    QCOMPARE(entries(tree->topLevelItem(0)).size(), int(imp.ios.size()));
    QVERIFY(entries(tree->topLevelItem(0)).contains("temp a (in_a)"));
    QVERIFY(entries(tree->topLevelItem(0)).contains("Salon AC (dev)"));
    QVERIFY(!dlg.findChild<QLabel *>("lead")->text().startsWith("No rule and no device"));
}

void tst_HomeTree::dialogForAnIoNothingUses()
{
    DeletionImpact imp = deletionImpact(io("out_light"));
    DialogDeleteImpact dlg("\"light\"", imp, DialogDeleteImpact::ListNone);

    QTreeWidget *tree = dlg.findChild<QTreeWidget *>("impactTree");
    QCOMPARE(tree->topLevelItemCount(), 0);
    QVERIFY(tree->isHidden());
    QCOMPARE(dlg.findChild<QLabel *>("lead")->text(), QString("No rule and no device uses it."));
}

void tst_HomeTree::dialogWarnsAboutLinksAndScripts()
{
    scriptOn("watches temp", io("in_a"));

    DialogDeleteImpact dlg("\"temp a\"", deletionImpact(io("in_a")), DialogDeleteImpact::ListNone);

    QTreeWidget *tree = dlg.findChild<QTreeWidget *>("impactTree");
    QCOMPARE(sectionTitles(tree), QStringList({
        "Script rules that will never run (1)",
        "Device links left pointing at nothing (1)",
    }));
    QCOMPARE(entries(tree->topLevelItem(1)), QStringList({ "Salon AC (dev): link \"current_temp\" to in_a" }));
    QVERIFY(!dlg.findChild<QLabel *>("lead")->text().startsWith("No rule"));
}

void tst_HomeTree::dialogListsAnAttachedCamera()
{
    Params cp;
    cp.Add("iid", "cam_i");
    cp.Add("oid", "cam_o");
    cp.Add("type", "planet");
    IOBase *camera = ListeRoom::Instance().createCamera(cp, room(2));
    QVERIFY(camera);
    Rule *r = ruleOn("mail me", io("in_a"));
    Action *mail = new Action(ACTION_MAIL);
    mail->setMailAttachment("cam_i_cam_o");
    r->AddAction(mail);

    DialogDeleteImpact dlg("camera", deletionImpact(camera), DialogDeleteImpact::ListNone);

    QTreeWidget *tree = dlg.findChild<QTreeWidget *>("impactTree");
    QCOMPARE(sectionTitles(tree), QStringList({ "Rules still naming a deleted variable (1)" }));
    QCOMPARE(entries(tree->topLevelItem(0)), QStringList({ "mail me (t), attached camera \"cam_i_cam_o\"" }));
}

// The base dropEvent, which stops the auto-scroll started while hovering near the
// edge, is not called: without stopAutoScroll the timer outlives the drop.
void tst_HomeTree::dropStopsTheAutoScroll()
{
    ProbeTree tree;
    populate(&tree);
    tree.resize(300, 90);
    tree.show();
    QVERIFY(QTest::qWaitForWindowExposed(&tree));
    tree.setDragged(io("out_light"));

    QMimeData mime;
    const QPoint nearEdge(20, tree.viewport()->height() - 2);
    QDragMoveEvent hover(nearEdge, Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    tree.dragMoveEvent(&hover);

    QTest::qWait(300);
    QVERIFY2(!tree.seen.isEmpty(), "hovering near the edge should start the auto-scroll");
    tree.frozen = true;

    const QPoint onC = tree.visualItemRect(tree.topLevelItem(2)).center();
    QDropEvent drop(onC, Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    tree.dropEvent(&drop);

    tree.ticks = 0;
    QTest::qWait(300);
    QCOMPARE(tree.ticks, 0);
}

void tst_HomeTree::dialogCancelIsTheDefault()
{
    DialogDeleteImpact dlg("\"light\"", deletionImpact(io("out_light")), DialogDeleteImpact::ListNone);

    auto *buttons = dlg.findChild<QDialogButtonBox *>();
    QVERIFY(buttons);
    QVERIFY(buttons->button(QDialogButtonBox::Cancel)->isDefault());

    QSignalSpy accepted(&dlg, &QDialog::accepted);
    QSignalSpy rejected(&dlg, &QDialog::rejected);
    buttons->button(QDialogButtonBox::Cancel)->click();
    QCOMPARE(rejected.count(), 1);
    QCOMPARE(accepted.count(), 0);

    for (QPushButton *b: buttons->findChildren<QPushButton *>())
        if (b->text() == "Delete")
            b->click();
    QCOMPARE(accepted.count(), 1);
}

void tst_HomeTree::deviceInfoShowsParamsLinksAndOwnedIos()
{
    DialogDeviceInfo dlg(ListeRoom::Instance().get_device("dev"));
    QTreeWidget *tree = dlg.findChild<QTreeWidget *>("deviceTree");
    QVERIFY(tree);

    QStringList flat;
    for (int g = 0;g < tree->topLevelItemCount();g++)
    {
        QTreeWidgetItem *group = tree->topLevelItem(g);
        flat << group->text(0);
        for (int i = 0;i < group->childCount();i++)
            flat << "  " + group->child(i)->text(0) + "=" + group->child(i)->text(1);
    }

    QCOMPARE(flat, QStringList({
        "Device", "  Id=dev", "  Name=Salon AC", "  Family=climate", "  Implementation=thermostat",
        "Parameters", "  min_range=2",
        "Links", "  current_temp=temp a (in_a)", "  output_heat=relay (dev_out)", "  ghost=nowhere (not found)",
        "Owned IOs", "  target=InternalInt (dev_target)", "  measure=WITemp (dev_in)", "  relay=WODigital (dev_out)",
    }));
}

QTEST_MAIN(tst_HomeTree)
#include "tst_hometree.moc"
