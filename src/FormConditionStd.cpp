#include "FormConditionStd.h"
#include "ui_FormConditionStd.h"

#include "formrules.h"

FormConditionStd::FormConditionStd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormConditionStd),
    rule(NULL), condition(NULL)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Popup);
    setFocusPolicy(Qt::NoFocus);
    setFocusProxy(parent);
    setMouseTracking(true);

    actionMenu = new QMenu(this);
    actionMenu->setProperty("class", QVariant("ActionMenu"));
}

FormConditionStd::~FormConditionStd()
{
    delete ui;
}

void FormConditionStd::changeEvent(QEvent *e)
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

string FormConditionStd::getRoomIcon(string room_type)
{
    string icon;

    if (room_type == "salon" || room_type == "lounge")
        icon = ":/img/rooms/lounge_small.png";
    else if (room_type == "sdb" || room_type == "bathroom")
        icon = ":/img/rooms/bathroom_small.png";
    else if (room_type == "chambre" || room_type == "bedroom")
        icon = ":/img/rooms/bedroom_small.png";
    else if (room_type == "cave" || room_type == "cellar")
        icon = ":/img/rooms/cellar_small.png";
    else if (room_type == "couloir" || room_type == "hall" || room_type == "corridor")
        icon = ":/img/rooms/corridor_small.png";
    else if (room_type == "sam" || room_type == "diningroom")
        icon = ":/img/rooms/diningroom_small.png";
    else if (room_type == "garage")
        icon = ":/img/rooms/garage_small.png";
    else if (room_type == "cuisine" || room_type == "kitchen")
        icon = ":/img/rooms/kitchen_small.png";
    else if (room_type == "bureau" || room_type == "office")
        icon = ":/img/rooms/office_small.png";
    else if (room_type == "exterieur" || room_type == "outside")
        icon = ":/img/rooms/outside_small.png";
    else if (room_type == "misc" || room_type == "divers" || room_type == "various")
        icon = ":/img/rooms/various_small.png";
    else
        icon = ":/img/rooms/various_small.png";

    return icon;
}

