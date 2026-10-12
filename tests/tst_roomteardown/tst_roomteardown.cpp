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
        return res;

    std::fflush(nullptr);
    const pid_t pid = ::fork();
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
    void removeFromTheListDestroysTheRoomsIOs();
    void clearDestroysTheRoomsIOs();
};

void tst_RoomTeardown::cleanup()
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();
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
            if (rules.searchRuleInput(io) || rules.searchRuleOutput(io))
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
        delete room;

        if (rules.size() != 0)
            report += std::to_string(rules.size()) + " rules survived their IOs";
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

        if (rules.size() != 1 || !rules.searchRuleInput(&outsider))
            return "the rule of an IO outside the room did not survive";
        return "";
    });
    VERIFY_CHILD_OK(res);
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
