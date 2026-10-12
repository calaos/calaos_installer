#include <QtTest>
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTimer>
#include <QXmlStreamReader>

#include <functional>

#include "projectmanager.h"

using namespace Calaos;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

QString paramsToString(Params &p)
{
    QStringList l;
    for (int i = 0;i < p.size();i++)
    {
        string k, v;
        p.get_item(i, k, v);
        l << u(k) + "=" + u(v);
    }
    return l.join(';');
}

QString rangesToString(const char *day, vector<TimeRange> &v)
{
    QStringList l;
    for (TimeRange &t : v)
        l << QString("%1:%2:%3(%4,%5)-%6:%7:%8(%9,%10)")
               .arg(u(t.shour), u(t.smin), u(t.ssec)).arg(t.start_type).arg(t.start_offset)
               .arg(u(t.ehour), u(t.emin), u(t.esec)).arg(t.end_type).arg(t.end_offset);
    return QString("%1[%2]").arg(day, l.join(','));
}

QString ioToString(IOBase *io)
{
    QString s = QString("gui=%1 in=%2 out=%3 inout=%4 {%5}")
                    .arg(u(io->get_gui_type()))
                    .arg(io->is_input()).arg(io->is_output()).arg(io->is_inout())
                    .arg(paramsToString(io->get_params()));
    if (io->get_gui_type() == "time_range")
    {
        s += " months=" + QString::fromStdString(io->range_months.to_string());
        s += " " + rangesToString("mon", io->range_monday);
        s += " " + rangesToString("tue", io->range_tuesday);
        s += " " + rangesToString("wed", io->range_wednesday);
        s += " " + rangesToString("thu", io->range_thursday);
        s += " " + rangesToString("fri", io->range_friday);
        s += " " + rangesToString("sat", io->range_saturday);
        s += " " + rangesToString("sun", io->range_sunday);
    }
    if (!io->getRemoteUIPagesXml().isEmpty())
        s += " pages=" + io->getRemoteUIPagesXml();
    return s;
}

// Everything the model keeps about the rooms, in a form QCOMPARE can diff line by line.
QStringList snapshotRooms()
{
    QStringList out;
    ListeRoom &lr = ListeRoom::Instance();
    for (int i = 0;i < lr.size();i++)
    {
        Room *r = lr.get_room(i);
        out << QString("room name=%1 type=%2 hits=%3").arg(u(r->get_name()), u(r->get_type())).arg(r->get_hits());
        for (int j = 0;j < r->get_size_in();j++)
            out << "  in  " + ioToString(r->get_input(j));
        for (int j = 0;j < r->get_size_out();j++)
            out << "  out " + ioToString(r->get_output(j));
    }
    return out;
}

QString idOf(IOBase *io)
{
    return io? u(io->get_param("id")) : QString("<null>");
}

QStringList snapshotRules()
{
    QStringList out;
    ListeRule &lr = ListeRule::Instance();
    for (int i = 0;i < lr.size();i++)
    {
        Rule *r = lr.get_rule(i);
        out << "rule " + paramsToString(r->ruleParams);

        for (int c = 0;c < r->get_size_conds();c++)
        {
            Condition *cd = r->get_condition(c);
            QString s = QString("  cond type=%1 trigger=%2").arg(cd->getType()).arg(cd->isTrigger());
            switch (cd->getType())
            {
            case COND_STD:
                for (int k = 0;k < cd->get_size();k++)
                {
                    string id = cd->get_input(k)->get_param("id");
                    s += QString(" [id=%1 oper=%2 val=%3 var=%4 gui=%5]")
                             .arg(u(id), u(cd->get_operator()[id]), u(cd->get_params()[id]),
                                  u(cd->get_params_var()[id]), u(cd->get_input(k)->get_gui_type()));
                }
                break;
            case COND_OUTPUT:
                s += QString(" [id=%1 oper=%2 val=%3 var=%4]")
                         .arg(idOf(cd->getOutput()), u(cd->getOutputOper()), u(cd->getOutputParam()),
                              u(cd->getOutputParamVar()));
                break;
            case COND_SCRIPT:
                for (int k = 0;k < cd->getScriptInputSize();k++)
                    s += " in=" + idOf(cd->getScriptInput(k));
                s += " script=<" + u(cd->getScript()) + ">";
                break;
            }
            out << s;
        }

        for (int a = 0;a < r->get_size_actions();a++)
        {
            Action *ac = r->get_action(a);
            QString s = QString("  action type=%1").arg(ac->getType());
            switch (ac->getType())
            {
            case ACTION_STD:
                for (int k = 0;k < ac->get_size();k++)
                {
                    string id = ac->get_output(k)->get_param("id");
                    s += QString(" [id=%1 val=%2 var=%3]")
                             .arg(u(id), u(ac->get_params()[id]), u(ac->get_params_var()[id]));
                }
                break;
            case ACTION_MAIL:
                s += QString(" sender=%1 rcpt=%2 subj=%3 att=%4 msg=<%5>")
                         .arg(u(ac->getMailSender()), u(ac->getMailRecipients()), u(ac->getMailSubject()),
                              u(ac->getMailAttachment()), u(ac->getMailMessage()));
                break;
            case ACTION_SCRIPT:
                s += " script=<" + u(ac->getScript()) + ">";
                break;
            case ACTION_TOUCHSCREEN:
                s += QString(" action=%1 cam=%2").arg(u(ac->getTouchscreenAction()), u(ac->getTouchscreenCamera()));
                break;
            case ACTION_PUSH:
                s += QString(" att=%1 msg=<%2>").arg(u(ac->getMailAttachment()), u(ac->getMailMessage()));
                break;
            }
            out << s;
        }
    }
    return out;
}

void resetModel()
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();
}

// Reports, from inside the room destructor, how many rooms the list still holds.
struct ProbeIO : public IOBase
{
    std::function<void()> onDestroy;

    ProbeIO(Params &p, int ioType, std::function<void()> cb):
        IOBase(p, "probe", TBOOL, ioType),
        onDestroy(std::move(cb))
    { }
    ~ProbeIO() override { onDestroy(); }
};

QByteArray readAll(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return QByteArray();
    return f.readAll();
}

bool writeAll(const QString &path, const QByteArray &data)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
        return false;
    f.write(data);
    return true;
}

struct XNode
{
    QString path;
    QMap<QString, QString> attrs;
    QString text;
};

QVector<XNode> parseNodes(const QByteArray &xml)
{
    QVector<XNode> nodes;
    QVector<int> open;
    QStringList path;
    QXmlStreamReader r(xml);
    while (!r.atEnd())
    {
        r.readNext();
        if (r.isStartElement())
        {
            path << r.name().toString();
            XNode n;
            n.path = path.join('/');
            for (const QXmlStreamAttribute &a : r.attributes())
                n.attrs.insert(a.name().toString(), a.value().toString());
            nodes << n;
            open << nodes.size() - 1;
        }
        else if (r.isEndElement())
        {
            path.removeLast();
            open.removeLast();
        }
        else if (r.isCharacters() && !r.isWhitespace() && !open.isEmpty())
            nodes[open.last()].text += r.text().toString();
    }
    return nodes;
}

// Attributes the installer adds to a file on save. Every entry is a default
// the model fills in (ListeRoom::create*, TimeRange) or an explicit spelling
// of what the reader assumes when the attribute is absent.
const QSet<QString> &knownAdditions()
{
    static const QSet<QString> s = {
        "port=502", "months=111111111111",
        "start_type=0", "start_offset=1", "end_type=0", "end_offset=1",
        "type=standard", "trigger=true", "time_down=28", "time_up=30",
    };
    return s;
}

// Empty when `written` is `source` plus knownAdditions() and nothing else.
QStringList unexpectedChanges(const QByteArray &source, const QByteArray &written)
{
    QStringList diffs;
    QVector<XNode> a = parseNodes(source);
    QVector<XNode> b = parseNodes(written);
    if (a.size() != b.size())
        diffs << QString("element count %1 -> %2").arg(a.size()).arg(b.size());
    for (int i = 0;i < qMin(a.size(), b.size());i++)
    {
        if (a[i].path != b[i].path)
        {
            diffs << QString("#%1 element %2 -> %3").arg(i).arg(a[i].path, b[i].path);
            break;
        }
        if (a[i].text != b[i].text)
            diffs << QString("#%1 %2 text changed").arg(i).arg(a[i].path);
        for (auto it = a[i].attrs.begin();it != a[i].attrs.end();++it)
            if (b[i].attrs.value(it.key(), "<missing>") != it.value())
                diffs << QString("#%1 %2 %3: %4 -> %5").arg(i).arg(a[i].path, it.key(), it.value(),
                                                             b[i].attrs.value(it.key(), "<missing>"));
        for (auto it = b[i].attrs.begin();it != b[i].attrs.end();++it)
            if (!a[i].attrs.contains(it.key()) && !knownAdditions().contains(it.key() + "=" + it.value()))
                diffs << QString("#%1 %2 gained %3=%4").arg(i).arg(a[i].path, it.key(), it.value());
    }
    return diffs;
}

