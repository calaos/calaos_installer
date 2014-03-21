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
                Input *input;

        public:
                QTreeWidgetItemInput(Input *r, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(type), input(r) {}
                QTreeWidgetItemInput(Input *r, const QStringList & strings, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(strings, type), input(r) {}
                QTreeWidgetItemInput(Input *r, QTreeWidget *parent, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, type), input(r) {}
                QTreeWidgetItemInput(Input *r, QTreeWidget *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, strings, type), input(r) {}
                QTreeWidgetItemInput(Input *r, QTreeWidget *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, preceding, type), input(r) {}
                QTreeWidgetItemInput(Input *r, QTreeWidgetItem *parent, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, type), input(r) {}
                QTreeWidgetItemInput(Input *r, QTreeWidgetItem *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, strings, type), input(r) {}
                QTreeWidgetItemInput(Input *r, QTreeWidgetItem *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, preceding, type), input(r) {}
                QTreeWidgetItemInput(const QTreeWidgetItemInput &other):
                                QTreeWidgetItem(other), input(other.input) {}

                Input *getInput() { return input; }
};

class QTreeWidgetItemOutput: public QTreeWidgetItem
{
        protected:
                Output *output;

        public:
                QTreeWidgetItemOutput(Output *r, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(type), output(r) {}
                QTreeWidgetItemOutput(Output *r, const QStringList & strings, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(strings, type), output(r) {}
                QTreeWidgetItemOutput(Output *r, QTreeWidget *parent, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, type), output(r) {}
                QTreeWidgetItemOutput(Output *r, QTreeWidget *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, strings, type), output(r) {}
                QTreeWidgetItemOutput(Output *r, QTreeWidget *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, preceding, type), output(r) {}
                QTreeWidgetItemOutput(Output *r, QTreeWidgetItem *parent, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, type), output(r) {}
                QTreeWidgetItemOutput(Output *r, QTreeWidgetItem *parent, const QStringList &strings, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, strings, type), output(r) {}
                QTreeWidgetItemOutput(Output *r, QTreeWidgetItem *parent, QTreeWidgetItem *preceding, int type = QTreeWidgetItem::UserType):
                                QTreeWidgetItem(parent, preceding, type), output(r) {}
                QTreeWidgetItemOutput(const QTreeWidgetItemOutput &other):
                                QTreeWidgetItem(other), output(other.output) {}

                Output *getOutput() { return output; }
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
