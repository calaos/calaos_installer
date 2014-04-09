#include "dialognewtemp.h"
#include "ui_DialogNewTemp.h"
#include "mainwindow.h"

DialogNewTemp::DialogNewTemp(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewTemp), temperature(NULL), room(r)
{
    ui->setupUi(this);

    //hide error labels by default.
    ui->label_error_empty->hide();
}

DialogNewTemp::~DialogNewTemp()
{
    delete ui;
}

void DialogNewTemp::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogNewTemp::on_buttonBox_accepted()
{
    if (ui->edit_name->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;
    p.Add("name", ui->edit_name->text().toUtf8().constData());
    p.Add("type", "WITemp");
    p.Add("var", to_string(ui->spin_addr->value()));

    temperature = ListeRoom::Instance().createInput(p, room);

    accept();
}