QByteArray ioSource(const QString &inner)
{
    return ("<?xml version=\"1.0\"?>\n"
            "<calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">\n<calaos:home>\n" +
            inner + "\n</calaos:home>\n</calaos:ioconfig>\n").toUtf8();
}

QString ioDoc(const QString &inner)
{
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<calaos:ioconfig xmlns:calaos=\"http://www.calaos.fr\">\n"
           "    <calaos:home>\n" + inner +
           "    </calaos:home>\n</calaos:ioconfig>\n";
}

QByteArray rulesSource(const QString &inner)
{
    return ("<?xml version=\"1.0\"?>\n<calaos:rules xmlns:calaos=\"http://www.calaos.fr\">\n" +
            inner + "\n</calaos:rules>\n").toUtf8();
}

QString rulesDoc(const QString &inner)
{
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<calaos:rules xmlns:calaos=\"http://www.calaos.fr\">\n" + inner +
           "</calaos:rules>\n";
}

}

class tst_ProjectManager: public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_settingsDir;
    QTemporaryDir m_workDir;
    QStringList m_dialogs; // texts of the QMessageBox the code under test tried to show
    QStringList m_expectedDialogs;
    QStringList m_otherModals; // class names of modals that are not a QMessageBox
    QTimer m_dialogKiller;

    QString sample(const QString &name, const QString &file) const
    {
        return QString(CALAOS_SAMPLES_DIR) + "/" + name + "/" + file;
    }

    QString work(const QString &file) const { return m_workDir.filePath(file); }

    bool loadIOs(const QString &path)
    {
        QString p = path;
        return ProjectManager::loadIOsFromFile(p);
    }
    bool loadRules(const QString &path)
    {
        QString p = path;
        return ProjectManager::loadRulesFromFile(p);
    }
    QByteArray saveIOs(const QString &file)
    {
        QString p = work(file);
        if (!ProjectManager::saveIOsToFile(p))
            return QByteArray();
        return readAll(p);
    }
    QByteArray saveRules(const QString &file)
    {
        QString p = work(file);
        if (!ProjectManager::saveRulesToFile(p))
            return QByteArray();
        return readAll(p);
    }

    // Loads `xml` through the real reader and returns what the real writer
    // makes of it. Starts from an empty model.
    QString ioThroughModel(const QByteArray &xml)
    {
        writeAll(work("src.xml"), xml);
        resetModel();
        loadIOs(work("src.xml"));
        return QString::fromUtf8(saveIOs("dst.xml"));
    }

    // io.xml first (rules resolve their ids against it), then rules.xml.
    QString rulesThroughModel(const QByteArray &io, const QByteArray &rules)
    {
        writeAll(work("src_io.xml"), io);
        writeAll(work("src_rules.xml"), rules);
        resetModel();
        loadIOs(work("src_io.xml"));
        loadRules(work("src_rules.xml"));
        return QString::fromUtf8(saveRules("dst_rules.xml"));
    }

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void ioRoundTrip_data();
    void ioRoundTrip();
    void ioSecondWriteIsIdentical_data();
    void ioSecondWriteIsIdentical();
    void ioWriteOnlyAddsKnownDefaults_data();
    void ioWriteOnlyAddsKnownDefaults();

    void rulesRoundTrip_data();
    void rulesRoundTrip();
    void rulesSecondWriteIsIdentical_data();
    void rulesSecondWriteIsIdentical();
    void rulesWriteOnlyAddsKnownDefaults_data();
    void rulesWriteOnlyAddsKnownDefaults();

    void ioUnknownRoomChildIsKeptWithItsSubtree();
    void ioUnknownLeafRoomChildIsKept();
    void ioUnknownInputChildIsKept();
    void ioUnknownHomeChildIsKept();
    void ioUnknownChildrenOfEveryIoKindAreKept();
    void ioUnknownTextCdataAndNamespacesAreKept();
    void ioUnknownElementsInTimeRangesAreSkippedWhole();
    void ioAllIoKindsRoundTrip();
    void ioFactoryDefaultsAreWritten();
    void ioUnknownTypeIsKeptVerbatim();
    void ioInputsAreWrittenBeforeOutputs();
    void ioTimeRangeWithoutMonthsStaysActiveAllYear();
    void ioTimeRangeEmptyMonthsDisablesAllMonths();
    void ioTimeRangeExplicitMonthsRoundTrip();
    void ioTimeRangeMonthsAndPlages();
    void ioRemoteUiPages();
    void ioDuplicateIdsAreKeptAndReported();
    void ioRoomHitsDefaultToZero();
    void ioIosWithoutIdGetFreshIdsWithoutModal();
    void ioEmptyIdIsKeptAndNeverReportedAsDuplicate();
    void listeRoomEmptyIdNeverMatches();
    void ioLoadFailsOnUnknownRoot();
    void ioLoadFailsOnTruncatedFile();
    void ioLoadFailsOnMissingFile();

    void rulesStandardEntriesWithSeveralIosAreSplit();
    void rulesEveryKindRoundTrips();
    void rulesUnsupportedEntriesAreDropped();
    void rulesMissingIOsSurvive();
    void rulesEmptyIdIsNotBoundToAnyIO();
    void rulesLegacyIidIsRewrittenToId();
    void rulesLoadFailsOnMalformedFile();

    void clearNeverLeavesADestroyedRoomInTheList();

    void deletingAnIOLeavesNoRuleOnIt_data();
    void deletingAnIOLeavesNoRuleOnIt();
    void deletingAnIOKeepsTheOtherRules();
    void savingAfterDeletingAnIOReadsNoFreedMemory_data() { deletingAnIOLeavesNoRuleOnIt_data(); }
    void savingAfterDeletingAnIOReadsNoFreedMemory();
    void deletingAScriptTriggerKeepsTheRuleAndItsLua_data();
    void deletingAScriptTriggerKeepsTheRuleAndItsLua();
    void anIOUsedAsAVariableKeepsItsIdReserved();
};

void tst_ProjectManager::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QVERIFY(m_workDir.isValid());

    // ConfigOptions (reached through ListeRoom) reads and writes QSettings:
    // keep the developer's real configuration out of the tests.
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_projectmanager");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());

    // The reader opens a modal QMessageBox on a duplicate id: close it, or the
    // run hangs forever on a headless runner.
    connect(&m_dialogKiller, &QTimer::timeout, this, [this]()
    {
        if (QWidget *w = QApplication::activeModalWidget())
        {
            if (QMessageBox *mb = qobject_cast<QMessageBox *>(w))
                m_dialogs << mb->text();
            else
                m_otherModals << QString::fromLatin1(w->metaObject()->className());
            w->close();
        }
    });
    m_dialogKiller.start(20);
}

void tst_ProjectManager::init()
{
    resetModel();
    m_dialogs.clear();
    m_expectedDialogs.clear();
    m_otherModals.clear();
}

// A modal the killer closes is invisible to the test body: without this, a
// spurious "already exists" box would pass unnoticed.
void tst_ProjectManager::cleanup()
{
    QCOMPARE(m_otherModals, QStringList());
    QCOMPARE(m_dialogs, m_expectedDialogs);
}

static QStringList samplesWith(const QString &file)
{
    QStringList names;
    QDir d(CALAOS_SAMPLES_DIR);
    for (const QString &n : d.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name))
        if (QFileInfo::exists(d.filePath(n + "/" + file)))
            names << n;
    return names;
}

static void addProjectRows(const QString &file)
{
    QTest::addColumn<QString>("project");
    for (const QString &n : samplesWith(file))
        QTest::newRow(qPrintable(n)) << n;
}

// ---------------------------------------------------------------------------
// Sample projects: tests/<project>/{io,rules}.xml
// ---------------------------------------------------------------------------

