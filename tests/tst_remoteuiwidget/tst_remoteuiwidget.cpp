#include <QtTest>
#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>

#include "../tst_devicetemplates/fixture.h"
#include "RemoteUIModel.h"

using namespace Calaos;
using namespace Fixture;

namespace
{

// The house of the template tests, with a panel in the living room. Its token and
// secret are the ones the panel simulator signs with (FIX-RUI-2).
QByteArray houseWithPanel(const QString &pages = QString())
{
    QByteArray xml = houseXml();
    const QByteArray panel = QStringLiteral(
        R"(<calaos:remote_ui id="panel" type="RemoteUI" name="Panel" grid_w="3" grid_h="3" )"
        R"(auth_token="sim-token" device_secret="sim-secret">%1</calaos:remote_ui>
)").arg(pages).toUtf8();
    xml.insert(xml.indexOf("</calaos:room>"), panel);
    return xml;
}

QString pagesWith(const QString &widgets)
{
    return QStringLiteral(R"(<calaos:pages><calaos:page name="Page" type="default">%1</calaos:page></calaos:pages>)")
        .arg(widgets);
}

// What the editor shows and stores when an IO is linked to a widget
// (DialogRemoteUIEditor::openIOSelector and validateIO).
QString linkedWidgetType(const QString &ioId)
{
    IOBase *io = ListeRoom::Instance().get_input(ioId.toStdString());
    if (!io)
        io = ListeRoom::Instance().get_output(ioId.toStdString());
    return io ? WidgetModel::widgetTypeForGuiType(u(io->get_gui_type())) : QString();
}

Device *newDevice(const QString &tpl)
{
    const QJsonObject t = shippedTemplate(tpl);
    QVariantMap values = filled(t);
    values["room"] = QVariant::fromValue(roomNamed("Salon"));
    QStringList notes;
    return DeviceTemplates::create(t, values, &notes);
}

// {io_id: type} of the <calaos:widget> elements of a saved io.xml.
QMap<QString, QString> writtenWidgets(const QByteArray &xml)
{
    QMap<QString, QString> res;
    QXmlStreamReader r(xml);
    while (!r.atEnd())
    {
        r.readNext();
        if (r.isStartElement() && r.name() == QLatin1String("widget"))
            res.insert(r.attributes().value("io_id").toString(), r.attributes().value("type").toString());
    }
    return res;
}

}

class tst_RemoteUIWidget : public QObject
{
    Q_OBJECT

    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;

    QString work(const QString &name) const { return m_workDir.filePath(name); }

private slots:
    void initTestCase();
    void cleanup();

    void everyClimateTemplateGetsTheClimateWidget_data();
    void everyClimateTemplateGetsTheClimateWidget();
    void theFamilyIsMatchedWhateverItsCase();
    void theEditorWritesTheTypeThePanelDraws();
    void aWidgetSavedAsGenericIsRetypedWhenTheEditorOpens();
};

void tst_RemoteUIWidget::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_remoteuiwidget");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
}

void tst_RemoteUIWidget::cleanup()
{
    resetModel();
}

void tst_RemoteUIWidget::everyClimateTemplateGetsTheClimateWidget_data()
{
    QTest::addColumn<QString>("tpl");
    for (const QJsonValue &t: shippedTemplates())
        QTest::newRow(qPrintable(t.toObject()["id"].toString())) << t.toObject()["id"].toString();
}

void tst_RemoteUIWidget::everyClimateTemplateGetsTheClimateWidget()
{
    QFETCH(QString, tpl);
    QVERIFY(loadIo(work("io.xml"), houseWithPanel()));
    Device *dev = newDevice(tpl);
    QVERIFY(dev);
    QCOMPARE(u(dev->family()), QString("climate"));

    // The firmware registers "Climate"; any other name is drawn as "Unsupported".
    QCOMPARE(linkedWidgetType(u(dev->get_param("id"))), QString("Climate"));
}

void tst_RemoteUIWidget::theFamilyIsMatchedWhateverItsCase()
{
    QCOMPARE(WidgetModel::widgetTypeForGuiType("climate"), QString("Climate"));
    QCOMPARE(WidgetModel::widgetTypeForGuiType("Climate"), QString("Climate"));
    QCOMPARE(WidgetModel::widgetTypeForGuiType("LIGHT"), QString("LightSwitch"));
    QCOMPARE(WidgetModel::widgetTypeForGuiType("no_such_type"), QString("Generic"));
}

void tst_RemoteUIWidget::theEditorWritesTheTypeThePanelDraws()
{
    QVERIFY(loadIo(work("io.xml"), houseWithPanel()));
    Device *dev = newDevice("esphome_climate");
    QVERIFY(dev);
    const QString devId = u(dev->get_param("id"));
    IOBase *panel = ListeRoom::Instance().get_output("panel");
    QVERIFY(panel);

    RemoteUIModel model;
    QVERIFY(model.loadFromXml(panel->getRemoteUIPagesXml()));
    model.currentPage()->createWidget(devId, linkedWidgetType(devId), 0, 0, 3, 2);
    model.currentPage()->createWidget("relay_heat", linkedWidgetType("relay_heat"), 0, 2, 1, 1);
    panel->setRemoteUIPagesXml(model.toXml());

    const QByteArray saved = saveIo(work("saved.xml"));

    // For a load by the real server and the panel simulator (see FIX-RUI-2).
    const QString dump = qEnvironmentVariable("CALAOS_RUI_XML_DIR");
    if (!dump.isEmpty())
        QVERIFY(writeFile(QDir(dump).filePath("io.xml"), saved));

    const QMap<QString, QString> widgets = writtenWidgets(saved);
    QCOMPARE(widgets.value(devId), QString("Climate"));
    QCOMPARE(widgets.value("relay_heat"), QString("LightSwitch"));
}

void tst_RemoteUIWidget::aWidgetSavedAsGenericIsRetypedWhenTheEditorOpens()
{
    QVERIFY(loadIo(work("io.xml"), houseWithPanel(pagesWith(
        R"(<calaos:widget io_id="other" type="Generic" x="0" y="0" w="3" h="2"/>)"
        R"(<calaos:widget io_id="text_out" type="Generic" x="0" y="2" w="1" h="1"/>)"
        R"(<calaos:widget io_id="gone" type="Generic" x="1" y="2" w="1" h="1"/>)"
        R"(<calaos:widget io_id="relay_heat" type="Scenario" x="2" y="2" w="1" h="1"/>)"))));
    IOBase *panel = ListeRoom::Instance().get_output("panel");
    QVERIFY(panel);

    RemoteUIModel model;
    QVERIFY(model.loadFromXml(panel->getRemoteUIPagesXml()));
    panel->setRemoteUIPagesXml(model.toXml());

    const QMap<QString, QString> widgets = writtenWidgets(saveIo(work("saved.xml")));
    QCOMPARE(widgets.value("other"), QString("Climate"));
    // Nothing the panel draws for a string output or a missing IO: left as found.
    QCOMPARE(widgets.value("text_out"), QString("Generic"));
    QCOMPARE(widgets.value("gone"), QString("Generic"));
    // A type that is not the fallback is never overridden.
    QCOMPARE(widgets.value("relay_heat"), QString("Scenario"));
    QVERIFY(!model.isModified());
}

QTEST_MAIN(tst_RemoteUIWidget)
#include "tst_remoteuiwidget.moc"
