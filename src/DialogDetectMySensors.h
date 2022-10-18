#ifndef DIALOGDETECTMYSENSORS_H
#define DIALOGDETECTMYSENSORS_H

#include <QDialog>
#include <QtNetwork>
#include "qanimationlabel.h"
#include "IEntryHelper.h"
#include "Params.h"

#include <unordered_map>

namespace Ui
{
class DialogDetectMySensors;
}

class DialogDetectMySensors : public QDialog, public IEntryHelper
{
    Q_OBJECT

public:
    explicit DialogDetectMySensors(const Params &parentParams, QWidget *parent = 0);
    ~DialogDetectMySensors();

    Params& getParams();
    int exec() { return QDialog::exec(); }
    int searchSensors();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogDetectMySensors *ui;

    QPointer<QWidget> spinnerLayoutWidget;
    QPointer<QHBoxLayout> spinnerLayout;
    QPointer<QAnimationLabel> spinner;

    QPointer<QTcpSocket> tcpSocket;
    QPointer<QTimer> timer;

    Params params;
    const Params &parentParams;

    std::list<QStringList> listSensor;

    void addSensorToList(QStringList fields);
    void sendPrensentationRequest(QString nodeId);
    void enableUI();

private slots:
    void on_buttonDiscover_clicked();
    void timerDone();
    void socketReadyRead();
    void socketConnected();
    void socketdisplayError(QAbstractSocket::SocketError socketError);
    void on_comboBoxGateway_activated(const QString &arg1);
};

#endif // DIALOGDETECTMYSENSORS_H
