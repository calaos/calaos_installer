#include <QtTest>
#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTreeWidget>

#include "WidgetIOProperties.h"

namespace
{

Params wodali(const QMap<QString, QString> &extra)
{
    Params p;
    p.Add("type", "WODali");
    p.Add("name", "lamp");
    for (auto it = extra.begin();it != extra.end();it++)
        p.Add(it.key().toStdString(), it.value().toStdString());
    return p;
}

// Column 2 is the editor and column 1 the revert button, on the row of the label.
QWidget *cellOf(WidgetIOProperties &w, const QString &prop, int column)
{
    for (QLabel *l : w.findChildren<QLabel *>())
    {
        if (l->text() != prop)
            continue;
        for (QGridLayout *g : w.findChildren<QGridLayout *>())
        {
            int idx = g->indexOf(l);
            if (idx < 0)
                continue;
            int row, col, rs, cs;
            g->getItemPosition(idx, &row, &col, &rs, &cs);
            QLayoutItem *item = g->itemAtPosition(row, column);
            return item ? item->widget() : nullptr;
        }
    }
    return nullptr;
}

template<class T> T *editor(WidgetIOProperties &w, const QString &prop)
{
    return qobject_cast<T *>(cellOf(w, prop, 2));
}

QPushButton *revertButton(WidgetIOProperties &w, const QString &prop)
{
    return qobject_cast<QPushButton *>(cellOf(w, prop, 1));
}

QString stored(WidgetIOProperties &w, const char *key)
{
    return QString::fromStdString(w.getParams()[key]);
}


// The entry helper's button sits on the first parameter in name order: "gateway".
Params sensorParams()
{
    Params p;
    p.Add("type", "MySensorsFixture");
    p.Add("name", "before");
    p.Add("node_id", "1");
    p.Add("sensor_id", "1");
    p.Add("port", "5000");
    p.Add("gateway", "stored-gateway");
    return p;
}

// Accepts the entry helper's dialog with one sensor picked and the given port typed.
void runEntryHelper(WidgetIOProperties &w, const QString &sensorId, const QString &port)
{
    QPushButton *helper = qobject_cast<QPushButton *>(cellOf(w, "gateway", 4));
    QDialog *dialog = w.findChild<QDialog *>();
    QTreeWidget *tree = dialog ? dialog->findChild<QTreeWidget *>("treeMySensors") : nullptr;
    QLineEdit *portEdit = dialog ? dialog->findChild<QLineEdit *>("lineEditPort") : nullptr;
    QVERIFY(helper && tree && portEdit);

    portEdit->setText(port);
    QTreeWidgetItem *item = new QTreeWidgetItem(tree, QStringList{"12", sensorId, "S_TEMP", "garage"});
    tree->setCurrentItem(item);

    QTimer::singleShot(0, dialog, &QDialog::accept);
    QTest::mouseClick(helper, Qt::LeftButton);
}

}

class tst_WidgetIOProperties : public QObject
{
    Q_OBJECT

    QTemporaryDir m_settingsDir;

private slots:
    void initTestCase();

    void inRangeValueKeepsTheSchemaBounds();
    void valueOnTheBoundIsNotFlagged();

    void outOfRangeValueIsDisplayedAsStored();
    void outOfRangeValueSurvivesAnUntouchedDialog();
    void outOfRangeValueSurvivesAGestureOnAnotherField();
    void outOfRangeValueIsFlagged();
    void outOfRangeValueCanStillBeEdited();
    void revertOnAnOutOfRangeFieldRestoresTheStoredValue();
    void revertAfterEditingBackInsideTheRangeRestoresTheStoredValue();
    void belowMinimumValueSurvivesAnUntouchedDialog();
    void outOfRangeFloatSurvivesAGestureOnAnotherField();

    void floatKeepsThreeDecimalsWhenWidened();
    void nonFiniteFloatIsNeverWidenedTo_data();
    void nonFiniteFloatIsNeverWidenedTo();
    void tooltipDoesNotShowTheSentinelBounds();

    void absentParameterKeepsTheSchemaBounds();
    void unparsableValueKeepsTheSchemaBounds();

    void entryHelperFillsAFloatEditor();
    void entryHelperLeavesAReadOnlyEditorAlone();
    void entryHelperLeavesANonEditableDialogAlone();
    void entryHelperValueInsideTheRangeKeepsTheBounds();
    void entryHelperOutOfRangeValueIsWidenedAndFlagged();
    void entryHelperOutOfRangeValueCanBeReverted();
};

