#include <QtTest>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslServer>
#include <QSslSocket>
#include <QTcpServer>
#include <functional>

#include "OnlineConfig.h"
#include "OnlineSession.h"

namespace
{
const QString kSession = QString(64, 'a');

struct Request
{
    QByteArray requestLine;
    QJsonObject body;
};

// Minimal /api.php: one request per connection, JSON answers from the test's handler.
class FakeApi: public QObject
{
public:
    using Handler = std::function<QJsonObject(const QJsonObject &)>;

    explicit FakeApi(QTcpServer *server, Handler handler):
        m_server(server), m_handler(std::move(handler))
    {
        connect(m_server, &QTcpServer::pendingConnectionAvailable, this, [this]()
        {
            while (QTcpSocket *s = m_server->nextPendingConnection())
            {
                auto *buffer = new QByteArray;
                connect(s, &QObject::destroyed, s, [buffer]() { delete buffer; });
                connect(s, &QTcpSocket::readyRead, s, [this, s, buffer]() { readSome(s, *buffer); });
                connect(s, &QTcpSocket::disconnected, s, &QObject::deleteLater);
                if (s->bytesAvailable())
                    readSome(s, *buffer);
            }
        });
    }

    bool listen() { return m_server->listen(QHostAddress::LocalHost, 0); }
    quint16 port() const { return m_server->serverPort(); }

    QList<Request> requests;

    int count(const QString &action, const QString &type = QString()) const
    {
        int n = 0;
        for (const Request &r : requests)
            if (r.body.value("action").toString() == action &&
                (type.isEmpty() || r.body.value("type").toString() == type))
                n++;
        return n;
    }

private:
    void readSome(QTcpSocket *s, QByteArray &buffer)
    {
        buffer += s->readAll();
        const int end = buffer.indexOf("\r\n\r\n");
        if (end < 0)
            return;
        const QByteArray head = buffer.left(end);
        int length = 0;
        for (const QByteArray &line : head.split('\n'))
            if (line.toLower().startsWith("content-length:"))
                length = line.mid(15).trimmed().toInt();
        if (buffer.size() < end + 4 + length)
            return;

        Request r;
        r.requestLine = head.left(head.indexOf("\r\n"));
        r.body = QJsonDocument::fromJson(buffer.mid(end + 4, length)).object();
        requests << r;

        const QByteArray answer = QJsonDocument(m_handler(r.body)).toJson(QJsonDocument::Compact);
        s->write("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\nContent-Length: " +
                 QByteArray::number(answer.size()) + "\r\n\r\n" + answer);
        s->disconnectFromHost();
    }

    QTcpServer *m_server;
    Handler m_handler;
};

QJsonObject refusal(const QString &code)
{
    return QJsonObject{ { "success", "false" }, { "error", code }, { "msg", "English text" } };
}

QJsonObject configAnswer(const QJsonObject &req)
{
    if (req.value("type").toString() == "put")
        return QJsonObject{ { "success", "true" } };
    return QJsonObject{ { "success", "true" }, { "io_format", 2 },
                        { "config_files", QJsonObject{ { "io.xml", "<io/>" }, { "rules.xml", "<rules/>" } } } };
}

// calaos_server once an admin is set, reached over https.
QJsonObject adminServer(const QJsonObject &req)
{
    const QString action = req.value("action").toString();
    if (action == "admin_unlock")
    {
        if (req.value("admin_pass").toString() != "correct horse battery")
            return refusal("bad_password");
        return QJsonObject{ { "success", "true" }, { "admin_session", kSession }, { "expires_in", "900" } };
    }
    if (req.value("admin_session").toString() != kSession)
        return refusal("bad_session");
    return configAnswer(req);
}

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
    QMetaObject::Connection c = QObject::connect(&session, &OnlineSession::finished,
        [&o](bool reached, const QByteArray &body, const QString &networkError)
    {
        o.done = true;
        o.reached = reached;
        o.body = body;
        o.networkError = networkError;
    });
    call();
    if (!QTest::qWaitFor([&o]() { return o.done; }, 10000))
        qWarning() << "no answer from the session";
    QObject::disconnect(c);
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

    void saveWithoutAdmin();
    void adminPasswordUnusedWithoutAdmin();
    void unlockOnceThenSave();
    void sessionNeverInUrl();
    void missingAdminPasswordIsReadable();
    void wrongAdminPassword();
    void expiredSessionAsksAgain();
    void httpsRequiredRetriesOnce();
    void httpsRetryNotLooped();
    void httpsUnreachableKeepsRefusal();

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

void TstOnlineSession::expiredSessionAsksAgain()
{
    bool expired = false;
    FakeApi api(newHttpsServer(), [&expired](const QJsonObject &req)
    {
        if (expired && req.value("action").toString() == "config")
            return refusal("bad_session");
        return adminServer(req);
    });
    QVERIFY(api.listen());

    OnlineSession session;
    session.setServer(local(api.port(), "https"));
    session.setCredentials("user", "pass", "correct horse battery");
    QVERIFY(OnlineConfig::parseGetResponse(run(session, [&]() { session.getConfig(); }).body).ok);

    expired = true;
    const Outcome o = run(session, [&]() { session.putConfig("a", "b"); });
    QCOMPARE(OnlineConfig::refusalCode(o.body), QString("bad_session"));
    // No unlock behind the user's back.
    QCOMPARE(api.count("admin_unlock"), 1);

    expired = false;
    QVERIFY(OnlineConfig::parsePutResponse(run(session, [&]() { session.putConfig("a", "b"); }).body).ok);
    QCOMPARE(api.count("admin_unlock"), 2);
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

QTEST_GUILESS_MAIN(TstOnlineSession)
#include "tst_onlinesession.moc"
