#include <QtTest>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslServer>
#include <QSslSocket>
#include <QTcpServer>
#include <functional>

#include "AdminCache.h"
#include "OnlineConfig.h"
#include "OnlineSession.h"

#include "FakeApi.h"

namespace
{
struct Outcome
{
    bool done = false;
    bool reached = false;
    QByteArray body;
    QString networkError;
};

Outcome run(OnlineSession &session, const std::function<void()> &call)
{
    Outcome o;
    auto done = [&o](bool reached, const QByteArray &body, const QString &networkError)
    {
        o.done = true;
        o.reached = reached;
        o.body = body;
        o.networkError = networkError;
    };
    QMetaObject::Connection g = QObject::connect(&session, &OnlineSession::getFinished, done);
    QMetaObject::Connection p = QObject::connect(&session, &OnlineSession::putFinished, done);
    call();
    if (!QTest::qWaitFor([&o]() { return o.done; }, 15000))
        qWarning() << "no answer from the session";
    QObject::disconnect(g);
    QObject::disconnect(p);
    return o;
}

QString local(quint16 port, const char *scheme = "http")
{
    return QString("%1://127.0.0.1:%2/api.php").arg(scheme).arg(port);
}
}

class TstOnlineSession: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void saveWithoutAdmin();
    void adminPasswordUnusedWithoutAdmin();
    void unlockOnceThenSave();
    void sessionNeverInUrl();
    void missingAdminPasswordIsReadable();
    void wrongAdminPassword();
    void expiredSessionUnlocksOnce();
    void expiredSessionWithRefusedPasswordAsksAgain();
    void secondSessionReusesUnlock();
    void secondSessionKeepsPasswordWhenSessionExpired();
    void wrongPasswordDropsCache();
    void otherHostOrUserIsNotServedFromCache();
    void typedPasswordReplacesCached();
    void invalidateAndClear();
    void httpsRequiredRetriesOnce();
    void httpsRetryNotLooped();
    void httpsUnreachableKeepsRefusal();
    void noAdminBodiesUnchanged();
    void failureAfterRetryIsNotTheOldRefusal();
    void unlockWithoutSessionIsAnError();
    void adminPasswordNeverInClear();
    void adminPasswordRefusedWithoutHttps();
    void unlockTimesOut();

private:
    QSslServer *newHttpsServer();
    QSslConfiguration m_tls;
};

void TstOnlineSession::initTestCase()
{
    QFile cert(":/test_cert.pem"), key(":/test_key.pem");
    QVERIFY(cert.open(QIODevice::ReadOnly));
    QVERIFY(key.open(QIODevice::ReadOnly));
    m_tls = QSslConfiguration::defaultConfiguration();
    m_tls.setLocalCertificate(QSslCertificate(cert.readAll()));
    m_tls.setPrivateKey(QSslKey(key.readAll(), QSsl::Ec));
}

void TstOnlineSession::init()
{
    AdminCache::instance().clear();
}

QSslServer *TstOnlineSession::newHttpsServer()
{
    auto *s = new QSslServer(this);
    s->setSslConfiguration(m_tls);
    return s;
}

void TstOnlineSession::saveWithoutAdmin()
{
    FakeApi api(new QTcpServer(this), configAnswer);
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port()));
    session.setCredentials("user", "pass", QString());

    const Outcome get = run(session, [&]() { session.getConfig(); });
    QVERIFY(get.reached);
    QVERIFY(OnlineConfig::parseGetResponse(get.body).ok);
    const Outcome put = run(session, [&]() { session.putConfig("<io/>", "<rules/>"); });
    QVERIFY(put.reached);
    QVERIFY(OnlineConfig::parsePutResponse(put.body).ok);

    QCOMPARE(api.requests.size(), 2);
    QCOMPARE(api.count("config", "get"), 1);
    QCOMPARE(api.count("config", "put"), 1);
    for (const Request &r : api.requests)
    {
        QVERIFY(!r.body.contains("admin_session"));
        QCOMPARE(r.body.value("cn_user").toString(), QString("user"));
    }
}

