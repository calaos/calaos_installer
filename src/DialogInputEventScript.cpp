#include "DialogInputEventScript.h"
#include "ui_DialogInputEventScript.h"

#include <qtreewidget_addition.h>
#include <ListeRoom.h>

DialogInputEventScript::DialogInputEventScript(Condition *cond, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInputEventScript),
    condition(cond)
{
    ui->setupUi(this);

    ui->tree_all->setUpdatesEnabled(false);
    ui->tree_active->setUpdatesEnabled(false);

    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *room = ListeRoom::Instance().get_room(i);

        for (int j = 0;j < room->get_size_in();j++)
        {
            IOBase *in = room->get_input(j);

            QTreeWidgetItemInput *item = new QTreeWidgetItemInput(in, ui->tree_all);

            item->setData(0, Qt::DisplayRole, QString::fromUtf8(in->get_param("name").c_str()));
            item->setData(1, Qt::DisplayRole, QString::fromUtf8(room->get_name().c_str()));
        }
    }

    for (int i = 0;i < condition->getScriptInputSize();i++)
    {
        IOBase *in = condition->getScriptInput(i);

        QTreeWidgetItemInput *item = new QTreeWidgetItemInput(in, ui->tree_active);

        item->setData(0, Qt::DisplayRole, QString::fromUtf8(in->get_param("name").c_str()));

        Room *room = ListeRoom::Instance().searchRoomByInput(in);
        if (room)
            item->setData(1, Qt::DisplayRole, QString::fromUtf8(room->get_name().c_str()));
    }

    ui->tree_all->setUpdatesEnabled(true);
    ui->tree_active->setUpdatesEnabled(true);
}

DialogInputEventScript::~DialogInputEventScript()
{
    delete ui;
}

void DialogInputEventScript::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogInputEventScript::on_buttonBox_accepted()
{
    accept();
}

void DialogInputEventScript::on_btAdd_clicked()
{
    QTreeWidgetItemInput *old_item = reinterpret_cast<QTreeWidgetItemInput *>(ui->tree_all->currentItem());

    if (old_item)
    {
        QTreeWidgetItemInput *item = new QTreeWidgetItemInput(old_item->getInput(), ui->tree_active);
        item->setData(0, Qt::DisplayRole, QString::fromUtf8(old_item->getInput()->get_param("name").c_str()));

        Room *room = ListeRoom::Instance().searchRoomByInput(old_item->getInput());
        if (room)
            item->setData(1, Qt::DisplayRole, QString::fromUtf8(room->get_name().c_str()));

        condition->addScriptInput(item->getInput());

        delete old_item;
    }
}

void DialogInputEventScript::on_btRemove_clicked()
{
    QTreeWidgetItemInput *old_item = reinterpret_cast<QTreeWidgetItemInput *>(ui->tree_active->currentItem());

    if (old_item)
    {
        QTreeWidgetItemInput *item = new QTreeWidgetItemInput(old_item->getInput(), ui->tree_all);
        item->setData(0, Qt::DisplayRole, QString::fromUtf8(old_item->getInput()->get_param("name").c_str()));

        Room *room = ListeRoom::Instance().searchRoomByInput(old_item->getInput());
        if (room)
            item->setData(1, Qt::DisplayRole, QString::fromUtf8(room->get_name().c_str()));

        condition->removeScriptInput(item->getInput());

        delete old_item;
    }
}
