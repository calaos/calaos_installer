#include <QtTest>
#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTemporaryDir>
#include <KCompressionDevice>
#include <KTar>
#include <KZip>
#include <unistd.h>

#include "BoundedDevice.h"
#include "ConfigOptions.h"
#include "FlashToolManager.h"

namespace
{
const QByteArray kBinary = "#!/bin/sh\necho esptool\n";

// Lets a test fail one directory swap.
class TestManager : public FlashToolManager
{
public:
    QString failRenameFrom;
    QString alsoFailFrom;
    QString shortenAfterCopy; // a file name that ends up shorter on disk than in the archive

protected:
    bool copyEntries(const KArchiveDirectory *root, const QString &destDir) const override
    {
        if (!shortenAfterCopy.isEmpty())
        {
            const bool copied = root->copyTo(destDir, true);
            QDirIterator it(destDir, QStringList() << shortenAfterCopy, QDir::Files,
                            QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                QFile f(it.next());
                f.resize(f.size() - 1);
            }
            return copied;
        }
        return FlashToolManager::copyEntries(root, destDir);
    }

    bool renameDir(const QString &from, const QString &to) override
    {
        if ((!failRenameFrom.isEmpty() && from.endsWith(failRenameFrom))
            || (!alsoFailFrom.isEmpty() && from.endsWith(alsoFailFrom)))
            return false;
        return FlashToolManager::renameDir(from, to);
    }
};

FlashConfig esptoolConfig()
{
    FlashConfig c;
    c.tool = "esptool";
    c.toolUrl = "https://github.com/test/esptool";
    c.toolGithubRelease = true;
    return c;
}

QString sha256Of(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return QString();
    return QString::fromLatin1(QCryptographicHash::hash(f.readAll(), QCryptographicHash::Sha256).toHex());
}

struct Outcome
{
    bool done = false;
    bool ok = false;
    QString text;
};
} // namespace

class TstFlashToolManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void installReplacesPreviousTool();
    void badDigestKeepsPreviousTool();
    void unsafeAssetNameIsRefused();
    void oversizedDownloadIsRefused();
    void hostileArchiveIsRefused_data();
    void hostileArchiveIsRefused();
    void failedSwapRestoresPreviousTool();
    void failedRestoreSaysWhereThePreviousToolIs();
    void unclearableStageIsRefused();
    void partialZipExtractionIsRefused();
    void decompressionBombIsRefused();
    void inflatedBombIsCutOffWhileReading();
    void boundedDeviceStopsAtItsLimit();
    void tooManyEntriesAreRefused();
    void tooDeepArchiveIsRefused();
    void previousToolSurvivesTwoFailedRestores();
    void leftoverOldToolIsRestoredBeforeTheNextInstall();
    void directDownloadIsBounded();
void failedDirectExtractionLeavesNothingToRun();
void shortFileIsRefused();
void directInstallKeepsOnlyTheNewFiles();
    void refusedUpdateIsNotInstalledNorProposedAgain();
    void acceptedUpdateFailureIsReported();
    void otherToolsAreNeverUpdated();

private:
    // tar.gz holding esptool-v6/esptool, plus whatever the test adds through extra
    QString makeArchive(const QString &name, const std::function<void(KTar &)> &extra = {});
    EsptoolAsset assetFor(const QString &archive, const QString &tag = "v6.0.0") const;
    Outcome install(FlashToolManager &mgr, const EsptoolAsset &asset);
    void installOldTool();
    Outcome directInstall(FlashToolManager &mgr, const QString &zip);
    QString releasesDir() const;
    void writeReleases();

    QTemporaryDir m_work;
    QString m_toolDir;
};

void TstFlashToolManager::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    QVERIFY(m_work.isValid());
    m_toolDir = FlashToolManager::toolsCacheDir() + "/esptool";
}

void TstFlashToolManager::init()
{
    QDir(FlashToolManager::toolsCacheDir()).removeRecursively();
    ConfigOptions::Instance().setOption("tools/esptool_path", QString());
    ConfigOptions::Instance().setOption("tools/esptool_declined_version", QString());
}