void tst_ProjectManager::ioRoundTrip_data() { addProjectRows("io.xml"); }

void tst_ProjectManager::ioRoundTrip()
{
    QFETCH(QString, project);

    QVERIFY(loadIOs(sample(project, "io.xml")));
    QStringList before = snapshotRooms();
    QVERIFY2(!before.isEmpty(), "sample loaded no room");

    QVERIFY(!saveIOs("io.xml").isEmpty());

    resetModel();
    QVERIFY(loadIOs(work("io.xml")));
    QCOMPARE(snapshotRooms(), before);
    QVERIFY(m_dialogs.isEmpty());
}

void tst_ProjectManager::ioSecondWriteIsIdentical_data() { addProjectRows("io.xml"); }

void tst_ProjectManager::ioSecondWriteIsIdentical()
{
    QFETCH(QString, project);

    QVERIFY(loadIOs(sample(project, "io.xml")));
    QByteArray first = saveIOs("first.xml");
    QVERIFY(!first.isEmpty());

    resetModel();
    QVERIFY(loadIOs(work("first.xml")));
    QVERIFY(first == saveIOs("second.xml"));
}

void tst_ProjectManager::ioWriteOnlyAddsKnownDefaults_data() { addProjectRows("io.xml"); }

// The first save of an old project is allowed to rewrite the file, but only by
// spelling out defaults. Anything else (a lost or altered attribute, a moved
// element) is a regression of the data, not a cosmetic change.
void tst_ProjectManager::ioWriteOnlyAddsKnownDefaults()
{
    QFETCH(QString, project);

    QVERIFY(loadIOs(sample(project, "io.xml")));
    QByteArray written = saveIOs("io.xml");
    QCOMPARE(unexpectedChanges(readAll(sample(project, "io.xml")), written), QStringList());
}

void tst_ProjectManager::rulesRoundTrip_data() { addProjectRows("rules.xml"); }

void tst_ProjectManager::rulesRoundTrip()
{
    QFETCH(QString, project);

    if (QFileInfo::exists(sample(project, "io.xml")))
        QVERIFY(loadIOs(sample(project, "io.xml")));
    QVERIFY(loadRules(sample(project, "rules.xml")));
    QStringList before = snapshotRules();

    QVERIFY(!saveRules("rules.xml").isEmpty());

    ListeRule::Instance().clear();
    QVERIFY(loadRules(work("rules.xml")));
    QCOMPARE(snapshotRules(), before);
}

void tst_ProjectManager::rulesSecondWriteIsIdentical_data() { addProjectRows("rules.xml"); }

void tst_ProjectManager::rulesSecondWriteIsIdentical()
{
    QFETCH(QString, project);

    if (QFileInfo::exists(sample(project, "io.xml")))
        QVERIFY(loadIOs(sample(project, "io.xml")));
    QVERIFY(loadRules(sample(project, "rules.xml")));
    QByteArray first = saveRules("first.xml");
    QVERIFY(!first.isEmpty());

    ListeRule::Instance().clear();
    QVERIFY(loadRules(work("first.xml")));
    QVERIFY(first == saveRules("second.xml"));
}

void tst_ProjectManager::rulesWriteOnlyAddsKnownDefaults_data() { addProjectRows("rules.xml"); }

void tst_ProjectManager::rulesWriteOnlyAddsKnownDefaults()
{
    QFETCH(QString, project);

    if (QFileInfo::exists(sample(project, "io.xml")))
        QVERIFY(loadIOs(sample(project, "io.xml")));
    QVERIFY(loadRules(sample(project, "rules.xml")));
    QByteArray written = saveRules("rules.xml");
    QCOMPARE(unexpectedChanges(readAll(sample(project, "rules.xml")), written), QStringList());
}

// ---------------------------------------------------------------------------
// io.xml: lossy or surprising behaviour, pinned as it is today
// ---------------------------------------------------------------------------

// An unknown element is carried as an opaque subtree: what it contains is not
// instantiated (input_2 is no IO of the model) and nothing after it is lost.
// Unknown elements are written after the known content of their parent.
void tst_ProjectManager::ioUnknownRoomChildIsKeptWithItsSubtree()
{
    QByteArray src = ioSource(R"(
<calaos:room name="Salon" type="salon" hits="3">
  <calaos:input id="input_1" name="A" type="WITemp" host="1.2.3.4" port="502" var="1"/>
  <calaos:future name="Thermo" kind="x">
    <calaos:input id="input_2" name="B" type="WITemp" host="1.2.3.4" port="502" var="2"/>
    <calaos:output id="output_3" name="C" type="WODigital" host="1.2.3.4" port="502" var="3"/>
  </calaos:future>
  <calaos:input id="input_4" name="D" type="WITemp" host="1.2.3.4" port="502" var="4"/>
</calaos:room>
<calaos:room name="Cuisine" type="cuisine" hits="0">
  <calaos:input id="input_5" name="E" type="WITemp" host="1.2.3.4" port="502" var="5"/>
</calaos:room>)");

    QString expected = ioDoc(
R"(        <calaos:room name="Salon" type="salon" hits="3">
            <calaos:input host="1.2.3.4" id="input_1" name="A" port="502" type="WITemp" var="1"/>
            <calaos:input host="1.2.3.4" id="input_4" name="D" port="502" type="WITemp" var="4"/>
            <calaos:future name="Thermo" kind="x">
                <calaos:input id="input_2" name="B" type="WITemp" host="1.2.3.4" port="502" var="2"/>
                <calaos:output id="output_3" name="C" type="WODigital" host="1.2.3.4" port="502" var="3"/>
            </calaos:future>
        </calaos:room>
        <calaos:room name="Cuisine" type="cuisine" hits="0">
            <calaos:input host="1.2.3.4" id="input_5" name="E" port="502" type="WITemp" var="5"/>
        </calaos:room>
)");
    QCOMPARE(ioThroughModel(src), expected);
    QCOMPARE(ListeRoom::Instance().size(), 2);
    QVERIFY(ListeRoom::Instance().get_input("input_2") == nullptr);
    QVERIFY(ListeRoom::Instance().get_output("output_3") == nullptr);

    QCOMPARE(ioThroughModel(expected.toUtf8()), expected);
}

void tst_ProjectManager::ioUnknownLeafRoomChildIsKept()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="i" type="WITemp" name="i" host="h" port="502" var="0"/>
  <calaos:foo bar="1"/>
  <calaos:input id="i2" type="WITemp" name="i2" host="h" port="502" var="0"/>
</calaos:room>)");

    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input host="h" id="i" name="i" port="502" type="WITemp" var="0"/>
            <calaos:input host="h" id="i2" name="i2" port="502" type="WITemp" var="0"/>
            <calaos:foo bar="1"/>
        </calaos:room>
)"));
}

void tst_ProjectManager::ioUnknownInputChildIsKept()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="a" type="WITemp" name="a" host="h" port="502" var="0">
    <calaos:bogus><calaos:input id="nested"/></calaos:bogus>
  </calaos:input>
  <calaos:output id="o" type="WODigital" name="o" host="h" port="502"><calaos:bogus2/></calaos:output>
  <calaos:input id="b" type="WITemp" name="b" host="h" port="502" var="0"/>
</calaos:room>
<calaos:room name="R2" type="salon" hits="1"/>)");

    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input host="h" id="a" name="a" port="502" type="WITemp" var="0">
                <calaos:bogus>
                    <calaos:input id="nested"/>
                </calaos:bogus>
            </calaos:input>
            <calaos:input host="h" id="b" name="b" port="502" type="WITemp" var="0"/>
            <calaos:output host="h" id="o" name="o" port="502" type="WODigital">
                <calaos:bogus2/>
            </calaos:output>
        </calaos:room>
        <calaos:room name="R2" type="salon" hits="1"/>
)"));
    QVERIFY(ListeRoom::Instance().get_input("nested") == nullptr);
}

