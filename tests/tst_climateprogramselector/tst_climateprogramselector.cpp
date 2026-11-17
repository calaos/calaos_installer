#include <QtTest>
#include <QApplication>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTreeWidget>

#include "ClearLineEdit.h"
#include "ClimateProgram.h"
#include "DialogIOSelector.h"
#include "projectmanager.h"

using namespace Calaos;

namespace
{

const QString kSep = QString::fromUtf8(" › ");

// Two climates with the same roles in two rooms, plus IOs that are not
// program IOs: the label must come from the owning climate, never from the
// room or from the io name.
const char *kHome =
    "<?xml version=\"1.0\"?>\n"
    "<calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">\n<calaos:home>\n"
    "<calaos:room name=\"Living\" type=\"salon\" hits=\"1\">\n"
    "  <calaos:input id=\"temp\" type=\"WITemp\" name=\"Probe\" host=\"h\" port=\"502\" var=\"0\"/>\n"
    "  <calaos:internal id=\"free\" type=\"InternalBool\" name=\"Free switch\" visible=\"false\"/>\n"
    "  <calaos:device id=\"clim\" type=\"climate\" impl=\"thermostat\" name=\"Salon\">\n"
    "    <calaos:param name=\"heat_levels\" value=\"1\"/>\n"
    "    <calaos:param name=\"cool_levels\" value=\"0\"/>\n"
    "    <calaos:input id=\"clim_heat_level1\" type=\"InPlageHoraire\" name=\"raw heat 1\" visible=\"false\" months=\"111111111111\"/>\n"
    "    <calaos:internal id=\"clim_heat_level1_setpoint\" type=\"InternalInt\" name=\"raw sp 1\" save=\"true\" value=\"19\" visible=\"false\"/>\n"
    "    <calaos:internal id=\"clim_program_enabled\" type=\"InternalBool\" name=\"raw enabled\" save=\"true\" value=\"true\" visible=\"false\"/>\n"
    "    <calaos:internal id=\"clim_target\" type=\"InternalInt\" name=\"target\" visible=\"false\"/>\n"
    "    <calaos:link name=\"current_temp\" io=\"temp\"/>\n"
    "    <calaos:link name=\"target_temp\" io=\"clim_target\"/>\n"
    "    <calaos:link name=\"heat_level1\" io=\"clim_heat_level1\"/>\n"
    "    <calaos:link name=\"heat_level1_setpoint\" io=\"clim_heat_level1_setpoint\"/>\n"
    "    <calaos:link name=\"program_enabled\" io=\"clim_program_enabled\"/>\n"
    "  </calaos:device>\n"
    "</calaos:room>\n"
    "<calaos:room name=\"Sleep\" type=\"chambre\" hits=\"1\">\n"
    "  <calaos:input id=\"temp2\" type=\"WITemp\" name=\"Probe 2\" host=\"h\" port=\"502\" var=\"0\"/>\n"
    "  <calaos:device id=\"clim2\" type=\"climate\" impl=\"proxy\" name=\"Chambre\">\n"
    "    <calaos:param name=\"heat_levels\" value=\"1\"/>\n"
    "    <calaos:param name=\"cool_levels\" value=\"0\"/>\n"
    "    <calaos:input id=\"clim2_heat_level1\" type=\"InPlageHoraire\" name=\"raw heat 1\" visible=\"false\" months=\"111111111111\"/>\n"
    "    <calaos:internal id=\"clim2_program_enabled\" type=\"InternalBool\" name=\"raw enabled\" save=\"true\" value=\"true\" visible=\"false\"/>\n"
    "    <calaos:link name=\"current_temp\" io=\"temp2\"/>\n"
    "    <calaos:link name=\"heat_level1\" io=\"clim2_heat_level1\"/>\n"
    "    <calaos:link name=\"program_enabled\" io=\"clim2_program_enabled\"/>\n"
    "  </calaos:device>\n"
    "</calaos:room>\n"
    "</calaos:home>\n</calaos:ioconfig>\n";

QTreeWidget *treeOf(DialogIOSelector &d)
{
    return d.findChild<QTreeWidget *>();
}

// An internal IO is listed twice (input and output): the id gives the first row.
// Item of an IO by the id shown in the third column.
QTreeWidgetItem *itemById(DialogIOSelector &d, const QString &id)
{
    QTreeWidget *t = treeOf(d);
    for (int i = 0;i < t->topLevelItemCount();i++)
        for (int j = 0;j < t->topLevelItem(i)->childCount();j++)
            if (t->topLevelItem(i)->child(j)->text(2) == id)
                return t->topLevelItem(i)->child(j);
    return nullptr;
}

QStringList visibleIds(DialogIOSelector &d)
{
    QStringList ids;
    QTreeWidget *t = treeOf(d);
    for (int i = 0;i < t->topLevelItemCount();i++)
    {
        QTreeWidgetItem *room = t->topLevelItem(i);
        for (int j = 0;j < room->childCount();j++)
            if (!room->child(j)->isHidden() && !room->isHidden())
                ids << room->child(j)->text(2);
    }
    ids.sort();
    ids.removeDuplicates();
    return ids;
}

}