void TstOnlineSession::adminPasswordUnusedWithoutAdmin()
{
    // An old server, or one with no admin set: the field must change nothing.
    FakeApi api(new QTcpServer(this), configAnswer);
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port()));
    session.setCredentials("user", "pass", "correct horse battery");
    QVERIFY(run(session, [&]() { session.getConfig(); }).reached);
    QVERIFY(run(session, [&]() { session.putConfig("<io/>", "<rules/>"); }).reached);

    QCOMPARE(api.count("admin_unlock"), 0);
    QCOMPARE(api.requests.size(), 2);
    for (const Request &r : api.requests)
        QVERIFY(!r.body.contains("admin_session") && !r.body.contains("admin_pass"));
}

void TstOnlineSession::unlockOnceThenSave()
{
    FakeApi api(newHttpsServer(), adminServer);
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port(), "https"));
    session.setCredentials("user", "pass", "correct horse battery");

    const Outcome get = run(session, [&]() { session.getConfig(); });
    QVERIFY(get.reached);
    QVERIFY(OnlineConfig::parseGetResponse(get.body).ok);
    const Outcome put = run(session, [&]() { session.putConfig("<io/>", "<rules/>"); });
    QVERIFY(OnlineConfig::parsePutResponse(put.body).ok);

    QCOMPARE(api.count("admin_unlock"), 1);
    // get refused, unlock, get, put
    QCOMPARE(api.requests.size(), 4);
    QVERIFY(!api.requests[0].body.contains("admin_session"));
    QCOMPARE(api.requests[1].body.value("admin_pass").toString(), QString("correct horse battery"));
    QCOMPARE(api.requests[2].body.value("admin_session").toString(), kSession);
    QCOMPARE(api.requests[3].body.value("type").toString(), QString("put"));
    QCOMPARE(api.requests[3].body.value("admin_session").toString(), kSession);
    for (const Request &r : api.requests)
        if (r.body.value("action").toString() == "config")
            QVERIFY(!r.body.contains("admin_pass"));
}

void TstOnlineSession::sessionNeverInUrl()
{
    FakeApi api(newHttpsServer(), adminServer);
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port(), "https"));
    session.setCredentials("user", "pass", "correct horse battery");
    run(session, [&]() { session.getConfig(); });

    QVERIFY(api.requests.size() >= 3);
    for (const Request &r : api.requests)
    {
        QCOMPARE(r.requestLine, QByteArray("POST /api.php HTTP/1.1"));
        QVERIFY(!r.requestLine.contains(kSession.toLatin1()));
    }
}

void TstOnlineSession::missingAdminPasswordIsReadable()
{
    FakeApi api(newHttpsServer(), adminServer);
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port(), "https"));
    session.setCredentials("user", "pass", QString());
    const Outcome o = run(session, [&]() { session.getConfig(); });

    QCOMPARE(api.count("admin_unlock"), 0);
    const OnlineConfig::GetResult r = OnlineConfig::parseGetResponse(o.body);
    QVERIFY(!r.ok);
    QVERIFY2(r.error.contains("Admin password"), qPrintable(r.error));
}

void TstOnlineSession::wrongAdminPassword()
{
    FakeApi api(newHttpsServer(), adminServer);
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port(), "https"));
    session.setCredentials("user", "pass", "wrong");
    const Outcome o = run(session, [&]() { session.getConfig(); });

    QCOMPARE(api.count("admin_unlock"), 1);
    QCOMPARE(api.count("config"), 1);
    QCOMPARE(OnlineConfig::parseGetResponse(o.body).error, QString("Wrong admin password."));

    // Corrected by the user: one more unlock, then the session is kept.
    session.setCredentials("user", "pass", "correct horse battery");
    QVERIFY(OnlineConfig::parseGetResponse(run(session, [&]() { session.getConfig(); }).body).ok);
    QVERIFY(OnlineConfig::parsePutResponse(run(session, [&]() { session.putConfig("a", "b"); }).body).ok);
    QCOMPARE(api.count("admin_unlock"), 2);
}

