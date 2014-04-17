#include "dialognewrule.h"
#include "ui_DialogNewRule.h"

DialogNewRule::DialogNewRule(string name, string type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewRule),
    rule(NULL)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ui->edit_name->setText(QString::fromUtf8(name.c_str()));
    ui->edit_type->setText(QString::fromUtf8(type.c_str()));

    //hide error labels by default.
    ui->label_error_exists->hide();
    ui->label_error_empty->hide();
}

DialogNewRule::~DialogNewRule()
{
    delete ui;
}

void DialogNewRule::changeEvent(QEvent *e)
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

void DialogNewRule::on_buttonBox_accepted()
{
    if (ui->edit_name->text().isEmpty() || ui->edit_type->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    string name, type;

    name = ui->edit_name->text().toUtf8().data();
    type = ui->edit_type->text().toUtf8().data();

    if (ListeRule::Instance().searchRule(type, name) != NULL)
    {
        ui->label_error_empty->hide();
        ui->label_error_exists->show();
        return;
    }

    rule = new Rule(type, name);
    ListeRule::Instance().Add(rule);

    accept();
}