QString TstFlashToolManager::makeArchive(const QString &name, const std::function<void(KTar &)> &extra)
{
    const QString path = m_work.filePath(name);
    KTar tar(path, "application/x-gzip");
    if (!tar.open(QIODevice::WriteOnly))
        return QString();
    tar.writeFile("esptool-v6/esptool", kBinary, 0755);
    tar.writeFile("esptool-v6/LICENSE", "license", 0644);
    if (extra)
        extra(tar);
    tar.close();
    return path;
}

EsptoolAsset TstFlashToolManager::assetFor(const QString &archive, const QString &tag) const
{
    EsptoolAsset a;
    a.tag = tag;
    a.name = QFileInfo(archive).fileName();
    a.url = QUrl::fromLocalFile(archive).toString();
    a.sha256 = sha256Of(archive);
    a.size = QFileInfo(archive).size();
    return a;
}

Outcome TstFlashToolManager::install(FlashToolManager &mgr, const EsptoolAsset &asset)
{
    Outcome out;
    mgr.installAsset(esptoolConfig(), asset, [&](bool ok, const QString &text)
    {
        out.done = true;
        out.ok = ok;
        out.text = text;
    });
    QTest::qWait(0);
    for (int i = 0; i < 100 && !out.done; ++i)
        QTest::qWait(50);
    return out;
}

void TstFlashToolManager::installOldTool()
{
    QVERIFY(QDir().mkpath(m_toolDir));
    QFile tool(m_toolDir + "/esptool");
    QVERIFY(tool.open(QIODevice::WriteOnly));
    tool.write("old");
    tool.close();
    QFile::setPermissions(tool.fileName(), QFile::permissions(tool.fileName()) | QFileDevice::ExeOwner);
    QFile stale(m_toolDir + "/stale.txt");
    QVERIFY(stale.open(QIODevice::WriteOnly));
    stale.close();
    QFile ver(m_toolDir + "/version.txt");
    QVERIFY(ver.open(QIODevice::WriteOnly));
    ver.write("v5.0.0");
}

void TstFlashToolManager::installReplacesPreviousTool()
{
    installOldTool();
    FlashToolManager mgr;
    const Outcome out = install(mgr, assetFor(makeArchive("ok.tar.gz")));

    QVERIFY2(out.ok, qPrintable(out.text));
    QCOMPARE(out.text, m_toolDir + "/esptool");
    QVERIFY(QFileInfo(out.text).isExecutable());
    QFile f(out.text);
    QVERIFY(f.open(QIODevice::ReadOnly));
    QCOMPARE(f.readAll(), kBinary);
    QVERIFY(!QFileInfo::exists(m_toolDir + "/stale.txt"));
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v6.0.0"));
    QVERIFY(!QFileInfo::exists(m_toolDir + ".new"));
    QVERIFY(!QFileInfo::exists(m_toolDir + ".old"));
}

void TstFlashToolManager::badDigestKeepsPreviousTool()
{
    installOldTool();
    EsptoolAsset a = assetFor(makeArchive("ok.tar.gz"));
    a.sha256 = QString(64, 'a');
    FlashToolManager mgr;
    const Outcome out = install(mgr, a);

    QVERIFY(out.done);
    QVERIFY(!out.ok);
    QVERIFY2(out.text.contains("Checksum"), qPrintable(out.text));
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));
    QVERIFY(!QFileInfo::exists(m_toolDir + ".new"));
}

void TstFlashToolManager::unsafeAssetNameIsRefused()
{
    installOldTool();
    EsptoolAsset a = assetFor(makeArchive("ok.tar.gz"));
    a.name = "../../escape.tar.gz";
    FlashToolManager mgr;
    const Outcome out = install(mgr, a);

    QVERIFY(out.done);
    QVERIFY(!out.ok);
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
    QVERIFY(!QFileInfo::exists(FlashToolManager::toolsCacheDir() + "/../escape.tar.gz"));
}