void tst_WidgetIOProperties::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_widgetioproperties");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());
}

void tst_WidgetIOProperties::inRangeValueKeepsTheSchemaBounds()
{
    WidgetIOProperties w(wodali({{"address", "200"}}));
    QSpinBox *s = editor<QSpinBox>(w, "address");
    QVERIFY(s);
    QCOMPARE(s->value(), 200);
    QCOMPARE(s->minimum(), 1);
    QCOMPARE(s->maximum(), 355);

    s->setValue(1000);
    QCOMPARE(s->value(), 355);
}

void tst_WidgetIOProperties::valueOnTheBoundIsNotFlagged()
{
    WidgetIOProperties w(wodali({{"address", "355"}}));
    QSpinBox *s = editor<QSpinBox>(w, "address");
    QVERIFY(s);
    QCOMPARE(s->maximum(), 355);
    QVERIFY(s->toolTip().isEmpty());
}

void tst_WidgetIOProperties::outOfRangeValueIsDisplayedAsStored()
{
    WidgetIOProperties w(wodali({{"address", "612"}}));
    QSpinBox *s = editor<QSpinBox>(w, "address");
    QVERIFY(s);
    QCOMPARE(s->value(), 612);
}

void tst_WidgetIOProperties::outOfRangeValueSurvivesAnUntouchedDialog()
{
    WidgetIOProperties w(wodali({{"address", "612"}}));
    QCOMPARE(stored(w, "address"), QString("612"));
}

void tst_WidgetIOProperties::outOfRangeValueSurvivesAGestureOnAnotherField()
{
    WidgetIOProperties w(wodali({{"address", "612"}, {"group", "0"}}));
    QSpinBox *group = editor<QSpinBox>(w, "group");
    QVERIFY(group);
    group->setValue(1);

    QCOMPARE(stored(w, "group"), QString("1"));
    QCOMPARE(stored(w, "address"), QString("612"));
}

void tst_WidgetIOProperties::outOfRangeValueIsFlagged()
{
    WidgetIOProperties w(wodali({{"address", "612"}}));
    QSpinBox *s = editor<QSpinBox>(w, "address");
    QVERIFY(s);
    QVERIFY2(s->toolTip().contains("612") && s->toolTip().contains("355"), qPrintable(s->toolTip()));
}

void tst_WidgetIOProperties::outOfRangeValueCanStillBeEdited()
{
    WidgetIOProperties w(wodali({{"address", "612"}}));
    QSpinBox *s = editor<QSpinBox>(w, "address");
    QVERIFY(s);
    s->setValue(300);
    QCOMPARE(stored(w, "address"), QString("300"));
}

void tst_WidgetIOProperties::revertOnAnOutOfRangeFieldRestoresTheStoredValue()
{
    WidgetIOProperties w(wodali({{"address", "612"}}));
    QSpinBox *s = editor<QSpinBox>(w, "address");
    QPushButton *revert = revertButton(w, "address");
    QVERIFY(s && revert);

    s->setValue(612 - 1);
    QCOMPARE(stored(w, "address"), QString("611"));
    revert->click();

    QCOMPARE(s->value(), 612);
    QCOMPARE(stored(w, "address"), QString("612"));
}

void tst_WidgetIOProperties::revertAfterEditingBackInsideTheRangeRestoresTheStoredValue()
{
    WidgetIOProperties w(wodali({{"address", "612"}}));
    QSpinBox *s = editor<QSpinBox>(w, "address");
    QPushButton *revert = revertButton(w, "address");
    QVERIFY(s && revert);

    s->setValue(300);
    revert->click();

    QCOMPARE(stored(w, "address"), QString("612"));
    QCOMPARE(s->value(), 612);
}

void tst_WidgetIOProperties::belowMinimumValueSurvivesAnUntouchedDialog()
{
    WidgetIOProperties w(wodali({{"fade_time", "0"}, {"group", "0"}}));
    QSpinBox *s = editor<QSpinBox>(w, "fade_time");
    QVERIFY(s);
    QCOMPARE(s->value(), 0);

    editor<QSpinBox>(w, "group")->setValue(1);
    QCOMPARE(stored(w, "fade_time"), QString("0"));
}

