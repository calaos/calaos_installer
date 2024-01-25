#include "DialogZigbee2mqtt.h"
#include "ui_DialogZigbee2mqtt.h"
#include <QMessageBox>
#include "ConfigOptions.h"
#include <QInputDialog>
#include <QFile>
#include <QFileDialog>

const char *DialogZigbee2mqtt::kDevicesTopic = "zigbee2mqtt/bridge/devices";

DialogZigbee2mqtt::DialogZigbee2mqtt(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogZigbee2mqtt)
{
    ui->setupUi(this);
    ui->spinBoxPort->setMinimum(1);
    ui->spinBoxPort->setMaximum(65535);

    mqttClient = new QMqttClient(this);
    mqttClient->setPort(static_cast<quint16>(ui->spinBoxPort->value()));

    connect(ui->lineEditHost, &QLineEdit::textChanged, mqttClient, &QMqttClient::setHostname);
    connect(ui->spinBoxPort, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [this](int p)
            {
                mqttClient->setPort(static_cast<quint16>(p));
            }
    );
    connect(ui->lineEditUsername, &QLineEdit::textChanged, mqttClient, &QMqttClient::setUsername);
    connect(ui->lineEditPassword, &QLineEdit::textChanged, mqttClient, &QMqttClient::setPassword);
    connect(mqttClient, &QMqttClient::disconnected, this, &DialogZigbee2mqtt::brokerDisconnected);
    connect(mqttClient, &QMqttClient::stateChanged, this, &DialogZigbee2mqtt::stateChanged);

    ui->spinBoxPort->setValue(ConfigOptions::Instance().getMqttBrokerPort());
    ui->lineEditHost->setText(ConfigOptions::Instance().getMqttBrokerHost());
    ui->lineEditUsername->setText(ConfigOptions::Instance().getMqttBrokerUser());
    ui->lineEditPassword->setText(ConfigOptions::Instance().getMqttBrokerPass());

    ui->treeDevices->setModel(&devModel);
    ui->treeDevices->setSelectionBehavior(QAbstractItemView::SelectRows);

#ifndef QT_DEBUG
    ui->pushButtonLoadDemo->hide();
#endif
}

DialogZigbee2mqtt::~DialogZigbee2mqtt()
{
    delete ui;
}

Params DialogZigbee2mqtt::getIOParam()
{
    return paramDevice;
}

void DialogZigbee2mqtt::on_pushButtonConnect_clicked()
{
    if (mqttClient->state() == QMqttClient::Disconnected)
    {
        ui->lineEditHost->setEnabled(false);
        ui->spinBoxPort->setEnabled(false);
        ui->lineEditUsername->setEnabled(false);
        ui->lineEditPassword->setEnabled(false);

        spinner = new QAnimationLabel(":/img/loader.gif", this);
        ui->verticalLayoutConnect->addWidget(spinner, 0, Qt::AlignCenter);
        spinner->start();

        ui->pushButtonConnect->setEnabled(false);

        mqttClient->connectToHost();
    }
    else
    {
        mqttClient->disconnectFromHost();
    }
}

