#include <QtTest>

#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <functional>

#include "ListeRoom.h"
#include "ListeRule.h"
#include "Room.h"
#include "Rule.h"

using namespace Calaos;

namespace
{

struct ProbeIO : public IOBase
{
    std::function<void(ProbeIO *)> onDestroy;

    ProbeIO(const std::string &id, int ioType, std::function<void(ProbeIO *)> cb = nullptr):
        IOBase(params(id), "probe", TBOOL, ioType),
        onDestroy(std::move(cb))
    { }
    ProbeIO(Params ioParams, int ioType):
        IOBase(ioParams, "probe", TBOOL, ioType)
    { }
    ~ProbeIO() override
    {
        if (onDestroy)
            onDestroy(this);
    }

    static Params &params(const std::string &id)
    {
        static Params p;
        p = Params();
        p.Add("id", id);
        return p;
    }
};

Rule *ruleOnInput(const std::string &name, IOBase *io)
{
    Params rp;
    rp.Add("name", name);
    Rule *r = new Rule(rp);
    Condition *c = new Condition(COND_STD);
    c->Add(io);
    r->AddCondition(c);
    return r;
}

Rule *ruleOnOutputState(const std::string &name, IOBase *io)
{
    Params rp;
    rp.Add("name", name);
    Rule *r = new Rule(rp);
    Condition *c = new Condition(COND_OUTPUT);
    c->setOutput(io);
    r->AddCondition(c);
    return r;
}

Rule *ruleOnScript(const std::string &name, IOBase *io)
{
    Params rp;
    rp.Add("name", name);
    Rule *r = new Rule(rp);
    Condition *c = new Condition(COND_SCRIPT);
    c->addScriptInput(io);
    r->AddCondition(c);
    return r;
}

Rule *ruleOnOutput(const std::string &name, IOBase *io)
{
    Params rp;
    rp.Add("name", name);
    Rule *r = new Rule(rp);
    Action *a = new Action(ACTION_STD);
    a->Add(io);
    r->AddAction(a);
    return r;
}

struct ChildResult
{
    bool forkFailed = false;
    bool timedOut = false;
    bool exited = false;
    int code = -1;
    std::string report;
};

// The defects under test are an endless loop and use-after-free: they would take
// the whole suite down, so the body runs in a child that is killed on a timeout.
// The body returns a description of what is wrong, empty when everything holds.
ChildResult runInChild(const std::function<std::string()> &body, int timeoutMs = 5000)
{
    ChildResult res;
    int fds[2];
    if (::pipe(fds) != 0)
    {
        res.forkFailed = true;
        return res;
    }

    std::fflush(nullptr);
    const pid_t pid = ::fork();
    if (pid < 0)
    {
        ::close(fds[0]);
        ::close(fds[1]);
        res.forkFailed = true;
        return res;
    }
    if (pid == 0)
    {
        ::close(fds[0]);
        const std::string report = body();
        if (::write(fds[1], report.data(), report.size()) < 0)
            ::_exit(2);
        ::_exit(0);
    }
    ::close(fds[1]);

    for (;;)
    {
        struct pollfd pfd = { fds[0], POLLIN, 0 };
        if (::poll(&pfd, 1, timeoutMs) <= 0)
        {
            res.timedOut = true;
            ::kill(pid, SIGKILL);
            break;
        }
        char buf[256];
        const ssize_t n = ::read(fds[0], buf, sizeof(buf));
        if (n <= 0)
            break;
        res.report.append(buf, n);
    }
    ::close(fds[0]);

    int status = 0;
    ::waitpid(pid, &status, 0);
    res.exited = WIFEXITED(status);
    res.code = res.exited ? WEXITSTATUS(status) : -1;
    return res;
}

#define VERIFY_CHILD_OK(res) \
    do { \
        QVERIFY2(!(res).forkFailed, "could not fork the child"); \
        QVERIFY2(!(res).timedOut, "the room destructor never returned"); \
        QVERIFY2((res).exited && (res).code == 0, "the child crashed"); \
        QVERIFY2((res).report.empty(), (res).report.c_str()); \
    } while (0)

}

class tst_RoomTeardown : public QObject
{
    Q_OBJECT

private slots:
    void cleanup();

    void roomOutsideTheListDestroysItsIOs();
    void roomOutsideTheListDestroysAnInOutIOOnce();
    void roomDropsItsRulesBeforeItsIOsDie();
    void roomLeavesTheRulesOfOtherIOsAlone();
    void rulesOnAnotherIOWithTheSameIdSurvive();
    void aCameraTakesItsRulesWithItButNotThoseOfIOsNamedLikeIt();
    void scriptConditionLosesOnlyTheDeletedTrigger();
    void scriptConditionWithoutTriggerKeepsItsRule();
    void removingAnUnknownScriptInputChangesNothing();
    void orphanVariableIdsAreKeptAndReserved();
    void orphanVariableReservedUnderItsLegacyName();
    void emptyIdsNeverMatch();
    void removeFromTheListDestroysTheRoomsIOs();
    void clearDestroysTheRoomsIOs();
};

