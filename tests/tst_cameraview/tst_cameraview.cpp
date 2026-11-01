#include <QtTest>
#include <QBuffer>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslKey>
#include <QSslServer>
#include <QSslSocket>
#include <QTcpServer>
#include <QTemporaryDir>
#include <functional>
#include <memory>

#include "IOBase.h"
#include "dialogcameraview.h"

// The preview is run against real TLS servers on 127.0.0.1. Their certificates
// are generated into a temporary directory by the openssl binary and never
// stored in the repository.

namespace
{
QStringList g_log;
QtMessageHandler g_prevHandler = nullptr;

void captureLog(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
{
    g_log << msg;
    if (g_prevHandler)
        g_prevHandler(type, ctx, msg);
}

// The preview reports errors through a modal QMessageBox::warning; record its
// text and dismiss it as soon as it is shown.
class BoxCatcher : public QObject
{
public:
    QStringList texts;

    bool eventFilter(QObject *o, QEvent *e) override
    {
        if (e->type() == QEvent::Show)
            if (auto *box = qobject_cast<QMessageBox *>(o))
            {
                texts << box->text();
                QTimer::singleShot(0, box, [box]() { box->accept(); });
            }
        return false;
    }
};

class FakeCam : public QObject
{
public:
    // The first okResponses requests get a JPEG, the following ones a 404:
    // the preview re-requests itself after each image, this ends the loop.
    FakeCam(const QSslConfiguration *tls, int okResponses, QObject *parent)
        : QObject(parent), m_ok(okResponses)
    {
        QImage img(8, 8, QImage::Format_RGB32);
        img.fill(Qt::red);
        QBuffer buf(&m_jpeg);
        buf.open(QIODevice::WriteOnly);
        img.save(&buf, "JPG");

        if (tls)
        {
            auto *s = new QSslServer(this);
            s->setSslConfiguration(*tls);
            m_srv = s;
        }
        else
            m_srv = new QTcpServer(this);
        connect(m_srv, &QTcpServer::pendingConnectionAvailable, this, &FakeCam::accept);
    }

    bool listen(const QHostAddress &addr = QHostAddress::LocalHost) { return m_srv->listen(addr); }
    quint16 port() const { return m_srv->serverPort(); }
    bool jpegValid() const { return !m_jpeg.isEmpty(); }

    // When set, builds the whole HTTP response from the request head and the
    // 1-based request number.
    std::function<QByteArray(const QByteArray &, int)> handler;
    const QByteArray &jpeg() const { return m_jpeg; }

    int requests = 0;

    static QByteArray response(int code, const QByteArray &reason, const QByteArray &body,
                               const QByteArray &extra = QByteArray(),
                               const QByteArray &type = "text/plain")
    {
        return "HTTP/1.1 " + QByteArray::number(code) + " " + reason + "\r\nContent-Type: " + type + "\r\n"
               + extra + "Content-Length: " + QByteArray::number(body.size())
               + "\r\nConnection: close\r\n\r\n" + body;
    }

private:
    void accept()
    {
        while (QTcpSocket *s = m_srv->nextPendingConnection())
        {
            auto buf = std::make_shared<QByteArray>();
            connect(s, &QTcpSocket::readyRead, this, [this, s, buf]() {
                *buf += s->readAll();
                if (!buf->contains("\r\n\r\n"))
                    return;
                const QByteArray head = *buf;
                buf->clear();
                requests++;
                if (handler)
                {
                    s->write(handler(head, requests));
                    s->disconnectFromHost();
                    return;
                }
                const bool ok = requests <= m_ok;
                const QByteArray body = ok ? m_jpeg : QByteArray("nope");
                s->write(QByteArray(ok ? "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n"
                                       : "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n")
                         + "Content-Length: " + QByteArray::number(body.size())
                         + "\r\nConnection: close\r\n\r\n" + body);
                s->disconnectFromHost();
            });
        }
    }

    QTcpServer *m_srv = nullptr;
    QByteArray m_jpeg;
    int m_ok;
};

QByteArray headerValue(const QByteArray &head, const QByteArray &name)
{
    for (const QByteArray &line : head.split('\n'))
        if (line.left(name.size() + 1).toLower() == name.toLower() + ":")
            return line.mid(name.size() + 1).trimmed();
    return QByteArray();
}

QByteArray requestPath(const QByteArray &head)
{
    return head.split(' ').value(1);
}

QByteArray basic(const char *user, const char *pw)
{
    return "Basic " + (QByteArray(user) + ":" + pw).toBase64();
}

struct Cert
{
    QSslConfiguration tls;
    QSslCertificate cert;
};
}

class TstCameraView : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

