#ifndef DIALOGZIGBEE2MQTT_H
#define DIALOGZIGBEE2MQTT_H

#include <QDialog>
#include "Params.h"
#include <QtMqtt/QMqttClient>
#include "qanimationlabel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardItemModel>

namespace Ui {
class DialogZigbee2mqtt;
}

class DialogZigbee2mqtt : public QDialog
{
    Q_OBJECT

public:
    explicit DialogZigbee2mqtt(QWidget *parent = nullptr);
    ~DialogZigbee2mqtt();

    Params getIOParam();

private slots:
    void on_pushButtonConnect_clicked();
    void on_buttonBox_accepted();
    void on_pushButtonLoadDemo_clicked();

    void brokerDisconnected();
    void stateChanged(QMqttClient::ClientState state);
    void messageReceived(const QByteArray &message);

private:
    Ui::DialogZigbee2mqtt *ui;

    QPointer<QAnimationLabel> spinner;
    QMqttClient *mqttClient = nullptr;

    QStandardItemModel devModel;
    Params paramDevice;

    static const char *kDevicesTopic;

    void addDevice(const QJsonObject &device);
    bool hasFeature(const QJsonArray &l, const QString &feature);
    QJsonObject getFeature(const QJsonArray &l, const QString &feature);
};

#endif // DIALOGZIGBEE2MQTT_H