void tst_RoomTeardown::cleanup()
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();
    ListeRoom::clearPendingIOIds();
}

void tst_RoomTeardown::roomOutsideTheListDestroysItsIOs()
{
    const ChildResult res = runInChild([]() -> std::string
    {
        int destroyed = 0;
        auto count = [&](ProbeIO *) { destroyed++; };

        Room *room = new Room("detached", "salon");
        room->AddInput(new ProbeIO("in0", IOBase::IO_INPUT, count));
        room->AddInput(new ProbeIO("in1", IOBase::IO_INPUT, count));
        room->AddOutput(new ProbeIO("out0", IOBase::IO_OUTPUT, count));
        delete room;

        if (destroyed != 3)
            return "destroyed " + std::to_string(destroyed) + " IOs instead of 3";
        return "";
    });
    VERIFY_CHILD_OK(res);
}

void tst_RoomTeardown::roomOutsideTheListDestroysAnInOutIOOnce()
{
    const ChildResult res = runInChild([]() -> std::string
    {
        int destroyed = 0;
        Room *room = new Room("detached", "salon");
        ProbeIO *both = new ProbeIO("both", IOBase::IO_BOTH, [&](ProbeIO *) { destroyed++; });
        room->AddInput(both);
        room->AddOutput(both);
        delete room;

        if (destroyed != 1)
            return "destroyed an input and output IO " + std::to_string(destroyed) + " times";
        return "";
    });
    VERIFY_CHILD_OK(res);
}

// The rules hold raw pointers to the IOs: they must be gone before an IO dies.
void tst_RoomTeardown::roomDropsItsRulesBeforeItsIOsDie()
{
    const ChildResult res = runInChild([]() -> std::string
    {
        ListeRule &rules = ListeRule::Instance();
        std::string report;

        auto check = [&](ProbeIO *io)
        {
            if (rules.searchRuleUsing(io))
                report += "a rule still references " + io->get_param("id") + " while it dies; ";
        };

        Room *room = new Room("detached", "salon");
        ProbeIO *in = new ProbeIO("in0", IOBase::IO_INPUT, check);
        ProbeIO *out = new ProbeIO("out0", IOBase::IO_OUTPUT, check);
        ProbeIO *both = new ProbeIO("both", IOBase::IO_BOTH, check);
        room->AddInput(in);
        room->AddOutput(out);
        room->AddInput(both);
        room->AddOutput(both);

        rules.Add(ruleOnInput("on in", in));
        rules.Add(ruleOnOutput("on out", out));
        rules.Add(ruleOnInput("on both as input", both));
        rules.Add(ruleOnOutput("on both as output", both));
        rules.Add(ruleOnOutputState("on out state", out));
        rules.Add(ruleOnScript("on in script", in));
        rules.Add(ruleOnOutputState("on both state", both));
        delete room;

        // The script rule only loses its trigger.
        if (rules.size() != 1 || rules.get_rule(0)->get_name() != "on in script")
            report += std::to_string(rules.size()) + " rules survived their IOs";
        else if (rules.get_rule(0)->get_condition(0)->getScriptInputSize() != 0)
            report += "the script kept a trigger on a dead IO";
        return report;
    });
    VERIFY_CHILD_OK(res);
}

void tst_RoomTeardown::roomLeavesTheRulesOfOtherIOsAlone()
{
    const ChildResult res = runInChild([]() -> std::string
    {
        ListeRule &rules = ListeRule::Instance();

        ProbeIO outsider("outsider", IOBase::IO_INPUT);
        rules.Add(ruleOnInput("outsider rule", &outsider));

        Room *room = new Room("detached", "salon");
        ProbeIO *in = new ProbeIO("in0", IOBase::IO_INPUT);
        room->AddInput(in);
        rules.Add(ruleOnInput("room rule", in));
        delete room;

        if (rules.size() != 1 || !rules.searchRuleUsing(&outsider))
            return "the rule of an IO outside the room did not survive";
        return "";
    });
    VERIFY_CHILD_OK(res);
}

Params idParams(const char *id, const char *iid = "", const char *oid = "")
{
    Params p;
    if (*id) p.Add("id", id);
    if (*iid) p.Add("iid", iid);
    if (*oid) p.Add("oid", oid);
    return p;
}