    void tolerableErrorsAreExactlyTheSelfSignedFamily();
    void selfSignedAndHostNameMismatch();
    void selfSignedWithMatchingName();
    void unknownIssuerLeaf();
    void expiredCertificateIsNotIgnored();
    void expiredCertificateWithMismatchedName();
    void otherRequestsStayRefused();
    void otherRequestInFlightStaysRefused();
    void defaultSslConfigurationUntouched();
    void plainHttpUnchanged();
    void networkErrorStillShown();
    void notFoundOverHttpsStillShown();
    void passwordNeverShown_data();
    void passwordNeverShown();
    void queryPasswordNeverShown_data();
    void queryPasswordNeverShown();
    void realErrorStringLeaksQueryPassword();

    void authNoChallenge_data();
    void authNoChallenge();
    void authChallengeThenOk_data();
    void authChallengeThenOk();
    void authWrongCredentials_data();
    void authWrongCredentials();
    void forbidden_data();
    void forbidden();
    void unauthorizedWithoutChallenge_data();
    void unauthorizedWithoutChallenge();
    void redirectSameHost_data();
    void redirectSameHost();
    void redirectOtherHost_data();
    void redirectOtherHost();

private:
    bool openssl(const QStringList &args);
    bool makeSelfSigned(const QString &name, const QString &san, const QStringList &extra = {});
    bool load(const QString &name, Cert *out);
    QString urlFor(quint16 port, const char *scheme, const QString &userinfo = QString()) const;
    // Runs the dialog until it shows its first message box; returns the label.
    bool preview(const QString &url, QStringList *boxes, int settleMs = 0);
    QSslConfiguration *tlsFor(const QString &scheme) { return scheme == "https" ? &m_certs["mismatch"].tls : nullptr; }

    QTemporaryDir m_dir;
    BoxCatcher m_boxes;
    bool m_opensslOk = false;
    bool m_lastPixmapSet = false;
    QHash<QString, Cert> m_certs;
};

bool TstCameraView::openssl(const QStringList &args)
{
    QProcess p;
    p.setWorkingDirectory(m_dir.path());
    p.start("openssl", args);
    if (!p.waitForFinished(30000) || p.exitStatus() != QProcess::NormalExit || p.exitCode() != 0)
    {
        qWarning() << "openssl" << args << p.readAllStandardError();
        return false;
    }
    return true;
}

bool TstCameraView::makeSelfSigned(const QString &name, const QString &san, const QStringList &extra)
{
    return openssl(QStringList{"req", "-x509", "-newkey", "ec", "-pkeyopt", "ec_paramgen_curve:prime256v1",
                               "-nodes", "-keyout", name + ".key", "-out", name + ".pem",
                               "-subj", "/CN=camera-test", "-addext", "subjectAltName=" + san}
                   + extra);
}

bool TstCameraView::load(const QString &name, Cert *out)
{
    QFile c(m_dir.filePath(name + ".pem")), k(m_dir.filePath(name + ".key"));
    if (!c.open(QIODevice::ReadOnly) || !k.open(QIODevice::ReadOnly))
        return false;
    out->cert = QSslCertificate(c.readAll());
    out->tls = QSslConfiguration::defaultConfiguration();
    out->tls.setLocalCertificate(out->cert);
    out->tls.setPrivateKey(QSslKey(k.readAll(), QSsl::Ec));
    return !out->cert.isNull() && !out->tls.privateKey().isNull();
}

