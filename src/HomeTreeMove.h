#ifndef HOMETREEMOVE_H
#define HOMETREEMOVE_H

#include <QTreeWidget>

#include <IOBase.h>
#include <Room.h>

//The home tree shows an input+output IO twice, so a move must relocate every
//item of the IO, not only the one the user grabbed.
bool moveIOInHome(QTreeWidget *tree, Calaos::IOBase *io, Calaos::Room *to);

bool inputHasTreeItem(Calaos::IOBase *in);

#endif // HOMETREEMOVE_H