class tst_ClimateProgramSelector: public QObject
{
    Q_OBJECT

    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;

    bool load()
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
        QString p = m_workDir.filePath("io.xml");
        QFile f(p);
        if (!f.open(QIODevice::WriteOnly))
            return false;
        f.write(kHome);
        f.close();
        return ProjectManager::loadIOsFromFile(p);
    }

private slots:
    void initTestCase()
    {
        QVERIFY(m_settingsDir.isValid());
        QVERIFY(m_workDir.isValid());
        QStandardPaths::setTestModeEnabled(true);
        QCoreApplication::setOrganizationName("calaos-installer-tests");
        QCoreApplication::setApplicationName("tst_climateprogramselector");
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
        QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());
    }
    void init() { QVERIFY(load()); }
    void cleanup()
    {
        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();
    }

    void programIosAreLabelledWithTheirClimate();
    void plainIosKeepTheirName();
    void sameRoleOnTwoClimatesGivesTwoLabels();
    void labelIsTheSameAsTheRuleLabel();
    void filterFindsProgramIosByClimateName();
    void filterIsCaseInsensitiveAndTrimmed();
    void filterOnTheRoleNarrowsToOneIo();
    void preselectionFindsAProgramIo();
    void selectingAProgramIoReturnsItsIoAndRoom();
    void roomItemsCannotBeChosen();
    void unknownPreselectionSelectsNothing();
};

void tst_ClimateProgramSelector::programIosAreLabelledWithTheirClimate()
{
    DialogIOSelector d;
    QCOMPARE(itemById(d, "clim_program_enabled")->text(0), "Salon" + kSep + "Program enabled");
    QCOMPARE(itemById(d, "clim_heat_level1")->text(0), "Salon" + kSep + "Heating level 1 active");
    QCOMPARE(itemById(d, "clim_heat_level1_setpoint")->text(0), "Salon" + kSep + "Heating level 1 setpoint");
    // The raw io name never shows once the io is a program role.
    QVERIFY(!itemById(d, "clim_heat_level1")->text(0).contains("raw"));
}

void tst_ClimateProgramSelector::plainIosKeepTheirName()
{
    DialogIOSelector d;
    QCOMPARE(itemById(d, "free")->text(0), QString("Free switch"));
    QCOMPARE(itemById(d, "temp")->text(0), QString("Probe"));
    // An io of the climate that is not a program role is not prefixed.
    QCOMPARE(itemById(d, "clim_target")->text(0), QString("target"));
}

void tst_ClimateProgramSelector::sameRoleOnTwoClimatesGivesTwoLabels()
{
    DialogIOSelector d;
    const QString a = itemById(d, "clim_program_enabled")->text(0);
    const QString b = itemById(d, "clim2_program_enabled")->text(0);
    QCOMPARE(b, "Chambre" + kSep + "Program enabled");
    QVERIFY(a != b);
    QCOMPARE(itemById(d, "clim2_heat_level1")->text(0), "Chambre" + kSep + "Heating level 1 active");
}

void tst_ClimateProgramSelector::labelIsTheSameAsTheRuleLabel()
{
    // The picker and the rule forms must not drift apart.
    DialogIOSelector d;
    QTreeWidget *t = treeOf(d);
    int checked = 0;
    for (int i = 0;i < t->topLevelItemCount();i++)
    {
        QTreeWidgetItem *room = t->topLevelItem(i);
        for (int j = 0;j < room->childCount();j++)
        {
            const std::string id = room->child(j)->text(2).toStdString();
            IOBase *io = ListeRoom::Instance().get_input(id);
            if (!io)
                io = ListeRoom::Instance().get_output(id);
            QVERIFY2(io, id.c_str());
            QCOMPARE(room->child(j)->text(0), ClimateProgram::ruleLabel(io));
            checked++;
        }
    }
    QCOMPARE(checked, 18);
}

