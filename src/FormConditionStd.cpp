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

void FormConditionStd::setCondition(QTreeWidgetItem *item, Rule *_rule, Condition *_condition)
{
        rule = _rule;
        condition = _condition;
        qitem = item;

        ui->labelRuleName->setText(QString::fromUtf8(rule->get_name().c_str()));
        ui->labelRuleType->setText(QString::fromUtf8(rule->get_type().c_str()));

        Input *input = NULL;
        if (condition->get_size() > 0)
                input = condition->get_input(0);

        if (!input) return;

        string type = input->get_param("type");
        string id = input->get_param("id");
        if (IOBase::isAudioType(input->get_param("type")) ||
            IOBase::isCameraType(input->get_param("type")))
                id = input->get_param("iid");

        setDone = false;

        //Search room icon
        for (int i = 0;i < ListeRoom::Instance().size();i++)
        {
                Room *room = ListeRoom::Instance().get_room(i);
                for (int j = 0;j < room->get_size_in();j++)
                {
                        if (room->get_input(j) == input)
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

        ui->labelItemName->setText(QString::fromUtf8(input->get_param("name").c_str()));

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
            type == "scenario" || type == "InPlageHoraire" || type == "InternalBool")
        {
                ui->comboValue->addItem(QString::fromUtf8("Activé"), QString::fromUtf8("true"));
                ui->comboValue->addItem(QString::fromUtf8("Désactivé"), QString::fromUtf8("false"));
                ui->comboValue->addItem(QString::fromUtf8("Les deux"), QString::fromUtf8("changed"));

                if (condition->get_params().get_param(id) == "true")
                        ui->comboValue->setCurrentIndex(0);
                else if (condition->get_params().get_param(id) == "false")
                        ui->comboValue->setCurrentIndex(1);
                else if (condition->get_params().get_param(id) == "changed")
                        ui->comboValue->setCurrentIndex(2);
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
                ui->comboValue->setEditable(true);
                QString s = QString::fromUtf8(condition->get_params().get_param(id).c_str());
                ui->comboValue->lineEdit()->setText(s);
        }

        setDone = true;
}

void FormConditionStd::on_btMore_clicked()
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

                 condition->get_params_var().Add(id, var_id);

                 FormRules::updateItemCondition(qitem, condition);
        }
}


void FormConditionStd::on_comboOp_currentIndexChanged(int)
{
        if (!setDone) return;

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

        FormRules::updateItemCondition(qitem, condition);
}

void FormConditionStd::on_comboValue_editTextChanged(QString)
{
        if (!setDone) return;

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

        FormRules::updateItemCondition(qitem, condition);
}

void FormConditionStd::on_comboValue_currentIndexChanged(int)
{
        if (!setDone) return;

        on_comboValue_editTextChanged("");
}
