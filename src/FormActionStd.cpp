#include "FormActionStd.h"
#include "ui_FormActionStd.h"

#include "formrules.h"

FormActionStd::FormActionStd(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FormActionStd),
        rule(NULL), action(NULL)
{
        ui->setupUi(this);

        setWindowFlags(Qt::Popup);
        setFocusPolicy(Qt::NoFocus);
        setFocusProxy(parent);
        setMouseTracking(true);
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

        Output *output = NULL;
        if (action->get_size() > 0)
                output = action->get_output(0);

        if (!output) return;

        string type = output->get_param("type");
        string id = output->get_param("id");
        if (IOBase::isAudioType(output->get_param("type")) ||
            IOBase::isCameraType(output->get_param("type")))
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

        ui->comboValue->clear();
        ui->comboValue->setEditable(false);

        if (type == "OutputFake" || type == "InputTimer" ||
            type == "scenario" || type == "InternalBool")
        {
                ui->comboValue->addItem(QString::fromUtf8("Activer"), QString::fromUtf8("true"));
                ui->comboValue->addItem(QString::fromUtf8("Désactiver"), QString::fromUtf8("false"));
                ui->comboValue->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));

                if (action->get_params().get_param(id) == "true")
                        ui->comboValue->setCurrentIndex(0);
                else if (action->get_params().get_param(id) == "false")
                        ui->comboValue->setCurrentIndex(1);
                else if (action->get_params().get_param(id) == "toggle")
                        ui->comboValue->setCurrentIndex(2);
        }
        else if (type == "WODigital")
        {
                ui->comboValue->setEditable(true);

                ui->comboValue->addItem(QString::fromUtf8("Activer"), QString::fromUtf8("true"));
                ui->comboValue->addItem(QString::fromUtf8("Désactiver"), QString::fromUtf8("false"));
                ui->comboValue->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                ui->comboValue->addItem(QString::fromUtf8("Impulsion X milisecondes"), QString::fromUtf8("impulse "));

                if (action->get_params().get_param(id) == "true")
                        ui->comboValue->setCurrentIndex(0);
                else if (action->get_params().get_param(id) == "false")
                        ui->comboValue->setCurrentIndex(1);
                else if (action->get_params().get_param(id) == "toggle")
                        ui->comboValue->setCurrentIndex(2);
                else
                {
                        QString s = QString::fromUtf8(action->get_params().get_param(id).c_str());
                        ui->comboValue->lineEdit()->setText(s);
                }
        }
        else if (type == "WOVolet")
        {
                ui->comboValue->setEditable(true);

                ui->comboValue->addItem(QString::fromUtf8("Monter"), QString::fromUtf8("up"));
                ui->comboValue->addItem(QString::fromUtf8("Descendre"), QString::fromUtf8("down"));
                ui->comboValue->addItem(QString::fromUtf8("Arrêter"), QString::fromUtf8("stop"));
                ui->comboValue->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                ui->comboValue->addItem(QString::fromUtf8("Impulsion sur la montée de X ms"), QString::fromUtf8("impulse up "));
                ui->comboValue->addItem(QString::fromUtf8("Impulsion sur la descente de X ms"), QString::fromUtf8("impulse down "));

                if (action->get_params().get_param(id) == "up")
                        ui->comboValue->setCurrentIndex(0);
                else if (action->get_params().get_param(id) == "down")
                        ui->comboValue->setCurrentIndex(1);
                else if (action->get_params().get_param(id) == "stop")
                        ui->comboValue->setCurrentIndex(2);
                else if (action->get_params().get_param(id) == "toggle")
                        ui->comboValue->setCurrentIndex(3);
                else
                {
                        QString s = QString::fromUtf8(action->get_params().get_param(id).c_str());
                        ui->comboValue->lineEdit()->setText(s);
                }
        }
        else if (type == "WOVoletSmart")
        {
                ui->comboValue->setEditable(true);

                ui->comboValue->addItem(QString::fromUtf8("Monter"), QString::fromUtf8("up"));
                ui->comboValue->addItem(QString::fromUtf8("Descendre"), QString::fromUtf8("down"));
                ui->comboValue->addItem(QString::fromUtf8("Arrêter"), QString::fromUtf8("stop"));
                ui->comboValue->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                ui->comboValue->addItem(QString::fromUtf8("Impulsion sur la montée de X ms"), QString::fromUtf8("impulse up "));
                ui->comboValue->addItem(QString::fromUtf8("Impulsion sur la descente de X ms"), QString::fromUtf8("impulse down "));
                ui->comboValue->addItem(QString::fromUtf8("Mettre à la position [0-100]"), QString::fromUtf8("set "));
                ui->comboValue->addItem(QString::fromUtf8("Monter de X pourcent"), QString::fromUtf8("up "));
                ui->comboValue->addItem(QString::fromUtf8("Descendre de X pourcent"), QString::fromUtf8("down "));
                ui->comboValue->addItem(QString::fromUtf8("Calibrer"), QString::fromUtf8("calibrate"));

                if (action->get_params().get_param(id) == "up")
                        ui->comboValue->setCurrentIndex(0);
                else if (action->get_params().get_param(id) == "down")
                        ui->comboValue->setCurrentIndex(1);
                else if (action->get_params().get_param(id) == "stop")
                        ui->comboValue->setCurrentIndex(2);
                else if (action->get_params().get_param(id) == "toggle")
                        ui->comboValue->setCurrentIndex(3);
                else
                {
                        QString s = QString::fromUtf8(action->get_params().get_param(id).c_str());
                        ui->comboValue->lineEdit()->setText(s);
                }
        }
        else if (type == "WODali" || type == "WONeon")
        {
                ui->comboValue->setEditable(true);

                ui->comboValue->addItem(QString::fromUtf8("Allumer"), QString::fromUtf8("true"));
                ui->comboValue->addItem(QString::fromUtf8("Eteindre"), QString::fromUtf8("false"));
                ui->comboValue->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                ui->comboValue->addItem(QString::fromUtf8("Mettre à X pourcent"), QString::fromUtf8("set "));
                ui->comboValue->addItem(QString::fromUtf8("Monter de X pourcent"), QString::fromUtf8("up "));
                ui->comboValue->addItem(QString::fromUtf8("Descendre de X pourcent"), QString::fromUtf8("down "));
                ui->comboValue->addItem(QString::fromUtf8("Mettre à X pourcent (OFF)"), QString::fromUtf8("set off "));

                if (type == "WODali")
                {
                        ui->comboValue->addItem(QString::fromUtf8("Variation appui"), QString::fromUtf8("hold press"));
                        ui->comboValue->addItem(QString::fromUtf8("Variation relachement"), QString::fromUtf8("hold stop"));
                }

                if (action->get_params().get_param(id) == "true")
                        ui->comboValue->setCurrentIndex(0);
                else if (action->get_params().get_param(id) == "false")
                        ui->comboValue->setCurrentIndex(1);
                else if (action->get_params().get_param(id) == "toggle")
                        ui->comboValue->setCurrentIndex(2);
                else
                {
                        QString s = QString::fromUtf8(action->get_params().get_param(id).c_str());
                        ui->comboValue->lineEdit()->setText(s);
                }
        }
        else if (type == "WODaliRVB")
        {
                ui->comboValue->setEditable(true);

                ui->comboValue->addItem(QString::fromUtf8("Allumer"), QString::fromUtf8("true"));
                ui->comboValue->addItem(QString::fromUtf8("Eteindre"), QString::fromUtf8("false"));
                ui->comboValue->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                ui->comboValue->addItem(QString::fromUtf8("Mettre à X couleur [0-65535]"), QString::fromUtf8("set "));
                ui->comboValue->addItem(QString::fromUtf8("Monter rouge de X pourcent"), QString::fromUtf8("up_red "));
                ui->comboValue->addItem(QString::fromUtf8("Descendre rouge de X pourcent"), QString::fromUtf8("down_red "));
                ui->comboValue->addItem(QString::fromUtf8("Mettre rouge à X pourcent"), QString::fromUtf8("set_red "));
                ui->comboValue->addItem(QString::fromUtf8("Monter vert de X pourcent"), QString::fromUtf8("up_green "));
                ui->comboValue->addItem(QString::fromUtf8("Descendre vert de X pourcent"), QString::fromUtf8("down_green "));
                ui->comboValue->addItem(QString::fromUtf8("Mettre vert à X pourcent"), QString::fromUtf8("set_green "));
                ui->comboValue->addItem(QString::fromUtf8("Monter bleu de X pourcent"), QString::fromUtf8("up_blue "));
                ui->comboValue->addItem(QString::fromUtf8("Descendre bleu de X pourcent"), QString::fromUtf8("down_blue "));
                ui->comboValue->addItem(QString::fromUtf8("Mettre bleu à X pourcent"), QString::fromUtf8("set_blue "));

                if (action->get_params().get_param(id) == "true")
                        ui->comboValue->setCurrentIndex(0);
                else if (action->get_params().get_param(id) == "false")
                        ui->comboValue->setCurrentIndex(1);
                else if (action->get_params().get_param(id) == "toggle")
                        ui->comboValue->setCurrentIndex(2);
                else
                {
                        QString s = QString::fromUtf8(action->get_params().get_param(id).c_str());
                        ui->comboValue->lineEdit()->setText(s);
                }
        }
        else if (IOBase::isAudioType(type))
        {
                ui->comboValue->setEditable(true);

                ui->comboValue->addItem(QString::fromUtf8("Lecture"), QString::fromUtf8("play"));
                ui->comboValue->addItem(QString::fromUtf8("Pause"), QString::fromUtf8("pause"));
                ui->comboValue->addItem(QString::fromUtf8("Stop"), QString::fromUtf8("stop"));
                ui->comboValue->addItem(QString::fromUtf8("Piste suivante"), QString::fromUtf8("next"));
                ui->comboValue->addItem(QString::fromUtf8("Piste précédente"), QString::fromUtf8("previous"));
                ui->comboValue->addItem(QString::fromUtf8("Allumer le lecteur"), QString::fromUtf8("power on"));
                ui->comboValue->addItem(QString::fromUtf8("Eteindre le lecteur"), QString::fromUtf8("power off"));
                ui->comboValue->addItem(QString::fromUtf8("Eteindre dans X sec."), QString::fromUtf8("sleep "));
                ui->comboValue->addItem(QString::fromUtf8("Synchronise avec player_id"), QString::fromUtf8("sync "));
                ui->comboValue->addItem(QString::fromUtf8("Désynchronise avec player_id"), QString::fromUtf8("unsync "));
                ui->comboValue->addItem(QString::fromUtf8("Lire élément"), QString::fromUtf8("play "));
                ui->comboValue->addItem(QString::fromUtf8("Ajouter élément"), QString::fromUtf8("add "));

                if (action->get_params().get_param(id) == "play")
                        ui->comboValue->setCurrentIndex(0);
                else if (action->get_params().get_param(id) == "pause")
                        ui->comboValue->setCurrentIndex(1);
                else if (action->get_params().get_param(id) == "stop")
                        ui->comboValue->setCurrentIndex(2);
                else if (action->get_params().get_param(id) == "next")
                        ui->comboValue->setCurrentIndex(3);
                else if (action->get_params().get_param(id) == "previous")
                        ui->comboValue->setCurrentIndex(4);
                else if (action->get_params().get_param(id) == "power on")
                        ui->comboValue->setCurrentIndex(5);
                else if (action->get_params().get_param(id) == "power off")
                        ui->comboValue->setCurrentIndex(6);
                else
                {
                        QString s = QString::fromUtf8(action->get_params().get_param(id).c_str());
                        ui->comboValue->lineEdit()->setText(s);
                }
        }
        else if (IOBase::isCameraType(type))
        {
                ui->comboValue->setEditable(true);

                ui->comboValue->addItem(QString::fromUtf8("Rappeler position X"), QString::fromUtf8("recall "));
                ui->comboValue->addItem(QString::fromUtf8("Sauvegarder position X"), QString::fromUtf8("save "));
                ui->comboValue->addItem(QString::fromUtf8("Déplacer vers le haut"), QString::fromUtf8("move up"));
                ui->comboValue->addItem(QString::fromUtf8("Déplacer vers le bas"), QString::fromUtf8("move down"));
                ui->comboValue->addItem(QString::fromUtf8("Déplacer vers la gauche"), QString::fromUtf8("move left"));
                ui->comboValue->addItem(QString::fromUtf8("Déplacer vers la droite"), QString::fromUtf8("move right"));
                ui->comboValue->addItem(QString::fromUtf8("Mettre position par defaut"), QString::fromUtf8("move home"));

                if (action->get_params().get_param(id) == "move up")
                        ui->comboValue->setCurrentIndex(2);
                else if (action->get_params().get_param(id) == "move down")
                        ui->comboValue->setCurrentIndex(3);
                else if (action->get_params().get_param(id) == "move left")
                        ui->comboValue->setCurrentIndex(4);
                else if (action->get_params().get_param(id) == "move right")
                        ui->comboValue->setCurrentIndex(5);
                else if (action->get_params().get_param(id) == "move home")
                        ui->comboValue->setCurrentIndex(6);
                else
                {
                        QString s = QString::fromUtf8(action->get_params().get_param(id).c_str());
                        ui->comboValue->lineEdit()->setText(s);
                }
        }
        else
        {
                ui->comboValue->setEditable(true);
                QString s = QString::fromUtf8(action->get_params().get_param(id).c_str());
                ui->comboValue->lineEdit()->setText(s);
        }

}

