#ifndef DIALOGDETECTXPL_H
#define DIALOGDETECTXPL_H

#include <QtWidgets>
#include <QDialog>
#include <QtNetwork>
#include "qanimationlabel.h"
#include "IEntryHelper.h"
#include "Params.h"

namespace Ui
{
class DialogDetectxPL;
}

class DialogDetectxPL : public QDialog, public IEntryHelper
{
    Q_OBJECT

public:
    explicit DialogDetectxPL(QWidget *parent = 0);
    ~DialogDetectxPL();

    Params& getParams();
    int exec() { return QDialog::exec(); }
    void SearchDevices();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogDetectxPL *ui;

    QPointer<QWidget> spinnerLayoutWidget;
    QPointer<QHBoxLayout> spinnerLayout;
    QPointer<QAnimationLabel> spinner;

    QPointer<QUdpSocket> udpSocket;
    QPointer<QTimer> timer;

    enum xPLTypeMessage { cmnd, stat };
    int xPLPort;
    Params params;

    void BindxPLPort();
    void SendxPLHeartBeat();
    void SendxPLHeartBeatEnd();
    void SendxPLSensorRequest();
    void SendxPLMessage(xPLTypeMessage type, QString target, QString body);

private slots:
    void on_buttonRefresh_clicked();
    void timerDone();
    void onReadyRead();
};

#endif // DIALOGDETECTXPL_H
