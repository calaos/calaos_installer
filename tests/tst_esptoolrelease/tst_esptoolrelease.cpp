#include <QtTest>
#include <QCryptographicHash>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "EsptoolRelease.h"

// release_v5.6.0.json and release_v4.8.1.json are recorded GitHub API answers (new naming with
// digests, old naming without). The other releases are built from them or from file names.
class TstEsptoolRelease : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void recordedNewScheme_data();
    void recordedNewScheme();
    void oldScheme_data();
    void oldScheme();
    void recordedOldReleaseHasNoDigest();
    void fallsBackToPreviousRelease();
    void newestReleaseWinsWhenItHasAnAsset();
    void highestVersionWinsOverApiOrder();
void underscoreSeparatedNamesAreRecognised();
void draftAndPrereleaseAreIgnored();
    void digestMustBeSha256_data();
    void digestMustBeSha256();
    void missingDigestIsRefused();
    void noBuildForPlatform_data();
    void noBuildForPlatform();
    void nonArchiveAssetsAreIgnored();
    void versionComparison_data();
    void versionComparison();
    void hostSelectionRuns();

private:
    static QJsonObject load(const QString &file);
    static QJsonObject release(const QString &tag, const QStringList &names);

    QJsonObject m_v56;
    QJsonObject m_v481;
};

QJsonObject TstEsptoolRelease::load(const QString &file)
{
    QFile f(QStringLiteral(ESPTOOL_FIXTURE_DIR "/") + file);
    if (!f.open(QIODevice::ReadOnly))
        return QJsonObject();
    return QJsonDocument::fromJson(f.readAll()).object();
}

// Release whose assets all carry a valid digest derived from the file name.
QJsonObject TstEsptoolRelease::release(const QString &tag, const QStringList &names)
{
    QJsonArray assets;
    for (const QString &n : names)
    {
        const QByteArray hex = QCryptographicHash::hash(n.toUtf8(), QCryptographicHash::Sha256).toHex();
        assets.append(QJsonObject{
            { "name", n },
            { "browser_download_url", "https://example.invalid/" + tag + "/" + n },
            { "digest", "sha256:" + QString::fromLatin1(hex) },
        });
    }
    return QJsonObject{ { "tag_name", tag }, { "draft", false }, { "prerelease", false },
                        { "assets", assets } };
}

void TstEsptoolRelease::initTestCase()
{
    m_v56 = load("release_v5.6.0.json");
    m_v481 = load("release_v4.8.1.json");
    QVERIFY(!m_v56.value("assets").toArray().isEmpty());
    QVERIFY(!m_v481.value("assets").toArray().isEmpty());
}

void TstEsptoolRelease::recordedNewScheme_data()
{
    QTest::addColumn<QString>("os");
    QTest::addColumn<QString>("cpu");
    QTest::addColumn<QString>("expectedName");

    QTest::newRow("linux x86_64")   << "linux"   << "x86_64"  << "esptool-v5.6.0-linux-amd64.tar.gz";
    QTest::newRow("linux arm64")    << "linux"   << "arm64"   << "esptool-v5.6.0-linux-aarch64.tar.gz";
    QTest::newRow("linux aarch64")  << "linux"   << "aarch64" << "esptool-v5.6.0-linux-aarch64.tar.gz";
    QTest::newRow("linux arm")      << "linux"   << "arm"     << "esptool-v5.6.0-linux-armv7.tar.gz";
    QTest::newRow("macos x86_64")   << "macos"   << "x86_64"  << "esptool-v5.6.0-macos-amd64.tar.gz";
    QTest::newRow("macos arm64")    << "macos"   << "arm64"   << "esptool-v5.6.0-macos-arm64.tar.gz";
    QTest::newRow("windows x86_64") << "windows" << "x86_64"  << "esptool-v5.6.0-windows-amd64.zip";
    // Windows on ARM runs the amd64 build through emulation: no native asset exists.
    QTest::newRow("windows arm64")  << "windows" << "arm64"   << "esptool-v5.6.0-windows-amd64.zip";
}

