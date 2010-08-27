#include "dialogiolist.h"
#include "ui_dialogiolist.h"

DialogIOList::DialogIOList(Input *_in, Output *_out, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIOList),
    input(_in), output(_out)
{
        ui->setupUi(this);

        if (input)
        {
                for (int i = 0;i < ListeRoom::Instance().get_nb_input();i++)
                {
                        Input *in = ListeRoom::Instance().get_input(i);

                        if (in == input) continue;

                        if (in->get_type() == input->get_type() ||
                            in->get_type() == TSTRING)
                        {
                                QTreeWidgetItemInput *item = new QTreeWidgetItemInput(in, ui->treeWidget);

                                item->setData(0, Qt::DisplayRole, QString::fromUtf8(in->get_param("type").c_str()));
                                item->setData(1, Qt::DisplayRole, QString::fromUtf8(in->get_param("name").c_str()));

                                if (i == 0)
                                        item->setSelected(true);
                        }
                }
        }
        else if (output)
        {
                for (int i = 0;i < ListeRoom::Instance().get_nb_output();i++)
                {
                        Output *out = ListeRoom::Instance().get_output(i);

                        if (out == output) continue;

                        if (out->get_type() == output->get_type() ||
                            out->get_type() == TSTRING)
                        {
                                QTreeWidgetItemOutput *item = new QTreeWidgetItemOutput(out, ui->treeWidget);

                                item->setData(0, Qt::DisplayRole, QString::fromUtf8(out->get_param("type").c_str()));
                                item->setData(1, Qt::DisplayRole, QString::fromUtf8(out->get_param("name").c_str()));

                                if (i == 0)
                                        item->setSelected(true);
                        }
                }
        }
}

DialogIOList::~DialogIOList()
{
        delete ui;
}

Input *DialogIOList::getInput()
{
        QTreeWidgetItemInput *item = dynamic_cast<QTreeWidgetItemInput *>(ui->treeWidget->currentItem());
        if (item)
                return item->getInput();

        return NULL;
}

Output *DialogIOList::getOutput()
{
        QTreeWidgetItemOutput *item = dynamic_cast<QTreeWidgetItemOutput *>(ui->treeWidget->currentItem());
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