void TstCameraView::initTestCase()
{
    g_prevHandler = qInstallMessageHandler(captureLog);
    qApp->installEventFilter(&m_boxes);

    QVERIFY(m_dir.isValid());
    m_opensslOk = QStandardPaths::findExecutable("openssl") != QString();
    if (!m_opensslOk)
    {
        // The CI image has openssl; a host without it must not turn red.
        if (qEnvironmentVariableIsSet("CALAOS_REQUIRE_OPENSSL"))
            QFAIL("openssl binary missing");
        QSKIP("openssl binary missing: no certificate can be generated");
    }
    QVERIFY(QSslSocket::supportsSsl());

    // Self-signed, name does not match 127.0.0.1.
    QVERIFY(makeSelfSigned("mismatch", "DNS:camera.invalid", {"-days", "30"}));
    // Self-signed, name matches: only the untrusted-issuer error remains.
    QVERIFY(makeSelfSigned("selfmatch", "IP:127.0.0.1", {"-days", "30"}));
    // Expired for years, name matches.
    QVERIFY(makeSelfSigned("expired", "IP:127.0.0.1",
                           {"-not_before", "20200101000000Z", "-not_after", "20200102000000Z"}));
    // Expired and name mismatch: every tolerable error plus one that is not.
    QVERIFY(makeSelfSigned("expiredmismatch", "DNS:camera.invalid",
                           {"-not_before", "20200101000000Z", "-not_after", "20200102000000Z"}));

    // Leaf signed by a private CA that the server does not send along.
    QVERIFY(makeSelfSigned("ca", "DNS:ca.invalid", {"-days", "30", "-addext", "basicConstraints=critical,CA:TRUE"}));
    QVERIFY(openssl({"req", "-new", "-newkey", "ec", "-pkeyopt", "ec_paramgen_curve:prime256v1", "-nodes",
                     "-keyout", "leaf.key", "-out", "leaf.csr", "-subj", "/CN=camera-leaf"}));
    QFile ext(m_dir.filePath("leaf.ext"));
    QVERIFY(ext.open(QIODevice::WriteOnly));
    ext.write("subjectAltName=IP:127.0.0.1\n");
    ext.close();
    QVERIFY(openssl({"x509", "-req", "-in", "leaf.csr", "-CA", "ca.pem", "-CAkey", "ca.key", "-CAcreateserial",
                     "-out", "leaf.pem", "-days", "30", "-extfile", "leaf.ext"}));

    for (const char *n : {"mismatch", "selfmatch", "expired", "expiredmismatch", "leaf"})
    {
        Cert c;
        QVERIFY2(load(n, &c), n);
        m_certs.insert(n, c);
    }
}

void TstCameraView::cleanupTestCase()
{
    qApp->removeEventFilter(&m_boxes);
    qInstallMessageHandler(g_prevHandler);
}

void TstCameraView::init()
{
    g_log.clear();
    m_boxes.texts.clear();
}

QString TstCameraView::urlFor(quint16 port, const char *scheme, const QString &userinfo) const
{
    return QString::fromLatin1(scheme) + "://" + (userinfo.isEmpty() ? QString() : userinfo + "@")
           + "127.0.0.1:" + QString::number(port) + "/cam.jpg";
}

bool TstCameraView::preview(const QString &url, QStringList *boxes, int settleMs)
{
    Params p;
    IOBase cam(p, "camera", TSTRING, IOBase::IO_NONE);
    cam.set_param("type", "standard_mjpeg");
    cam.set_param("url_jpeg", url.toStdString());

    DialogCameraView dlg(&cam);
    dlg.DownloadPicture();
    // Failsafe bound only: the wait ends on the first message box.
    const bool shown = QTest::qWaitFor([this]() { return !m_boxes.texts.isEmpty(); }, 20000);
    // Lets a runaway retry loop show up as extra requests or message boxes.
    if (settleMs)
        QTest::qWait(settleMs);
    *boxes = m_boxes.texts;
    m_lastPixmapSet = dlg.findChild<QLabel *>("label_cam") && !dlg.findChild<QLabel *>("label_cam")->pixmap().isNull();
    return shown;
}

