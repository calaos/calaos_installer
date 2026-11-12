#include <QtTest>
#include <QApplication>
#include <QQmlContext>
#include <QQuickWidget>
#include <QSet>
#include <QTimeZone>

#include "DialogRemoteUIEditor.h"
#include "ListeRoom.h"
#include "Room.h"
#include "TimezoneHelper.h"
#include "WidgetModel.h"

using namespace Calaos;

class TstTimezoneHelper : public QObject
{
    Q_OBJECT

private slots:
    void listIsNotEmpty();
    void listIsSortedWithoutDuplicates();
    void listKeepsOnlyGeographicZonesAndUtc();
    void listOnlyHoldsValidZones();
    void listIsStableAcrossCalls();
    void editorExposesTheList();
    void legacyUtcOffsetsMapToAnOffsetPreservingZone_data();
    void legacyUtcOffsetsMapToAnOffsetPreservingZone();
    void legacyUtcResaveWritesValidPosix();
    void unknownValuesFallBackToUtc();
    void zonelessOffsetsAreKept();
    void zeroOffsetIsUtc();
    void etcGmtFallback();

    void cleanup();
};

void TstTimezoneHelper::listIsNotEmpty()
{
    const QStringList zones = TimezoneHelper::filteredIanaTimezones();
    QVERIFY(zones.contains(QStringLiteral("UTC")));
    QVERIFY(zones.contains(QStringLiteral("Europe/Paris")));
    QVERIFY(zones.contains(QStringLiteral("America/New_York")));
    QVERIFY(zones.contains(QStringLiteral("Asia/Tokyo")));
}

void TstTimezoneHelper::listIsSortedWithoutDuplicates()
{
    const QStringList zones = TimezoneHelper::filteredIanaTimezones();
    QStringList sorted = zones;
    sorted.sort();
    QCOMPARE(zones, sorted);
    QCOMPARE(QSet<QString>(zones.cbegin(), zones.cend()).size(), zones.size());
}

void TstTimezoneHelper::listKeepsOnlyGeographicZonesAndUtc()
{
    static const QStringList allowed = {
        "Africa/", "America/", "Antarctica/", "Arctic/", "Asia/", "Atlantic/",
        "Australia/", "Europe/", "Indian/", "Pacific/",
    };

    // Present in every tzdata as aliases or non-geographic zones: must be filtered out.
    const QStringList zones = TimezoneHelper::filteredIanaTimezones();
    for (const char *rejected : {"Etc/UTC", "Etc/GMT+1", "US/Eastern", "GMT", "EST5EDT", "Zulu"})
        QVERIFY2(!zones.contains(QLatin1String(rejected)), rejected);

    for (const QString &id : zones)
    {
        if (id == QLatin1String("UTC"))
            continue;
        bool ok = false;
        for (const QString &prefix : allowed)
            ok = ok || id.startsWith(prefix);
        QVERIFY2(ok, qPrintable(id));
    }
}

void TstTimezoneHelper::listOnlyHoldsValidZones()
{
    for (const QString &id : TimezoneHelper::filteredIanaTimezones())
        QVERIFY2(QTimeZone(id.toUtf8()).isValid(), qPrintable(id));
}

void TstTimezoneHelper::listIsStableAcrossCalls()
{
    const QStringList first = TimezoneHelper::filteredIanaTimezones();
    QCOMPARE(TimezoneHelper::filteredIanaTimezones(), first);
    QCOMPARE(WidgetModel::availableTimezones(), first);
}

void TstTimezoneHelper::editorExposesTheList()
{
    Room *room = new Room("lounge", "lounge");
    ListeRoom::Instance().Add(room);

    Params p;
    p.Add("name", "rui");
    p.Add("device_type", "calaos_rui_4inch");
    p.Add("provisioning_code", "ABC123");
    p.Add("visible", "false");
    p.Add("type", "RemoteUI");
    p.Add("screensaver_clock_timezone", "CET-1CEST,M3.5.0,M10.5.0/3");
    IOBase *io = ListeRoom::Instance().createOutput(p, room);
    QVERIFY(io);

    // A QML load error opens a modal box: close it rather than hang the suite.
    bool modalSeen = false;
    QTimer::singleShot(0, this, [&modalSeen]()
    {
        if (QWidget *w = QApplication::activeModalWidget())
        {
            modalSeen = true;
            w->close();
        }
    });

    DialogRemoteUIEditor editor(io);
    QCoreApplication::processEvents();
    QVERIFY(!modalSeen);

    QQuickWidget *quick = editor.findChild<QQuickWidget *>();
    QVERIFY(quick);
    QCOMPARE(quick->rootContext()->contextProperty("availableTimezones").toStringList(),
             TimezoneHelper::filteredIanaTimezones());
}

