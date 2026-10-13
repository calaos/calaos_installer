#include <QtTest>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>

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

// Room A: a free IO of each direction, an input+output, and two devices; room B is
// where a device in another room links to something in A.
const char *kRooms = R"(
<calaos:room name="A" type="salon" hits="1">
  <calaos:input id="in_a" type="WITemp" name="temp a" host="h" port="502" var="0"/>
  <calaos:input id="in_b" type="WITemp" name="temp b" host="h" port="502" var="1"/>
  <calaos:output id="out_light" type="WODigital" name="light" host="h" port="502" var="2"/>
  <calaos:internal id="int_bool" type="InternalBool" name="flag" visible="false"/>
  <calaos:camera id="cam_i_cam_o" iid="cam_i" oid="cam_o" type="planet" name="cam" host="h" port="80" model="ICA-300"/>
  <calaos:device id="dev" type="climate" impl="thermostat" name="Salon AC">
    <calaos:internal id="dev_target" type="InternalInt" name="target" visible="false"/>
    <calaos:input id="dev_in" type="WITemp" name="measure" host="h" port="502" var="3" visible="false"/>
    <calaos:output id="dev_out" type="WODigital" name="relay" host="h" port="502" var="4" visible="false"/>
    <calaos:link name="current_temp" io="in_a"/>
    <calaos:link name="output_heat" io="dev_out" command_io="out_light"/>
  </calaos:device>
</calaos:room>
<calaos:room name="B" type="salon" hits="1">
  <calaos:input id="in_c" type="WITemp" name="temp c" host="h" port="502" var="5"/>
  <calaos:device id="other" type="climate" impl="proxy" name="Bedroom AC">
    <calaos:link name="current_temp" io="in_a"/>
    <calaos:link name="mode" io="dev_in"/>
    <calaos:link name="fan" io="in_c"/>
  </calaos:device>
</calaos:room>
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

Rule *newRule(const std::string &name)
{
    Params rp;
    rp.Add("name", name);
    rp.Add("type", "t");
    Rule *r = new Rule(rp);
    ListeRule::Instance().Add(r);
    return r;
}

Rule *onInput(const std::string &name, IOBase *in)
{
    Rule *r = newRule(name);
    Condition *c = new Condition(COND_STD);
    c->Add(in);
    r->AddCondition(c);
    return r;
}

Rule *onOutputState(const std::string &name, IOBase *out)
{
    Rule *r = newRule(name);
    Condition *c = new Condition(COND_OUTPUT);
    c->setOutput(out);
    r->AddCondition(c);
    return r;
}

Rule *onScript(const std::string &name, std::initializer_list<IOBase *> triggers)
{
    Rule *r = newRule(name);
    Condition *c = new Condition(COND_SCRIPT);
    for (IOBase *t: triggers)
        c->addScriptInput(t);
    c->setScript("return true");
    r->AddCondition(c);
    return r;
}

void addAction(Rule *r, IOBase *out)
{
    Action *a = new Action(ACTION_STD);
    a->Add(out);
    r->AddAction(a);
}

QStringList names(const std::vector<Rule *> &rules)
{
    QStringList l;
    for (Rule *r: rules)
        l << u(r->get_name());
    l.sort();
    return l;
}

QStringList ids(const std::vector<IOBase *> &ios)
{
    QStringList l;
    for (IOBase *i: ios)
        l << u(i->get_param("id"));
    return l;
}

QStringList ruleNames()
{
    QStringList l;
    for (int i = 0;i < ListeRule::Instance().size();i++)
        l << u(ListeRule::Instance().get_rule(i)->get_name());
    l.sort();
    return l;
}

QSet<QString> roomIds(Room *room)
{
    QSet<QString> l;
    for (int i = 0;i < room->get_size_in();i++)
        l << u(room->get_input(i)->get_param("id"));
    for (int i = 0;i < room->get_size_out();i++)
        l << u(room->get_output(i)->get_param("id"));
    return l;
}

// The model as the announcement is about to be checked against it.
struct World
{
    QSet<QString> ios;
    QMap<QString, int> scriptTriggers; // by rule name
    QStringList rules;
};

