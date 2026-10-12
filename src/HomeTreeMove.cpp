#include "HomeTreeMove.h"
#include "qtreewidget_addition.h"

#include <ListeRoom.h>
#include <algorithm>

using namespace Calaos;

bool inputHasTreeItem(IOBase *in)
{
    const std::string type = in->get_gui_type();
    return type == "switch" ||
           type == "switch3" ||
           type == "switch_long" ||
           type == "time" ||
           type == "time_range" ||
           type == "temp" ||
           type == "analog_in" ||
           type == "string_in";
}

bool moveIOInHome(QTreeWidget *tree, IOBase *io, Room *to)
{
    //Taken before the move: a device's owned IOs follow it.
    const std::vector<IOBase *> ios = ListeRoom::iosDeletedWith(io);

    if (!ListeRoom::Instance().moveIO(io, to))
        return false;

    QTreeWidgetItem *target = nullptr;
    QList<QTreeWidgetItem *> items;
    for (QTreeWidgetItemIterator it(tree);*it;++it)
    {
        if (QTreeWidgetItemRoom *room = dynamic_cast<QTreeWidgetItemRoom *>(*it))
        {
            if (room->getRoom() == to)
                target = room;
            continue;
        }

        IOBase *item = nullptr;
        if (QTreeWidgetItemInput *in = dynamic_cast<QTreeWidgetItemInput *>(*it))
            item = in->getInput();
        else if (QTreeWidgetItemOutput *out = dynamic_cast<QTreeWidgetItemOutput *>(*it))
            item = out->getOutput();

        //Only room children: anything nested travels with its parent item.
        if (item && std::find(ios.begin(), ios.end(), item) != ios.end() &&
            dynamic_cast<QTreeWidgetItemRoom *>((*it)->parent()))
            items.append(*it);
    }

    if (!target)
        return true;

    //Re-parented rather than rebuilt: the items keep their labels and icons.
    QTreeWidgetItem *current = tree->currentItem();
    for (QTreeWidgetItem *item: items)
    {
        item->parent()->removeChild(item);
        target->addChild(item);
    }
    if (current)
        tree->setCurrentItem(current);

    return true;
}
