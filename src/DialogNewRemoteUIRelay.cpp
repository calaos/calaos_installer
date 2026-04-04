#include "DialogNewRemoteUIRelay.h"
#include "ui_DialogNewRemoteUIRelay.h"

#include <QPushButton>

DialogNewRemoteUIRelay::DialogNewRemoteUIRelay(Room *r, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogNewRemoteUIRelay)
    , room(r)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->labelError->hide();

    ui->comboBoxStyle->addItem(tr("Light"), QString("light"));
    ui->comboBoxStyle->addItem(tr("Boiler"), QString("boiler"));
    ui->comboBoxStyle->addItem(tr("Heater"), QString("heater"));
    ui->comboBoxStyle->addItem(tr("Outlet"), QString("outlet"));
    ui->comboBoxStyle->addItem(tr("Pump"), QString("pump"));

    populateRemoteUICombo();
}

DialogNewRemoteUIRelay::~DialogNewRemoteUIRelay()
{
    delete ui;
}

void DialogNewRemoteUIRelay::populateRemoteUICombo()
{
    for (int i = 0; i < ListeRoom::Instance().size(); i++)
    {
        Room *r = ListeRoom::Instance().get_room(i);
        for (int j = 0; j < r->get_size_out(); j++)
        {
            IOBase *io = r->get_output(j);
            if (io->get_gui_type() == "remote_ui")
            {
                QString label = QString("%1 (%2)")
                    .arg(QString::fromUtf8(io->get_param("name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("id").c_str()));
                ui->comboBoxRemoteUI->addItem(label,
                    QString::fromUtf8(io->get_param("id").c_str()));
            }
        }
    }

    if (ui->comboBoxRemoteUI->count() == 0)
    {
        ui->labelError->setText(tr("No Remote UI device found. Add one first."));
        ui->labelError->show();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

void DialogNewRemoteUIRelay::on_buttonBox_accepted()
{
    if (ui->lineEditName->text().isEmpty())
    {
        ui->labelError->setText(tr("Name cannot be empty"));
        ui->labelError->show();
        return;
    }

    if (ui->comboBoxRemoteUI->count() == 0)
    {
        ui->labelError->setText(tr("No Remote UI device available"));
        ui->labelError->show();
        return;
    }

    Params p;
    p.Add("name", ui->lineEditName->text().toUtf8().constData());
    p.Add("type", "RemoteUIOutputRelay");
    p.Add("remote_ui_id",
          ui->comboBoxRemoteUI->itemData(ui->comboBoxRemoteUI->currentIndex())
              .toString().toUtf8().constData());
    p.Add("relay_num",
          QString::number(ui->spinBoxRelayNum->value()).toUtf8().constData());
    p.Add("io_style",
          ui->comboBoxStyle->itemData(ui->comboBoxStyle->currentIndex())
              .toString().toUtf8().constData());
    p.Add("visible", "true");

    output = ListeRoom::Instance().createOutput(p, room);

    accept();
}
