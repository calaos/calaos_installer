#include "conditiondelegate.h"

ConditionDelegate::ConditionDelegate(Condition *cond, Input *in, QObject */*parent*/):
                condition(cond), input(in)
{
}

QWidget *ConditionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
        if (index.column() == 1) /* Edit operator */
        {
                QComboBox *combo = new QComboBox(parent);

                combo->addItem(QString::fromUtf8("=="), QString::fromUtf8("=="));
                combo->addItem(QString::fromUtf8("!="), QString::fromUtf8("!="));
                combo->addItem(QString::fromUtf8(">"), QString::fromUtf8("SUP"));
                combo->addItem(QString::fromUtf8(">="), QString::fromUtf8("SUP="));
                combo->addItem(QString::fromUtf8("<"), QString::fromUtf8("INF"));
                combo->addItem(QString::fromUtf8("<="), QString::fromUtf8("INF="));

                return combo;
        }

        if (index.column() == 2) /* Edit value */
        {
                IOEditorSelection *object = new IOEditorSelection(parent);

                connect(object->getButton(), SIGNAL(clicked()), this, SLOT(editMoreClick()));

                string type = input->get_param("type");

                if (type == "InputTime" || type == "InputTimer" || type == "WIDigitalBP" ||
                    type == "scenario" || type == "InPlageHoraire" || type == "InternalBool")
                {
                        object->getComboBox()->addItem(QString::fromUtf8("Activé"), QString::fromUtf8("true"));
                        object->getComboBox()->addItem(QString::fromUtf8("Désactivé"), QString::fromUtf8("false"));
                        object->getComboBox()->addItem(QString::fromUtf8("Les deux"), QString::fromUtf8("changed"));
                }
                else if (type == "WIDigitalTriple")
                {
                        object->getComboBox()->addItem(QString::fromUtf8("1 appui"), QString::fromUtf8("1"));
                        object->getComboBox()->addItem(QString::fromUtf8("2 appuis"), QString::fromUtf8("2"));
                        object->getComboBox()->addItem(QString::fromUtf8("3 appuis"), QString::fromUtf8("3"));
                }
                else if (type == "WITemp")
                {
                        object->getComboBox()->setEditable(true);

                        for (int i = 10;i < 30;i++)
                                object->getComboBox()->addItem(QString::fromStdString(to_string(i)) + QString::fromUtf8(" °C"), QString::fromStdString(to_string(i)));
                }
                else if (type == "InternalInt")
                {
                        object->getComboBox()->setEditable(true);

                        for (int i = 0;i < 30;i++)
                                object->getComboBox()->addItem(QString::fromStdString(to_string(i)), QString::fromStdString(to_string(i)));
                }
                else if (IOBase::isAudioType(type))
                {
                        object->getComboBox()->addItem(QString::fromUtf8("Lecture"), QString::fromUtf8("onplay"));
                        object->getComboBox()->addItem(QString::fromUtf8("Pause"), QString::fromUtf8("onpause"));
                        object->getComboBox()->addItem(QString::fromUtf8("Stop"), QString::fromUtf8("onstop"));
                        object->getComboBox()->addItem(QString::fromUtf8("Changement de piste"), QString::fromUtf8("onsongchange"));
                        object->getComboBox()->addItem(QString::fromUtf8("Erreur"), QString::fromUtf8("onerror"));
                }
                else
                {
                        object->getComboBox()->setEditable(true);
                }

                return object;
        }

        return NULL;
}

void ConditionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
        if (index.column() == 1) /* Edit operator */
        {
                QComboBox *combo = dynamic_cast<QComboBox *>(editor);

                if (condition->get_operator().get_param(input->get_param("id")) == "==")
                        combo->setCurrentIndex(0);
                else if (condition->get_operator().get_param(input->get_param("id")) == "!=")
                        combo->setCurrentIndex(1);
                else if (condition->get_operator().get_param(input->get_param("id")) == "SUP")
                        combo->setCurrentIndex(2);
                else if (condition->get_operator().get_param(input->get_param("id")) == "SUP=")
                        combo->setCurrentIndex(3);
                else if (condition->get_operator().get_param(input->get_param("id")) == "INF")
                        combo->setCurrentIndex(4);
                else if (condition->get_operator().get_param(input->get_param("id")) == "INF=")
                        combo->setCurrentIndex(5);
        }
        else if (index.column() == 2) /* Edit value */
        {
                IOEditorSelection *object = dynamic_cast<IOEditorSelection *>(editor);

                string type = input->get_param("type");

                if (type == "InputTime" || type == "InputTimer" || type == "WIDigitalBP" ||
                    type == "scenario" || type == "InPlageHoraire" || type == "InternalBool")
                {
                        if (condition->get_params().get_param(input->get_param("id")) == "true")
                                object->getComboBox()->setCurrentIndex(0);
                        else if (condition->get_params().get_param(input->get_param("id")) == "false")
                                object->getComboBox()->setCurrentIndex(1);
                        else if (condition->get_params().get_param(input->get_param("id")) == "changed")
                                object->getComboBox()->setCurrentIndex(2);
                }
                else if (type == "WIDigitalTriple")
                {
                        if (condition->get_params().get_param(input->get_param("id")) == "1")
                                object->getComboBox()->setCurrentIndex(0);
                        else if (condition->get_params().get_param(input->get_param("id")) == "2")
                                object->getComboBox()->setCurrentIndex(1);
                        else if (condition->get_params().get_param(input->get_param("id")) == "3")
                                object->getComboBox()->setCurrentIndex(2);
                }
                else if (IOBase::isAudioType(type))
                {
                        if (condition->get_params().get_param(input->get_param("id")) == "onplay")
                                object->getComboBox()->setCurrentIndex(0);
                        else if (condition->get_params().get_param(input->get_param("id")) == "onpause")
                                object->getComboBox()->setCurrentIndex(1);
                        else if (condition->get_params().get_param(input->get_param("id")) == "onstop")
                                object->getComboBox()->setCurrentIndex(2);
                        else if (condition->get_params().get_param(input->get_param("id")) == "onsongchange")
                                object->getComboBox()->setCurrentIndex(3);
                        else if (condition->get_params().get_param(input->get_param("id")) == "onerror")
                                object->getComboBox()->setCurrentIndex(4);
                }
                else
                {
                        QString s = QString::fromUtf8(condition->get_params().get_param(input->get_param("id")).c_str());
                        object->getComboBox()->lineEdit()->setText(s);
                }
        }
}

void ConditionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
        if (index.column() == 1) /* Edit operator */
        {
                QComboBox *combo = dynamic_cast<QComboBox *>(editor);

                int current = combo->currentIndex();

                model->setData(index, combo->itemText(current), Qt::DisplayRole);
                string v = combo->itemData(current).toString().toLocal8Bit().constData();

                condition->get_operator().Add(input->get_param("id"), v);
        }
        else if (index.column() == 2) /* Edit value */
        {
                IOEditorSelection *object = dynamic_cast<IOEditorSelection *>(editor);

                string value;
                int current = object->getComboBox()->currentIndex();

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
                condition->get_params().Add(input->get_param("id"), value);
        }
}

void ConditionDelegate::editMoreClick()
{
        DialogIOList dio(input, NULL);
        dio.exec();
}

//void ConditionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
//{
//        editor->setGeometry(option.rect);
//}
