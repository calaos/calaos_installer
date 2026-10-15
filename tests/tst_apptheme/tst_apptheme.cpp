#include <QtTest>
#include <QApplication>
#include <QDir>
#include <QGuiApplication>
#include <QStyleHints>

#include "AppTheme.h"
#include "DialogRemoteUIEditor.h"
#include "ListeRoom.h"

using namespace Calaos;

namespace
{

bool isLight(const QColor &c) { return c.lightness() > 160; }
bool isDark(const QColor &c) { return c.lightness() < 96; }

// The offscreen platform ignores setColorScheme(); the GTK platform theme is the
// only way to start dark without a desktop. Local runs without the plugin skip
// instead of failing; CI sets CALAOS_REQUIRE_DARK_SYSTEM.
bool systemIsDark()
{
    return isDark(QGuiApplication::palette().color(QPalette::Window));
}

void requireDarkSystem()
{
    if (systemIsDark())
        return;
    if (qEnvironmentVariableIsSet("CALAOS_REQUIRE_DARK_SYSTEM"))
        QFAIL("the GTK platform theme did not give a dark system palette");
    QSKIP("GTK platform theme plugin unavailable, cannot simulate a dark system");
}

}

class TestAppTheme: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanup();
    void lightPaletteIsLight();
    void paletteStaysLightUnderDarkScheme();
    void widgetsStayLightUnderDarkScheme();
    void remoteUiEditorShots();
};

void TestAppTheme::initTestCase()
{
    // Same style as main.cpp: Fusion is what turns the scheme into a palette.
    QVERIFY(QApplication::setStyle("fusion"));
}

void TestAppTheme::cleanup()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QGuiApplication::styleHints()->unsetColorScheme();
#endif
}

void TestAppTheme::lightPaletteIsLight()
{
    const QPalette p = AppTheme::lightPalette();
    QVERIFY(isLight(p.color(QPalette::Window)));
    QVERIFY(isDark(p.color(QPalette::WindowText)));
    QVERIFY(isLight(p.color(QPalette::Base)));
    QVERIFY(isDark(p.color(QPalette::Text)));
    QVERIFY(isLight(p.color(QPalette::Button)));
    QVERIFY(isDark(p.color(QPalette::ButtonText)));
}

void TestAppTheme::paletteStaysLightUnderDarkScheme()
{
    // Witness: the system really is dark, otherwise the test proves nothing.
    requireDarkSystem();

    AppTheme::forceLight();

    const QPalette p = QGuiApplication::palette();
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QCOMPARE(QGuiApplication::styleHints()->colorScheme(), Qt::ColorScheme::Light);
#endif
    QVERIFY(isLight(p.color(QPalette::Window)));
    QVERIFY(isDark(p.color(QPalette::WindowText)));
    QVERIFY(isLight(p.color(QPalette::Base)));
    QVERIFY(isDark(p.color(QPalette::Text)));
}

void TestAppTheme::widgetsStayLightUnderDarkScheme()
{
    requireDarkSystem();
    AppTheme::forceLight();

    QWidget w;
    w.ensurePolished();
    QVERIFY(isLight(w.palette().color(QPalette::Window)));
    QVERIFY(isDark(w.palette().color(QPalette::WindowText)));
}

void TestAppTheme::remoteUiEditorShots()
{
    const QString dir = qEnvironmentVariable("CALAOS_SHOTS_DIR");
    if (dir.isEmpty())
        QSKIP("CALAOS_SHOTS_DIR not set");
    requireDarkSystem();
    QDir().mkpath(dir);

    Params p;
    p.Add("name", "Tablet");
    p.Add("type", "WODigital");
    Room *room = new Room("Salon", "salon");
    ListeRoom::Instance().Add(room);
    IOBase *io = ListeRoom::Instance().createOutput(p, room);
    QVERIFY(io);

    // One scheme per process: a dialog created before forceLight() keeps stale button
    // palettes, which the real application never has. CALAOS_SHOTS_FIX=1 applies the fix.
    const bool fixed = qEnvironmentVariableIntValue("CALAOS_SHOTS_FIX") == 1;
    if (fixed)
        AppTheme::forceLight();

    DialogRemoteUIEditor dlg(io);
    dlg.resize(1280, 800);
    dlg.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dlg));
    QTest::qWait(500);
    const QString name = fixed ? "remoteui_dark_system_after.png" : "remoteui_dark_system_before.png";
    QVERIFY(dlg.grab().save(dir + "/" + name));
}

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORMTHEME", "gtk3");
    qputenv("GTK_THEME", "Adwaita:dark");
    QApplication app(argc, argv);
    TestAppTheme t;
    return QTest::qExec(&t, argc, argv);
}

#include "tst_apptheme.moc"
