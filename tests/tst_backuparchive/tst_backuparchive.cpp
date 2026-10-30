#include "BackupArchive.h"

#include <KZip>

#include <QBuffer>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QTemporaryDir>
#include <QTest>

namespace
{

const QByteArray kIo = "<?xml version=\"1.0\"?><calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">"
                       "<calaos:home/></calaos:ioconfig>";
const QByteArray kRules = "<?xml version=\"1.0\"?><calaos:rules xmlns:calaos=\"http://www.calaos.fr\"/>";

struct RawEntry
{
    QString name;
    QByteArray data;
    // Hostile archives: what the headers claim, and the method of data as given.
    qint64 declaredSize = -1;
    quint16 method = 0;
};

QByteArray rawDeflate(const QByteArray &data)
{
    // qCompress: 4 bytes of length, then a zlib stream around the raw deflate data.
    const QByteArray zlib = qCompress(data);
    return zlib.mid(6, zlib.size() - 10);
}

// Hand-written like the web app's zip (stored entries, no data descriptor), and the only way
// to get names and sizes KZip would not write itself. KZip does not check the CRC.
QByteArray buildZip(const QList<RawEntry> &entries)
{
    QByteArray zip;
    QByteArray central;
    QDataStream body(&zip, QIODevice::WriteOnly);
    body.setByteOrder(QDataStream::LittleEndian);
    QDataStream directory(&central, QIODevice::WriteOnly);
    directory.setByteOrder(QDataStream::LittleEndian);

    for (const RawEntry &entry : entries)
    {
        const QByteArray name = entry.name.toLatin1();
        const quint32 size = entry.declaredSize < 0 ? entry.data.size() : entry.declaredSize;
        const quint32 offset = zip.size();

        body << quint32(0x04034b50) << quint16(20) << quint16(0) << entry.method << quint16(0)
             << quint16(0x21) << quint32(0) << quint32(entry.data.size()) << size
             << quint16(name.size()) << quint16(0);
        body.writeRawData(name.constData(), name.size());
        body.writeRawData(entry.data.constData(), entry.data.size());

        directory << quint32(0x02014b50) << quint16(20) << quint16(20) << quint16(0) << entry.method
                  << quint16(0) << quint16(0x21) << quint32(0) << quint32(entry.data.size()) << size
                  << quint16(name.size()) << quint16(0) << quint16(0) << quint16(0) << quint16(0)
                  << quint32(0) << offset;
        directory.writeRawData(name.constData(), name.size());
    }

    const quint32 centralOffset = zip.size();
    zip += central;

    QDataStream end(&zip, QIODevice::Append);
    end.setByteOrder(QDataStream::LittleEndian);
    end << quint32(0x06054b50) << quint16(0) << quint16(0) << quint16(entries.size())
        << quint16(entries.size()) << quint32(central.size()) << centralOffset << quint16(0);

    return zip;
}

QList<RawEntry> validEntries()
{
    return {{"io.xml", kIo}, {"rules.xml", kRules}};
}

} // namespace

class TestBackupArchive : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_dir;
    int m_count = 0;

    QString saveZip(const QByteArray &bytes)
    {
        const QString path = m_dir.filePath(QStringLiteral("archive%1.zip").arg(++m_count));
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
            qFatal("cannot write the test archive");
        return path;
    }

    QString saveZip(const QList<RawEntry> &entries) { return saveZip(buildZip(entries)); }

    QString deflatedZip(const QHash<QString, QByteArray> &files)
    {
        const QString path = m_dir.filePath(QStringLiteral("archive%1.zip").arg(++m_count));
        KZip zip(path);
        if (!zip.open(QIODevice::WriteOnly))
            qFatal("cannot create the test archive");
        for (auto it = files.cbegin(); it != files.cend(); ++it)
            zip.writeFile(it.key(), it.value());
        zip.close();
        return path;
    }