void DialogZigbee2mqtt::on_buttonBox_accepted()
{
    //Get the selected device from the treeview
    auto selection = ui->treeDevices->selectionModel()->selectedRows();
    if (selection.size() != 1)
    {
        QMessageBox::warning(this, tr("Error"), tr("Please select a device"));
        return;
    }

    //check if selection is a child item and not a root
    auto index = selection.at(0);
    if (!index.parent().isValid())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please select a parameter from a device, not the full device item"));
        return;
    }

    //get the item from the model
    auto item = devModel.itemFromIndex(index);
    qDebug() << "Selected item: " << item->text();

    //get parent from model
    auto parent = devModel.itemFromIndex(index.parent());
    qDebug() << "Parent item: " << parent->text();

    auto dev = parent->data(Qt::UserRole + 2).toJsonObject();
    auto expose = item->data(Qt::UserRole + 1).toJsonObject();

    QString exposeLabel = expose["label"].toString();
    if (exposeLabel.isEmpty())
        exposeLabel = expose["name"].toString();

    QString name = QStringLiteral("%1 - %2")
                       .arg(dev["friendly_name"].toString(), exposeLabel);
    if (expose["label"].toString().isEmpty() && !expose["features"].toArray().isEmpty())
        name = QStringLiteral("%1 - %2")
                   .arg(dev["friendly_name"].toString(),
                        expose["features"].toArray().at(0).toObject()["label"].toString());

    paramDevice.Add("name", name.toStdString());
    paramDevice.Add("host", ConfigOptions::Instance().getMqttBrokerHost().toStdString());
    paramDevice.Add("port", QString::number(ConfigOptions::Instance().getMqttBrokerPort()).toStdString());
    paramDevice.Add("user", ConfigOptions::Instance().getMqttBrokerUser().toStdString());
    paramDevice.Add("password", ConfigOptions::Instance().getMqttBrokerPass().toStdString());

    if (expose["type"].toString() == "enum" ||
        expose["type"].toString() == "binary")
    {
        paramDevice.Add("type", "MqttInputSwitch");
        paramDevice.Add("io_type", "input");
        paramDevice.Add("topic_sub", "zigbee2mqtt/" + dev["friendly_name"].toString().toStdString());
        paramDevice.Add("path", expose["property"].toString().toStdString());

        QStringList items;
        auto arr = expose["values"].toArray();
        for (const auto &val: arr)
            items << val.toString();

        bool ok;
        QString val = QInputDialog::getItem(this, tr("Choose"), tr("Select action from device:"), items, 0, false, &ok);
        if (!ok) return;

        paramDevice.Add("on_value", val.toStdString());
    }
    else if (expose["type"].toString() == "numeric")
    {
        paramDevice.Add("io_type", "input");

        if (expose["name"].toString() == "temperature")
        {
            paramDevice.Add("type", "MqttInputTemp");
            paramDevice.Add("topic_sub", "zigbee2mqtt/" + dev["friendly_name"].toString().toStdString());
            paramDevice.Add("path", expose["property"].toString().toStdString());
        }
        else
        {
            paramDevice.Add("type", "MqttInputAnalog");
            paramDevice.Add("topic_sub", "zigbee2mqtt/" + dev["friendly_name"].toString().toStdString());
            paramDevice.Add("path", expose["property"].toString().toStdString());
            paramDevice.Add("unit", expose["unit"].toString().toStdString());

            if (expose["name"].toString() == "humidity")
                paramDevice.Add("io_style", "humidity");
            else if (expose["name"].toString() == "pressure")
                paramDevice.Add("io_style", "pressure");
            else if (expose["name"].toString() == "voltage")
                paramDevice.Add("io_style", "voltage");
            else if (expose["name"].toString() == "linkquality")
                paramDevice.Add("unit", "LQI");
            else if (expose["name"].toString() == "current")
                paramDevice.Add("io_style", "current");
            else if (expose["name"].toString() == "power" ||
                     expose["name"].toString() == "energy")
                paramDevice.Add("io_style", "watt");
            else if (expose["name"].toString() == "luminosity")
                paramDevice.Add("io_style", "luminosity");

        }
    }
    else if (expose["type"].toString() == "switch" ||
             expose["type"].toString() == "lock")
    {
        paramDevice.Add("type", "MqttOutputLight");
        paramDevice.Add("io_type", "output");
        paramDevice.Add("topic_sub", "zigbee2mqtt/" + dev["friendly_name"].toString().toStdString());
        paramDevice.Add("topic_pub", "zigbee2mqtt/" + dev["friendly_name"].toString().toStdString() + "/set");

        QJsonObject feature;
        if (expose["features"].toArray().size() > 0)
            feature = expose["features"].toArray().at(0).toObject();

        paramDevice.Add("on_value", feature["value_on"].toString().toStdString());
        paramDevice.Add("off_value", feature["value_off"].toString().toStdString());

        QString data = QStringLiteral("{\"%1\": \"__##VALUE##__\"}")
                           .arg(feature["property"].toString());

        paramDevice.Add("data", data.toStdString());
        paramDevice.Add("path", feature["property"].toString().toStdString());
    }
    else if (expose["type"].toString() == "light")
    {
        paramDevice.Add("io_type", "output");
        paramDevice.Add("topic_sub", "zigbee2mqtt/" + dev["friendly_name"].toString().toStdString());
        paramDevice.Add("topic_pub", "zigbee2mqtt/" + dev["friendly_name"].toString().toStdString() + "/set");

        //check if dimmer or RGB
        if (hasFeature(expose["features"].toArray(), "color_xy"))
        {
            paramDevice.Add("type", "MqttOutputLightRGB");

            QJsonObject feature = getFeature(expose["features"].toArray(), "brightness");

            QString data;

            if (hasFeature(expose["features"].toArray(), "color_xy"))
                data = QStringLiteral("{\"color\":{\"x\":__##VALUE_X##__,\"y\":__##VALUE_Y##__},\"%1\":__##VALUE_BRIGHTNESS##__}")
                           .arg(feature["property"].toString());
            else
                data = QStringLiteral("{\"color\":{\"hex\":\"__##VALUE_HEX##__\"},\"%1\":__##VALUE_BRIGHTNESS##__}")
                           .arg(feature["property"].toString());

            paramDevice.Add("data", data.toStdString());
            paramDevice.Add("path_brightness", feature["property"].toString().toStdString());
            paramDevice.Add("path_x", "color/y");
            paramDevice.Add("path_y", "color/x");
        }
        else
        {
            paramDevice.Add("type", "MqttOutputLightDimmer");

            QJsonObject feature = getFeature(expose["features"].toArray(), "brightness");

            QString data = QStringLiteral("{\"%1\": \"__##VALUE##__\"}")
                               .arg(feature["property"].toString());

            paramDevice.Add("data", data.toStdString());
            paramDevice.Add("path", feature["property"].toString().toStdString());
        }
    }

    accept();
}

