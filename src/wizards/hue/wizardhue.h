#ifndef WIZARDHUE_H
#define WIZARDHUE_H

#include <QDialog>
#include <QTimer>
#include <QUdpSocket>
#include <ListeRoom.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTreeWidgetItem>

using namespace Calaos;

namespace Ui {
class WizardHue;
}

class WizardHue : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(WizardHue)
public:
    explicit WizardHue(Room *room, QWidget *parent = 0);
    QList<Params> paramsGet() {return params; }
    ~WizardHue();

private:
    Ui::WizardHue *ui;
    QUdpSocket *udpSocket;
    QTimer *timer;
    QHostAddress bridgeAddress;
    bool bridgeFound;
    bool apiKeyFound;
    QNetworkAccessManager *networkAccessManager;
    QString apiKey;
    int retry;

    QJsonObject hueConfig;

    void askForApiKey();
    void askForConfig();
    void hueBridgeDiscover();

    QList<Params>  params;

private slots:
    void readPendingDatagrams(void);
    void timeout(void);
    void apiKeyReceived();
    void configReceived();
    void on_buttonAdd_clicked();
    void on_buttonDel_clicked();
    void on_accepted();
    void on_treeWidgetItemDoubleClicked(QTreeWidgetItem * item, int column);
};

#endif // WIZARDHUE_H