private slots:
    void backupFromTheWebApp();
    void zipWrittenByTheWebAppCode();
    void zipWrittenByAnotherTool();
    void projectFolderZipped();
    void localConfigIsNoticedNotRead();
    void macosLeftoversAreIgnored();
    void sampleProjectsOpen();
    void missingFile_data();
    void missingFile();
    void invalidFileIsRefused_data();
    void invalidFileIsRefused();
    void notAZip();
    void missingArchive();
    void entryAtTheLimit();
    void entryTooLarge();
    void anyEntryIsLimited();
    void compressedSizeIsNotTheLimit();
    void declaredSizeIsNotTrusted();
    void truncatedEntry();
    void entryCountIsLimited();
    void archiveTooLarge();
    void traversalIsRefused_data();
    void traversalIsRefused();
    void sameFileInTwoFolders();
    void twoFoldersAreRefused_data();
    void twoFoldersAreRefused();
    void backslashFolder();
    void samePathTwiceIsRefusedByKZip();
    void failureLeavesTheProjectAlone();
    void writeReplacesTheProjectFiles();
    void writeFailure();
};

void TestBackupArchive::backupFromTheWebApp()
{
    const QString zip = saveZip({{"io.xml", kIo},
                                 {"rules.xml", kRules},
                                 {"local_config.xml", "<?xml version=\"1.0\"?><calaos:config/>"},
                                 {"calaos-backup.json", "{\"format\": \"calaos-backup/2\"}"},
                                 {"README.txt", "Contains passwords."}});

    BackupArchive::Project project;
    QString error;
    QVERIFY2(BackupArchive::read(zip, project, error), qPrintable(error));
    QCOMPARE(project.io, kIo);
    QCOMPARE(project.rules, kRules);
}

// The zip, and the three files it was made from, are produced by the web app's own encodeZip.
void TestBackupArchive::zipWrittenByTheWebAppCode()
{
    const QString dir = QStringLiteral(CALAOS_SAMPLES_DIR "/tst_backuparchive/fixtures/");
    const auto fixture = [&dir](const QString &name) {
        QFile file(dir + name);
        if (!file.open(QIODevice::ReadOnly))
            qFatal("missing fixture %s", qPrintable(name));
        return file.readAll();
    };

    BackupArchive::Project project;
    QString error;
    QVERIFY2(BackupArchive::read(dir + QStringLiteral("calaos-backup-maison-20261025-0912.zip"),
                                 project, error),
             qPrintable(error));
    QVERIFY(project.io.contains("<calaos:room name=\"Salon\""));
    QCOMPARE(project.io, fixture(QStringLiteral("io.xml")));
    QVERIFY(project.rules.contains("lumiere_salon"));
    QCOMPARE(project.rules, fixture(QStringLiteral("rules.xml")));
    QVERIFY(project.hasLocalConfig);
    QVERIFY(!fixture(QStringLiteral("local_config.xml")).isEmpty());
}

void TestBackupArchive::zipWrittenByAnotherTool()
{
    const QString zip = deflatedZip({{"io.xml", kIo}, {"rules.xml", kRules}});

    BackupArchive::Project project;
    QString error;
    QVERIFY2(BackupArchive::read(zip, project, error), qPrintable(error));
    QCOMPARE(project.io, kIo);
    QCOMPARE(project.rules, kRules);
    QVERIFY(!project.hasLocalConfig);
}

void TestBackupArchive::projectFolderZipped()
{
    const QString zip = saveZip({{"maison/", {}},
                                 {"maison/io.xml", kIo},
                                 {"./maison/rules.xml", kRules}});

    BackupArchive::Project project;
    QString error;
    QVERIFY2(BackupArchive::read(zip, project, error), qPrintable(error));
    QCOMPARE(project.io, kIo);
    QCOMPARE(project.rules, kRules);
}

