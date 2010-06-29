#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QSignalMapper>
#include <QMessageBox>
#include <QTreeWidgetItem>

#include "textedit.h"

#include <projectmanager.h>

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
#include <dialognewrule.h>
#include <dialogconnect.h>
#include <dialogopenonline.h>
#include <dialogsaveonline.h>
#include <dialogioplagehoraire.h>

#include <conditiondelegate.h>
#include <actiondelegate.h>

#include <qtreewidget_addition.h>

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

enum { PAGE_PROG=0, PAGE_DALI, PAGE_ABOUT };
enum { ITEM_ROOM=0, ITEM_INTER, ITEM_LIGHT, ITEM_VOLET, ITEM_DALI,
       ITEM_DALIRGB, ITEM_TEMP, ITEM_CAMERA, ITEM_MUSIC, ITEM_INTERN,
       ITEM_SCENARIO, ITEM_TIME };

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

                QMessageBox messageBox;
                QLabel *statusConnectIcon;
                QLabel *statusConnectText;
                QProgressBar *progressBar;
                QPushButton *buttonStopProcess;

                QDir tempDir;

                TextEdit teditor;

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
                void itemShowCamera();
                void itemPlagesHoraires();
                void itemConvertInterTriple();
                void itemConvertInterBP();
                void itemConvertVoletStandard();
                void itemConvertVoletSmart();

                void wagoConnected(QString &ip, bool proxy);
                void wagoDisconnected();
                void wagoUpdateNeeded(QString &version);
                void wagoError(int error);

        private slots:
                void on_actionG_n_rer_fichier_d_impression_triggered();
                void on_actionDALI_triggered();
                void on_pushButton_clicked();
                void on_actionSauvegarder_un_projet_en_ligne_triggered();
                void on_actionOuvrir_un_projet_en_ligne_triggered();
                void on_actionSe_d_connecter_triggered();
                void on_actionProgrammer_l_automate_triggered();
                void on_actionSe_connecter_triggered();
                void button_wagostop_clicked();
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

                void on_closeDaliForm_clicked();
};

#endif // MAINWINDOW_H