void TstFlashToolManager::oversizedDownloadIsRefused()
{
    installOldTool();
    EsptoolAsset a = assetFor(makeArchive("ok.tar.gz"));
    a.size = 10; // GitHub announced less than what the server sends
    FlashToolManager mgr;
    const Outcome out = install(mgr, a);

    QVERIFY(out.done);
    QVERIFY(!out.ok);
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
    QVERIFY(!QFileInfo::exists(m_toolDir + ".new"));
}

void TstFlashToolManager::hostileArchiveIsRefused_data()
{
    QTest::addColumn<QString>("kind");
    QTest::addColumn<bool>("refused");
    QTest::newRow("parent directory") << "dotdot" << true;
    // KArchive re-roots absolute names under the destination: harmless, but never outside it.
    QTest::newRow("absolute path")    << "absolute" << false;
    QTest::newRow("symlink")          << "symlink" << true;
}

void TstFlashToolManager::hostileArchiveIsRefused()
{
    QFETCH(QString, kind);
    QFETCH(bool, refused);
    installOldTool();
    const QString victim = m_work.filePath("victim");
    QFile::remove(victim);
    const QString archive = makeArchive("hostile.tar.gz", [&](KTar &tar)
    {
        if (kind == "dotdot")
            tar.writeFile("../../../../../../../../" + victim.mid(1), "pwned", 0644);
        else if (kind == "absolute")
            tar.writeFile(victim, "pwned", 0644);
        else
            tar.writeSymLink("esptool-v6/link", "/etc/passwd");
    });
    QVERIFY(!archive.isEmpty());

    FlashToolManager mgr;
    const Outcome out = install(mgr, assetFor(archive));

    QVERIFY(out.done);
    QVERIFY2(out.ok != refused, qPrintable(out.text));
    QVERIFY(!QFileInfo::exists(victim));
    if (refused)
    {
        QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
        QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));
    }
}

void TstFlashToolManager::failedSwapRestoresPreviousTool()
{
    installOldTool();
    TestManager mgr;
    mgr.failRenameFrom = ".new"; // the stage cannot take the place of the old install
    const Outcome out = install(mgr, assetFor(makeArchive("ok.tar.gz")));

    QVERIFY(out.done);
    QVERIFY(!out.ok);
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));
    QVERIFY(!QFileInfo::exists(m_toolDir + ".new"));
    QVERIFY(!QFileInfo::exists(m_toolDir + ".old"));
}

void TstFlashToolManager::failedRestoreSaysWhereThePreviousToolIs()
{
    installOldTool();
    TestManager mgr;
    mgr.failRenameFrom = ".new";
    mgr.alsoFailFrom = ".old"; // and the previous install cannot be put back either
    const Outcome out = install(mgr, assetFor(makeArchive("ok.tar.gz")));

    QVERIFY(out.done);
    QVERIFY(!out.ok);
    QVERIFY2(out.text.contains(m_toolDir + ".old"), qPrintable(out.text));
    QVERIFY(QFileInfo::exists(m_toolDir + ".old/stale.txt"));
}

void TstFlashToolManager::unclearableStageIsRefused()
{
    if (geteuid() == 0)
        QSKIP("root can delete a read-only folder");
    installOldTool();
    const QString stuck = m_toolDir + ".new/stuck";
    QVERIFY(QDir().mkpath(stuck));
    QFile leftover(stuck + "/leftover");
    QVERIFY(leftover.open(QIODevice::WriteOnly));
    leftover.close();
    QVERIFY(QFile::setPermissions(stuck, QFileDevice::ReadOwner | QFileDevice::ExeOwner));

    FlashToolManager mgr;
    const Outcome out = install(mgr, assetFor(makeArchive("ok.tar.gz")));
    QFile::setPermissions(stuck, QFile::permissions(stuck) | QFileDevice::WriteOwner);

    QVERIFY(out.done);
    QVERIFY2(!out.ok, qPrintable(out.text));
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));
}

