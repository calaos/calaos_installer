#include <QtTest>
#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>

#include "dialogioproperties.h"

namespace
{

// The editor of a parameter sits next to its title label, in column 2 of the same grid row.
QWidget *editorOf(QWidget *root, const QString &param)
{
    for (QGridLayout *g: root->findChildren<QGridLayout *>())
    {
        for (int i = 0;i < g->count();i++)
        {
            QLabel *l = qobject_cast<QLabel *>(g->itemAt(i)->widget());
            if (!l || l->text() != param)
                continue;
            int row, col, rs, cs;
            g->getItemPosition(i, &row, &col, &rs, &cs);
            QLayoutItem *item = g->itemAtPosition(row, 2);
            return item ? item->widget() : nullptr;
        }
    }
    return nullptr;
}

QString val(Params p, const char *k)
{
    return QString::fromUtf8(p[k].c_str());
}

}

class tst_IoDocCatalogue: public QObject
{
    Q_OBJECT

    QTemporaryDir m_settingsDir;

private slots:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
        QCoreApplication::setOrganizationName("calaos-installer-tests");
        QCoreApplication::setApplicationName("tst_iodoccatalogue");
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
        QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());
    }

    void mqttOutputStringShowsItsParameters()
    {
        Params p;
        p.Add("type", "MqttOutputString");
        p.Add("name", "mode");
        DialogIOProperties d(p);
        for (const char *name: { "topic_pub", "topic_sub", "data", "path" })
            QVERIFY2(qobject_cast<QLineEdit *>(editorOf(&d, name)), name);
    }

    void mqttOutputStringEditedValuesAreReturned()
    {
        Params p;
        p.Add("type", "MqttOutputString");
        p.Add("name", "mode");
        DialogIOProperties d(p);
        qobject_cast<QLineEdit *>(editorOf(&d, "topic_pub"))->setText("a/set");
        qobject_cast<QLineEdit *>(editorOf(&d, "data"))->setText("{{ value }}");
        QCOMPARE(val(d.getParams(), "topic_pub"), QString("a/set"));
        QCOMPARE(val(d.getParams(), "data"), QString("{{ value }}"));
    }

    void boundsAreOptionalFieldsOnBoundedTypes_data()
    {
        QTest::addColumn<QString>("type");
        for (const char *t: { "InternalInt", "MqttOutputAnalog", "KNXOutputAnalog", "WOAnalog", "WebOutputAnalog" })
            QTest::newRow(t) << t;
    }

    void boundsAreOptionalFieldsOnBoundedTypes()
    {
        QFETCH(QString, type);
        Params p;
        p.Add("type", type.toUtf8().constData());
        p.Add("name", "x");
        DialogIOProperties d(p);
        QLineEdit *lo = qobject_cast<QLineEdit *>(editorOf(&d, "value_min"));
        QLineEdit *hi = qobject_cast<QLineEdit *>(editorOf(&d, "value_max"));
        QVERIFY(lo && hi);
        QVERIFY(lo->text().isEmpty() && hi->text().isEmpty());
        // Untouched: nothing is invented.
        QVERIFY(!d.getParams().Exists("value_min"));
        QVERIFY(!d.getParams().Exists("value_max"));
    }

    void internalIntBoundsSetClearAndRoundTrip()
    {
        Params p;
        p.Add("type", "InternalInt");
        p.Add("name", "x");
        p.Add("value_min", "-5");
        p.Add("value_max", "40.5");

        DialogIOProperties d(p);
        QLineEdit *lo = qobject_cast<QLineEdit *>(editorOf(&d, "value_min"));
        QLineEdit *hi = qobject_cast<QLineEdit *>(editorOf(&d, "value_max"));
        QCOMPARE(lo->text(), QString("-5"));
        QCOMPARE(hi->text(), QString("40.5"));
        // Opened and closed untouched: byte for byte what was stored.
        QCOMPARE(val(d.getParams(), "value_min"), QString("-5"));
        QCOMPARE(val(d.getParams(), "value_max"), QString("40.5"));

        lo->setText("10");
        QCOMPARE(val(d.getParams(), "value_min"), QString("10"));

        lo->clear();
        QVERIFY(!d.getParams().Exists("value_min"));
        QCOMPARE(val(d.getParams(), "value_max"), QString("40.5"));

        hi->clear();
        QVERIFY(!d.getParams().Exists("value_max"));
        QCOMPARE(val(d.getParams(), "name"), QString("x"));

        // Set again after a clear, then reopen from what the dialog returned.
        hi->setText("7");
        Params out = d.getParams();
        DialogIOProperties d2(out);
        QCOMPARE(qobject_cast<QLineEdit *>(editorOf(&d2, "value_max"))->text(), QString("7"));
        QVERIFY(!d2.getParams().Exists("value_min"));
    }

    void settingABoundOnAnAbsentParamAndRevertingRemovesIt()
    {
        Params p;
        p.Add("type", "InternalInt");
        p.Add("name", "x");
        DialogIOProperties d(p);
        QLineEdit *lo = qobject_cast<QLineEdit *>(editorOf(&d, "value_min"));
        lo->setText("3");
        QVERIFY(d.getParams().Exists("value_min"));
        lo->setText("");
        QVERIFY(!d.getParams().Exists("value_min"));
    }

    void boundFieldRejectsNonNumbers()
    {
        Params p;
        p.Add("type", "InternalInt");
        p.Add("name", "x");
        DialogIOProperties d(p);
        QLineEdit *lo = qobject_cast<QLineEdit *>(editorOf(&d, "value_min"));
        lo->setText("1,000");
        QVERIFY(!lo->hasAcceptableInput());
        lo->setText("-");
        QVERIFY(!lo->hasAcceptableInput());
        lo->setText("-1.5");
        QVERIFY(lo->hasAcceptableInput());
    }

    void shippedCatalogueDropsTheOldDaliBound()
    {
        Params p;
        p.Add("type", "WODali");
        p.Add("name", "x");
        p.Add("address", "500");
        DialogIOProperties d(p);
        QSpinBox *sb = qobject_cast<QSpinBox *>(editorOf(&d, "address"));
        QVERIFY(sb);
        // Stored value past the 355 bound is displayed and kept (T3.247 guard).
        QCOMPARE(sb->value(), 500);
        QCOMPARE(val(d.getParams(), "address"), QString("500"));
    }
};

QTEST_MAIN(tst_IoDocCatalogue)
#include "tst_iodoccatalogue.moc"
