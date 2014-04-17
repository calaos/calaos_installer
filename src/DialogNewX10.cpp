#include "DialogNewX10.h"
#include "ui_DialogNewX10.h"

DialogNewX10::DialogNewX10(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewX10),
    room(r)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ui->label_error_empty->hide();
}

DialogNewX10::~DialogNewX10()
{
    delete ui;
}

void DialogNewX10::on_buttonBox_accepted()
{
    if (ui->edit_name->text().isEmpty() ||
        ui->edit_code->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;

    p.Add("name", ui->edit_name->text().toUtf8().constData());
    p.Add("type", "X10Output");
    p.Add("code", ui->edit_code->text().toUtf8().constData());

    io = ListeRoom::Instance().createOutput(p, room);

    accept();
}
