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
        addActionMenu(QString::fromUtf8("start / true"), QString::fromUtf8("Démarre la tempo"), QString::fromUtf8("true"));
        addActionMenu(QString::fromUtf8("stop / false"), QString::fromUtf8("Arrête la tempo"), QString::fromUtf8("false"));
        addActionMenu(QString::fromUtf8("h:m:s:ms"), QString::fromUtf8("Change la durée de la tempo"), QString::fromUtf8("0:0:0:200"));
    }
    else if (gtype == "scenario")
    {
        addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("Lance le scénario"), QString::fromUtf8("true"));
        addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("Arrête le scénario (si possible)"), QString::fromUtf8("false"));
    }
    else if (gtype == "var_bool")
    {
        addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("Mets l'état actif"), QString::fromUtf8("true"));
        addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("Mets l'état inactif"), QString::fromUtf8("false"));
        addActionMenu(QString::fromUtf8("toggle"), QString::fromUtf8("Inverse l'état"), QString::fromUtf8("toggle"));
    }
    else if (gtype == "light")
    {
        addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("Allume la lumière"), QString::fromUtf8("true"));
        addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("Eteint la lumière"), QString::fromUtf8("false"));
        addActionMenu(QString::fromUtf8("toggle"), QString::fromUtf8("Inverser l'état de la lumière"), QString::fromUtf8("toggle"));
        addActionMenu(QString::fromUtf8("impulse X"), QString::fromUtf8("Allume la lumière pendant X ms"), QString::fromUtf8("impulse 500"));
        addActionMenu(QString::fromUtf8("impulse W X Y Z"), QString::fromUtf8("Allume/éteint la lumière suivant un schéma"), QString::fromUtf8("impulse 500 200 500 200"));
    }
    else if (gtype == "shutter")
    {
        addActionMenu(QString::fromUtf8("up"), QString::fromUtf8("Monte le volet"), QString::fromUtf8("up"));
        addActionMenu(QString::fromUtf8("down"), QString::fromUtf8("Descend le volet"), QString::fromUtf8("down"));
        addActionMenu(QString::fromUtf8("stop"), QString::fromUtf8("Arrête le volet"), QString::fromUtf8("stop"));
        addActionMenu(QString::fromUtf8("toggle"), QString::fromUtf8("Inverse l'état du volet"), QString::fromUtf8("toggle"));
        addActionMenu(QString::fromUtf8("impulse up X"), QString::fromUtf8("Impulsion sur la montée de X ms"), QString::fromUtf8("impulse up 200"));
        addActionMenu(QString::fromUtf8("impulse down X"), QString::fromUtf8("Impulsion sur la montée de X ms"), QString::fromUtf8("impulse down 200"));
    }
    else if (gtype == "shutter_smart")
    {
        addActionMenu(QString::fromUtf8("up"), QString::fromUtf8("Monte le volet"), QString::fromUtf8("up"));
        addActionMenu(QString::fromUtf8("down"), QString::fromUtf8("Descend le volet"), QString::fromUtf8("down"));
        addActionMenu(QString::fromUtf8("stop"), QString::fromUtf8("Arrête le volet"), QString::fromUtf8("stop"));
        addActionMenu(QString::fromUtf8("toggle"), QString::fromUtf8("Inverse l'état du volet"), QString::fromUtf8("toggle"));
        addActionMenu(QString::fromUtf8("impulse up X"), QString::fromUtf8("Impulsion sur la montée de X ms"), QString::fromUtf8("impulse up 200"));
        addActionMenu(QString::fromUtf8("impulse down X"), QString::fromUtf8("Impulsion sur la montée de X ms"), QString::fromUtf8("impulse down 200"));
        addActionMenu(QString::fromUtf8("set X"), QString::fromUtf8("Mettre la volet à la position X pourcent"), QString::fromUtf8("set 50"));
        addActionMenu(QString::fromUtf8("up X"), QString::fromUtf8("Monte le volet de X pourcent"), QString::fromUtf8("up 2"));
        addActionMenu(QString::fromUtf8("down X"), QString::fromUtf8("Descend le volet de X pourcent"), QString::fromUtf8("down 2"));
        addActionMenu(QString::fromUtf8("calibrate"), QString::fromUtf8("Lance la calibration du volet"), QString::fromUtf8("calibrate"));
    }
    else if (gtype == "light_dimmer")
    {
        addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("Allume la lumière"), QString::fromUtf8("true"));
        addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("Eteint la lumière"), QString::fromUtf8("false"));
        addActionMenu(QString::fromUtf8("toggle"), QString::fromUtf8("Inverser l'état de la lumière"), QString::fromUtf8("toggle"));
        addActionMenu(QString::fromUtf8("set X"), QString::fromUtf8("Mettre la lumière à X pourcent"), QString::fromUtf8("set 50"));
        addActionMenu(QString::fromUtf8("set off X"), QString::fromUtf8("Mettre à X pourcent sans allumer"), QString::fromUtf8("set off 50"));
        addActionMenu(QString::fromUtf8("up X"), QString::fromUtf8("Monter l'intensité de X pourcent"), QString::fromUtf8("up 2"));
        addActionMenu(QString::fromUtf8("down X"), QString::fromUtf8("Descendre l'intensité de X pourcent"), QString::fromUtf8("down 2"));
        addActionMenu(QString::fromUtf8("hold press"), QString::fromUtf8("Varier l'intensité appui"), QString::fromUtf8("hold press"));
        addActionMenu(QString::fromUtf8("hold stop"), QString::fromUtf8("Varier l'intensité relachement"), QString::fromUtf8("hold stop"));
    }
    else if (gtype == "light_rgb")
    {
        addActionMenu(QString::fromUtf8("true"), QString::fromUtf8("Allume la lumière"), QString::fromUtf8("true"));
        addActionMenu(QString::fromUtf8("false"), QString::fromUtf8("Eteint la lumière"), QString::fromUtf8("false"));
        addActionMenu(QString::fromUtf8("toggle"), QString::fromUtf8("Inverser l'état de la lumière"), QString::fromUtf8("toggle"));
        addActionMenu(QString::fromUtf8("set X"), QString::fromUtf8("Mettre la couleur à la valeur X"), QString::fromUtf8("set 0"));
        addActionMenu(QString::fromUtf8("up_red X"), QString::fromUtf8("Monter le rouge de X pourcent"), QString::fromUtf8("up_red 1"));
        addActionMenu(QString::fromUtf8("down_red X"), QString::fromUtf8("Descendre le rouge de X pourcent"), QString::fromUtf8("down_red 1"));
        addActionMenu(QString::fromUtf8("set_red X"), QString::fromUtf8("Mettre le rouge à X pourcent"), QString::fromUtf8("set_red 1"));
        addActionMenu(QString::fromUtf8("up_green X"), QString::fromUtf8("Monter le vert de X pourcent"), QString::fromUtf8("up_green 1"));
        addActionMenu(QString::fromUtf8("down_green X"), QString::fromUtf8("Descendre le vert de X pourcent"), QString::fromUtf8("down_green 1"));
        addActionMenu(QString::fromUtf8("set_green X"), QString::fromUtf8("Mettre le vert à X pourcent"), QString::fromUtf8("set_green 1"));
        addActionMenu(QString::fromUtf8("up_blue X"), QString::fromUtf8("Monter le bleu de X pourcent"), QString::fromUtf8("up_blue 1"));
        addActionMenu(QString::fromUtf8("down_blue X"), QString::fromUtf8("Descendre le bleu de X pourcent"), QString::fromUtf8("down_blue 1"));
        addActionMenu(QString::fromUtf8("set_blue X"), QString::fromUtf8("Mettre le bleu à X pourcent"), QString::fromUtf8("set_blue 1"));
    }
    else if (gtype == "audio")
    {
        addActionMenu(QString::fromUtf8("play"), QString::fromUtf8("Lecture"), QString::fromUtf8("play"));
        addActionMenu(QString::fromUtf8("pause"), QString::fromUtf8("Pause"), QString::fromUtf8("pause"));
        addActionMenu(QString::fromUtf8("stop"), QString::fromUtf8("Stop"), QString::fromUtf8("stop"));
        addActionMenu(QString::fromUtf8("next"), QString::fromUtf8("Piste suivante"), QString::fromUtf8("next"));
        addActionMenu(QString::fromUtf8("previous"), QString::fromUtf8("Piste précédente"), QString::fromUtf8("previous"));
        addActionMenu(QString::fromUtf8("volume set X"), QString::fromUtf8("Change le volume a X pourcent"), QString::fromUtf8("volume set 10"));
        addActionMenu(QString::fromUtf8("volume up X"), QString::fromUtf8("Monte le volume de X pourcent"), QString::fromUtf8("volume up 1"));
        addActionMenu(QString::fromUtf8("volumle down X"), QString::fromUtf8("Descend le volume de X pourcent"), QString::fromUtf8("volume down 1"));
        addActionMenu(QString::fromUtf8("power on"), QString::fromUtf8("Allumer le lecteur"), QString::fromUtf8("power on"));
        addActionMenu(QString::fromUtf8("power off"), QString::fromUtf8("Eteindre le lecteur"), QString::fromUtf8("power off"));
        addActionMenu(QString::fromUtf8("sleep X"), QString::fromUtf8("Eteindre dans X sec"), QString::fromUtf8("sleep 60"));
        addActionMenu(QString::fromUtf8("sync ID"), QString::fromUtf8("Synchronise avec un autre lecteur"), QString::fromUtf8("sync 00:11:22:33:44:55"));
        addActionMenu(QString::fromUtf8("unsync ID"), QString::fromUtf8("Désynchronise avec un autre lecteur"), QString::fromUtf8("unsync 00:11:22:33:44:55"));
        addActionMenu(QString::fromUtf8("play ID"), QString::fromUtf8("Lire un élément de la base de donnée"), QString::fromUtf8("play 0"));
        addActionMenu(QString::fromUtf8("add ID"), QString::fromUtf8("Ajoute un élément à la playlist"), QString::fromUtf8("add 0"));
    }
    else if (gtype == "camera")
    {
        addActionMenu(QString::fromUtf8("recall X"), QString::fromUtf8("Rappeler la position X"), QString::fromUtf8("recall 0"));
        addActionMenu(QString::fromUtf8("save X"), QString::fromUtf8("Sauvegarder à la position X"), QString::fromUtf8("save 0"));
        addActionMenu(QString::fromUtf8("move up"), QString::fromUtf8("Déplacer vers le haut"), QString::fromUtf8("move up"));
        addActionMenu(QString::fromUtf8("move down"), QString::fromUtf8("Déplacer vers le bas"), QString::fromUtf8("move down"));
        addActionMenu(QString::fromUtf8("move left"), QString::fromUtf8("Déplacer vers la gauche"), QString::fromUtf8("move left"));
        addActionMenu(QString::fromUtf8("move right"), QString::fromUtf8("Déplacer vers la droite"), QString::fromUtf8("move right"));
        addActionMenu(QString::fromUtf8("move home"), QString::fromUtf8("Déplacer vers la position par défaut"), QString::fromUtf8("move home"));
    }
    else if (gtype == "avreceiver")
    {
        addActionMenu("volume X", tr("Set volume to X percent"), "volume 10");
        addActionMenu("power on", tr("Switch on the receiver"), "power on");
        addActionMenu("power off", tr("Switch off the receiver"), "power off");
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

void FormActionStd::addActionMenu(QString action, QString help, QString cmd)
{
    QString h = RuleActionTpl.arg(help);
    RuleActionMenu *ac = new RuleActionMenu(NULL, action, h, cmd);
    actionMenu->addAction(ac);
    connect(ac, SIGNAL(triggered(RuleActionMenu*)), this, SLOT(menuAction(RuleActionMenu*)));
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
