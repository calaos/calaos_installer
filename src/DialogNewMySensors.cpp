#include "DialogNewMySensors.h"
#include "ui_DialogNewMySensors.h"
#include "formrules.h"

DialogNewMySensors::DialogNewMySensors(Room *r, int it, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewMySensors),
    room(r),
    itemType(it),
    analogWidget(NULL)
{
    ui->setupUi(this);

    ui->label_error_empty->hide();
    ui->comboBoxSwitch->setVisible(itemType == ITEM_INPUT_SWITCH);
    on_check_smart_stateChanged(0);
    on_comboBoxType_currentIndexChanged(0);

    switch(itemType)
    {
    case ITEM_ANALOG_IN: setWindowTitle(tr("Add a MySensors Analog in")); break;
    case ITEM_ANALOG_OUT: setWindowTitle(tr("Add a MySensors Analog out")); break;
    case ITEM_STRINGIN: setWindowTitle(tr("Add a MySensors String input")); break;
    case ITEM_STRINGOUT: setWindowTitle(tr("Add a MySensors String output")); break;
    case ITEM_INPUT_SWITCH: setWindowTitle(tr("Add a MySensors Switch")); break;
    case ITEM_TEMP: setWindowTitle(tr("Add a MySensors temperature")); break;
    case ITEM_DALI: setWindowTitle(tr("Add a MySensors Light dimmer")); break;
    case ITEM_LIGHT: setWindowTitle(tr("Add a MySensors Light")); break;
    case ITEM_LIGHT_RGB: setWindowTitle(tr("Add a MySensors RGB Light")); break;
    case ITEM_SHUTTER: setWindowTitle(tr("Add a MySensors Shutter")); break;
    default: break;
    }

    if (itemType == ITEM_ANALOG_IN)
    {
        analogWidget = new FormAnalogProperties(this, false);
        ui->verticalLayout->insertWidget(1, analogWidget);
    }
    else if (itemType == ITEM_ANALOG_OUT)
    {
        analogWidget = new FormAnalogProperties(this, true);
        ui->verticalLayout->insertWidget(1, analogWidget);
    }

    resize(minimumSize());
}

DialogNewMySensors::~DialogNewMySensors()
{
    delete ui;
}

void DialogNewMySensors::on_buttonBox_accepted()
{
    if (ui->lineEditName->text().isEmpty() ||
        ui->lineEditNodeid->text().isEmpty() ||
        ui->lineEditSensorid->text().isEmpty() ||
        (itemType == ITEM_SHUTTER &&
         (ui->lineEditNodeidDown->text().isEmpty() ||
          ui->lineEditSensoridDown->text().isEmpty())))
    {
        ui->label_error_empty->show();
        return;
    }

    Params p;
    p.Add("name", ui->lineEditName->text().toUtf8().constData());
    p.Add("node_id", ui->lineEditNodeid->text().toUtf8().constData());
    p.Add("sensor_id", ui->lineEditSensorid->text().toUtf8().constData());
    p.Add("gateway", ui->comboBoxType->currentIndex() == 0?"serial":"tcp");
    if (ui->comboBoxType->currentIndex() == 0)
    {
        p.Add("port", ui->comboBoxSerialport->currentText().toUtf8().constData());
    }
    else
    {
        p.Add("port", ui->lineEditTcpPort->text().toUtf8().constData());
        p.Add("host", ui->lineEditTcpIP->text().toUtf8().constData());
    }

    switch(itemType)
    {
    case ITEM_ANALOG_IN:
    {
        p.Add("type", "MySensorsInputAnalog");
        p.Add("unit", analogWidget->getUnit().toUtf8().constData());
        p.Add("coeff_a",  QString::number(analogWidget->getCoeff()).toUtf8().constData());
        p.Add("coeff_b",  QString::number(analogWidget->getOffset()).toUtf8().constData());
        io = ListeRoom::Instance().createInput(p, room);
        break;
    }
    case ITEM_ANALOG_OUT:
    {
        p.Add("type", "MySensorsOutputAnalog");
        p.Add("unit", analogWidget->getUnit().toUtf8().constData());
        p.Add("coeff_a",  QString::number(analogWidget->getCoeff()).toUtf8().constData());
        p.Add("coeff_b",  QString::number(analogWidget->getOffset()).toUtf8().constData());
        p.Add("step",  QString::number(analogWidget->getStep()).toUtf8().constData());
        io = ListeRoom::Instance().createOutput(p, room);
        break;
    }
    case ITEM_STRINGIN:
    {
        p.Add("type", "MySensorsInputString");
        io = ListeRoom::Instance().createInput(p, room);
        break;
    }
    case ITEM_STRINGOUT:
    {
        p.Add("type", "MySensorsOutputString");
        io = ListeRoom::Instance().createOutput(p, room);
        break;
    }
    case ITEM_INPUT_SWITCH:
    {
        if (ui->comboBoxSwitch->currentIndex() == 0)
            p.Add("type", "MySensorsInputSwitch");
        else if (ui->comboBoxSwitch->currentIndex() == 1)
            p.Add("type", "MySensorsInputSwitchTriple");
        else if (ui->comboBoxSwitch->currentIndex() == 2)
            p.Add("type", "MySensorsInputSwitchLongPress");

        io = ListeRoom::Instance().createInput(p, room);
        break;
    }
    case ITEM_TEMP:
    {
        p.Add("type", "MySensorsInputTemp");
        io = ListeRoom::Instance().createInput(p, room);
        break;
    }
    case ITEM_DALI:
    {
        p.Add("type", "MySensorsOutputDimmer");
        io = ListeRoom::Instance().createOutput(p, room);
        break;
    }
    case ITEM_LIGHT:
    {
        p.Add("type", "MySensorsOutputLight");
        io = ListeRoom::Instance().createOutput(p, room);
        break;
    }
    case ITEM_LIGHT_RGB:
    {
        p.Add("type", "MySensorsOutputLightRGB");
        p.Delete("node_id");
        p.Delete("sensor_id");
        p.Add("node_id_red", ui->lineEditNodeid->text().toUtf8().constData());
        p.Add("sensor_id_red", ui->lineEditSensorid->text().toUtf8().constData());
        p.Add("node_id_green", ui->lineEditNodeidDown->text().toUtf8().constData());
        p.Add("sensor_id_green", ui->lineEditSensoridDown->text().toUtf8().constData());
        p.Add("node_id_blue", ui->lineEditNodeidBlue->text().toUtf8().constData());
        p.Add("sensor_id_blue", ui->lineEditSensoridBlue->text().toUtf8().constData());

        io = ListeRoom::Instance().createOutput(p, room);
        break;
    }
    case ITEM_SHUTTER:
    {
        if (ui->check_smart->isChecked())
        {
            p.Add("type", "MySensorsOutputShutterSmart");
            p.Add("time_up", to_string(ui->spin_time_up->value()));
            p.Add("time_down", to_string(ui->spin_time_down->value()));
        }
        else
        {
            p.Add("type", "MySensorsOutputShutter");
            p.Add("time", to_string(ui->spin_time_up->value()));
        }

        p.Delete("node_id");
        p.Delete("sensor_id");

        p.Add("node_id_up", ui->lineEditNodeid->text().toUtf8().constData());
        p.Add("sensor_id_up", ui->lineEditSensorid->text().toUtf8().constData());
        p.Add("node_id_down", ui->lineEditNodeidDown->text().toUtf8().constData());
        p.Add("sensor_id_down", ui->lineEditSensoridDown->text().toUtf8().constData());

        if (ui->check_impulse->isChecked())
            p.Add("impulse_time", "200");

        io = ListeRoom::Instance().createOutput(p, room);

        break;
    }
    default: break;
    }

    accept();
}

