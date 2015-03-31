#include "DialogOla.h"
#include "ui_DialogOla.h"
#include "formrules.h"

DialogOla::DialogOla(Room *r, int it, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOla),
    room(r),
    itemType(it)
{
    ui->setupUi(this);

    ui->label_error_empty->hide();

    switch(itemType)
    {
    case ITEM_DALI:
        setWindowTitle(tr("Add a Light dimmer from OLA"));
        ui->label_chan_red->setText(tr("Channel"));
        ui->label_chan_green->hide();
        ui->label_chan_blue->hide();
        ui->lineEditChannelGreen->hide();
        ui->lineEditChannelBlue->hide();
        ui->lineEditChannelGreen->setText("0");
        ui->lineEditChannelBlue->setText("0");
        break;
    case ITEM_LIGHT_RGB:
        setWindowTitle(tr("Add a RGB Light from OLA"));
        ui->label_chan_red->setText(tr("Channel (red)"));
        ui->label_chan_green->show();
        ui->label_chan_blue->show();
        ui->lineEditChannelGreen->show();
        ui->lineEditChannelBlue->show();
        break;
    default: break;
    }
}

DialogOla::~DialogOla()
{
    delete ui;
}

void DialogOla::on_buttonBox_accepted()
{
    if (ui->lineEditName->text().isEmpty() ||
        ui->lineEditUniverse->text().isEmpty() ||
        ui->lineEditChannelRed->text().isEmpty() ||
        ui->lineEditChannelGreen->text().isEmpty() ||
        ui->lineEditChannelBlue->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;
    p.Add("name", ui->lineEditName->text().toUtf8().constData());
    p.Add("universe", ui->lineEditUniverse->text().toUtf8().constData());

    switch(itemType)
    {
    case ITEM_DALI:
    {
        p.Add("type", "OLAOutputLightDimmer");
        p.Add("channel", ui->lineEditChannelRed->text().toUtf8().constData());
        io = ListeRoom::Instance().createOutput(p, room);
        break;
    }
    case ITEM_LIGHT_RGB:
    {
        p.Add("type", "OLAOutputLightRGB");
        p.Add("channel_red", ui->lineEditChannelRed->text().toUtf8().constData());
        p.Add("channel_green", ui->lineEditChannelGreen->text().toUtf8().constData());
        p.Add("channel_blue", ui->lineEditChannelBlue->text().toUtf8().constData());
        io = ListeRoom::Instance().createOutput(p, room);
        break;
    }
    default: break;
    }

    accept();
}
