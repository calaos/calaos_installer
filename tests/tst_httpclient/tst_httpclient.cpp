#include <QtTest>
#include <QFile>
#include <QTemporaryDir>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QUrl>

#include "HttpClient.h"

// file:// URLs keep these tests off the network.
class TstHttpClient : public QObject
{
    Q_OBJECT

private slots:
    void unwritableDestinationSetsError();
    void successClearsPreviousError();
    void getJsonParsesDocument();
    void getJsonMissingFileSetsError();
    void getJsonInvalidJsonSetsError();
    void getJsonGivesUpOnSilentServer();
    void downloadGivesUpOnSilentServer();
    void getJsonSlowButActiveServerTimesOut();
    void getJsonAbortSaysCancelled();
    void downloadAbortSaysCancelled();
    void getJsonTooLargeIsRefused();
    void downloadAnnouncedTooLargeIsRefused();
    void downloadReceivedTooLargeIsRefused();
    void downloadWritesExactContent();
    void downloadAtTheLimitSucceeds();
};

namespace
{
// Answers every request with `header`, then `body` pieces: `chunk` bytes every `intervalMs`
// (a single piece when intervalMs is 0), for at most `total` bytes.
class FakeServer : public QObject
{
public:
    FakeServer(const QByteArray &header, int chunk, int intervalMs, qint64 total)
        : m_header(header), m_chunk(chunk), m_total(total)
    {
        listen(QHostAddress::LocalHost);
        connect(&m_server, &QTcpServer::newConnection, this, [this, intervalMs]()
        {
            QTcpSocket *sock = m_server.nextPendingConnection();
            connect(sock, &QTcpSocket::readyRead, this, [this, sock, intervalMs]()
            {
                sock->readAll();
                if (m_started.contains(sock))
                    return;
                m_started.append(sock);
                sock->write(m_header);
                if (m_chunk <= 0)
                    return;
                auto *timer = new QTimer(sock);
                auto sent = std::make_shared<qint64>(0);
                connect(timer, &QTimer::timeout, sock, [this, sock, timer, sent]()
                {
                    if (*sent >= m_total || sock->state() != QAbstractSocket::ConnectedState)
                    {
                        timer->stop();
                        return;
                    }
                    sock->write(QByteArray(m_chunk, 'x'));
                    *sent += m_chunk;
                });
                timer->start(intervalMs);
            });
        });
    }

    bool listening() const { return m_server.isListening(); }
    QUrl url() const { return QUrl(QString("http://127.0.0.1:%1/x").arg(m_server.serverPort())); }

private:
    void listen(const QHostAddress &a) { m_server.listen(a); }

    QTcpServer m_server;
    QByteArray m_header;
    int m_chunk;
    qint64 m_total;
    QList<QTcpSocket *> m_started;
};
} // namespace

// Accepts connections and never answers: the case of a firewall dropping the replies.
void TstHttpClient::getJsonGivesUpOnSilentServer()
{
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));

    HttpClient http;
    http.setTransferTimeouts(300, 300);
    bool done = false;
    bool ok = true;
    http.getJson(QUrl(QString("http://127.0.0.1:%1/releases").arg(server.serverPort())),
                 [&](bool success, const QJsonDocument &) { done = true; ok = success; });
    QTRY_VERIFY_WITH_TIMEOUT(done, 3000);
    QVERIFY(!ok);
    QVERIFY(!http.lastError().isEmpty());
}

void TstHttpClient::downloadGivesUpOnSilentServer()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));

    HttpClient http;
    http.setTransferTimeouts(300, 300);
    bool done = false;
    bool ok = true;
    http.downloadFile(QUrl(QString("http://127.0.0.1:%1/a.zip").arg(server.serverPort())),
                      dir.filePath("a.zip"), [&](bool success) { done = true; ok = success; });
    QTRY_VERIFY_WITH_TIMEOUT(done, 3000);
    QVERIFY(!ok);
}

void TstHttpClient::getJsonParsesDocument()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QFile src(dir.filePath("a.json"));
    QVERIFY(src.open(QIODevice::WriteOnly));
    src.write("[{\"tag_name\":\"v1\"}]");
    src.close();

    HttpClient http;
    bool done = false;
    bool ok = false;
    QJsonDocument got;
    http.getJson(QUrl::fromLocalFile(src.fileName()),
                 [&](bool success, const QJsonDocument &doc) { done = true; ok = success; got = doc; },
                 "application/vnd.github+json");
    QTRY_VERIFY_WITH_TIMEOUT(done, 5000);
    QVERIFY(ok);
    QVERIFY(got.isArray());
    QVERIFY(http.lastError().isEmpty());
}

