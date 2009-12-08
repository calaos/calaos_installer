#include "dialognewcamera.h"
#include "ui_dialognewcamera.h"
#include "mainwindow.h"

DialogNewCamera::DialogNewCamera(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewCamera), output(NULL), room(r)
{
        ui->setupUi(this);

        //hide error labels by default.
        ui->label_error_empty->hide();
}

DialogNewCamera::~DialogNewCamera()
{
        delete ui;
}

void DialogNewCamera::changeEvent(QEvent *e)
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

void DialogNewCamera::on_buttonBox_accepted()
{
        if (ui->edit_name->text().isEmpty())
        {
                ui->label_error_empty->show();
                return;
        }

        Params p;
        p.Add("name", ui->edit_name->text().toLocal8Bit().constData());

        if (ui->tab_camera_model->currentIndex() == 0) //Axis
        {
                p.Add("type", "axis");
                p.Add("port", "80");
                p.Add("host", ui->axis_ip->text().toLocal8Bit().constData());
                p.Add("model", ui->axis_cam->text().toLocal8Bit().constData());
        }
        else if (ui->tab_camera_model->currentIndex() == 1) //Planet
        {
                p.Add("type", "planet");
                p.Add("port", "80");
                p.Add("host", ui->planet_ip->text().toLocal8Bit().constData());
                p.Add("model", ui->planet_model->currentItem()->text().toLocal8Bit().constData());
        }
        else if (ui->tab_camera_model->currentIndex() == 2) //Gadspot
        {
                p.Add("type", "gadspot");
                p.Add("port", "80");
                p.Add("host", ui->gadspot_ip->text().toLocal8Bit().constData());
        }
        else if (ui->tab_camera_model->currentIndex() == 3) //Standard
        {
                p.Add("type", "standard_mjpeg");
                p.Add("port", "80");
                p.Add("host", ui->std_ip->text().toLocal8Bit().constData());
                p.Add("url_jpeg", ui->std_url_jpeg->text().toLocal8Bit().constData());
                p.Add("url_mjpeg", ui->std_url_mjpeg->text().toLocal8Bit().constData());
                p.Add("url_mpeg", ui->std_url_mpeg4->text().toLocal8Bit().constData());
        }
        else
        {
                QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Type de caméra inconnu!"));

                return;
        }

        Camera *cam = ListeRoom::Instance().createCamera(p, room);
        output = dynamic_cast<Output *>(cam);

        accept();
}