void DialogNewMySensors::on_check_smart_stateChanged(int)
{
    if (itemType == ITEM_SHUTTER)
    {
        ui->label_nodeid_down->show();
        ui->lineEditNodeidDown->show();
        ui->label_sensorid_down->show();
        ui->lineEditSensoridDown->show();

        ui->lineEditNodeidBlue->hide();
        ui->label_nodeid_blue->hide();
        ui->lineEditSensoridBlue->hide();
        ui->label_sensorid_blue->hide();

        ui->label_time_up->show();
        ui->spin_time_up->show();
        ui->label_time_down->setVisible(ui->check_smart->isChecked());
        ui->spin_time_down->setVisible(ui->check_smart->isChecked());

        ui->label_nodeid_up->setText(tr("Node ID (Up)"));
        ui->label_sensorid_up->setText(tr("Sensor ID (Up)"));
        ui->label_nodeid_down->setText(tr("Node ID (Down)"));
        ui->label_sensorid_down->setText(tr("Sensor ID (Down)"));
        ui->label_time_up->setText(ui->check_smart->isChecked()?tr("Up duration (sec.) :"):
                                                                tr("Duration (sec.) :"));

        ui->check_impulse->show();
        ui->check_smart->show();
    }
    else if (itemType == ITEM_LIGHT_RGB)
    {
        ui->label_nodeid_down->show();
        ui->lineEditNodeidDown->show();
        ui->label_sensorid_down->show();
        ui->lineEditSensoridDown->show();

        ui->lineEditNodeidBlue->show();
        ui->label_nodeid_blue->show();
        ui->lineEditSensoridBlue->show();
        ui->label_sensorid_blue->show();

        ui->label_time_up->hide();
        ui->spin_time_up->hide();
        ui->label_time_down->hide();
        ui->spin_time_down->hide();

        ui->label_nodeid_up->setText(tr("Node ID (Red)"));
        ui->label_sensorid_up->setText(tr("Sensor ID (Red)"));
        ui->label_nodeid_down->setText(tr("Node ID (Green)"));
        ui->label_sensorid_down->setText(tr("Sensor ID (Green)"));

        ui->check_impulse->hide();
        ui->check_smart->hide();
    }
    else
    {
        ui->label_nodeid_down->hide();
        ui->lineEditNodeidDown->hide();
        ui->label_sensorid_down->hide();
        ui->lineEditSensoridDown->hide();

        ui->lineEditNodeidBlue->hide();
        ui->label_nodeid_blue->hide();
        ui->lineEditSensoridBlue->hide();
        ui->label_sensorid_blue->hide();

        ui->label_time_up->hide();
        ui->spin_time_up->hide();
        ui->label_time_down->hide();
        ui->spin_time_down->hide();
    }

    ui->check_impulse->setVisible(itemType == ITEM_SHUTTER);
    ui->check_smart->setVisible(itemType == ITEM_SHUTTER);
    ui->comboBoxSwitch->setVisible(itemType == ITEM_INPUT_SWITCH);
}

void DialogNewMySensors::on_comboBoxType_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->comboBoxSerialport->show();
        ui->label_gw_serialport->show();
        ui->label_gw_tcpip->hide();
        ui->label_gw_tcpport->hide();
        ui->lineEditTcpIP->hide();
        ui->lineEditTcpPort->hide();
    }
    else
    {
        ui->comboBoxSerialport->hide();
        ui->label_gw_serialport->hide();
        ui->label_gw_tcpip->show();
        ui->label_gw_tcpport->show();
        ui->lineEditTcpIP->show();
        ui->lineEditTcpPort->show();
    }
}