void DialogZigbee2mqtt::brokerDisconnected()
{
    ui->lineEditHost->setEnabled(true);
    ui->spinBoxPort->setEnabled(true);
    ui->lineEditUsername->setEnabled(true);
    ui->lineEditPassword->setEnabled(true);
    ui->pushButtonConnect->show();
}

void DialogZigbee2mqtt::stateChanged(QMqttClient::ClientState state)
{
    qDebug() << "stateChanged: " << state;

    if (state == QMqttClient::Connected)
    {
        ui->pushButtonConnect->setText(tr("Disconnect"));
        ui->pushButtonConnect->setEnabled(true);

        if (spinner)
        {
            spinner->stop();
            delete spinner;
        }

        ConfigOptions::Instance().setMqttBrokerHost(ui->lineEditHost->text());
        ConfigOptions::Instance().setMqttBrokerPort(ui->spinBoxPort->value());
        ConfigOptions::Instance().setMqttBrokerUser(ui->lineEditUsername->text());
        ConfigOptions::Instance().setMqttBrokerPass(ui->lineEditPassword->text());

        //Register to the topic
        auto subscription = mqttClient->subscribe({kDevicesTopic});
        if (!subscription)
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not subscribe. Is there a valid connection?"));
            return;
        }

        connect(subscription, &QMqttSubscription::messageReceived, this, [this](QMqttMessage msg)
        {
            messageReceived(msg.payload());
        });
    }
    else if (state == QMqttClient::Disconnected)
    {
        ui->lineEditHost->setEnabled(true);
        ui->spinBoxPort->setEnabled(true);
        ui->lineEditUsername->setEnabled(true);
        ui->lineEditPassword->setEnabled(true);
        ui->pushButtonConnect->setText(tr("Connect"));
        ui->pushButtonConnect->setEnabled(true);

        ui->pushButtonConnect->setEnabled(true);

        if (spinner)
        {
            spinner->stop();
            delete spinner;
        }
    }
}

