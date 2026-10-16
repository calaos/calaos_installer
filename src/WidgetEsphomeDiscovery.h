#ifndef WIDGETESPHOMEDISCOVERY_H
#define WIDGETESPHOMEDISCOVERY_H

#include <QGroupBox>
#include <functional>

#include "EsphomeDiscovery.h"
#include "MqttListener.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QTreeWidget;

// "Find it on the broker" in the template dialog: listens to the MQTT broker,
// lists the ESPHome climates it knows, and hands the chosen one to the dialog.
class WidgetEsphomeDiscovery: public QGroupBox
{
    Q_OBJECT

public:
    //settings: port, user and password to reach that host, read at each search.
    explicit WidgetEsphomeDiscovery(std::function<MqttListener::Settings(const QString &host)> settings,
                                    QWidget *parent = nullptr);

    //MqttBroker::probeHost() of these, until the user types another address.
    void setHosts(const QString &brokerHost, const QString &reachedBroker, const QString &calaosHost);
    void setTimings(int listenMs, int answerTimeoutMs);
    void search();
    void showApplied(const EsphomeDiscovery::Climate &c, const QStringList &summary);

    QList<EsphomeDiscovery::Climate> climates() const { return m_found; }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void climateChosen(const EsphomeDiscovery::Climate &c);
    void searchFinished();

private:
    std::function<MqttListener::Settings(const QString &host)> m_settings;
    MqttListener *m_listener;
    EsphomeDiscovery m_discovery;
    QList<EsphomeDiscovery::Climate> m_found;
    int m_listenMs = 3000;
    int m_answerTimeoutMs = 5000;
    bool m_hostTyped = false;

    QLineEdit *m_host;
    QPushButton *m_button;
    QLabel *m_status;
    QTreeWidget *m_list;
    QLabel *m_applied;

    void finished(const QString &error);
    void setStatus(const QString &text, bool error);
};

#endif // WIDGETESPHOMEDISCOVERY_H