void tst_WidgetIOProperties::outOfRangeFloatSurvivesAGestureOnAnotherField()
{
    WidgetIOProperties w(wodali({{"ratio", "20.5"}, {"group", "0"}}));
    QDoubleSpinBox *s = editor<QDoubleSpinBox>(w, "ratio");
    QVERIFY(s);
    QCOMPARE(s->value(), 20.5);

    editor<QSpinBox>(w, "group")->setValue(1);
    QCOMPARE(stored(w, "ratio"), QString("20.5"));

    s->setValue(5.0);
    revertButton(w, "ratio")->click();
    QCOMPARE(s->value(), 20.5);
    QCOMPARE(stored(w, "ratio"), QString("20.5"));
}

void tst_WidgetIOProperties::floatKeepsThreeDecimalsWhenWidened()
{
    WidgetIOProperties w(wodali({{"ratio", "10.004"}, {"offset", "3.333"}}));
    QDoubleSpinBox *s = editor<QDoubleSpinBox>(w, "ratio");
    QVERIFY(s);
    QCOMPARE(s->value(), 10.004);
    QCOMPARE(s->maximum(), 10.004);
    QVERIFY(s->toolTip().contains("10.004"));

    QDoubleSpinBox *o = editor<QDoubleSpinBox>(w, "offset");
    QVERIFY(o);
    QCOMPARE(o->value(), 3.333);
}

void tst_WidgetIOProperties::nonFiniteFloatIsNeverWidenedTo_data()
{
    QTest::addColumn<QString>("value");
    QTest::newRow("nan") << "nan";
    QTest::newRow("inf") << "inf";
    QTest::newRow("-inf") << "-inf";
}

void tst_WidgetIOProperties::nonFiniteFloatIsNeverWidenedTo()
{
    QFETCH(QString, value);
    WidgetIOProperties w(wodali({{"ratio", value}}));
    QDoubleSpinBox *s = editor<QDoubleSpinBox>(w, "ratio");
    QVERIFY(s);
    QCOMPARE(s->minimum(), 0.0);
    QCOMPARE(s->maximum(), 10.0);
    QVERIFY(s->toolTip().isEmpty());
    QCOMPARE(stored(w, "ratio"), value);
}

void tst_WidgetIOProperties::tooltipDoesNotShowTheSentinelBounds()
{
    WidgetIOProperties w(wodali({{"level", "150"}, {"offset", "1000000000000"}}));

    QSpinBox *level = editor<QSpinBox>(w, "level");
    QVERIFY(level);
    QVERIFY2(level->toolTip().contains("150") && level->toolTip().contains("100"), qPrintable(level->toolTip()));
    QVERIFY2(!level->toolTip().contains("999999999"), qPrintable(level->toolTip()));

    QDoubleSpinBox *offset = editor<QDoubleSpinBox>(w, "offset");
    QVERIFY(offset);
    QVERIFY(!offset->toolTip().isEmpty());
    QVERIFY2(!offset->toolTip().contains("999999999") && !offset->toolTip().contains("e+"), qPrintable(offset->toolTip()));
}

void tst_WidgetIOProperties::absentParameterKeepsTheSchemaBounds()
{
    // The default is not a stored value: it is not widened to, and not written.
    WidgetIOProperties w(wodali({}));
    QSpinBox *s = editor<QSpinBox>(w, "fade_time");
    QVERIFY(s);
    QCOMPARE(s->minimum(), 1);
    QVERIFY(!w.getParams().Exists("fade_time"));
}

void tst_WidgetIOProperties::unparsableValueKeepsTheSchemaBounds()
{
    WidgetIOProperties w(wodali({{"address", "abc"}}));
    QSpinBox *s = editor<QSpinBox>(w, "address");
    QVERIFY(s);
    QCOMPARE(s->minimum(), 1);
    QCOMPARE(s->maximum(), 355);
    QCOMPARE(stored(w, "address"), QString("abc"));
}

// The entry helper's values must reach a QDoubleSpinBox through its own pointer:
// the UiObject's QSpinBox one is not set for that editor kind.
void tst_WidgetIOProperties::entryHelperFillsAFloatEditor()
{
    Params p;
    p.Add("type", "MySensorsFixture");
    p.Add("name", "before");
    p.Add("node_id", "1");
    WidgetIOProperties w(p);

    QDoubleSpinBox *node = editor<QDoubleSpinBox>(w, "node_id");
    QLineEdit *name = editor<QLineEdit>(w, "name");
    QPushButton *helper = qobject_cast<QPushButton *>(cellOf(w, "gateway", 4));
    QDialog *dialog = w.findChild<QDialog *>();
    QVERIFY(node && name && helper && dialog);

    QTreeWidget *tree = dialog->findChild<QTreeWidget *>("treeMySensors");
    QVERIFY(tree);
    QTreeWidgetItem *item = new QTreeWidgetItem(tree, QStringList{"12.5", "3", "S_TEMP", "garage"});
    tree->setCurrentItem(item);

    QTimer::singleShot(0, dialog, &QDialog::accept);
    QTest::mouseClick(helper, Qt::LeftButton);

    QCOMPARE(node->value(), 12.5);
    QCOMPARE(name->text(), QString("garage"));
    QCOMPARE(stored(w, "node_id"), QString("12.5"));
}