void DialogZigbee2mqtt::messageReceived(const QByteArray &message)
{
    QJsonParseError err;
    QJsonDocument jdoc = QJsonDocument::fromJson(message, &err);
    if (err.error != QJsonParseError::NoError)
    {
        qWarning() << "JSON parse error " << err.errorString() << " at offset: " << err.offset;
        const int maxlen = 100;
        int start = err.offset - maxlen;
        if (start < 0) start = 0;
        qWarning() << message.mid(start, err.offset + maxlen);
        QString s;
        for (int i = start;i < err.offset + maxlen;i++)
            s += (i == err.offset?"^":"-");
        qWarning() << s;
    }

    devModel.clear();
    devModel.setHorizontalHeaderLabels(QStringList() << "Name" << "Type" << "Description");

    QJsonArray devices = jdoc.array();
    for (const auto &obj: devices)
    {
        QJsonObject device = obj.toObject();
        if (device["type"].toString() == "Coordinator")
            continue;

        addDevice(device);
    }
}

void DialogZigbee2mqtt::addDevice(const QJsonObject &device)
{
    auto def = device["definition"].toObject();
    auto item = new QStandardItem(QString("%1 [%2 - %3]")
                                      .arg(device["friendly_name"].toString(),
                                           def["vendor"].toString(),
                                           def["description"].toString()));
    item->setData(device["ieeeAddr"].toString(), Qt::UserRole + 1);
    item->setData(device, Qt::UserRole + 2);

    auto exposes = def["exposes"].toArray();
    for (const auto &expose: exposes)
    {
        QList<QStandardItem *> rowItems;
        QJsonObject obj = expose.toObject();

        QString desc = obj["name"].toString();
        if (obj.contains("label"))
            desc = obj["label"].toString();

        if (desc.isEmpty() && obj.contains("features") &&
            !obj["features"].toArray().isEmpty())
        {
            auto o = obj["features"].toArray().at(0).toObject();
            if (o.contains("label"))
                desc = o["label"].toString();
            else
                desc = o["name"].toString();
        }

        auto itemChild = new QStandardItem(desc);
        itemChild->setData(obj, Qt::UserRole + 1);
        rowItems << itemChild;

        if (obj["type"].toString() == "enum" ||
            obj["type"].toString() == "binary")
            itemChild = new QStandardItem("Switch");
        else if (obj["type"].toString() == "numeric")
            itemChild = new QStandardItem("Numeric Sensor");
        else if (obj["type"].toString() == "switch" ||
                 obj["type"].toString() == "lock")
            itemChild = new QStandardItem("Light output");
        else if (obj["type"].toString() == "light") //most likely a Dimmer or RGB
        {
            //check if it has required features
            if (hasFeature(obj["features"].toArray(), "state") &&
                hasFeature(obj["features"].toArray(), "brightness"))
            {
                if (hasFeature(obj["features"].toArray(), "color_xy"))
                    itemChild = new QStandardItem("RGB");
                else
                    itemChild = new QStandardItem("Dimmer");
            }
        }
        else
            itemChild = new QStandardItem("");

        rowItems << itemChild;

        if (obj["type"].toString() == "switch" ||
            obj["type"].toString() == "lock")
        {
            QString desc;
            if (!obj["features"].toArray().isEmpty())
            {
                auto feature = obj["features"].toArray().at(0).toObject();
                desc = feature[""].toString();
            }
            itemChild = new QStandardItem(desc);
        }
        else
        {
            itemChild = new QStandardItem(obj["description"].toString());
        }

        rowItems << itemChild;

        item->appendRow(rowItems);
    }

    devModel.appendRow(item);

    ui->treeDevices->setFirstColumnSpanned(devModel.rowCount() - 1, ui->treeDevices->rootIndex(), true);
}

bool DialogZigbee2mqtt::hasFeature(const QJsonArray &l, const QString &feature)
{
    for (const auto &obj: l)
    {
        if (obj.toObject()["name"].toString() == feature)
            return true;
    }

    return false;
}

QJsonObject DialogZigbee2mqtt::getFeature(const QJsonArray &l, const QString &feature)
{
    for (const auto &obj: l)
    {
        if (obj.toObject()["name"].toString() == feature)
            return obj.toObject();
    }

    return QJsonObject();
}

void DialogZigbee2mqtt::on_pushButtonLoadDemo_clicked()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Open zigbee2mqtt json file"), QString(), tr("JSON files (*.*)")));
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not open json file"));
        return;
    }

    QByteArray data = file.readAll();
    messageReceived(data);
}