void TestBackupArchive::localConfigIsNoticedNotRead()
{
    // Unreadable on purpose: reading it would fail the whole archive.
    RawEntry localConfig{"local_config.xml", "not deflate", -1, 99};
    QList<RawEntry> entries = validEntries();
    entries.append(localConfig);

    BackupArchive::Project project;
    QString error;
    QVERIFY2(BackupArchive::read(saveZip(entries), project, error), qPrintable(error));
    QVERIFY(project.hasLocalConfig);
}

void TestBackupArchive::macosLeftoversAreIgnored()
{
    const QString zip = saveZip({{"__MACOSX/io.xml", "resource fork"},
                                 {".hidden/rules.xml", "resource fork"},
                                 {"maison/.io.xml", "resource fork"},
                                 {"maison/io.xml", kIo},
                                 {"maison/rules.xml", kRules}});

    BackupArchive::Project project;
    QString error;
    QVERIFY2(BackupArchive::read(zip, project, error), qPrintable(error));
    QCOMPARE(project.io, kIo);
    QCOMPARE(project.rules, kRules);
}

void TestBackupArchive::sampleProjectsOpen()
{
    int checked = 0;

    const QStringList folders = QDir(CALAOS_SAMPLES_DIR).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folder : folders)
    {
        QFile io(QStringLiteral(CALAOS_SAMPLES_DIR "/%1/io.xml").arg(folder));
        QFile rules(QStringLiteral(CALAOS_SAMPLES_DIR "/%1/rules.xml").arg(folder));
        if (!io.open(QIODevice::ReadOnly) || !rules.open(QIODevice::ReadOnly))
            continue;

        const QString zip = deflatedZip({{"io.xml", io.readAll()}, {"rules.xml", rules.readAll()}});

        BackupArchive::Project project;
        QString error;
        QVERIFY2(BackupArchive::read(zip, project, error), qPrintable(folder + ": " + error));
        ++checked;
    }

    QVERIFY(checked > 0);
}

void TestBackupArchive::missingFile_data()
{
    QTest::addColumn<QStringList>("names");
    QTest::addColumn<QStringList>("expected");

    QTest::newRow("no io.xml") << QStringList{"rules.xml"} << QStringList{"io.xml"};
    QTest::newRow("no rules.xml") << QStringList{"io.xml"} << QStringList{"rules.xml"};
    QTest::newRow("neither") << QStringList{"README.txt"} << QStringList{"io.xml", "rules.xml"};
    QTest::newRow("too deep") << QStringList{"a/b/io.xml", "a/b/rules.xml"}
                              << QStringList{"io.xml", "rules.xml"};
    QTest::newRow("case matters") << QStringList{"IO.XML", "rules.xml"} << QStringList{"io.xml"};
}

void TestBackupArchive::missingFile()
{
    QFETCH(QStringList, names);
    QFETCH(QStringList, expected);

    QList<RawEntry> entries;
    for (const QString &name : std::as_const(names))
        entries.append({name, "<?xml version=\"1.0\"?>"});

    BackupArchive::Project project;
    QString error;
    QVERIFY(!BackupArchive::read(saveZip(entries), project, error));
    QVERIFY2(error.contains("not a Calaos backup"), qPrintable(error));
    for (const QString &name : std::as_const(expected))
        QVERIFY2(error.contains(name), qPrintable(error));
}