// Used to end <home> early, and the next <room> was then rejected as a
// misplaced element: the whole load failed.
void tst_ProjectManager::ioUnknownHomeChildIsKept()
{
    QByteArray src = ioSource(R"(
<calaos:room name="A" type="salon" hits="1"/>
<calaos:zones v="2"><calaos:room name="Inner" type="salon"/></calaos:zones>
<calaos:room name="B" type="salon" hits="1"/>)");

    writeAll(work("src.xml"), src);
    QVERIFY(loadIOs(work("src.xml")));
    QCOMPARE(ListeRoom::Instance().size(), 2);

    QCOMPARE(QString::fromUtf8(saveIOs("dst.xml")), ioDoc(
R"(        <calaos:room name="A" type="salon" hits="1"/>
        <calaos:room name="B" type="salon" hits="1"/>
        <calaos:zones v="2">
            <calaos:room name="Inner" type="salon"/>
        </calaos:zones>
)"));

    // A new project must not inherit them.
    QCOMPARE(ioThroughModel(ioSource(R"(<calaos:room name="A" type="salon" hits="1"/>)")), ioDoc(
R"(        <calaos:room name="A" type="salon" hits="1"/>
)"));
}

void tst_ProjectManager::ioUnknownChildrenOfEveryIoKindAreKept()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:internal id="n1" type="InternalBool" name="b"><calaos:x1/></calaos:internal>
  <calaos:input id="s1" type="scenario" name="scn"><calaos:x2/></calaos:input>
  <calaos:camera name="cam" type="axis" iid="ci" oid="co" id="ci_co"><calaos:x3/></calaos:camera>
  <calaos:audio name="aud" type="slim" iid="ai" oid="ao"><calaos:x4/></calaos:audio>
  <calaos:avr name="avr" type="AVReceiver" id="avr_0" host="x" model="pioneer"><calaos:x5/></calaos:avr>
  <calaos:input id="last" type="WITemp" name="last" host="h" port="502" var="0"/>
</calaos:room>)");

    QString expected = ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:internal id="n1" name="b" type="InternalBool">
                <calaos:x1/>
            </calaos:internal>
            <calaos:input id="s1" name="scn" type="scenario">
                <calaos:x2/>
            </calaos:input>
            <calaos:input host="h" id="last" name="last" port="502" type="WITemp" var="0"/>
            <calaos:camera id="ci_co" iid="ci" name="cam" oid="co" type="axis">
                <calaos:x3/>
            </calaos:camera>
            <calaos:audio iid="ai" name="aud" oid="ao" type="slim">
                <calaos:x4/>
            </calaos:audio>
            <calaos:avr host="x" id="avr_0" model="pioneer" name="avr" type="AVReceiver">
                <calaos:x5/>
            </calaos:avr>
        </calaos:room>
)");
    QCOMPARE(ioThroughModel(src), expected);
    QCOMPARE(ioThroughModel(expected.toUtf8()), expected);
}

void tst_ProjectManager::ioUnknownTextCdataAndNamespacesAreKept()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:note lang="fr">a &amp; b</calaos:note>
  <calaos:script><![CDATA[if a < b then]]></calaos:script>
  <ext:thing xmlns:ext="urn:example" ext:k="v"><ext:sub/></ext:thing>
  <plain a="1"/>
</calaos:room>)");

    QString expected = ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:note lang="fr">a &amp; b</calaos:note>
            <calaos:script><![CDATA[if a < b then]]></calaos:script>
            <ext:thing xmlns:ext="urn:example" ext:k="v">
                <ext:sub/>
            </ext:thing>
            <plain a="1"/>
        </calaos:room>
)");
    QCOMPARE(ioThroughModel(src), expected);
    QCOMPARE(ioThroughModel(expected.toUtf8()), expected);
}

// Nothing in a time range can be kept as is (the model holds TimeRange
// values), but an unknown element must still be skipped with its subtree.
void tst_ProjectManager::ioUnknownElementsInTimeRangesAreSkippedWhole()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="t" type="InPlageHoraire" name="t" months="111111111111">
    <calaos:lundi>
      <calaos:junk><calaos:plage start_hour="5"/></calaos:junk>
      <calaos:plage start_hour="8"><calaos:junk/></calaos:plage>
      <calaos:plage start_hour="9"/>
    </calaos:lundi>
    <calaos:mardi><calaos:plage start_hour="10"/></calaos:mardi>
  </calaos:input>
  <calaos:input id="after" type="WITemp" name="after" host="h" port="502" var="0"/>
</calaos:room>)");

    ioThroughModel(src);
    IOBase *t = ListeRoom::Instance().get_input("t");
    QVERIFY(t);
    QCOMPARE(int(t->range_monday.size()), 2);
    QCOMPARE(QString::fromStdString(t->range_monday[0].shour), QString("8"));
    QCOMPARE(QString::fromStdString(t->range_monday[1].shour), QString("9"));
    QCOMPARE(int(t->range_tuesday.size()), 1);
    QVERIFY(ListeRoom::Instance().get_input("after"));
}

void tst_ProjectManager::ioAllIoKindsRoundTrip()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:internal id="n1" type="InternalBool" name="b"/>
  <calaos:input id="n2" type="InternalInt" name="i"/>
  <calaos:input id="s1" type="scenario" name="scn"/>
  <calaos:input id="tm1" type="InputTimer" name="tm" msec="5"/>
  <calaos:input id="tm2" type="InputTime" name="time"/>
  <calaos:camera name="cam" type="axis" iid="input_9" oid="output_9" id="input_9_output_9" host="x"/>
  <calaos:camera name="cam2" type="axis"/>
  <calaos:audio name="aud" type="slim" iid="input_10" oid="output_10" host="x"/>
  <calaos:avr name="avr" type="AVReceiver" id="avr_0" host="x" model="pioneer"/>
  <calaos:avr/>
</calaos:room>)");

    // InternalInt read from <input> comes back as <internal>; the camera
    // without ids gets generated ones, the audio player never gets an id.
    QString expected = ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:internal id="n1" name="b" type="InternalBool"/>
            <calaos:internal id="n2" name="i" type="InternalInt"/>
            <calaos:input id="s1" name="scn" type="scenario"/>
            <calaos:input hour="0" id="tm1" min="0" msec="5" name="tm" sec="0" type="InputTimer"/>
            <calaos:input hour="0" id="tm2" min="0" name="time" sec="0" type="InputTime"/>
            <calaos:camera host="x" id="input_9_output_9" iid="input_9" name="cam" oid="output_9" type="axis"/>
            <calaos:camera id="input_0_output_0" iid="input_0" name="cam2" oid="output_0" type="axis"/>
            <calaos:audio host="x" iid="input_10" name="aud" oid="output_10" type="slim"/>
            <calaos:avr host="x" id="avr_0" model="pioneer" name="avr" type="AVReceiver"/>
            <calaos:avr host="192.168.1.10" id="avr_1" model="pioneer" name="A/V Receiver" type="AVReceiver"/>
        </calaos:room>
)");
    QCOMPARE(ioThroughModel(src), expected);

    QStringList model = snapshotRooms();
    QCOMPARE(ioThroughModel(expected.toUtf8()), expected);
    QCOMPARE(snapshotRooms(), model);
}

// Defaults injected by ListeRoom::createInput/createOutput, which the reader
// goes through. Loading never preserves "absent": it becomes an explicit value.
void tst_ProjectManager::ioFactoryDefaultsAreWritten()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="i1" type="WITemp"/>
  <calaos:input id="i2"/>
  <calaos:output id="o1"/>
  <calaos:output id="o2" type="WODali" host="h" address="1"/>
  <calaos:output id="o3" type="WOVolet" host="h"/>
</calaos:room>)");

    // 10.0.0.123 is WAGO_HOST, used when no Wago host is configured.
    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input host="10.0.0.123" id="i1" name="Input" port="502" type="WITemp" var="0"/>
            <calaos:input host="10.0.0.123" id="i2" name="Input" port="502" type="WIDigitalBP" var="0"/>
            <calaos:output host="10.0.0.123" id="o1" name="Output" port="502" type="WODigital"/>
            <calaos:output address="1" host="h" id="o2" name="Output" port="502" type="WODali"/>
            <calaos:output host="h" id="o3" name="Output" port="502" time="30" type="WOVolet" var_down="0" var_up="0"/>
        </calaos:room>
)"));
}

// No factory defaults for an unregistered type: the attributes pass through as
// they are, and nothing validates them.
void tst_ProjectManager::ioUnknownTypeIsKeptVerbatim()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="i4" type="Bogus" name="x" zzz="1"/>
  <calaos:output id="o4" type="Bogus" name="y"/>
</calaos:room>)");

    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input id="i4" name="x" type="Bogus" zzz="1"/>
            <calaos:output id="o4" name="y" type="Bogus"/>
        </calaos:room>
)"));
}

