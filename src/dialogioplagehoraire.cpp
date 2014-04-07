#include "dialogioplagehoraire.h"
#include "ui_DialogPlageHoraire.h"

#include "dialogplageinput.h"
#include <QTime>

DialogPlageHoraire::DialogPlageHoraire(IOBase *p, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogPlageHoraire),
        plage(p)
{
        ui->setupUi(this);

        if (!p)
        {
                cout << "DialogPlageHoraire:: p is NULL !!" << endl;
        }

        ui->listDay->setCurrentRow(0);
}

DialogPlageHoraire::~DialogPlageHoraire()
{
        delete ui;
}

void DialogPlageHoraire::changeEvent(QEvent *e)
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


void DialogPlageHoraire::on_listDay_currentRowChanged(int currentRow)
{
        if (currentRow == 0) day = &plage->plg_lundi;
        if (currentRow == 1) day = &plage->plg_mardi;
        if (currentRow == 2) day = &plage->plg_mercredi;
        if (currentRow == 3) day = &plage->plg_jeudi;
        if (currentRow == 4) day = &plage->plg_vendredi;
        if (currentRow == 5) day = &plage->plg_samedi;
        if (currentRow == 6) day = &plage->plg_dimanche;

        ui->treeHoraire->clear();

        for (uint i = 0;i < day->size();i++)
        {
                Horaire &h = (*day)[i];

                QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeHoraire);
                item->setData(0, Qt::DisplayRole, QString("%1:%2:%3").arg(h.shour.c_str(),
                                                                          h.smin.c_str(),
                                                                          h.ssec.c_str()));
                item->setData(1, Qt::DisplayRole, QString("%1:%2:%3").arg(h.ehour.c_str(),
                                                                          h.emin.c_str(),
                                                                          h.esec.c_str()));
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_clock.png"));
        }
}

void DialogPlageHoraire::on_addButton_clicked()
{
        DialogPlageInput d;
        if (d.exec() == QDialog::Accepted)
        {
                QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeHoraire);

                item->setData(0, Qt::DisplayRole, d.getTimeBegin().toString("H:m:s"));
                item->setData(1, Qt::DisplayRole, d.getTimeEnd().toString("H:m:s"));
                item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_clock.png"));

                item->setSelected(true);
                ui->treeHoraire->setCurrentItem(item);

                Horaire h;
                h.shour = to_string(d.getTimeBegin().hour());
                h.smin = to_string(d.getTimeBegin().minute());
                h.ssec = to_string(d.getTimeBegin().second());
                h.ehour = to_string(d.getTimeEnd().hour());
                h.emin = to_string(d.getTimeEnd().minute());
                h.esec = to_string(d.getTimeEnd().second());

                day->push_back(h);
        }
}

void DialogPlageHoraire::on_delButton_clicked()
{
        if (ui->treeHoraire->currentItem())
        {
                vector<Horaire>::iterator it;
                int cpt = 0;
                for (it = day->begin(); it != day->end() && cpt != ui->treeHoraire->currentIndex().row(); it++, cpt++) ;
                day->erase(it);

                delete ui->treeHoraire->currentItem();
        }
}

void DialogPlageHoraire::on_modifyButton_clicked()
{
        if (ui->treeHoraire->currentItem())
        {
                Horaire &h = (*day)[ui->treeHoraire->currentIndex().row()];

                int shour, smin, ssec;
                int ehour, emin, esec;
                from_string(h.shour, shour);
                from_string(h.smin, smin);
                from_string(h.ssec, ssec);
                from_string(h.ehour, ehour);
                from_string(h.emin, emin);
                from_string(h.esec, esec);

                QTime t1(shour, smin, ssec), t2(ehour, emin, esec);

                DialogPlageInput d;
                d.setTimeBegin(t1);
                d.setTimeEnd(t2);
                if (d.exec() == QDialog::Accepted)
                {
                        h.shour = to_string(d.getTimeBegin().hour());
                        h.smin = to_string(d.getTimeBegin().minute());
                        h.ssec = to_string(d.getTimeBegin().second());
                        h.ehour = to_string(d.getTimeEnd().hour());
                        h.emin = to_string(d.getTimeEnd().minute());
                        h.esec = to_string(d.getTimeEnd().second());

                        QTreeWidgetItem *item = ui->treeHoraire->currentItem();

                        item->setData(0, Qt::DisplayRole, d.getTimeBegin().toString("H:m:s"));
                        item->setData(1, Qt::DisplayRole, d.getTimeEnd().toString("H:m:s"));
                }
        }
}
