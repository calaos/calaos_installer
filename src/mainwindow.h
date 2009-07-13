#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QSignalMapper>
#include <QMessageBox>
#include <QTreeWidgetItem>

#include <projectmanager.h>

#include <dialognewroom.h>
#include <dialognewwago.h>
#include <dialognewvolet.h>
#include <dialognewdali.h>
#include <dialogioproperties.h>
#include <dialognewrule.h>

#include <conditiondelegate.h>
#include <actiondelegate.h>

#include <wagouploader.h>

#include <ListeRoom.h>
#include <ListeRule.h>
#include <Room.h>
#include <IOBase.h>

using namespace Utils;
using namespace Calaos;

namespace Ui
{
        class MainWindow;
}

enum { PAGE_PROG=0, PAGE_TRANSFERT, PAGE_ABOUT };
enum { ITEM_ROOM=0, ITEM_INTER, ITEM_LIGHT, ITEM_VOLET, ITEM_DALI,
       ITEM_DALIRGB, ITEM_TEMP, ITEM_CAMERA, ITEM_MUSIC, ITEM_INTERN,
       ITEM_TIME };

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

class MainWindow : public QMainWindow
{
        Q_OBJECT

        public:
                MainWindow(QWidget *parent = 0);
                ~MainWindow();

                void ShowPage(int page = PAGE_ABOUT);

                void PopulateRoomsTree();
                void PopulateRulesTree();

                QTreeWidgetItemRoom *addItemRoom(Room *room, bool selected = false);
                QTreeWidgetItemInput *addItemInput(Input *in, QTreeWidgetItemRoom *parent, bool selected = false);
                QTreeWidgetItemOutput *addItemOutput(Output *out, QTreeWidgetItemRoom *parent, bool selected = false);
                QTreeWidgetItemInput *addItemInput(Input *in, Room *parent, bool selected = false);
                QTreeWidgetItemOutput *addItemOutput(Output *out, Room *parent, bool selected = false);

                QTreeWidgetItemRule *addItemRule(Rule *rule, bool selected = false);
                QTreeWidgetItem *addItemCondition(Condition *condition, Input *input, bool selected = false);
                QTreeWidgetItem *addItemAction(Action *action, Output *output, bool selected = false);

                static void updateItemInfos(QTreeWidgetItemRoom *item);
                static void updateItemInfos(QTreeWidgetItemInput *item);
                static void updateItemInfos(QTreeWidgetItemOutput *item);
                static void updateItemInfos(QTreeWidgetItemRule *item);

                Rule *getCurrentRule();

                void Save(QString path = QString());
                void Load(QString path = QString());

        private:
                Ui::MainWindow *ui;

                Room *current_room; //Current used room in the interface
                WagoUploader wuploader;

                QMenu *add_menu;

                //Item used by right-click menu in tree
                QTreeWidgetItem *treeItem, *treeItem_condition, *treeItem_action;

                bool project_changed;
                QString project_path;


                void onShowProg();
                void onShowTransfert();

        public slots:
                void addCalaosItem(int item);
                void showPopup_tree(const QPoint point);
                void showPopup_condition(const QPoint point);
                void showPopup_action(const QPoint point);
                void deleteItem();
                void deleteItemCondition();
                void deleteItemAction();
                void goSelectRule();
                void showPropertiesItem();
                void wagoStatusProgress(int status);

                void itemLightOn();
                void itemLightOff();
                void itemVoletUp();
                void itemVoletDown();
                void itemVoletStop();

        private slots:
                void on_button_wagostop_clicked();
                void on_button_wagostart_clicked();
                void on_bt_rules_del_clicked();
                void on_tree_rules_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
                void on_bt_rules_add_clicked();
                void on_actionCharger_un_projet_triggered();
                void on_actionSauvegarder_triggered();
                void on_actionNouveau_projet_triggered();
                void on_tree_home_itemDoubleClicked(QTreeWidgetItem* item, int column);
                void on_actionSauvegarder_un_projet_triggered();
                void on_tree_home_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
                void on_Pages_currentChanged(int );
                void on_actionA_propos_de_Qt_triggered();
                void on_mainmenu_list_currentRowChanged(int currentRow);
                void on_actionAbout_activated();
                void on_actionQuit_activated();
};

#endif // MAINWINDOW_H