// Rules hold the IO pointer, which is what would dangle: the other IO that
// shares the id is still alive and keeps its rules.
void tst_RoomTeardown::rulesOnAnotherIOWithTheSameIdSurvive()
{
    ListeRule &rules = ListeRule::Instance();
    ProbeIO first(idParams("dup"), IOBase::IO_INPUT), second(idParams("dup"), IOBase::IO_INPUT);
    ProbeIO other(idParams("other"), IOBase::IO_INPUT);
    rules.Add(ruleOnInput("on first", &first));
    rules.Add(ruleOnInput("on second", &second));
    rules.Add(ruleOnInput("on other", &other));

    rules.RemoveRulesUsing(&second);

    QCOMPARE(rules.size(), 2);
    QVERIFY(rules.searchRuleUsing(&first));
    QVERIFY(rules.searchRuleUsing(&other));
    QVERIFY(!rules.searchRuleUsing(&second));
}

// A camera is one IO under id, iid and oid; rules loaded from the legacy names
// hold that very pointer. Another IO that merely carries the same name is not it.
void tst_RoomTeardown::aCameraTakesItsRulesWithItButNotThoseOfIOsNamedLikeIt()
{
    ListeRule &rules = ListeRule::Instance();
    ProbeIO camera(idParams("cam", "cam_in", "cam_out"), IOBase::IO_BOTH);
    ProbeIO namesakeIn(idParams("cam_in"), IOBase::IO_INPUT);
    ProbeIO namesakeOut(idParams("cam_out"), IOBase::IO_OUTPUT);

    rules.Add(ruleOnInput("camera in", &camera));
    rules.Add(ruleOnOutput("camera out", &camera));
    rules.Add(ruleOnOutputState("camera out state", &camera));
    rules.Add(ruleOnInput("namesake in", &namesakeIn));
    rules.Add(ruleOnOutput("namesake out", &namesakeOut));
    rules.RemoveRulesUsing(&camera);

    QCOMPARE(rules.size(), 2);
    QVERIFY(rules.searchRuleUsing(&namesakeIn));
    QVERIFY(rules.searchRuleUsing(&namesakeOut));
    QVERIFY(!rules.searchRuleUsing(&camera));
}

Rule *scriptRule(const std::string &name, std::initializer_list<IOBase *> triggers)
{
    Params rp;
    rp.Add("name", name);
    Rule *r = new Rule(rp);
    Condition *c = new Condition(COND_SCRIPT);
    for (IOBase *io: triggers)
        c->addScriptInput(io);
    c->setScript("return true");
    r->AddCondition(c);
    return r;
}

void tst_RoomTeardown::scriptConditionLosesOnlyTheDeletedTrigger()
{
    ListeRule &rules = ListeRule::Instance();
    ProbeIO gone(idParams("gone"), IOBase::IO_INPUT), kept(idParams("kept"), IOBase::IO_INPUT);
    rules.Add(scriptRule("script", {&gone, &kept}));

    rules.RemoveRulesUsing(&gone);

    QCOMPARE(rules.size(), 1);
    Condition *c = rules.get_rule(0)->get_condition(0);
    QCOMPARE(c->getScriptInputSize(), 1);
    QVERIFY(c->getScriptInput(0) == &kept);
    QCOMPARE(c->getScript(), std::string("return true"));
}

// The rule and its Lua are kept so the user can attach a new trigger, but a
// script condition with none is never evaluated by the server: the rule is inert.
void tst_RoomTeardown::scriptConditionWithoutTriggerKeepsItsRule()
{
    ListeRule &rules = ListeRule::Instance();
    ProbeIO only(idParams("only"), IOBase::IO_INPUT);
    rules.Add(scriptRule("script", {&only}));

    rules.RemoveRulesUsing(&only);

    QCOMPARE(rules.size(), 1);
    Condition *c = rules.get_rule(0)->get_condition(0);
    QCOMPARE(c->getScriptInputSize(), 0);
    QCOMPARE(c->getScript(), std::string("return true"));
}

void tst_RoomTeardown::removingAnUnknownScriptInputChangesNothing()
{
    ProbeIO listed(idParams("listed"), IOBase::IO_INPUT), stranger(idParams("stranger"), IOBase::IO_INPUT);
    Condition c(COND_SCRIPT);
    c.addScriptInput(&listed);

    c.removeScriptInput(&stranger);

    QCOMPARE(c.getScriptInputSize(), 1);
}

