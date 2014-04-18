#include "dialognewwebioshutter.h"
#include "ui_DialogNewWebIoShutter.h"

DialogNewWebIOShutter::DialogNewWebIOShutter(Room *r, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogNewWebIOShutter), io(NULL), room(r)
{
        ui->setupUi(this);
}

DialogNewWebIOShutter::~DialogNewWebIOShutter()
{
        delete ui;
}


void DialogNewWebIOShutter::on_buttonBox_accepted()
{
#if 0
    if (ui->edit_name->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }
    else if (ui->label_error_path_empty->text().isEmpty())
    {
        ui->label_error_path_empty->show();
        return;
    }
    else if (ui->label_error_url_empty->text().isEmpty())
    {
        ui->label_error_url_empty->show();
        return;
    }

    Params p;
    p.Add("name", ui->edit_name->text().toUtf8().constData());
    p.Add("type", "test");
    p.Add("url", to_string(ui->edit_url->text().toUtf8().constData()));
    p.Add("path", to_string(ui->edit_path->text().toUtf8().constData()));
    p.Add("file_type", to_string(ui->data_type->currentText().toLower().toUtf8().constData()));

    io = ListeRoom::Instance().createInput(p, room);
#endif
    accept();
}
