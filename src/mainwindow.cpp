#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow), current_room(NULL),
    wuploader(parent),
    project_changed(false),
    messageBox(QMessageBox::Information, tr("Calaos Installer"), "")
{
        ui->setupUi(this);

        ShowPage(PAGE_PROG);

        add_menu = new QMenu(parent);
        ui->ioexplorer_add->setMenu(add_menu);

        QAction *action = NULL;
        QSignalMapper *sig = new QSignalMapper(this);

        action = add_menu->addAction(QString::fromUtf8("Pièce"));
        action->setIcon(QIcon(":/img/rooms/various_small.png"));
        sig->setMapping(action, ITEM_ROOM);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        add_menu->addSeparator();

        action = add_menu->addAction(QString::fromUtf8("Interrupteur"));
        action->setIcon(QIcon(":/img/icon_inter.png"));
        sig->setMapping(action, ITEM_INTER);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        action = add_menu->addAction(QString::fromUtf8("Lumière / Prise commandée"));
        action->setIcon(QIcon(":/img/icon_light_on.png"));
        sig->setMapping(action, ITEM_LIGHT);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        action = add_menu->addAction(QString::fromUtf8("Volet"));
        action->setIcon(QIcon(":/img/icon_shutter.png"));
        sig->setMapping(action, ITEM_VOLET);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        action = add_menu->addAction(QString::fromUtf8("DALI"));
        action->setIcon(QIcon(":/img/icon_light_on.png"));
        sig->setMapping(action, ITEM_DALI);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        action = add_menu->addAction(QString::fromUtf8("DALI RGB"));
        action->setIcon(QIcon(":/img/icon_light_on.png"));
        sig->setMapping(action, ITEM_DALIRGB);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        action = add_menu->addAction(QString::fromUtf8("Sonde de température"));
        action->setIcon(QIcon(":/img/temp.png"));
        sig->setMapping(action, ITEM_TEMP);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        add_menu->addSeparator();

        action = add_menu->addAction(QString::fromUtf8("Caméra"));
        action->setIcon(QIcon(":/img/icon_camera_on.png"));
        sig->setMapping(action, ITEM_CAMERA);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        action = add_menu->addAction(QString::fromUtf8("Lecteur de musique"));
        action->setIcon(QIcon(":/img/icon_sound.png"));
        sig->setMapping(action, ITEM_MUSIC);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        add_menu->addSeparator();

        action = add_menu->addAction(QString::fromUtf8("Variable Interne"));
        action->setIcon(QIcon(":/img/text.png"));
        sig->setMapping(action, ITEM_INTERN);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        action = add_menu->addAction(QString::fromUtf8("Variable Scénario"));
        action->setIcon(QIcon(":/img/icon_scenario.png"));
        sig->setMapping(action, ITEM_SCENARIO);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        action = add_menu->addAction(QString::fromUtf8("Variable Horaire"));
        action->setIcon(QIcon(":/img/icon_clock.png"));
        sig->setMapping(action, ITEM_TIME);
        connect(action, SIGNAL(triggered()), sig, SLOT(map()));

        //connect the mapper
        connect(sig, SIGNAL(mapped(int)), this, SLOT(addCalaosItem(int)));
        connect(ui->tree_home, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_tree(QPoint)));
        connect(ui->tree_condition, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_condition(QPoint)));
        connect(ui->tree_action, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_action(QPoint)));

        //Add toolbar
        ui->toolBar->addAction(ui->actionNouveau_projet);
        ui->toolBar->addAction(ui->actionCharger_un_projet);
        ui->toolBar->addAction(ui->actionSauvegarder);
        ui->toolBar->addSeparator();
        ui->toolBar->addAction(ui->actionSe_connecter);
        ui->toolBar->addAction(ui->actionSe_d_connecter);
        ui->toolBar->addSeparator();
        ui->toolBar->addAction(ui->actionQuit);

        ui->label_current_room->setText("<aucun>");

        //WagoConnect
        connect(&WagoConnect::Instance(), SIGNAL(connected(QString&,bool)), this, SLOT(wagoConnected(QString&,bool)));
        connect(&WagoConnect::Instance(), SIGNAL(disconnected()), this, SLOT(wagoDisconnected()));
        connect(&WagoConnect::Instance(), SIGNAL(updateNeeded(QString&)), this, SLOT(wagoUpdateNeeded(QString&)));
        connect(&WagoConnect::Instance(), SIGNAL(error(int)), this, SLOT(wagoError(int)));

        buttonStopProcess = new QPushButton(QIcon(":/img/process-stop_16x16.png"), "", this);
        statusBar()->addPermanentWidget(buttonStopProcess);
        buttonStopProcess->hide();

        progressBar = new QProgressBar(this);
        statusBar()->addPermanentWidget(progressBar);
        progressBar->hide();

        statusConnectText = new QLabel(QString::fromUtf8("Déconnecté."), this);
        statusBar()->addPermanentWidget(statusConnectText);

        statusConnectIcon = new QLabel(this);
        statusConnectIcon->setPixmap(QPixmap(":/img/user-invisible_16x16.png"));
        statusBar()->addPermanentWidget(statusConnectIcon);

        connect(&wuploader, SIGNAL(progressUpdate(int)), progressBar, SLOT(setValue(int)));
        connect(&wuploader, SIGNAL(statusUpdate(int)), this, SLOT(wagoStatusProgress(int)));

        connect(ui->widgetDali, SIGNAL(closeDaliForm()), this, SLOT(on_closeDaliForm_clicked()));

        /* Search and create a new temp dir */
        tempDir.setPath(QDir::tempPath());
        int cpt = 0;

        do
        {
                tempDir.setPath(QDir::tempPath() + "/calaos_installer_" + QString::number(cpt));
                cpt++;
        } while (tempDir.exists());

        tempDir.mkdir(tempDir.path());

        const QStringList args = QCoreApplication::arguments();
        if(args.count() > 1)
        {
                QString path = args[1];

                Load(path);
        }
        else
        {
                on_actionNouveau_projet_triggered();
        }
}

MainWindow::~MainWindow()
{
        tempDir.remove("io.xml");
        tempDir.remove("rules.xml");
        tempDir.rmdir(tempDir.path());

        delete ui;
}