void TstCameraView::tolerableErrorsAreExactlyTheSelfSignedFamily()
{
    const QSet<int> tolerated = {
        QSslError::HostNameMismatch, QSslError::SelfSignedCertificate, QSslError::SelfSignedCertificateInChain,
        QSslError::UnableToGetLocalIssuerCertificate, QSslError::UnableToVerifyFirstCertificate,
        QSslError::CertificateUntrusted};
    // Walk the whole enum range, not a hand-picked list: a new tolerated value
    // has to be added here on purpose.
    for (int v = QSslError::UnspecifiedError; v <= QSslError::OcspStatusUnknown; ++v)
        QCOMPARE(DialogCameraView::isTolerableSslError(static_cast<QSslError::SslError>(v)), tolerated.contains(v));

    for (QSslError::SslError e : {QSslError::CertificateExpired, QSslError::CertificateNotYetValid,
                                  QSslError::CertificateRevoked, QSslError::InvalidCaCertificate,
                                  QSslError::CertificateSignatureFailed, QSslError::InvalidPurpose,
                                  QSslError::CertificateRejected, QSslError::CertificateBlacklisted,
                                  QSslError::SubjectIssuerMismatch, QSslError::NoPeerCertificate,
                                  QSslError::NoError, QSslError::UnspecifiedError})
        QVERIFY(!DialogCameraView::isTolerableSslError(e));
}

void TstCameraView::selfSignedAndHostNameMismatch()
{
    FakeCam cam(&m_certs["mismatch"].tls, 1, this);
    QVERIFY(cam.listen());
    QVERIFY(cam.jpegValid());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), "https"), &boxes));
    // First image shown, then the 404 of the second round ends the loop.
    QVERIFY(m_lastPixmapSet);
    QCOMPARE(cam.requests, 2);
    QCOMPARE(boxes.size(), 1);
    QVERIFY2(boxes[0].contains("Not Found"), qPrintable(boxes[0]));
    QVERIFY2(!boxes[0].contains("SSL", Qt::CaseInsensitive), qPrintable(boxes[0]));
}

void TstCameraView::selfSignedWithMatchingName()
{
    FakeCam cam(&m_certs["selfmatch"].tls, 1, this);
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), "https"), &boxes));
    QVERIFY(m_lastPixmapSet);
    QCOMPARE(cam.requests, 2);
}

void TstCameraView::unknownIssuerLeaf()
{
    FakeCam cam(&m_certs["leaf"].tls, 1, this);
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), "https"), &boxes));
    QVERIFY(m_lastPixmapSet);
    QCOMPARE(cam.requests, 2);
}

void TstCameraView::expiredCertificateIsNotIgnored()
{
    FakeCam cam(&m_certs["expired"].tls, 5, this);
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), "https"), &boxes));
    // The handshake is refused: not a single HTTP request reached the server.
    QCOMPARE(cam.requests, 0);
    QVERIFY(!m_lastPixmapSet);
    QCOMPARE(boxes.size(), 1);
    QVERIFY(!boxes[0].trimmed().isEmpty());
    QVERIFY2(boxes[0].contains("SSL", Qt::CaseInsensitive), qPrintable(boxes[0]));
}

void TstCameraView::expiredCertificateWithMismatchedName()
{
    // Tolerable errors alone are ignorable, but the expiry makes the whole
    // list unacceptable: nothing may be ignored, not even the tolerable ones.
    FakeCam cam(&m_certs["expiredmismatch"].tls, 5, this);
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), "https"), &boxes));
    QCOMPARE(cam.requests, 0);
    QVERIFY(!m_lastPixmapSet);
    QCOMPARE(boxes.size(), 1);
}

void TstCameraView::otherRequestsStayRefused()
{
    FakeCam cam(&m_certs["mismatch"].tls, 1, this);
    QVERIFY(cam.listen());
    const QUrl url(urlFor(cam.port(), "https"));

    auto outside = [&]() {
        QNetworkAccessManager nam;
        QNetworkReply *r = nam.get(QNetworkRequest(url));
        QSignalSpy done(r, &QNetworkReply::finished);
        const bool finished = done.wait(20000) || done.count() > 0;
        const QNetworkReply::NetworkError err = r->error();
        r->deleteLater();
        return finished ? err : QNetworkReply::UnknownNetworkError;
    };

    QCOMPARE(outside(), QNetworkReply::SslHandshakeFailedError);
    QCOMPARE(cam.requests, 0);

    QStringList boxes;
    QVERIFY(preview(url.toString(), &boxes));
    QVERIFY(m_lastPixmapSet);
    QCOMPARE(cam.requests, 2);

    // After a successful preview in the same process.
    QCOMPARE(outside(), QNetworkReply::SslHandshakeFailedError);
    QCOMPARE(cam.requests, 2);
}