void FormActionStd::on_btMore_clicked()
{
        Output *output = NULL;
        if (action->get_size() > 0)
                output = action->get_output(0);

        if (!output) return;

        string type = output->get_param("type");
        string id = output->get_param("id");
        if (IOBase::isAudioType(output->get_param("type")) ||
            IOBase::isCameraType(output->get_param("type")))
                id = output->get_param("oid");

        DialogIOList dio(NULL, output);

        if (dio.exec() == QDialog::Accepted)
        {
                 string var_id = dio.getOutput()->get_param("id");
                 if (IOBase::isAudioType(dio.getOutput()->get_param("type")) ||
                     IOBase::isCameraType(dio.getOutput()->get_param("type")))
                         var_id = dio.getOutput()->get_param("iid");

                 action->get_params_var().Add(id, var_id);

                 FormRules::updateItemAction(qitem, action);
        }
}

void FormActionStd::on_comboValue_editTextChanged(QString)
{
        Output *output = NULL;
        if (action->get_size() > 0)
                output = action->get_output(0);

        if (!output) return;

        string type = output->get_param("type");
        string id = output->get_param("id");
        if (IOBase::isAudioType(output->get_param("type")) ||
            IOBase::isCameraType(output->get_param("type")))
                id = output->get_param("oid");

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

        action->get_params().Add(id, value);
        action->get_params_var().Delete(id);

        FormRules::updateItemAction(qitem, action);
}

void FormActionStd::on_comboValue_currentIndexChanged(int)
{
        on_comboValue_editTextChanged("");
}
