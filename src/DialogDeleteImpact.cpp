#include "DialogDeleteImpact.h"

#include <QDialogButtonBox>
#include <QFont>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QTreeWidget>
#include <QVBoxLayout>

using namespace Calaos;

static QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

QString DialogDeleteImpact::ioLabel(IOBase *io)
{
    QString name = u(io->get_param("name"));
    const QString id = u(io->get_param("id"));
    return name.isEmpty() ? id : QStringLiteral("%1 (%2)").arg(name, id);
}

QString DialogDeleteImpact::ruleLabel(Rule *rule)
{
    const QString name = u(rule->get_name());
    const QString type = u(rule->get_type());
    return type.isEmpty() ? name : QStringLiteral("%1 (%2)").arg(name, type);
}

DialogDeleteImpact::DialogDeleteImpact(const QString &subject, const DeletionImpact &impact,
                                       bool listOwnedIos, QWidget *parent):
    QDialog(parent)
{
    setWindowTitle(tr("Calaos Installer"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumWidth(520);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);

    QLabel *title = new QLabel(tr("Delete %1?").arg(subject), this);
    QFont f = title->font();
    f.setBold(true);
    f.setPointSizeF(f.pointSizeF() * 1.2);
    title->setFont(f);
    title->setWordWrap(true);
    title->setObjectName("title");
    layout->addWidget(title);

    QLabel *lead = new QLabel(this);
    lead->setObjectName("lead");
    lead->setWordWrap(true);
    layout->addWidget(lead);

    m_tree = new QTreeWidget(this);
    m_tree->setObjectName("impactTree");
    m_tree->setHeaderHidden(true);
    m_tree->setRootIsDecorated(false);
    m_tree->setSelectionMode(QAbstractItemView::NoSelection);
    m_tree->setFocusPolicy(Qt::NoFocus);
    m_tree->setIndentation(18);
    m_tree->setMinimumHeight(260);
    layout->addWidget(m_tree, 1);

    const QIcon trash(":/img/user-trash.png");
    const QIcon warn = style()->standardIcon(QStyle::SP_MessageBoxWarning);

    QStringList owned;
    if (listOwnedIos)
        for (size_t i = 1;i < impact.ios.size();i++)
            owned << ioLabel(impact.ios[i]);
    if (!owned.isEmpty())
        addSection(tr("Deleted with it (%1)").arg(owned.size()),
                   tr("Its own IOs."), owned, trash, false);

    QStringList removed;
    for (Rule *rule: impact.removedRules)
        removed << ruleLabel(rule);
    if (!removed.isEmpty())
        addSection(tr("Rules deleted (%1)").arg(removed.size()),
                   tr("They use something that is being deleted."), removed, trash, false);

    QStringList inert;
    for (Rule *rule: impact.inertRules)
        inert << ruleLabel(rule);
    if (!inert.isEmpty())
        addSection(tr("Script rules that will never run (%1)").arg(inert.size()),
                   tr("No trigger left: the script is kept, but the server never runs it."),
                   inert, warn, true);

    QStringList orphans;
    for (const OrphanVariable &v: impact.orphanVariables)
        orphans << tr("%1, variable \"%2\"").arg(ruleLabel(v.rule), u(v.id));
    if (!orphans.isEmpty())
        addSection(tr("Rules still naming a deleted variable (%1)").arg(orphans.size()),
                   tr("The id stays in the rule and is not given to a new IO."),
                   orphans, warn, true);

    QStringList links;
    for (const BrokenLink &l: impact.brokenLinks)
        links << tr("%1: link \"%2\" to %3").arg(ioLabel(l.device), u(l.link), u(l.io));
    if (!links.isEmpty())
        addSection(tr("Device links left pointing at nothing (%1)").arg(links.size()),
                   tr("The device still loads, without what that link gave it."),
                   links, warn, true);

    if (impact.isEmpty())
        lead->setText(owned.isEmpty() ? tr("No rule and no device uses it.")
                                      : tr("No rule and no other device uses them."));
    else
        lead->setText(tr("This also changes the rest of the project:"));
    m_tree->setVisible(m_tree->topLevelItemCount() > 0);

    m_buttons = new QDialogButtonBox(this);
    QPushButton *del = m_buttons->addButton(tr("Delete"), QDialogButtonBox::AcceptRole);
    del->setIcon(trash);
    del->setAutoDefault(false);
    QPushButton *cancel = m_buttons->addButton(QDialogButtonBox::Cancel);
    cancel->setDefault(true);
    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(m_buttons);

    resize(560, sizeHint().height());
}

void DialogDeleteImpact::addSection(const QString &title, const QString &hint, const QStringList &entries,
                                    const QIcon &icon, bool warning)
{
    QTreeWidgetItem *head = new QTreeWidgetItem(m_tree);
    head->setText(0, title);
    head->setIcon(0, icon);
    head->setToolTip(0, hint);
    head->setData(0, Qt::UserRole, warning);
    head->setFlags(Qt::ItemIsEnabled);
    QFont f = head->font(0);
    f.setBold(true);
    head->setFont(0, f);

    QTreeWidgetItem *note = new QTreeWidgetItem(head);
    note->setText(0, hint);
    note->setFlags(Qt::ItemIsEnabled);
    note->setForeground(0, palette().brush(QPalette::Disabled, QPalette::WindowText));
    QFont nf = note->font(0);
    nf.setItalic(true);
    note->setFont(0, nf);

    for (const QString &entry: entries)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(head);
        item->setText(0, entry);
        item->setToolTip(0, entry);
        item->setFlags(Qt::ItemIsEnabled);
    }

    head->setExpanded(true);
}