void TstCameraView::otherRequestInFlightStaysRefused()
{
    FakeCam cam(&m_certs["mismatch"].tls, 1, this);
    QVERIFY(cam.listen());
    const QUrl url(urlFor(cam.port(), "https"));

    // Both handshakes are pending in the same event loop.
    QNetworkAccessManager nam;
    QNetworkReply *outside = nam.get(QNetworkRequest(url));
    QSignalSpy outsideDone(outside, &QNetworkReply::finished);

    QStringList boxes;
    QVERIFY(preview(url.toString(), &boxes));
    QVERIFY(outsideDone.count() > 0 || outsideDone.wait(20000));
    QVERIFY(m_lastPixmapSet);
    QCOMPARE(outside->error(), QNetworkReply::SslHandshakeFailedError);
    QCOMPARE(cam.requests, 2);
    outside->deleteLater();
}

void TstCameraView::defaultSslConfigurationUntouched()
{
    const QSslConfiguration before = QSslConfiguration::defaultConfiguration();
    FakeCam cam(&m_certs["mismatch"].tls, 1, this);
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), "https"), &boxes));
    QVERIFY(m_lastPixmapSet);

    const QSslConfiguration after = QSslConfiguration::defaultConfiguration();
    QCOMPARE(after.caCertificates(), before.caCertificates());
    QVERIFY(!after.caCertificates().contains(m_certs["mismatch"].cert));
    QCOMPARE(after.peerVerifyMode(), before.peerVerifyMode());
    QCOMPARE(after.protocol(), before.protocol());
    QVERIFY(after.peerVerifyMode() != QSslSocket::VerifyNone);
}

void TstCameraView::plainHttpUnchanged()
{
    FakeCam cam(nullptr, 1, this);
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), "http"), &boxes));
    QVERIFY(m_lastPixmapSet);
    QCOMPARE(cam.requests, 2);
    QCOMPARE(boxes.size(), 1);
    QVERIFY2(boxes[0].contains("Not Found"), qPrintable(boxes[0]));
}

void TstCameraView::networkErrorStillShown()
{
    // A port nobody listens on: bind, note it, release it.
    quint16 port = 0;
    {
        QTcpServer s;
        QVERIFY(s.listen(QHostAddress::LocalHost));
        port = s.serverPort();
    }
    for (const char *scheme : {"http", "https"})
    {
        m_boxes.texts.clear();
        QStringList boxes;
        QVERIFY(preview(urlFor(port, scheme), &boxes));
        QVERIFY(!m_lastPixmapSet);
        QCOMPARE(boxes.size(), 1);
        QVERIFY2(!boxes[0].trimmed().isEmpty(), scheme);
    }
}

void TstCameraView::notFoundOverHttpsStillShown()
{
    // A tolerated certificate does not turn an HTTP error into a success.
    FakeCam cam(&m_certs["mismatch"].tls, 0, this);
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), "https"), &boxes));
    QVERIFY(!m_lastPixmapSet);
    QCOMPARE(cam.requests, 1);
    QCOMPARE(boxes.size(), 1);
    QVERIFY2(boxes[0].contains("Not Found"), qPrintable(boxes[0]));
}

void TstCameraView::passwordNeverShown_data()
{
    QTest::addColumn<QString>("scenario");
    QTest::addColumn<QString>("password");

    const QStringList passwords = {"S3cretPw", "Zq7Kmn:p@ss/w%41!", "Spaced Pw&q?x#y9"};
    for (const QString &scenario : {"expired", "notfound-https", "notfound-http", "refused-http", "refused-https"})
        for (const QString &pw : passwords)
            QTest::newRow(qPrintable(scenario + " " + pw)) << scenario << pw;
}