void TstEsptoolRelease::recordedNewScheme()
{
    QFETCH(QString, os);
    QFETCH(QString, cpu);
    QFETCH(QString, expectedName);

    const EsptoolAsset a = selectEsptoolAsset(QJsonArray{ m_v56 }, os, cpu);
    QVERIFY2(a.isValid(), qPrintable(a.error));
    QCOMPARE(a.name, expectedName);
    QCOMPARE(a.tag, QString("v5.6.0"));

    // The URL and the hash must come from the recorded answer, not from the file name.
    bool found = false;
    for (const QJsonValue &v : m_v56.value("assets").toArray())
    {
        const QJsonObject o = v.toObject();
        if (o.value("name").toString() != a.name)
            continue;
        found = true;
        QCOMPARE(a.url, o.value("browser_download_url").toString());
        QCOMPARE(QString("sha256:") + a.sha256, o.value("digest").toString());
    }
    QVERIFY(found);
}

void TstEsptoolRelease::oldScheme_data()
{
    QTest::addColumn<QString>("os");
    QTest::addColumn<QString>("cpu");
    QTest::addColumn<QString>("expectedName");

    QTest::newRow("linux x86_64")   << "linux"   << "x86_64"  << "esptool-v4.8.1-linux-amd64.tar.gz";
    QTest::newRow("linux arm64")    << "linux"   << "arm64"   << "esptool-v4.8.1-linux-arm64.tar.gz";
    QTest::newRow("linux arm")      << "linux"   << "arm"     << "esptool-v4.8.1-linux-armv7.tar.gz";
    QTest::newRow("macos x86_64")   << "macos"   << "x86_64"  << "esptool-v4.8.1-macos-amd64.tar.gz";
    QTest::newRow("macos arm64")    << "macos"   << "arm64"   << "esptool-v4.8.1-macos-arm64.tar.gz";
    QTest::newRow("windows x86_64") << "windows" << "x86_64"  << "esptool-v4.8.1-win64.zip";
    QTest::newRow("windows arm64")  << "windows" << "arm64"   << "esptool-v4.8.1-win64.zip";
}

void TstEsptoolRelease::oldScheme()
{
    QFETCH(QString, os);
    QFETCH(QString, cpu);
    QFETCH(QString, expectedName);

    const QJsonObject rel = release("v4.8.1", {
        "esptool-v4.8.1-linux-amd64.tar.gz", "esptool-v4.8.1-linux-arm64.tar.gz",
        "esptool-v4.8.1-linux-armv7.tar.gz", "esptool-v4.8.1-macos-amd64.tar.gz",
        "esptool-v4.8.1-macos-arm64.tar.gz", "esptool-v4.8.1-win64.zip" });
    const EsptoolAsset a = selectEsptoolAsset(QJsonArray{ rel }, os, cpu);
    QVERIFY2(a.isValid(), qPrintable(a.error));
    QCOMPARE(a.name, expectedName);
}

void TstEsptoolRelease::recordedOldReleaseHasNoDigest()
{
    // The real 4.8.1 answer: universal macos zip, arm32/arm64 names, digest null everywhere.
    const struct { const char *os; const char *cpu; const char *name; } rows[] = {
        { "linux", "x86_64", "esptool-v4.8.1-linux-amd64.zip" },
        { "linux", "arm", "esptool-v4.8.1-linux-arm32.zip" },
        { "linux", "arm64", "esptool-v4.8.1-linux-arm64.zip" },
        { "macos", "arm64", "esptool-v4.8.1-macos.zip" },
        { "windows", "x86_64", "esptool-v4.8.1-win64.zip" },
    };
    for (const auto &r : rows)
    {
        const EsptoolAsset a = selectEsptoolAsset(QJsonArray{ m_v481 }, r.os, r.cpu);
        QCOMPARE(a.name, QString(r.name));
        QVERIFY(!a.isValid());
        QVERIFY2(a.error.contains("SHA-256"), qPrintable(a.error));
    }
}

void TstEsptoolRelease::fallsBackToPreviousRelease()
{
    const QJsonObject newest = release("v6.0.0", { "esptool-v6.0.0-linux-amd64.tar.gz" });
    const QJsonObject older = release("v5.6.0", { "esptool-v5.6.0-linux-amd64.tar.gz",
                                                  "esptool-v5.6.0-windows-amd64.zip" });
    const EsptoolAsset a = selectEsptoolAsset(QJsonArray{ newest, older }, "windows", "x86_64");
    QVERIFY2(a.isValid(), qPrintable(a.error));
    QCOMPARE(a.tag, QString("v5.6.0"));
    QCOMPARE(a.name, QString("esptool-v5.6.0-windows-amd64.zip"));
}