void TestBackupArchive::invalidFileIsRefused_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QByteArray>("content");

    const QByteArray ns = "xmlns:calaos=\"http://www.calaos.fr\"";

    QTest::newRow("empty rules.xml") << "rules.xml" << QByteArray();
    QTest::newRow("truncated rules.xml")
        << "rules.xml" << "<calaos:rules " + ns + "><calaos:rule name=\"a\"";
    QTest::newRow("rules.xml, not XML") << "rules.xml" << QByteArray("not XML");
    QTest::newRow("rules.xml holding an io.xml") << "rules.xml" << kIo;
    QTest::newRow("rules.xml, no prefix") << "rules.xml" << QByteArray("<rules><rule/></rules>");
    QTest::newRow("rules.xml, nothing but a prolog") << "rules.xml" << QByteArray("<?xml version=\"1.0\"?>");

    QTest::newRow("empty io.xml") << "io.xml" << QByteArray();
    QTest::newRow("truncated io.xml") << "io.xml" << "<calaos:ioconfig " + ns + "><calaos:home>";
    QTest::newRow("io.xml holding a rules.xml") << "io.xml" << kRules;
    QTest::newRow("io.xml starting at home") << "io.xml" << "<calaos:home " + ns + "/>";
    QTest::newRow("io.xml, unknown top element")
        << "io.xml" << "<calaos:ioconfig " + ns + "><calaos:other/></calaos:ioconfig>";
    QTest::newRow("io.xml, a sibling after home")
        << "io.xml" << "<calaos:ioconfig " + ns + "><calaos:home/><calaos:other/></calaos:ioconfig>";
    QTest::newRow("io.xml, undeclared prefix") << "io.xml" << QByteArray("<calaos:ioconfig/>");
}

void TestBackupArchive::invalidFileIsRefused()
{
    QFETCH(QString, name);
    QFETCH(QByteArray, content);

    const QList<RawEntry> entries{{"io.xml", name == "io.xml" ? content : kIo},
                                  {"rules.xml", name == "rules.xml" ? content : kRules}};

    BackupArchive::Project project;
    project.io = "open project";
    project.rules = "open project";

    QString error;
    QVERIFY(!BackupArchive::read(saveZip(entries), project, error));
    QVERIFY2(error.contains(name) && error.contains("not a valid"), qPrintable(error));
    QCOMPARE(project.io, QByteArray("open project"));
    QCOMPARE(project.rules, QByteArray("open project"));
}

void TestBackupArchive::notAZip()
{
    BackupArchive::Project project;
    QString error;
    QVERIFY(!BackupArchive::read(saveZip(QByteArray("<?xml version=\"1.0\"?>")), project, error));
    QVERIFY2(error.contains("zip"), qPrintable(error));
}

void TestBackupArchive::missingArchive()
{
    BackupArchive::Project project;
    QString error;
    QVERIFY(!BackupArchive::read(m_dir.filePath("nothing.zip"), project, error));
    QVERIFY(!error.isEmpty());
}

void TestBackupArchive::entryAtTheLimit()
{
    const qint64 padding = BackupArchive::kMaxEntrySize - kIo.size() - strlen("<!---->");
    const QByteArray largest = kIo + "<!--" + QByteArray(padding, 'x') + "-->";
    const QString zip = saveZip({{"io.xml", largest}, {"rules.xml", kRules}});

    BackupArchive::Project project;
    QString error;
    QVERIFY2(BackupArchive::read(zip, project, error), qPrintable(error));
    QCOMPARE(project.io.size(), BackupArchive::kMaxEntrySize);
}

void TestBackupArchive::entryTooLarge()
{
    const QByteArray tooLarge(BackupArchive::kMaxEntrySize + 1, 'x');
    const QString zip = saveZip({{"io.xml", tooLarge}, {"rules.xml", kRules}});

    BackupArchive::Project project;
    QString error;
    QVERIFY(!BackupArchive::read(zip, project, error));
    QVERIFY2(error.contains("io.xml") && error.contains("2 MiB"), qPrintable(error));
}

void TestBackupArchive::anyEntryIsLimited()
{
    QList<RawEntry> entries = validEntries();
    entries.append({"README.txt", QByteArray(BackupArchive::kMaxEntrySize + 1, 'x')});

    BackupArchive::Project project;
    QString error;
    QVERIFY(!BackupArchive::read(saveZip(entries), project, error));
    QVERIFY2(error.contains("README.txt"), qPrintable(error));
}

