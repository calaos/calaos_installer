#ifndef DIALOGSAVEONLINE_H
#define DIALOGSAVEONLINE_H

#include <QtNetwork>
#include <QtGui>
#include <qanimationlabel.h>
#include <formpost.h>

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
                QNetworkAccessManager networkAccess;

                QNetworkReply *reply_io;
                QNetworkReply *reply_rules;

                QString currentDir, currentIP;
                FormPost formPost;

                void loadFromNetwork();
                void uploadXmlFiles(QString ip);

        private slots:
                void on_buttonBox_rejected();
                void on_calaosfrCheck_stateChanged(int);
                void on_buttonBox_accepted();

                void uploadFinished(QByteArray &reply);
                void downloadFinishedCalaosFr(QNetworkReply *reply);
                void downloadFinishedRestart(QNetworkReply *reply);
                void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
};

#endif // DIALOGSAVEONLINE_H
