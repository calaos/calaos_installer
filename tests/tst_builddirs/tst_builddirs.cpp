#include <QtTest>
#include <QLibraryInfo>
#include <QProcess>
#include <QRegularExpression>
#include <QTemporaryDir>

// qmake does not make objects depend on Qt's headers: after a Qt update an incremental
// build links objects compiled against the old headers with new ones. Inline Qt
// templates (QtPrivate::QMovableArrayOps<QString>::emplace changed layout in Qt 6.11)
// then resolve to one instance called with the other layout's `this`, and crash.
// Every intermediate file must live in a directory named after the Qt version.
class TstBuildDirs : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void intermediatesArePerQtVersion_data();
    void intermediatesArePerQtVersion();
    void formerLayoutIsRefused_data();
    void formerLayoutIsRefused();

private:
    bool runQmake(const QStringList &args, const QString &workDir, QString *output = nullptr);

    QString m_qmake;
    QString m_qtVersion;
};

bool TstBuildDirs::runQmake(const QStringList &args, const QString &workDir, QString *output)
{
    QProcess p;
    p.setWorkingDirectory(workDir);
    p.setProcessChannelMode(QProcess::MergedChannels);
    p.start(m_qmake, args);
    const bool ok = p.waitForFinished(120000) && p.exitStatus() == QProcess::NormalExit && p.exitCode() == 0;
    const QString out = QString::fromLocal8Bit(p.readAll()).trimmed();
    if (output)
        *output = out;
    else if (!ok)
        qWarning().noquote() << out;
    return ok;
}

void TstBuildDirs::initTestCase()
{
    m_qmake = QLibraryInfo::path(QLibraryInfo::BinariesPath) + QStringLiteral("/qmake");
    QVERIFY2(QFileInfo(m_qmake).isExecutable(), qPrintable(m_qmake));
    QVERIFY(runQmake({QStringLiteral("-query"), QStringLiteral("QT_VERSION")}, QDir::tempPath(), &m_qtVersion));
    QVERIFY(!m_qtVersion.isEmpty());
}

void TstBuildDirs::intermediatesArePerQtVersion_data()
{
    QTest::addColumn<QString>("project");
    QTest::newRow("application") << QStringLiteral("calaos_installer.pro");
    QTest::newRow("machine_creator") << QStringLiteral("machine_creator/machine_creator.pro");
    QTest::newRow("test with the model") << QStringLiteral("tests/tst_timezonehelper/tst_timezonehelper.pro");
    QTest::newRow("test without the model") << QStringLiteral("tests/tst_onlineconfig/tst_onlineconfig.pro");
}

void TstBuildDirs::intermediatesArePerQtVersion()
{
    QFETCH(QString, project);

    QTemporaryDir build;
    QVERIFY(build.isValid());
    const QString pro = QStringLiteral(CALAOS_SOURCE_DIR "/") + project;
    QVERIFY2(runQmake({pro}, build.path()), qPrintable(pro));

    QFile f(build.filePath(QStringLiteral("Makefile")));
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QString makefile = QString::fromLocal8Bit(f.readAll());

    const QString dir = QStringLiteral("build-qt") + m_qtVersion + QLatin1Char('/');

    QRegularExpressionMatch objects =
        QRegularExpression(QStringLiteral("^OBJECTS\\s*=\\s*((?:.*\\\\\\n)*.*)$"),
                           QRegularExpression::MultilineOption).match(makefile);
    QVERIFY(objects.hasMatch());
    const QStringList objs = objects.captured(1).split(QRegularExpression(QStringLiteral("[\\s\\\\]+")),
                                                       Qt::SkipEmptyParts);
    QVERIFY(!objs.isEmpty());
    for (const QString &o : objs)
        QVERIFY2(o.startsWith(dir + QStringLiteral("obj/")), qPrintable(o));

    // Generated sources too: moc output carries a Qt version check of its own, and uic/rcc
    // output is compiled into the objects above.
    QRegularExpression generated(QStringLiteral("[^\\s:]*\\b(?:moc_|qrc_|ui_)[\\w.]+\\.(?:cpp|h)\\b"));
    int seen = 0;
    for (auto it = generated.globalMatch(makefile); it.hasNext();)
    {
        const QString path = it.next().captured(0);
        seen++;
        QVERIFY2(path.contains(dir), qPrintable(path));
    }
    QVERIFY(seen > 0);
}

void TstBuildDirs::formerLayoutIsRefused_data()
{
    QTest::addColumn<QString>("leftover");
    QTest::newRow("moc of a header") << QStringLiteral("moc_qftp.cpp");
    QTest::newRow("moc of a source") << QStringLiteral("qftp.moc");
    QTest::newRow("uic") << QStringLiteral("ui_mainwindow.h");
    QTest::newRow("rcc") << QStringLiteral("qrc_qml.cpp");
}

// A build directory of the former layout would compile stale generated files that
// shadow the new ones: qmake must stop with a message rather than build.
void TstBuildDirs::formerLayoutIsRefused()
{
    QFETCH(QString, leftover);

    QTemporaryDir build;
    QVERIFY(build.isValid());
    QFile f(build.filePath(leftover));
    QVERIFY(f.open(QIODevice::WriteOnly));
    f.close();

    QString output;
    QVERIFY(!runQmake({QStringLiteral(CALAOS_SOURCE_DIR "/calaos_installer.pro")}, build.path(), &output));
    QVERIFY2(output.contains(QStringLiteral("holds files of a build made before")), qPrintable(output));

    QVERIFY(QFile::remove(build.filePath(leftover)));
    QVERIFY(runQmake({QStringLiteral(CALAOS_SOURCE_DIR "/calaos_installer.pro")}, build.path()));
}

QTEST_GUILESS_MAIN(TstBuildDirs)
#include "tst_builddirs.moc"