void TestBackupArchive::compressedSizeIsNotTheLimit()
{
    const QString zip = deflatedZip({{"io.xml", QByteArray(BackupArchive::kMaxEntrySize + 1, 'x')},
                                     {"rules.xml", kRules}});
    QVERIFY(QFile(zip).size() < BackupArchive::kMaxEntrySize / 100);

    BackupArchive::Project project;
    QString error;
    QVERIFY(!BackupArchive::read(zip, project, error));
    QVERIFY2(error.contains("2 MiB"), qPrintable(error));
}

void TestBackupArchive::declaredSizeIsNotTrusted()
{
    const QByteArray bomb = rawDeflate(QByteArray(64 * 1024 * 1024, 'x'));
    QVERIFY(bomb.size() < BackupArchive::kMaxEntrySize);

    BackupArchive::Project project;
    QString error;
    const QString zip = saveZip({{"io.xml", bomb, 10, 8}, {"rules.xml", kRules}});
    QVERIFY(!BackupArchive::read(zip, project, error));
    QVERIFY2(error.contains("io.xml") && error.contains("2 MiB"), qPrintable(error));
}

void TestBackupArchive::truncatedEntry()
{
    BackupArchive::Project project;
    QString error;
    const QString zip = saveZip({{"io.xml", rawDeflate(kIo), 4096, 8}, {"rules.xml", kRules}});
    QVERIFY(!BackupArchive::read(zip, project, error));
    QVERIFY2(error.contains("io.xml") && error.contains("damaged"), qPrintable(error));
}

void TestBackupArchive::entryCountIsLimited()
{
    QList<RawEntry> entries = validEntries();
    while (entries.size() < BackupArchive::kMaxEntries)
        entries.append({QStringLiteral("extra%1.txt").arg(entries.size()), "x"});

    BackupArchive::Project project;
    QString error;
    QVERIFY2(BackupArchive::read(saveZip(entries), project, error), qPrintable(error));

    entries.append(RawEntry{"one_too_many.txt", "x"});
    QVERIFY(!BackupArchive::read(saveZip(entries), project, error));
    QVERIFY2(error.contains(QString::number(BackupArchive::kMaxEntries)), qPrintable(error));
}

void TestBackupArchive::archiveTooLarge()
{
    QByteArray bytes = buildZip(validEntries());
    bytes.append(BackupArchive::kMaxArchiveSize, '\0');

    BackupArchive::Project project;
    QString error;
    QVERIFY(!BackupArchive::read(saveZip(bytes), project, error));
    QVERIFY2(error.contains("8 MiB"), qPrintable(error));
}

void TestBackupArchive::traversalIsRefused_data()
{
    QTest::addColumn<QString>("name");

    QTest::newRow("parent") << "../io.xml";
    QTest::newRow("parent, deeper") << "a/../../io.xml";
    QTest::newRow("back to the root") << "a/../other.txt";
    QTest::newRow("absolute") << "/tmp/io.xml";
    QTest::newRow("backslashes") << "..\\io.xml";
    QTest::newRow("drive") << "C:/io.xml";
    QTest::newRow("a file the installer would not even read") << "../evil.txt";
    QTest::newRow("a folder") << "../folder/";
}

void TestBackupArchive::traversalIsRefused()
{
    QFETCH(QString, name);

    QList<RawEntry> entries = validEntries();
    entries.append({name, name.endsWith('/') ? QByteArray() : QByteArray("x")});

    BackupArchive::Project project;
    QString error;
    QVERIFY(!BackupArchive::read(saveZip(entries), project, error));
    QVERIFY2(error.contains("refused"), qPrintable(error));
}

void TestBackupArchive::sameFileInTwoFolders()
{
    QList<RawEntry> entries = validEntries();
    entries.append({"copy/io.xml", kIo});

    BackupArchive::Project project;
    QString error;
    QVERIFY(!BackupArchive::read(saveZip(entries), project, error));
    QVERIFY2(error.contains("io.xml") && error.contains("twice"), qPrintable(error));
}

