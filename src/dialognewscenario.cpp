#include "dialognewscenario.h"
#include "ui_DialogNewScenario.h"
#include "mainwindow.h"

DialogNewScenario::DialogNewScenario(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewScenario), output(NULL), room(r)
{
    ui->setupUi(this);

    //hide error labels by default.
    ui->label_error_empty->hide();
}

DialogNewScenario::~DialogNewScenario()
{
    delete ui;
}

void DialogNewScenario::changeEvent(QEvent *e)
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

void DialogNewScenario::on_buttonBox_accepted()
{
    if (ui->edit_name->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;
    p.Add("name", ui->edit_name->text().toUtf8().constData());
    p.Add("type", "scenario");

    if (ui->checkBox_visible->isChecked())
        p.Add("visible", "true");
    else
        p.Add("visible", "false");

    IOBase *in = ListeRoom::Instance().createInput(p, room);
    output = in;

    accept();
}