void TstFlashToolManager::partialZipExtractionIsRefused()
{
    installOldTool();
    // The binary comes out first, then a name over NAME_MAX makes copyTo() fail midway.
    const QString path = m_work.filePath("partial.zip");
    {
        KZip zip(path);
        QVERIFY(zip.open(QIODevice::WriteOnly));
        zip.writeFile("esptool-v6/esptool", kBinary, 0755);
        zip.writeFile("esptool-v6/" + QString(300, 'z'), "x", 0644);
        zip.close();
    }
    FlashToolManager mgr;
    const Outcome out = install(mgr, assetFor(path));

    QVERIFY(out.done);
    QVERIFY2(!out.ok, qPrintable(out.text));
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));
}

void TstFlashToolManager::decompressionBombIsRefused()
{
    installOldTool();
    FlashToolManager mgr;
    mgr.setMaxExtractedBytes(16); // the archive unpacks to a few dozen bytes
    const Outcome out = install(mgr, assetFor(makeArchive("ok.tar.gz")));

    QVERIFY(out.done);
    QVERIFY2(!out.ok, qPrintable(out.text));
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));
}

// Small on the wire, big once inflated: the walk of the archive must not be what stops it.
void TstFlashToolManager::inflatedBombIsCutOffWhileReading()
{
    installOldTool();
    const QString archive = makeArchive("bomb.tar.gz", [](KTar &tar)
    {
        tar.writeFile("esptool-v6/big", QByteArray(40 * 1024 * 1024, '\0'), 0644);
    });
    QVERIFY(QFileInfo(archive).size() < 1024 * 1024);

    FlashToolManager mgr;
    mgr.setMaxExtractedBytes(1024 * 1024);
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Cannot open tar archive"));
    const Outcome out = install(mgr, assetFor(archive));

    QVERIFY(out.done);
    QVERIFY2(!out.ok, qPrintable(out.text));
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
}

void TstFlashToolManager::boundedDeviceStopsAtItsLimit()
{
    const QString archive = makeArchive("bomb2.tar.gz", [](KTar &tar)
    {
        tar.writeFile("esptool-v6/big", QByteArray(8 * 1024 * 1024, '\0'), 0644);
    });
    QFile file(archive);
    QVERIFY(file.open(QIODevice::ReadOnly));
    KCompressionDevice gz(&file, false, KCompressionDevice::GZip);
    QVERIFY(gz.open(QIODevice::ReadOnly));
    BoundedDevice bounded(&gz, 100000);
    QVERIFY(bounded.open(QIODevice::ReadOnly));
    QVERIFY(!bounded.seek(5000000)); // skipping ahead is bounded too
    QVERIFY(bounded.exceeded());
    QVERIFY(bounded.seek(0));

    qint64 total = 0;
    char buf[4096];
    qint64 n;
    while ((n = bounded.read(buf, sizeof buf)) > 0)
        total += n;
    QCOMPARE(n, qint64(-1));
    QCOMPARE(total, qint64(100000));
    QVERIFY(bounded.exceeded());
}

void TstFlashToolManager::tooManyEntriesAreRefused()
{
    installOldTool();
    const QString archive = makeArchive("many.tar.gz", [](KTar &tar)
    {
        for (int i = 0; i < 20; ++i)
            tar.writeFile(QString("esptool-v6/f%1").arg(i), "x", 0644);
    });
    FlashToolManager mgr;
    mgr.setMaxArchiveEntries(10);
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Unsafe entry"));
    const Outcome out = install(mgr, assetFor(archive));

    QVERIFY(out.done);
    QVERIFY2(!out.ok, qPrintable(out.text));
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));

    // The same archive passes with room for it.
    FlashToolManager roomy;
    roomy.setMaxArchiveEntries(100);
    QVERIFY2(install(roomy, assetFor(archive)).ok, "entries within the cap");
}

