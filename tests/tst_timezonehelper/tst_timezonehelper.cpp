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

void TstTimezoneHelper::cleanup()
{
    ListeRoom::Instance().clear();
}

QTEST_MAIN(TstTimezoneHelper)
#include "tst_timezonehelper.moc"
