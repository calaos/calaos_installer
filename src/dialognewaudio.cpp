#include "dialognewaudio.h"
#include "ui_DialogNewAudio.h"
#include "mainwindow.h"
#include "dialogdetectsqueezebox.h"

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
        p.Add("name", ui->edit_name->text().toUtf8().constData());
        p.Add("host", ui->edit_ip->text().toUtf8().constData());
        p.Add("id", ui->edit_mac->text().toUtf8().constData());
        p.Add("type", "slim");

        Audio *audio = ListeRoom::Instance().createAudio(p, room);
        output = dynamic_cast<Output *>(audio);

        accept();
}

void DialogNewAudio::on_pushButton_clicked()
{
        DialogDetectSqueezebox d;
        if (d.exec() == QDialog::Accepted)
        {
                Squeezebox *sq = d.getSelectedDevice();

		if (!sq) 
			return;

                ui->edit_ip->setText(sq->server);
                ui->edit_mac->setText(sq->id);
                ui->edit_name->setText(sq->name);
        }
}