World snap()
{
    World w;
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
    {
        Room *room = lr.get_room(r);
        for (int i = 0;i < room->get_size_in();i++)
            w.ios << u(room->get_input(i)->get_param("id"));
        for (int i = 0;i < room->get_size_out();i++)
            w.ios << u(room->get_output(i)->get_param("id"));
    }
    for (int i = 0;i < ListeRule::Instance().size();i++)
    {
        Rule *rule = ListeRule::Instance().get_rule(i);
        int n = 0;
        for (int c = 0;c < rule->get_size_conds();c++)
            n += rule->get_condition(c)->getScriptInputSize();
        w.scriptTriggers[u(rule->get_name())] = n;
        w.rules << u(rule->get_name());
    }
    w.rules.sort();
    return w;
}

// Whether the server could still start this rule by itself.
bool canRun(Rule *rule)
{
    for (int c = 0;c < rule->get_size_conds();c++)
    {
        Condition *cond = rule->get_condition(c);
        if (cond->getType() == COND_SCRIPT)
        {
            if (cond->getScriptInputSize() > 0)
                return true;
        }
        else if (cond->isTrigger() && (cond->getType() == COND_START || cond->get_size() > 0 || cond->getOutput()))
            return true;
    }
    return false;
}

// Names and ids taken from an impact before the deletion frees what it points at.
struct Announcement
{
    QSet<QString> ios;
    QStringList removed, inert;

    explicit Announcement(const DeletionImpact &imp)
    {
        for (const QString &id: ids(imp.ios))
            ios << id;
        removed = names(imp.removedRules);
        inert = names(imp.inertRules);
    }

    // `before` is the world the impact was computed in.
    void verify(const World &before) const
    {
        const World after = snap();

        QCOMPARE(ios, before.ios - after.ios);

        QStringList actualRemoved = before.rules;
        for (const QString &n: after.rules)
            actualRemoved.removeOne(n);
        actualRemoved.sort();
        QCOMPARE(removed, actualRemoved);

        QStringList actualInert;
        for (int i = 0;i < ListeRule::Instance().size();i++)
        {
            Rule *rule = ListeRule::Instance().get_rule(i);
            const QString n = u(rule->get_name());
            if (before.scriptTriggers.value(n) > 0 && !canRun(rule))
                actualInert << n;
        }
        actualInert.sort();
        QCOMPARE(inert, actualInert);
    }
};

}

class tst_DeleteImpact: public QObject
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

    void anUnusedIoHasNoImpact();
    void rulesHoldingTheIoAreRemoved();
    void scriptRuleLeftWithoutTriggerIsInert();
    void scriptRuleKeepingATriggerIsNotInert();
    void scriptRuleAlreadyWithoutTriggerIsNotBlamed();
    void scriptRuleAlsoActingOnTheIoIsRemovedNotInert();
    void orphanVariablesAreListedPerRule();
void cameraAttachmentsAreListedAsOrphans();
void inertRuleSurvivesSaveAndReload();
    void removedRuleIsNotAlsoAnOrphan();
    void deviceImpactCoversItsOwnedIos();
    void linksOfOtherDevicesAreBroken_data();
    void linksOfOtherDevicesAreBroken();
    void aDeviceDoesNotBreakItsOwnLinks();
    void deletingAnOwnedIoAloneBreaksItsDevicesLink();
    void roomImpactIsTheUnionOfItsIos();
    void scriptRuleWhoseOtherConditionsDoNotTriggerIsInert();
    void impactIsWhatTheDeletionDoes();
    void roomImpactIsWhatRemovingTheRoomDoes();
    void impactOfSeveralIosDoesNotCountTwice();
};

void tst_DeleteImpact::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_deleteimpact");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());
}

void tst_DeleteImpact::anUnusedIoHasNoImpact()
{
    onInput("on b", io("in_b"));

    DeletionImpact imp = deletionImpact(io("int_bool"));

    QVERIFY(imp.isEmpty());
    QVERIFY(!imp.affectsRules());
    QCOMPARE(ids(imp.ios), QStringList({ "int_bool" }));
}