// Document order inside a room is not preserved (all inputs, then all
// outputs), and attributes are written sorted by name (Params is a std::map).
void tst_ProjectManager::ioInputsAreWrittenBeforeOutputs()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:output type="WODigital" id="o" host="h" port="502"/>
  <calaos:input type="WITemp" id="i" var="0" host="h" port="502" name="n"/>
</calaos:room>)");

    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input host="h" id="i" name="n" port="502" type="WITemp" var="0"/>
            <calaos:output host="h" id="o" name="Output" port="502" type="WODigital"/>
        </calaos:room>
)"));
}

// The server treats a missing or unparsable "months" as all months active;
// the reader must not turn an absent attribute into "000000000000" on save.
void tst_ProjectManager::ioTimeRangeWithoutMonthsStaysActiveAllYear()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="t1" type="InPlageHoraire" name="absent"/>
  <calaos:input id="t3" type="InPlageHoraire" name="garbage" months="abc"/>
</calaos:room>)");

    const char *days = R"(                <calaos:lundi/>
                <calaos:mardi/>
                <calaos:mercredi/>
                <calaos:jeudi/>
                <calaos:vendredi/>
                <calaos:samedi/>
                <calaos:dimanche/>
)";
    QCOMPARE(ioThroughModel(src), ioDoc(
QString(R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input months="111111111111" id="t1" name="absent" type="InPlageHoraire">
%1            </calaos:input>
            <calaos:input months="111111111111" id="t3" name="garbage" type="InPlageHoraire">
%1            </calaos:input>
        </calaos:room>
)").arg(days)));
}

// months="" is present, so it is not the "absent" case above: the server
// parses it as an empty set and keeps every month disabled.
void tst_ProjectManager::ioTimeRangeEmptyMonthsDisablesAllMonths()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="t1" type="InPlageHoraire" name="empty" months=""/>
</calaos:room>)");

    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input months="000000000000" id="t1" name="empty" type="InPlageHoraire">
                <calaos:lundi/>
                <calaos:mardi/>
                <calaos:mercredi/>
                <calaos:jeudi/>
                <calaos:vendredi/>
                <calaos:samedi/>
                <calaos:dimanche/>
            </calaos:input>
        </calaos:room>
)"));
}

void tst_ProjectManager::ioTimeRangeExplicitMonthsRoundTrip()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="t1" type="InPlageHoraire" name="none" months="000000000000"/>
  <calaos:input id="t2" type="InPlageHoraire" name="some" months="010100000110"/>
</calaos:room>)");

    const char *days = R"(                <calaos:lundi/>
                <calaos:mardi/>
                <calaos:mercredi/>
                <calaos:jeudi/>
                <calaos:vendredi/>
                <calaos:samedi/>
                <calaos:dimanche/>
)";
    QCOMPARE(ioThroughModel(src), ioDoc(
QString(R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input months="000000000000" id="t1" name="none" type="InPlageHoraire">
%1            </calaos:input>
            <calaos:input months="010100000110" id="t2" name="some" type="InPlageHoraire">
%1            </calaos:input>
        </calaos:room>
)").arg(days)));
}

void tst_ProjectManager::ioTimeRangeMonthsAndPlages()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="t2" type="InPlageHoraire" name="months" months="110000000001">
    <calaos:lundi><calaos:plage start_hour="8" start_min="1" start_sec="2" end_hour="9" end_min="3" end_sec="4" start_type="1" start_offset="-1" end_type="2" end_offset="-1"/></calaos:lundi>
    <calaos:dimanche><calaos:plage start_hour="1"/></calaos:dimanche>
  </calaos:input>
</calaos:room>)");

    // A plage with missing fields is completed with zeroes, not rejected.
    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input months="110000000001" id="t2" name="months" type="InPlageHoraire">
                <calaos:lundi>
                    <calaos:plage start_hour="8" start_min="1" start_sec="2" start_type="1" start_offset="-1" end_hour="9" end_min="3" end_sec="4" end_type="2" end_offset="-1"/>
                </calaos:lundi>
                <calaos:mardi/>
                <calaos:mercredi/>
                <calaos:jeudi/>
                <calaos:vendredi/>
                <calaos:samedi/>
                <calaos:dimanche>
                    <calaos:plage start_hour="1" start_min="0" start_sec="0" start_type="0" start_offset="1" end_hour="0" end_min="0" end_sec="0" end_type="0" end_offset="1"/>
                </calaos:dimanche>
            </calaos:input>
        </calaos:room>
)"));
}

// The pages subtree is carried as an opaque string, like any other child of
// <remote_ui>.
void tst_ProjectManager::ioRemoteUiPages()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:remote_ui id="ui1" type="RemoteUI" name="ui">
    <calaos:pages>
      <calaos:page name="p1" a="1"><calaos:widget x="1">hello</calaos:widget></calaos:page>
    </calaos:pages>
    <calaos:other><calaos:x/></calaos:other>
  </calaos:remote_ui>
  <calaos:remote_ui id="ui2" type="RemoteUI" name="ui2"/>
</calaos:room>)");

    QString expected = ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:remote_ui id="ui1" name="ui" type="RemoteUI">
                <calaos:pages>
                    <calaos:page name="p1" a="1">
                        <calaos:widget x="1">hello</calaos:widget>
                    </calaos:page>
                </calaos:pages>
                <calaos:other>
                    <calaos:x/>
                </calaos:other>
            </calaos:remote_ui>
            <calaos:remote_ui id="ui2" name="ui2" type="RemoteUI"/>
        </calaos:room>
)");
    QCOMPARE(ioThroughModel(src), expected);
    QCOMPARE(ioThroughModel(expected.toUtf8()), expected);
}

void tst_ProjectManager::ioDuplicateIdsAreKeptAndReported()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="same" type="WITemp" name="a" host="h" port="502" var="0"/>
  <calaos:input id="same" type="WITemp" name="b" host="h" port="502" var="0"/>
  <calaos:output id="same" type="WODigital" name="c" host="h" port="502"/>
  <calaos:output id="same" type="WODigital" name="d" host="h" port="502"/>
</calaos:room>)");

    // The modal box comes from the reader itself (see m_dialogKiller).
    m_expectedDialogs = { "Input with id \"same\" already exists!",
                          "Output with id \"same\" already exists!" };
    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input host="h" id="same" name="a" port="502" type="WITemp" var="0"/>
            <calaos:input host="h" id="same" name="b" port="502" type="WITemp" var="0"/>
            <calaos:output host="h" id="same" name="c" port="502" type="WODigital"/>
            <calaos:output host="h" id="same" name="d" port="502" type="WODigital"/>
        </calaos:room>
)"));
    QCOMPARE(m_dialogs, m_expectedDialogs);
}

// The server reads hits with as_int(0): absent, empty and unparsable all
// land on 0. The reader used to leave an uninitialised int in the first two.
void tst_ProjectManager::ioRoomHitsDefaultToZero()
{
    QByteArray src = ioSource(R"(
<calaos:room name="absent" type="salon"/>
<calaos:room name="empty" type="salon" hits=""/>
<calaos:room name="garbage" type="salon" hits="abc"/>
<calaos:room name="set" type="salon" hits="7"/>)");

    resetModel();
    writeAll(work("hits.xml"), src);
    QVERIFY(loadIOs(work("hits.xml")));
    ListeRoom &lr = ListeRoom::Instance();
    QCOMPARE(lr.size(), 4);
    QCOMPARE(lr.get_room(0)->get_hits(), 0);
    QCOMPARE(lr.get_room(1)->get_hits(), 0);
    QCOMPARE(lr.get_room(2)->get_hits(), 0);
    QCOMPARE(lr.get_room(3)->get_hits(), 7);
}

// An IO without an id attribute gets a generated one, like on the server
// (createIO adds "io_N"): it is not a duplicate of the IOs already loaded.
void tst_ProjectManager::ioIosWithoutIdGetFreshIdsWithoutModal()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="input_0" type="WITemp" name="a" host="h" port="502" var="0"/>
  <calaos:input type="WITemp" name="b" host="h" port="502" var="0"/>
  <calaos:input type="WITemp" name="c" host="h" port="502" var="0"/>
  <calaos:output id="output_0" type="WODigital" name="d" host="h" port="502"/>
  <calaos:output type="WODigital" name="e" host="h" port="502"/>
