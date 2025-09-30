#include "DialogRemoteUI.h"
#include "ui_DialogRemoteUI.h"

DialogRemoteUI::DialogRemoteUI(Room *r, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogRemoteUI),
    room(r)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->labelError->hide();

    ui->comboBoxType->addItem("Waveshare ESP32-P4-86-Panel", QString("waveshare-esp32-p4-86-panel"));
    ui->comboBoxType->addItem("Luckfox Luckfox-Pico-86-Panel", QString("luckfox-pico-86-panel"));
}

DialogRemoteUI::~DialogRemoteUI()
{
    delete ui;
}

void DialogRemoteUI::on_buttonBox_accepted()
{
    if (ui->lineEditName->text().isEmpty())
    {
        ui->labelError->setText("Name cannot be empty");
        ui->labelError->show();
        return;
    }

    if (ui->lineEditCode->text().isEmpty())
    {
        ui->labelError->setText("Code cannot be empty. Use the code written on the device screen");
        ui->labelError->show();
        return;
    }

    Params p;
    p.Add("name", ui->lineEditName->text().toUtf8().constData());
    p.Add("device_type", ui->comboBoxType->itemData(ui->comboBoxType->currentIndex()).toString().toUtf8().constData());
    p.Add("provisioning_code", ui->lineEditCode->text().toUtf8().constData());
    p.Add("visible", "false");
    p.Add("type", "RemoteUI");

    output = ListeRoom::Instance().createOutput(p, room);

    accept();
}