void MainWindow::PopulateRoomsTree()
{
        QStringList headers;
        headers << QString::fromUtf8("Maison");
        ui->tree_home->setHeaderLabels(headers);

        for (int i = 0;i < ListeRoom::Instance().size();i++)
        {
                Room *room = ListeRoom::Instance().get_room(i);
                bool selected = false;
                if (i == 0) selected = true;
                QTreeWidgetItemRoom *iroom = addItemRoom(room, selected);

                for (int j = 0;j < room->get_size_in();j++)
                {
                        Input *in = room->get_input(j);

                        if (in->get_param("type") == "WIDigitalBP" ||
                            in->get_param("type") == "WIDigitalTriple" ||
                            in->get_param("type") == "InPlageHoraire" ||
                            in->get_param("type") == "InputTime" ||
                            in->get_param("type") == "WITemp")
                        {
                                addItemInput(in, iroom);
                        }

                        QCoreApplication::processEvents();
                }

                for (int j = 0;j < room->get_size_out();j++)
                {
                        Output *out = room->get_output(j);
                        addItemOutput(out, iroom);

                        QCoreApplication::processEvents();
                }
        }

        ui->tree_home->expandAll();
}

void MainWindow::PopulateRulesTree()
{
        QStringList headers;
        headers << QString::fromUtf8("Type") << QString::fromUtf8("Nom");
        ui->tree_rules->setHeaderLabels(headers);

        for (int i = 0;i < ListeRule::Instance().size();i++)
        {
                Rule *rule = ListeRule::Instance().get_rule(i);
                bool selected = false;
                if (i == 0) selected = true;
                addItemRule(rule, selected);

                QCoreApplication::processEvents();
        }
}

void MainWindow::ShowPage(int page)
{
        ui->Pages->setCurrentIndex(page);
}

void MainWindow::on_actionQuit_activated()
{
        close();
}

void MainWindow::on_actionAbout_activated()
{
        ShowPage();
}

void MainWindow::on_mainmenu_list_currentRowChanged(int currentRow)
{
        ShowPage(currentRow);
}

void MainWindow::on_actionA_propos_de_Qt_triggered()
{
        qApp->aboutQt();
}

void MainWindow::on_Pages_currentChanged(int page)
{
        if (page == PAGE_PROG)
                onShowProg();
        else if (page == PAGE_DALI)
                onShowTransfert();
}

void MainWindow::onShowProg()
{

}

void MainWindow::onShowTransfert()
{

}

void MainWindow::addCalaosItem(int item)
{
        //some tests.
        if (current_room == NULL && item != ITEM_ROOM)
        {
                if (ListeRoom::Instance().size() <= 0)
                {
                        QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Vous devez créer au moins une pièce !"));
                        return;
                }
                else
                {
                        QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Vous devez sélectionner une pièce pour ajouter des éléments !"));
                        return;
                }
        }

        switch (item)
        {
          case ITEM_ROOM:
                {
                        DialogNewRoom dialog;
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                current_room = ListeRoom::Instance().get_room(ListeRoom::Instance().size() - 1);
                                ui->label_current_room->setText(QString::fromUtf8(current_room->get_name().c_str()));

                                //Add the new room to the main list
                                Room *room = ListeRoom::Instance().get_room(ListeRoom::Instance().size() - 1);
                                addItemRoom(room, true);

                                ui->tree_home->expandAll();
                        }
                }
                break;

          case ITEM_INTER:
                {
                        bool another;

                        do
                        {
                                DialogNewWago dialog(item, current_room);
                                if (dialog.exec() == QDialog::Accepted)
                                {
                                        another = dialog.wantAnother();

                                        Input *input = dialog.getInput();
                                        if (input)
                                                addItemInput(input, current_room, true);
                                        else
                                        {
                                                QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                                                another = false;
                                        }
                                }
                                else
                                        another = false;

                        } while (another);
                }
                break;

          case ITEM_LIGHT:
                {
                        bool another;
                        do
                        {
                                DialogNewWago dialog(item, current_room);
                                if (dialog.exec() == QDialog::Accepted)
                                {
                                        another = dialog.wantAnother();

                                        Output *output = dialog.getOutput();
                                        if (output)
                                                addItemOutput(output, current_room, true);
                                        else
                                        {
                                                QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                                                another = false;
                                        }
                                }
                                else
                                        another = false;
                        }while (another);
                }
                break;
          case ITEM_VOLET:
                {
                        DialogNewVolet dialog(current_room);
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                Output *output = dialog.getOutput();
                                if (output)
                                        addItemOutput(output, current_room, true);
                                else
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                        }
                }
                break;
          case ITEM_DALI:
                {
                        DialogNewDali dialog(current_room);
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                Output *output = dialog.getOutput();
                                if (output)
                                        addItemOutput(output, current_room, true);
                                else
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                        }
                }
                break;
          case ITEM_DALIRGB:
                {
                        DialogNewDaliRGB dialog(current_room);
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                Output *output = dialog.getOutput();
                                if (output)
                                        addItemOutput(output, current_room, true);
                                else
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                        }
                }
                break;
          case ITEM_TEMP:
                {
                        DialogNewTemp dialog(current_room);
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                Input *input = dialog.getInput();
                                if (input)
                                        addItemInput(input, current_room, true);
                                else
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                        }
                }
                break;
          case ITEM_CAMERA:
                {
                        DialogNewCamera dialog(current_room);
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                Output *output = dialog.getOutput();
                                if (output)
                                        addItemOutput(output, current_room, true);
                                else
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                        }
                }
                break;
          case ITEM_MUSIC:
                {
                        DialogNewAudio dialog(current_room);
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                Output *output = dialog.getOutput();
                                if (output)
                                        addItemOutput(output, current_room, true);
                                else
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                        }
                }
                break;
          case ITEM_INTERN:
                {
                        DialogNewInternal dialog(current_room);
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                Output *output = dialog.getOutput();
                                if (output)
                                        addItemOutput(output, current_room, true);
                                else
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                        }
                }
                break;
          case ITEM_SCENARIO:
                {
                        DialogNewScenario dialog(current_room);
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                Output *output = dialog.getOutput();
                                if (output)
                                        addItemOutput(output, current_room, true);
                                else
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                        }
                }
                break;
          case ITEM_TIME:
                {
                        DialogNewTime dialog(current_room);
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                Input *input = dialog.getInput();
                                if (input)
                                {
                                        if (input->get_param("type") == "InputTimer")
                                                addItemOutput(dynamic_cast<Output *>(input), current_room, true);
                                        else
                                                addItemInput(input, current_room, true);
                                }
                                else
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                        }
                }
                break;

          default:
                QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Type d'elément (%1) inconnu!").arg(item));
        }
}

