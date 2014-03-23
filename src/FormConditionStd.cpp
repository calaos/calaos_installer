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
        string id = io->get_param("id");
        if (IOBase::isAudioType(io->get_param("type")) ||
            IOBase::isCameraType(io->get_param("type")))
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
        ui->comboOp->addItem(QString::fromUtf8("=="), QString::fromUtf8("=="));
        ui->comboOp->addItem(QString::fromUtf8("!="), QString::fromUtf8("!="));
        ui->comboOp->addItem(QString::fromUtf8(">"), QString::fromUtf8("SUP"));
        ui->comboOp->addItem(QString::fromUtf8(">="), QString::fromUtf8("SUP="));
        ui->comboOp->addItem(QString::fromUtf8("<"), QString::fromUtf8("INF"));
        ui->comboOp->addItem(QString::fromUtf8("<="), QString::fromUtf8("INF="));

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

        if (condition->getType() == COND_STD)
                ui->editValue->setText(QString::fromUtf8(condition->get_params().get_param(id).c_str()));
        else
                ui->editValue->setText(QString::fromUtf8(condition->getOutputParam().c_str()));

        if (type == "InputTime")
        {
                addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("Evennement sur heure/date choisie"), QString::fromUtf8("true"));
                addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("Evennement en dehors de l'heure/date choisie"), QString::fromUtf8("false"));
                addActionMenu(QString::fromUtf8("changed"), QString::fromUtf8("Evennement sur n'importe quel état"), QString::fromUtf8("changed"));
        }
        else if (type == "InputTimer")
        {
                addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("Evennement sur tempo terminé"), QString::fromUtf8("true"));
                addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("Evennement sur tempo non terminé"), QString::fromUtf8("false"));
                addActionMenu(QString::fromUtf8("changed"), QString::fromUtf8("Evennement sur n'importe quel état"), QString::fromUtf8("changed"));
        }
        else if (type == "WIDigitalBP")
        {
                addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("L'entrée est activé"), QString::fromUtf8("true"));
                addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("L'entrée est désactivé"), QString::fromUtf8("false"));
                addActionMenu(QString::fromUtf8("changed"), QString::fromUtf8("Evennement sur n'importe quel état"), QString::fromUtf8("changed"));
        }
        else if (type == "scenario")
        {
                addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("Le bouton scénario est executé"), QString::fromUtf8("true"));
                addActionMenu(QString::fromUtf8("changed"), QString::fromUtf8("Evennement sur n'importe quel état"), QString::fromUtf8("changed"));
        }
        else if (type == "InPlageHoraire")
        {
                addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("Evennement sur entrée dans la plage horaire"), QString::fromUtf8("true"));
                addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("Evennement sur sortie de la plage horaire"), QString::fromUtf8("false"));
                addActionMenu(QString::fromUtf8("changed"), QString::fromUtf8("Evennement sur n'importe quel état"), QString::fromUtf8("changed"));
        }
        else if (type == "InternalBool")
        {
                addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("La variable est activée"), QString::fromUtf8("true"));
                addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("La variable est désactivée"), QString::fromUtf8("false"));
                addActionMenu(QString::fromUtf8("changed"), QString::fromUtf8("Evennement sur n'importe quel état"), QString::fromUtf8("changed"));
        }
        else if (type == "WODigital")
        {
                addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("La sortie est activée"), QString::fromUtf8("true"));
                addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("La sortie est désactivée"), QString::fromUtf8("false"));
                addActionMenu(QString::fromUtf8("changed"), QString::fromUtf8("Evennement sur n'importe quel état"), QString::fromUtf8("changed"));
        }
        else if (type == "WIDigitalTriple")
        {
                addActionMenu(QString::fromUtf8("1"), QString::fromUtf8("Evennement sur 1 appui"), QString::fromUtf8("1"));
                addActionMenu(QString::fromUtf8("2"), QString::fromUtf8("Evennement sur 2 appuis"), QString::fromUtf8("2"));
                addActionMenu(QString::fromUtf8("3"), QString::fromUtf8("Evennement sur 3 appuis"), QString::fromUtf8("3"));
        }
        else if (type == "WIDigitalLong")
        {
                addActionMenu(QString::fromUtf8("1"), tr("Event on normal click"), QString::fromUtf8("1"));
                addActionMenu(QString::fromUtf8("2"), tr("Event on long press"), QString::fromUtf8("2"));
        }
        else if (type == "WITemp" || type == "OWTemp")
        {
                addActionMenu(QString::fromUtf8("20"), QString::fromUtf8("Evennement sur une témperature en °C"), QString::fromUtf8("20"));
        }
        else if (IOBase::isAudioType(type))
        {
                addActionMenu(QString::fromUtf8("onplay"), QString::fromUtf8("Evennement lors de la lecture"), QString::fromUtf8("onplay"));
                addActionMenu(QString::fromUtf8("onpause"), QString::fromUtf8("Evennement lors de la pause"), QString::fromUtf8("onpause"));
                addActionMenu(QString::fromUtf8("onstop"), QString::fromUtf8("Evennement lors de l'arrêt"), QString::fromUtf8("onstop"));
                addActionMenu(QString::fromUtf8("onsongchange"), QString::fromUtf8("Evennement lors du changement de piste"), QString::fromUtf8("onsongchange"));
                addActionMenu(QString::fromUtf8("onerror"), QString::fromUtf8("Evennement lors d'erreur"), QString::fromUtf8("onerror"));
        }
        else
        {
                if (condition->getType() == COND_OUTPUT)
                        addActionMenu(QString::fromUtf8("changed"), QString::fromUtf8("Evennement lors du changement d'état"), QString::fromUtf8("changed"));
                else
                        ui->buttonMore->setEnabled(false);
        }

        onStart = false;
}