void TstOnlineSession::expiredSessionUnlocksOnce()
{
    bool expired = false;
    FakeApi api(newHttpsServer(), [&expired](const QJsonObject &req)
    {
        if (expired && req.value("action").toString() == "config" &&
            req.value("admin_session").toString() == kSession)
        {
            expired = false;
            return refusal("bad_session");
        }
        return adminServer(req);
    });
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port(), "https"));
    session.setCredentials("user", "pass", "correct horse battery");
    QVERIFY(OnlineConfig::parseGetResponse(run(session, [&]() { session.getConfig(); }).body).ok);
    QCOMPARE(api.count("admin_unlock"), 1);

    expired = true;
    const Outcome o = run(session, [&]() { session.putConfig("a", "b"); });
    QVERIFY(OnlineConfig::parsePutResponse(o.body).ok);
    QCOMPARE(api.count("admin_unlock"), 2);
}

void TstOnlineSession::expiredSessionWithRefusedPasswordAsksAgain()
{
    // The session is refused whatever happens: one unlock, then the refusal is shown, no loop.
    FakeApi api(newHttpsServer(), [](const QJsonObject &req)
    {
        if (req.value("action").toString() == "config")
            return refusal("bad_session");
        return adminServer(req);
    });
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port(), "https"));
    session.setCredentials("user", "pass", "correct horse battery");
    const Outcome o = run(session, [&]() { session.getConfig(); });
    QCOMPARE(OnlineConfig::refusalCode(o.body), QString("bad_session"));
    QCOMPARE(api.count("admin_unlock"), 1);
}

void TstOnlineSession::secondSessionReusesUnlock()
{
    FakeApi api(newHttpsServer(), adminServer);
    QVERIFY(api.listen());
    const QString host = local(api.port(), "https");

    {
        OnlineSession first;
        first.setServer(host);
        first.setCredentials("user", "pass", "correct horse battery");
        QVERIFY(OnlineConfig::parseGetResponse(run(first, [&]() { first.getConfig(); }).body).ok);
    }
    QCOMPARE(api.count("admin_unlock"), 1);
    const int before = api.requests.size();

    // A new dialog: no password typed, the first request already carries the session.
    OnlineSession second;
    second.setServer(host);
    second.setCredentials("user", "pass", QString());
    QVERIFY(OnlineConfig::parsePutResponse(run(second, [&]() { second.putConfig("a", "b"); }).body).ok);

    QCOMPARE(api.count("admin_unlock"), 1);
    QCOMPARE(api.requests.size(), before + 1);
    QCOMPARE(api.requests.last().body.value("admin_session").toString(), kSession);
}

void TstOnlineSession::secondSessionKeepsPasswordWhenSessionExpired()
{
    bool expired = false;
    FakeApi api(newHttpsServer(), [&expired](const QJsonObject &req)
    {
        if (expired && req.value("action").toString() == "config")
        {
            expired = false;
            return refusal("bad_session");
        }
        return adminServer(req);
    });
    QVERIFY(api.listen());
    const QString host = local(api.port(), "https");

    {
        OnlineSession first;
        first.setServer(host);
        first.setCredentials("user", "pass", "correct horse battery");
        QVERIFY(run(first, [&]() { first.getConfig(); }).reached);
    }
    expired = true;
    OnlineSession second;
    second.setServer(host);
    second.setCredentials("user", "pass", QString());
    QVERIFY(OnlineConfig::parseGetResponse(run(second, [&]() { second.getConfig(); }).body).ok);
    QCOMPARE(api.count("admin_unlock"), 2);
    QCOMPARE(api.requests.last().body.value("admin_session").toString(), kSession);
}