void TstHttpClient::getJsonMissingFileSetsError()
{
    HttpClient http;
    bool done = false;
    bool ok = true;
    http.getJson(QUrl::fromLocalFile("/nonexistent/dir/a.json"),
                 [&](bool success, const QJsonDocument &) { done = true; ok = success; });
    QTRY_VERIFY_WITH_TIMEOUT(done, 5000);
    QVERIFY(!ok);
    QVERIFY(!http.lastError().isEmpty());
}

void TstHttpClient::getJsonInvalidJsonSetsError()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QFile src(dir.filePath("bad.json"));
    QVERIFY(src.open(QIODevice::WriteOnly));
    src.write("<html>");
    src.close();

    HttpClient http;
    bool done = false;
    bool ok = true;
    http.getJson(QUrl::fromLocalFile(src.fileName()),
                 [&](bool success, const QJsonDocument &) { done = true; ok = success; });
    QTRY_VERIFY_WITH_TIMEOUT(done, 5000);
    QVERIFY(!ok);
    QVERIFY(http.lastError().contains("JSON"));
}

void TstHttpClient::unwritableDestinationSetsError()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QFile src(dir.filePath("src.bin"));
    QVERIFY(src.open(QIODevice::WriteOnly));
    src.write("data");
    src.close();

    HttpClient http;
    int calls = 0;
    bool ok = true;
    http.downloadFile(QUrl::fromLocalFile(src.fileName()),
                      dir.filePath("missing_dir/dest.bin"),
                      [&](bool success) { ++calls; ok = success; });

    QCOMPARE(calls, 1);
    QVERIFY(!ok);
    QVERIFY(!http.lastError().isEmpty());
}

void TstHttpClient::successClearsPreviousError()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QFile src(dir.filePath("src.bin"));
    QVERIFY(src.open(QIODevice::WriteOnly));
    src.write("data");
    src.close();

    HttpClient http;
    http.downloadFile(QUrl::fromLocalFile(src.fileName()),
                      dir.filePath("missing_dir/dest.bin"), [](bool) {});
    QVERIFY(!http.lastError().isEmpty());

    bool done = false;
    bool ok = false;
    http.downloadFile(QUrl::fromLocalFile(src.fileName()), dir.filePath("dest.bin"),
                      [&](bool success) { done = true; ok = success; });
    QTRY_VERIFY_WITH_TIMEOUT(done, 5000);
    QVERIFY(ok);
    QVERIFY(http.lastError().isEmpty());
}

// A peer that trickles bytes passes the inactivity timeout: only the total deadline catches it.
void TstHttpClient::getJsonSlowButActiveServerTimesOut()
{
    FakeServer server("HTTP/1.1 200 OK\r\nContent-Length: 100000\r\n\r\n", 1, 60, 100000);
    QVERIFY(server.listening());

    HttpClient http;
    http.setTransferTimeouts(400, 400);
    http.setJsonDeadline(900);
    bool done = false;
    bool ok = true;
    http.getJson(server.url(), [&](bool success, const QJsonDocument &) { done = true; ok = success; });
    QTRY_VERIFY_WITH_TIMEOUT(done, 4000);
    QVERIFY(!ok);
    QVERIFY2(http.lastError().contains("Timed out"), qPrintable(http.lastError()));
    QVERIFY(!http.lastError().contains("canceled", Qt::CaseInsensitive));
}

void TstHttpClient::getJsonAbortSaysCancelled()
{
    FakeServer server("", 0, 0, 0);
    QVERIFY(server.listening());

    HttpClient http;
    bool done = false;
    bool ok = true;
    http.getJson(server.url(), [&](bool success, const QJsonDocument &) { done = true; ok = success; });
    QTimer::singleShot(100, &http, [&]() { http.abort(); });
    QTRY_VERIFY_WITH_TIMEOUT(done, 3000);
    QVERIFY(!ok);
    QCOMPARE(http.lastError(), QString("Cancelled"));
}