void tst_DeleteImpact::rulesHoldingTheIoAreRemoved()
{
    onInput("as condition", io("in_b"));
    onOutputState("as output state", io("in_b"));
    Rule *act = onInput("as action", io("in_c"));
    addAction(act, io("in_b"));
    onInput("elsewhere", io("in_c"));

    DeletionImpact imp = deletionImpact(io("in_b"));

    QCOMPARE(names(imp.removedRules), QStringList({ "as action", "as condition", "as output state" }));
    QVERIFY(imp.inertRules.empty());
    QVERIFY(imp.orphanVariables.empty());
    QVERIFY(imp.affectsRules());
    QVERIFY(!imp.isEmpty());
}

void tst_DeleteImpact::scriptRuleLeftWithoutTriggerIsInert()
{
    onScript("one trigger", { io("in_b") });
    onScript("two triggers", { io("in_b"), io("in_c") });

    QCOMPARE(names(deletionImpact(io("in_b")).inertRules), QStringList({ "one trigger" }));

    // Both triggers going together leave the second rule without any.
    DeletionImpact both = deletionImpact({ io("in_b"), io("in_c") });
    QCOMPARE(names(both.inertRules), QStringList({ "one trigger", "two triggers" }));
    QVERIFY(both.removedRules.empty());
}

void tst_DeleteImpact::scriptRuleKeepingATriggerIsNotInert()
{
    onScript("another trigger", { io("in_b"), io("in_c") });

    Rule *withInput = onScript("also reads an input", { io("in_b") });
    Condition *std = new Condition(COND_STD);
    std->Add(io("in_c"));
    withInput->AddCondition(std);

    Rule *atStart = onScript("also runs at start", { io("in_b") });
    atStart->AddCondition(new Condition(COND_START));

    DeletionImpact imp = deletionImpact(io("in_b"));

    QVERIFY(imp.inertRules.empty());
    QVERIFY(imp.removedRules.empty());
}

// It never ran, and deleting this IO changes nothing about that.
void tst_DeleteImpact::scriptRuleAlreadyWithoutTriggerIsNotBlamed()
{
    onScript("dead already", {});

    QVERIFY(deletionImpact(io("in_b")).isEmpty());
}

void tst_DeleteImpact::scriptRuleAlsoActingOnTheIoIsRemovedNotInert()
{
    Rule *r = onScript("script and action", { io("in_b") });
    addAction(r, io("out_light"));

    DeletionImpact imp = deletionImpact({ io("in_b"), io("out_light") });

    QCOMPARE(names(imp.removedRules), QStringList({ "script and action" }));
    QVERIFY(imp.inertRules.empty());
}

void tst_DeleteImpact::orphanVariablesAreListedPerRule()
{
    Rule *r = onInput("vars", io("in_c"));
    r->get_condition(0)->get_params_var().Add("a", "in_b");
    r->get_condition(0)->get_params_var().Add("unrelated", "somewhere_else");
    Condition *oc = new Condition(COND_OUTPUT);
    oc->setOutput(io("out_light"));
    oc->setOutputParamVar("in_b");
    r->AddCondition(oc);
    Action *a = new Action(ACTION_STD);
    a->Add(io("out_light"));
    a->get_params_var().Add("v", "cam_i");
    r->AddAction(a);

    DeletionImpact imp = deletionImpact(io("in_b"));
    QVERIFY(imp.removedRules.empty());
    QCOMPARE(imp.orphanVariables.size(), size_t(1));
    QCOMPARE(imp.orphanVariables[0].rule, r);
    // One entry for the rule however many times it names the id.
    QCOMPARE(u(imp.orphanVariables[0].id), QString("in_b"));

    // A camera answers to its legacy iid/oid too.
    DeletionImpact cam = deletionImpact(io("cam_i_cam_o"));
    QCOMPARE(cam.orphanVariables.size(), size_t(1));
    QCOMPARE(u(cam.orphanVariables[0].id), QString("cam_i"));
}