void TstEsptoolRelease::newestReleaseWinsWhenItHasAnAsset()
{
    const QJsonObject newest = release("v6.0.0", { "esptool-v6.0.0-linux-amd64.tar.gz" });
    const QJsonObject older = release("v5.6.0", { "esptool-v5.6.0-linux-amd64.tar.gz" });
    const EsptoolAsset a = selectEsptoolAsset(QJsonArray{ newest, older }, "linux", "x86_64");
    QCOMPARE(a.tag, QString("v6.0.0"));
}

void TstEsptoolRelease::highestVersionWinsOverApiOrder()
{
    // Order as the API gave it: a 4.x maintenance release comes after a 5.x one.
    const QJsonObject v5 = release("v5.3.1", { "esptool-v5.3.1-linux-amd64.tar.gz" });
    const QJsonObject v4 = release("v4.12.0", { "esptool-v4.12.0-linux-amd64.tar.gz" });
    const QJsonObject v10 = release("v10.0.0", { "esptool-v10.0.0-linux-amd64.tar.gz" });

    QCOMPARE(selectEsptoolAsset(QJsonArray{ v5, v4 }, "linux", "x86_64").tag, QString("v5.3.1"));
    QCOMPARE(selectEsptoolAsset(QJsonArray{ v4, v5 }, "linux", "x86_64").tag, QString("v5.3.1"));
    QCOMPARE(selectEsptoolAsset(QJsonArray{ v5, v4, v10 }, "linux", "x86_64").tag, QString("v10.0.0"));

    // Fallback goes down by version, not by API order.
    const QJsonObject v5win = release("v5.3.1", { "esptool-v5.3.1-windows-amd64.zip" });
    const QJsonObject v4win = release("v4.12.0", { "esptool-v4.12.0-windows-amd64.zip" });
    const QJsonObject noWin = release("v6.0.0", { "esptool-v6.0.0-linux-amd64.tar.gz" });
    QCOMPARE(selectEsptoolAsset(QJsonArray{ v4win, noWin, v5win }, "windows", "x86_64").tag,
             QString("v5.3.1"));
}

void TstEsptoolRelease::underscoreSeparatedNamesAreRecognised()
{
    const QJsonObject rel = release("v6.0.0", { "esptool_linux_amd64.zip",
                                                "esptool-v6.0.0-macos-x86_64.tar.gz" });
    QCOMPARE(selectEsptoolAsset(QJsonArray{ rel }, "linux", "x86_64").name,
             QString("esptool_linux_amd64.zip"));
    QCOMPARE(selectEsptoolAsset(QJsonArray{ rel }, "macos", "x86_64").name,
             QString("esptool-v6.0.0-macos-x86_64.tar.gz"));
}

void TstEsptoolRelease::draftAndPrereleaseAreIgnored()
{
    QJsonObject draft = release("v7.0.0", { "esptool-v7.0.0-linux-amd64.tar.gz" });
    draft["draft"] = true;
    QJsonObject pre = release("v6.1.0b1", { "esptool-v6.1.0b1-linux-amd64.tar.gz" });
    pre["prerelease"] = true;
    const QJsonObject stable = release("v6.0.0", { "esptool-v6.0.0-linux-amd64.tar.gz" });

    const EsptoolAsset a = selectEsptoolAsset(QJsonArray{ draft, pre, stable }, "linux", "x86_64");
    QVERIFY2(a.isValid(), qPrintable(a.error));
    QCOMPARE(a.tag, QString("v6.0.0"));

    const EsptoolAsset none = selectEsptoolAsset(QJsonArray{ draft, pre }, "linux", "x86_64");
    QVERIFY(!none.isValid());
    QVERIFY(none.url.isEmpty());
}

void TstEsptoolRelease::digestMustBeSha256_data()
{
    QTest::addColumn<QString>("digest");

    QTest::newRow("sha512")    << "sha512:" + QString(128, 'a');
    QTest::newRow("no prefix") << QString(64, 'a');
    QTest::newRow("too short") << "sha256:abcd";
    QTest::newRow("not hex")   << "sha256:" + QString(64, 'z');
    QTest::newRow("empty")     << QString();
}