// The variable of a comparison is stored by id only, so the rule keeps working
// once an IO with that id exists again: no new IO may take it.
void tst_RoomTeardown::orphanVariableIdsAreKeptAndReserved()
{
    ListeRule &rules = ListeRule::Instance();
    ProbeIO in(idParams("in"), IOBase::IO_INPUT), out(idParams("out"), IOBase::IO_OUTPUT);
    ProbeIO varIn(idParams("var_in"), IOBase::IO_INPUT);
    ProbeIO varOut(idParams("var_out"), IOBase::IO_OUTPUT);
    ProbeIO varAct(idParams("var_act"), IOBase::IO_OUTPUT);
    ProbeIO bystander(idParams("bystander"), IOBase::IO_INPUT);

    Rule *r = ruleOnInput("vars", &in);
    r->get_condition(0)->get_params_var().Add("in", "var_in");
    r->get_condition(0)->get_params_var().Add("alive", "var_alive");
    Condition *oc = new Condition(COND_OUTPUT);
    oc->setOutput(&out);
    oc->setOutputParamVar("var_out");
    r->AddCondition(oc);
    Action *a = new Action(ACTION_STD);
    a->Add(&out);
    a->get_params_var().Add("out", "var_act");
    r->AddAction(a);
    rules.Add(r);

    rules.RemoveRulesUsing(&bystander);
    QVERIFY(!ListeRoom::isPendingIOId("bystander"));

    rules.RemoveRulesUsing(&varIn);
    rules.RemoveRulesUsing(&varOut);
    rules.RemoveRulesUsing(&varAct);

    QCOMPARE(rules.size(), 1);
    QCOMPARE(r->get_condition(0)->get_params_var().get_param("in"), std::string("var_in"));
    QCOMPARE(oc->getOutputParamVar(), std::string("var_out"));
    QCOMPARE(a->get_params_var().get_param("out"), std::string("var_act"));
    QVERIFY(ListeRoom::isPendingIOId("var_in"));
    QVERIFY(ListeRoom::isPendingIOId("var_out"));
    QVERIFY(ListeRoom::isPendingIOId("var_act"));
    QVERIFY(!ListeRoom::isPendingIOId("var_alive"));
}

// The legacy iid/oid are names a variable may still carry in a rule.
void tst_RoomTeardown::orphanVariableReservedUnderItsLegacyName()
{
    ListeRule &rules = ListeRule::Instance();
    ProbeIO in(idParams("in"), IOBase::IO_INPUT);
    ProbeIO camera(idParams("cam", "cam_in", "cam_out"), IOBase::IO_BOTH);

    Rule *r = ruleOnInput("legacy var", &in);
    r->get_condition(0)->get_params_var().Add("in", "cam_in");
    rules.Add(r);

    rules.RemoveRulesUsing(&camera);

    QCOMPARE(rules.size(), 1);
    QVERIFY(ListeRoom::isPendingIOId("cam_in"));
}

// An IO without id is not any other IO without id.
void tst_RoomTeardown::emptyIdsNeverMatch()
{
    ListeRule &rules = ListeRule::Instance();
    ProbeIO deleted(idParams(""), IOBase::IO_INPUT), bystander(idParams(""), IOBase::IO_INPUT);
    rules.Add(ruleOnInput("bystander", &bystander));
    rules.Add(ruleOnInput("deleted", &deleted));

    rules.RemoveRulesUsing(&deleted);

    QCOMPARE(rules.size(), 1);
    QVERIFY(rules.searchRuleUsing(&bystander));
    QVERIFY(!rules.searchRuleUsing(&deleted));
}

void tst_RoomTeardown::removeFromTheListDestroysTheRoomsIOs()
{
    ListeRoom &lr = ListeRoom::Instance();
    int destroyed = 0;
    auto count = [&](ProbeIO *) { destroyed++; };

    Room *byPointer = new Room("a", "salon");
    byPointer->AddInput(new ProbeIO("a_in", IOBase::IO_INPUT, count));
    byPointer->AddOutput(new ProbeIO("a_out", IOBase::IO_OUTPUT, count));
    lr.Add(byPointer);

    Room *byIndex = new Room("b", "salon");
    byIndex->AddOutput(new ProbeIO("b_out", IOBase::IO_OUTPUT, count));
    lr.Add(byIndex);

    lr.Remove(byPointer);
    QCOMPARE(destroyed, 2);
    QCOMPARE(lr.size(), 1);

    lr.Remove(0);
    QCOMPARE(destroyed, 3);
    QCOMPARE(lr.size(), 0);
}

void tst_RoomTeardown::clearDestroysTheRoomsIOs()
{
    ListeRoom &lr = ListeRoom::Instance();
    int destroyed = 0;

    for (int i = 0;i < 3;i++)
    {
        Room *r = new Room("r" + std::to_string(i), "salon");
        ProbeIO *in = new ProbeIO("in" + std::to_string(i), IOBase::IO_INPUT, [&](ProbeIO *) { destroyed++; });
        r->AddInput(in);
        ListeRule::Instance().Add(ruleOnInput("rule" + std::to_string(i), in));
        lr.Add(r);
    }

    lr.clear();

    QCOMPARE(destroyed, 3);
    QCOMPARE(lr.size(), 0);
    QCOMPARE(ListeRule::Instance().size(), 0);
}

QTEST_APPLESS_MAIN(tst_RoomTeardown)
#include "tst_roomteardown.moc"
