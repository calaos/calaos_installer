#include "DialogNewWOL.h"
#include "ui_DialogNewWOL.h"

DialogNewWOL::DialogNewWOL(Room *r, int it, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewWOL),
    room(r),
    itemType(it)
{
    ui->setupUi(this);
    ui->label_error_empty->hide();
}

DialogNewWOL::~DialogNewWOL()
{
    delete ui;
}

void DialogNewWOL::on_buttonBox_accepted()
{
    if (ui->lineEditName->text().isEmpty() ||
        ui->lineEditHost->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;
    p.Add("type", "WOLOutputBool");
    p.Add("name", ui->lineEditName->text().toUtf8().constData());
    p.Add("address", ui->lineEditHost->text().toUtf8().constData());
    io = ListeRoom::Instance().createOutput(p, room);

    accept();
}
