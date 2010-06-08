#ifndef FORMDALI_H
#define FORMDALI_H

#include <QtGui>
#include "qanimationlabel.h"

namespace Ui
{
        class FormDali;
}

class FormDali : public QWidget
{
        Q_OBJECT

        public:
                explicit FormDali(QWidget *parent = 0);
                ~FormDali();

                void Init();

        protected:
                void changeEvent(QEvent *e);

        private:
                Ui::FormDali *ui;

                QMenu *menu_addressing;
                QPointer<QTimer> timer_addressing;

                QVector<bool> itemsCache;
                QMap<QListWidgetItem *, int> mapCache;
                QMap<QListWidgetItem *, int>::const_iterator mapIter;

                QVector<QString> cacheGroups;

                QPointer<QAnimationLabel> spinner;

                int currentGroup;

                void DisableControls();
                void EnableControls();

                void LoadGroup();
                void ShowGroup(int grp);

        signals:
                void closeDaliForm();

        private slots:
                void on_buttonGrpBlinkStop_clicked();
                void on_buttonGrpBlink_clicked();
                void on_buttonGrpOff_clicked();
                void on_buttonGrpOn_clicked();
                void on_listGroups_customContextMenuRequested(QPoint pos);
                void on_listLights_customContextMenuRequested(QPoint pos);
                void on_comboGroups_currentIndexChanged(int index);
                void on_sliderLight_sliderReleased();
                void on_buttonAdvanced_clicked();
                void on_buttonBlinkStop_clicked();
                void on_buttonBlink_clicked();
                void on_buttonOff_clicked();
                void on_buttonOn_clicked();
                void on_listLights_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
                void on_pushButton_clicked();
                void on_buttonCentralOff_clicked();
                void on_buttonCentralOn_clicked();
                void on_buttonAddressing_clicked();

                void searchDevices_cb(QString cmd, QString res);

                void newAddressing();
                void expansionAddressing();

                void addToGroup();
                void delFromGroup();

                void deleteAddress();
                void changeAddress(int new_address);

                void addressingInProgress_cb();
                void addressingStatus_cb(QString cmd, QString res);

                void Init_cb(QString cmd, QString res);
                void LoadGroups_cb(QString cmd, QString res);
                void modifyGroup_cb(QString cmd, QString res);
                void advancedInfo_cb(QString cmd, QString res);
};

#endif // FORMDALI_H
