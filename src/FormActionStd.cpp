#include "FormActionStd.h"
#include "ui_FormActionStd.h"

#include "formrules.h"

FormActionStd::FormActionStd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormActionStd),
    rule(NULL),
    action(NULL),
    actionMenu(NULL)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Popup);
    setFocusPolicy(Qt::NoFocus);
    setFocusProxy(parent);
    setMouseTracking(true);

    actionMenu = new QMenu(this);
    actionMenu->setProperty("class", QVariant("ActionMenu"));
}

FormActionStd::~FormActionStd()
{
    delete ui;
}

void FormActionStd::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FormActionStd::setAction(QTreeWidgetItem *item, Rule *_rule, Action *_action)
{
    rule = _rule;
    action = _action;
    qitem = item;

    ui->labelRuleName->setText(QString::fromUtf8(rule->get_name().c_str()));
    ui->labelRuleType->setText(QString::fromUtf8(rule->get_type().c_str()));

    IOBase *output = NULL;
    if (action->get_size() > 0)
        output = action->get_output(0);

    if (!output) return;

    onStart = true;

    string type = output->get_param("type");
    string gtype = output->get_gui_type();
    string id = output->get_param("id");
    if (output->get_gui_type() == "audio" ||
        output->get_gui_type() == "camera")
        id = output->get_param("oid");

    //Search room icon
    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *room = ListeRoom::Instance().get_room(i);
        for (int j = 0;j < room->get_size_out();j++)
        {
            if (room->get_output(j) == output)
            {
                ui->labelRoomName->setText(QString::fromUtf8(room->get_name().c_str()));

                if (room->get_type() == "salon" || room->get_type() == "lounge")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/lounge_small.png"));
                else if (room->get_type() == "sdb" || room->get_type() == "bathroom")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/bathroom_small.png"));
                else if (room->get_type() == "chambre" || room->get_type() == "bedroom")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/bedroom_small.png"));
                else if (room->get_type() == "cave" || room->get_type() == "cellar")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/cellar_small.png"));
                else if (room->get_type() == "couloir" || room->get_type() == "hall" || room->get_type() == "corridor")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/corridor_small.png"));
                else if (room->get_type() == "sam" || room->get_type() == "diningroom")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/diningroom_small.png"));
                else if (room->get_type() == "garage")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/garage_small.png"));
                else if (room->get_type() == "cuisine" || room->get_type() == "kitchen")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/kitchen_small.png"));
                else if (room->get_type() == "bureau" || room->get_type() == "office")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/office_small.png"));
                else if (room->get_type() == "exterieur" || room->get_type() == "outside")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/outside_small.png"));
                else if (room->get_type() == "misc" || room->get_type() == "divers" || room->get_type() == "various")
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/various_small.png"));
                else
                    ui->labelRoomIcon->setPixmap(QPixmap(":/img/rooms/various_small.png"));

                break;
            }
        }
    }

    ui->labelItemName->setText(QString::fromUtf8(output->get_param("name").c_str()));

    ui->editValue->clear();
    ui->editValue->setText(QString::fromUtf8(action->get_params().get_param(id).c_str()));

    actionMenu->clear();
    ui->buttonMore->setEnabled(true);

    if (gtype == "timer")
    {
        addActionMenu("start / true", tr("Start timer"), "true");
        addActionMenu("stop / false", tr("Stop timer"), "false");
        addActionMenu("h:m:s:ms", tr("Change timer duration"), "0:0:0:200");
    }
    else if (gtype == "scenario")
    {
        addActionMenu("true", tr("Start scenario"), "true");
        addActionMenu("false", tr("Stop scenario (if possible)"), "false");
    }
    else if (gtype == "var_bool")
    {
        addActionMenu("true", tr("Set state to false"), "true");
        addActionMenu("false", tr("Set state to true"), "false");
        addActionMenu("toggle", tr("Toggle state"), "toggle");
    }
    else if (gtype == "shutter" || gtype == "shutter_smart")
    {
        addActionMenu("up", tr("Open shutter"), "up");
        addActionMenu("down", tr("Close shutter"), "down");
        addActionMenu("stop", tr("Stop the shutter"), "stop");
        addActionMenu("toggle", tr("Toggle shutter state"), "toggle");
        addActionMenu("impulse up X", tr("Open shutter for X ms"), "impulse up 200");
        addActionMenu("impulse down X", tr("Close shutter for X ms"), "impulse down 200");

        if (gtype == "shutter_smart")
        {
            addActionMenu("set X", tr("Set shutter position to X percent"), "set 50");
            addActionMenu("up X", tr("Open shutter for X percent"), "up 2");
            addActionMenu("down X", tr("Close shutter for X percent"), "down 2");
            addActionMenu("calibrate", tr("Start shutter calibration"), "calibrate");
        }
    }
    else if (gtype == "light" ||
             gtype == "light_dimmer" ||
             gtype == "light_rgb")
    {
        addActionMenu("true", tr("Switch light on"), "true");
        addActionMenu("false", tr("Switch light off"), "false");
        addActionMenu("toggle", tr("Toggle light status"), "toggle");

        if (gtype == "light" || gtype == "light_dimmer")
        {
            addActionMenu("impulse X", tr("Switch light on for X ms"), "impulse 500");
            addActionMenu("impulse W X Y Z", tr("Blink the light following a pattern"), "impulse 500 200 500 200");
        }
        if (gtype == "light_dimmer")
        {
            addActionMenu("set X", tr("Set light dimming to X percent"), "set 50");
            addActionMenu("set off X", tr("Set light dimming without switching on"), "set off 50");
            addActionMenu("up X", tr("Dim light of X percent"), "up 2");
            addActionMenu("down X", tr("Brighten light of X percent"), "down 2");
            addActionMenu("hold press", tr("Change brightness on hold"), "hold press");
            addActionMenu("hold stop", tr("Stop changing brigthness on hold"), "hold stop");
        }
        if (gtype == "light_rgb")
        {
            addActionMenu("set X", tr("Set color to X"), "set 0");
            addActionMenu("up_red X", tr("Increase red by X percent"), "up_red 1");
            addActionMenu("down_red X", tr("Decrease red by X percent"), "down_red 1");
            addActionMenu("set_red X", tr("Set red to X percent"), "set_red 1");
            addActionMenu("up_green X", tr("Increase green by X percent"), "up_green 1");
            addActionMenu("down_green X", tr("Decrease green by X percent"), "down_green 1");
            addActionMenu("set_green X", tr("Set green to X percent"), "set_green 1");
            addActionMenu("up_blue X", tr("Increase blue by X percent"), "up_blue 1");
            addActionMenu("down_blue X", tr("Decrease blue by X percent"), "down_blue 1");
            addActionMenu("set_blue X", tr("Set blue to X percent"), "set_blue 1");
        }
    }
    else if (gtype == "audio")
    {
        addActionMenu("play", tr("Play"), "play");
        addActionMenu("pause", tr("Pause"), "pause");
        addActionMenu("stop", tr("Stop"), "stop");
        addActionMenu("next", tr("Next song"), "next");
        addActionMenu("previous", tr("Previous song"), "previous");
        addActionMenu("volume set X", tr("Set volume to X percent"), "volume set 10");
        addActionMenu("volume up X", tr("Increase volume by X percent"), "volume up 1");
        addActionMenu("volumle down X", tr("Decrease volume by X percent"), "volume down 1");
        addActionMenu("power on", tr("Power on"), "power on");
        addActionMenu("power off", tr("Power off"), "power off");
        addActionMenu("sleep X", tr("Power of in X sec. (sleep mode)"), "sleep 60");
        addActionMenu("sync ID", tr("Syncronize with another player"), "sync 00:11:22:33:44:55");
        addActionMenu("unsync ID", tr("Remove sync with another player"), "unsync 00:11:22:33:44:55");
        addActionMenu("play ID", tr("Play something in the database"), "play 0");
        addActionMenu("add ID", tr("Add something from the database to the playlist"), "add 0");
    }
    else if (gtype == "camera")
    {
        addActionMenu("recall X", tr("Recall position position X"), "recall 0");
        addActionMenu("save X", tr("Save position X"), "save 0");
        addActionMenu("move up", tr("Move up"), "move up");
        addActionMenu("move down", tr("Move down"), "move down");
        addActionMenu("move left", tr("Move left"), "move left");
        addActionMenu("move right", tr("Move right"), "move right");
        addActionMenu("move home", tr("Move to default position"), "move home");
    }
    else if (gtype == "avreceiver")
    {
        addActionMenu("volume X", tr("Set volume to X percent"), "volume 10");
        addActionMenu("power on", tr("Switch on the receiver"), "power on");
        addActionMenu("power off", tr("Switch off the receiver"), "power off");

        if (!output->source_names.empty())
        {
            RuleActionMenu *acsub = addActionMenu("source X", tr("Change input source"), "source 0");
            QMenu *submenu = new QMenu(actionMenu);
            submenu->setProperty("class", QVariant("ActionMenu"));
            acsub->setMenu(submenu);

            for (auto &src: output->source_names)
            {
                QString h = RuleActionTpl.arg(QString("Set source to %1").arg(src.second.c_str()));
                QString action = QString("source %1").arg(src.first);
                RuleActionMenu *ac = new RuleActionMenu(NULL, action, h, action);
                submenu->addAction(ac);
                connect(ac, SIGNAL(triggered(RuleActionMenu*)), this, SLOT(menuAction(RuleActionMenu*)));
            }
        }

        addActionMenu("custom CMD", tr("Send directly a custom command to the receiver"), "custom ");
    }
    else
    {
        ui->buttonMore->setEnabled(false);
    }

    onStart = false;
}