QTreeWidgetItemRoom *MainWindow::addItemRoom(Room *room, bool selected)
{
        QTreeWidgetItemRoom *item = new QTreeWidgetItemRoom(room, ui->tree_home);

        if (selected) ui->tree_home->setCurrentItem(item);

        updateItemInfos(item);

        return item;
}

void MainWindow::updateItemInfos(QTreeWidgetItemRoom *item)
{
        Room *room = item->getRoom();

        item->setData(0, Qt::DisplayRole, QString::fromUtf8(room->get_name().c_str()));

        if (room->get_type() == "salon" || room->get_type() == "lounge")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/lounge_small.png"));
        else if (room->get_type() == "sdb" || room->get_type() == "bathroom")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/bathroom_small.png"));
        else if (room->get_type() == "chambre" || room->get_type() == "bedroom")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/bedroom_small.png"));
        else if (room->get_type() == "cave" || room->get_type() == "cellar")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/cellar_small.png"));
        else if (room->get_type() == "couloir" || room->get_type() == "hall" || room->get_type() == "corridor")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/corridor_small.png"));
        else if (room->get_type() == "sam" || room->get_type() == "diningroom")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/diningroom_small.png"));
        else if (room->get_type() == "garage")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/garage_small.png"));
        else if (room->get_type() == "cuisine" || room->get_type() == "kitchen")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/kitchen_small.png"));
        else if (room->get_type() == "bureau" || room->get_type() == "office")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/office_small.png"));
        else if (room->get_type() == "exterieur" || room->get_type() == "outside")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/outside_small.png"));
        else if (room->get_type() == "misc" || room->get_type() == "divers" || room->get_type() == "various")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/various_small.png"));
        else
                item->setData(0, Qt::DecorationRole, QIcon(":/img/rooms/various_small.png"));

        QString s = QString::fromUtf8(room->get_name().c_str());
        s += " (" + QString::fromUtf8(room->get_type().c_str()) + ")";
        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
}

QTreeWidgetItemInput *MainWindow::addItemInput(Input *in, Room *parent, bool selected)
{
        QTreeWidgetItemIterator it(ui->tree_home);
        while (*it)
        {
                QTreeWidgetItemRoom *item = dynamic_cast<QTreeWidgetItemRoom *>(*it);
                if (item && item->getRoom() == parent)
                        return addItemInput(in, item, selected);
                ++it;
        }

        return NULL;
}

QTreeWidgetItemInput *MainWindow::addItemInput(Input *in, QTreeWidgetItemRoom *parent, bool selected)
{
        QTreeWidgetItemInput *item = new QTreeWidgetItemInput(in, parent);

        if (selected) ui->tree_home->setCurrentItem(item);

        updateItemInfos(item);

        return item;
}

void MainWindow::updateItemInfos(QTreeWidgetItemInput *item)
{
        Input *in = item->getInput();

        item->setData(0, Qt::DisplayRole, QString::fromUtf8(in->get_param("name").c_str()));

        string type = in->get_param("type");
        if (type == "WIDigitalBP" || type == "WIDigitalTriple")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_inter.png"));
        else if (type == "WITemp")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/temp.png"));
        else if (type == "InPlageHoraire" || type == "InputTime")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_clock.png"));

        QString s = QString::fromUtf8(in->get_param("name").c_str());
        s += " (" + QString::fromUtf8(in->get_param("type").c_str()) + ")";
        if (in->get_param("type") == "WIDigitalBP" || in->get_param("type") == "WIDigitalTriple")
                s += " #" + QString::fromUtf8(in->get_param("var").c_str());

        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
}

QTreeWidgetItemOutput *MainWindow::addItemOutput(Output *out, Room *parent, bool selected)
{
        QTreeWidgetItemIterator it(ui->tree_home);
        while (*it)
        {
                QTreeWidgetItemRoom *item = dynamic_cast<QTreeWidgetItemRoom *>(*it);
                if (item && item->getRoom() == parent)
                        return addItemOutput(out, item, selected);
                ++it;
        }

        return NULL;
}

QTreeWidgetItemOutput *MainWindow::addItemOutput(Output *out, QTreeWidgetItemRoom *parent, bool selected)
{
        QTreeWidgetItemOutput *item = new QTreeWidgetItemOutput(out, parent);

        if (selected) ui->tree_home->setCurrentItem(item);

        updateItemInfos(item);

        return item;
}

void MainWindow::updateItemInfos(QTreeWidgetItemOutput *item)
{
        Output *out = item->getOutput();

        item->setData(0, Qt::DisplayRole, QString::fromUtf8(out->get_param("name").c_str()));

        string type = out->get_param("type");
        if (type == "WODigital" && out->get_param("gtype") == "light")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_light_on.png"));
        else if (type == "WODigital" && out->get_param("gtype") != "light")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_tor_on.png"));
        else if (type == "WOVolet" || out->get_param("type") == "WOVoletSmart")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_shutter.png"));
        else if (type == "Scenario" || type == "scenario")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_scenario.png"));
        else if (type == "InputTimer")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_clock.png"));
        else if (type == "InternalBool")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_bool_on.png"));
        else if (type == "InternalInt")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_int.png"));
        else if (type == "InternalString")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/text.png"));
        else if (type == "WODali" || type == "WODaliRVB" || type == "WONeon" || type == "X10Output")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_light_on.png"));
        else if (IOBase::isAudioType(type))
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_sound.png"));
        else if (IOBase::isCameraType(type))
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_camera_on.png"));

        QString s = QString::fromUtf8(out->get_param("name").c_str());
        s += " (" + QString::fromUtf8(out->get_param("type").c_str()) + ")";
        if (out->get_param("type") == "WODigital")
                s += " #" + QString::fromUtf8(out->get_param("var").c_str());
        if (out->get_param("type") == "WOVolet" || out->get_param("type") == "WOVoletSmart")
                s += " #" + QString::fromUtf8(out->get_param("var_up").c_str()) +
                     " #" + QString::fromUtf8(out->get_param("var_down").c_str());

        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
}

QTreeWidgetItemRule *MainWindow::addItemRule(Rule *rule, bool selected)
{
        QTreeWidgetItemRule *item = new QTreeWidgetItemRule(rule, ui->tree_rules);

        if (selected) ui->tree_rules->setCurrentItem(item);

        updateItemInfos(item);

        return item;
}

void MainWindow::updateItemInfos(QTreeWidgetItemRule *item)
{
        Rule *rule = item->getRule();

        item->setData(0, Qt::DisplayRole, QString::fromUtf8(rule->get_type().c_str()));
        item->setData(1, Qt::DisplayRole, QString::fromUtf8(rule->get_name().c_str()));
}