void tst_DeleteImpact::cameraAttachmentsAreListedAsOrphans()
{
    Rule *r = onInput("mails", io("in_b"));
    Action *mail = new Action(ACTION_MAIL);
    mail->setMailAttachment("cam_i_cam_o");
    r->AddAction(mail);
    Action *push = new Action(ACTION_PUSH);
    push->setMailAttachment("cam_o");
    r->AddAction(push);
    Action *view = new Action(ACTION_TOUCHSCREEN);
    view->setTouchscreenAction("view_camera");
    view->setTouchscreenCamera("cam_i");
    r->AddAction(view);
    Action *elsewhere = new Action(ACTION_MAIL);
    elsewhere->setMailAttachment("other_cam");
    r->AddAction(elsewhere);

    DeletionImpact imp = deletionImpact(io("cam_i_cam_o"));

    QCOMPARE(imp.orphanVariables.size(), size_t(3));
    QStringList got;
    for (const OrphanVariable &v: imp.orphanVariables)
    {
        QCOMPARE(v.rule, r);
        QVERIFY(v.attachment);
        got << u(v.id);
    }
    got.sort();
    QCOMPARE(got, QStringList({ "cam_i", "cam_i_cam_o", "cam_o" }));
    QVERIFY(imp.removedRules.empty());

    QVERIFY(deletionImpact(io("in_a")).orphanVariables.empty());
}

// The reloaded rule is the one the user saw warned about: kept with its Lua, still
// without a trigger, and still never run.
void tst_DeleteImpact::inertRuleSurvivesSaveAndReload()
{
    Rule *r = onScript("inert", { io("in_b") });
    addAction(r, io("out_light"));
    onScript("alive", { io("in_b"), io("in_c") });

    const DeletionImpact imp = deletionImpact(io("in_b"));
    QCOMPARE(names(imp.inertRules), QStringList({ "inert" }));
    QVERIFY(ListeRoom::Instance().deleteIOInput(io("in_b")));

    QString ioFile = m_workDir.filePath("saved_io.xml");
    QString rulesFile = m_workDir.filePath("saved_rules.xml");
    QVERIFY(ProjectManager::saveIOsToFile(ioFile));
    QVERIFY(ProjectManager::saveRulesToFile(rulesFile));

    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();
    QVERIFY(ProjectManager::loadIOsFromFile(ioFile));
    QVERIFY(ProjectManager::loadRulesFromFile(rulesFile));

    QCOMPARE(ruleNames(), QStringList({ "alive", "inert" }));
    Rule *inert = ListeRule::Instance().searchRule("t", "inert");
    QVERIFY(inert);
    QCOMPARE(inert->get_size_conds(), 1);
    Condition *c = inert->get_condition(0);
    QCOMPARE(c->getType(), int(COND_SCRIPT));
    QCOMPARE(c->getScriptInputSize(), 0);
    QCOMPARE(c->getScript(), std::string("return true"));
    QCOMPARE(inert->get_size_actions(), 1);
    QVERIFY(!canRun(inert));

    Rule *alive = ListeRule::Instance().searchRule("t", "alive");
    QVERIFY(alive);
    QCOMPARE(alive->get_condition(0)->getScriptInputSize(), 1);
    QVERIFY(canRun(alive));

    // Already without trigger: later deletions do not blame it.
    QVERIFY(deletionImpact(io("out_light")).inertRules.empty());
    QVERIFY(deletionImpact(io("in_a")).inertRules.empty());
    QCOMPARE(names(deletionImpact(io("in_c")).inertRules), QStringList({ "alive" }));
}

void tst_DeleteImpact::removedRuleIsNotAlsoAnOrphan()
{
    Rule *r = onInput("on b", io("in_b"));
    r->get_condition(0)->get_params_var().Add("a", "in_b");

    DeletionImpact imp = deletionImpact(io("in_b"));

    QCOMPARE(names(imp.removedRules), QStringList({ "on b" }));
    QVERIFY(imp.orphanVariables.empty());
}