void TestBackupArchive::twoFoldersAreRefused_data()
{
    QTest::addColumn<QStringList>("names");

    QTest::newRow("side by side") << QStringList{"a/io.xml", "b/rules.xml"};
    QTest::newRow("io.xml at the root") << QStringList{"io.xml", "a/rules.xml"};
    QTest::newRow("rules.xml at the root") << QStringList{"a/io.xml", "rules.xml"};
    QTest::newRow("backslashes") << QStringList{"a\\io.xml", "b\\rules.xml"};
    QTest::newRow("local_config.xml elsewhere")
        << QStringList{"a/io.xml", "a/rules.xml", "b/local_config.xml"};
    QTest::newRow("local_config.xml at the root")
        << QStringList{"a/io.xml", "a/rules.xml", "local_config.xml"};
    QTest::newRow("local_config.xml in a folder")
        << QStringList{"io.xml", "rules.xml", "a/local_config.xml"};
}

void TestBackupArchive::twoFoldersAreRefused()
{
    QFETCH(QStringList, names);

    QList<RawEntry> entries;
    for (const QString &name : std::as_const(names))
        entries.append({name, name.endsWith("rules.xml") ? kRules : kIo});

    BackupArchive::Project project;
    project.io = "open project";

    QString error;
    QVERIFY(!BackupArchive::read(saveZip(entries), project, error));
    QVERIFY2(error.contains("two project folders"), qPrintable(error));
    QCOMPARE(project.io, QByteArray("open project"));
}

void TestBackupArchive::backslashFolder()
{
    const QString zip = saveZip({{"maison\\io.xml", kIo}, {"maison\\rules.xml", kRules}});

    BackupArchive::Project project;
    QString error;
    QVERIFY2(BackupArchive::read(zip, project, error), qPrintable(error));
    QCOMPARE(project.io, kIo);
    QCOMPARE(project.rules, kRules);
}

void TestBackupArchive::samePathTwiceIsRefusedByKZip()
{
    QList<RawEntry> entries = validEntries();
    entries.append(RawEntry{"io.xml", "second"});

    BackupArchive::Project project;
    QString error;
    QVERIFY(!BackupArchive::read(saveZip(entries), project, error));
    QVERIFY2(error.contains("zip"), qPrintable(error));
}

void TestBackupArchive::failureLeavesTheProjectAlone()
{
    BackupArchive::Project project;
    project.io = kIo;

    QString error;
    const QString zip = saveZip({{"io.xml", "other"}, {"rules.xml", rawDeflate(kRules), 4096, 8}});
    QVERIFY(!BackupArchive::read(zip, project, error));
    QCOMPARE(project.io, kIo);
}

void TestBackupArchive::writeReplacesTheProjectFiles()
{
    QTemporaryDir target;
    QFile stale(target.filePath("io.xml"));
    QVERIFY(stale.open(QIODevice::WriteOnly));
    QVERIFY(stale.write("a much longer project than the one in the archive, by far, in fact") > 0);
    stale.close();

    BackupArchive::Project project;
    project.io = kIo;
    project.rules = kRules;
    project.hasLocalConfig = true;

    QString error;
    QVERIFY2(BackupArchive::write(project, target.path(), error), qPrintable(error));

    QFile io(target.filePath("io.xml"));
    QVERIFY(io.open(QIODevice::ReadOnly));
    QCOMPARE(io.readAll(), kIo);
    QFile rules(target.filePath("rules.xml"));
    QVERIFY(rules.open(QIODevice::ReadOnly));
    QCOMPARE(rules.readAll(), kRules);
    QVERIFY(!QFile::exists(target.filePath("local_config.xml")));
}

void TestBackupArchive::writeFailure()
{
    BackupArchive::Project project;
    project.io = kIo;
    project.rules = kRules;

    QString error;
    const QString target = m_dir.filePath("does/not/exist");
    QVERIFY(!BackupArchive::write(project, target, error));
    QVERIFY2(error.contains(target), qPrintable(error));
}

QTEST_GUILESS_MAIN(TestBackupArchive)
#include "tst_backuparchive.moc"