</calaos:room>)");

    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input host="h" id="input_0" name="a" port="502" type="WITemp" var="0"/>
            <calaos:input host="h" id="input_1" name="b" port="502" type="WITemp" var="0"/>
            <calaos:input host="h" id="input_2" name="c" port="502" type="WITemp" var="0"/>
            <calaos:output host="h" id="output_0" name="d" port="502" type="WODigital"/>
            <calaos:output host="h" id="output_1" name="e" port="502" type="WODigital"/>
        </calaos:room>
)"));
    QCOMPARE(m_dialogs, QStringList());
}

// The server keeps an explicit id="" as is (and then refuses to resolve it);
// the installer keeps it too rather than rewriting the file behind the
// user's back, and the "already exists" box is not about empty ids.
void tst_ProjectManager::ioEmptyIdIsKeptAndNeverReportedAsDuplicate()
{
    QByteArray src = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="" type="WITemp" name="a" host="h" port="502" var="0"/>
  <calaos:input id="" type="WITemp" name="b" host="h" port="502" var="0"/>
  <calaos:output id="" type="WODigital" name="c" host="h" port="502"/>
  <calaos:output id="" type="WODigital" name="d" host="h" port="502"/>
</calaos:room>)");

    QCOMPARE(ioThroughModel(src), ioDoc(
R"(        <calaos:room name="R" type="salon" hits="1">
            <calaos:input host="h" id="" name="a" port="502" type="WITemp" var="0"/>
            <calaos:input host="h" id="" name="b" port="502" type="WITemp" var="0"/>
            <calaos:output host="h" id="" name="c" port="502" type="WODigital"/>
            <calaos:output host="h" id="" name="d" port="502" type="WODigital"/>
        </calaos:room>
)"));
    QCOMPARE(m_dialogs, QStringList());
}

void tst_ProjectManager::listeRoomEmptyIdNeverMatches()
{
    writeAll(work("src.xml"), ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="i1" type="WITemp" name="i1" host="h" port="502" var="1"/>
  <calaos:output id="o2" type="WODigital" name="o2" host="h" port="502" var="2"/>
  <calaos:camera name="cam" type="axis" iid="ci" oid="co" id="ci_co"/>
</calaos:room>)"));
    QVERIFY(loadIOs(work("src.xml")));
    ListeRoom &lr = ListeRoom::Instance();

    QVERIFY(lr.get_input("") == nullptr);
    QVERIFY(lr.get_output("") == nullptr);

    // The legacy camera lookups by iid / oid still work.
    QCOMPARE(idOf(lr.get_input("ci")), QString("ci_co"));
    QCOMPARE(idOf(lr.get_output("co")), QString("ci_co"));
    QCOMPARE(idOf(lr.get_input("i1")), QString("i1"));
    QCOMPARE(idOf(lr.get_output("o2")), QString("o2"));
}

// A file the reader rejected must be reported as such, even though the rooms
// parsed before the error stay in the model.
void tst_ProjectManager::ioLoadFailsOnUnknownRoot()
{
    writeAll(work("bad.xml"), "<?xml version=\"1.0\"?><notcalaos/>");
    QVERIFY(!loadIOs(work("bad.xml")));
    QCOMPARE(ListeRoom::Instance().size(), 0);
}

void tst_ProjectManager::ioLoadFailsOnTruncatedFile()
{
    // Truncated in the middle of the second room.
    writeAll(work("cut.xml"), ioSource(R"(
<calaos:room name="A" type="salon" hits="1"><calaos:input id="a" type="WITemp"/></calaos:room>
<calaos:room name="B" type="salon" hits="1"><calaos:input id="b" type="WITemp"/>)"));
    QVERIFY(!loadIOs(work("cut.xml")));
    QCOMPARE(ListeRoom::Instance().size(), 2);
    QCOMPARE(ListeRoom::Instance().get_room(1)->get_size_in(), 1);
}

void tst_ProjectManager::ioLoadFailsOnMissingFile()
{
    QVERIFY(!loadIOs(work("does_not_exist.xml")));
}

// ---------------------------------------------------------------------------
// rules.xml
// ---------------------------------------------------------------------------

static const char *kRulesIo = R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="i1" type="WIDigitalBP" name="i1" host="h" port="502" var="1"/>
  <calaos:input id="i2" type="WITemp" name="i2" host="h" port="502" var="2"/>
  <calaos:output id="o1" type="WODigital" name="o1" host="h" port="502" var="1"/>
  <calaos:output id="o2" type="WODigital" name="o2" host="h" port="502" var="2"/>
  <calaos:camera name="cam" type="axis" iid="ci" oid="co" id="ci_co"/>
</calaos:room>)";

