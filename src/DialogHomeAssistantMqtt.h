#ifndef DIALOGHOMEASSISTANTMQTT_H
#define DIALOGHOMEASSISTANTMQTT_H

#include <QDialog>
#include <QJsonArray>

#include "HomeAssistantDiscovery.h"
#include "MqttListener.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTreeWidget;
class QTreeWidgetItem;

// Lists the entities Home Assistant MQTT discovery announces on a broker and
// imports the checked ones: an IO each, a device for a climate.
class DialogHomeAssistantMqtt: public QDialog
{
    Q_OBJECT

public:
    //templates: where climates take their device from; room: the one offered first.
    DialogHomeAssistantMqtt(const QJsonArray &templates, Calaos::Room *room, QWidget *parent = nullptr);

    void setTimings(int listenMs, int answerTimeoutMs);
    void search();
    QList<HomeAssistantDiscovery::Entity> entities() const { return m_found; }

    //Set once Import succeeded.
    const HomeAssistantDiscovery::Imported &imported() const { return m_imported; }
    Calaos::Room *room() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void searchFinished();

private:
    QJsonArray m_templates;
    MqttListener *m_listener;
    HomeAssistantDiscovery m_discovery;
    QList<HomeAssistantDiscovery::Entity> m_found;
    HomeAssistantDiscovery::Imported m_imported;
    int m_listenMs = 3000;
    int m_answerTimeoutMs = 5000;
    bool m_probeTyped = false;
    bool m_credentialsTyped = false;

    QLineEdit *m_serverHost;
    QSpinBox *m_serverPort;
    QLineEdit *m_serverUser;
    QLineEdit *m_serverPassword;
    QLineEdit *m_probeHost;
    QLineEdit *m_prefix;
    QPushButton *m_searchButton;
    QLabel *m_status;
    QTreeWidget *m_list;
    QComboBox *m_room;
    QLabel *m_error;
    QPushButton *m_importButton;

    MqttBroker serverBroker() const;
    void updateProbeHost();
    void finished(const QString &error);
    void fillList();
    QList<HomeAssistantDiscovery::Entity> checkedEntities() const;
    void updateImportButton();
    void importChecked();
    void setStatus(const QString &text, bool error);
};

#endif // DIALOGHOMEASSISTANTMQTT_H