void tst_WidgetIOProperties::entryHelperLeavesAReadOnlyEditorAlone()
{
    WidgetIOProperties w(sensorParams());
    QLineEdit *gateway = editor<QLineEdit>(w, "gateway");
    QLineEdit *name = editor<QLineEdit>(w, "name");
    QVERIFY(gateway && name);
    QVERIFY(!gateway->isEnabled());

    runEntryHelper(w, "3", "5000");

    QCOMPARE(gateway->text(), QString("stored-gateway"));
    QCOMPARE(stored(w, "gateway"), QString("stored-gateway"));
    QCOMPARE(name->text(), QString("garage"));
}

void tst_WidgetIOProperties::entryHelperLeavesANonEditableDialogAlone()
{
    WidgetIOProperties w(sensorParams(), false);
    QLineEdit *name = editor<QLineEdit>(w, "name");
    QSpinBox *sensor = editor<QSpinBox>(w, "sensor_id");
    QDoubleSpinBox *node = editor<QDoubleSpinBox>(w, "node_id");
    QVERIFY(name && sensor && node);
    QVERIFY(!name->isEnabled());

    runEntryHelper(w, "300", "70000");

    QCOMPARE(name->text(), QString("before"));
    QCOMPARE(sensor->value(), 1);
    QCOMPARE(sensor->maximum(), 254);
    QCOMPARE(node->value(), 1.0);
    QCOMPARE(stored(w, "name"), QString("before"));
    QCOMPARE(stored(w, "sensor_id"), QString("1"));
}

void tst_WidgetIOProperties::entryHelperValueInsideTheRangeKeepsTheBounds()
{
    WidgetIOProperties w(sensorParams());
    runEntryHelper(w, "3", "8080");

    QSpinBox *sensor = editor<QSpinBox>(w, "sensor_id");
    QDoubleSpinBox *port = editor<QDoubleSpinBox>(w, "port");
    QVERIFY(sensor && port);
    QCOMPARE(sensor->value(), 3);
    QCOMPARE(sensor->maximum(), 254);
    QVERIFY(sensor->toolTip().isEmpty());
    QCOMPARE(port->value(), 8080.0);
    QCOMPARE(port->maximum(), 65535.0);
    QVERIFY(port->toolTip().isEmpty());
}

// Like a value read from io.xml, a helper value outside the documented range is kept
// as is, not clamped to a bound the user never typed.
void tst_WidgetIOProperties::entryHelperOutOfRangeValueIsWidenedAndFlagged()
{
    WidgetIOProperties w(sensorParams());
    runEntryHelper(w, "300", "70000");

    QSpinBox *sensor = editor<QSpinBox>(w, "sensor_id");
    QDoubleSpinBox *port = editor<QDoubleSpinBox>(w, "port");
    QVERIFY(sensor && port);

    QCOMPARE(sensor->value(), 300);
    QCOMPARE(stored(w, "sensor_id"), QString("300"));
    QVERIFY2(sensor->toolTip().contains("300") && sensor->toolTip().contains("254"), qPrintable(sensor->toolTip()));

    QCOMPARE(port->value(), 70000.0);
    QCOMPARE(stored(w, "port"), QString("70000"));
    QVERIFY2(port->toolTip().contains("70000") && port->toolTip().contains("65535"), qPrintable(port->toolTip()));
}

void tst_WidgetIOProperties::entryHelperOutOfRangeValueCanBeReverted()
{
    WidgetIOProperties w(sensorParams());
    runEntryHelper(w, "300", "5000");

    QSpinBox *sensor = editor<QSpinBox>(w, "sensor_id");
    QPushButton *revert = revertButton(w, "sensor_id");
    QVERIFY(sensor && revert);
    QCOMPARE(sensor->value(), 300);

    revert->click();

    QCOMPARE(sensor->value(), 1);
    QCOMPARE(stored(w, "sensor_id"), QString("1"));
}

QTEST_MAIN(tst_WidgetIOProperties)
#include "tst_widgetioproperties.moc"
