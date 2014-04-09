#ifndef DIALOGDETECTSQUEEZEBOX_H
#define DIALOGDETECTSQUEEZEBOX_H

#include <QtWidgets>
#include <QDialog>
#include <QtNetwork>
#include "qanimationlabel.h"

namespace Ui
{
class DialogDetectSqueezebox;
}


/* SLIMProto */
typedef union slim
{
    struct
    {
        char cmd;
        char reserved1;
        char device_id;
        char firm_rev;
        char reserved2[8];
        char mac_addr[6];
    } client;
    struct
    {
        char cmd;
        char reserved1;
        char server_ip[4];
        int server_port;
        char reserved2[10];
    } server;
} SLIMProto;

const SLIMProto discoverPacket = {{ 'd', /* command */
                                    0x00, /* reserved */
                                    0x01, /* device id */
                                    0x11, /* firmware revision 1.1 */
                                    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* reserved */
                                    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }}; /* MAC address */

struct SqueezeServerCommand
{
    QString cmd;
    QString res;
    QString slot;
    QObject *obj;
};

class SqueezeServer: public QObject
{
    Q_OBJECT

public :
    explicit SqueezeServer(QObject *parent = 0);
    ~SqueezeServer();

    void Connect(QHostAddress &addr);
    void SendCommand(QString cmd, QObject *obj, QString slot);

private:
    QPointer<QTcpSocket> cliSocket;
    SqueezeServerCommand cmd;

signals:
    void error();
    void connectError();
    void connected();

private slots:
    void readTCPPacket();
    void tcpError(QAbstractSocket::SocketError socketError);
    void tcpConnected();
};

typedef struct _Squeezebox
{
    QString server;
    QString id;
    QString name;
} Squeezebox;

class DialogDetectSqueezebox : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDetectSqueezebox(QWidget *parent = 0);
    ~DialogDetectSqueezebox();

    void SearchDevices();

    Squeezebox *getSelectedDevice();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogDetectSqueezebox *ui;

    QPointer<QWidget> spinnerLayoutWidget;
    QPointer<QHBoxLayout> spinnerLayout;

    QPointer<QUdpSocket> socket;
    QPointer<SqueezeServer> squeeze;
    QPointer<QTimer> timer;
    QPointer<QAnimationLabel> spinner;
    QVector<QHostAddress> squeezeserver;
    QVector<Squeezebox> squeezebox;
    int cpt, cptSqueeze;
    int numberSqueeze;
    Squeezebox sq;

    void scanSqueezeserver();
    void getSqueezeId();
    void getSqueezeName();

private slots:
    void on_buttonRefresh_clicked();

    void searchDone();
    void timerDoneAndDiscoverPlayers();
    void readUDPDatagrams();
    void squeezeError();
    void squeezeConnected();
    void squeezePlayerCount(QString cmd, QString res);
    void squeezePlayerId(QString cmd, QString res);
    void squeezePlayerName(QString cmd, QString res);
};

#endif // DIALOGDETECTSQUEEZEBOX_H
