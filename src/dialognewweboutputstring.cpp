#include "dialognewweboutputstring.h"
#include "ui_DialogNewWebOutputString.h"

DialogNewWebOutputString::DialogNewWebOutputString(Room *r, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogNewWebOutputString), room(r)
{
        ui->setupUi(this);
        ui->label_error_name_empty->hide();
        ui->label_error_url_empty->hide();
}

DialogNewWebOutputString::~DialogNewWebOutputString()
{
        delete ui;
}

void DialogNewWebOutputString::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_name_empty->show();
                return;
        }
        else if (ui->edit_url->text().isEmpty())
        {
                ui->label_error_url_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toUtf8().constData());
        p.Add("type", "WebOutputString");
        p.Add("url", to_string(ui->edit_url->text().toUtf8().constData()));
        p.Add("data", to_string(ui->data_text->toPlainText().toUtf8().constData()));
        p.Add("data_type", to_string(ui->data_type->text().toUtf8().constData()));
        p.Add("request_type", to_string(ui->request_type->currentText().toUtf8().constData()));

        output = ListeRoom::Instance().createOutput(p, room);
        accept();
}
