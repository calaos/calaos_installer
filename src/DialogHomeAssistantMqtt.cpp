#include "DialogHomeAssistantMqtt.h"
#include "ui_DialogHomeAssistantMqtt.h"
#include "ConfigOptions.h"
#include "ListeRoom.h"
#include "Room.h"
#include "IOBase.h"
#include <QMessageBox>
#include <QMqttSubscription>

using namespace Calaos;

DialogHomeAssistantMqtt::DialogHomeAssistantMqtt(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogHomeAssistantMqtt)
{
    ui->setupUi(this);
    ui->spinBoxPort->setMinimum(1);
    ui->spinBoxPort->setMaximum(65535);

    mqttClient = new QMqttClient(this);

    connect(ui->lineEditHost, &QLineEdit::textChanged, mqttClient, &QMqttClient::setHostname);
    connect(ui->spinBoxPort, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [this](int p) { mqttClient->setPort(static_cast<quint16>(p)); });
    connect(ui->lineEditUsername, &QLineEdit::textChanged, mqttClient, &QMqttClient::setUsername);
    connect(ui->lineEditPassword, &QLineEdit::textChanged, mqttClient, &QMqttClient::setPassword);
    connect(mqttClient, &QMqttClient::disconnected, this, &DialogHomeAssistantMqtt::brokerDisconnected);
    connect(mqttClient, &QMqttClient::stateChanged, this, &DialogHomeAssistantMqtt::stateChanged);

    ui->spinBoxPort->setValue(ConfigOptions::Instance().getMqttBrokerPort());
    ui->lineEditHost->setText(ConfigOptions::Instance().getMqttBrokerHost());
    ui->lineEditUsername->setText(ConfigOptions::Instance().getMqttBrokerUser());
    ui->lineEditPassword->setText(ConfigOptions::Instance().getMqttBrokerPass());
    ui->lineEditPrefix->setText(ConfigOptions::Instance().getHADiscoveryPrefix());

    mqttClient->setHostname(ui->lineEditHost->text());
    mqttClient->setPort(static_cast<quint16>(ui->spinBoxPort->value()));
    mqttClient->setUsername(ui->lineEditUsername->text());
    mqttClient->setPassword(ui->lineEditPassword->text());

    // Populate room combo from current project.
    for (int i = 0; i < ListeRoom::Instance().size(); ++i) {
        Room *r = ListeRoom::Instance().get_room(i);
        ui->comboBoxRoom->addItem(QString::fromStdString(r->get_name()), QVariant::fromValue(reinterpret_cast<quintptr>(r)));
    }

    model.setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Component") << tr("Topic"));
    ui->treeEntities->setModel(&model);

    connect(&discovery, &HomeAssistantDiscovery::entityAddedOrUpdated,
            this, &DialogHomeAssistantMqtt::onEntity);
    connect(&discovery, &HomeAssistantDiscovery::entityRemoved,
            this, &DialogHomeAssistantMqtt::onEntityRemoved);
}

DialogHomeAssistantMqtt::~DialogHomeAssistantMqtt()
{
    delete ui;
}

QString DialogHomeAssistantMqtt::currentPrefix() const
{
    QString p = ui->lineEditPrefix->text().trimmed();
    if (p.isEmpty()) p = "homeassistant";
    return p;
}

void DialogHomeAssistantMqtt::on_pushButtonConnect_clicked()
{
    if (mqttClient->state() == QMqttClient::Disconnected) {
        ui->lineEditHost->setEnabled(false);
        ui->spinBoxPort->setEnabled(false);
        ui->lineEditUsername->setEnabled(false);
        ui->lineEditPassword->setEnabled(false);
        ui->lineEditPrefix->setEnabled(false);

        spinner = new QAnimationLabel(":/img/loader.gif", this);
        ui->verticalLayoutConnect->addWidget(spinner, 0, Qt::AlignCenter);
        spinner->start();

        ui->pushButtonConnect->setEnabled(false);
        mqttClient->connectToHost();
    } else {
        mqttClient->disconnectFromHost();
    }
}

void DialogHomeAssistantMqtt::subscribeDiscovery()
{
    QString topic = currentPrefix() + "/#";
    auto sub = mqttClient->subscribe({topic});
    if (!sub) {
        QMessageBox::critical(this, tr("Error"), tr("Could not subscribe to %1").arg(topic));
        return;
    }
    connect(sub, &QMqttSubscription::messageReceived, this, [this](QMqttMessage msg) {
        QString t = msg.topic().name();
        if (!HomeAssistantDiscovery::isDiscoveryTopic(t, currentPrefix()))
            return;
        discovery.handleMessage(t, msg.payload());
    });
    ui->labelStatus->setText(tr("Subscribed to %1 - waiting for retained discovery messages...").arg(topic));
}

void DialogHomeAssistantMqtt::stateChanged(QMqttClient::ClientState state)
{
    if (state == QMqttClient::Connected) {
        ui->pushButtonConnect->setText(tr("Disconnect"));
        ui->pushButtonConnect->setEnabled(true);
        if (spinner) { spinner->stop(); delete spinner; }

        ConfigOptions::Instance().setMqttBrokerHost(ui->lineEditHost->text());
        ConfigOptions::Instance().setMqttBrokerPort(ui->spinBoxPort->value());
        ConfigOptions::Instance().setMqttBrokerUser(ui->lineEditUsername->text());
        ConfigOptions::Instance().setMqttBrokerPass(ui->lineEditPassword->text());
        ConfigOptions::Instance().setHADiscoveryPrefix(currentPrefix());

        subscribeDiscovery();
    } else if (state == QMqttClient::Disconnected) {
        ui->lineEditHost->setEnabled(true);
        ui->spinBoxPort->setEnabled(true);
        ui->lineEditUsername->setEnabled(true);
        ui->lineEditPassword->setEnabled(true);
        ui->lineEditPrefix->setEnabled(true);
        ui->pushButtonConnect->setText(tr("Connect"));
        ui->pushButtonConnect->setEnabled(true);
        if (spinner) { spinner->stop(); delete spinner; }
    }
}