void FormActionStd::on_btMore_clicked()
{
    IOBase *output = NULL;
    if (action->get_size() > 0)
        output = action->get_output(0);

    if (!output) return;

    string type = output->get_param("type");
    string id = output->get_param("id");
    if (output->get_gui_type() == "audio" ||
        output->get_gui_type() == "camera")
        id = output->get_param("oid");

    DialogIOList dio(NULL, output);

    if (dio.exec() == QDialog::Accepted)
    {
        string var_id = dio.getOutput()->get_param("id");
        if (dio.getOutput()->get_gui_type() == "audio" ||
            dio.getOutput()->get_gui_type() == "camera")
            var_id = dio.getOutput()->get_param("iid");

        action->get_params_var().Add(id, var_id);

        FormRules::updateItemAction(qitem, action);
    }
}

RuleActionMenu *FormActionStd::addActionMenu(QString action, QString help, QString cmd)
{
    QString h = RuleActionTpl.arg(help);
    RuleActionMenu *ac = new RuleActionMenu(NULL, action, h, cmd);
    actionMenu->addAction(ac);
    connect(ac, SIGNAL(triggered(RuleActionMenu*)), this, SLOT(menuAction(RuleActionMenu*)));
    return ac;
}

void FormActionStd::on_buttonMore_clicked()
{
    actionMenu->exec(QCursor::pos());
}

void FormActionStd::menuAction(RuleActionMenu *action)
{
    ui->editValue->setText(action->getCommand());
}

void FormActionStd::on_editValue_textChanged(const QString &arg1)
{
    if (onStart) return;

    IOBase *output = NULL;
    if (action->get_size() > 0)
        output = action->get_output(0);

    if (!output) return;

    string type = output->get_param("type");
    string id = output->get_param("id");
    if (output->get_gui_type() == "audio" ||
        output->get_gui_type() == "camera")
        id = output->get_param("oid");

    string value = ui->editValue->text().toUtf8().constData();

    action->get_params().Add(id, value);
    action->get_params_var().Delete(id);

    FormRules::updateItemAction(qitem, action);
}