void tst_ClimateProgramSelector::filterFindsProgramIosByClimateName()
{
    DialogIOSelector d;
    auto *edit = d.findChild<ClearLineEdit *>();
    QVERIFY(edit);

    edit->setText("chambre");
    QCOMPARE(visibleIds(d), QStringList({ "clim2", "clim2_heat_level1", "clim2_program_enabled" }));

    edit->setText("salon");
    // Room "Living" has type salon, but only names are matched: the climate
    // name selects the device and its program IOs, not the other IOs of the room.
    QCOMPARE(visibleIds(d), QStringList({ "clim", "clim_heat_level1", "clim_heat_level1_setpoint", "clim_program_enabled" }));

    edit->setText(QString());
    QCOMPARE(visibleIds(d).size(), 11);
}

void tst_ClimateProgramSelector::filterIsCaseInsensitiveAndTrimmed()
{
    DialogIOSelector d;
    auto *edit = d.findChild<ClearLineEdit *>();
    edit->setText("  CHAMBRE\t");
    QCOMPARE(visibleIds(d), QStringList({ "clim2", "clim2_heat_level1", "clim2_program_enabled" }));

    // Only blanks: everything is back.
    edit->setText("   ");
    QCOMPARE(visibleIds(d).size(), 11);
}

void tst_ClimateProgramSelector::filterOnTheRoleNarrowsToOneIo()
{
    DialogIOSelector d;
    auto *edit = d.findChild<ClearLineEdit *>();
    edit->setText(QString::fromUtf8("salon › program"));
    QCOMPARE(visibleIds(d), QStringList({ "clim_program_enabled" }));

    // Nothing matches: every room is hidden.
    edit->setText("no such io");
    QVERIFY(visibleIds(d).isEmpty());
    for (int i = 0;i < treeOf(d)->topLevelItemCount();i++)
        QVERIFY(treeOf(d)->topLevelItem(i)->isHidden());

    // A regex-looking or markup filter is plain text.
    edit->setText("(.*)<b>");
    QVERIFY(visibleIds(d).isEmpty());
}

void tst_ClimateProgramSelector::preselectionFindsAProgramIo()
{
    DialogIOSelector d("clim2_program_enabled");
    QVERIFY(d.getSelectedIO());
    QCOMPARE(QString::fromStdString(d.getSelectedIO()->get_param("id")), QString("clim2_program_enabled"));
    QCOMPARE(QString::fromStdString(d.getSelectedRoom()->get_name()), QString("Sleep"));
    QVERIFY(d.findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok)->isEnabled());
}

void tst_ClimateProgramSelector::selectingAProgramIoReturnsItsIoAndRoom()
{
    DialogIOSelector d;
    QVERIFY(!d.getSelectedIO());
    QVERIFY(!d.findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok)->isEnabled());

    // Every row, input or output side, resolves to its own IO and room.
    int rows = 0;
    QTreeWidget *t = treeOf(d);
    for (int i = 0;i < t->topLevelItemCount();i++)
        for (int j = 0;j < t->topLevelItem(i)->childCount();j++)
        {
            QTreeWidgetItem *item = t->topLevelItem(i)->child(j);
            t->setCurrentItem(item);
            const QString id = item->text(2);
            QVERIFY2(d.getSelectedIO(), qPrintable(id));
            QCOMPARE(QString::fromStdString(d.getSelectedIO()->get_param("id")), id);
            QCOMPARE(QString::fromStdString(d.getSelectedRoom()->get_name()), t->topLevelItem(i)->text(0));
            rows++;
        }
    QCOMPARE(rows, 18);
}

void tst_ClimateProgramSelector::roomItemsCannotBeChosen()
{
    DialogIOSelector d("clim_heat_level1");
    QVERIFY(d.getSelectedIO());

    treeOf(d)->setCurrentItem(treeOf(d)->topLevelItem(0));
    treeOf(d)->topLevelItem(0)->setSelected(true);
    QVERIFY(!d.getSelectedIO());
    QVERIFY(!d.findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok)->isEnabled());
}

void tst_ClimateProgramSelector::unknownPreselectionSelectsNothing()
{
    for (const QString &id: { QString("nope"), QString("Salon" + kSep + "Program enabled"), QString("CLIM_PROGRAM_ENABLED") })
    {
        DialogIOSelector d(id);
        QVERIFY2(!d.getSelectedIO(), qPrintable(id));
    }
}

QTEST_MAIN(tst_ClimateProgramSelector)
#include "tst_climateprogramselector.moc"
