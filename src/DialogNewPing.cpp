#include "DialogNewPing.h"
#include "ui_DialogNewPing.h"

DialogNewPing::DialogNewPing(Room *r, int it, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewPing),
    room(r),
    itemType(it)
{
    ui->setupUi(this);
    ui->label_error_empty->hide();
}

DialogNewPing::~DialogNewPing()
{
    delete ui;
}

void DialogNewPing::on_buttonBox_accepted()
{
    if (ui->lineEditName->text().isEmpty() ||
        ui->lineEditHost->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;
    p.Add("type", "PingInputSwitch");
    p.Add("name", ui->lineEditName->text().toUtf8().constData());
    p.Add("host", ui->lineEditHost->text().toUtf8().constData());
    p.Add("interval", QString("%1").arg(ui->spinBoxInterval->value()).toUtf8().constData());
    io = ListeRoom::Instance().createInput(p, room);

    accept();
}