void TstOnlineSession::wrongPasswordDropsCache()
{
    bool changed = false;
    FakeApi api(newHttpsServer(), [&changed](const QJsonObject &req)
    {
        // The box admin password was changed behind our back: sessions and password are stale.
        if (changed)
            return req.value("action").toString() == "admin_unlock" ? refusal("bad_password")
                                                                    : refusal("bad_session");
        return adminServer(req);
    });
    QVERIFY(api.listen());
    const QString host = local(api.port(), "https");

    OnlineSession session;
    session.setServer(host);
    session.setCredentials("user", "pass", "correct horse battery");
    QVERIFY(run(session, [&]() { session.getConfig(); }).reached);
    QVERIFY(AdminCache::instance().has(host, "user"));

    changed = true;
    const Outcome o = run(session, [&]() { session.getConfig(); });
    QCOMPARE(OnlineConfig::refusalCode(o.body), QString("bad_password"));
    QVERIFY(!AdminCache::instance().has(host, "user"));

    // Nothing is retried on its own with the refused password.
    session.setCredentials("user", "pass", QString());
    const int unlocks = api.count("admin_unlock");
    run(session, [&]() { session.getConfig(); });
    QCOMPARE(api.count("admin_unlock"), unlocks);
}

void TstOnlineSession::otherHostOrUserIsNotServedFromCache()
{
    FakeApi api(newHttpsServer(), adminServer);
    QVERIFY(api.listen());
    FakeApi other(newHttpsServer(), adminServer);
    QVERIFY(other.listen());
    const QString host = local(api.port(), "https");

    OnlineSession session;
    session.setServer(host);
    session.setCredentials("user", "pass", "correct horse battery");
    QVERIFY(run(session, [&]() { session.getConfig(); }).reached);

    OnlineSession otherHost;
    otherHost.setServer(local(other.port(), "https"));
    otherHost.setCredentials("user", "pass", QString());
    QCOMPARE(OnlineConfig::refusalCode(run(otherHost, [&]() { otherHost.getConfig(); }).body),
             QString("bad_session"));
    QCOMPARE(other.count("admin_unlock"), 0);
    QVERIFY(!other.requests[0].body.contains("admin_session"));

    // Another user on the same box: the cached entry is dropped, not lent.
    OnlineSession otherUser;
    otherUser.setServer(host);
    otherUser.setCredentials("someone", "pass", QString());
    QVERIFY(!AdminCache::instance().has(host, "user"));
    const int unlocks = api.count("admin_unlock");
    run(otherUser, [&]() { otherUser.getConfig(); });
    QCOMPARE(api.count("admin_unlock"), unlocks);
}

void TstOnlineSession::typedPasswordReplacesCached()
{
    FakeApi api(newHttpsServer(), adminServer);
    QVERIFY(api.listen());
    const QString host = local(api.port(), "https");
    AdminCache::instance().store(host, "user", "old password", kSession);

    OnlineSession session;
    session.setServer(host);
    session.setCredentials("user", "pass", "correct horse battery");
    // The stale session is not sent with a password typed to replace it.
    const Outcome o = run(session, [&]() { session.getConfig(); });
    QVERIFY(OnlineConfig::parseGetResponse(o.body).ok);
    QCOMPARE(api.count("admin_unlock"), 1);
    QCOMPARE(AdminCache::instance().get(host, "user").adminPass, QString("correct horse battery"));
}

void TstOnlineSession::invalidateAndClear()
{
    AdminCache &c = AdminCache::instance();
    c.store("a", "u", "pw", "s");
    c.store("b", "u", "pw", "s");
    QVERIFY(c.has("a", "u"));
    c.invalidate("a");
    QVERIFY(!c.has("a", "u"));
    QVERIFY(c.has("b", "u"));
    c.dropSession("b");
    QVERIFY(c.get("b", "u").session.isEmpty());
    QCOMPARE(c.get("b", "u").adminPass, QString("pw"));
    c.clear();
    QVERIFY(!c.has("b", "u"));
}

