#include "DialogNewZibaseOutput.h"
#include "ui_DialogNewZibaseOutput.h"
#include "formrules.h"

DialogNewZibaseOutput::DialogNewZibaseOutput(Room *r, int it, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewZibaseOutput),
    room(r),
    item(it)
{

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ui->label_error_empty->hide();

    if (item == ITEM_LIGHT)
    {
        ui->comboBox->addItem(tr("DEFAULT_PROTOCOL"), QString("0"));
        ui->comboBox->addItem(tr("VISONIC433"), QString("1"));
        ui->comboBox->addItem(tr("VISONIC868"), QString("2"));
        ui->comboBox->addItem(tr("CHACON"), QString("3"));
        ui->comboBox->addItem(tr("DOMIA"), QString("4"));
        ui->comboBox->addItem(tr("RFX10"), QString("5"));
        ui->comboBox->addItem(tr("ZWAVE"), QString("6"));
        ui->comboBox->addItem(tr("RFSTS10"), QString("7"));
        ui->comboBox->addItem(tr("XDD433alrm"), QString("8"));
        ui->comboBox->addItem(tr("XDD868alrmn"), QString("9"));
        ui->comboBox->addItem(tr("XDD868shutter"), QString("10"));
        ui->comboBox->addItem(tr("XDD868pilot"), QString("11"));
        ui->comboBox->addItem(tr("XDD868boiler"), QString("12"));

    }
}

DialogNewZibaseOutput::~DialogNewZibaseOutput()
{
    delete ui;
}

void DialogNewZibaseOutput::on_buttonBox_accepted()
{
    if (ui->edit_name->text().isEmpty() ||
        ui->edit_host->text().isEmpty() ||
        ui->edit_id->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;

    p.Add("name", ui->edit_name->text().toUtf8().constData());
    p.Add("host", ui->edit_host->text().toUtf8().constData());
    p.Add("zibase_id", ui->edit_id->text().toUtf8().constData());
    p.Add("port", "17100");

    p.Add("type", "ZibaseDigitalOut");
    if (item == ITEM_LIGHT)
    {
        p.Add("protocol", ui->comboBox->itemData(ui->comboBox->currentIndex()).toString().toUtf8().constData());
        p.Add("nbburst", ui->nbburstLineEdit->text().toUtf8().constData());

    }
    else
        reject();

    io = ListeRoom::Instance().createOutput(p, room);

    accept();
}
