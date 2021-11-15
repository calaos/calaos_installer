#ifndef DIALOGNEWCAMERA_H
#define DIALOGNEWCAMERA_H

#include <QtWidgets>
#include <QDialog>
#include <iostream>
#include <QNetworkAccessManager>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
class DialogNewCamera;
}

class DialogNewCamera : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogNewCamera)
public:
    explicit DialogNewCamera(Room *room, QWidget *parent = nullptr);
    virtual ~DialogNewCamera();

    IOBase *getOutput() { return output; }

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogNewCamera *ui;
    IOBase *output = nullptr;
    Room *room = nullptr;

    QNetworkAccessManager *nm = nullptr;
    bool isRunning = false;
    QString authUrl;
    QString apiSid;
    QString listUrl;

    QJsonObject parseResult(const QString &data, bool &error, QString &errStr);
    void getApiInfo(const QString &api, const QString &method, const QString &version,
                    std::function<void(const QString &url)> cb);
    void login(std::function<void(const QString &sid)> cb);
    void doGetList();
    void tryGetList();
    void tryLogin();
    void listCameras(std::function<void(const QJsonArray &arr)> dataCb);

private slots:
    void on_buttonBox_accepted();
    void on_pushButtonConnectDsm_clicked();
};

#endif // DIALOGNEWCAMERA_H