void TstCameraView::passwordNeverShown()
{
    QFETCH(QString, scenario);
    QFETCH(QString, password);

    std::unique_ptr<FakeCam> cam;
    quint16 port = 0;
    if (scenario == "expired")
        cam.reset(new FakeCam(&m_certs["expired"].tls, 5, this));
    else if (scenario == "notfound-https")
        cam.reset(new FakeCam(&m_certs["mismatch"].tls, 0, this));
    else if (scenario == "notfound-http")
        cam.reset(new FakeCam(nullptr, 0, this));
    if (cam)
    {
        QVERIFY(cam->listen());
        port = cam->port();
    }
    else
    {
        QTcpServer s;
        QVERIFY(s.listen(QHostAddress::LocalHost));
        port = s.serverPort();
    }

    const QString encoded = QString::fromLatin1(QUrl::toPercentEncoding(password));
    const QString userinfo = "camuser7q:" + encoded;
    const char *scheme = scenario.endsWith("http") ? "http" : "https";

    QStringList boxes;
    QVERIFY(preview(urlFor(port, scheme, userinfo), &boxes));
    QCOMPARE(boxes.size(), 1);
    QVERIFY(!boxes[0].trimmed().isEmpty());

    // The URL itself is still logged, so the log check below is not vacuous.
    QVERIFY(g_log.filter("127.0.0.1").size() >= 1);
    for (const QString &line : std::as_const(g_log))
        QVERIFY2(!line.contains("camuser7q"), qPrintable("log: " + line));

    // Every spelling Qt may use for the password, and its longest plain run in
    // case only part of it is re-encoded.
    QStringList secrets = {password, encoded, QUrl(urlFor(port, scheme, userinfo)).password(QUrl::FullyEncoded),
                           QUrl(urlFor(port, scheme, userinfo)).password(QUrl::PrettyDecoded)};
    const QRegularExpressionMatch run = QRegularExpression("[A-Za-z0-9]{5,}").match(password);
    QVERIFY(run.hasMatch());
    secrets << run.captured(0);
    bool dialogLeaks = false;
    for (const QString &secret : std::as_const(secrets))
    {
        QVERIFY(!secret.isEmpty());
        dialogLeaks = dialogLeaks || boxes[0].contains(secret);
        for (const QString &line : std::as_const(g_log))
            QVERIFY2(!line.contains(secret), qPrintable("log: " + line));
    }

    QVERIFY2(!dialogLeaks, qPrintable("dialog: " + boxes[0]));
}

void TstCameraView::queryPasswordNeverShown_data()
{
    QTest::addColumn<QString>("scheme");
    QTest::addColumn<int>("status");
    QTest::addColumn<QString>("password");

    for (const char *scheme : {"http", "https"})
        for (int status : {404, 403, 401})
            for (const QString &pw : {QString("S3cretPw"), QString("Zq7Kmn:p@ss/w%41!"), QString("Spaced Pw&q?x#y9")})
                QTest::newRow(qPrintable(QString("%1 %2 %3").arg(scheme).arg(status).arg(pw)))
                    << scheme << status << pw;
}

void TstCameraView::queryPasswordNeverShown()
{
    QFETCH(QString, scheme);
    QFETCH(int, status);
    QFETCH(QString, password);

    FakeCam cam(tlsFor(scheme), 0, this);
    cam.handler = [status](const QByteArray &, int) {
        return FakeCam::response(status, "Err", "nope");
    };
    QVERIFY(cam.listen());

    const QString encoded = QString::fromLatin1(QUrl::toPercentEncoding(password));
    const QString url = urlFor(cam.port(), scheme.toLatin1()) + "?usr=camuser7q&pwd=" + encoded;

    QStringList boxes;
    QVERIFY(preview(url, &boxes, 200));
    QCOMPARE(boxes.size(), 1);
    QCOMPARE(cam.requests, 1);
    QVERIFY2(boxes[0].contains(QString::number(status)), qPrintable(boxes[0]));
    QVERIFY(g_log.filter("127.0.0.1").size() >= 1);

    const QUrl parsed(url);
    QStringList secrets = {password, encoded, parsed.query(QUrl::FullyEncoded), parsed.query(QUrl::PrettyDecoded),
                           QUrl::fromPercentEncoding(encoded.toLatin1()), "camuser7q"};
    const QRegularExpressionMatch run = QRegularExpression("[A-Za-z0-9]{5,}").match(password);
    QVERIFY(run.hasMatch());
    secrets << run.captured(0);
    for (const QString &secret : std::as_const(secrets))
    {
        QVERIFY(!secret.isEmpty());
        QVERIFY2(!boxes[0].contains(secret), qPrintable("dialog: " + boxes[0]));
        for (const QString &line : std::as_const(g_log))
            QVERIFY2(!line.contains(secret), qPrintable("log: " + line));
    }
}

