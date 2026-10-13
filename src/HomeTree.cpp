#include "HomeTree.h"
#include "HomeTreeMove.h"

#include <ListeRoom.h>
#include <QSet>
#include <QIcon>
#include <QColor>
#include <QCoreApplication>
#include <algorithm>
#include <map>

using namespace Calaos;

QTreeWidgetItemRoom *roomItemOf(QTreeWidgetItem *item)
{
    while (item)
    {
        if (QTreeWidgetItemRoom *room = dynamic_cast<QTreeWidgetItemRoom *>(item))
            return room;
        item = item->parent();
    }

    return nullptr;
}

IOBase *ioOfItem(QTreeWidgetItem *item)
{
    if (QTreeWidgetItemInput *in = dynamic_cast<QTreeWidgetItemInput *>(item))
        return in->getInput();
    if (QTreeWidgetItemOutput *out = dynamic_cast<QTreeWidgetItemOutput *>(item))
        return out->getOutput();

    return nullptr;
}

void markUnsupportedByServer(QTreeWidgetItem *item)
{
    IOBase *io = ioOfItem(item);
    if (!io || io->isSupportedByServer())
        return;

    item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_unkown.png"));
    item->setData(0, Qt::ForegroundRole, QColor(Qt::gray));

    const QString note = QCoreApplication::translate("HomeTree", "Not supported by the server: kept in the file but it will not run");
    const QString tip = item->data(0, Qt::ToolTipRole).toString() + "\n" + note;
    item->setData(0, Qt::ToolTipRole, tip);
    item->setData(0, Qt::StatusTipRole, tip);
}

void populateRoomItems(QTreeWidgetItemRoom *roomItem, const std::function<void(QTreeWidgetItem *)> &created)
{
    Room *room = roomItem->getRoom();
    auto done = [&](QTreeWidgetItem *item)
    {
        if (created)
            created(item);
    };

    //Owned IOs wait for their device's item, which stands where the device is listed.
    std::vector<std::pair<IOBase *, bool>> owned;
    std::map<Device *, QTreeWidgetItemDevice *> devices;

    auto addItem = [&](IOBase *io, bool asInput, QTreeWidgetItem *parent)
    {
        QTreeWidgetItem *item;
        if (asInput)
            item = new QTreeWidgetItemInput(io, parent);
        else
            item = new QTreeWidgetItemOutput(io, parent);
        done(item);
    };

    //Inputs first, like the outputs after them: the order the tree always had.
    for (int j = 0;j < room->get_size_in();j++)
    {
        IOBase *in = room->get_input(j);
        if (dynamic_cast<Device *>(in) || !inputHasTreeItem(in))
            continue;

        if (in->getOwnerDevice())
            owned.push_back({ in, true });
        else
            addItem(in, true, roomItem);
    }

    for (int j = 0;j < room->get_size_out();j++)
    {
        IOBase *out = room->get_output(j);
        if (Device *dev = dynamic_cast<Device *>(out))
        {
            QTreeWidgetItemDevice *item = new QTreeWidgetItemDevice(dev, roomItem);
            devices[dev] = item;
            done(item);
        }
        else if (out->getOwnerDevice())
            owned.push_back({ out, false });
        else
            addItem(out, false, roomItem);
    }

    for (const auto &entry: owned)
    {
        auto dev = devices.find(entry.first->getOwnerDevice());
        addItem(entry.first, entry.second, dev != devices.end() ? static_cast<QTreeWidgetItem *>(dev->second) : roomItem);
    }
}

void removeIOItems(QTreeWidget *tree, const std::vector<IOBase *> &ios)
{
    QSet<QTreeWidgetItem *> matching;
    for (QTreeWidgetItemIterator it(tree);*it;++it)
    {
        IOBase *io = ioOfItem(*it);
        if (io && std::find(ios.begin(), ios.end(), io) != ios.end())
            matching.insert(*it);
    }

    QList<QTreeWidgetItem *> topmost;
    for (QTreeWidgetItem *item: matching)
    {
        bool nested = false;
        for (QTreeWidgetItem *p = item->parent();p && !nested;p = p->parent())
            nested = matching.contains(p);
        if (!nested)
            topmost.append(item);
    }

    qDeleteAll(topmost);
}

void deleteIOInHome(QTreeWidget *tree, IOBase *io)
{
    const std::vector<IOBase *> gone = ListeRoom::iosDeletedWith(io);

    if (io->is_input())
        ListeRoom::Instance().deleteIOInput(io);
    else
        ListeRoom::Instance().deleteIOOutput(io);

    removeIOItems(tree, gone);
}