void TstFlashToolManager::tooDeepArchiveIsRefused()
{
    installOldTool();
    const QString archive = makeArchive("deep.tar.gz", [](KTar &tar)
    {
        tar.writeFile("esptool-v6/a/b/c/d/e/f/g/h/file", "x", 0644);
    });
    FlashToolManager mgr;
    mgr.setMaxArchiveDepth(4);
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Unsafe entry"));
    const Outcome out = install(mgr, assetFor(archive));

    QVERIFY(out.done);
    QVERIFY2(!out.ok, qPrintable(out.text));
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));

    FlashToolManager roomy;
    roomy.setMaxArchiveDepth(16);
    QVERIFY2(install(roomy, assetFor(archive)).ok, "depth within the cap");
}

// After a double failure the previous tool only exists in .old; a second failed attempt
// must not delete it, since the error message sends the user there.
void TstFlashToolManager::previousToolSurvivesTwoFailedRestores()
{
    installOldTool();
    const QString archive = makeArchive("ok.tar.gz");
    for (int attempt = 0; attempt < 2; ++attempt)
    {
        TestManager mgr;
        mgr.failRenameFrom = ".new";
        mgr.alsoFailFrom = ".old";
        const Outcome out = install(mgr, assetFor(archive));
        QVERIFY(out.done);
        QVERIFY(!out.ok);
        QVERIFY2(out.text.contains(m_toolDir + ".old"), qPrintable(out.text));
        QVERIFY2(QFileInfo::exists(m_toolDir + ".old/stale.txt"), qPrintable(QString::number(attempt)));
    }
}

void TstFlashToolManager::leftoverOldToolIsRestoredBeforeTheNextInstall()
{
    installOldTool();
    const QString archive = makeArchive("ok.tar.gz");
    {
        TestManager mgr;
        mgr.failRenameFrom = ".new";
        mgr.alsoFailFrom = ".old";
        QVERIFY(!install(mgr, assetFor(archive)).ok);
    }
    QVERIFY(!QFileInfo::exists(m_toolDir));

    TestManager mgr;
    mgr.failRenameFrom = ".new"; // the install fails again, but the previous tool comes back
    const Outcome out = install(mgr, assetFor(archive));
    QVERIFY(!out.ok);
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));
}

// upgrade_tool has no digest to check; its download is only bounded.
void TstFlashToolManager::directDownloadIsBounded()
{
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));
    QObject::connect(&server, &QTcpServer::newConnection, &server, [&server]()
    {
        QTcpSocket *sock = server.nextPendingConnection();
        QObject::connect(sock, &QTcpSocket::readyRead, sock, [sock]()
        {
            sock->readAll();
            sock->write("HTTP/1.1 200 OK\r\nContent-Length: 300000000\r\n\r\n");
        });
    });

    FlashConfig c;
    c.tool = "upgrade_tool";
    const QString url = QString("http://127.0.0.1:%1/upgrade_tool.zip").arg(server.serverPort());
    for (const QString &os : { "linux", "macos", "windows" })
        c.toolUrls.insert(os, url);
    FlashToolManager mgr;
    Outcome out;
    mgr.downloadTool(c, [&](bool ok, const QString &text) { out.done = true; out.ok = ok; out.text = text; });
    QTRY_VERIFY_WITH_TIMEOUT(out.done, 5000);

    QVERIFY(!out.ok);
    QVERIFY2(out.text.contains("larger"), qPrintable(out.text));
    QVERIFY(!QFileInfo::exists(FlashToolManager::toolsCacheDir() + "/upgrade_tool/upgrade_tool.zip"));
}

Outcome TstFlashToolManager::directInstall(FlashToolManager &mgr, const QString &zip)
{
    FlashConfig c;
    c.tool = "upgrade_tool";
    for (const QString &os : { "linux", "macos", "windows" })
        c.toolUrls.insert(os, QUrl::fromLocalFile(zip).toString());
    Outcome out;
    mgr.downloadTool(c, [&](bool ok, const QString &text) { out.done = true; out.ok = ok; out.text = text; });
    for (int i = 0; i < 100 && !out.done; ++i)
        QTest::qWait(50);
    return out;
}

