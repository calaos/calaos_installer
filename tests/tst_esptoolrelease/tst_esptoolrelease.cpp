#include <QtTest>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "EsptoolRelease.h"

// release_v5.6.0.json is the recorded GitHub API answer for the pinned tag. The pinned
// names, URLs and hashes must match it for every platform the installer claims to support.
class TstEsptoolRelease : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void pinnedAssetMatchesRecordedRelease_data();
    void pinnedAssetMatchesRecordedRelease();
    void unsupportedPlatformIsInvalid_data();
    void unsupportedPlatformIsInvalid();
    void hostAssetIsAvailable();

private:
    QJsonObject m_release;
};

void TstEsptoolRelease::initTestCase()
{
    QFile f(QStringLiteral(ESPTOOL_FIXTURE_DIR "/release_v5.6.0.json"));
    QVERIFY(f.open(QIODevice::ReadOnly));
    m_release = QJsonDocument::fromJson(f.readAll()).object();
    QVERIFY(!m_release.value("assets").toArray().isEmpty());
}

void TstEsptoolRelease::pinnedAssetMatchesRecordedRelease_data()
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

void TstEsptoolRelease::pinnedAssetMatchesRecordedRelease()
{
    QFETCH(QString, os);
    QFETCH(QString, cpu);
    QFETCH(QString, expectedName);

    const EsptoolAsset a = esptoolAssetFor(os, cpu);
    QVERIFY(a.isValid());
    QCOMPARE(a.name, expectedName);
    QCOMPARE(a.tag, m_release.value("tag_name").toString());

    bool found = false;
    for (const QJsonValue &v : m_release.value("assets").toArray())
    {
        const QJsonObject o = v.toObject();
        if (o.value("name").toString() != a.name)
            continue;
        found = true;
        QCOMPARE(a.url, o.value("browser_download_url").toString());
        QCOMPARE(QString("sha256:") + a.sha256, o.value("digest").toString());
    }
    QVERIFY2(found, qPrintable(a.name + " is not in the recorded release"));
}

void TstEsptoolRelease::unsupportedPlatformIsInvalid_data()
{
    QTest::addColumn<QString>("os");
    QTest::addColumn<QString>("cpu");

    QTest::newRow("linux i386")     << "linux"   << "i386";
    QTest::newRow("macos arm")      << "macos"   << "arm";
    QTest::newRow("windows i386")   << "windows" << "i386";
    QTest::newRow("unknown os")     << "freebsd" << "x86_64";
}

void TstEsptoolRelease::unsupportedPlatformIsInvalid()
{
    QFETCH(QString, os);
    QFETCH(QString, cpu);
    QVERIFY(!esptoolAssetFor(os, cpu).isValid());
}

void TstEsptoolRelease::hostAssetIsAvailable()
{
    // CI hosts are x86_64 or arm64 Linux/macOS/Windows; all have a build.
    QVERIFY(esptoolAssetForHost().isValid());
}

QTEST_APPLESS_MAIN(TstEsptoolRelease)
#include "tst_esptoolrelease.moc"
