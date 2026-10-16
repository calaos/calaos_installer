#include <QtTest>
#include <QApplication>
#include <QMenu>
#include <memory>

#include "FormConditionStd.h"
#include "RuleActionMenu.h"
#include "formrules.h"
#include <ListeRoom.h>
#include <ListeRule.h>

// The form only asks FormRules to refresh the tree item, which this suite
// does not build.
void FormRules::updateItemCondition(QTreeWidgetItem *, Condition *) {}

using namespace Calaos;

class tst_ConditionForm: public QObject
{
    Q_OBJECT

private:
    // Commands proposed by the form for an IO of the given display type.
    static QStringList proposals(const std::string &guiType, int condType)
    {
        Params p = { { "id", "io" }, { "name", "io" }, { "type", "Test" } };
        std::unique_ptr<IOBase> io(new IOBase(p, guiType, TSTRING, IOBase::IO_BOTH));
        Params rp = { { "name", "r" }, { "type", "standard" } };
        Rule rule(rp);
        Condition cond(condType);
        if (condType == COND_STD)
            cond.Add(io.get());
        else
            cond.setOutput(io.get());

        FormConditionStd form;
        form.setCondition(nullptr, &rule, &cond);

        QStringList cmds;
        for (QMenu *menu: form.findChildren<QMenu *>())
        {
            for (QAction *a: menu->actions())
            {
                if (auto *ra = dynamic_cast<RuleActionMenu *>(a))
                    cmds << ra->getCommand();
            }
        }
        return cmds;
    }

private slots:
    void audioPlayerEventsAreProposed_data()
    {
        QTest::addColumn<QString>("guiType");
        QTest::addColumn<int>("condType");
        for (const char *t: { "audio_player", "audio" })
        {
            QTest::newRow(qPrintable(QString("%1 standard").arg(t))) << t << int(COND_STD);
            QTest::newRow(qPrintable(QString("%1 output").arg(t))) << t << int(COND_OUTPUT);
        }
    }

    void audioPlayerEventsAreProposed()
    {
        QFETCH(QString, guiType);
        QFETCH(int, condType);

        const QStringList cmds = proposals(guiType.toStdString(), condType);
        for (const char *ev: { "onplay", "onpause", "onstop", "onsongchange",
                               "onplaylistchange", "onvolumechange", "onerror" })
            QVERIFY2(cmds.contains(ev), ev);
    }

    void otherTypesDoNotGetThePlayerEvents()
    {
        QVERIFY(!proposals("var_string", COND_STD).contains("onplay"));
    }
};

QTEST_MAIN(tst_ConditionForm)
#include "tst_conditionform.moc"