void FormConditionStd::setCondition(QTreeWidgetItem *item, Rule *_rule, Condition *_condition)
{
    rule = _rule;
    condition = _condition;
    qitem = item;

    ui->labelRuleName->setText(QString::fromUtf8(rule->get_name().c_str()));
    ui->labelRuleType->setText(QString::fromUtf8(rule->get_type().c_str()));

    if (condition->getType() == COND_OUTPUT)
        ui->labelRuleIcon->setPixmap(QPixmap(":/img/icon_rule_out.png"));

    IOBase *io = NULL;

    if (condition->get_size() > 0 && condition->getType() == COND_STD)
        io = condition->get_input(0);
    if (condition->getType() == COND_OUTPUT)
        io = condition->getOutput();

    if (!io) return;

    onStart = true;

    string type = io->get_param("type");
    string gtype = io->get_gui_type();
    string id = io->get_param("id");
    if (io->get_gui_type() == "audio" ||
        io->get_gui_type() == "camera")
        id = io->get_param("iid");

    //Search room icon
    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *room = ListeRoom::Instance().get_room(i);

        if (condition->getType() == COND_STD)
        {
            for (int j = 0;j < room->get_size_in();j++)
            {
                if (room->get_input(j) == io)
                {
                    ui->labelRoomName->setText(QString::fromUtf8(room->get_name().c_str()));
                    ui->labelRoomIcon->setPixmap(QPixmap(getRoomIcon(room->get_type()).c_str()));

                    break;
                }
            }
        }
        else
        {
            for (int j = 0;j < room->get_size_out();j++)
            {
                if (room->get_output(j) == io)
                {
                    ui->labelRoomName->setText(QString::fromUtf8(room->get_name().c_str()));
                    ui->labelRoomIcon->setPixmap(QPixmap(getRoomIcon(room->get_type()).c_str()));

                    break;
                }
            }
        }
    }

    ui->labelItemName->setText(QString::fromUtf8(io->get_param("name").c_str()));

    ui->comboOp->clear();
    ui->comboOp->addItem("==", "==");
    ui->comboOp->addItem("!=", "!=");
    ui->comboOp->addItem(">", "SUP");
    ui->comboOp->addItem(">=", "SUP=");
    ui->comboOp->addItem("<", "INF");
    ui->comboOp->addItem("<=", "INF=");

    if (condition->get_operator().get_param(id) == "==")
        ui->comboOp->setCurrentIndex(0);
    else if (condition->get_operator().get_param(id) == "!=")
        ui->comboOp->setCurrentIndex(1);
    else if (condition->get_operator().get_param(id) == "SUP")
        ui->comboOp->setCurrentIndex(2);
    else if (condition->get_operator().get_param(id) == "SUP=")
        ui->comboOp->setCurrentIndex(3);
    else if (condition->get_operator().get_param(id) == "INF")
        ui->comboOp->setCurrentIndex(4);
    else if (condition->get_operator().get_param(id) == "INF=")
        ui->comboOp->setCurrentIndex(5);

    actionMenu->clear();
    ui->editValue->clear();
    ui->buttonMore->setEnabled(true);

    ui->checkTrigger->setChecked(condition->isTrigger());

    if (condition->getType() == COND_STD)
        ui->editValue->setText(QString::fromUtf8(condition->get_params().get_param(id).c_str()));
    else
        ui->editValue->setText(QString::fromUtf8(condition->getOutputParam().c_str()));

    if (gtype == "time")
    {
        addActionMenu("true", tr("Event when entering data/time"), "true");
        addActionMenu("false", tr("Event when exiting date/time"), "false");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "timer")
    {
        addActionMenu("true", tr("Event on timer finished"), "true");
        addActionMenu("false", tr("Event on timer start"), "false");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "switch")
    {
        addActionMenu("true", tr("Input is active"), "true");
        addActionMenu("false", tr("Input is inactive"), "false");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "scenario")
    {
        addActionMenu("true", tr("Scenario is executed"), "true");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "time_range")
    {
        addActionMenu("true", tr("Event entering time range"), "true");
        addActionMenu("false", tr("Event exiting time range"), "false");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "var_bool")
    {
        addActionMenu("true", tr("Variable is true"), "true");
        addActionMenu("false", tr("Variable is false"), "false");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "light")
    {
        addActionMenu("true", tr("Output is active"), "true");
        addActionMenu("false", tr("Output is inactive"), "false");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "light_dimmer")
    {
        addActionMenu("true", tr("Light is on"), "true");
        addActionMenu("false", tr("Light is off"), "false");
        addActionMenu("50", tr("Light is at X%"), "50");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "light_rgb")
    {
        addActionMenu("true", tr("Light is on"), "true");
        addActionMenu("false", tr("Light is off"), "false");
        addActionMenu("123", tr("Light is at XXXX color"), "123");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "shutter")
    {
        addActionMenu("open", tr("Shutter is open"), "open");
        addActionMenu("closed", tr("Shutter is closed"), "closed");
        addActionMenu("stop", tr("Shutter is stopped"), "stop");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "shutter_smart")
    {
        addActionMenu("open", tr("Shutter is open"), "open");
        addActionMenu("closed", tr("Shutter is closed"), "closed");
        addActionMenu("stop", tr("Shutter is stopped"), "stop");
        addActionMenu("50", tr("Shutter is at X%"), "50");
        addActionMenu("changed", tr("On any event"), "changed");
    }
    else if (gtype == "switch3")
    {
        addActionMenu("1", tr("Event on 1 press"), "1");
        addActionMenu("2", tr("Event on 2 presses"), "2");
        addActionMenu("3", tr("Event on 3 presses"), "3");
    }
    else if (gtype == "switch_long")
    {
        addActionMenu("1", tr("Event on normal press"), "1");
        addActionMenu("2", tr("Event on long press"), "2");
    }
    else if (gtype == "temp")
    {
        addActionMenu("20", tr("Event when temperature changes to a value"), "20");
        addActionMenu("changed", tr("Event when temperature changes"), "changed");
    }
    else if (gtype == "var_int")
    {
        addActionMenu("changed", tr("Event when value changes"), "changed");
    }
    else if (gtype == "var_string")
    {
        addActionMenu("changed", tr("Event when value changes"), "changed");
    }
    else if (gtype == "audio")
    {
        addActionMenu("onplay", tr("Event on play"), "onplay");
        addActionMenu("onpause", tr("Event on pause"), "onpause");
        addActionMenu("onstop", tr("Event on stop"), "onstop");
        addActionMenu("onsongchange", tr("Event on song change"), "onsongchange");
        addActionMenu("onerror", tr("Event on error"), "onerror");
    }
    else
    {
        if (condition->getType() == COND_OUTPUT)
            addActionMenu("changed", tr("On any event"), "changed");
        else
            ui->buttonMore->setEnabled(false);
    }

    onStart = false;
}

