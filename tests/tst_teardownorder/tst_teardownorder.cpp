#include <QtTest>

#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>

#include "ListeRoom.h"
#include "ListeRule.h"
#include "Room.h"
#include "Rule.h"

using namespace Calaos;

// ~ListeRoom drops the IOs of the rooms left at exit from ListeRule, so ListeRule
// must outlive it whichever of the two a program touches first. The parent never
// touches the model: the child must meet them as a fresh process would.

namespace
{

int probeFd = -1;

void mark(char c)
{
    if (probeFd >= 0 && ::write(probeFd, &c, 1) != 1)
        probeFd = -1;
}

struct ProbeIO : public IOBase
{
    ProbeIO(Params &p):
        IOBase(p, "probe", TBOOL, IOBase::IO_INPUT)
    { }
    ~ProbeIO() override { mark('I'); }
};

struct ProbeRule : public Rule
{
    ProbeRule(Params p):
        Rule(p)
    { }
    ~ProbeRule() override { mark('R'); }
};

[[noreturn]] void runChild(int fd, bool ruleListFirst)
{
    probeFd = fd;

    Params rp;
    rp.Add("name", "probe");
    Params ip;
    ip.Add("id", "probe_io");

    if (ruleListFirst)
        ListeRule::Instance().Add(new ProbeRule(rp));

    Room *room = new Room("room", "salon");
    ListeRoom::Instance().Add(room);
    room->AddInput(new ProbeIO(ip));

    if (!ruleListFirst)
        ListeRule::Instance().Add(new ProbeRule(rp));

    // exit(), not _exit(): the static destructors are the subject.
    std::exit(0);
}

}

class tst_TeardownOrder : public QObject
{
    Q_OBJECT

private slots:
    void roomsLeftAtExitDieBeforeTheRuleList_data();
    void roomsLeftAtExitDieBeforeTheRuleList();
};

void tst_TeardownOrder::roomsLeftAtExitDieBeforeTheRuleList_data()
{
    QTest::addColumn<bool>("ruleListFirst");
    QTest::newRow("room list touched first") << false;
    QTest::newRow("rule list touched first") << true;
}

void tst_TeardownOrder::roomsLeftAtExitDieBeforeTheRuleList()
{
    QFETCH(bool, ruleListFirst);

    int fds[2];
    QCOMPARE(::pipe(fds), 0);

    std::fflush(nullptr);
    const pid_t pid = ::fork();
    QVERIFY(pid >= 0);
    if (pid == 0)
    {
        ::close(fds[0]);
        runChild(fds[1], ruleListFirst);
    }
    ::close(fds[1]);

    std::string order;
    bool timedOut = false;
    for (;;)
    {
        struct pollfd pfd = { fds[0], POLLIN, 0 };
        if (::poll(&pfd, 1, 30000) <= 0)
        {
            timedOut = true;
            ::kill(pid, SIGKILL);
            break;
        }
        char c;
        if (::read(fds[0], &c, 1) != 1)
            break;
        order += c;
    }
    ::close(fds[0]);

    int status = 0;
    ::waitpid(pid, &status, 0);

    QVERIFY2(!timedOut, order.c_str());
    QVERIFY2(WIFEXITED(status), ("the child died at exit, order seen: " + order).c_str());
    QCOMPARE(WEXITSTATUS(status), 0);
    QCOMPARE(QString::fromStdString(order), QString("IR"));
}

QTEST_APPLESS_MAIN(tst_TeardownOrder)
#include "tst_teardownorder.moc"