// copyTo() fails midway: the files it already wrote must not survive for a later run.
void TstFlashToolManager::failedDirectExtractionLeavesNothingToRun()
{
    const QString toolDir = FlashToolManager::toolsCacheDir() + "/upgrade_tool";
    const QString bad = m_work.filePath("bad_upgrade_tool.zip");
    {
        KZip zip(bad);
        QVERIFY(zip.open(QIODevice::WriteOnly));
        zip.writeFile("upgrade_tool", "partial", 0755);
        zip.writeFile(QString(300, 'z'), "x", 0644);
        zip.close();
    }
    FlashToolManager mgr;
    const Outcome failed = directInstall(mgr, bad);
    QVERIFY(failed.done);
    QVERIFY(!failed.ok);
    QVERIFY(!QFileInfo::exists(toolDir + "/upgrade_tool"));
    QVERIFY(!QFileInfo::exists(toolDir + "/version.txt"));
    QVERIFY(!QFileInfo::exists(toolDir + ".new"));

    // The retry finds the binary in a subfolder: a top-level leftover would be taken instead.
    const QString good = m_work.filePath("good_upgrade_tool.zip");
    {
        KZip zip(good);
        QVERIFY(zip.open(QIODevice::WriteOnly));
        zip.writeFile("upgrade_tool_v1/upgrade_tool", "fresh", 0755);
        zip.close();
    }
    const Outcome retry = directInstall(mgr, good);
    QVERIFY2(retry.ok, qPrintable(retry.text));
    QFile f(retry.text);
    QVERIFY(f.open(QIODevice::ReadOnly));
    QCOMPARE(f.readAll(), QByteArray("fresh"));
}

void TstFlashToolManager::directInstallKeepsOnlyTheNewFiles()
{
    const QString toolDir = FlashToolManager::toolsCacheDir() + "/upgrade_tool";
    QVERIFY(QDir().mkpath(toolDir));
    QFile stale(toolDir + "/stale.txt");
    QVERIFY(stale.open(QIODevice::WriteOnly));
    stale.close();
    const QString zipPath = m_work.filePath("fresh_upgrade_tool.zip");
    {
        KZip zip(zipPath);
        QVERIFY(zip.open(QIODevice::WriteOnly));
        zip.writeFile("upgrade_tool", "fresh", 0755);
        zip.close();
    }
    FlashToolManager mgr;
    const Outcome out = directInstall(mgr, zipPath);
    QVERIFY2(out.ok, qPrintable(out.text));
    QCOMPARE(out.text, toolDir + "/upgrade_tool");
    QVERIFY(!QFileInfo::exists(toolDir + "/stale.txt"));
    QVERIFY(QFileInfo::exists(toolDir + "/version.txt"));
    QVERIFY(!QFileInfo::exists(toolDir + ".new"));
    QVERIFY(!QFileInfo::exists(toolDir + ".old"));
}

// copyTo() says true although a write fell short: the installed binary would be truncated.
void TstFlashToolManager::shortFileIsRefused()
{
    installOldTool();
    TestManager mgr;
    mgr.shortenAfterCopy = "esptool";
    const Outcome out = install(mgr, assetFor(makeArchive("ok.tar.gz")));

    QVERIFY(out.done);
    QVERIFY2(!out.ok, qPrintable(out.text));
    QVERIFY(QFileInfo::exists(m_toolDir + "/stale.txt"));
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));
}

QString TstFlashToolManager::releasesDir() const
{
    return m_work.filePath("api/repos/test/esptool");
}