void tst_DeleteImpact::deviceImpactCoversItsOwnedIos()
{
    Device *dev = ListeRoom::Instance().get_device("dev");
    onInput("on device", dev);
    onInput("on owned input", io("dev_in"));
    Rule *r = onInput("on free", io("in_b"));
    addAction(r, io("dev_out"));
    onScript("script on owned", { io("dev_target") });
    onInput("untouched", io("in_c"));

    DeletionImpact imp = deletionImpact(dev);

    QCOMPARE(ids(imp.ios), QStringList({ "dev", "dev_target", "dev_in", "dev_out" }));
    QCOMPARE(names(imp.removedRules), QStringList({ "on device", "on free", "on owned input" }));
    QCOMPARE(names(imp.inertRules), QStringList({ "script on owned" }));
}

void tst_DeleteImpact::linksOfOtherDevicesAreBroken_data()
{
    QTest::addColumn<QString>("target");
    QTest::addColumn<QStringList>("links"); // "device:link:io"

    QTest::newRow("external, linked twice") << "in_a"
        << QStringList({ "dev:current_temp:in_a", "other:current_temp:in_a" });
    QTest::newRow("external, linked once") << "in_c" << QStringList({ "other:fan:in_c" });
    QTest::newRow("command io") << "out_light" << QStringList({ "dev:output_heat:out_light" });
    QTest::newRow("unlinked") << "in_b" << QStringList();
}

void tst_DeleteImpact::linksOfOtherDevicesAreBroken()
{
    QFETCH(QString, target);
    QFETCH(QStringList, links);

    DeletionImpact imp = deletionImpact(io(target.toUtf8().constData()));

    QStringList got;
    for (const BrokenLink &l: imp.brokenLinks)
        got << u(l.device->get_param("id")) + ":" + u(l.link) + ":" + u(l.io);
    got.sort();
    links.sort();
    QCOMPARE(got, links);
    QCOMPARE(imp.isEmpty(), links.isEmpty());
    QVERIFY(!imp.affectsRules());
}

// Its own links go with it; another device that links one of its owned IOs does not.
void tst_DeleteImpact::aDeviceDoesNotBreakItsOwnLinks()
{
    DeletionImpact imp = deletionImpact(io("dev"));

    QStringList got;
    for (const BrokenLink &l: imp.brokenLinks)
        got << u(l.device->get_param("id")) + ":" + u(l.link) + ":" + u(l.io);
    QCOMPARE(got, QStringList({ "other:mode:dev_in" }));
}

void tst_DeleteImpact::deletingAnOwnedIoAloneBreaksItsDevicesLink()
{
    DeletionImpact imp = deletionImpact(io("dev_out"));

    QCOMPARE(ids(imp.ios), QStringList({ "dev_out" }));
    QCOMPARE(imp.brokenLinks.size(), size_t(1));
    QCOMPARE(u(imp.brokenLinks[0].device->get_param("id")), QString("dev"));
    QCOMPARE(u(imp.brokenLinks[0].link), QString("output_heat"));
}

void tst_DeleteImpact::roomImpactIsTheUnionOfItsIos()
{
    onInput("in room A", io("in_b"));
    onInput("in room B", io("in_c"));

    Room *a = ListeRoom::Instance().get_room(0);
    DeletionImpact imp = deletionImpactOfRoom(a);

    const QStringList listed = ids(imp.ios);
    QCOMPARE(QSet<QString>(listed.begin(), listed.end()), roomIds(a));
    QCOMPARE(int(imp.ios.size()), roomIds(a).size()); // each IO once, though some are inputs and outputs
    QCOMPARE(names(imp.removedRules), QStringList({ "in room A" }));

    // Room B's device keeps links into room A, and into the device being deleted.
    QStringList got;
    for (const BrokenLink &l: imp.brokenLinks)
        got << u(l.device->get_param("id")) + ":" + u(l.link);
    got.sort();
    QCOMPARE(got, QStringList({ "other:current_temp", "other:mode" }));

    // Room B holds the other device: its own links are not broken.
    DeletionImpact b = deletionImpactOfRoom(ListeRoom::Instance().get_room(1));
    QCOMPARE(b.brokenLinks.size(), size_t(0));
    QCOMPARE(names(b.removedRules), QStringList({ "in room B" }));
}