void TstCameraView::realErrorStringLeaksQueryPassword()
{
    // Guards the premise of the tests above: Qt's own error text for an HTTP
    // error does carry the query, so the dialog must not use it.
    FakeCam cam(nullptr, 0, this);
    QVERIFY(cam.listen());
    QNetworkAccessManager nam;
    QNetworkReply *r = nam.get(QNetworkRequest(QUrl(urlFor(cam.port(), "http") + "?pwd=S3cretPw")));
    QSignalSpy done(r, &QNetworkReply::finished);
    QVERIFY(done.wait(20000) || done.count() > 0);
    QCOMPARE(r->error(), QNetworkReply::ContentNotFoundError);
    QVERIFY2(r->errorString().contains("S3cretPw"), qPrintable(r->errorString()));
    r->deleteLater();
}

void TstCameraView::authNoChallenge_data()
{
    QTest::addColumn<QString>("scheme");
    QTest::newRow("http") << "http";
    QTest::newRow("https") << "https";
}

void TstCameraView::authNoChallenge()
{
    QFETCH(QString, scheme);
    FakeCam cam(tlsFor(scheme), 1, this);
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), scheme.toLatin1(), "camuser7q:S3cretPw"), &boxes));
    QVERIFY(m_lastPixmapSet);
    QCOMPARE(cam.requests, 2);
    QVERIFY2(boxes[0].contains("Not Found"), qPrintable(boxes[0]));
}

void TstCameraView::authChallengeThenOk_data()
{
    authNoChallenge_data();
}

void TstCameraView::authChallengeThenOk()
{
    QFETCH(QString, scheme);
    FakeCam cam(tlsFor(scheme), 0, this);
    int authorized = 0, anonymousWithUserinfo = 0;
    cam.handler = [&](const QByteArray &head, int) {
        if (head.contains("@"))
            anonymousWithUserinfo++;
        if (headerValue(head, "Authorization") == basic("camuser7q", "S3cretPw"))
            return ++authorized == 1 ? FakeCam::response(200, "OK", cam.jpeg(), "", "image/jpeg")
                                     : FakeCam::response(404, "Not Found", "nope");
        return FakeCam::response(401, "Unauthorized", "auth", "WWW-Authenticate: Basic realm=\"cam\"\r\n");
    };
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), scheme.toLatin1(), "camuser7q:S3cretPw"), &boxes));
    QVERIFY(m_lastPixmapSet);
    // Per picture: one unanswered request, one authenticated.
    QCOMPARE(cam.requests, 4);
    QCOMPARE(authorized, 2);
    QCOMPARE(anonymousWithUserinfo, 0);
    QVERIFY2(boxes[0].contains("Not Found"), qPrintable(boxes[0]));
}

void TstCameraView::authWrongCredentials_data()
{
    authNoChallenge_data();
}

void TstCameraView::authWrongCredentials()
{
    QFETCH(QString, scheme);
    FakeCam cam(tlsFor(scheme), 0, this);
    cam.handler = [&](const QByteArray &head, int) {
        if (headerValue(head, "Authorization") == basic("camuser7q", "S3cretPw"))
            return FakeCam::response(200, "OK", cam.jpeg(), "", "image/jpeg");
        return FakeCam::response(401, "Unauthorized", "auth", "WWW-Authenticate: Basic realm=\"cam\"\r\n");
    };
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), scheme.toLatin1(), "camuser7q:WrongPw9"), &boxes, 400));
    QVERIFY(!m_lastPixmapSet);
    // One unanswered request and one retry with the wrong password: no more,
    // even after the dialog had time to loop.
    QCOMPARE(cam.requests, 2);
    QCOMPARE(boxes.size(), 1);
    QVERIFY(!boxes[0].contains("WrongPw9"));
}

void TstCameraView::forbidden_data()
{
    authNoChallenge_data();
}