void TstEsptoolRelease::digestMustBeSha256()
{
    QFETCH(QString, digest);

    QJsonObject rel = release("v6.0.0", { "esptool-v6.0.0-linux-amd64.tar.gz" });
    QJsonArray assets = rel.value("assets").toArray();
    QJsonObject asset = assets.at(0).toObject();
    asset["digest"] = digest;
    assets[0] = asset;
    rel["assets"] = assets;

    const EsptoolAsset a = selectEsptoolAsset(QJsonArray{ rel }, "linux", "x86_64");
    QVERIFY(!a.isValid());
    QVERIFY(a.sha256.isEmpty());
    QVERIFY2(a.error.contains("SHA-256"), qPrintable(a.error));
}

void TstEsptoolRelease::missingDigestIsRefused()
{
    QJsonObject rel = release("v6.0.0", { "esptool-v6.0.0-linux-amd64.tar.gz" });
    QJsonArray assets = rel.value("assets").toArray();
    QJsonObject asset = assets.at(0).toObject();
    asset.remove("digest");
    assets[0] = asset;
    rel["assets"] = assets;

    const EsptoolAsset a = selectEsptoolAsset(QJsonArray{ rel }, "linux", "x86_64");
    QVERIFY(!a.isValid());
    QVERIFY2(a.error.contains("SHA-256"), qPrintable(a.error));
}

void TstEsptoolRelease::noBuildForPlatform_data()
{
    QTest::addColumn<QString>("os");
    QTest::addColumn<QString>("cpu");

    QTest::newRow("linux i386")   << "linux"   << "i386";
    QTest::newRow("macos arm")    << "macos"   << "arm";
    QTest::newRow("windows i386") << "windows" << "i386";
    QTest::newRow("unknown os")   << "freebsd" << "x86_64";
}

void TstEsptoolRelease::noBuildForPlatform()
{
    QFETCH(QString, os);
    QFETCH(QString, cpu);
    const EsptoolAsset a = selectEsptoolAsset(QJsonArray{ m_v56 }, os, cpu);
    QVERIFY(!a.isValid());
    QVERIFY(!a.error.isEmpty());
}

void TstEsptoolRelease::nonArchiveAssetsAreIgnored()
{
    const QJsonObject rel = release("v6.0.0", { "esptool-v6.0.0-linux-amd64.tar.gz.sha256",
                                                "esptool-v6.0.0-linux-amd64.sig" });
    QVERIFY(!selectEsptoolAsset(QJsonArray{ rel }, "linux", "x86_64").isValid());
}

void TstEsptoolRelease::versionComparison_data()
{
    QTest::addColumn<QString>("candidate");
    QTest::addColumn<QString>("cached");
    QTest::addColumn<bool>("newer");

    QTest::newRow("newer minor")      << "v5.6.0" << "v5.2.0"  << true;
    QTest::newRow("newer major")      << "v5.0.0" << "v4.9.9"  << true;
    QTest::newRow("numeric not text") << "v5.10.0" << "v5.9.0" << true;
    QTest::newRow("same")             << "v5.6.0" << "v5.6.0"  << false;
    QTest::newRow("older")            << "v5.2.0" << "v5.6.0"  << false;
    QTest::newRow("no cached version") << "v5.6.0" << QString() << true;
    QTest::newRow("latest from 4.6")  << "v5.6.0" << "latest"  << true;
    QTest::newRow("bad candidate")    << "nightly" << "v5.6.0" << false;
}

void TstEsptoolRelease::versionComparison()
{
    QFETCH(QString, candidate);
    QFETCH(QString, cached);
    QFETCH(bool, newer);
    QCOMPARE(isNewerEsptoolVersion(candidate, cached), newer);
}

void TstEsptoolRelease::hostSelectionRuns()
{
    // The host may be a CPU without a build; the call must still answer cleanly.
    const EsptoolAsset a = selectEsptoolAssetForHost(QJsonArray{ m_v56 });
    QVERIFY(a.isValid() || !a.error.isEmpty());
}

QTEST_APPLESS_MAIN(TstEsptoolRelease)
#include "tst_esptoolrelease.moc"