void TstTimezoneHelper::legacyUtcOffsetsMapToAnOffsetPreservingZone_data()
{
    QTest::addColumn<QString>("stored");
    QTest::addColumn<int>("offsetSecs");
    QTest::newRow("UTC+1") << "UTC+1" << 3600;
    QTest::newRow("UTC-5") << "UTC-5" << -5 * 3600;
    QTest::newRow("UTC+14") << "UTC+14" << 14 * 3600;
    QTest::newRow("UTC-12") << "UTC-12" << -12 * 3600;
    QTest::newRow("UTC+5:30") << "UTC+5:30" << 5 * 3600 + 1800;
    QTest::newRow("UTC-9:30") << "UTC-9:30" << -(9 * 3600 + 1800);
}

void TstTimezoneHelper::legacyUtcOffsetsMapToAnOffsetPreservingZone()
{
    QFETCH(QString, stored);
    QFETCH(int, offsetSecs);

    const QString iana = TimezoneHelper::posixTzToIana(TimezoneHelper::convertStoredTimezone(stored));
    QVERIFY2(iana != QLatin1String("UTC"), qPrintable(iana));

    QTimeZone tz(iana.toUtf8());
    QVERIFY2(tz.isValid(), qPrintable(iana));
    const int year = QDate::currentDate().year();
    QCOMPARE(tz.offsetFromUtc(QDateTime(QDate(year, 1, 15), QTime(12, 0), QTimeZone::UTC)), offsetSecs);
    QCOMPARE(tz.offsetFromUtc(QDateTime(QDate(year, 7, 15), QTime(12, 0), QTimeZone::UTC)), offsetSecs);
}

void TstTimezoneHelper::legacyUtcResaveWritesValidPosix()
{
    // "UTC+1" must end up as a POSIX string one hour east of UTC, not "UTC0"
    const QString iana = TimezoneHelper::posixTzToIana(TimezoneHelper::convertStoredTimezone("UTC+1"));
    const QString posix = TimezoneHelper::ianaToPosixTz(iana);
    QVERIFY2(posix.endsWith("-1"), qPrintable(posix));
    QVERIFY2(!posix.contains(','), qPrintable(posix));

    QCOMPARE(TimezoneHelper::posixTzToIana(TimezoneHelper::convertStoredTimezone("UTC")), QString("UTC"));
    // Zones sharing a rule collapse to the first one, but the POSIX string is stable
    const QString paris = TimezoneHelper::ianaToPosixTz("Europe/Paris");
    QCOMPARE(TimezoneHelper::ianaToPosixTz(TimezoneHelper::posixTzToIana(paris)), paris);
}

void TstTimezoneHelper::unknownValuesFallBackToUtc()
{
    QCOMPARE(TimezoneHelper::posixTzToIana("garbage"), QString("UTC"));
    QCOMPARE(TimezoneHelper::posixTzToIana("UTC+x"), QString("UTC"));
    QCOMPARE(TimezoneHelper::posixTzToIana(""), QString("UTC"));
}

void TstTimezoneHelper::zonelessOffsetsAreKept()
{
    for (const QString &posix : {"UTC-15", "UTC13", "UTC-4:15", "UTC-13:30"})
    {
        const QString id = TimezoneHelper::posixTzToIana(posix);
        QCOMPARE(TimezoneHelper::ianaToPosixTz(id), posix);
    }
    QCOMPARE(TimezoneHelper::convertStoredTimezone("UTC+15"), QString("UTC-15"));
    QCOMPARE(TimezoneHelper::ianaToPosixTz(
                 TimezoneHelper::posixTzToIana(TimezoneHelper::convertStoredTimezone("UTC+15"))),
             QString("UTC-15"));
}

void TstTimezoneHelper::zeroOffsetIsUtc()
{
    QCOMPARE(TimezoneHelper::posixTzToIana("UTC+0"), QString("UTC"));
    QCOMPARE(TimezoneHelper::posixTzToIana("UTC-0"), QString("UTC"));
    QCOMPARE(TimezoneHelper::posixTzToIana("UTC"), QString("UTC"));
}

void TstTimezoneHelper::etcGmtFallback()
{
    // UTC-12: no listed zone, only Etc/GMT+12
    const QString id = TimezoneHelper::posixTzToIana("UTC12");
    QVERIFY2(!TimezoneHelper::filteredIanaTimezones().contains(id), qPrintable(id));
    QCOMPARE(id, QString("Etc/GMT+12"));
    QVERIFY(!TimezoneHelper::ianaToPosixTz(id).isEmpty());
    QVERIFY(TimezoneHelper::ianaToPosixTz(id) != QString("UTC0"));
}

void TstTimezoneHelper::cleanup()
{
    ListeRoom::Instance().clear();
}

QTEST_MAIN(TstTimezoneHelper)
#include "tst_timezonehelper.moc"
