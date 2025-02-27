#include "dialognewaudio.h"
#include "ui_DialogNewAudio.h"
#include "mainwindow.h"
#include "dialogdetectsqueezebox.h"

DialogNewAudio::DialogNewAudio(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewAudio), output(NULL), room(r)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    //hide error labels by default.
    ui->label_error_empty->hide();

    ui->tabWidget->setCurrentIndex(0);
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
        ui->label_error_empty->setText(tr("Please enter a name for this item.."));
        ui->label_error_empty->show();
        return;
    }

    if (ui->tabWidget->currentWidget() == ui->tabRoon &&
        ui->edit_roon_zid->text().isEmpty())
    {
        ui->label_error_empty->setText(tr("Please enter valid Roon Zone ID"));
        ui->label_error_empty->show();
        return;
    }

    if (ui->tabWidget->currentWidget() == ui->tabSqueezebox &&
        ui->edit_mac->text().isEmpty())
    {
        ui->label_error_empty->setText(tr("Please enter valid Squeezebox ID"));
        ui->label_error_empty->show();
        return;
    }

    Params p;
    p.Add("name", ui->edit_name->text().toUtf8().constData());

    if (ui->tabWidget->currentWidget() == ui->tabSqueezebox)
    {
        p.Add("host", ui->edit_ip->text().toUtf8().constData());
        p.Add("id", ui->edit_mac->text().toUtf8().constData());
        if (!p.Exists("host")) p.Add("host", "192.168.1.10");
        if (!p.Exists("port")) p.Add("port", "9090");
        p.Add("type", "slim");
    }
    else if (ui->tabWidget->currentWidget() == ui->tabRoon)
    {
        p.Add("zone_id", ui->edit_roon_zid->text().toUtf8().constData());
        p.Add("id", ListeRoom::get_new_id("io_"));
        p.Add("type", "roon");
    }

    output = ListeRoom::Instance().createAudio(p, room);

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
