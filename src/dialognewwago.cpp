#include "dialognewwago.h"
#include "ui_DialogNewWago.h"
#include "mainwindow.h"

#include "projectmanager.h"

DialogNewWago::DialogNewWago(int t, Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewWago), io(NULL), room(r), detect_in_progress(false),
    another(false)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    moreButton = new QPushButton(tr("Ok, next"), this);
    moreButton->setAutoDefault(true);
    ui->buttonBox->addButton(moreButton, QDialogButtonBox::ActionRole);

    //hide error labels by default.
    ui->label_error_empty->hide();

    if (t == ITEM_INPUT_SWITCH)
    {
        ui->pager->setCurrentIndex(1);
        type = "WIDigital";
    }
    else if (t == ITEM_LIGHT)
    {
        ui->pager->setCurrentIndex(0);
        type = "WODigital";
        ui->comboBox->hide();
    }
}

DialogNewWago::~DialogNewWago()
{
    delete ui;
}

void DialogNewWago::changeEvent(QEvent *e)
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

void DialogNewWago::on_buttonBox_accepted()
{
    if (ui->edit_name->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }

    if (type == "WIDigital")
    {
        if (ui->comboBox->currentIndex() == 1)
            type = "WIDigitalTriple";
        else if (ui->comboBox->currentIndex() == 2)
            type = "WIDigitalLong";
        else
            type = "WIDigitalBP";
    }

    Params p;
    p.Add("name", ui->edit_name->text().toUtf8().data());
    p.Add("type", type);
    p.Add("var", to_string(ui->spin_var->value()));

    if (ui->checkKNX->isChecked())
        p.Add("knx", "true");

    if (type == "WODigital")
    {
        p.Add("gtype", "light");
        io = ListeRoom::Instance().createOutput(p, room);
    }
    else
    {
        io = ListeRoom::Instance().createInput(p, room);
    }

    //Check if we need 841 or 842 here. By default it's 841 now.
    if (ProjectManager::wagoTypeCache.find(io->get_param("host")) == ProjectManager::wagoTypeCache.end())
    {
        ProjectManager::wagoTypeCache[io->get_param("host")] = true;
        io->set_param("wago_841", "true");
    }
    else
    {
        if (ProjectManager::wagoTypeCache[io->get_param("host")])
            io->set_param("wago_841", "true");
        else
            io->set_param("wago_841", "false");
    }

    accept();
}

void DialogNewWago::on_button_detect_clicked()
{
    //Start an udp socket to listen for wago inputs
    if (!detect_in_progress)
    {
        if (WagoConnect::Instance().getConnectionStatus() != WAGO_CONNECTED)
        {
            WagoConnect::Instance().emitErrorNotConnectedSignal();

            return;
        }

        detect_in_progress = true;

        ui->button_detect->setText(tr("Stop..."));
        ui->edit_name->setEnabled(false);
        ui->spin_var->setEnabled(false);
        ui->checkKNX->setEnabled(false);
        ui->comboBox->setEnabled(false);
        ui->buttonBox->setEnabled(false);

        spinner = new QAnimationLabel(":/img/loader.gif", this);
        ui->spinnerLayout->addWidget(spinner, 0, Qt::AlignCenter);
        spinner->start();

        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        connect(&WagoConnect::Instance(), &WagoConnect::responseReceived, this, &DialogNewWago::processUDPRequest);
    }
    else
    {
        detect_in_progress = false;

        ui->button_detect->setText(tr("Detection"));
        ui->edit_name->setEnabled(true);
        ui->spin_var->setEnabled(true);
        ui->checkKNX->setEnabled(true);
        ui->comboBox->setEnabled(true);
        ui->buttonBox->setEnabled(true);
        delete spinner;

        QApplication::restoreOverrideCursor();

        disconnect(&WagoConnect::Instance(), &WagoConnect::responseReceived, this, &DialogNewWago::processUDPRequest);
    }
}

void DialogNewWago::processUDPRequest(QString &, QString response)
{
    string req = response.toUtf8().data();

    vector<string> tok;
    Utils::split(req, tok, " ", 4);

    if (tok[0] == "WAGO" && (tok[1] == "INT" || tok[1] == "KNX"))
    {
        int var;
        Utils::from_string(tok[2], var);

        if (tok[1] == "KNX")
            ui->checkKNX->setChecked(true);
        else
            ui->checkKNX->setChecked(false);

        ui->spin_var->setValue(var);

        on_button_detect_clicked();
    }
}

void DialogNewWago::setWagoOutput(bool enable)
{
    //send udp datagram to enable output
    QString cmd;

    if (ui->checkKNX->isChecked())
        cmd = "WAGO_SET_KNX_OUTPUT " + QString::number(ui->spin_var->value());
    else
        cmd = "WAGO_SET_OUTPUT " + QString::number(ui->spin_var->value());

    if (enable)
        cmd += " 1";
    else
        cmd += " 0";

    WagoConnect::Instance().SendCommand(cmd);
}

void DialogNewWago::on_button_test_enable_clicked()
{
    setWagoOutput(true);
}

void DialogNewWago::on_button_test_disable_clicked()
{
    setWagoOutput(false);
}

void DialogNewWago::on_buttonBox_clicked(QAbstractButton* button)
{
    if (button == moreButton)
    {
        another = true;
        on_buttonBox_accepted();
    }
}


void DialogNewWago::on_checkKNX_stateChanged(int state)
{
    if (ui->check841->isChecked() && state == Qt::Checked)
        ui->check841->setChecked(false);
}

void DialogNewWago::on_check841_stateChanged(int state)
{
    if (ui->checkKNX->isChecked() && state == Qt::Checked)
        ui->checkKNX->setChecked(false);
}
