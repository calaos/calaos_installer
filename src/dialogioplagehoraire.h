#ifndef DIALOGPLAGEHORAIRE_H
#define DIALOGPLAGEHORAIRE_H

#include <QtWidgets>
#include <QDialog>
#include <QInputDialog>
#include <QTreeWidgetItem>
#include <QMessageBox>

#include <Params.h>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui
{
        class DialogPlageHoraire;
}

class DialogPlageHoraire : public QDialog
{
        Q_OBJECT
        Q_DISABLE_COPY(DialogPlageHoraire)
        public:
                explicit DialogPlageHoraire(InPlageHoraire *p, QWidget *parent = 0);
                virtual ~DialogPlageHoraire();

                InPlageHoraire *getPlageHoraire() { return plage; }

        protected:
                virtual void changeEvent(QEvent *e);

        private:
                Ui::DialogPlageHoraire *ui;

                InPlageHoraire *plage;
                vector<Horaire> *day;


        private slots:
                void on_modifyButton_clicked();
                void on_delButton_clicked();
                void on_addButton_clicked();
                void on_listDay_currentRowChanged(int currentRow);
};

#endif // DIALOGPLAGEHORAIRE_H
