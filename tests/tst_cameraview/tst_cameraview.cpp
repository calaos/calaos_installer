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

    bool listen() { return m_srv->listen(QHostAddress::LocalHost); }
    quint16 port() const { return m_srv->serverPort(); }
    bool jpegValid() const { return !m_jpeg.isEmpty(); }

    int requests = 0;

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
                buf->clear();
                requests++;
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

private:
    bool openssl(const QStringList &args);
    bool makeSelfSigned(const QString &name, const QString &san, const QStringList &extra = {});
    bool load(const QString &name, Cert *out);
    QString urlFor(quint16 port, const char *scheme, const QString &userinfo = QString()) const;
    // Runs the dialog until it shows its first message box; returns the label.
    bool preview(const QString &url, QStringList *boxes);

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

bool TstCameraView::preview(const QString &url, QStringList *boxes)
{
    Params p;
    IOBase cam(p, "camera", TSTRING, IOBase::IO_NONE);
    cam.set_param("type", "standard_mjpeg");
    cam.set_param("url_jpeg", url.toStdString());

    DialogCameraView dlg(&cam);
    dlg.DownloadPicture();
    // Failsafe bound only: the wait ends on the first message box.
    const bool shown = QTest::qWaitFor([this]() { return !m_boxes.texts.isEmpty(); }, 20000);
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
    for (int v = 0; v < 32; ++v)
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
    const QString userinfo = "admin:" + encoded;
    const char *scheme = scenario.endsWith("http") ? "http" : "https";

    QStringList boxes;
    QVERIFY(preview(urlFor(port, scheme, userinfo), &boxes));
    QCOMPARE(boxes.size(), 1);
    QVERIFY(!boxes[0].trimmed().isEmpty());

    // The URL itself is still logged, so the log check below is not vacuous.
    QVERIFY(g_log.filter("127.0.0.1").size() >= 1);

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

QTEST_MAIN(TstCameraView)
#include "tst_cameraview.moc"