QTreeWidgetItem *MainWindow::addItemCondition(Condition *condition, Input *input, bool selected)
{
        string name, oper, value;

        name = input->get_param("name");
        oper = condition->get_operator().get_param(input->get_param("id"));

        if (oper == "INF") oper = "<";
        if (oper == "INF=") oper = "<=";
        if (oper == "SUP") oper = ">";
        if (oper == "SUP=") oper = ">=";

        if (condition->get_params_var().get_param(input->get_param("id")) != "")
        {
                string var_id = condition->get_params_var().get_param(input->get_param("id"));
                Input *in = ListeRoom::Instance().get_input(var_id);
                if (in)
                {
                        value = in->get_param("name");
                }
                else
                {
                        /* Wrong param_var, remove it */
                        value = condition->get_params().get_param(input->get_param("id"));
                        condition->get_params_var().Delete(input->get_param("id"));
                }
        }
        else
                value = condition->get_params().get_param(input->get_param("id"));

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree_condition);
        item->setData(0, Qt::DisplayRole, QString::fromUtf8(name.c_str()));
        item->setData(1, Qt::DisplayRole, QString::fromUtf8(oper.c_str()));
        item->setData(2, Qt::DisplayRole, QString::fromUtf8(value.c_str()));

        int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(input));
        string room_name;
        if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();
        QString s = "Input: " + QString::fromUtf8(input->get_param("type").c_str()) + " - " + QString::fromUtf8(room_name.c_str());
        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
        item->setData(1, Qt::ToolTipRole, s);
        item->setData(1, Qt::StatusTipRole, s);
        item->setData(2, Qt::ToolTipRole, s);
        item->setData(2, Qt::StatusTipRole, s);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setSizeHint(0, QSize(0, 25));

        //set delegate
        int row = ui->tree_condition->invisibleRootItem()->indexOfChild(item);
        ui->tree_condition->setItemDelegateForRow(row, new ConditionDelegate(condition, input));

        if (selected) ui->tree_condition->setCurrentItem(item);

        return item;
}

QTreeWidgetItem *MainWindow::addItemAction(Action *action, Output *output, bool selected)
{
        string name, value;

        name = output->get_param("name");

        if (action->get_params_var().get_param(output->get_param("id")) != "")
        {
                string var_id = action->get_params_var().get_param(output->get_param("id"));
                Output *out = ListeRoom::Instance().get_output(var_id);
                if (out)
                {
                        value = out->get_param("name");
                }
                else
                {
                        /* wrong param_var, remove it */
                        value = action->get_params().get_param(output->get_param("id"));
                        action->get_params_var().Delete(output->get_param("id"));
                }
        }
        else
                value = action->get_params().get_param(output->get_param("id"));

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree_action);
        item->setData(0, Qt::DisplayRole, QString::fromUtf8(name.c_str()));
        item->setData(1, Qt::DisplayRole, QString::fromUtf8(value.c_str()));

        int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(output));
        string room_name;
        if (r > 0) room_name = ListeRoom::Instance().get_room(r)->get_name();
        QString s = "Output: " + QString::fromUtf8(output->get_param("type").c_str()) + " - " + QString::fromUtf8(room_name.c_str());
        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
        item->setData(1, Qt::ToolTipRole, s);
        item->setData(1, Qt::StatusTipRole, s);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setSizeHint(0, QSize(0, 25));

        //set delegate
        int row = ui->tree_action->invisibleRootItem()->indexOfChild(item);
        ui->tree_action->setItemDelegateForRow(row, new ActionDelegate(action, output));

        if (selected) ui->tree_action->setCurrentItem(item);

        return item;
}

void MainWindow::goSelectRule()
{
        if (!treeItem) return;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                //Also select the corresponding rule if any...
                Rule *rule = ListeRule::Instance().searchRule(itinput->getInput());
                if (rule)
                {
                        QTreeWidgetItemIterator it(ui->tree_rules);
                        while (*it)
                        {
                                QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
                                if (item && item->getRule() == rule)
                                {
                                        ui->tree_rules->setCurrentItem(item);
                                        break;
                                }
                                ++it;
                        }
                }
        }

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                //Also select the corresponding rule if any...
                Rule *rule = ListeRule::Instance().searchRule(itoutput->getOutput());
                if (rule)
                {
                        QTreeWidgetItemIterator it(ui->tree_rules);
                        while (*it)
                        {
                                QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
                                if (item && item->getRule() == rule)
                                {
                                        ui->tree_rules->setCurrentItem(item);
                                        break;
                                }
                                ++it;
                        }
                }
        }
}

void MainWindow::on_tree_home_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
        QTreeWidgetItemRoom *itroom = dynamic_cast<QTreeWidgetItemRoom *>(current);
        if (itroom)
        {
                current_room = itroom->getRoom();
                ui->label_current_room->setText(QString::fromUtf8(current_room->get_name().c_str()));
        }

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(current);
        if (itinput)
        {
                itroom = dynamic_cast<QTreeWidgetItemRoom *>(itinput->parent());
                current_room = itroom->getRoom();
                ui->label_current_room->setText(QString::fromUtf8(current_room->get_name().c_str()));
        }

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(current);
        if (itoutput)
        {
                itroom = dynamic_cast<QTreeWidgetItemRoom *>(itoutput->parent());
                current_room = itroom->getRoom();
                ui->label_current_room->setText(QString::fromUtf8(current_room->get_name().c_str()));
        }
}