void FormConditionStd::on_btMore_clicked()
{
    if (condition->getType() == COND_STD)
    {
        IOBase *input = NULL;
        if (condition->get_size() > 0)
            input = condition->get_input(0);

        if (!input) return;

        string id = input->get_param("id");
        if (input->get_gui_type() == "audio" ||
            input->get_gui_type() == "camera")
            id = input->get_param("iid");

        DialogIOList dio(input, NULL);

        if (dio.exec() == QDialog::Accepted)
        {
            string var_id = dio.getInput()->get_param("id");
            if (dio.getInput()->get_gui_type() == "audio" ||
                dio.getInput()->get_gui_type() == "camera")
                var_id = dio.getInput()->get_param("iid");

            condition->get_params().Add(id, "");
            condition->get_params_var().Add(id, var_id);

            FormRules::updateItemCondition(qitem, condition);
        }
    }
    else
    {
        IOBase *output = condition->getOutput();

        if (!output) return;

        string id = output->get_param("id");
        if (output->get_gui_type() == "audio" ||
            output->get_gui_type() == "camera")
            id = output->get_param("iid");

        DialogIOList dio(NULL, output);

        if (dio.exec() == QDialog::Accepted)
        {
            string var_id = dio.getOutput()->get_param("id");
            if (dio.getOutput()->get_gui_type() == "audio" ||
                dio.getOutput()->get_gui_type() == "camera")
                var_id = dio.getOutput()->get_param("iid");

            condition->setOutputParamVar(var_id);
            condition->setOutputParam("");

            FormRules::updateItemCondition(qitem, condition);
        }
    }
}

void FormConditionStd::on_comboOp_currentIndexChanged(int)
{
    if (onStart) return;

    if (condition->getType() == COND_STD)
    {
        IOBase *input = NULL;
        if (condition->get_size() > 0)
            input = condition->get_input(0);

        if (!input) return;

        string id = input->get_param("id");
        if (input->get_gui_type() == "audio" ||
            input->get_gui_type() == "camera")
            id = input->get_param("iid");

        int current = ui->comboOp->currentIndex();

        string v = ui->comboOp->itemData(current).toString().toUtf8().constData();
        condition->get_operator().Add(id, v);
    }
    else
    {
        IOBase *output = condition->getOutput();

        if (!output) return;

        string id = output->get_param("id");
        if (output->get_gui_type() == "audio" ||
            output->get_gui_type() == "camera")
            id = output->get_param("iid");

        int current = ui->comboOp->currentIndex();

        string v = ui->comboOp->itemData(current).toString().toUtf8().constData();
        condition->setOutputOper(v);
    }

    FormRules::updateItemCondition(qitem, condition);
}

void FormConditionStd::on_editValue_textChanged(const QString &arg1)
{
    if (onStart) return;

    if (condition->getType() == COND_STD)
    {

        IOBase *input = NULL;
        if (condition->get_size() > 0)
            input = condition->get_input(0);

        if (!input) return;

        string id = input->get_param("id");
        if (input->get_gui_type() == "audio" ||
            input->get_gui_type() == "camera")
            id = input->get_param("iid");

        string value = ui->editValue->text().toUtf8().constData();

        condition->get_params().Add(id, value);
        condition->get_params_var().Delete(id);
    }
    else
    {
        IOBase *output = condition->getOutput();

        if (!output) return;

        string id = output->get_param("id");
        if (output->get_gui_type() == "audio" ||
            output->get_gui_type() == "camera")
            id = output->get_param("iid");

        string value = ui->editValue->text().toUtf8().constData();

        condition->setOutputParam(value);
        condition->setOutputParamVar("");
    }

    FormRules::updateItemCondition(qitem, condition);
}

void FormConditionStd::addActionMenu(QString action, QString help, QString cmd)
{
    QString h = RuleActionTpl.arg(help);
    RuleActionMenu *ac = new RuleActionMenu(NULL, action, h, cmd);
    actionMenu->addAction(ac);
    connect(ac, SIGNAL(triggered(RuleActionMenu*)), this, SLOT(menuAction(RuleActionMenu*)));
}

void FormConditionStd::on_buttonMore_clicked()
{
    actionMenu->exec(QCursor::pos());
}

void FormConditionStd::menuAction(RuleActionMenu *action)
{
    ui->editValue->setText(action->getCommand());
}

void FormConditionStd::on_checkTrigger_stateChanged(int)
{
    condition->setTrigger(ui->checkTrigger->isChecked());
}
