#ifndef DIALOGSAVEONLINE_H
#define DIALOGSAVEONLINE_H

#include <QtNetwork>
#include <QDialog>
#include <qanimationlabel.h>
#include "ConfigOptions.h"

class OnlineSession;

#include <iostream>
using namespace std;

namespace Ui
{
class DialogSaveOnline;
}

class DialogSaveOnline : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogSaveOnline)
public:
    explicit DialogSaveOnline(QString path, QWidget *parent = 0);
    virtual ~DialogSaveOnline();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogSaveOnline *ui;

    QPointer<QAnimationLabel> spinner;
    OnlineSession *online;
    bool putting = false;

    QString currentDir, currentIP;

    QVector<CalaosAuth> auths;

    void uploadXmlFiles(QString ip);
    void probeServerFormat();
    void probeFinished(bool reached, const QByteArray &body, const QString &networkError);
    void putFiles();
    void uploadFinished(bool reached, const QByteArray &body, const QString &networkError);
    void fail(const QString &message);
    QString readFileContent(QString fileName);

private slots:
    void on_checkShowPass_toggled(bool checked);
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_pushButtonDelPref_clicked();
    void on_comboIP_currentIndexChanged(int index);
};

#endif // DIALOGSAVEONLINE_H
