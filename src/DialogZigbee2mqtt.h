#ifndef DIALOGZIGBEE2MQTT_H
#define DIALOGZIGBEE2MQTT_H

#include <QDialog>
#include <QPointer>
#include "Params.h"
#include "MqttListener.h"
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

    void brokerAccepted();
    void listening();
    void listenerFinished(const QString &error);
    void messageReceived(const QByteArray &message);

private:
    Ui::DialogZigbee2mqtt *ui;

    QPointer<QAnimationLabel> spinner;
    MqttListener *listener = nullptr;

    QStandardItemModel devModel;
    Params paramDevice;

    static const char *kDevicesTopic;

    void showDisconnected();
    void addDevice(const QJsonObject &device);
    bool hasFeature(const QJsonArray &l, const QString &feature);
    QJsonObject getFeature(const QJsonArray &l, const QString &feature);
};

#endif // DIALOGZIGBEE2MQTT_H
