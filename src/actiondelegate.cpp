#include "actiondelegate.h"

ActionDelegate::ActionDelegate(Action *act, Output *out, QObject */*parent*/):
                action(act), output(out)
{
}

QWidget *ActionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
        if (index.column() == 1) /* Edit value */
        {
                IOEditorSelection *object = new IOEditorSelection(parent);

                QSignalMapper *sig = new QSignalMapper((QObject *)this);
                connect(object->getButton(), SIGNAL(clicked()), sig, SLOT(map()));
                sig->setMapping(object->getButton(), (QWidget *)object);
                connect(sig, SIGNAL(mapped(QWidget *)), this, SLOT(editMoreClick(QWidget *)));

                string type = output->get_param("type");

                if (type == "OutputFake" || type == "InputTimer" ||
                    type == "scenario" || type == "InternalBool")
                {
                        object->getComboBox()->addItem(QString::fromUtf8("Activer"), QString::fromUtf8("true"));
                        object->getComboBox()->addItem(QString::fromUtf8("Désactiver"), QString::fromUtf8("false"));
                        object->getComboBox()->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                }
                else if (type == "OutTouchscreen")
                {
                        object->getComboBox()->setEditable(true);

                        object->getComboBox()->addItem(QString::fromUtf8("Afficher caméra n°<num>"), QString::fromUtf8("show,cam,"));
                }
                else if (type == "WODigital")
                {
                        object->getComboBox()->setEditable(true);

                        object->getComboBox()->addItem(QString::fromUtf8("Activer"), QString::fromUtf8("true"));
                        object->getComboBox()->addItem(QString::fromUtf8("Désactiver"), QString::fromUtf8("false"));
                        object->getComboBox()->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                        object->getComboBox()->addItem(QString::fromUtf8("Impulsion X milisecondes"), QString::fromUtf8("impulse "));
                }
                else if (type == "WOVolet")
                {
                        object->getComboBox()->setEditable(true);

                        object->getComboBox()->addItem(QString::fromUtf8("Monter"), QString::fromUtf8("up"));
                        object->getComboBox()->addItem(QString::fromUtf8("Descendre"), QString::fromUtf8("down"));
                        object->getComboBox()->addItem(QString::fromUtf8("Arrêter"), QString::fromUtf8("stop"));
                        object->getComboBox()->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                        object->getComboBox()->addItem(QString::fromUtf8("Impulsion sur la montée de X ms"), QString::fromUtf8("impulse up "));
                        object->getComboBox()->addItem(QString::fromUtf8("Impulsion sur la descente de X ms"), QString::fromUtf8("impulse down "));
                }
                else if (type == "WOVoletSmart")
                {
                        object->getComboBox()->setEditable(true);

                        object->getComboBox()->addItem(QString::fromUtf8("Monter"), QString::fromUtf8("up"));
                        object->getComboBox()->addItem(QString::fromUtf8("Descendre"), QString::fromUtf8("down"));
                        object->getComboBox()->addItem(QString::fromUtf8("Arrêter"), QString::fromUtf8("stop"));
                        object->getComboBox()->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                        object->getComboBox()->addItem(QString::fromUtf8("Impulsion sur la montée de X ms"), QString::fromUtf8("impulse up "));
                        object->getComboBox()->addItem(QString::fromUtf8("Impulsion sur la descente de X ms"), QString::fromUtf8("impulse down "));
                        object->getComboBox()->addItem(QString::fromUtf8("Mettre à la position [0-100]"), QString::fromUtf8("set "));
                        object->getComboBox()->addItem(QString::fromUtf8("Monter de X pourcent"), QString::fromUtf8("up "));
                        object->getComboBox()->addItem(QString::fromUtf8("Descendre de X pourcent"), QString::fromUtf8("down "));
                        object->getComboBox()->addItem(QString::fromUtf8("Calibrer"), QString::fromUtf8("calibrate"));
                }
                else if (type == "WODali" || type == "WONeon")
                {
                        object->getComboBox()->setEditable(true);

                        object->getComboBox()->addItem(QString::fromUtf8("Allumer"), QString::fromUtf8("true"));
                        object->getComboBox()->addItem(QString::fromUtf8("Eteindre"), QString::fromUtf8("false"));
                        object->getComboBox()->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                        object->getComboBox()->addItem(QString::fromUtf8("Mettre à X pourcent"), QString::fromUtf8("set "));
                        object->getComboBox()->addItem(QString::fromUtf8("Monter de X pourcent"), QString::fromUtf8("up "));
                        object->getComboBox()->addItem(QString::fromUtf8("Descendre de X pourcent"), QString::fromUtf8("down "));
                        object->getComboBox()->addItem(QString::fromUtf8("Mettre à X pourcent (OFF)"), QString::fromUtf8("set off "));
                }
                else if (type == "WODaliRVB")
                {
                        object->getComboBox()->setEditable(true);

                        object->getComboBox()->addItem(QString::fromUtf8("Allumer"), QString::fromUtf8("true"));
                        object->getComboBox()->addItem(QString::fromUtf8("Eteindre"), QString::fromUtf8("false"));
                        object->getComboBox()->addItem(QString::fromUtf8("Inverser"), QString::fromUtf8("toggle"));
                        object->getComboBox()->addItem(QString::fromUtf8("Mettre à X couleur [0-65535]"), QString::fromUtf8("set "));
                        object->getComboBox()->addItem(QString::fromUtf8("Monter rouge de X pourcent"), QString::fromUtf8("up_red "));
                        object->getComboBox()->addItem(QString::fromUtf8("Descendre rouge de X pourcent"), QString::fromUtf8("down_red "));
                        object->getComboBox()->addItem(QString::fromUtf8("Mettre rouge à X pourcent"), QString::fromUtf8("set_red "));
                        object->getComboBox()->addItem(QString::fromUtf8("Monter vert de X pourcent"), QString::fromUtf8("up_green "));
                        object->getComboBox()->addItem(QString::fromUtf8("Descendre vert de X pourcent"), QString::fromUtf8("down_green "));
                        object->getComboBox()->addItem(QString::fromUtf8("Mettre vert à X pourcent"), QString::fromUtf8("set_green "));
                        object->getComboBox()->addItem(QString::fromUtf8("Monter bleu de X pourcent"), QString::fromUtf8("up_blue "));
                        object->getComboBox()->addItem(QString::fromUtf8("Descendre bleu de X pourcent"), QString::fromUtf8("down_blue "));
                        object->getComboBox()->addItem(QString::fromUtf8("Mettre bleu à X pourcent"), QString::fromUtf8("set_blue "));
                }
                else if (IOBase::isAudioType(type))
                {
                        object->getComboBox()->setEditable(true);

                        object->getComboBox()->addItem(QString::fromUtf8("Lecture"), QString::fromUtf8("play"));
                        object->getComboBox()->addItem(QString::fromUtf8("Pause"), QString::fromUtf8("pause"));
                        object->getComboBox()->addItem(QString::fromUtf8("Stop"), QString::fromUtf8("stop"));
                        object->getComboBox()->addItem(QString::fromUtf8("Piste suivante"), QString::fromUtf8("next"));
                        object->getComboBox()->addItem(QString::fromUtf8("Piste précédente"), QString::fromUtf8("previous"));
                        object->getComboBox()->addItem(QString::fromUtf8("Allumer le lecteur"), QString::fromUtf8("power on"));
                        object->getComboBox()->addItem(QString::fromUtf8("Eteindre le lecteur"), QString::fromUtf8("power off"));
                        object->getComboBox()->addItem(QString::fromUtf8("Eteindre dans X sec."), QString::fromUtf8("sleep "));
                        object->getComboBox()->addItem(QString::fromUtf8("Synchronise avec player_id"), QString::fromUtf8("sync "));
                        object->getComboBox()->addItem(QString::fromUtf8("Désynchronise avec player_id"), QString::fromUtf8("unsync "));
                        object->getComboBox()->addItem(QString::fromUtf8("Lire élément"), QString::fromUtf8("play "));
                        object->getComboBox()->addItem(QString::fromUtf8("Ajouter élément"), QString::fromUtf8("add "));
                }
                else if (IOBase::isCameraType(type))
                {
                        object->getComboBox()->setEditable(true);

                        object->getComboBox()->addItem(QString::fromUtf8("Rappeler position"), QString::fromUtf8("recall "));
                        object->getComboBox()->addItem(QString::fromUtf8("Sauvegarder position"), QString::fromUtf8("save "));
                        object->getComboBox()->addItem(QString::fromUtf8("Déplacer vers le haut"), QString::fromUtf8("move up"));
                        object->getComboBox()->addItem(QString::fromUtf8("Déplacer vers le bas"), QString::fromUtf8("move down"));
                        object->getComboBox()->addItem(QString::fromUtf8("Déplacer vers la gauche"), QString::fromUtf8("move left"));
                        object->getComboBox()->addItem(QString::fromUtf8("Déplacer vers la droite"), QString::fromUtf8("move right"));
                        object->getComboBox()->addItem(QString::fromUtf8("Mettre position par defaut"), QString::fromUtf8("move home"));
                }
                else
                {
                        object->getComboBox()->setEditable(true);
                }

                return object;
        }

        return NULL;
}

void ActionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
        if (index.column() == 1) /* Edit value */
        {
                IOEditorSelection *object = dynamic_cast<IOEditorSelection *>(editor);

                string type = output->get_param("type");

                if (type == "OutputFake" || type == "InputTimer" ||
                    type == "scenario" || type == "InternalBool")
                {
                        if (action->get_params().get_param(output->get_param("id")) == "true")
                                object->getComboBox()->setCurrentIndex(0);
                        else if (action->get_params().get_param(output->get_param("id")) == "false")
                                object->getComboBox()->setCurrentIndex(1);
                        else if (action->get_params().get_param(output->get_param("id")) == "toggle")
                                object->getComboBox()->setCurrentIndex(2);
                }
                else if (type == "WODigital" || type == "WODali" || type == "WODaliRVB" || type == "WONeon")
                {
                        if (action->get_params().get_param(output->get_param("id")) == "true")
                                object->getComboBox()->setCurrentIndex(0);
                        else if (action->get_params().get_param(output->get_param("id")) == "false")
                                object->getComboBox()->setCurrentIndex(1);
                        else if (action->get_params().get_param(output->get_param("id")) == "toggle")
                                object->getComboBox()->setCurrentIndex(2);
                        else
                        {
                                QString s = QString::fromUtf8(action->get_params().get_param(output->get_param("id")).c_str());
                                object->getComboBox()->lineEdit()->setText(s);
                        }
                }
                else if (type == "WOVolet" || type == "WOVoletSmart")
                {
                        if (action->get_params().get_param(output->get_param("id")) == "up")
                                object->getComboBox()->setCurrentIndex(0);
                        else if (action->get_params().get_param(output->get_param("id")) == "down")
                                object->getComboBox()->setCurrentIndex(1);
                        else if (action->get_params().get_param(output->get_param("id")) == "stop")
                                object->getComboBox()->setCurrentIndex(2);
                        else if (action->get_params().get_param(output->get_param("id")) == "toggle")
                                object->getComboBox()->setCurrentIndex(3);
                        else
                        {
                                QString s = QString::fromUtf8(action->get_params().get_param(output->get_param("id")).c_str());
                                object->getComboBox()->lineEdit()->setText(s);
                        }
                }
                else if (IOBase::isAudioType(type))
                {
                        if (action->get_params().get_param(output->get_param("id")) == "play")
                                object->getComboBox()->setCurrentIndex(0);
                        else if (action->get_params().get_param(output->get_param("id")) == "pause")
                                object->getComboBox()->setCurrentIndex(1);
                        else if (action->get_params().get_param(output->get_param("id")) == "stop")
                                object->getComboBox()->setCurrentIndex(2);
                        else if (action->get_params().get_param(output->get_param("id")) == "next")
                                object->getComboBox()->setCurrentIndex(3);
                        else if (action->get_params().get_param(output->get_param("id")) == "previous")
                                object->getComboBox()->setCurrentIndex(4);
                        else if (action->get_params().get_param(output->get_param("id")) == "power on")
                                object->getComboBox()->setCurrentIndex(5);
                        else if (action->get_params().get_param(output->get_param("id")) == "power off")
                                object->getComboBox()->setCurrentIndex(6);
                        else
                        {
                                QString s = QString::fromUtf8(action->get_params().get_param(output->get_param("id")).c_str());
                                object->getComboBox()->lineEdit()->setText(s);
                        }
                }
                else if (IOBase::isCameraType(type))
                {
                        if (action->get_params().get_param(output->get_param("id")) == "move up")
                                object->getComboBox()->setCurrentIndex(2);
                        else if (action->get_params().get_param(output->get_param("id")) == "move down")
                                object->getComboBox()->setCurrentIndex(3);
                        else if (action->get_params().get_param(output->get_param("id")) == "move left")
                                object->getComboBox()->setCurrentIndex(4);
                        else if (action->get_params().get_param(output->get_param("id")) == "move right")
                                object->getComboBox()->setCurrentIndex(5);
                        else if (action->get_params().get_param(output->get_param("id")) == "move home")
                                object->getComboBox()->setCurrentIndex(6);
                        else
                        {
                                QString s = QString::fromUtf8(action->get_params().get_param(output->get_param("id")).c_str());
                                object->getComboBox()->lineEdit()->setText(s);
                        }
                }
                else
                {
                        QString s = QString::fromUtf8(action->get_params().get_param(output->get_param("id")).c_str());
                        object->getComboBox()->lineEdit()->setText(s);
                }
        }
}

void ActionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
        if (index.column() == 1) /* Edit value */
        {
                IOEditorSelection *object = dynamic_cast<IOEditorSelection *>(editor);

                string value;
                int current = object->getComboBox()->currentIndex();

                if (object->other_output)
                {
                        string var_id = object->other_output->get_param("id");
                        value = ListeRoom::Instance().get_output(var_id)->get_param("name");
                        model->setData(index, QString::fromUtf8(value.c_str()), Qt::DisplayRole);
                        action->get_params_var().Add(output->get_param("id"), var_id);
                }
                else
                {
                        if (object->getComboBox()->isEditable() && current > -1 &&
                            object->getComboBox()->itemData(current).isNull())
                        {
                                value = object->getComboBox()->lineEdit()->text().toLocal8Bit().constData();
                        }
                        else
                        {
                                value = object->getComboBox()->itemData(current).toString().toLocal8Bit().constData();
                        }

                        model->setData(index, QString::fromUtf8(value.c_str()), Qt::DisplayRole);
                        action->get_params().Add(output->get_param("id"), value);
                        action->get_params_var().Delete(output->get_param("id"));
                }
        }
}

void ActionDelegate::editMoreClick(QWidget *editor)
{
        DialogIOList dio(NULL, output);

        if (dio.exec() == QDialog::Accepted)
        {
                IOEditorSelection *object = dynamic_cast<IOEditorSelection *>(editor);
                object->other_output = dio.getOutput();

                emit commitData(editor);
                emit closeEditor(editor);
        }
}

//void ActionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
//{
//        editor->setGeometry(option.rect);
//}