void TstOnlineSession::httpsRequiredRetriesOnce()
{
    FakeApi plain(new QTcpServer(this), [](const QJsonObject &) { return refusal("https_required"); });
    QVERIFY(plain.listen());
    FakeApi https(newHttpsServer(), adminServer);
    QVERIFY(https.listen());

    OnlineSession session;
    session.setHttpsPort(https.port());
    session.setServer(local(plain.port()));
    session.setCredentials("user", "pass", "correct horse battery");

    const Outcome get = run(session, [&]() { session.getConfig(); });
    QVERIFY(OnlineConfig::parseGetResponse(get.body).ok);
    QCOMPARE(session.url(), QUrl(local(https.port(), "https")));
    QVERIFY(OnlineConfig::parsePutResponse(run(session, [&]() { session.putConfig("a", "b"); }).body).ok);

    // The rewrite sticks for the dialog: the put goes straight to https.
    QCOMPARE(plain.requests.size(), 1);
    QCOMPARE(https.count("admin_unlock"), 1);
    QCOMPARE(https.count("config", "put"), 1);
}

void TstOnlineSession::httpsRetryNotLooped()
{
    FakeApi plain(new QTcpServer(this), [](const QJsonObject &) { return refusal("https_required"); });
    QVERIFY(plain.listen());
    FakeApi https(newHttpsServer(), [](const QJsonObject &) { return refusal("https_required"); });
    QVERIFY(https.listen());

    OnlineSession session;
    session.setHttpsPort(https.port());
    session.setServer(local(plain.port()));
    session.setCredentials("user", "pass", QString());
    const Outcome o = run(session, [&]() { session.getConfig(); });

    QCOMPARE(OnlineConfig::refusalCode(o.body), QString("https_required"));
    QCOMPARE(plain.requests.size(), 1);
    QCOMPARE(https.requests.size(), 1);
}

void TstOnlineSession::httpsUnreachableKeepsRefusal()
{
    FakeApi plain(new QTcpServer(this), [](const QJsonObject &) { return refusal("https_required"); });
    QVERIFY(plain.listen());
    QTcpServer closed;
    QVERIFY(closed.listen(QHostAddress::LocalHost, 0));
    const quint16 port = closed.serverPort();
    closed.close();

    OnlineSession session;
    session.setHttpsPort(port);
    session.setServer(local(plain.port()));
    session.setCredentials("user", "pass", QString());
    const Outcome o = run(session, [&]() { session.getConfig(); });

    QVERIFY(o.reached);
    QCOMPARE(OnlineConfig::refusalCode(o.body), QString("https_required"));
}

void TstOnlineSession::noAdminBodiesUnchanged()
{
    FakeApi api(new QTcpServer(this), configAnswer);
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port()));
    session.setCredentials("user", "pass", "correct horse battery");
    run(session, [&]() { session.getConfig(); });
    run(session, [&]() { session.putConfig("<io/>", "<rules/>"); });

    // What the dialogs sent before the admin password existed.
    QCOMPARE(api.requests.size(), 2);
    QCOMPARE(api.requests[0].raw, QByteArray("{\n"
                                             "    \"action\": \"config\",\n"
                                             "    \"cn_pass\": \"pass\",\n"
                                             "    \"cn_user\": \"user\",\n"
                                             "    \"type\": \"get\"\n"
                                             "}\n"));
    QCOMPARE(api.requests[1].raw, QByteArray("{\n"
                                             "    \"action\": \"config\",\n"
                                             "    \"cn_pass\": \"pass\",\n"
                                             "    \"cn_user\": \"user\",\n"
                                             "    \"config_files\": {\n"
                                             "        \"io.xml\": \"<io/>\",\n"
                                             "        \"rules.xml\": \"<rules/>\"\n"
                                             "    },\n"
                                             "    \"io_format\": 2,\n"
                                             "    \"type\": \"put\"\n"
                                             "}\n"));
}

