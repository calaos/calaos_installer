#include <QtTest>
#include <QApplication>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>

#include "dialognewanalog.h"
#include "formanalogproperties.h"
#include "projectmanager.h"

using namespace Calaos;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

QByteArray ioSource(const QString &inner)
{
    return ("<?xml version=\"1.0\"?>\n"
            "<calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">\n<calaos:home>\n"
            "<calaos:room name=\"Salon\" type=\"livingroom\" hits=\"0\">\n" + inner +
            "\n</calaos:room>\n</calaos:home>\n</calaos:ioconfig>\n").toUtf8();
}

}

class tst_AnalogBounds: public QObject
{
    Q_OBJECT

    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;

    QString roundTrip(const QByteArray &xml)
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
        QString src = m_workDir.filePath("src.xml");
        QString dst = m_workDir.filePath("dst.xml");
        QFile f(src);
        if (!f.open(QIODevice::WriteOnly))
            return QString();
        f.write(xml);
        f.close();
        if (!ProjectManager::loadIOsFromFile(src) || !ProjectManager::saveIOsToFile(dst))
            return QString();
        QFile d(dst);
        if (!d.open(QIODevice::ReadOnly))
            return QString();
        return QString::fromUtf8(d.readAll());
    }

private slots:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
        QCoreApplication::setOrganizationName("calaos-installer-tests");
        QCoreApplication::setApplicationName("tst_analogbounds");
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
        QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());
    }

    void legacyBoundsAreDroppedAtLoad_data()
    {
        QTest::addColumn<QString>("kind");
        QTest::addColumn<QString>("type");
        for (const char *t: { "WOAnalog", "WagoOutputAnalog", "WIAnalog", "WITemp", "WebOutputAnalog",
                              "WebInputAnalog", "MqttOutputAnalog", "MqttInputAnalog", "MqttInputTemp",
                              "KNXOutputAnalog", "OWTemp" })
            QTest::newRow(t) << (QString(t).contains("Output") || QString(t) == "WOAnalog" ? "output" : "input") << t;
        QTest::newRow("InternalInt") << "internal" << "InternalInt";
    }

    void legacyBoundsAreDroppedAtLoad()
    {
        QFETCH(QString, kind);
        QFETCH(QString, type);
        QString out = roundTrip(ioSource(QString("<calaos:%1 id=\"x\" type=\"%2\" name=\"x\" min=\"-10000\" max=\"10000\" unit=\"C\"/>")
                                         .arg(kind, type)));
        QVERIFY(out.contains("id=\"x\""));
        QVERIFY(out.contains("unit=\"C\""));
        QVERIFY2(!out.contains("min="), qPrintable(out));
        QVERIFY2(!out.contains("max="), qPrintable(out));
    }

    void minutesOfTimeInputsAreKept()
    {
        QString out = roundTrip(ioSource("<calaos:input id=\"t\" type=\"InputTimer\" name=\"t\" hour=\"0\" min=\"5\" sec=\"0\" msec=\"0\"/>"));
        QVERIFY2(out.contains("min=\"5\""), qPrintable(out));
    }

    void valueBoundsRoundTrip()
    {
        QString out = roundTrip(ioSource("<calaos:output id=\"a\" type=\"MqttOutputAnalog\" name=\"a\" value_min=\"16\" value_max=\"30.5\" min=\"-10000\"/>"
                                         "<calaos:internal id=\"i\" type=\"InternalInt\" name=\"i\" value_max=\"-3\"/>"));
        QVERIFY2(out.contains("value_min=\"16\""), qPrintable(out));
        QVERIFY2(out.contains("value_max=\"30.5\""), qPrintable(out));
        QVERIFY2(out.contains("value_max=\"-3\""), qPrintable(out));
        QVERIFY2(!out.contains(" min="), qPrintable(out));
        QCOMPARE(out.count("value_min"), 1);
        QCOMPARE(roundTrip(out.toUtf8()), out);
    }

    void mqttOutputStringRoundTrip()
    {
        QString out = roundTrip(ioSource("<calaos:output id=\"s\" type=\"MqttOutputString\" name=\"mode\" topic_pub=\"a/set\" "
                                         "topic_sub=\"a/state\" data=\"{{ value }}\" path=\"mode\"/>"));
        QVERIFY2(out.contains("type=\"MqttOutputString\""), qPrintable(out));
        QVERIFY2(out.contains("topic_pub=\"a/set\""), qPrintable(out));
        QVERIFY2(out.contains("path=\"mode\""), qPrintable(out));
        IOBase *io = ListeRoom::Instance().get_output("s");
        QVERIFY(io);
        // A real type, not the "unknown" fallback IO.
        QCOMPARE(u(io->get_gui_type()), QString("string_out"));
        QCOMPARE(roundTrip(out.toUtf8()), out);
    }

    void formWritesBoundsOnlyWhenSet()
    {
        FormAnalogProperties w(nullptr, true);
        Params p;
        w.addBoundsTo(p);
        QVERIFY(!p.Exists("value_min") && !p.Exists("value_max"));
        QVERIFY(!p.Exists("min") && !p.Exists("max"));

        w.findChild<QLineEdit *>("edit_value_min")->setText(" 16 ");
        w.addBoundsTo(p);
        QCOMPARE(u(p["value_min"]), QString("16"));
        QVERIFY(!p.Exists("value_max"));

        w.findChild<QLineEdit *>("edit_value_max")->setText("30.5");
        w.addBoundsTo(p);
        QCOMPARE(u(p["value_max"]), QString("30.5"));
        QVERIFY(!p.Exists("min") && !p.Exists("max"));
    }

    void formHidesBoundsForInputs()
    {
        FormAnalogProperties w(nullptr, false);
        w.show();
        QVERIFY(!w.findChild<QLineEdit *>("edit_value_min")->isVisible());
        w.updateUI(true);
        QVERIFY(w.findChild<QLineEdit *>("edit_value_min")->isVisible());
        QVERIFY(w.findChild<QLineEdit *>("edit_value_max")->isVisible());
    }

    void newAnalogDialogWritesNoLegacyBounds()
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
        Room *room = new Room("Salon", "livingroom", 0);
        ListeRoom::Instance().Add(room);

        DialogNewAnalog dlg(room);
        dlg.findChild<QLineEdit *>("edit_name")->setText("target");
        dlg.findChild<QRadioButton *>("radio_output")->setChecked(true);
        dlg.findChild<QLineEdit *>("edit_value_min")->setText("5");
        dlg.findChild<QDialogButtonBox *>("buttonBox")->button(QDialogButtonBox::Ok)->click();

        QCOMPARE(room->get_size_out(), 1);
        IOBase *io = room->get_output(0);
        QCOMPARE(u(io->get_param("value_min")), QString("5"));
        QVERIFY(!io->get_params().Exists("value_max"));
        QVERIFY(!io->get_params().Exists("min"));
        QVERIFY(!io->get_params().Exists("max"));
    }
};

QTEST_MAIN(tst_AnalogBounds)
#include "tst_analogbounds.moc"
