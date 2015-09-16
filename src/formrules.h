#ifndef FORMRULES_H
#define FORMRULES_H

#include <QtWidgets>

#include <dialognewroom.h>
#include <dialognewwago.h>
#include <dialognewvolet.h>
#include <dialognewdali.h>
#include <dialognewdalirgb.h>
#include <dialognewtemp.h>
#include <dialognewcamera.h>
#include <dialognewaudio.h>
#include <dialognewinternal.h>
#include <dialognewscenario.h>
#include <dialognewtime.h>
#include <dialogcameraview.h>
#include <dialogioproperties.h>
#include <DialogListProperties.h>
#include <dialognewrule.h>
#include <dialognewanalog.h>
#include <dialognewwebio.h>

#include <TempWizard.h>

#include <FormConditionStd.h>
#include <FormConditionStart.h>
#include <FormConditionScript.h>
#include <FormActionStd.h>
#include <FormActionMail.h>
#include <FormActionScript.h>
#include <FormActionTouchscreen.h>

#include <qtreewidget_addition.h>

#include <ListeRoom.h>
#include <ListeRule.h>
#include <Room.h>
#include <IOBase.h>

enum { HW_NONE=0, HW_WAGO, HW_ONEWIRE, HW_X10, HW_GPIO,
       HW_ZIBASE, HW_WEB, HW_MYSENSORS, HW_OLA, HW_LAN };

enum { ITEM_ROOM=0, ITEM_INPUT_SWITCH, ITEM_LIGHT, ITEM_SHUTTER, ITEM_DALI,
       ITEM_LIGHT_RGB, ITEM_TEMP, ITEM_CAMERA, ITEM_MUSIC, ITEM_INTERN,
       ITEM_SCENARIO, ITEM_TIME, ITEM_ANALOG, ITEM_AVR, ITEM_STRINGIN, ITEM_STRINGOUT,
       ITEM_ANALOG_IN, ITEM_ANALOG_OUT, ITEM_BOOL_OUT
     };

namespace Ui
{
class FormRules;
}

class FormRules : public QWidget
{
    Q_OBJECT

public:
    explicit FormRules(QWidget *parent = 0);
    ~FormRules();

    void PopulateRoomsTree();
    void PopulateRulesTree();

    QTreeWidgetItemRoom *addItemRoom(Room *room, bool selected = false);
    QTreeWidgetItemInput *addItemInput(IOBase *in, QTreeWidgetItemRoom *parent, bool selected = false);
    QTreeWidgetItemOutput *addItemOutput(IOBase *out, QTreeWidgetItemRoom *parent, bool selected = false);
    QTreeWidgetItemInput *addItemInput(IOBase *in, Room *parent, bool selected = false);
    QTreeWidgetItemOutput *addItemOutput(IOBase *out, Room *parent, bool selected = false);

    QTreeWidgetItemRule *addItemRule(Rule *rule, bool selected = false);
    QTreeWidgetItem *addItemCondition(Condition *condition, bool selected = false, bool show_popup = false);
    QTreeWidgetItem *addItemAction(Action *action, bool selected = false, bool show_popup = false);

    static void updateItemInfos(QTreeWidgetItemRoom *item);
    static void updateItemInfos(QTreeWidgetItemInput *item);
    static void updateItemInfos(QTreeWidgetItemOutput *item);
    static void updateItemInfos(QTreeWidgetItemRule *item);
    static void updateItemCondition(QTreeWidgetItem *item, Condition *condition);
    static void updateItemAction(QTreeWidgetItem *item, Action *action);

    Rule *getCurrentRule();

    bool projectChanged() { return project_changed; }

    static QString getIconFromRoom(Room *room);

    IOBase *addCalaosItemInputSwitch(int item, int hw_type);
    IOBase *addCalaosItemLight(int item, int hw_type);
    IOBase *addCalaosItemString(int item, int hw_type);
    IOBase *addCalaosItemShutter(int item, int hw_type);
    IOBase *addCalaosItemDimmer(int item, int hw_type);
    IOBase *addCalaosItemRGB(int item, int hw_type);
    IOBase *addCalaosItemTemp(int item, int hw_type);
    IOBase *addCalaosItemCamera(int item, int hw_type);
    IOBase *addCalaosItemAudio(int item, int hw_type);
    IOBase *addCalaosItemAVR(int item, int hw_type);
    IOBase *addCalaosItemInternal(int item, int hw_type);
    IOBase *addCalaosItemAnalog(int item, int hw_type);

    //new version
    void addCalaosIO(Params &params);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FormRules *ui;

    Room *current_room; //Current used room in the interface

    QMenu *add_menu, *addConditionMenu, *addActionMenu;

    //Item used by right-click menu in tree
    QTreeWidgetItem *treeItem, *treeItem_condition, *treeItem_action;

    bool project_changed;

    FormConditionStd *popupConditionStd;
    FormConditionStart *popupConditionStart;
    FormConditionScript *popupConditionScript;

    FormActionStd *popupActionStd;
    FormActionMail *popupActionMail;
    FormActionScript *popupActionScript;
    FormActionTouchscreen *popupActionTouchscreen;

signals:
    void projectModified(bool modified);


public slots:
    void addCalaosItem(int hw_type, int item);
    void showPopup_tree(const QPoint point);
    void showPopup_condition(const QPoint point);
    void showPopup_action(const QPoint point);
    void showPopup_rule(const QPoint point);
    void deleteItem();
    void deleteItemCondition();
    void deleteItemAction();
    void goSelectRule();
    void showPropertiesItem();
    void cloneRule();
    void addCondition(int type);
    void addAction(int type);

    void itemLightOn();
    void itemLightOff();
    void itemVoletUp();
    void itemVoletDown();
    void itemVoletStop();
    void itemShowCamera();
    void itemPlagesHoraires();
    void itemConvertInterTriple();
    void itemConvertInterBP();
    void itemConvertInterLong();
    void itemConvertShutterStandard();
    void itemConvertShutterSmart();
    void itemTempWizard();

    void ClearProject();
    void setProjectModified(bool modified);

    void moveIOToRoom();

private slots:
    void on_bt_action_down_clicked();
    void on_bt_action_up_clicked();
    void on_bt_condition_down_clicked();
    void on_bt_condition_up_clicked();
    void on_tree_rules_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_bt_action_del_clicked();
    void on_bt_condition_del_clicked();
    void on_tree_action_itemClicked(QTreeWidgetItem* item, int column);
    void on_tree_condition_itemClicked(QTreeWidgetItem* item, int column);
    void on_filterEditRules_textChanged(QString );
    void on_filterEditHome_textChanged(QString );
    void on_bt_rules_del_clicked();
    void on_tree_rules_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void on_bt_rules_add_clicked();
    void on_tree_home_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_tree_home_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
};

#endif // FORMRULES_H
