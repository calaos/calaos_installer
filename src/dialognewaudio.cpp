#include "dialognewaudio.h"
#include "ui_dialognewaudio.h"
#include "mainwindow.h"

DialogNewAudio::DialogNewAudio(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewAudio), output(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();
}

DialogNewAudio::~DialogNewAudio()
{
        delete ui;
}

void DialogNewAudio::changeEvent(QEvent *e)
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

void DialogNewAudio::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toLocal8Bit().constData());
        p.Add("host", ui->edit_ip->text().toLocal8Bit().constData());
        p.Add("id", ui->edit_mac->text().toLocal8Bit().constData());
        p.Add("type", "slim");

        Audio *audio = ListeRoom::Instance().createAudio(p, room);
        output = dynamic_cast<Output *>(audio);

        accept();
}
