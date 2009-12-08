#ifndef DIALOGOPENONLINE_H
#define DIALOGOPENONLINE_H

#include <QtNetwork>
#include <QtGui>
#include <qanimationlabel.h>

#include <iostream>
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

                QNetworkReply *reply_io;
                QNetworkReply *reply_rules;

                QString tempDir, currentIP;

                void loadFromNetwork();
                void loadXmlFiles(QString ip);

        private slots:
                void on_buttonBox_rejected();
                void on_calaosfrCheck_stateChanged(int);
                void on_buttonBox_accepted();

                void downloadFinishedCalaosFr(QNetworkReply *reply);
                void downloadFinishedFiles(QNetworkReply *reply);
                void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
};

#endif // DIALOGOPENONLINE_H
