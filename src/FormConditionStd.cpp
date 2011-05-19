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

        string type = io->get_param("type");
        string id = io->get_param("id");
        if (IOBase::isAudioType(io->get_param("type")) ||
            IOBase::isCameraType(io->get_param("type")))
                id = io->get_param("iid");

        setDone = false;

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
        ui->comboValue->clear();
        ui->comboValue->setEditable(false);

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

        if (type == "InputTime" || type == "InputTimer" || type == "WIDigitalBP" ||
            type == "scenario" || type == "InPlageHoraire" || type == "InternalBool" ||
            type == "WODigital")
        {
                ui->comboValue->addItem(QString::fromUtf8("Activé"), QString::fromUtf8("true"));
                ui->comboValue->addItem(QString::fromUtf8("Désactivé"), QString::fromUtf8("false"));
                ui->comboValue->addItem(QString::fromUtf8("Les deux"), QString::fromUtf8("changed"));

                if (condition->getType() == COND_STD)
                {
                        if (condition->get_params().get_param(id) == "true")
                                ui->comboValue->setCurrentIndex(0);
                        else if (condition->get_params().get_param(id) == "false")
                                ui->comboValue->setCurrentIndex(1);
                        else if (condition->get_params().get_param(id) == "changed")
                                ui->comboValue->setCurrentIndex(2);
                }
                else
                {
                        if (condition->getOutputParam() == "true")
                                ui->comboValue->setCurrentIndex(0);
                        else if (condition->getOutputParam() == "false")
                                ui->comboValue->setCurrentIndex(1);
                        else if (condition->getOutputParam() == "changed")
                                ui->comboValue->setCurrentIndex(2);
                }
        }
        else if (type == "WIDigitalTriple")
        {
                ui->comboValue->addItem(QString::fromUtf8("1 appui"), QString::fromUtf8("1"));
                ui->comboValue->addItem(QString::fromUtf8("2 appuis"), QString::fromUtf8("2"));
                ui->comboValue->addItem(QString::fromUtf8("3 appuis"), QString::fromUtf8("3"));

                if (condition->get_params().get_param(id) == "1")
                        ui->comboValue->setCurrentIndex(0);
                else if (condition->get_params().get_param(id) == "2")
                        ui->comboValue->setCurrentIndex(1);
                else if (condition->get_params().get_param(id) == "3")
                        ui->comboValue->setCurrentIndex(2);
        }
        else if (type == "WITemp" || type == "OWTemp")
        {
                ui->comboValue->setEditable(true);

                for (int i = 10;i < 30;i++)
                        ui->comboValue->addItem(QString::fromStdString(to_string(i)) + QString::fromUtf8(" °C"), QString::fromStdString(to_string(i)));
        }
        else if (type == "InternalInt")
        {
                ui->comboValue->setEditable(true);

                for (int i = 0;i < 30;i++)
                        ui->comboValue->addItem(QString::fromStdString(to_string(i)), QString::fromStdString(to_string(i)));
        }
        else if (IOBase::isAudioType(type))
        {
                ui->comboValue->addItem(QString::fromUtf8("Lecture"), QString::fromUtf8("onplay"));
                ui->comboValue->addItem(QString::fromUtf8("Pause"), QString::fromUtf8("onpause"));
                ui->comboValue->addItem(QString::fromUtf8("Stop"), QString::fromUtf8("onstop"));
                ui->comboValue->addItem(QString::fromUtf8("Changement de piste"), QString::fromUtf8("onsongchange"));
                ui->comboValue->addItem(QString::fromUtf8("Erreur"), QString::fromUtf8("onerror"));

                if (condition->get_params().get_param(id) == "onplay")
                        ui->comboValue->setCurrentIndex(0);
                else if (condition->get_params().get_param(id) == "onpause")
                        ui->comboValue->setCurrentIndex(1);
                else if (condition->get_params().get_param(id) == "onstop")
                        ui->comboValue->setCurrentIndex(2);
                else if (condition->get_params().get_param(id) == "onsongchange")
                        ui->comboValue->setCurrentIndex(3);
                else if (condition->get_params().get_param(id) == "onerror")
                        ui->comboValue->setCurrentIndex(4);
        }
        else
        {
                if (condition->getType() == COND_OUTPUT)
                {
                        ui->comboValue->addItem(QString::fromUtf8("changed"), QString::fromUtf8("changed"));

                        if (condition->getOutputParam() == "changed")
                                ui->comboValue->setCurrentIndex(0);
                }

                ui->comboValue->setEditable(true);
                QString s = QString::fromUtf8(condition->get_params().get_param(id).c_str());
                ui->comboValue->lineEdit()->setText(s);
        }

        setDone = true;
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
        if (!setDone) return;

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

void FormConditionStd::on_comboValue_editTextChanged(QString)
{
        if (!setDone) return;

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

                string value;
                int current = ui->comboValue->currentIndex();

                if (ui->comboValue->isEditable() && current > -1 &&
                                ui->comboValue->itemData(current).isNull())
                {
                        value = ui->comboValue->lineEdit()->text().toUtf8().constData();
                }
                else
                {
                        value = ui->comboValue->itemData(current).toString().toUtf8().constData();
                }

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

                string value;
                int current = ui->comboValue->currentIndex();

                if (ui->comboValue->isEditable() && current > -1 &&
                                ui->comboValue->itemData(current).isNull())
                {
                        value = ui->comboValue->lineEdit()->text().toUtf8().constData();
                }
                else
                {
                        value = ui->comboValue->itemData(current).toString().toUtf8().constData();
                }

                condition->setOutputParam(value);
                condition->setOutputParamVar("");
        }

        FormRules::updateItemCondition(qitem, condition);
}

void FormConditionStd::on_comboValue_currentIndexChanged(int)
{
        if (!setDone) return;

        on_comboValue_editTextChanged("");
}
