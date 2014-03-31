#include "dialognewinternal.h"
#include "ui_DialogNewInternal.h"
#include "mainwindow.h"

DialogNewInternal::DialogNewInternal(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewInternal), output(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();
        ui->value_int->hide();
        ui->value_text->hide();
}

DialogNewInternal::~DialogNewInternal()
{
        delete ui;
}

void DialogNewInternal::changeEvent(QEvent *e)
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

void DialogNewInternal::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toUtf8().constData());

        if (ui->radioButton_bool->isChecked())
        {
                p.Add("type", "InternalBool");
                if (ui->value_bool->isChecked())
                        p.Add("value", "true");
                else
                        p.Add("value", "false");
        }
        else if (ui->radioButton_int->isChecked())
        {
                p.Add("type", "InternalInt");
                p.Add("value", to_string(ui->value_int->value()));
        }
        else if (ui->radioButton_text->isChecked())
        {
                p.Add("type", "InternalString");
                p.Add("value", ui->value_text->text().toUtf8().constData());
        }

        if (ui->checkBox_rw->isChecked())
                p.Add("rw", "true");
        else
                p.Add("rw", "false");

        if (ui->checkBox_save->isChecked())
                p.Add("save", "true");
        else
                p.Add("save", "false");

        if (ui->checkBox_visible->isChecked())
                p.Add("visible", "true");
        else
                p.Add("visible", "false");

        Input *in = ListeRoom::Instance().createInput(p, room);
        output = dynamic_cast<Output *>(in);

        accept();
}

void DialogNewInternal::on_radioButton_bool_clicked()
{
        if (ui->radioButton_bool->isChecked())
        {
                ui->value_bool->show();
                ui->value_int->hide();
                ui->value_text->hide();
        }
        else if (ui->radioButton_int->isChecked())
        {
                ui->value_bool->hide();
                ui->value_int->show();
                ui->value_text->hide();
        }
        else if (ui->radioButton_text->isChecked())
        {
                ui->value_bool->hide();
                ui->value_int->hide();
                ui->value_text->show();
        }
}

void DialogNewInternal::on_radioButton_int_clicked()
{
        on_radioButton_bool_clicked();
}

void DialogNewInternal::on_radioButton_text_clicked()
{
        on_radioButton_bool_clicked();
}
