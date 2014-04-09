#ifndef QTREEWIDGET_ADDITION_H
#define QTREEWIDGET_ADDITION_H

#include <QtWidgets>
#include "Room.h"
#include "Rule.h"
#include "IOBase.h"

using namespace Calaos;

class QTreeWidgetItemRoom: public QTreeWidgetItem
{
protected:
    Room *room;

public:
    QTreeWidgetItemRoom(Room *r, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(type), room(r) {}
    QTreeWidgetItemRoom(Room *r, const QStringList & strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(strings, type), room(r) {}
    QTreeWidgetItemRoom(Room *r, QTreeWidget *parent, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, type), room(r) {}
    QTreeWidgetItemRoom(Room *r, QTreeWidget *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, strings, type), room(r) {}
    QTreeWidgetItemRoom(Room *r, QTreeWidget *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, preceding, type), room(r) {}
    QTreeWidgetItemRoom(Room *r, QTreeWidgetItem *parent, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, type), room(r) {}
    QTreeWidgetItemRoom(Room *r, QTreeWidgetItem *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, strings, type), room(r) {}
    QTreeWidgetItemRoom(Room *r, QTreeWidgetItem *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, preceding, type), room(r) {}
    QTreeWidgetItemRoom(const QTreeWidgetItemRoom &other):
        QTreeWidgetItem(other), room(other.room) {}

    Room *getRoom() { return room; }
};

class QTreeWidgetItemInput: public QTreeWidgetItem
{
protected:
    IOBase *input;

public:
    QTreeWidgetItemInput(IOBase *r, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(type), input(r) {}
    QTreeWidgetItemInput(IOBase *r, const QStringList & strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(strings, type), input(r) {}
    QTreeWidgetItemInput(IOBase *r, QTreeWidget *parent, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, type), input(r) {}
    QTreeWidgetItemInput(IOBase *r, QTreeWidget *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, strings, type), input(r) {}
    QTreeWidgetItemInput(IOBase *r, QTreeWidget *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, preceding, type), input(r) {}
    QTreeWidgetItemInput(IOBase *r, QTreeWidgetItem *parent, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, type), input(r) {}
    QTreeWidgetItemInput(IOBase *r, QTreeWidgetItem *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, strings, type), input(r) {}
    QTreeWidgetItemInput(IOBase *r, QTreeWidgetItem *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, preceding, type), input(r) {}
    QTreeWidgetItemInput(const QTreeWidgetItemInput &other):
        QTreeWidgetItem(other), input(other.input) {}

    IOBase *getInput() { return input; }
};

class QTreeWidgetItemOutput: public QTreeWidgetItem
{
protected:
    IOBase *output;

public:
    QTreeWidgetItemOutput(IOBase *r, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(type), output(r) {}
    QTreeWidgetItemOutput(IOBase *r, const QStringList & strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(strings, type), output(r) {}
    QTreeWidgetItemOutput(IOBase *r, QTreeWidget *parent, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, type), output(r) {}
    QTreeWidgetItemOutput(IOBase *r, QTreeWidget *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, strings, type), output(r) {}
    QTreeWidgetItemOutput(IOBase *r, QTreeWidget *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, preceding, type), output(r) {}
    QTreeWidgetItemOutput(IOBase *r, QTreeWidgetItem *parent, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, type), output(r) {}
    QTreeWidgetItemOutput(IOBase *r, QTreeWidgetItem *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, strings, type), output(r) {}
    QTreeWidgetItemOutput(IOBase *r, QTreeWidgetItem *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, preceding, type), output(r) {}
    QTreeWidgetItemOutput(const QTreeWidgetItemOutput &other):
        QTreeWidgetItem(other), output(other.output) {}

    IOBase *getOutput() { return output; }
};

class QTreeWidgetItemRule: public QTreeWidgetItem
{
protected:
    Rule *rule;

public:
    QTreeWidgetItemRule(Rule *r, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(type), rule(r) {}
    QTreeWidgetItemRule(Rule *r, const QStringList & strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(strings, type), rule(r) {}
    QTreeWidgetItemRule(Rule *r, QTreeWidget *parent, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, type), rule(r) {}
    QTreeWidgetItemRule(Rule *r, QTreeWidget *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, strings, type), rule(r) {}
    QTreeWidgetItemRule(Rule *r, QTreeWidget *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, preceding, type), rule(r) {}
    QTreeWidgetItemRule(Rule *r, QTreeWidgetItem *parent, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, type), rule(r) {}
    QTreeWidgetItemRule(Rule *r, QTreeWidgetItem *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, strings, type), rule(r) {}
    QTreeWidgetItemRule(Rule *r, QTreeWidgetItem *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
        QTreeWidgetItem(parent, preceding, type), rule(r) {}
    QTreeWidgetItemRule(const QTreeWidgetItemRule &other):
        QTreeWidgetItem(other), rule(other.rule) {}

    Rule *getRule() { return rule; }
};

#endif // QTREEWIDGET_ADDITION_H
