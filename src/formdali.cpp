#include "formdali.h"
#include "ui_formdali.h"

#include "mainwindow.h"
#include "dialogdalideviceconfig.h"

#include "wagoconnect.h"

FormDali::FormDali(QWidget *parent) :
                QWidget(parent),
                ui(new Ui::FormDali)
{
        ui->setupUi(this);

        QListWidgetItem *item;

        for (int i = 1;i < 65;i++)
        {
                item = new QListWidgetItem(QIcon(":/img/dali.png"), QString::fromUtf8("Lumière #%1").arg(i), ui->listLights);
                item->setFlags(Qt::NoItemFlags);
        }

        ui->comboGroups->addItem(QString::fromUtf8("<Choisir un groupe>"), 0);
        for (int i = 1;i < 17;i++)
                ui->comboGroups->addItem(QString::fromUtf8("Groupe #%1").arg(i), i);

        menu_addressing = new QMenu(this);
        QAction *action = NULL;

        action = menu_addressing->addAction(QString::fromUtf8("Adressage complet"));
        action->setIcon(QIcon(":/img/system-software-update.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(newAddressing()));

        action = menu_addressing->addAction(QString::fromUtf8("Adressage des nouveaux équipements"));
        action->setIcon(QIcon(":/img/system-software-update.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(expansionAddressing()));

}

FormDali::~FormDali()
{
        delete ui;
}

void FormDali::changeEvent(QEvent *e)
{
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}

void FormDali::DisableControls()
{
        ui->buttonAddressing->setEnabled(false);
        ui->buttonCentralOff->setEnabled(false);
        ui->buttonCentralOn->setEnabled(false);
        ui->listLights->setEnabled(false);
        ui->groupProps->setEnabled(false);
        ui->groupBoxGroups->setEnabled(false);

        if (spinner) delete spinner;
        spinner = new QAnimationLabel(":/img/loader.gif", this);
        ui->spinnerLayout->addWidget(spinner, 0, Qt::AlignCenter);
        spinner->start();
}

void FormDali::EnableControls()
{
        ui->buttonAddressing->setEnabled(true);
        ui->buttonCentralOff->setEnabled(true);
        ui->buttonCentralOn->setEnabled(true);
        ui->listLights->setEnabled(true);
        ui->groupProps->setEnabled(true);
        ui->groupBoxGroups->setEnabled(true);

        delete spinner;
}

void FormDali::Init()
{
        DisableControls();

        ui->groupProps->setVisible(false);

        cacheGroups.clear();
        ui->comboGroups->setCurrentIndex(0);
        ui->listGroups->clear();
        mapIter = mapCache.constEnd();

        WagoConnect::Instance().SendCommand("WAGO_DALI_ADDRESSING_STATUS 1", this, "Init_cb"); //to be sure
}

void FormDali::Init_cb(QString cmd, QString res)
{
        if (res == "WAGO_DALI_ADDRESSING_STATUS 1")
                WagoConnect::Instance().SendCommand("WAGO_DALI_GET_ADDR", this, "searchDevices_cb");
        else
                WagoConnect::Instance().SendCommand("WAGO_DALI_ADDRESSING_STATUS 1", this, "Init_cb");
}

void FormDali::searchDevices_cb(QString cmd, QString res)
{
        EnableControls();
        ui->listLights->clear();
        mapCache.clear();
        itemsCache.clear();

        QStringList list = res.split(" ");
        if (list.size() >= 2)
        {
                QString s = list[1];
                for (int i = 0;i < 64 && i < s.length();i++)
                {
                        QListWidgetItem *item = new QListWidgetItem(QIcon(":/img/dali.png"), QString::fromUtf8("Lumière #%1").arg(i + 1), ui->listLights);
                        if (s[i] == '0')
                        {
                                item->setFlags(Qt::NoItemFlags);
                                itemsCache.push_back(false);
                        }
                        else
                        {
                                mapCache[item] = i + 1;
                                itemsCache.push_back(true);
                        }
                }
        }
}

void FormDali::on_buttonAddressing_clicked()
{
        menu_addressing->exec(ui->buttonAddressing->mapToGlobal(QPoint(ui->buttonAddressing->width(),0)));
}

void FormDali::on_buttonCentralOn_clicked()
{
        WagoConnect::Instance().SendCommand("WAGO_DALI_CENTRAL 1 1");
}

void FormDali::on_buttonCentralOff_clicked()
{
        WagoConnect::Instance().SendCommand("WAGO_DALI_CENTRAL 1 0");
}

void FormDali::on_pushButton_clicked()
{
        emit closeDaliForm();

        if (timer_addressing) delete timer_addressing;
}

void FormDali::newAddressing()
{
        if (QMessageBox::question(this, tr("Calaos Installer"),
                                  QString::fromUtf8("Attention !\nEtes-vous sûr de vouloir effectuer un adressage complet?\nLes équipements déjà adressés vont être remis à zéro."),
                                  QMessageBox::Ok,
                                  QMessageBox::Cancel
                ) == QMessageBox::Ok)
        {
                WagoConnect::Instance().SendCommand("WAGO_DALI_ADDRESSING_NEW 1 1");
                DisableControls();

                timer_addressing = new QTimer(this);
                connect(timer_addressing, SIGNAL(timeout()), this, SLOT(addressingInProgress_cb()));
                timer_addressing->start(500);
        }
}

void FormDali::expansionAddressing()
{
        WagoConnect::Instance().SendCommand("WAGO_DALI_ADDRESSING_ADD 1 1");
        DisableControls();

        timer_addressing = new QTimer(this);
        connect(timer_addressing, SIGNAL(timeout()), this, SLOT(addressingInProgress_cb()));
        timer_addressing->start(500);
}

void FormDali::addressingInProgress_cb()
{
        WagoConnect::Instance().SendCommand("WAGO_DALI_ADDRESSING_STATUS 1", this, "addressingStatus_cb");
}

void FormDali::addressingStatus_cb(QString cmd, QString res)
{
        if (res == "WAGO_DALI_ADDRESSING_STATUS 1")
        {
                //Addressing done
                delete timer_addressing;
                EnableControls();
                Init();
        }
}

void FormDali::on_listLights_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
        if (mapCache.contains(current))
        {
                ui->groupProps->setVisible(true);
                ui->groupProps->setTitle(QString::fromUtf8("Propriétés #%1").arg(mapCache[current]));
        }
}

void FormDali::on_buttonOn_clicked()
{
        if (mapCache.contains(ui->listLights->currentItem()))
        {
                WagoConnect::Instance().SendCommand(QString("WAGO_DALI_SET 1 0 %1 100 1").arg(mapCache[ui->listLights->currentItem()]));
        }
}

void FormDali::on_buttonOff_clicked()
{
        if (mapCache.contains(ui->listLights->currentItem()))
        {
                WagoConnect::Instance().SendCommand(QString("WAGO_DALI_SET 1 0 %1 0 1").arg(mapCache[ui->listLights->currentItem()]));
        }
}

void FormDali::on_buttonBlink_clicked()
{
        if (mapCache.contains(ui->listLights->currentItem()))
        {
                WagoConnect::Instance().SendCommand(QString("WAGO_DALI_BLINK 1 %1 0 20").arg(mapCache[ui->listLights->currentItem()]));
        }
}

void FormDali::on_buttonBlinkStop_clicked()
{
        if (mapCache.contains(ui->listLights->currentItem()))
        {
                WagoConnect::Instance().SendCommand(QString("WAGO_DALI_BLINK_STOP 1"));
        }
}

void FormDali::on_buttonAdvanced_clicked()
{
        if (mapCache.contains(ui->listLights->currentItem()))
        {
                DisableControls();
                WagoConnect::Instance().SendCommand(QString("WAGO_DALI_GET_DEVICE_INFO 1 %1").arg(mapCache[ui->listLights->currentItem()]), this, "advancedInfo_cb");
        }
}

void FormDali::advancedInfo_cb(QString cmd, QString res)
{
        QStringList list = res.split(" ");

        EnableControls();

        if (list.size() < 7)
        {
                cout << "FormDali::advancedInfo_cb: error in answer !" << endl;
                return;
        }

        DialogDaliDeviceConfig d;

        d.setFadeRate(list[1].toInt());
        d.setFadeTime(list[2].toInt());
        d.setMaxValue(list[3].toInt());
        d.setMinValue(list[4].toInt());
        d.setSystemFailureLevel(list[5].toInt());
        d.setPowerOnLevel(list[6].toInt());

        if (d.exec() == QDialog::Accepted)
        {
                WagoConnect::Instance().SendCommand(
                                QString("WAGO_DALI_SET_DEVICE_INFO 1 %1 %2 %3 %4 %5 %6 %7")
                                        .arg(mapCache[ui->listLights->currentItem()])
                                        .arg(d.getFadeRate())
                                        .arg(d.getFadeTime())
                                        .arg(d.getMaxValue())
                                        .arg(d.getMinValue())
                                        .arg(d.getSystemFailureLevel())
                                        .arg(d.getPowerOnLevel())
                );
        }
}

void FormDali::on_sliderLight_sliderReleased()
{
        if (mapCache.contains(ui->listLights->currentItem()))
        {
                WagoConnect::Instance().SendCommand(QString("WAGO_DALI_SET 1 0 %1 %2 1").arg(mapCache[ui->listLights->currentItem()]).arg(ui->sliderLight->value()));
        }
}

void FormDali::on_comboGroups_currentIndexChanged(int index)
{
        ui->listGroups->clear();

        if (index == 0) return;

        if (cacheGroups.size() != 64)
        {
                currentGroup = index;
                LoadGroup();
        }
        else
        {
                ShowGroup(index);
        }
}

void FormDali::ShowGroup(int grp)
{
        for (int i = 0;i < cacheGroups.size();i++)
        {
                QString s = cacheGroups[i];

                if (s.length() != 16 && !s.isEmpty())
                        std::cout << "FormDali::ShowGroup(): Error, length != 16 !" << std::endl;

                if (s.size() == 16 && grp > 0 && grp <= 16 && s[grp - 1] == '1')
                {
                        QListWidgetItem *item = new QListWidgetItem(QIcon(":/img/dali.png"), QString::fromUtf8("Lumière #%1").arg(i + 1), ui->listGroups);
                        item->setData(Qt::UserRole, i + 1);
                }
        }
}

void FormDali::LoadGroup()
{
        if (mapCache.size() <= 0) return;

        if (!spinner)
        {
                DisableControls();
                cacheGroups.clear();
                ui->listGroups->clear();
                for (int i = 0;i < 64;i++) cacheGroups.push_back("");

                mapIter = mapCache.constBegin();
        }

        WagoConnect::Instance().SendCommand(QString("WAGO_DALI_GET_DEVICE_GROUP 1 %1").arg(mapIter.value()), this, "LoadGroups_cb");
}

void FormDali::LoadGroups_cb(QString cmd, QString res)
{
        QStringList sl = res.split(" ");

        if (sl.size() != 3)
        {
                std::cout << "LoadGroups_cb: command response error: " << res.toUtf8().data() << std::endl;

                return;
        }

        cacheGroups[ mapIter.value() - 1 ] = sl[2];
        mapIter++;

        if (mapIter == mapCache.constEnd())
        {
                ShowGroup(currentGroup);
                EnableControls();
        }
        else
        {
                LoadGroup();
        }
}

void FormDali::on_listLights_customContextMenuRequested(QPoint point)
{
        QListWidgetItem *item = ui->listLights->itemAt(point);

        if (item && item->flags() != Qt::NoItemFlags)
        {
                QMenu *menu = new QMenu(this);
                QAction *action = NULL;

                if (ui->comboGroups->currentIndex() != 0)
                {
                        action = menu->addAction(QString::fromUtf8("Ajouter au groupe #%1").arg(ui->comboGroups->currentIndex()));
                        action->setIcon(QIcon(":/img/go-last.png"));
                        connect(action, SIGNAL(triggered()), this, SLOT(addToGroup()));

                        menu->addSeparator();
                }

                action = menu->addAction(QString::fromUtf8("Allumer").arg(ui->comboGroups->currentIndex()));
                action->setIcon(QIcon(":/img/icon_light_on.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(on_buttonOn_clicked()));

                action = menu->addAction(QString::fromUtf8("Eteindre").arg(ui->comboGroups->currentIndex()));
                action->setIcon(QIcon(":/img/icon_light_off.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(on_buttonOff_clicked()));

                menu->addSeparator();

                action = menu->addAction(QString::fromUtf8("Supprimer l'adresse").arg(ui->comboGroups->currentIndex()));
                action->setIcon(QIcon(":/img/user-trash.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(deleteAddress()));

                QMenu *submenu = menu->addMenu(QString::fromUtf8("Changer l'addresse"));

                QSignalMapper *sig = new QSignalMapper(this);

                for (int i = 1;i <= 64;i++)
                {
                        action = submenu->addAction(QString::fromUtf8("Adresse #%1").arg(i));
                        if (itemsCache[i - 1]) action->setEnabled(false);
                        sig->setMapping(action, i);
                        connect(action, SIGNAL(triggered()), sig, SLOT(map()));
                }

                connect(sig, SIGNAL(mapped(int)), this, SLOT(changeAddress(int)));

                menu->addSeparator();

                action = menu->addAction(QString::fromUtf8("Propriétés..."));
                action->setIcon(QIcon(":/img/edit-rename.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(on_buttonAdvanced_clicked()));

                menu->exec(QCursor::pos());
        }
}

void FormDali::addToGroup()
{
        int addr = mapCache[ui->listLights->currentItem()];

        WagoConnect::Instance().SendCommand(QString("WAGO_DALI_DEVICE_ADD_GROUP 1 %1 %2").arg(addr).arg(ui->comboGroups->currentIndex()),
                                            this, "modifyGroup_cb");
}

void FormDali::delFromGroup()
{
        int addr = ui->listGroups->currentItem()->data(Qt::UserRole).toInt();

        WagoConnect::Instance().SendCommand(QString("WAGO_DALI_DEVICE_DEL_GROUP 1 %1 %2").arg(addr).arg(ui->comboGroups->currentIndex()),
                                            this, "modifyGroup_cb");
}

void FormDali::modifyGroup_cb(QString cmd, QString res)
{
        LoadGroup();
}

void FormDali::deleteAddress()
{
        int old = mapCache[ui->listLights->currentItem()];

        WagoConnect::Instance().SendCommand(QString("WAGO_DALI_ADDRESSING_DEL 1 %1").arg(old));

        DisableControls();

        timer_addressing = new QTimer(this);
        connect(timer_addressing, SIGNAL(timeout()), this, SLOT(addressingInProgress_cb()));
        timer_addressing->start(500);
}

void FormDali::changeAddress(int new_address)
{
        int old = mapCache[ui->listLights->currentItem()];

        WagoConnect::Instance().SendCommand(QString("WAGO_DALI_ADDRESSING_CHANGE 1 %1 %2").arg(old).arg(new_address));

        DisableControls();

        timer_addressing = new QTimer(this);
        connect(timer_addressing, SIGNAL(timeout()), this, SLOT(addressingInProgress_cb()));
        timer_addressing->start(500);
}

void FormDali::on_listGroups_customContextMenuRequested(QPoint point)
{
        QListWidgetItem *item = ui->listGroups->itemAt(point);

        if (item)
        {
                QMenu *menu = new QMenu(this);
                QAction *action = NULL;

                action = menu->addAction(QString::fromUtf8("Supprimer du groupe #%1").arg(ui->comboGroups->currentIndex()));
                action->setIcon(QIcon(":/img/user-trash.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(delFromGroup()));

                menu->exec(QCursor::pos());
        }
}

void FormDali::on_buttonGrpOn_clicked()
{
        if (ui->comboGroups->currentIndex() > 0)
                WagoConnect::Instance().SendCommand(QString("WAGO_DALI_SET 1 1 %1 100 1").arg(ui->comboGroups->currentIndex()));
}

void FormDali::on_buttonGrpOff_clicked()
{
        if (ui->comboGroups->currentIndex() > 0)
                WagoConnect::Instance().SendCommand(QString("WAGO_DALI_SET 1 1 %1 0 1").arg(ui->comboGroups->currentIndex()));
}

void FormDali::on_buttonGrpBlink_clicked()
{
        if (ui->comboGroups->currentIndex() > 0)
                WagoConnect::Instance().SendCommand(QString("WAGO_DALI_BLINK 1 %1 1 20").arg(ui->comboGroups->currentIndex()));
}

void FormDali::on_buttonGrpBlinkStop_clicked()
{
        WagoConnect::Instance().SendCommand(QString("WAGO_DALI_BLINK_STOP 1"));
}