// A release newer than the installed v5.0.0, with a build for every Linux CPU esptool ships.
void TstFlashToolManager::writeReleases()
{
    QJsonArray assets;
    for (const QString &arch : { "amd64", "arm64", "armv7" })
    {
        const QString name = QString("esptool-v6.0.0-linux-%1.tar.gz").arg(arch);
        assets.append(QJsonObject{
            { "name", name },
            { "browser_download_url", "https://127.0.0.1:1/" + name },
            { "digest", "sha256:" + QString(64, 'b') },
            { "size", 1000 },
        });
    }
    const QJsonArray releases{ QJsonObject{ { "tag_name", "v6.0.0" }, { "draft", false },
                                            { "prerelease", false }, { "assets", assets } } };
    QVERIFY(QDir().mkpath(releasesDir()));
    QFile f(releasesDir() + "/releases");
    QVERIFY(f.open(QIODevice::WriteOnly));
    f.write(QJsonDocument(releases).toJson());
}

void TstFlashToolManager::refusedUpdateIsNotInstalledNorProposedAgain()
{
    installOldTool();
    writeReleases();
    FlashToolManager mgr;
    mgr.setApiBaseUrl(QUrl::fromLocalFile(m_work.filePath("api")).toString());

    int asked = 0;
    auto run = [&]() -> Outcome
    {
        Outcome out;
        mgr.updateCachedTool(esptoolConfig(),
            [&](const QString &, const QString &) { ++asked; return false; },
            [&](bool updated, const QString &, const QString &error)
            {
                out.done = true;
                out.ok = updated;
                out.text = error;
            });
        for (int i = 0; i < 100 && !out.done; ++i)
            QTest::qWait(50);
        return out;
    };

    const Outcome first = run();
    QVERIFY(first.done);
    QVERIFY(!first.ok);
    QVERIFY(first.text.isEmpty());
    QCOMPARE(asked, 1);
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));

    const Outcome second = run();
    QVERIFY(second.done);
    QVERIFY(!second.ok);
    QCOMPARE(asked, 1);
}

void TstFlashToolManager::acceptedUpdateFailureIsReported()
{
    installOldTool();
    writeReleases();
    FlashToolManager mgr;
    mgr.setApiBaseUrl(QUrl::fromLocalFile(m_work.filePath("api")).toString());

    Outcome out;
    QString path;
    mgr.updateCachedTool(esptoolConfig(),
        [&](const QString &, const QString &) { return true; },
        [&](bool updated, const QString &p, const QString &error)
        {
            out.done = true;
            out.ok = updated;
            out.text = error;
            path = p;
        });
    QTRY_VERIFY_WITH_TIMEOUT(out.done, 10000);

    QVERIFY(!out.ok);
    QVERIFY(!out.text.isEmpty());
    QCOMPARE(path, m_toolDir + "/esptool");
    QCOMPARE(mgr.cachedToolVersion("esptool"), QString("v5.0.0"));
}

void TstFlashToolManager::otherToolsAreNeverUpdated()
{
    FlashConfig c = esptoolConfig();
    c.tool = "upgrade_tool";
    FlashToolManager mgr;

    Outcome dl;
    mgr.downloadTool(c, [&](bool ok, const QString &text) { dl.done = true; dl.ok = ok; dl.text = text; });
    QVERIFY(dl.done);
    QVERIFY(!dl.ok);
    QVERIFY(!dl.text.isEmpty());

    // Even with a cached copy, the GitHub flow stays closed to it.
    QVERIFY(QDir().mkpath(FlashToolManager::toolsCacheDir() + "/upgrade_tool"));
    QFile tool(FlashToolManager::toolsCacheDir() + "/upgrade_tool/upgrade_tool");
    QVERIFY(tool.open(QIODevice::WriteOnly));
    tool.write("x");
    tool.close();
    QFile::setPermissions(tool.fileName(), QFile::permissions(tool.fileName()) | QFileDevice::ExeOwner);
    bool asked = false;
    Outcome up;
    mgr.updateCachedTool(c, [&](const QString &, const QString &) { asked = true; return true; },
                         [&](bool updated, const QString &, const QString &) { up.done = true; up.ok = updated; });
    QVERIFY(up.done);
    QVERIFY(!up.ok);
    QVERIFY(!asked);
}

QTEST_MAIN(TstFlashToolManager)
#include "tst_flashtoolmanager.moc"