void TstOnlineSession::failureAfterRetryIsNotTheOldRefusal()
{
    FakeApi plain(new QTcpServer(this), [](const QJsonObject &) { return refusal("https_required"); });
    QVERIFY(plain.listen());
    FakeApi https(newHttpsServer(), [](const QJsonObject &req)
    {
        if (req.value("action").toString() == "admin_unlock")
            return kAbort;
        return adminServer(req);
    });
    QVERIFY(https.listen());

    OnlineSession session;
    session.setHttpsPort(https.port());
    session.setServer(local(plain.port()));
    session.setCredentials("user", "pass", "correct horse battery");
    const Outcome o = run(session, [&]() { session.getConfig(); });

    QCOMPARE(https.count("admin_unlock"), 1);
    QVERIFY(!o.reached);
    QVERIFY(!o.networkError.isEmpty());
}

void TstOnlineSession::unlockWithoutSessionIsAnError()
{
    FakeApi api(newHttpsServer(), [](const QJsonObject &req)
    {
        if (req.value("action").toString() == "admin_unlock")
            return QJsonObject{ { "success", "true" }, { "admin_session", "" } };
        return adminServer(req);
    });
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port(), "https"));
    session.setCredentials("user", "pass", "correct horse battery");
    const Outcome o = run(session, [&]() { session.getConfig(); });

    QVERIFY(o.reached);
    const OnlineConfig::GetResult r = OnlineConfig::parseGetResponse(o.body);
    QVERIFY(!r.ok);
    QCOMPARE(OnlineConfig::refusalCode(o.body), QString("no_admin_session"));
    QCOMPARE(api.count("config"), 1);
}

void TstOnlineSession::adminPasswordNeverInClear()
{
    // A server that does not say https_required first: the unlock still goes over https.
    FakeApi plain(new QTcpServer(this), [](const QJsonObject &) { return refusal("bad_session"); });
    QVERIFY(plain.listen());
    FakeApi https(newHttpsServer(), adminServer);
    QVERIFY(https.listen());

    OnlineSession session;
    session.setHttpsPort(https.port());
    session.setServer(local(plain.port()));
    session.setCredentials("user", "pass", "correct horse battery");
    QVERIFY(OnlineConfig::parseGetResponse(run(session, [&]() { session.getConfig(); }).body).ok);

    QCOMPARE(plain.requests.size(), 1);
    QVERIFY(!plain.requests[0].body.contains("admin_pass"));
    QCOMPARE(https.count("admin_unlock"), 1);
}

void TstOnlineSession::adminPasswordRefusedWithoutHttps()
{
    // https cannot be reached: the admin password is not sent in clear instead.
    FakeApi plain(new QTcpServer(this), [](const QJsonObject &) { return refusal("bad_session"); });
    QVERIFY(plain.listen());

    OnlineSession session;
    session.setServer(local(plain.port()));
    session.setCredentials("user", "pass", "correct horse battery");
    QTcpServer closed;
    QVERIFY(closed.listen(QHostAddress::LocalHost, 0));
    session.setHttpsPort(closed.serverPort());
    closed.close();
    const Outcome o = run(session, [&]() { session.getConfig(); });

    QCOMPARE(plain.count("admin_unlock"), 0);
    QVERIFY(!o.reached);
}

void TstOnlineSession::unlockTimesOut()
{
    FakeApi api(newHttpsServer(), [](const QJsonObject &req)
    {
        if (req.value("action").toString() == "admin_unlock")
            return kHang;
        return adminServer(req);
    });
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port(), "https"));
    session.setCredentials("user", "pass", "correct horse battery");
    QElapsedTimer t;
    t.start();
    const Outcome o = run(session, [&]() { session.getConfig(); });

    QVERIFY(o.done);
    QVERIFY(!o.reached);
    QVERIFY(t.elapsed() >= OnlineSession::kUnlockTimeoutMs - 500);
    QVERIFY(t.elapsed() < OnlineSession::kUnlockTimeoutMs + 5000);
}

QTEST_GUILESS_MAIN(TstOnlineSession)
#include "tst_onlinesession.moc"