void TstCameraView::forbidden()
{
    QFETCH(QString, scheme);
    FakeCam cam(tlsFor(scheme), 0, this);
    cam.handler = [](const QByteArray &, int) { return FakeCam::response(403, "Forbidden", "no"); };
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), scheme.toLatin1(), "camuser7q:S3cretPw"), &boxes, 300));
    QVERIFY(!m_lastPixmapSet);
    QCOMPARE(cam.requests, 1);
    QCOMPARE(boxes.size(), 1);
    QVERIFY2(boxes[0].contains("Forbidden"), qPrintable(boxes[0]));
}

void TstCameraView::unauthorizedWithoutChallenge_data()
{
    authNoChallenge_data();
}

void TstCameraView::unauthorizedWithoutChallenge()
{
    QFETCH(QString, scheme);
    FakeCam cam(tlsFor(scheme), 0, this);
    cam.handler = [](const QByteArray &, int) { return FakeCam::response(401, "Unauthorized", "no"); };
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), scheme.toLatin1(), "camuser7q:S3cretPw"), &boxes, 300));
    QVERIFY(!m_lastPixmapSet);
    QCOMPARE(cam.requests, 1);
    QCOMPARE(boxes.size(), 1);
    QVERIFY2(boxes[0].contains("401"), qPrintable(boxes[0]));
}

void TstCameraView::redirectSameHost_data()
{
    authNoChallenge_data();
}

void TstCameraView::redirectSameHost()
{
    QFETCH(QString, scheme);
    FakeCam cam(tlsFor(scheme), 0, this);
    int images = 0;
    cam.handler = [&](const QByteArray &head, int) {
        if (requestPath(head) == "/cam.jpg")
            return FakeCam::response(302, "Found", "", "Location: /real.jpg\r\n");
        return ++images == 1 ? FakeCam::response(200, "OK", cam.jpeg(), "", "image/jpeg")
                             : FakeCam::response(404, "Not Found", "nope");
    };
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), scheme.toLatin1()), &boxes));
    QVERIFY(m_lastPixmapSet);
    QCOMPARE(cam.requests, 4);
    QVERIFY2(boxes[0].contains("Not Found"), qPrintable(boxes[0]));
}

void TstCameraView::redirectOtherHost_data()
{
    QTest::addColumn<QString>("from");
    QTest::addColumn<QString>("to");
    QTest::addColumn<QString>("targetHost");
    QTest::newRow("https to https, other host") << "https" << "https" << "127.0.0.2";
    QTest::newRow("http to http, other host") << "http" << "http" << "127.0.0.2";
    QTest::newRow("https to http, same host") << "https" << "http" << "127.0.0.1";
    QTest::newRow("http to https, same host") << "http" << "https" << "127.0.0.1";
}

void TstCameraView::redirectOtherHost()
{
    QFETCH(QString, from);
    QFETCH(QString, to);
    QFETCH(QString, targetHost);

    // The target would accept any SSL error tolerance and ask for credentials:
    // it must not even receive a connection.
    FakeCam target(tlsFor(to), 0, this);
    int targetAuth = 0;
    target.handler = [&](const QByteArray &head, int) {
        if (!headerValue(head, "Authorization").isEmpty())
            targetAuth++;
        return FakeCam::response(401, "Unauthorized", "auth", "WWW-Authenticate: Basic realm=\"x\"\r\n");
    };
    QVERIFY(target.listen(QHostAddress(targetHost)));

    FakeCam cam(tlsFor(from), 0, this);
    const QString loc = to + "://" + targetHost + ":" + QString::number(target.port()) + "/cam.jpg";
    cam.handler = [&](const QByteArray &, int) {
        return FakeCam::response(302, "Found", "", "Location: " + loc.toLatin1() + "\r\n");
    };
    QVERIFY(cam.listen());

    QStringList boxes;
    QVERIFY(preview(urlFor(cam.port(), from.toLatin1(), "camuser7q:S3cretPw"), &boxes, 300));
    QVERIFY(!m_lastPixmapSet);
    QCOMPARE(cam.requests, 1);
    QCOMPARE(target.requests, 0);
    QCOMPARE(targetAuth, 0);
    QCOMPARE(boxes.size(), 1);
    QVERIFY(!boxes[0].contains("S3cretPw"));
}

QTEST_MAIN(TstCameraView)
#include "tst_cameraview.moc"
