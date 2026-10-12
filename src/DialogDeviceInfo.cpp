#include "DialogDeviceInfo.h"

#include <QDialogButtonBox>
#include <QFont>
#include <QHeaderView>
#include <QTreeWidget>
#include <QVBoxLayout>

#include <ListeRoom.h>

using namespace Calaos;

static QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

DialogDeviceInfo::DialogDeviceInfo(Device *device, QWidget *parent):
    QDialog(parent)
{
    setWindowTitle(tr("Device: %1").arg(u(device->get_param("name"))));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    resize(560, 460);

    QVBoxLayout *layout = new QVBoxLayout(this);

    m_tree = new QTreeWidget(this);
    m_tree->setObjectName("deviceTree");
    m_tree->setColumnCount(2);
    m_tree->setHeaderLabels({ tr("Name"), tr("Value") });
    m_tree->setRootIsDecorated(false);
    m_tree->setSelectionMode(QAbstractItemView::NoSelection);
    m_tree->setAlternatingRowColors(true);
    m_tree->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    layout->addWidget(m_tree);

    QTreeWidgetItem *group = addGroup(tr("Device"));
    new QTreeWidgetItem(group, { tr("Id"), u(device->get_param("id")) });
    new QTreeWidgetItem(group, { tr("Name"), u(device->get_param("name")) });
    new QTreeWidgetItem(group, { tr("Family"), u(device->family()) });
    new QTreeWidgetItem(group, { tr("Implementation"), u(device->impl()) });

    if (!device->deviceParams().empty())
    {
        group = addGroup(tr("Parameters"));
        for (const auto &p: device->deviceParams())
            new QTreeWidgetItem(group, { u(p.first), u(p.second) });
    }

    if (!device->links().empty())
    {
        group = addGroup(tr("Links"));
        auto target = [](const std::string &id)
        {
            IOBase *io = ListeRoom::Instance().get_input(id);
            if (!io)
                io = ListeRoom::Instance().get_output(id);
            if (!io)
                return tr("%1 (not found)").arg(u(id));
            return QStringLiteral("%1 (%2)").arg(u(io->get_param("name")), u(id));
        };

        for (const DeviceLink &l: device->links())
        {
            QString value = target(l.io);
            if (!l.commandIo.empty())
                value += tr(", commands: %1").arg(target(l.commandIo));
            new QTreeWidgetItem(group, { u(l.name), value });
        }
    }

    if (!device->ownedIos().empty())
    {
        group = addGroup(tr("Owned IOs"));
        for (IOBase *io: device->ownedIos())
            new QTreeWidgetItem(group, { u(io->get_param("name")),
                                         QStringLiteral("%1 (%2)").arg(u(io->get_param("type")), u(io->get_param("id"))) });
    }

    m_tree->expandAll();

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

QTreeWidgetItem *DialogDeviceInfo::addGroup(const QString &title)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(m_tree, { title });
    QFont f = group->font(0);
    f.setBold(true);
    group->setFont(0, f);
    group->setFirstColumnSpanned(true);
    return group;
}