void TstHttpClient::downloadAbortSaysCancelled()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    FakeServer server("HTTP/1.1 200 OK\r\nContent-Length: 100000\r\n\r\n", 1, 50, 100000);
    QVERIFY(server.listening());

    HttpClient http;
    bool done = false;
    bool ok = true;
    http.downloadFile(server.url(), dir.filePath("a.bin"), [&](bool success) { done = true; ok = success; });
    QTimer::singleShot(200, &http, [&]() { http.abort(); });
    QTRY_VERIFY_WITH_TIMEOUT(done, 3000);
    QVERIFY(!ok);
    QCOMPARE(http.lastError(), QString("Cancelled"));
    QVERIFY(!QFileInfo::exists(dir.filePath("a.bin")));
}

void TstHttpClient::getJsonTooLargeIsRefused()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QFile src(dir.filePath("big.json"));
    QVERIFY(src.open(QIODevice::WriteOnly));
    src.write("[\"" + QByteArray(5000, 'a') + "\"]");
    src.close();

    HttpClient http;
    http.setMaxJsonBytes(1000);
    bool done = false;
    bool ok = true;
    http.getJson(QUrl::fromLocalFile(src.fileName()),
                 [&](bool success, const QJsonDocument &) { done = true; ok = success; });
    QTRY_VERIFY_WITH_TIMEOUT(done, 5000);
    QVERIFY(!ok);
    QVERIFY2(http.lastError().contains("larger"), qPrintable(http.lastError()));
}

// The announced size alone is enough: the server never sends a body byte.
void TstHttpClient::downloadAnnouncedTooLargeIsRefused()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    FakeServer server("HTTP/1.1 200 OK\r\nContent-Length: 5000000\r\n\r\n", 0, 0, 0);
    QVERIFY(server.listening());

    HttpClient http;
    http.setTransferTimeouts(10000, 10000);
    bool done = false;
    bool ok = true;
    http.downloadFile(server.url(), dir.filePath("a.bin"),
                      [&](bool success) { done = true; ok = success; }, 1000);
    QTRY_VERIFY_WITH_TIMEOUT(done, 2000);
    QVERIFY(!ok);
    QVERIFY2(http.lastError().contains("larger"), qPrintable(http.lastError()));
    QVERIFY(!QFileInfo::exists(dir.filePath("a.bin")));
}

// No Content-Length: only the bytes received can tell.
void TstHttpClient::downloadReceivedTooLargeIsRefused()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    FakeServer server("HTTP/1.1 200 OK\r\n\r\n", 500, 20, 1000000);
    QVERIFY(server.listening());

    HttpClient http;
    http.setTransferTimeouts(10000, 10000);
    bool done = false;
    bool ok = true;
    http.downloadFile(server.url(), dir.filePath("a.bin"),
                      [&](bool success) { done = true; ok = success; }, 2000);
    QTRY_VERIFY_WITH_TIMEOUT(done, 4000);
    QVERIFY(!ok);
    QVERIFY2(http.lastError().contains("larger"), qPrintable(http.lastError()));
    QVERIFY(!QFileInfo::exists(dir.filePath("a.bin")));
}

void TstHttpClient::downloadWritesExactContent()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QByteArray payload;
    for (int i = 0; i < 100000; ++i)
        payload.append(char(i * 7 + i / 251));
    QFile src(dir.filePath("src.bin"));
    QVERIFY(src.open(QIODevice::WriteOnly));
    src.write(payload);
    src.close();

    HttpClient http;
    bool done = false;
    bool ok = false;
    http.downloadFile(QUrl::fromLocalFile(src.fileName()), dir.filePath("dest.bin"),
                      [&](bool success) { done = true; ok = success; });
    QTRY_VERIFY_WITH_TIMEOUT(done, 5000);
    QVERIFY(ok);
    QFile dest(dir.filePath("dest.bin"));
    QVERIFY(dest.open(QIODevice::ReadOnly));
    QVERIFY(dest.readAll() == payload);
}

void TstHttpClient::downloadAtTheLimitSucceeds()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QFile src(dir.filePath("src.bin"));
    QVERIFY(src.open(QIODevice::WriteOnly));
    src.write(QByteArray(4096, 'z'));
    src.close();

    HttpClient http;
    bool done = false;
    bool ok = false;
    http.downloadFile(QUrl::fromLocalFile(src.fileName()), dir.filePath("dest.bin"),
                      [&](bool success) { done = true; ok = success; }, 4096);
    QTRY_VERIFY_WITH_TIMEOUT(done, 5000);
    QVERIFY2(ok, qPrintable(http.lastError()));
    QCOMPARE(QFileInfo(dir.filePath("dest.bin")).size(), qint64(4096));
}

QTEST_MAIN(TstHttpClient)
#include "tst_httpclient.moc"
