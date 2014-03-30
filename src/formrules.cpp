#include "formrules.h"
#include "ui_formrules.h"

#include "TwoLineItemDelegate.h"
#include "mainwindow.h"

FormRules::FormRules(QWidget *parent) :
                QWidget(parent),
                ui(new Ui::FormRules),
                current_room(NULL),
                project_changed(false),
                popupConditionStd(new FormConditionStd(this)),
                popupConditionStart(new FormConditionStart(this)),
                popupConditionScript(new FormConditionScript(this)),
                popupActionStd(new FormActionStd(this)),
                popupActionMail(new FormActionMail(this)),
                popupActionScript(new FormActionScript(this)),
                popupActionTouchscreen(new FormActionTouchscreen(this))
{
        ui->setupUi(this);

        add_menu = new QMenu(parent);
        ui->ioexplorer_add->setMenu(add_menu);

        QAction *action = NULL;

        action = add_menu->addAction(tr("Room"));
        action->setIcon(QIcon(":/img/rooms/various_small.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_ROOM); });

        add_menu->addSeparator();

        QMenu *wago_menu = add_menu->addMenu(QIcon("://img/logo_wago.png"), "Wago PLC");

        action = wago_menu->addAction(tr("Switch"));
        action->setIcon(QIcon(":/img/icon_inter.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_INTER); });

        action = wago_menu->addAction(tr("Light"));
        action->setIcon(QIcon(":/img/icon_light_on.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_LIGHT); });

        action = wago_menu->addAction(tr("Shutter"));
        action->setIcon(QIcon(":/img/icon_shutter.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_VOLET); });

        action = wago_menu->addAction(tr("DALI/DMX"));
        action->setIcon(QIcon(":/img/icon_light_on.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_DALI); });

        action = wago_menu->addAction(tr("DALI/DMX RGB"));
        action->setIcon(QIcon(":/img/icon_light_on.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_DALIRGB); });

        action = wago_menu->addAction(tr("Temperature sensor"));
        action->setIcon(QIcon(":/img/temp.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_TEMP); });

        action = wago_menu->addAction(tr("Analog Input/Output"));
        action->setIcon(QIcon(":/img/icon_analog.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_ANALOG); });

        QMenu *owire_menu = add_menu->addMenu(QIcon("://img/chip.png"), "OneWire");

        action = owire_menu->addAction(tr("Temperature sensor"));
        action->setIcon(QIcon(":/img/temp.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_ONEWIRE, ITEM_TEMP); });

        QMenu *x10_menu = add_menu->addMenu(QIcon("://img/x10.png"), "X10");

        action = x10_menu->addAction(tr("Light dimmer"));
        action->setIcon(QIcon(":/img/icon_light_on.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_X10, ITEM_DALI); });

        QMenu *zibase_menu = add_menu->addMenu(QIcon("://img/zibase.png"), "Zibase");

        action = zibase_menu->addAction(tr("Switch input"));
        action->setIcon(QIcon(":/img/icon_inter.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_ZIBASE, ITEM_INTER); });

        action = zibase_menu->addAction(tr("Analog input"));
        action->setIcon(QIcon(":/img/icon_analog.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_ZIBASE, ITEM_ANALOG); });

        QMenu *web_menu = add_menu->addMenu(QIcon("://img/web.png"), "Web");

        action = web_menu->addAction(tr("Light"));
        action->setIcon(QIcon(":/img/icon_light_on.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_LIGHT); });

        action = wago_menu->addAction(tr("Shutter"));
        action->setIcon(QIcon(":/img/icon_shutter.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_VOLET); });

        action = wago_menu->addAction(tr("Temperature sensor"));
        action->setIcon(QIcon(":/img/temp.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_TEMP); });

        action = wago_menu->addAction(tr("Analog Input"));
        action->setIcon(QIcon(":/img/icon_analog.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_ANALOG); });

        QMenu *gpio_menu = add_menu->addMenu(QIcon("://img/chip.png"), "GPIO");

        action = gpio_menu->addAction(tr("Switch input"));
        action->setIcon(QIcon(":/img/icon_inter.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_GPIO, ITEM_INTER); });

        action = gpio_menu->addAction(tr("Light"));
        action->setIcon(QIcon(":/img/icon_light_on.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_GPIO, ITEM_LIGHT); });

        action = gpio_menu->addAction(tr("Shutter"));
        action->setIcon(QIcon(":/img/icon_shutter.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_GPIO, ITEM_VOLET); });

        add_menu->addSeparator();

        action = add_menu->addAction(tr("Camera"));
        action->setIcon(QIcon(":/img/icon_camera_on.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_CAMERA); });

        action = add_menu->addAction(tr("Music zone"));
        action->setIcon(QIcon(":/img/icon_sound.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_MUSIC); });

        add_menu->addSeparator();

        action = add_menu->addAction(tr("Internal Variable"));
        action->setIcon(QIcon(":/img/text.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_INTERN); });

        action = add_menu->addAction(tr("Scenario variable"));
        action->setIcon(QIcon(":/img/icon_scenario.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_SCENARIO); });

        action = add_menu->addAction(tr("Time/Scheduling Variable"));
        action->setIcon(QIcon(":/img/icon_clock.png"));
        connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_TIME); });

        addConditionMenu = new QMenu(parent);
        ui->bt_condition_add->setMenu(addConditionMenu);

        action = addConditionMenu->addAction(tr("Normal Condition"));
        action->setIcon(QIcon(":/img/icon_rule.png"));
        connect(action, &QAction::triggered, [=]() { addCondition(COND_STD); });

        action = addConditionMenu->addAction(tr("Condition on output event"));
        action->setIcon(QIcon(":/img/icon_rule_out.png"));
        connect(action, &QAction::triggered, [=]() { addCondition(COND_OUTPUT); });

        action = addConditionMenu->addAction(tr("Condition at start"));
        action->setIcon(QIcon(":/img/icon_rule_start.png"));
        connect(action, &QAction::triggered, [=]() { addCondition(COND_START); });

        action = addConditionMenu->addAction(tr("Script Condition"));
        action->setIcon(QIcon(":/img/icon_rule_script.png"));
        connect(action, &QAction::triggered, [=]() { addCondition(COND_SCRIPT); });

        addActionMenu = new QMenu(parent);
        ui->bt_action_add->setMenu(addActionMenu);

        action = addActionMenu->addAction(tr("Normal Action"));
        action->setIcon(QIcon(":/img/icon_rule.png"));
        connect(action, &QAction::triggered, [=]() { addAction(ACTION_STD); });

        action = addActionMenu->addAction(tr("E-Mail Action"));
        action->setIcon(QIcon(":/img/icon_rule_mail.png"));
        connect(action, &QAction::triggered, [=]() { addAction(ACTION_MAIL); });

        action = addActionMenu->addAction(tr("Script Action"));
        action->setIcon(QIcon(":/img/icon_rule_script.png"));
        connect(action, &QAction::triggered, [=]() { addAction(ACTION_SCRIPT); });

        action = addActionMenu->addAction(tr("Touchscreen Action"));
        action->setIcon(QIcon(":/img/icon_rule.png"));
        connect(action, &QAction::triggered, [=]() { addAction(ACTION_TOUCHSCREEN); });


        connect(ui->tree_home, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_tree(QPoint)));
        connect(ui->tree_condition, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_condition(QPoint)));
        connect(ui->tree_action, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_action(QPoint)));
        connect(ui->tree_rules, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_rule(QPoint)));
}

FormRules::~FormRules()
{
        delete ui;
}

void FormRules::changeEvent(QEvent *e)
{
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}

void FormRules::PopulateRoomsTree()
{
        ui->tree_home->setUpdatesEnabled(false);
        QCoreApplication::processEvents();

        QStringList headers;
        headers << QString::fromUtf8("Maison");
        ui->tree_home->setHeaderLabels(headers);

        ui->tree_condition->header()->setSectionsMovable(false);
        ui->tree_condition->header()->resizeSection(0, 240);
        ui->tree_condition->header()->resizeSection(1, 100);
        ui->tree_condition->header()->resizeSection(2, 240);

        ui->tree_action->header()->setSectionsMovable(false);
        ui->tree_action->header()->resizeSection(0, 240);
        ui->tree_action->header()->resizeSection(1, 240);

        ui->tree_rules->header()->setSectionsMovable(false);
        ui->tree_rules->header()->resizeSection(0, 200);
        ui->tree_rules->header()->resizeSection(1, 200);

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
                            in->get_param("type") == "WIDigitalLong" ||
                            in->get_param("type") == "InPlageHoraire" ||
                            in->get_param("type") == "InputTime" ||
                            in->get_param("type") == "WITemp" ||
                            in->get_param("type") == "OWTemp" ||
                            in->get_param("type") == "WIAnalog")
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

        QCoreApplication::processEvents();
        ui->tree_home->setUpdatesEnabled(true);
}

void FormRules::PopulateRulesTree()
{
        QStringList headers;
        headers << QString::fromUtf8("Type") << QString::fromUtf8("Nom");
        ui->tree_rules->setHeaderLabels(headers);
        ui->tree_condition->header()->setSectionResizeMode(QHeaderView::Interactive);
        ui->tree_action->header()->setSectionResizeMode(QHeaderView::Interactive);
        ui->tree_rules->header()->setSectionResizeMode(QHeaderView::Interactive);

        for (int i = 0;i < ListeRule::Instance().size();i++)
        {
                Rule *rule = ListeRule::Instance().get_rule(i);
                bool selected = false;
                if (i == 0) selected = true;
                addItemRule(rule, selected);

                QCoreApplication::processEvents();
        }
}

void FormRules::ClearProject()
{
        setProjectModified(false);

        ui->tree_home->clear();
        ui->tree_condition->clear_all();
        ui->tree_action->clear_all();
        ui->tree_rules->clear();
        current_room = NULL;
}

void FormRules::setProjectModified(bool modified)
{
        project_changed = modified;

        emit projectModified(project_changed);
}

void FormRules::addCalaosItem(int hw_type, int item)
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

        setProjectModified(true);

        switch (item)
        {
          case ITEM_ROOM:
                {
                        DialogNewRoom dialog;
                        if (dialog.exec() == QDialog::Accepted)
                        {
                                current_room = ListeRoom::Instance().get_room(ListeRoom::Instance().size() - 1);

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
                        qDebug() << "HW TYPE : " << hw_type;
                        if (hw_type == HW_WAGO)
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
                        else if (hw_type == HW_ONEWIRE)
                        {
                                DialogNewOneWireTemp dialog(current_room);
                                if (dialog.exec() == QDialog::Accepted)
                                {
                                        Input *input = dialog.getInput();

                                        if (input)
                                                addItemInput(input, current_room, true);
                                        else
                                                QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                                }
                        }
                        else
                                    QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Unknown hardware type"));

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
        case ITEM_ANALOG:
              {
                      DialogNewAnalog dialog(current_room);
                      if (dialog.exec() == QDialog::Accepted)
                      {
                              if (dialog.isInputType())
                              {
                                      Input *input = dialog.getInput();
                                      if (input)
                                              addItemInput(input, current_room, true);
                                      else
                                              QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                              }
                              else
                              {
                                      Output *output = dialog.getOutput();
                                      if (output)
                                              addItemOutput(output, current_room, true);
                                      else
                                              QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur lors de la création de l'objet !"));
                              }
                      }
              }
              break;

          default:
                QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Type d'elément (%1) inconnu!").arg(item));
        }
}

QTreeWidgetItemRoom *FormRules::addItemRoom(Room *room, bool selected)
{
        QTreeWidgetItemRoom *item = new QTreeWidgetItemRoom(room, ui->tree_home);
        item->setSizeHint(0, QSize(24, 24));

        if (selected) ui->tree_home->setCurrentItem(item);

        updateItemInfos(item);

        setProjectModified(true);

        return item;
}

QString FormRules::getIconFromRoom(Room *room)
{
        QString icon;

        if (room->get_type() == "salon" || room->get_type() == "lounge")
                icon = ":/img/rooms/lounge_small.png";
        else if (room->get_type() == "sdb" || room->get_type() == "bathroom")
                icon = ":/img/rooms/bathroom_small.png";
        else if (room->get_type() == "chambre" || room->get_type() == "bedroom")
                icon = ":/img/rooms/bedroom_small.png";
        else if (room->get_type() == "cave" || room->get_type() == "cellar")
                icon = ":/img/rooms/cellar_small.png";
        else if (room->get_type() == "couloir" || room->get_type() == "hall" || room->get_type() == "corridor")
                icon = ":/img/rooms/corridor_small.png";
        else if (room->get_type() == "sam" || room->get_type() == "diningroom")
                icon = ":/img/rooms/diningroom_small.png";
        else if (room->get_type() == "garage")
                icon = ":/img/rooms/garage_small.png";
        else if (room->get_type() == "cuisine" || room->get_type() == "kitchen")
                icon = ":/img/rooms/kitchen_small.png";
        else if (room->get_type() == "bureau" || room->get_type() == "office")
                icon = ":/img/rooms/office_small.png";
        else if (room->get_type() == "exterieur" || room->get_type() == "outside")
                icon = ":/img/rooms/outside_small.png";
        else if (room->get_type() == "misc" || room->get_type() == "divers" || room->get_type() == "various")
                icon = ":/img/rooms/various_small.png";
        else
                icon = ":/img/rooms/various_small.png";

        return icon;
}

void FormRules::updateItemInfos(QTreeWidgetItemRoom *item)
{
        Room *room = item->getRoom();

        item->setData(0, Qt::DisplayRole, QString::fromUtf8(room->get_name().c_str()));

        item->setData(0, Qt::DecorationRole, QIcon(getIconFromRoom(room)));

        QString s = QString::fromUtf8(room->get_name().c_str());
        s += " (" + QString::fromUtf8(room->get_type().c_str()) + ")";
        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
}

QTreeWidgetItemInput *FormRules::addItemInput(Input *in, Room *parent, bool selected)
{
        QTreeWidgetItemIterator it(ui->tree_home);
        while (*it)
        {
                QTreeWidgetItemRoom *item = dynamic_cast<QTreeWidgetItemRoom *>(*it);
                if (item && item->getRoom() == parent)
                        return addItemInput(in, item, selected);
                ++it;
        }

        setProjectModified(true);

        return NULL;
}

QTreeWidgetItemInput *FormRules::addItemInput(Input *in, QTreeWidgetItemRoom *parent, bool selected)
{
        QTreeWidgetItemInput *item = new QTreeWidgetItemInput(in, parent);

        if (selected) ui->tree_home->setCurrentItem(item);

        updateItemInfos(item);

        setProjectModified(true);

        return item;
}

void FormRules::updateItemInfos(QTreeWidgetItemInput *item)
{
        Input *in = item->getInput();

        item->setData(0, Qt::DisplayRole, QString::fromUtf8(in->get_param("name").c_str()));

        string type = in->get_param("type");
        if (type == "WIDigitalBP" || type == "WIDigitalTriple" || type == "WIDigitalLong")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_inter.png"));
        else if (type == "WITemp" || type == "OWTemp")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/temp.png"));
        else if (type == "InPlageHoraire" || type == "InputTime")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_clock.png"));
        else if (type == "WIAnalog")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_analog.png"));

        QString s = QString::fromUtf8(in->get_param("name").c_str());
        s += " (" + QString::fromUtf8(in->get_param("type").c_str()) + ")";
        if (type == "WIDigitalBP" || type == "WIDigitalTriple" || type == "WIDigitalLong" ||
            type == "WITemp" || type == "WIAnalog")
                s += " #" + QString::fromUtf8(in->get_param("var").c_str());

        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
}

QTreeWidgetItemOutput *FormRules::addItemOutput(Output *out, Room *parent, bool selected)
{
        QTreeWidgetItemIterator it(ui->tree_home);
        while (*it)
        {
                QTreeWidgetItemRoom *item = dynamic_cast<QTreeWidgetItemRoom *>(*it);
                if (item && item->getRoom() == parent)
                        return addItemOutput(out, item, selected);
                ++it;
        }

        setProjectModified(true);

        return NULL;
}

QTreeWidgetItemOutput *FormRules::addItemOutput(Output *out, QTreeWidgetItemRoom *parent, bool selected)
{
        QTreeWidgetItemOutput *item = new QTreeWidgetItemOutput(out, parent);

        if (selected) ui->tree_home->setCurrentItem(item);

        updateItemInfos(item);

        setProjectModified(true);

        return item;
}

void FormRules::updateItemInfos(QTreeWidgetItemOutput *item)
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
        else if (type == "WOAnalog")
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_analog.png"));

        QString s = QString::fromUtf8(out->get_param("name").c_str());
        s += " (" + QString::fromUtf8(out->get_param("type").c_str()) + ")";
        if (type == "WODigital" || type == "WOAnalog")
                s += " #" + QString::fromUtf8(out->get_param("var").c_str());
        if (type == "WOVolet" || type == "WOVoletSmart")
                s += " #" + QString::fromUtf8(out->get_param("var_up").c_str()) +
                     " #" + QString::fromUtf8(out->get_param("var_down").c_str());

        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
}

QTreeWidgetItemRule *FormRules::addItemRule(Rule *rule, bool selected)
{
        QTreeWidgetItemRule *item = new QTreeWidgetItemRule(rule, ui->tree_rules);

        if (selected) ui->tree_rules->setCurrentItem(item);

        updateItemInfos(item);

        setProjectModified(true);

        return item;
}

void FormRules::updateItemInfos(QTreeWidgetItemRule *item)
{
        Rule *rule = item->getRule();

        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule.png"));
        item->setData(0, Qt::DisplayRole, QString::fromUtf8(rule->get_type().c_str()));
        item->setData(1, Qt::DisplayRole, QString::fromUtf8(rule->get_name().c_str()));
}

QTreeWidgetItem *FormRules::addItemCondition(Condition *condition, bool selected, bool show_popup)
{
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree_condition);

        updateItemCondition(item, condition);

        if (selected)
                ui->tree_condition->setCurrentItem(item);

        if (show_popup)
                on_tree_condition_itemClicked(item, 0);

        setProjectModified(true);

        return item;
}

void FormRules::updateItemCondition(QTreeWidgetItem *item, Condition *condition)
{
        if (condition->getType() == COND_STD)
        {
                string name, oper, value;
                bool isParamValue = false;

                Input *input = NULL;
                if (condition->get_size() > 0)
                        input = condition->get_input(0);
                if (!input) return;

                string id = input->get_param("id");
                if (IOBase::isAudioType(input->get_param("type")) ||
                    IOBase::isCameraType(input->get_param("type")))
                        id = input->get_param("iid");

                name = input->get_param("name");
                oper = condition->get_operator().get_param(id);

                if (oper == "INF") oper = "<";
                if (oper == "INF=") oper = "<=";
                if (oper == "SUP") oper = ">";
                if (oper == "SUP=") oper = ">=";

                if (condition->get_params_var().get_param(id) != "")
                {
                        string var_id = condition->get_params_var().get_param(id);
                        Input *in = ListeRoom::Instance().get_input(var_id);
                        if (in)
                        {
                                value = "";

                                int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(in));
                                string room_name;
                                if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

                                item->setData(2, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(in->get_param("name").c_str()));
                                item->setData(2, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

                                isParamValue = true;
                        }
                        else
                        {
                                /* Wrong param_var, remove it */
                                value = condition->get_params().get_param(id);
                                condition->get_params_var().Delete(id);
                        }
                }
                else
                        value = condition->get_params().get_param(id);

                item->setData(1, Qt::DisplayRole, QString::fromUtf8(oper.c_str()));

                if (!isParamValue)
                {
                        item->setData(2, Qt::DisplayRole, QString::fromUtf8(value.c_str()));

                        item->setData(2, TwoLineItemDelegate::headerTextRole, QVariant());
                        item->setData(2, TwoLineItemDelegate::subHeaderTextRole, QVariant());
                }

                int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(input));
                string room_name;
                if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

                item->setData(0, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(name.c_str()));
                item->setData(0, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

                QString s = "Input: " + QString::fromUtf8(input->get_param("type").c_str()) + " - " + QString::fromUtf8(room_name.c_str());
                item->setData(0, Qt::ToolTipRole, s);
                item->setData(0, Qt::StatusTipRole, s);
                item->setData(1, Qt::ToolTipRole, s);
                item->setData(1, Qt::StatusTipRole, s);
                item->setData(2, Qt::ToolTipRole, s);
                item->setData(2, Qt::StatusTipRole, s);

                item->setFlags(item->flags() | Qt::ItemIsEditable);
                item->setSizeHint(0, QSize(0, 25));

                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule.png"));
        }
        if (condition->getType() == COND_OUTPUT)
        {
                string name, oper, value;
                bool isParamValue = false;

                Output *output = condition->getOutput();
                if (!output) return;

                string id = output->get_param("id");
                if (IOBase::isAudioType(output->get_param("type")) ||
                    IOBase::isCameraType(output->get_param("type")))
                        id = output->get_param("iid");

                name = output->get_param("name");
                oper = condition->getOutputOper();

                if (oper == "INF") oper = "<";
                if (oper == "INF=") oper = "<=";
                if (oper == "SUP") oper = ">";
                if (oper == "SUP=") oper = ">=";

                if (condition->getOutputParamVar() != "")
                {
                        string var_id = condition->getOutputParamVar();
                        Output *out = ListeRoom::Instance().get_output(var_id);
                        if (out)
                        {
                                value = "";

                                int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(out));
                                string room_name;
                                if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

                                item->setData(2, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(out->get_param("name").c_str()));
                                item->setData(2, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

                                isParamValue = true;
                        }
                        else
                        {
                                /* Wrong param_var, remove it */
                                value = condition->getOutputParam();
                                condition->setOutputParamVar("");
                        }
                }
                else
                        value = condition->getOutputParam();


                item->setData(1, Qt::DisplayRole, QString::fromUtf8(oper.c_str()));

                if (!isParamValue)
                {
                        item->setData(2, Qt::DisplayRole, QString::fromUtf8(value.c_str()));

                        item->setData(2, TwoLineItemDelegate::headerTextRole, QVariant());
                        item->setData(2, TwoLineItemDelegate::subHeaderTextRole, QVariant());
                }

                int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(output));
                string room_name;
                if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

                item->setData(0, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(name.c_str()));
                item->setData(0, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

                QString s = "Input: " + QString::fromUtf8(output->get_param("type").c_str()) + " - " + QString::fromUtf8(room_name.c_str());
                item->setData(0, Qt::ToolTipRole, s);
                item->setData(0, Qt::StatusTipRole, s);
                item->setData(1, Qt::ToolTipRole, s);
                item->setData(1, Qt::StatusTipRole, s);
                item->setData(2, Qt::ToolTipRole, s);
                item->setData(2, Qt::StatusTipRole, s);

                item->setFlags(item->flags() | Qt::ItemIsEditable);
                item->setSizeHint(0, QSize(0, 25));

                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule_out.png"));
        }
        else if (condition->getType() == COND_START)
        {
                item->setData(0, Qt::DisplayRole, QString::fromUtf8("Démarrage"));
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule_start.png"));
        }
        else if (condition->getType() == COND_SCRIPT)
        {
                item->setData(0, Qt::DisplayRole, QString::fromUtf8("Script"));
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule_script.png"));
        }
}

QTreeWidgetItem *FormRules::addItemAction(Action *action, bool selected, bool show_popup)
{
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree_action);

        updateItemAction(item, action);

        if (selected) ui->tree_action->setCurrentItem(item);

        if (show_popup)
                on_tree_action_itemClicked(item, 0);

        setProjectModified(true);

        return item;
}

void FormRules::updateItemAction(QTreeWidgetItem *item, Action *action)
{
        if (action->getType() == ACTION_STD)
        {
                string name, value;
                bool isParamValue = false;

                Output *output = NULL;
                if (action->get_size() > 0)
                        output = action->get_output(0);

                if (!output) return;

                string id = output->get_param("id");
                if (IOBase::isAudioType(output->get_param("type")) ||
                    IOBase::isCameraType(output->get_param("type")))
                        id = output->get_param("oid");

                name = output->get_param("name");

                if (action->get_params_var().get_param(id) != "")
                {
                        string var_id = action->get_params_var().get_param(id);
                        Output *out = ListeRoom::Instance().get_output(var_id);
                        if (out)
                        {
                                value = "";

                                int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(out));
                                string room_name;
                                if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

                                item->setData(1, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(out->get_param("name").c_str()));
                                item->setData(1, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

                                isParamValue = true;
                        }
                        else
                        {
                                /* wrong param_var, remove it */
                                value = action->get_params().get_param(id);
                                action->get_params_var().Delete(id);
                        }
                }
                else
                        value = action->get_params().get_param(id);

                if (!isParamValue)
                {
                        item->setData(1, Qt::DisplayRole, QString::fromUtf8(value.c_str()));

                        item->setData(1, TwoLineItemDelegate::headerTextRole, QVariant());
                        item->setData(1, TwoLineItemDelegate::subHeaderTextRole, QVariant());
                }

                int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(output));
                string room_name;
                if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

                item->setData(0, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(name.c_str()));
                item->setData(0, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

                QString s = "Output: " + QString::fromUtf8(output->get_param("type").c_str()) + " - " + QString::fromUtf8(room_name.c_str());
                item->setData(0, Qt::ToolTipRole, s);
                item->setData(0, Qt::StatusTipRole, s);
                item->setData(1, Qt::ToolTipRole, s);
                item->setData(1, Qt::StatusTipRole, s);

                item->setFlags(item->flags() | Qt::ItemIsEditable);
                item->setSizeHint(0, QSize(0, 25));

                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule.png"));
        }
        else if (action->getType() == ACTION_MAIL)
        {
                item->setData(0, Qt::DisplayRole, QString::fromUtf8("E-Mail"));
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule_mail.png"));
        }
        else if (action->getType() == ACTION_SCRIPT)
        {
                item->setData(0, Qt::DisplayRole, QString::fromUtf8("Script"));
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule_script.png"));
        }
        else if (action->getType() == ACTION_TOUCHSCREEN)
        {
                item->setData(0, Qt::DisplayRole, QString::fromUtf8("Ecran tactile"));
                item->setData(1, Qt::DisplayRole, QString::fromUtf8(action->getTouchscreenAction().c_str()));
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule.png"));
        }
}

void FormRules::goSelectRule()
{
        if (!treeItem) return;

        QString filter_text = "inout:";

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                string id = itinput->getInput()->get_param("id");
                if (IOBase::isAudioType(itinput->getInput()->get_param("type")) ||
                    IOBase::isCameraType(itinput->getInput()->get_param("type")))
                        id = itinput->getInput()->get_param("iid");

                filter_text += id.c_str();
        }

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                string id = itoutput->getOutput()->get_param("id");
                if (IOBase::isAudioType(itoutput->getOutput()->get_param("type")) ||
                    IOBase::isCameraType(itoutput->getOutput()->get_param("type")))
                        id = itoutput->getOutput()->get_param("oid");

                filter_text += id.c_str();
        }

        ui->filterEditRules->setText(filter_text);
}

void FormRules::on_tree_home_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
        QTreeWidgetItemRoom *itroom = dynamic_cast<QTreeWidgetItemRoom *>(current);
        if (itroom)
        {
                current_room = itroom->getRoom();
        }

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(current);
        if (itinput)
        {
                itroom = dynamic_cast<QTreeWidgetItemRoom *>(itinput->parent());
                current_room = itroom->getRoom();
        }

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(current);
        if (itoutput)
        {
                itroom = dynamic_cast<QTreeWidgetItemRoom *>(itoutput->parent());
                current_room = itroom->getRoom();
        }
}

void FormRules::showPopup_tree(const QPoint point)
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

                QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
                QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);

                if (ListeRoom::Instance().size() > 1 &&
                    (itoutput || itinput))
                {
                        QMenu *moveMenu = item_menu.addMenu(QString::fromUtf8("Déplacer vers..."));
                        Room *current;

                        if (itinput) current = ListeRoom::Instance().searchRoomByInput(itinput->getInput());
                        if (itoutput) current = ListeRoom::Instance().searchRoomByOutput(itoutput->getOutput());

                        for (int i = 0;i < ListeRoom::Instance().size();i++)
                        {
                                Room *room = ListeRoom::Instance().get_room(i);
                                if (room == current) continue;

                                action = moveMenu->addAction(QString::fromUtf8(room->get_name().c_str()));
                                action->setIcon(QIcon(getIconFromRoom(room)));
                                action->setData(QVariant::fromValue<void *>(room));

                                connect(action, SIGNAL(triggered()), this, SLOT(moveIOToRoom()));
                        }
                }

                action = item_menu.addAction(QString::fromUtf8("Supprimer"));
                action->setIcon(QIcon(":/img/user-trash.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(deleteItem()));

                item_menu.addSeparator();

                //Here we have to add Item action (ON/OFF/UP/DOWN/...)
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
                                if (o->get_param("type") == "WOVoletSmart")
                                {
                                        action = item_menu.addAction(QString::fromUtf8("Convertir en volet classique"));
                                        action->setIcon(QIcon(":/img/icon_shutter.png"));
                                        connect(action, SIGNAL(triggered()), this, SLOT(itemConvertVoletStandard()));
                                }
                                else
                                {
                                        action = item_menu.addAction(QString::fromUtf8("Convertir en volet intelligent"));
                                        action->setIcon(QIcon(":/img/icon_shutter.png"));
                                        connect(action, SIGNAL(triggered()), this, SLOT(itemConvertVoletSmart()));
                                }

                                item_menu.addSeparator();

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
                                action = item_menu.addAction(tr("Convert in triple switch..."));
                                action->setIcon(QIcon(":/img/icon_inter.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemConvertInterTriple()));

                                item_menu.addSeparator();
                        }

                        if (o->get_param("type") == "WIDigitalTriple")
                        {
                                action = item_menu.addAction(QString::fromUtf8("Convert in normal switch..."));
                                action->setIcon(QIcon(":/img/icon_inter.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemConvertInterBP()));

                                item_menu.addSeparator();
                        }

                        if (o->get_param("type") == "WITemp" || o->get_param("type") == "OWTemp")
                        {
                                action = item_menu.addAction(QString::fromUtf8("Associer à une consigne..."));
                                action->setIcon(QIcon(":/img/icon_temp.png"));
                                connect(action, SIGNAL(triggered()), this, SLOT(itemTempWizard()));

                                item_menu.addSeparator();
                        }
                }

                action = item_menu.addAction(QString::fromUtf8("Propriétés"));
                action->setIcon(QIcon(":/img/document-properties.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(showPropertiesItem()));

                item_menu.exec(QCursor::pos());
        }
}

void FormRules::moveIOToRoom()
{
        if (!treeItem) return;

        QAction *action = reinterpret_cast<QAction *>(sender());
        if (!action) return;

        Room *to_room = static_cast<Room *>(action->data().value<void *>());
        if (!to_room) return;

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                Output *output = itoutput->getOutput();
                if (output)
                {
                        int _i = ListeRoom::Instance().searchIO(output);
                        if (_i < 0) return;
                        Room *old_room = ListeRoom::Instance().get_room(_i);

                        if (old_room == to_room) return;

                        //delete old item
                        old_room->RemoveOutput(output);

                        delete treeItem;

                        to_room->AddOutput(output);
                        addItemOutput(output, to_room, true);

                        return;
                }
        }

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                Input *input = itinput->getInput();
                if (input)
                {
                        int _i = ListeRoom::Instance().searchIO(input);
                        if (_i < 0) return;
                        Room *old_room = ListeRoom::Instance().get_room(_i);

                        if (old_room == to_room) return;

                        //delete old item
                        old_room->RemoveInput(input);

                        delete treeItem;

                        to_room->AddInput(input);
                        addItemInput(input, to_room, true);

                        return;
                }
        }
}

void FormRules::showPopup_condition(const QPoint point)
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

void FormRules::showPopup_action(const QPoint point)
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

void FormRules::deleteItem()
{
        if (!treeItem) return;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calaos Installer"),
                              QString::fromUtf8("Etes vous sûr de vouloir supprimer l'élément \"%1\"").arg(treeItem->text(0)),
                              QMessageBox::Yes | QMessageBox::No);

        if (reply != QMessageBox::Yes)
                return;

        setProjectModified(true);

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

void FormRules::showPropertiesItem()
{
        Params *p = NULL;
        Params proom, prule;
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
                proom.Add("hits", to_string(itroom->getRoom()->get_hits()));
                p = &proom;
                type = OBJ_ROOM;
        }

        QTreeWidgetItemRule *itrule = dynamic_cast<QTreeWidgetItemRule *>(treeItem);
        if (itrule)
        {
                prule.Add("name", itrule->getRule()->get_name());
                prule.Add("type", itrule->getRule()->get_type());
                p = &prule;
                type = OBJ_RULE;
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

                        int hits;
                        from_string((*p)["hits"], hits);
                        room->set_hits(hits);

                        updateItemInfos(itroom);
                }
                else if (type == OBJ_INPUT)
                {
                        updateItemInfos(itinput);

                        //Handle change of wago_841 parameter and change corresponding cache
                        if (itinput->getInput()->get_param("type") == "WIDigitalBP" ||
                            itinput->getInput()->get_param("type") == "WIDigitalTriple" ||
                            itinput->getInput()->get_param("type") == "WIDigitalLong")
                        {
                                if (itinput->getInput()->get_param("wago_841") != "true")
                                        ProjectManager::wagoTypeCache[itinput->getInput()->get_param("host")] = false;
                                else
                                        ProjectManager::wagoTypeCache[itinput->getInput()->get_param("host")] = true;
                        }
                }
                else if (type == OBJ_OUTPUT)
                {
                        updateItemInfos(itoutput);

                        //Handle change of wago_841 parameter and change corresponding cache
                        if (itoutput->getOutput()->get_param("type") == "WODigital" ||
                            itoutput->getOutput()->get_param("type") == "WOVolet" ||
                            itoutput->getOutput()->get_param("type") == "WOVoletSmart")
                        {
                                if (itoutput->getOutput()->get_param("wago_841") != "true")
                                        ProjectManager::wagoTypeCache[itoutput->getOutput()->get_param("host")] = false;
                                else
                                        ProjectManager::wagoTypeCache[itoutput->getOutput()->get_param("host")] = true;
                        }
                }
                else if (type == OBJ_RULE)
                {
                        Rule *rule = itrule->getRule();
                        string n;
                        n = (*p)["name"];
                        rule->set_name(n);
                        n = (*p)["type"];
                        rule->set_type(n);

                        updateItemInfos(itrule);
                }

                setProjectModified(true);
        }
}

void FormRules::on_tree_home_itemDoubleClicked(QTreeWidgetItem* item, int)
{
        treeItem = item;
        showPropertiesItem();
}

void FormRules::on_bt_rules_add_clicked()
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

void FormRules::on_tree_rules_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
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
                        addItemCondition(rule->get_condition(i));
                }

                for (int i = 0;i < rule->get_size_actions();i++)
                {
                        addItemAction(rule->get_action(i));
                }
        }
}

Rule *FormRules::getCurrentRule()
{
        QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(ui->tree_rules->currentItem());
        if (item)
                return item->getRule();

        return NULL;
}

void FormRules::on_bt_rules_del_clicked()
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

                        setProjectModified(true);
                }
        }
}

void FormRules::itemLightOn()
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

void FormRules::itemLightOff()
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

void FormRules::itemShowCamera()
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

void FormRules::itemPlagesHoraires()
{
        if (!treeItem) return;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                if (itinput->getInput()->get_param("type") == "InPlageHoraire")
                {
                        DialogPlageHoraire d(dynamic_cast<InPlageHoraire *>(itinput->getInput()));
                        d.exec();

                        setProjectModified(true);
                }
        }
}

void FormRules::itemConvertInterTriple()
{
        if (!treeItem) return;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                if (itinput->getInput()->get_param("type") == "WIDigitalBP")
                {
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(this, tr("Calaos Installer"),
                                              QString::fromUtf8("Etes vous sûr de vouloir convertir en interrupteur triple?"),
                                              QMessageBox::Yes | QMessageBox::No);

                        if (reply == QMessageBox::Yes)
                        {
                                Input *input = itinput->getInput();

                                input->get_params().Add("type", "WIDigitalTriple");
                                updateItemInfos(itinput);

                                goSelectRule();

                                QMessageBox::StandardButton reply;
                                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                                      QString::fromUtf8("Voulez vous transformer automatiquement les règles?"),
                                                      QMessageBox::Yes | QMessageBox::No);

                                if (reply == QMessageBox::Yes)
                                {
                                        QTreeWidgetItemIterator it(ui->tree_rules);
                                        while (*it)
                                        {
                                                QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
                                                if (!item)
                                                {
                                                        ++it;
                                                        continue;
                                                }

                                                Rule *rule = item->getRule();

                                                for (int i = 0;i < rule->get_size_conds();i++)
                                                {
                                                        Condition *cond = rule->get_condition(i);
                                                        for (int j = 0;j < cond->get_size();j++)
                                                        {
                                                                if (input == cond->get_input(j))
                                                                {
                                                                        cond->get_params().Add(input->get_param("id"), "1");
                                                                }
                                                        }
                                                }

                                                ++it;
                                        }

                                        //refresh ui
                                        on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);
                                }

                                setProjectModified(true);
                        }
                }
        }
}

void FormRules::itemConvertInterBP()
{
        if (!treeItem) return;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                if (itinput->getInput()->get_param("type") == "WIDigitalTriple")
                {
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(this, tr("Calaos Installer"),
                                              QString::fromUtf8("Etes vous sûr de vouloir convertir en interrupteur classique?"),
                                              QMessageBox::Yes | QMessageBox::No);

                        if (reply == QMessageBox::Yes)
                        {
                                Input *input = itinput->getInput();

                                input->get_params().Add("type", "WIDigitalBP");
                                updateItemInfos(itinput);

                                goSelectRule();

                                QMessageBox::StandardButton reply;
                                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                                      QString::fromUtf8("Voulez vous transformer automatiquement les règles?"),
                                                      QMessageBox::Yes | QMessageBox::No);

                                if (reply == QMessageBox::Yes)
                                {
                                        bool more_click_found = false;

                                        QTreeWidgetItemIterator it(ui->tree_rules);
                                        while (*it)
                                        {
                                                QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
                                                if (!item)
                                                {
                                                        ++it;
                                                        continue;
                                                }

                                                Rule *rule = item->getRule();

                                                for (int i = 0;i < rule->get_size_conds();i++)
                                                {
                                                        Condition *cond = rule->get_condition(i);
                                                        for (int j = 0;j < cond->get_size();j++)
                                                        {
                                                                if (input == cond->get_input(j))
                                                                {
                                                                        if (cond->get_params().get_param(input->get_param("id")) == "1")
                                                                                cond->get_params().Add(input->get_param("id"), "true");
                                                                        else
                                                                                more_click_found = true;
                                                                }
                                                        }
                                                }

                                                ++it;
                                        }

                                        if (more_click_found)
                                        {
                                                QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Attention, les règles double/triple click n'ont pas été convertis."));
                                        }

                                        //refresh ui
                                        on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);
                                }


                                setProjectModified(true);
                        }
                }
        }
}

void FormRules::itemTempWizard()
{
        if (!treeItem) return;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
        if (itinput)
        {
                if (itinput->getInput()->get_param("type") == "WITemp" ||
                    itinput->getInput()->get_param("type") == "OWTemp")
                {
                        TempWizard wizard;

                        if (wizard.exec() == QDialog::Accepted)
                        {
                                setProjectModified(true);

                                QString name = wizard.field("consigneName").toString();
                                bool create_rules = wizard.field("checkRules").toBool();

                                //Search for an InternalInt or create one
                                Room *room = ListeRoom::Instance().searchRoomByInput(itinput->getInput());

                                if (!room)
                                {
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur interne !"));

                                        return;
                                }

                                Input *consigne = NULL;
                                for (int i = 0;i < room->get_size_in();i++)
                                {
                                        Input *in = room->get_input(i);
                                        if (in->get_param("type") == "InternalInt" &&
                                            in->get_param("name") == name.toUtf8().data())
                                        {
                                                consigne = in;

                                                break;
                                        }
                                }

                                if (!consigne)
                                {
                                        Params p;
                                        p.Add("name", name.toUtf8().constData());
                                        p.Add("type", "InternalInt");

                                        consigne = ListeRoom::Instance().createInput(p, room);

                                        Output *output = dynamic_cast<Output *>(consigne);
                                        if (output) addItemOutput(output, room, true);
                                }

                                if (!consigne)
                                {
                                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Erreur interne !"));

                                        return;
                                }

                                //Link both together
                                int num = 0;
                                while (ListeRoom::Instance().get_chauffage_var(Utils::to_string(num), CONSIGNE))
                                {
                                        num++;
                                }

                                consigne->set_param("chauffage_id", Utils::to_string(num));
                                itinput->getInput()->set_param("chauffage_id", Utils::to_string(num));

                                consigne->set_param("value", "20");
                                consigne->set_param("rw", "true");
                                consigne->set_param("visible", "true");

                                //Do we need to create the rules?
                                if (create_rules)
                                {
                                        //The rule (if input <= consigne)
                                        {
                                                Rule *rule = new Rule(room->get_name(), "chauffage on");
                                                addItemRule(rule, true);

                                                Condition *cond = new Condition(COND_STD);
                                                cond->Add(itinput->getInput());
                                                cond->get_params().Add(itinput->getInput()->get_param("id"), "");
                                                cond->get_operator().Add(itinput->getInput()->get_param("id"), "INF=");
                                                cond->get_params_var().Add(itinput->getInput()->get_param("id"),
                                                                           consigne->get_param("id"));

                                                rule->AddCondition(cond);
                                                addItemCondition(cond, true, true);

                                                ListeRule::Instance().Add(rule);
                                        }

                                        //The rule (if input > consigne)
                                        {
                                                Rule *rule = new Rule(room->get_name(), "chauffage off");
                                                addItemRule(rule, true);

                                                Condition *cond = new Condition(COND_STD);
                                                cond->Add(itinput->getInput());
                                                cond->get_params().Add(itinput->getInput()->get_param("id"), "");
                                                cond->get_operator().Add(itinput->getInput()->get_param("id"), "SUP");
                                                cond->get_params_var().Add(itinput->getInput()->get_param("id"),
                                                                           consigne->get_param("id"));

                                                rule->AddCondition(cond);
                                                addItemCondition(cond, true, true);

                                                ListeRule::Instance().Add(rule);
                                        }
                                }
                        }
                }
        }
}

void FormRules::itemVoletUp()
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

void FormRules::itemVoletDown()
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

void FormRules::itemVoletStop()
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

void FormRules::itemConvertVoletSmart()
{
        if (!treeItem) return;

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                if (itoutput->getOutput()->get_param("type") == "WOVolet")
                {
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(this, tr("Calaos Installer"),
                                              QString::fromUtf8("Etes vous sûr de vouloir convertir en volet intelligent?"),
                                              QMessageBox::Yes | QMessageBox::No);

                        if (reply == QMessageBox::Yes && itoutput->getOutput())
                        {
                                Output *out = itoutput->getOutput();
                                out->get_params().Add("type", "WOVoletSmart");

                                if (!out->get_params().Exists("time_up"))
                                        out->get_params().Add("time_up", out->get_param("time"));
                                if (!out->get_params().Exists("time_down"))
                                        out->get_params().Add("time_down", out->get_param("time"));

                                if (!out->get_params().Exists("var_save")) out->get_params().Add("var_save", ListeRoom::get_new_varsave());

                                updateItemInfos(itoutput);
                                setProjectModified(true);
                        }
                }
        }
}

void FormRules::itemConvertVoletStandard()
{
        if (!treeItem) return;

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        if (itoutput)
        {
                if (itoutput->getOutput()->get_param("type") == "WOVoletSmart")
                {
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(this, tr("Calaos Installer"),
                                              QString::fromUtf8("Etes vous sûr de vouloir convertir en volet classique?"),
                                              QMessageBox::Yes | QMessageBox::No);

                        if (reply == QMessageBox::Yes && itoutput->getOutput())
                        {
                                Output *out = itoutput->getOutput();
                                out->get_params().Add("type", "WOVolet");

                                if (!out->get_params().Exists("time"))
                                        out->get_params().Add("time", out->get_param("time_up"));

                                out->get_params().Delete("var_save");

                                updateItemInfos(itoutput);
                                setProjectModified(true);
                        }
                }
        }
}

void FormRules::on_filterEditHome_textChanged(QString filter_text)
{
        if (filter_text.isEmpty())
        {
                QTreeWidgetItemIterator it(ui->tree_home);
                while (*it)
                {
                        QTreeWidgetItem *item = (*it);
                        item->setHidden(false);
                        ++it;
                }

                return;
        }

        QTreeWidgetItemIterator it(ui->tree_home);
        while (*it)
        {
                QTreeWidgetItemRoom *item = dynamic_cast<QTreeWidgetItemRoom *>(*it);
                if (!item)
                {
                        ++it;
                        continue;
                }

                bool hideItem = true;

                QStringList searchList;
                searchList.push_back(item->getRoom()->get_name().c_str());
                searchList.push_back(item->getRoom()->get_type().c_str());

                if (!searchList.filter(filter_text, Qt::CaseInsensitive).isEmpty())
                        hideItem = false;

                item->setHidden(hideItem);

                ++it;
        }
}

void FormRules::on_filterEditRules_textChanged(QString filter_text)
{
        if (filter_text.isEmpty())
        {
                QTreeWidgetItemIterator it(ui->tree_rules);
                while (*it)
                {
                        QTreeWidgetItem *item = (*it);
                        item->setHidden(false);
                        ++it;
                }

                return;
        }

        QString filter_type = filter_text.section(":", 0, 0);

        QTreeWidgetItemRule *item_selected = NULL;

        if (filter_type.toLower() == "output" ||
            filter_type.toLower() == "input" ||
            filter_type.toLower() == "inout")
        {
                filter_text.remove(0, filter_type.length() + 1);

                QTreeWidgetItemIterator it(ui->tree_rules);
                while (*it)
                {
                        QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
                        if (!item)
                        {
                                ++it;
                                continue;
                        }

                        Rule *rule = item->getRule();

                        QStringList searchList;

                        if (filter_type.toLower() == "output" ||
                            filter_type.toLower() == "inout")
                        {
                                for (int i = 0;i < rule->get_size_actions();i++)
                                {
                                        Action *action = rule->get_action(i);
                                        for (int j = 0;j < action->get_size();j++)
                                        {
                                                searchList << action->get_output(j)->get_param("name").c_str();

                                                string id = action->get_output(j)->get_param("id");
                                                if (IOBase::isAudioType(action->get_output(j)->get_param("type")) ||
                                                    IOBase::isCameraType(action->get_output(j)->get_param("type")))
                                                        id = action->get_output(j)->get_param("oid");

                                                searchList << id.c_str();
                                        }
                                }
                        }

                        if (filter_type.toLower() == "input" ||
                            filter_type.toLower() == "inout")
                        {
                                for (int i = 0;i < rule->get_size_conds();i++)
                                {
                                        Condition *cond = rule->get_condition(i);
                                        for (int j = 0;j < cond->get_size();j++)
                                        {
                                                searchList << cond->get_input(j)->get_param("name").c_str();

                                                string id = cond->get_input(j)->get_param("id");
                                                if (IOBase::isAudioType(cond->get_input(j)->get_param("type")) ||
                                                    IOBase::isCameraType(cond->get_input(j)->get_param("type")))
                                                        id = cond->get_input(j)->get_param("iid");

                                                searchList << id.c_str();
                                        }
                                }
                        }

                        bool hideItem = true;

                        QString str;
                        foreach (str, searchList)
                        {
                                if (str == filter_text)
                                        hideItem = false;
                        }

                        item->setHidden(hideItem);

                        if (!hideItem && !item_selected)
                                item_selected = item;


                        ++it;
                }

                ui->tree_rules->setCurrentItem(item_selected);

                return;
        }

        QTreeWidgetItemIterator it(ui->tree_rules);
        while (*it)
        {
                QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
                if (!item)
                {
                        ++it;
                        continue;
                }

                bool hideItem = true;

                QStringList searchList;
                searchList.push_back(item->getRule()->get_name().c_str());
                searchList.push_back(item->getRule()->get_type().c_str());

                if (!searchList.filter(filter_text, Qt::CaseInsensitive).isEmpty())
                        hideItem = false;

                item->setHidden(hideItem);

                if (!hideItem && !item_selected)
                        item_selected = item;

                ++it;
        }

        ui->tree_rules->setCurrentItem(item_selected);
}

void FormRules::on_tree_condition_itemClicked(QTreeWidgetItem* item, int column)
{
        Rule *rule = getCurrentRule();
        if (!rule) return;

        int num = ui->tree_condition->invisibleRootItem()->indexOfChild(ui->tree_condition->currentItem());

        if (num < 0 || num >= rule->get_size_conds())
                return;

        Condition *cond = rule->get_condition(num);

        setProjectModified(true);

        switch (cond->getType())
        {
        case COND_STD:
        case COND_OUTPUT:
                {
                        popupConditionStd->setCondition(item, rule, cond);
                        popupConditionStd->layout()->update();
                        popupConditionStd->move(ui->tree_condition->mapToGlobal(QPoint(0 - popupConditionStd->width(), 0)));
                        popupConditionStd->setFocus();
                        popupConditionStd->show();

                        break;
                }
        case COND_START:
                {
                        popupConditionStart->setCondition(item, rule, cond);
                        popupConditionStart->layout()->update();
                        popupConditionStart->move(ui->tree_condition->mapToGlobal(QPoint(0 - popupConditionStart->width(), 0)));
                        popupConditionStart->setFocus();
                        popupConditionStart->show();

                        break;
                }
        case COND_SCRIPT:
                {
                        popupConditionScript->setCondition(item, rule, cond);
                        popupConditionScript->layout()->update();
                        popupConditionScript->move(ui->tree_condition->mapToGlobal(QPoint(0 - popupConditionScript->width(), 0)));
                        popupConditionScript->setFocus();
                        popupConditionScript->show();

                        break;
                }
        }

}

void FormRules::on_tree_action_itemClicked(QTreeWidgetItem* item, int column)
{
        Rule *rule = getCurrentRule();
        if (!rule) return;

        int num = ui->tree_action->invisibleRootItem()->indexOfChild(ui->tree_action->currentItem());

        if (num < 0 || num >= rule->get_size_actions())
                return;

        Action *action = rule->get_action(num);

        setProjectModified(true);

        switch (action->getType())
        {
        case ACTION_STD:
                {
                        popupActionStd->setAction(item, rule, action);
                        popupActionStd->layout()->update();
                        popupActionStd->move(ui->tree_action->mapToGlobal(QPoint(0 - popupActionStd->width(), 0)));
                        popupActionStd->setFocus();
                        popupActionStd->show();

                        break;
                }
        case ACTION_MAIL:
                {
                        popupActionMail->setAction(item, rule, action);
                        popupActionMail->layout()->update();
                        popupActionMail->move(ui->tree_action->mapToGlobal(QPoint(0 - popupActionMail->width(), 0)));
                        popupActionMail->setFocus();
                        popupActionMail->show();

                        break;
                }
        case ACTION_SCRIPT:
                {
                        popupActionScript->setAction(item, rule, action);
                        popupActionScript->layout()->update();
                        popupActionScript->move(ui->tree_action->mapToGlobal(QPoint(0 - popupActionScript->width(), 0)));
                        popupActionScript->setFocus();
                        popupActionScript->show();

                        break;
                }
        case ACTION_TOUCHSCREEN:
                {
                        popupActionTouchscreen->setAction(item, rule, action);
                        popupActionTouchscreen->layout()->update();
                        popupActionTouchscreen->move(ui->tree_action->mapToGlobal(QPoint(0 - popupActionTouchscreen->width(), 0)));
                        popupActionTouchscreen->setFocus();
                        popupActionTouchscreen->show();

                        break;
                }
        }

}

void FormRules::on_bt_condition_del_clicked()
{
        treeItem_condition = ui->tree_condition->currentItem();

        deleteItemCondition();

        setProjectModified(true);
}

void FormRules::on_bt_action_del_clicked()
{
        treeItem_action = ui->tree_action->currentItem();

        deleteItemAction();

        setProjectModified(true);
}

void FormRules::showPopup_rule(const QPoint point)
{
        QTreeWidgetItem *item = NULL;
        item = ui->tree_rules->itemAt(point);

        if (!item)
                return;

        treeItem = item;

        QMenu item_menu(ui->tree_rules);

        QAction *action = NULL;

        action = item_menu.addAction(QString::fromUtf8("Propriétés"));
        action->setIcon(QIcon(":/img/document-properties.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(showPropertiesItem()));
        item_menu.addSeparator();
        action = item_menu.addAction(QString::fromUtf8("Supprimer"));
        action->setIcon(QIcon(":/img/user-trash.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(on_bt_rules_del_clicked()));

        item_menu.exec(QCursor::pos());
}

void FormRules::on_tree_rules_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
        treeItem = ui->tree_rules->currentItem();
        showPropertiesItem();
}

void FormRules::addCondition(int type)
{
        if (type == COND_STD)
        {
                Input *input = NULL;
                QTreeWidgetItemInput *initem = dynamic_cast<QTreeWidgetItemInput *>(ui->tree_home->currentItem());
                if (!initem)
                {
                        QTreeWidgetItemOutput *outitem = dynamic_cast<QTreeWidgetItemOutput *>(ui->tree_home->currentItem());
                        if (outitem)
                        {
                                string type = outitem->getOutput()->get_param("type");
                                if (type == "InputTimer" || type == "scenario" ||
                                    type == "Scenario" || type == "InternalBool" ||
                                    type == "InternalInt" || type == "InternalString")
                                {
                                        input = ListeRoom::Instance().get_input(outitem->getOutput()->get_param("id"));
                                }

                                if (IOBase::isAudioType(type))
                                        input = ListeRoom::Instance().get_input(outitem->getOutput()->get_param("iid"));
                                if (IOBase::isCameraType(type))
                                        input = ListeRoom::Instance().get_input(outitem->getOutput()->get_param("iid"));
                        }
                }
                else
                {
                        input = initem->getInput();
                }

                if (!input)
                {
                        QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Vous devez sélectionner une entrée."));
                        return;
                }

                string id = input->get_param("id");
                if (IOBase::isAudioType(input->get_param("type")) ||
                    IOBase::isCameraType(input->get_param("type")))
                        id = input->get_param("iid");

                Rule *rule = getCurrentRule();
                if (!rule) return;

                Condition *cond = new Condition(COND_STD);
                cond->Add(input);

                cond->get_operator().Add(id, "==");

                if (input->get_param("type") == "WIDigitalBP" || input->get_param("type") == "InputTime" ||
                    input->get_param("type") == "InputTimer" || input->get_param("type") == "Scenario" ||
                    input->get_param("type") == "scenario" || input->get_param("type") == "InPlageHoraire" ||
                    input->get_param("type") == "InternalBool")
                      cond->get_params().Add(id, "true");
                else if (input->get_param("type") == "WIDigitalTriple" ||
                         input->get_param("type") == "WIDigitalLong")
                      cond->get_params().Add(id, "1");

                rule->AddCondition(cond);

                addItemCondition(cond, true, true);

                setProjectModified(true);
        }
        if (type == COND_OUTPUT)
        {
                QTreeWidgetItemOutput *initem = dynamic_cast<QTreeWidgetItemOutput *>(ui->tree_home->currentItem());
                if (!initem)
                {
                        QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Vous devez sélectionner une sortie."));
                        return;
                }

                Output *output = initem->getOutput();
                if (!output) return;

                string id = output->get_param("id");
                if (IOBase::isAudioType(output->get_param("type")) ||
                    IOBase::isCameraType(output->get_param("type")))
                        id = output->get_param("iid");

                Rule *rule = getCurrentRule();
                if (!rule) return;

                Condition *cond = new Condition(COND_OUTPUT);
                cond->setOutput(output);
                cond->setOutputOper("==");
                cond->setOutputParam("changed");

                rule->AddCondition(cond);

                addItemCondition(cond, true, true);

                setProjectModified(true);
        }
        else if (type == COND_START)
        {
                Rule *rule = getCurrentRule();
                if (!rule) return;

                Condition *cond = new Condition(COND_START);

                rule->AddCondition(cond);

                addItemCondition(cond, true, true);

                setProjectModified(true);
        }
        else if (type == COND_SCRIPT)
        {
                Rule *rule = getCurrentRule();
                if (!rule) return;

                Condition *cond = new Condition(COND_SCRIPT);

                rule->AddCondition(cond);

                addItemCondition(cond, true, true);

                setProjectModified(true);
        }
}

void FormRules::addAction(int type)
{
        if (type == ACTION_STD)
        {
                QTreeWidgetItemOutput *outitem = dynamic_cast<QTreeWidgetItemOutput *>(ui->tree_home->currentItem());
                if (!outitem)
                {
                        QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Vous devez sélectionner une sortie."));
                        return;
                }

                Output *output = outitem->getOutput();
                if (!output) return;

                string id = output->get_param("id");
                if (IOBase::isAudioType(output->get_param("type")) ||
                    IOBase::isCameraType(output->get_param("type")))
                        id = output->get_param("oid");

                Rule *rule = getCurrentRule();
                if (!rule) return;

                Action *action = new Action(ACTION_STD);

                action->Add(output);

                if (output->get_param("type") == "WODigital" || output->get_param("type") == "WODali" ||
                    output->get_param("type") == "WODaliRVB" || output->get_param("type") == "WONeon" ||
                    output->get_param("type") == "WOVolet" || output->get_param("type") == "WOVoletSmart")
                      action->get_params().Add(id, "toggle");
                else if (output->get_param("type") == "scenario" || output->get_param("type") == "InputTimer")
                      action->get_params().Add(id, "true");

                rule->AddAction(action);

                addItemAction(action, true, true);

                setProjectModified(true);
        }
        else if (type == ACTION_MAIL)
        {
                Rule *rule = getCurrentRule();
                if (!rule) return;

                Action *action = new Action(ACTION_MAIL);

                rule->AddAction(action);

                addItemAction(action, true, true);

                setProjectModified(true);
        }
        else if (type == ACTION_SCRIPT)
        {
                Rule *rule = getCurrentRule();
                if (!rule) return;

                Action *action = new Action(ACTION_SCRIPT);

                rule->AddAction(action);

                addItemAction(action, true, true);

                setProjectModified(true);
        }
        else if (type == ACTION_TOUCHSCREEN)
        {
                Rule *rule = getCurrentRule();
                if (!rule) return;

                Action *action = new Action(ACTION_TOUCHSCREEN);

                rule->AddAction(action);

                addItemAction(action, true, true);

                setProjectModified(true);
        }
}

void FormRules::deleteItemCondition()
{
        if (!treeItem_condition) return;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calaos Installer"),
                              QString::fromUtf8("Etes vous sûr de vouloir supprimer l'élément \"%1\"").arg(treeItem_condition->data(0, TwoLineItemDelegate::headerTextRole).toString()),
                              QMessageBox::Yes | QMessageBox::No);

        if (reply != QMessageBox::Yes)
                return;

        Rule *rule = getCurrentRule();
        if (!rule) return;

        int num = ui->tree_condition->invisibleRootItem()->indexOfChild(ui->tree_condition->currentItem());

        if (num < 0 || num >= rule->get_size_conds())
                return;

        setProjectModified(true);

        rule->RemoveCondition(num);

        QTreeWidgetItem *item = ui->tree_rules->selectedItems().first();
        ui->tree_rules->setCurrentItem(NULL);
        ui->tree_rules->setCurrentItem(item);
}

void FormRules::deleteItemAction()
{
        if (!treeItem_action) return;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calaos Installer"),
                              QString::fromUtf8("Etes vous sûr de vouloir supprimer l'élément \"%1\"").arg(treeItem_action->data(0, TwoLineItemDelegate::headerTextRole).toString()),
                              QMessageBox::Yes | QMessageBox::No);

        if (reply != QMessageBox::Yes)
                return;

        Rule *rule = getCurrentRule();
        if (!rule) return;

        int num = ui->tree_action->invisibleRootItem()->indexOfChild(ui->tree_action->currentItem());

        if (num < 0 || num >= rule->get_size_actions())
                return;

        setProjectModified(true);

        rule->RemoveAction(num);

        QTreeWidgetItem *item = ui->tree_rules->selectedItems().first();
        ui->tree_rules->setCurrentItem(NULL);
        ui->tree_rules->setCurrentItem(item);
}

void FormRules::on_bt_condition_up_clicked()
{
        treeItem_condition = ui->tree_condition->currentItem();
        if (!treeItem_condition) return;

        Rule *rule = getCurrentRule();
        if (!rule) return;

        int num = ui->tree_condition->invisibleRootItem()->indexOfChild(ui->tree_condition->currentItem());

        if (num < 0 || num >= rule->get_size_conds())
                return;

        rule->MoveConditionUp(num);

        on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);

        setProjectModified(true);
}

void FormRules::on_bt_condition_down_clicked()
{
        treeItem_condition = ui->tree_condition->currentItem();
        if (!treeItem_condition) return;

        Rule *rule = getCurrentRule();
        if (!rule) return;

        int num = ui->tree_condition->invisibleRootItem()->indexOfChild(ui->tree_condition->currentItem());

        if (num < 0 || num >= rule->get_size_conds())
                return;

        rule->MoveConditionDown(num);

        on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);

        setProjectModified(true);
}

void FormRules::on_bt_action_up_clicked()
{
        treeItem_action = ui->tree_action->currentItem();
        if (!treeItem_action) return;

        Rule *rule = getCurrentRule();
        if (!rule) return;

        int num = ui->tree_action->invisibleRootItem()->indexOfChild(ui->tree_action->currentItem());

        if (num < 0 || num >= rule->get_size_actions())
                return;

        rule->MoveActionUp(num);

        on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);

        setProjectModified(true);
}

void FormRules::on_bt_action_down_clicked()
{
        treeItem_action = ui->tree_action->currentItem();
        if (!treeItem_action) return;

        Rule *rule = getCurrentRule();
        if (!rule) return;

        int num = ui->tree_action->invisibleRootItem()->indexOfChild(ui->tree_action->currentItem());

        if (num < 0 || num >= rule->get_size_actions())
                return;

        rule->MoveActionDown(num);

        on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);

        setProjectModified(true);
}
