#ifndef DIALOGOPENONLINE_H
#define DIALOGOPENONLINE_H

#include <QtNetwork>
#include <QDialog>
#include <qanimationlabel.h>
#include "ConfigOptions.h"

using namespace std;

namespace Ui
{
class DialogOpenOnline;
}

class DialogOpenOnline : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogOpenOnline)
public:
    explicit DialogOpenOnline(QString tempDir, QWidget *parent = 0);
    virtual ~DialogOpenOnline();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogOpenOnline *ui;

    QPointer<QAnimationLabel> spinner;
    QNetworkAccessManager networkAccess;

    QString tempDir, currentIP;

    QVector<CalaosAuth> auths;

    void loadXmlFiles(QString ip);
    void saveXMLFile(QString filename, QString data);

private slots:
    void on_checkShowPass_toggled(bool checked);
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();

    void downloadFinishedFiles(QNetworkReply *reply);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void on_pushButtonDelPref_clicked();
    void on_comboIP_currentIndexChanged(int index);
};

#endif // DIALOGOPENONLINE_H
