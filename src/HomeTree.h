#ifndef HOMETREE_H
#define HOMETREE_H

#include <QTreeWidget>
#include <functional>
#include <vector>

#include <IOBase.h>
#include <Room.h>

#include "qtreewidget_addition.h"

//Shape of the home tree: room -> free IOs and devices -> a device's owned IOs.

//Nearest room above item, or the item itself; nullptr for anything else.
QTreeWidgetItemRoom *roomItemOf(QTreeWidgetItem *item);
//The IO an input, output or device item stands for.
Calaos::IOBase *ioOfItem(QTreeWidgetItem *item);

//Dimmed unknown-type icon and a tooltip line for an IO whose type the server dropped.
//Call after the item's own icon and tooltip are set; a no-op for supported IOs.
void markUnsupportedByServer(QTreeWidgetItem *item);

//Builds the children of a room item. `created` runs on every new item, parents first.
void populateRoomItems(QTreeWidgetItemRoom *roomItem,
                       const std::function<void(QTreeWidgetItem *)> &created = nullptr);

//Item of a device added after its room's items were built, with its owned IOs
//laid out as populateRoomItems() lays them out.
QTreeWidgetItemDevice *addDeviceItem(QTreeWidgetItemRoom *roomItem, Calaos::Device *dev,
                                     const std::function<void(QTreeWidgetItem *)> &created = nullptr);

//Deletes every item of these IOs. An owned IO's item is a child of its device's:
//only the topmost ones are deleted, their children go with them.
void removeIOItems(QTreeWidget *tree, const std::vector<Calaos::IOBase *> &ios);

//Model deletion (rules and a device's owned IOs included) followed by the matching items.
void deleteIOInHome(QTreeWidget *tree, Calaos::IOBase *io);

#endif // HOMETREE_H
