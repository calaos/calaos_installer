#include "dialogiolist.h"
#include "ui_DialogIoList.h"

DialogIOList::DialogIOList(IOBase *_in, IOBase *_out, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogIOList),
        input(_in),
        output(_out),
        item_current(NULL)
{
        ui->setupUi(this);

        ui->treeWidget->setUpdatesEnabled(false);

        if (input)
        {
                for (int i = 0;i < ListeRoom::Instance().size();i++)
                {
                        Room *room = ListeRoom::Instance().get_room(i);

                        for (int j = 0;j < room->get_size_in();j++)
                        {
                                IOBase *in = room->get_input(j);
                                if (in == input) continue;

                                if (in->get_type() == input->get_type() ||
                                    in->get_type() == TSTRING)
                                {
                                        QTreeWidgetItemInput *item = new QTreeWidgetItemInput(in, ui->treeWidget);

                                        item->setData(0, Qt::DisplayRole, QString::fromUtf8(in->get_param("type").c_str()));
                                        item->setData(1, Qt::DisplayRole, QString::fromUtf8(room->get_name().c_str()));
                                        item->setData(2, Qt::DisplayRole, QString::fromUtf8(in->get_param("name").c_str()));

                                        if (ui->treeWidget->topLevelItemCount() == 1)
                                        {
                                                item->setSelected(true);
                                                item_current = item;
                                        }
                                }
                        }
                }
        }
        else if (output)
        {
                for (int i = 0;i < ListeRoom::Instance().size();i++)
                {
                        Room *room = ListeRoom::Instance().get_room(i);

                        for (int j = 0;j < room->get_size_out();j++)
                        {
                                IOBase *out = room->get_output(j);
                                if (out == output) continue;

                                if (out->get_type() == output->get_type() ||
                                    out->get_type() == TSTRING)
                                {
                                        QTreeWidgetItemOutput *item = new QTreeWidgetItemOutput(out, ui->treeWidget);

                                        item->setData(0, Qt::DisplayRole, QString::fromUtf8(out->get_param("type").c_str()));
                                        item->setData(1, Qt::DisplayRole, QString::fromUtf8(room->get_name().c_str()));
                                        item->setData(2, Qt::DisplayRole, QString::fromUtf8(out->get_param("name").c_str()));

                                        if (ui->treeWidget->topLevelItemCount() == 1)
                                        {
                                                item->setSelected(true);
                                                item_current = item;
                                        }
                                }
                        }
                }
        }

        ui->treeWidget->setUpdatesEnabled(true);

        ui->treeWidget->resizeColumnToContents(0);
        ui->treeWidget->resizeColumnToContents(1);
        ui->treeWidget->resizeColumnToContents(2);
}

DialogIOList::~DialogIOList()
{
        delete ui;
}

IOBase *DialogIOList::getInput()
{
        QTreeWidgetItemInput *item = dynamic_cast<QTreeWidgetItemInput *>(item_current);
        if (item)
                return item->getInput();

        return NULL;
}

IOBase *DialogIOList::getOutput()
{
        QTreeWidgetItemOutput *item = dynamic_cast<QTreeWidgetItemOutput *>(item_current);
        if (item)
                return item->getOutput();

        return NULL;
}

void DialogIOList::changeEvent(QEvent *e)
{
        QDialog::changeEvent(e);
        switch (e->type())
        {
          case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
          default:
                break;
        }
}

void DialogIOList::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
        item_current = current;
}