void FormConditionStd::on_btMore_clicked()
{
        if (condition->getType() == COND_STD)
        {
                Input *input = NULL;
                if (condition->get_size() > 0)
                        input = condition->get_input(0);

                if (!input) return;

                string id = input->get_param("id");
                if (IOBase::isAudioType(input->get_param("type")) ||
                    IOBase::isCameraType(input->get_param("type")))
                        id = input->get_param("iid");

                DialogIOList dio(input, NULL);

                if (dio.exec() == QDialog::Accepted)
                {
                        string var_id = dio.getInput()->get_param("id");
                        if (IOBase::isAudioType(dio.getInput()->get_param("type")) ||
                            IOBase::isCameraType(dio.getInput()->get_param("type")))
                                var_id = dio.getInput()->get_param("iid");

                        condition->get_params().Add(id, "");
                        condition->get_params_var().Add(id, var_id);

                        FormRules::updateItemCondition(qitem, condition);
                }
        }
        else
        {
                Output *output = condition->getOutput();

                if (!output) return;

                string id = output->get_param("id");
                if (IOBase::isAudioType(output->get_param("type")) ||
                    IOBase::isCameraType(output->get_param("type")))
                        id = output->get_param("iid");

                DialogIOList dio(NULL, output);

                if (dio.exec() == QDialog::Accepted)
                {
                        string var_id = dio.getOutput()->get_param("id");
                        if (IOBase::isAudioType(dio.getOutput()->get_param("type")) ||
                            IOBase::isCameraType(dio.getOutput()->get_param("type")))
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
                Input *input = NULL;
                if (condition->get_size() > 0)
                        input = condition->get_input(0);

                if (!input) return;

                string id = input->get_param("id");
                if (IOBase::isAudioType(input->get_param("type")) ||
                                IOBase::isCameraType(input->get_param("type")))
                        id = input->get_param("iid");

                int current = ui->comboOp->currentIndex();

                string v = ui->comboOp->itemData(current).toString().toUtf8().constData();
                condition->get_operator().Add(id, v);
        }
        else
        {
                Output *output = condition->getOutput();

                if (!output) return;

                string id = output->get_param("id");
                if (IOBase::isAudioType(output->get_param("type")) ||
                                IOBase::isCameraType(output->get_param("type")))
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

                Input *input = NULL;
                if (condition->get_size() > 0)
                        input = condition->get_input(0);

                if (!input) return;

                string id = input->get_param("id");
                if (IOBase::isAudioType(input->get_param("type")) ||
                    IOBase::isCameraType(input->get_param("type")))
                        id = input->get_param("iid");

                string value = ui->editValue->text().toUtf8().constData();

                condition->get_params().Add(id, value);
                condition->get_params_var().Delete(id);
        }
        else
        {
                Output *output = condition->getOutput();

                if (!output) return;

                string id = output->get_param("id");
                if (IOBase::isAudioType(output->get_param("type")) ||
                    IOBase::isCameraType(output->get_param("type")))
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