// What the dialog announces is what the model then does.
void tst_DeleteImpact::impactIsWhatTheDeletionDoes()
{
    onInput("gone: condition", io("dev_in"));
    Rule *both = onInput("gone: action", io("in_b"));
    addAction(both, io("dev_out"));
    onScript("inert", { io("dev_target") });
    onScript("alive", { io("dev_target"), io("in_c") });
    Rule *orphan = onInput("orphan", io("in_c"));
    orphan->get_condition(0)->get_params_var().Add("v", "dev_in");
    onInput("untouched", io("in_c"));

    Device *dev = ListeRoom::Instance().get_device("dev");
    const World before = snap();
    DeletionImpact imp = deletionImpact(dev);
    const Announcement announced(imp);
    QCOMPARE(imp.orphanVariables.size(), size_t(1));
    QCOMPARE(announced.inert, QStringList({ "inert" }));

    QVERIFY(ListeRoom::Instance().deleteIOInput(dev));

    announced.verify(before);
    QVERIFY(ListeRoom::isPendingIOId("dev_in"));
}

void tst_DeleteImpact::roomImpactIsWhatRemovingTheRoomDoes()
{
    ListeRoom &lr = ListeRoom::Instance();
    onInput("on free", io("in_b"));
    onInput("on owned", io("dev_in"));
    Rule *act = onInput("kept: other room", io("in_c"));
    addAction(act, io("in_c"));
    onScript("inert: all triggers in the room", { io("in_a"), io("dev_target") });
    onScript("alive: trigger elsewhere", { io("in_a"), io("in_c") });
    Rule *vars = onInput("orphan", io("in_c"));
    vars->get_condition(0)->get_params_var().Add("v", "int_bool");

    Room *a = lr.get_room(0);
    const World before = snap();
    DeletionImpact imp = deletionImpactOfRoom(a);
    const Announcement announced(imp);
    QCOMPARE(announced.inert, QStringList({ "inert: all triggers in the room" }));
    QCOMPARE(imp.orphanVariables.size(), size_t(1));

    lr.Remove(a);

    announced.verify(before);
    QVERIFY(ListeRoom::isPendingIOId("int_bool"));
}

// A condition saved with trigger="false" never starts the rule: it does not keep a script rule alive.
void tst_DeleteImpact::scriptRuleWhoseOtherConditionsDoNotTriggerIsInert()
{
    Rule *quietStd = onScript("quiet std", { io("in_b") });
    Condition *std = new Condition(COND_STD);
    std->Add(io("in_c"));
    std->setTrigger(false);
    quietStd->AddCondition(std);

    Rule *quietOut = onScript("quiet output", { io("in_b") });
    Condition *oc = new Condition(COND_OUTPUT);
    oc->setOutput(io("out_light"));
    oc->setTrigger(false);
    quietOut->AddCondition(oc);

    Rule *loud = onScript("loud std", { io("in_b") });
    Condition *std2 = new Condition(COND_STD);
    std2->Add(io("in_c"));
    loud->AddCondition(std2);

    const World before = snap();
    DeletionImpact imp = deletionImpact(io("in_b"));
    const Announcement announced(imp);
    QCOMPARE(announced.inert, QStringList({ "quiet output", "quiet std" }));

    QVERIFY(ListeRoom::Instance().deleteIOInput(io("in_b")));
    announced.verify(before);
}

void tst_DeleteImpact::impactOfSeveralIosDoesNotCountTwice()
{
    Device *dev = ListeRoom::Instance().get_device("dev");
    onInput("on owned", io("dev_in"));

    // The owned IO is asked for twice: on its own and through its device.
    DeletionImpact imp = deletionImpact({ dev, io("dev_in"), dev });

    QCOMPARE(ids(imp.ios), QStringList({ "dev", "dev_target", "dev_in", "dev_out" }));
    QCOMPARE(imp.removedRules.size(), size_t(1));
}

QTEST_MAIN(tst_DeleteImpact)
#include "tst_deleteimpact.moc"
