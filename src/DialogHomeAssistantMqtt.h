#ifndef DIALOGHOMEASSISTANTMQTT_H
#define DIALOGHOMEASSISTANTMQTT_H

#include <QDialog>
#include <QPointer>
#include <QStandardItemModel>
#include <QtMqtt/QMqttClient>
#include "Params.h"
#include "HomeAssistantDiscovery.h"
#include "qanimationlabel.h"

namespace Calaos { class Room; }

namespace Ui {
class DialogHomeAssistantMqtt;
}

class DialogHomeAssistantMqtt : public QDialog
{
    Q_OBJECT
public:
    explicit DialogHomeAssistantMqtt(QWidget *parent = nullptr);
    ~DialogHomeAssistantMqtt();

    struct ImportItem {
        Params params;
        Calaos::Room *room;
    };

    // Items the user selected for import (after Accepted).
    QList<ImportItem> getImportItems() const { return importItems; }

    // Skipped duplicates / errors collected during accept().
    QStringList getWarnings() const { return warnings; }

private slots:
    void on_pushButtonConnect_clicked();
    void on_buttonBox_accepted();
    void brokerDisconnected();
    void stateChanged(QMqttClient::ClientState state);

private:
    Ui::DialogHomeAssistantMqtt *ui;
    QPointer<QAnimationLabel> spinner;
    QMqttClient *mqttClient = nullptr;

    HomeAssistantDiscovery discovery;
    QStandardItemModel model;
    // Map deviceKey -> root QStandardItem
    QHash<QString, QStandardItem*> deviceItems;
    // Map uniqueId -> child QStandardItem
    QHash<QString, QStandardItem*> entityItems;

    QList<ImportItem> importItems;
    QStringList warnings;

    void onEntity(const HomeAssistantDiscovery::Entity &e);
    void onEntityRemoved(const QString &uniqueId);
    void subscribeDiscovery();
    QString currentPrefix() const;
};

#endif // DIALOGHOMEASSISTANTMQTT_H