// Reader and writer disagree on the unit: a standard <condition>/<action>
// with several children is one object in the model but is written back as one
// element per child. Conditions are ANDed so the meaning is the same, the
// shape of the file is not.
void tst_ProjectManager::rulesStandardEntriesWithSeveralIosAreSplit()
{
    QByteArray rules = rulesSource(R"(
<calaos:rule name="r" type="t">
  <calaos:condition type="standard" trigger="false">
    <calaos:input id="i2" oper="&gt;" val="5" val_var="i1"/>
    <calaos:input id="i1" oper="==" val="false"/>
  </calaos:condition>
  <calaos:action type="standard">
    <calaos:output id="o1" val="toggle"/>
    <calaos:output id="o2" val="true" val_var="o1"/>
  </calaos:action>
</calaos:rule>)");

    QCOMPARE(rulesThroughModel(ioSource(kRulesIo), rules), rulesDoc(
R"(    <calaos:rule name="r" type="t">
        <calaos:condition type="standard" trigger="false">
            <calaos:input id="i2" oper="&gt;" val="5" val_var="i1"/>
        </calaos:condition>
        <calaos:condition type="standard" trigger="false">
            <calaos:input id="i1" oper="==" val="false"/>
        </calaos:condition>
        <calaos:action type="standard">
            <calaos:output id="o1" val="toggle"/>
        </calaos:action>
        <calaos:action type="standard">
            <calaos:output id="o2" val="true" val_var="o1"/>
        </calaos:action>
    </calaos:rule>
)"));
}

void tst_ProjectManager::rulesEveryKindRoundTrips()
{
    QByteArray rules = rulesSource(R"(
<calaos:rule name="r1" type="salon" extra="x">
  <calaos:condition type="output" trigger="false"><calaos:output id="o1" oper="==" val="true" val_var="o2"/></calaos:condition>
  <calaos:condition type="start"/>
  <calaos:condition type="script"><calaos:input id="i1"/><calaos:script type="lua"><![CDATA[return a
  and b]]></calaos:script></calaos:condition>
  <calaos:action type="mail"><calaos:mail sender="a@b" recipients="c@d" subject="s" attachment="att"><![CDATA[line1
line2 é]]></calaos:mail></calaos:action>
  <calaos:action type="script"><calaos:script type="lua"><![CDATA[print(1)]]></calaos:script></calaos:action>
  <calaos:action type="touchscreen" action="view_camera" camera="cam"/>
  <calaos:action type="push"><calaos:push attachment="pa"><![CDATA[pushed]]></calaos:push></calaos:action>
</calaos:rule>
<calaos:rule name="empty"/>)");

    QString expected = rulesDoc(
R"(    <calaos:rule extra="x" name="r1" type="salon">
        <calaos:condition type="output" trigger="false">
            <calaos:output id="o1" oper="==" val="true" val_var="o2"/>
        </calaos:condition>
        <calaos:condition type="start"/>
        <calaos:condition type="script">
            <calaos:input id="i1"/>
            <calaos:script type="lua"><![CDATA[return a
  and b]]></calaos:script>
        </calaos:condition>
        <calaos:action type="mail">
            <calaos:mail sender="a@b" recipients="c@d" subject="s" attachment="att"><![CDATA[line1
line2 é]]></calaos:mail>
        </calaos:action>
        <calaos:action type="script">
            <calaos:script type="lua"><![CDATA[print(1)]]></calaos:script>
        </calaos:action>
        <calaos:action type="touchscreen" action="view_camera" camera="cam"/>
        <calaos:action type="push">
            <calaos:push attachment="pa"><![CDATA[pushed]]></calaos:push>
        </calaos:action>
    </calaos:rule>
    <calaos:rule name="empty"/>
)");
    QCOMPARE(rulesThroughModel(ioSource(kRulesIo), rules), expected);
    QStringList model = snapshotRules();
    QCOMPARE(rulesThroughModel(ioSource(kRulesIo), expected.toUtf8()), expected);
    QCOMPARE(snapshotRules(), model);
}

// Silent data loss: whatever the reader does not recognise is not kept, and
// nothing tells the user.
void tst_ProjectManager::rulesUnsupportedEntriesAreDropped()
{
    QByteArray rules = rulesSource(R"(
<calaos:rule name="r" type="t">
  <calaos:condition type="output"/>
  <calaos:condition type="bogus"><calaos:input id="i1"/></calaos:condition>
  <calaos:action type="bogus"/>
  <calaos:action type="touchscreen" action="goto_page" camera="cam"/>
</calaos:rule>)");

    // The camera attribute only survives for the view_camera action.
    QCOMPARE(rulesThroughModel(ioSource(kRulesIo), rules), rulesDoc(
R"(    <calaos:rule name="r" type="t">
        <calaos:action type="touchscreen" action="goto_page"/>
    </calaos:rule>
)"));
}

void tst_ProjectManager::rulesMissingIOsSurvive()
{
    QByteArray rules = rulesSource(R"(
<calaos:rule name="ghost" type="x">
  <calaos:condition><calaos:input id="ghost1" oper="==" val="true"/></calaos:condition>
  <calaos:condition type="output"><calaos:output id="ghost2" oper="==" val="true"/></calaos:condition>
  <calaos:condition type="script"><calaos:input id="ghost1"/></calaos:condition>
  <calaos:action><calaos:output id="ghost3" val="true"/></calaos:action>
</calaos:rule>)");

    QCOMPARE(rulesThroughModel(ioSource(kRulesIo), rules), rulesDoc(
R"(    <calaos:rule name="ghost" type="x">
        <calaos:condition type="standard" trigger="true">
            <calaos:input id="ghost1" oper="==" val="true"/>
        </calaos:condition>
        <calaos:condition type="output" trigger="true">
            <calaos:output id="ghost2" oper="==" val="true"/>
        </calaos:condition>
        <calaos:condition type="script">
            <calaos:input id="ghost1"/>
            <calaos:script type="lua"><![CDATA[]]></calaos:script>
        </calaos:condition>
        <calaos:action type="standard">
            <calaos:output id="ghost3" val="true"/>
        </calaos:action>
    </calaos:rule>
)"));

    QCOMPARE(ProjectManager::missingIOReport(),
             QStringList({ QString::fromUtf8("• input ghost1 (ghost)"),
                           QString::fromUtf8("• output ghost2 (ghost)"),
                           QString::fromUtf8("• output ghost3 (ghost)") }));

    // The placeholders belong to no room: io.xml must not grow from them.
    QCOMPARE(snapshotRooms().filter("ghost").size(), 0);
}

// An empty id must not be resolved: it used to match get_param("iid") == ""
// on any IO without an iid, re-targeting the action to the first output.
// getMissingIO() refuses "" as well, so the reference is dropped, not kept.
void tst_ProjectManager::rulesEmptyIdIsNotBoundToAnyIO()
{
    QByteArray rules = rulesSource(R"(
<calaos:rule name="r" type="t">
  <calaos:condition><calaos:input id="" oper="==" val="true"/></calaos:condition>
  <calaos:action><calaos:output id="" val="true"/></calaos:action>
</calaos:rule>)");

    QCOMPARE(rulesThroughModel(ioSource(kRulesIo), rules), rulesDoc(
R"(    <calaos:rule name="r" type="t"/>
)"));
    QVERIFY(ProjectManager::missingIOReport().isEmpty());
}

// Rules written for the old camera model reference the camera's iid or oid;
// they are rewritten to the combined id on the first save.
void tst_ProjectManager::rulesLegacyIidIsRewrittenToId()
{
    QByteArray rules = rulesSource(R"(
<calaos:rule name="r" type="t">
  <calaos:condition><calaos:input id="ci" oper="==" val="true"/></calaos:condition>
  <calaos:action><calaos:output id="co" val="true"/></calaos:action>
</calaos:rule>)");

    QCOMPARE(rulesThroughModel(ioSource(kRulesIo), rules), rulesDoc(
R"(    <calaos:rule name="r" type="t">
        <calaos:condition type="standard" trigger="true">
            <calaos:input id="ci_co" oper="==" val="true"/>
        </calaos:condition>
        <calaos:action type="standard">
            <calaos:output id="ci_co" val="true"/>
        </calaos:action>
    </calaos:rule>
)"));
}

void tst_ProjectManager::rulesLoadFailsOnMalformedFile()
{
    writeAll(work("bad_rules.xml"), "<calaos:rules xmlns:calaos=\"http://www.calaos.fr\"><calaos:rule>");
    QVERIFY(!loadRules(work("bad_rules.xml")));
    QVERIFY(!loadRules(work("does_not_exist.xml")));
}

// A room is still in the list while its IOs are deleted (they look themselves up
// there), so a room already destroyed must not be in it: the walk would read freed
// memory, which only faults when the heap layout allows it.
void tst_ProjectManager::clearNeverLeavesADestroyedRoomInTheList()
{
    ListeRoom &lr = ListeRoom::Instance();
    const int count = 4;
    int pending = count;
    int staleRooms = 0;

    for (int i = 0;i < count;i++)
    {
        Room *r = new Room("room" + std::to_string(i), "salon");
        lr.Add(r);

        Params p;
        p.Add("id", "io" + std::to_string(i));
        const bool asInput = i % 2;
        IOBase *io = new ProbeIO(p, asInput ? IOBase::IO_INPUT : IOBase::IO_OUTPUT, [&]()
        {
            if (lr.size() > pending)
                staleRooms += lr.size() - pending;
            pending--;
        });
        if (asInput)
            r->AddInput(io);
        else
            r->AddOutput(io);
    }

    lr.clear();

    QCOMPARE(pending, 0);
    QCOMPARE(lr.size(), 0);
    QCOMPARE(staleRooms, 0);
}

// ---------------------------------------------------------------------------
// Rules and deleted IOs
// ---------------------------------------------------------------------------

namespace
{

enum Route { DeleteIO, DestroyRoom };

// Every way a condition or an action can point at an IO, each on the IO that is
// deleted by the test, plus a rule on an IO that must survive.
const char *kRulesOnTheIOs = R"(
<calaos:rule name="std_cond" type="t">
  <calaos:condition><calaos:input id="i1" oper="==" val="true"/></calaos:condition>
</calaos:rule>
<calaos:rule name="out_cond" type="t">
  <calaos:condition type="output"><calaos:output id="o1" oper="==" val="true"/></calaos:condition>
</calaos:rule>
<calaos:rule name="script_cond" type="t">
  <calaos:condition type="script"><calaos:input id="i1"/><calaos:script type="lua"><![CDATA[return true]]></calaos:script></calaos:condition>
</calaos:rule>
<calaos:rule name="std_action" type="t">
  <calaos:action><calaos:output id="o1" val="true"/></calaos:action>
</calaos:rule>
<calaos:rule name="camera" type="t">
  <calaos:condition><calaos:input id="ci" oper="==" val="true"/></calaos:condition>
</calaos:rule>
<calaos:rule name="bystander" type="t">
  <calaos:condition><calaos:input id="i2" oper="==" val="true"/></calaos:condition>
  <calaos:condition type="output"><calaos:output id="o2" oper="==" val="true"/></calaos:condition>
  <calaos:action><calaos:output id="o2" val="true"/></calaos:action>
</calaos:rule>)";

const char *kBystander = "bystander";

// i2 and o2 live in a room of their own so that destroying the room of the other
// IOs leaves them alone.
const char *kSplitIo = R"(
<calaos:room name="Doomed" type="salon" hits="1">
  <calaos:input id="i1" type="WIDigitalBP" name="i1" host="h" port="502" var="1"/>
  <calaos:output id="o1" type="WODigital" name="o1" host="h" port="502" var="1"/>
  <calaos:camera name="cam" type="axis" iid="ci" oid="co" id="ci_co"/>
</calaos:room>
<calaos:room name="Safe" type="salon" hits="1">
  <calaos:input id="i2" type="WITemp" name="i2" host="h" port="502" var="2"/>
  <calaos:output id="o2" type="WODigital" name="o2" host="h" port="502" var="2"/>
</calaos:room>)";

}

// Names of the rules still holding the address `io`. Only compares pointers: the
// IO may be freed already.
static QStringList rulesHolding(const void *io)
{
    QStringList names;
    ListeRule &rules = ListeRule::Instance();
    for (int i = 0;i < rules.size();i++)
    {
        Rule *r = rules.get_rule(i);
        bool holds = false;
        for (int c = 0;c < r->get_size_conds();c++)
        {
            Condition *cd = r->get_condition(c);
            for (int k = 0;k < cd->get_size();k++)
                holds |= cd->get_input(k) == io;
            for (int k = 0;k < cd->getScriptInputSize();k++)
                holds |= cd->getScriptInput(k) == io;
            holds |= cd->getOutput() == io;
        }
        for (int a = 0;a < r->get_size_actions();a++)
            for (int k = 0;k < r->get_action(a)->get_size();k++)
                holds |= r->get_action(a)->get_output(k) == io;
        if (holds)
            names << u(r->get_name());
    }
    return names;
}

// Loads the rules, then deletes `io` the way the installer does (route) and
// returns its address, which is dangling by then.
static const void *loadAndDelete(const QString &ioId, int route)
{
    ListeRoom &lr = ListeRoom::Instance();
    IOBase *target = lr.get_input(ioId.toStdString());
    if (!target)
        target = lr.get_output(ioId.toStdString());
    if (!target)
        return nullptr;

    const int roomIndex = lr.searchIO(target);
    if (roomIndex < 0)
        return nullptr;

    const void *address = target;
    if (route == DestroyRoom)
        lr.Remove(lr.get_room(roomIndex));
    else if (target->is_input())
        lr.deleteIOInput(target);
    else
        lr.deleteIOOutput(target);
    return address;
}

void tst_ProjectManager::deletingAnIOLeavesNoRuleOnIt_data()
{
    QTest::addColumn<QString>("rule");
    QTest::addColumn<QString>("io");
    QTest::addColumn<int>("route");

    const struct { const char *rule; const char *io; } cases[] = {
        { "std_cond", "i1" }, { "out_cond", "o1" },
        { "std_action", "o1" }, { "camera", "ci_co" },
    };
    for (const auto &c: cases)
    {
        QTest::newRow(qPrintable(QString("%1 / delete %2").arg(c.rule, c.io))) << c.rule << c.io << int(DeleteIO);
        QTest::newRow(qPrintable(QString("%1 / destroy room").arg(c.rule))) << c.rule << c.io << int(DestroyRoom);
    }
}

void tst_ProjectManager::deletingAnIOLeavesNoRuleOnIt()
{
    QFETCH(QString, rule);
    QFETCH(QString, io);
    QFETCH(int, route);

    const QString initial = rulesThroughModel(ioSource(kSplitIo), rulesSource(kRulesOnTheIOs));
    QVERIFY2(initial.contains(QString("name=\"%1\"").arg(rule)), qPrintable(initial));
    ListeRule &rules = ListeRule::Instance();
    const int before = rules.size();
    QVERIFY(rules.searchRule("t", rule.toStdString()));

    const void *deleted = loadAndDelete(io, route);
    QVERIFY(deleted);

    QVERIFY2(!rules.searchRule("t", rule.toStdString()), "the rule survived the deletion of its IO");
    QCOMPARE(rulesHolding(deleted), QStringList());
    QVERIFY(rules.size() < before);
    QVERIFY(rules.searchRule("t", kBystander));
}

// Saving walks every remaining rule: a pointer left to the freed IO is read here.
void tst_ProjectManager::savingAfterDeletingAnIOReadsNoFreedMemory()
{
    QFETCH(QString, rule);
    QFETCH(QString, io);
    QFETCH(int, route);

    rulesThroughModel(ioSource(kSplitIo), rulesSource(kRulesOnTheIOs));
    QVERIFY(loadAndDelete(io, route));

    const QString saved = QString::fromUtf8(saveRules("after_delete.xml"));
    QVERIFY(!saved.isEmpty());
    QVERIFY2(!saved.contains(QString("name=\"%1\"").arg(rule)), qPrintable(saved));
    QVERIFY2(saved.contains(kBystander), qPrintable(saved));
}

void tst_ProjectManager::deletingAnIOKeepsTheOtherRules()
{
    QVERIFY(rulesThroughModel(ioSource(kSplitIo), rulesSource(kRulesOnTheIOs)).contains(kBystander));
    ListeRoom &lr = ListeRoom::Instance();
    ListeRule &rules = ListeRule::Instance();
    QCOMPARE(rules.size(), 6);

    // i1, o1 and ci_co carry every other rule; none of them belongs to the bystander.
    // The script rule only loses its trigger.
    lr.Remove(lr.get_room(lr.searchIO(lr.get_input("i1"))));

    QCOMPARE(rules.size(), 2);
    QVERIFY(rules.searchRule("t", "script_cond"));
    Rule *bystander = rules.searchRule("t", kBystander);
    QVERIFY(bystander);
    QCOMPARE(bystander->get_size_conds(), 2);
    QCOMPARE(bystander->get_size_actions(), 1);
}

void tst_ProjectManager::deletingAScriptTriggerKeepsTheRuleAndItsLua_data()
{
    QTest::addColumn<int>("route");
    QTest::newRow("delete the IO") << int(DeleteIO);
    QTest::newRow("destroy the room") << int(DestroyRoom);
}

void tst_ProjectManager::deletingAScriptTriggerKeepsTheRuleAndItsLua()
{
    QFETCH(int, route);

    const QByteArray io = ioSource(R"(
<calaos:room name="Doomed" type="salon" hits="1">
  <calaos:input id="i1" type="WIDigitalBP" name="i1" host="h" port="502" var="1"/>
</calaos:room>
<calaos:room name="Safe" type="salon" hits="1">
  <calaos:input id="i2" type="WITemp" name="i2" host="h" port="502" var="2"/>
</calaos:room>)");
    const QByteArray rules = rulesSource(R"(
<calaos:rule name="script_cond" type="t">
  <calaos:condition type="script"><calaos:input id="i1"/><calaos:input id="i2"/><calaos:script type="lua"><![CDATA[return 42]]></calaos:script></calaos:condition>
</calaos:rule>)");

    rulesThroughModel(io, rules);
    QVERIFY(loadAndDelete("i1", route));

    const QString saved = QString::fromUtf8(saveRules("after_delete.xml"));
    QVERIFY2(saved.contains("name=\"script_cond\""), qPrintable(saved));
    QVERIFY2(saved.contains("return 42"), qPrintable(saved));
    QVERIFY2(saved.contains("<calaos:input id=\"i2\"/>"), qPrintable(saved));
    QVERIFY2(!saved.contains("id=\"i1\""), qPrintable(saved));
}

// A variable that has lost its IO stays in the rule on purpose (the user decides);
// the id must not be handed to a new IO, which would rewire the rule silently.
void tst_ProjectManager::anIOUsedAsAVariableKeepsItsIdReserved()
{
    const QByteArray io = ioSource(R"(
<calaos:room name="R" type="salon" hits="1">
  <calaos:input id="input_0" type="WITemp" name="var" host="h" port="502" var="1"/>
  <calaos:input id="i2" type="WITemp" name="i2" host="h" port="502" var="2"/>
  <calaos:output id="o2" type="WODigital" name="o2" host="h" port="502" var="2"/>
</calaos:room>)");
    const QByteArray rules = rulesSource(R"(
<calaos:rule name="vars" type="t">
  <calaos:condition><calaos:input id="i2" oper="&lt;" val="" val_var="input_0"/></calaos:condition>
  <calaos:condition type="output"><calaos:output id="o2" oper="==" val="" val_var="input_0"/></calaos:condition>
  <calaos:action><calaos:output id="o2" val="" val_var="input_0"/></calaos:action>
</calaos:rule>)");

    rulesThroughModel(io, rules);
    ListeRoom &lr = ListeRoom::Instance();
    QVERIFY(lr.deleteIOInput(lr.get_input("input_0")));

    IOBase *created = lr.createInput(Params(), lr.get_room(0));
    QVERIFY(created);
    QVERIFY2(created->get_param("id") != "input_0", "a new IO took the id of the deleted variable");

    const QString saved = QString::fromUtf8(saveRules("after_delete.xml"));
    QCOMPARE(saved.count("val_var=\"input_0\""), 3);
}

QTEST_MAIN(tst_ProjectManager)
#include "tst_projectmanager.moc"
