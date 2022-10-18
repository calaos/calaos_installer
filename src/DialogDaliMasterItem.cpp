#include "DialogDaliMasterItem.h"
#include "ui_DialogDaliMasterItem.h"

DialogDaliMasterItem::DialogDaliMasterItem(QTreeWidget *w, QTreeWidgetItem *item, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDaliMasterItem),
    editItem(item),
    treeWidget(w)
{
    ui->setupUi(this);

    ui->comboBox->addItem(tr("Dali Ballast"), "DaliEcg");
    ui->comboBox->addItem(tr("Dali Ballast Group"), "DaliGroup");
    ui->comboBox->addItem(tr("Multi Sensor Type 1"), "MultiSensorType1");
    ui->comboBox->addItem(tr("Push Button Sensor Type 1"), "PushbuttonSensorType1");
    ui->comboBox->addItem(tr("Multi Sensor Type 2"), "MultiSensorType2");
    ui->comboBox->addItem(tr("Push Button Sensor Type 2"), "PushbuttonSensorType2");

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(ui->comboBox, &QComboBox::currentIndexChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
#else
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
#endif

    ui->stackedWidget->setCurrentIndex(0);

    if (editItem)
    {
        QString t = editItem->data(0, RoleType).toString();
        if (t == "DaliEcg")
        {
            ui->comboBox->setCurrentIndex(0);
            ui->spinBox1_addr->setValue(editItem->data(0, RoleAddr1).toInt());
        }
        else if (t == "DaliGroup")
        {
            ui->comboBox->setCurrentIndex(1);
            ui->spinBox2_addr->setValue(editItem->data(0, RoleAddr1).toInt());
        }
        else if (t == "MultiSensorType1")
        {
            ui->comboBox->setCurrentIndex(2);
            ui->spinBox3_addrPresence->setValue(editItem->data(0, RoleAddr1).toInt());
            ui->spinBox3_addrLight->setValue(editItem->data(0, RoleAddr2).toInt());
            ui->spinBox3_timeout->setValue(editItem->data(0, RoleAddr3).toInt());
        }
        else if (t == "PushbuttonSensorType1")
        {
            ui->comboBox->setCurrentIndex(3);
            ui->spinBox4_addr1->setValue(editItem->data(0, RoleAddr1).toInt());
            ui->spinBox4_addr2->setValue(editItem->data(0, RoleAddr2).toInt());
            ui->spinBox4_addr3->setValue(editItem->data(0, RoleAddr3).toInt());
            ui->spinBox4_addr4->setValue(editItem->data(0, RoleAddr4).toInt());
        }
        else if (t == "MultiSensorType2")
        {
            ui->comboBox->setCurrentIndex(4);
            ui->spinBox5_addrPresence->setValue(editItem->data(0, RoleAddr1).toInt());
            ui->spinBox5_addrLight->setValue(editItem->data(0, RoleAddr2).toInt());
            ui->spinBox5_addrIR->setValue(editItem->data(0, RoleAddr3).toInt());
            ui->spinBox5_timeout->setValue(editItem->data(0, RoleAddr4).toInt());
        }
        else if (t == "PushbuttonSensorType2")
        {
            ui->comboBox->setCurrentIndex(5);
            ui->spinBox6_addr->setValue(editItem->data(0, RoleAddr1).toInt());
        }
    }
}

DialogDaliMasterItem::~DialogDaliMasterItem()
{
    delete ui;
}

void DialogDaliMasterItem::on_buttonBox_accepted()
{
    if (!editItem)
        editItem = new QTreeWidgetItem(treeWidget);

    QString t = ui->comboBox->currentData().toString();

    if (ui->comboBox->currentIndex() == 0)
        updateItem(editItem, t, ui->spinBox1_addr->value());
    else if (ui->comboBox->currentIndex() == 1)
        updateItem(editItem, t, ui->spinBox2_addr->value());
    else if (ui->comboBox->currentIndex() == 2)
        updateItem(editItem, t, ui->spinBox3_addrPresence->value(), ui->spinBox3_addrLight->value(), ui->spinBox3_timeout->value());
    else if (ui->comboBox->currentIndex() == 3)
        updateItem(editItem, t, ui->spinBox4_addr1->value(), ui->spinBox4_addr2->value(), ui->spinBox4_addr3->value(), ui->spinBox4_addr4->value());
    else if (ui->comboBox->currentIndex() == 4)
        updateItem(editItem, t, ui->spinBox5_addrPresence->value(), ui->spinBox5_addrLight->value(), ui->spinBox5_addrIR->value(), ui->spinBox5_timeout->value());
    else if (ui->comboBox->currentIndex() == 5)
        updateItem(editItem, t, ui->spinBox6_addr->value());

    accept();
}

void DialogDaliMasterItem::updateItem(QTreeWidgetItem *item, QString type, int addr1, int addr2, int addr3, int addr4)
{
    item->setData(0, RoleType, type);
    item->setData(0, RoleAddr1, addr1);
    item->setData(0, RoleAddr2, addr2);
    item->setData(0, RoleAddr3, addr3);
    item->setData(0, RoleAddr4, addr4);

    if (type == "DaliEcg")
    {
        item->setText(0, tr("Dali Ballast"));
        item->setText(1, tr("Address: %1").arg(addr1));
        item->setText(2, QString());
        item->setIcon(0, QIcon(":/img/icon_light_on.png"));
    }
    else if (type == "DaliGroup")
    {
        item->setText(0, tr("Dali Ballast Group"));
        item->setText(1, tr("Group address: %1").arg(addr1));
        item->setText(2, QString());
        item->setIcon(0, QIcon(":/img/icon_light_on.png"));
    }
    else if (type == "MultiSensorType1")
    {
        item->setText(0, tr("Multi Sensor Type 1"));
        item->setText(1, tr("Presence_address: %1 Light_address: %2").arg(addr1).arg(addr2));
        item->setText(2, tr("%1 ms").arg(addr3));
        item->setIcon(0, QIcon(":/img/icon_inter.png"));
    }
    else if (type == "PushbuttonSensorType1")
    {
        item->setText(0, tr("Push Button Sensor Type 1"));
        item->setText(1, tr("Button1: %1 Button2: %2 Button3: %3 Button4: %4")
                      .arg(addr1).arg(addr2)
                      .arg(addr3).arg(addr4));
        item->setText(2, QString());
        item->setIcon(0, QIcon(":/img/icon_inter.png"));
    }
    else if (type == "MultiSensorType2")
    {
        item->setText(0, tr("Multi Sensor Type 2"));
        item->setText(1, tr("Presence_address: %1 Light_address: %2 IR_address: %3").arg(addr1).arg(addr2).arg(addr3));
        item->setText(2, tr("%1 ms").arg(addr4));
        item->setIcon(0, QIcon(":/img/icon_inter.png"));
    }
    else if (type == "PushbuttonSensorType2")
    {
        item->setText(0, tr("Push Button Sensor Type 2"));
        item->setText(1, tr("Buttons_address: %1").arg(addr1));
        item->setText(2, QString());
        item->setIcon(0, QIcon(":/img/icon_inter.png"));
    }
}