void MainWindow::showPopup_tree(const QPoint point)
{
        QTreeWidgetItem *item = NULL;
        item = ui->tree_home->itemAt(point);

        if (!item)
        {
                add_menu->exec(QCursor::pos());
        }
        else
        {
                QMenu item_menu(ui->tree_home);

                QAction *action = NULL;

                treeItem = item;

                action = item_menu.addAction(QString::fromUtf8("Voir la règle associé"));
                action->setIcon(QIcon(":/img/go-last.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(goSelectRule()));

                action = item_menu.addAction(QString::fromUtf8("Supprimer"));
                action->setIcon(QIcon(":/img/user-trash.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(deleteItem()));

                item_menu.addSeparator();

                //Here we have to add Item action (ON/OFF/UP/DOWN/...)
                QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
                if (itoutput)
                {
                        Output *o = itoutput->getOutput();

                        if (o->get_param("type") == "WODigital" || o->get_param("type") == "WODali" || o->get_param("type") == "WODaliRVB")
                        {
                                action = item_menu.addAction(QString::fromUtf8("Allumer"));
                                action->setIcon(QIcon(":/img/icon_light_on.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemLightOn()));

                                action = item_menu.addAction(QString::fromUtf8("Eteindre"));
                                action->setIcon(QIcon(":/img/icon_light_on.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemLightOff()));

                                item_menu.addSeparator();
                        }

                        if (IOBase::isCameraType(o->get_param("type")))
                        {
                                action = item_menu.addAction(QString::fromUtf8("Voir la caméra"));
                                action->setIcon(QIcon(":/img/icon_camera_on.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemShowCamera()));

                                item_menu.addSeparator();
                        }


                        if (o->get_param("type") == "WOVolet" || o->get_param("type") == "WOVoletSmart")
                        {
                                action = item_menu.addAction(QString::fromUtf8("Monter"));
                                action->setIcon(QIcon(":/img/icon_shutter.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemVoletUp()));

                                action = item_menu.addAction(QString::fromUtf8("Descendre"));
                                action->setIcon(QIcon(":/img/icon_shutter.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemVoletDown()));

                                action = item_menu.addAction(QString::fromUtf8("Arrêter"));
                                action->setIcon(QIcon(":/img/icon_shutter.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemVoletStop()));

                                item_menu.addSeparator();
                        }
                }

                QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
                if (itinput)
                {
                        Input *o = itinput->getInput();

                        if (o->get_param("type") == "InPlageHoraire")
                        {
                                action = item_menu.addAction(QString::fromUtf8("Modifier les plages horaires..."));
                                action->setIcon(QIcon(":/img/icon_clock.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemPlagesHoraires()));

                                item_menu.addSeparator();
                        }

                        if (o->get_param("type") == "WIDigitalBP")
                        {
                                action = item_menu.addAction(QString::fromUtf8("Convertir en interupteur triple..."));
                                action->setIcon(QIcon(":/img/icon_inter.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemConvertInterTriple()));

                                item_menu.addSeparator();
                        }

                        if (o->get_param("type") == "WIDigitalTriple")
                        {
                                action = item_menu.addAction(QString::fromUtf8("Convertir en interupteur standard..."));
                                action->setIcon(QIcon(":/img/icon_inter.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemConvertInterBP()));

                                item_menu.addSeparator();
                        }
                }

                action = item_menu.addAction(QString::fromUtf8("Propriétés"));
                action->setIcon(QIcon(":/img/document-properties.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(showPropertiesItem()));

                item_menu.exec(QCursor::pos());
        }
}

void MainWindow::showPopup_condition(const QPoint point)
{
        QTreeWidgetItem *item = NULL;
        item = ui->tree_condition->itemAt(point);

        if (!item)
                return;

        QMenu item_menu(ui->tree_condition);

        QAction *action = NULL;

        treeItem_condition = item;

        action = item_menu.addAction(QString::fromUtf8("Supprimer"));
        action->setIcon(QIcon(":/img/user-trash.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(deleteItemCondition()));

        item_menu.exec(QCursor::pos());
}

void MainWindow::showPopup_action(const QPoint point)
{
        QTreeWidgetItem *item = NULL;
        item = ui->tree_action->itemAt(point);

        if (!item)
                return;

        QMenu item_menu(ui->tree_action);

        QAction *action = NULL;

        treeItem_action = item;

        action = item_menu.addAction(QString::fromUtf8("Supprimer"));
        action->setIcon(QIcon(":/img/user-trash.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(deleteItemAction()));

        item_menu.exec(QCursor::pos());
}

void MainWindow::deleteItem()
{
        if (!treeItem) return;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calaos Installer"),
                              QString::fromUtf8("Etes vous sûr de vouloir supprimer l'élément \"%1\"").arg(treeItem->text(0)),
                              QMessageBox::Yes | QMessageBox::No);

        if (reply != QMessageBox::Yes)
                return;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                ListeRoom::Instance().deleteIO(itinput->getInput());
                delete itinput;

                ui->tree_action->clear_all();
                ui->tree_condition->clear_all();
                ui->tree_rules->clear();

                PopulateRulesTree();
                return;
        }

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                ListeRoom::Instance().deleteIO(itoutput->getOutput());
                delete itoutput;

                ui->tree_action->clear_all();
                ui->tree_condition->clear_all();
                ui->tree_rules->clear();

                PopulateRulesTree();
                return;
        }

        QTreeWidgetItemRoom *itroom = dynamic_cast<QTreeWidgetItemRoom *>(treeItem);
        if (itroom)
        {
                ListeRoom::Instance().Remove(itroom->getRoom());
                delete itroom;

                ui->tree_action->clear_all();
                ui->tree_condition->clear_all();
                ui->tree_rules->clear();

                PopulateRulesTree();
                return;
        }
}

void MainWindow::deleteItemCondition()
{
        if (!treeItem_condition) return;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calaos Installer"),
                              QString::fromUtf8("Etes vous sûr de vouloir supprimer l'élément \"%1\"").arg(treeItem_condition->text(0)),
                              QMessageBox::Yes | QMessageBox::No);

        if (reply != QMessageBox::Yes)
                return;

        Rule *rule = getCurrentRule();
        if (!rule) return;

        int num = ui->tree_condition->invisibleRootItem()->indexOfChild(treeItem_condition);

        if (num < 0 || num >= rule->get_size_conds() || rule->get_condition(num)->get_size() <= 0)
                return;

        rule->get_condition(num)->Remove(0);

        QTreeWidgetItem *item = ui->tree_rules->selectedItems().first();
        ui->tree_rules->setCurrentItem(NULL);
        ui->tree_rules->setCurrentItem(item);
}

void MainWindow::deleteItemAction()
{
        if (!treeItem_action) return;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calaos Installer"),
                              QString::fromUtf8("Etes vous sûr de vouloir supprimer l'élément \"%1\"").arg(treeItem_action->text(0)),
                              QMessageBox::Yes | QMessageBox::No);

        if (reply != QMessageBox::Yes)
                return;

        Rule *rule = getCurrentRule();
        if (!rule) return;

        int num = ui->tree_action->invisibleRootItem()->indexOfChild(treeItem_action);

        if (num < 0 || num >= rule->get_size_actions() || rule->get_action(num)->get_size() <= 0)
                return;

        rule->get_action(num)->Remove(0);

        QTreeWidgetItem *item = ui->tree_rules->selectedItems().first();
        ui->tree_rules->setCurrentItem(NULL);
        ui->tree_rules->setCurrentItem(item);
}

void MainWindow::showPropertiesItem()
{
        Params *p = NULL;
        Params proom;
        int type = OBJ_NONE;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                p = &itinput->getInput()->get_params();
                type = OBJ_INPUT;
        }

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                p = &itoutput->getOutput()->get_params();
                type = OBJ_OUTPUT;
        }

        QTreeWidgetItemRoom *itroom = dynamic_cast<QTreeWidgetItemRoom *>(treeItem);
        if (itroom)
        {
                proom.Add("name", itroom->getRoom()->get_name());
                proom.Add("type", itroom->getRoom()->get_type());
                p = &proom;
                type = OBJ_ROOM;
        }

        if (!p)
                return;

        DialogIOProperties dialog(*p, type, this);
        if (dialog.exec() == QDialog::Accepted)
        {
                *p = dialog.getParams();
                if (type == OBJ_ROOM)
                {
                        Room *room = itroom->getRoom();
                        string n;
                        n = (*p)["name"];
                        room->set_name(n);
                        n = (*p)["type"];
                        room->set_type(n);

                        updateItemInfos(itroom);
                }
                else if (type == OBJ_INPUT)
                {
                        updateItemInfos(itinput);
                }
                else if (type == OBJ_OUTPUT)
                {
                        updateItemInfos(itoutput);
                }
        }
}

void MainWindow::on_actionSauvegarder_un_projet_triggered()
{
        QString dir = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Choisir un dossier de projet..."),
                                                 QString(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

        project_path = dir;

        Save();
}

void MainWindow::on_tree_home_itemDoubleClicked(QTreeWidgetItem* item, int)
{
        treeItem = item;
        showPropertiesItem();
}

void MainWindow::on_actionNouveau_projet_triggered()
{
        if (project_changed)
        {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Un projet est ouvert et modifié, voulez-vous l'enregistrer?"),
                                      QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                        on_actionSauvegarder_un_projet_triggered();
        }

        project_changed = false;

        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();

        ui->tree_home->clear();
        ui->tree_condition->clear_all();
        ui->tree_action->clear_all();
        ui->tree_rules->clear();

        current_room = NULL;
        ui->label_current_room->setText("<aucun>");
}

void MainWindow::on_actionSauvegarder_triggered()
{
        if (project_path.isEmpty())
                on_actionSauvegarder_un_projet_triggered();

        Save();
}

void MainWindow::Save(QString path)
{
        if (!path.isEmpty())
                project_path = path;

        QString iofile = project_path + "/io.xml";
        QString rulefile = project_path + "/rules.xml";

        ProjectManager::saveIOsToFile(iofile);
        ProjectManager::saveRulesToFile(rulefile);

        statusBar()->showMessage(QString::fromUtf8("Projet sauvegardé: ") + project_path, 3000);
}

void MainWindow::Load(QString path)
{
        if (!path.isEmpty())
                project_path = path;

        QProgressDialog bar("Chargement en cours...", 0, 0, 0, this);
        bar.setWindowModality(Qt::ApplicationModal);
        bar.show();

        QString iofile = project_path + "/io.xml";
        QString rulefile = project_path + "/rules.xml";

        ProjectManager::loadIOsFromFile(iofile);
        QCoreApplication::processEvents();
        ProjectManager::loadRulesFromFile(rulefile);
        QCoreApplication::processEvents();

        ui->tree_condition->header()->setResizeMode(QHeaderView::Interactive);
        ui->tree_action->header()->setResizeMode(QHeaderView::Interactive);
        ui->tree_rules->header()->setResizeMode(QHeaderView::Interactive);
        ui->tree_home->setUpdatesEnabled(false);
        QCoreApplication::processEvents();

        PopulateRoomsTree();
        PopulateRulesTree();

//        ui->tree_condition->header()->setResizeMode(QHeaderView::ResizeToContents);
//        QCoreApplication::processEvents();
//        ui->tree_action->header()->setResizeMode(QHeaderView::ResizeToContents);
//        QCoreApplication::processEvents();
//        ui->tree_rules->header()->setResizeMode(QHeaderView::ResizeToContents);
        QCoreApplication::processEvents();
        ui->tree_home->setUpdatesEnabled(true);

        statusBar()->showMessage(QString::fromUtf8("Projet chargé: ") + project_path, 3000);
}

void MainWindow::on_actionCharger_un_projet_triggered()
{
        if (project_changed)
        {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Un projet est ouvert et modifié, voulez-vous l'enregistrer?"),
                                      QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                        on_actionSauvegarder_un_projet_triggered();
        }

        project_changed = false;

        QString dir = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Choisir un dossier de projet..."),
                                                 QString(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

        if (dir.isEmpty()) return;

        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();

        ui->tree_home->clear();
        ui->tree_condition->clear_all();
        ui->tree_action->clear_all();
        ui->tree_rules->clear();
        current_room = NULL;
        ui->label_current_room->setText("<aucun>");

        project_path = dir;

        Load();
}

void MainWindow::on_bt_rules_add_clicked()
{
        string type = "", name = "";

        if (current_room) type = current_room->get_name();

        if (ui->tree_home->selectedItems().size() > 0)
        {
                QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(ui->tree_home->selectedItems().first());
                if (itinput)
                {
                        Input *input = itinput->getInput();
                        name = input->get_param("name");
                }
        }

        DialogNewRule dialog(name, type);
        if (dialog.exec() == QDialog::Accepted)
        {
                Rule *rule = dialog.getRule();
                if (rule)
                        addItemRule(rule, true);
                else
                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
        }
}

void MainWindow::on_tree_rules_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
        QTreeWidgetItemRule *itrule = dynamic_cast<QTreeWidgetItemRule *>(current);
        if (itrule)
        {
                //clear both lists
                ui->tree_condition->clear_all();
                ui->tree_action->clear_all();

                //add new elements from selected rule
                Rule *rule = itrule->getRule();

                for (int i = 0;i < rule->get_size_conds();i++)
                {
                        for (int j = 0;j < rule->get_condition(i)->get_size();j++)
                        {
                                addItemCondition(rule->get_condition(i), rule->get_condition(i)->get_input(j));
                        }
                }

                for (int i = 0;i < rule->get_size_actions();i++)
                {
                        for (int j = 0;j < rule->get_action(i)->get_size();j++)
                        {
                                addItemAction(rule->get_action(i), rule->get_action(i)->get_output(j));
                        }
                }
        }
}

Rule *MainWindow::getCurrentRule()
{
        if (ui->tree_rules->selectedItems().size() <= 0)
                return NULL;

        QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(ui->tree_rules->selectedItems().first());
        if (item)
                return item->getRule();

        return NULL;
}

void MainWindow::on_bt_rules_del_clicked()
{
        if (ui->tree_rules->selectedItems().size() <= 0)
                return;

        QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(ui->tree_rules->selectedItems().first());
        if (item)
        {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Etes vous sûr de vouloir supprimer la règle?"),
                                      QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                {
                        Rule *r = item->getRule();
                        ListeRule::Instance().Remove(r);

                        delete item;

                        //clear both lists
                        ui->tree_condition->clear_all();
                        ui->tree_action->clear_all();

                        if (ui->tree_rules->selectedItems().size() > 0)
                        {
                                QTreeWidgetItem *item = ui->tree_rules->selectedItems().first();
                                ui->tree_rules->setCurrentItem(NULL);
                                ui->tree_rules->setCurrentItem(item);
                        }
                }
        }
}

void MainWindow::on_actionProgrammer_l_automate_triggered()
{
        ui->actionProgrammer_l_automate->setEnabled(false);
        buttonStopProcess->show();
        progressBar->show();
        connect(buttonStopProcess, SIGNAL(clicked()), this, SLOT(button_wagostop_clicked()));

        wuploader.startUpload(WagoConnect::Instance().getWagoIP());
}

void MainWindow::button_wagostop_clicked()
{
        wuploader.stopUpload();
        ui->actionProgrammer_l_automate->setEnabled(true);
        buttonStopProcess->hide();
        progressBar->hide();

        disconnect(buttonStopProcess, SIGNAL(clicked()), this, SLOT(button_wagostop_clicked()));
}

void MainWindow::wagoStatusProgress(int status)
{
        if (status == WAGOST_START)
        {
                progressBar->setFormat(QString::fromUtf8("Veuillez patienter..."));
        }
        else if (status == WAGOST_CREATING)
        {
                progressBar->setFormat(QString::fromUtf8("Création %p%"));
        }
        else if (status == WAGOST_UPLOADING)
        {
                progressBar->setFormat(QString::fromUtf8("Chargement automate %p%"));
        }
        else if (status == WAGOST_ABORTED)
        {
                progressBar->setFormat(QString::fromUtf8("Annulé"));
                progressBar->setValue(0);
        }
        else if (status == WAGOST_DONE)
        {
                progressBar->setFormat("%p%");
                progressBar->setValue(100);

                ui->actionProgrammer_l_automate->setEnabled(true);
                buttonStopProcess->hide();
                progressBar->hide();
        }
}

void MainWindow::itemLightOn()
{
        if (!treeItem) return;

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                if (itoutput->getOutput()->get_param("type") == "WODigital")
                {
                        QString cmd;

                        if (itoutput->getOutput()->get_param("knx") == "true")
                                cmd = "WAGO_SET_KNX_OUTPUT ";
                        else
                                cmd = "WAGO_SET_OUTPUT ";
                        cmd += itoutput->getOutput()->get_param("var").c_str();
                        cmd += " 1";

                        WagoConnect::Instance().SendCommand(cmd);
                }
                else if (itoutput->getOutput()->get_param("type") == "WODali")
                {
                        QString cmd = "WAGO_DALI_SET 1 ";
                        if (itoutput->getOutput()->get_param("group") == "1")
                                cmd += "1 ";
                        else
                                cmd += "0 ";

                        cmd += itoutput->getOutput()->get_param("address").c_str();
                        cmd += " 100 ";
                        cmd += itoutput->getOutput()->get_param("fade_time").c_str();

                        WagoConnect::Instance().SendCommand(cmd);
                }
        }
}

void MainWindow::itemLightOff()
{
        if (!treeItem) return;

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                if (itoutput->getOutput()->get_param("type") == "WODigital")
                {
                        QString cmd;

                        if (itoutput->getOutput()->get_param("knx") == "true")
                                cmd = "WAGO_SET_KNX_OUTPUT ";
                        else
                                cmd = "WAGO_SET_OUTPUT ";
                        cmd += itoutput->getOutput()->get_param("var").c_str();
                        cmd += " 0";

                        WagoConnect::Instance().SendCommand(cmd);
                }
                else if (itoutput->getOutput()->get_param("type") == "WODali")
                {
                        QString cmd = "WAGO_DALI_SET 1 ";
                        if (itoutput->getOutput()->get_param("group") == "1")
                                cmd += "1 ";
                        else
                                cmd += "0 ";

                        cmd += itoutput->getOutput()->get_param("address").c_str();
                        cmd += " 0 ";
                        cmd += itoutput->getOutput()->get_param("fade_time").c_str();

                        WagoConnect::Instance().SendCommand(cmd);
                }
        }
}

void MainWindow::itemShowCamera()
{
        if (!treeItem) return;

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                if (IOBase::isCameraType(itoutput->getOutput()->get_param("type")))
                {
                        DialogCameraView d(dynamic_cast<Camera *>(itoutput->getOutput()));
                        d.DownloadPicture();
                        d.exec();
                }
        }
}

void MainWindow::itemPlagesHoraires()
{
        if (!treeItem) return;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                if (itinput->getInput()->get_param("type") == "InPlageHoraire")
                {
                        DialogPlageHoraire d(dynamic_cast<InPlageHoraire *>(itinput->getInput()));
                        d.exec();
                }
        }
}

void MainWindow::itemConvertInterTriple()
{
        if (!treeItem) return;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                if (itinput->getInput()->get_param("type") == "WIDigitalBP")
                {
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(this, tr("Calaos Installer"),
                                              QString::fromUtf8("Etes vous sûr de vouloir convertir en interupteur triple?"),
                                              QMessageBox::Yes | QMessageBox::No);

                        if (reply == QMessageBox::Yes)
                        {
                                itinput->getInput()->get_params().Add("type", "WIDigitalTriple");
                                updateItemInfos(itinput);
                        }
                }
        }
}

void MainWindow::itemConvertInterBP()
{
        if (!treeItem) return;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                if (itinput->getInput()->get_param("type") == "WIDigitalTriple")
                {
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(this, tr("Calaos Installer"),
                                              QString::fromUtf8("Etes vous sûr de vouloir convertir en interupteur classique?"),
                                              QMessageBox::Yes | QMessageBox::No);

                        if (reply == QMessageBox::Yes)
                        {
                                itinput->getInput()->get_params().Add("type", "WIDigitalBP");
                                updateItemInfos(itinput);
                        }
                }
        }
}

void MainWindow::itemVoletUp()
{
        if (!treeItem) return;

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                if (itoutput->getOutput()->get_param("type") == "WOVolet" ||
                    itoutput->getOutput()->get_param("type") == "WOVoletSmart")
                {
                        QString cmd = "WAGO_SET_OUTPUT ";
                        cmd += itoutput->getOutput()->get_param("var_down").c_str();
                        cmd += " 0";

                        WagoConnect::Instance().SendCommand(cmd);

                        cmd = "WAGO_SET_OUTPUT ";
                        cmd += itoutput->getOutput()->get_param("var_up").c_str();
                        cmd += " 1";

                        WagoConnect::Instance().SendCommand(cmd);
                }
        }
}

void MainWindow::itemVoletDown()
{
        if (!treeItem) return;

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                if (itoutput->getOutput()->get_param("type") == "WOVolet" ||
                    itoutput->getOutput()->get_param("type") == "WOVoletSmart")
                {
                        QString cmd = "WAGO_SET_OUTPUT ";
                        cmd += itoutput->getOutput()->get_param("var_up").c_str();
                        cmd += " 0";

                        WagoConnect::Instance().SendCommand(cmd);

                        cmd = "WAGO_SET_OUTPUT ";
                        cmd += itoutput->getOutput()->get_param("var_down").c_str();
                        cmd += " 1";

                        WagoConnect::Instance().SendCommand(cmd);
                }
        }
}

void MainWindow::itemVoletStop()
{
        if (!treeItem) return;

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                if (itoutput->getOutput()->get_param("type") == "WOVolet" ||
                    itoutput->getOutput()->get_param("type") == "WOVoletSmart")
                {
                        QString cmd = "WAGO_SET_OUTPUT ";
                        cmd += itoutput->getOutput()->get_param("var_down").c_str();
                        cmd += " 0";

                        WagoConnect::Instance().SendCommand(cmd);

                        cmd = "WAGO_SET_OUTPUT ";
                        cmd += itoutput->getOutput()->get_param("var_up").c_str();
                        cmd += " 0";

                        WagoConnect::Instance().SendCommand(cmd);
                }
        }
}


void MainWindow::on_actionSe_connecter_triggered()
{
        DialogConnect dconnect;
        dconnect.exec();
}

void MainWindow::on_actionSe_d_connecter_triggered()
{
        WagoConnect::Instance().Disconnect();
}

void MainWindow::wagoConnected(QString &, bool)
{
        ui->actionSe_connecter->setEnabled(false);
        ui->actionSe_d_connecter->setEnabled(true);
        ui->actionProgrammer_l_automate->setEnabled(true);

        statusConnectText->setText(QString::fromUtf8("Connecté (") + WagoConnect::Instance().getWagoVersion() + ")");
        statusConnectIcon->setPixmap(QPixmap(":/img/user-online_16x16.png"));
}

void MainWindow::wagoDisconnected()
{
        ui->actionSe_connecter->setEnabled(true);
        ui->actionSe_d_connecter->setEnabled(false);
        ui->actionProgrammer_l_automate->setEnabled(false);

        statusConnectText->setText(QString::fromUtf8("Déconnecté."));
        statusConnectIcon->setPixmap(QPixmap(":/img/user-invisible_16x16.png"));
}

void MainWindow::wagoUpdateNeeded(QString &version)
{
        messageBox.setText(QString::fromUtf8("L'automate doit être mis à jour.\n\nIl est actuellement en version %1, la dernière version est la %2.").arg(version, WAGO_FW_VESION));
        messageBox.show();
}

void MainWindow::wagoError(int error)
{
        switch (error)
        {
          case WERROR_CONNECT_FAILED:
                if (QMessageBox::question(this, tr("Calaos Installer"),
                                          QString::fromUtf8("La connection a échoué !\nVoulez-vous essayer à nouveau?"),
                                          QMessageBox::Ok,
                                          QMessageBox::Cancel
                        ) == QMessageBox::Ok)
                        on_actionSe_connecter_triggered();
                break;
          case WERROR_NOTCONNECTED:
                if (QMessageBox::question(this, tr("Calaos Installer"),
                                          QString::fromUtf8("L'automate n'est pas connecté !\nVoulez-vous effectuer une connexion maintenant?"),
                                          QMessageBox::Ok,
                                          QMessageBox::Cancel
                        ) == QMessageBox::Ok)
                        on_actionSe_connecter_triggered();
                break;
          case WERROR_TIMEOUT:
                if (QMessageBox::question(this, tr("Calaos Installer"),
                                          QString::fromUtf8("Le délai d'attente de la réponse est dépassé !\nVoulez-vous essayer à nouveau?"),
                                          QMessageBox::Ok,
                                          QMessageBox::Cancel
                        ) == QMessageBox::Ok)
                        on_actionSe_connecter_triggered();
                break;
          default:
                QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Une erreur inconnue est survenue !"));
                break;
        }
}

void MainWindow::on_actionOuvrir_un_projet_en_ligne_triggered()
{
        DialogOpenOnline dopen(tempDir.path());

        if (dopen.exec() == QDialog::Accepted)
        {
                ListeRule::Instance().clear();
                ListeRoom::Instance().clear();

                ui->tree_home->clear();
                ui->tree_condition->clear_all();
                ui->tree_action->clear_all();
                ui->tree_rules->clear();
                current_room = NULL;
                ui->label_current_room->setText("<aucun>");

                project_path = tempDir.path();

                Load();
        }
}

void MainWindow::on_actionSauvegarder_un_projet_en_ligne_triggered()
{
        DialogSaveOnline dsave(project_path);

        Save(); //Save the project before sending it

        if (dsave.exec() == QDialog::Accepted)
        {
                statusBar()->showMessage(QString::fromUtf8("Projet envoyé sur la centrale..."), 3000);
        }
}

void MainWindow::on_pushButton_clicked()
{
        ShowPage(PAGE_PROG);
}

void MainWindow::on_closeDaliForm_clicked()
{
        ShowPage(PAGE_PROG);
}

void MainWindow::on_actionDALI_triggered()
{
        if (WagoConnect::Instance().getConnectionStatus() == WAGO_CONNECTED)
        {
                if (WagoConnect::Instance().getWagoType() == "750-841" ||
                    WagoConnect::Instance().getWagoType() == "750-849")
                {
                        ShowPage(PAGE_DALI);
                        ui->widgetDali->Init();
                }
                else
                {
                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("L'adressage DALI n'est supporté que sur les contrôleurs\nWago 750-841 et 750-849!"));
                }
        }
        else
        {
                WagoConnect::Instance().SendCommand("WAGO_DALI_ADDRESSING_STATUS");
        }
}