void DialogHomeAssistantMqtt::brokerDisconnected()
{
    ui->labelStatus->setText(tr("Disconnected."));
}

void DialogHomeAssistantMqtt::onEntity(const HomeAssistantDiscovery::Entity &e)
{
    QStandardItem *devItem = deviceItems.value(e.deviceKey, nullptr);
    if (!devItem) {
        devItem = new QStandardItem(e.deviceName.isEmpty() ? e.deviceKey : e.deviceName);
        QFont f = devItem->font(); f.setBold(true); devItem->setFont(f);
        deviceItems.insert(e.deviceKey, devItem);
        model.appendRow(devItem);
        ui->treeEntities->setFirstColumnSpanned(model.rowCount() - 1, ui->treeEntities->rootIndex(), false);
    }

    QString name = e.name.isEmpty() ? e.objectId : e.name;
    QString topic = e.payload.value("state_topic").toString();
    if (topic.isEmpty()) topic = e.payload.value("command_topic").toString();

    QStandardItem *existing = entityItems.value(e.uniqueId, nullptr);
    if (existing) {
        existing->setText(name);
        existing->parent()->child(existing->row(), 1)->setText(e.component);
        existing->parent()->child(existing->row(), 2)->setText(topic);
    } else {
        auto *nameItem = new QStandardItem(name);
        nameItem->setData(e.uniqueId, Qt::UserRole + 1);
        auto *compItem = new QStandardItem(e.component);
        auto *topicItem = new QStandardItem(topic);
        devItem->appendRow({nameItem, compItem, topicItem});
        entityItems.insert(e.uniqueId, nameItem);
    }
}

void DialogHomeAssistantMqtt::onEntityRemoved(const QString &uniqueId)
{
    QStandardItem *item = entityItems.take(uniqueId);
    if (!item) return;
    QStandardItem *parent = item->parent();
    if (parent) parent->removeRow(item->row());
}

static bool ioWithUniqueIdExists(const QString &uniqueId)
{
    if (uniqueId.isEmpty()) return false;
    auto check = [&](IOBase *io) {
        // Stored ids may be suffixed "<uid>#<n>" when one entity yielded several IOs.
        QString stored = QString::fromStdString(io->get_params()["ha_unique_id"]);
        return stored == uniqueId || stored.section('#', 0, 0) == uniqueId;
    };
    auto &lr = ListeRoom::Instance();
    for (int i = 0; i < lr.size(); ++i) {
        Room *r = lr.get_room(i);
        for (int j = 0; j < r->get_size_in(); ++j)
            if (check(r->get_input(j))) return true;
        for (int j = 0; j < r->get_size_out(); ++j)
            if (check(r->get_output(j))) return true;
    }
    return false;
}

void DialogHomeAssistantMqtt::on_buttonBox_accepted()
{
    auto selection = ui->treeEntities->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Please select at least one entity to import."));
        return;
    }

    Room *targetRoom = reinterpret_cast<Room*>(static_cast<quintptr>(
        ui->comboBoxRoom->currentData().toULongLong()));
    if (!targetRoom) {
        QMessageBox::warning(this, tr("Error"), tr("Please select a target room."));
        return;
    }

    HomeAssistantDiscovery::BrokerCreds b;
    b.host = ui->lineEditHost->text();
    b.port = ui->spinBoxPort->value();
    b.user = ui->lineEditUsername->text();
    b.password = ui->lineEditPassword->text();

    auto byDevice = discovery.entitiesByDevice();
    QHash<QString, HomeAssistantDiscovery::Entity> byUniqueId;
    for (const auto &list : byDevice)
        for (const auto &e : list)
            byUniqueId.insert(e.uniqueId, e);

    importItems.clear();
    warnings.clear();

    for (const auto &idx : selection) {
        // Skip device (root) rows.
        if (!idx.parent().isValid()) continue;
        auto *item = model.itemFromIndex(idx);
        if (!item) continue;
        QString uniqueId = item->data(Qt::UserRole + 1).toString();
        auto eIt = byUniqueId.constFind(uniqueId);
        if (eIt == byUniqueId.constEnd()) continue;

        if (ioWithUniqueIdExists(uniqueId)) {
            warnings << tr("Skipped (already imported): %1").arg(item->text());
            continue;
        }

        QList<Params> ios;
        QString reason;
        if (!HomeAssistantDiscovery::toCalaosParams(eIt.value(), b, ios, reason)) {
            warnings << tr("Skipped %1: %2").arg(item->text(), reason);
            continue;
        }
        // One entity may yield several IOs (e.g. climate). Tag them all with the same
        // ha_unique_id (suffixed per-IO when there are several) so re-import is detected.
        int n = 0;
        for (Params p : ios) {
            QString uid = ios.size() > 1
                ? QStringLiteral("%1#%2").arg(uniqueId).arg(n++)
                : uniqueId;
            p.Add("ha_unique_id", uid.toStdString());
            importItems.append({p, targetRoom});
        }
    }

    if (importItems.isEmpty() && !warnings.isEmpty()) {
        QMessageBox::warning(this, tr("Nothing to import"), warnings.join('\n'));
        return;
    }

    accept();
}
