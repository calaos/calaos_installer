#ifndef WAGOUPLOADER_H
#define WAGOUPLOADER_H

#include <QtGui>
#include <wagoconnect.h>
#include <ListeRoom.h>
#include <ListeRule.h>

using namespace Calaos;
using namespace std;

enum { WAGOST_NONE = 0, WAGOST_START, WAGOST_CREATING, WAGOST_UPLOADING, WAGOST_DONE, WAGOST_ABORTED };

enum { TWAGO_NONE = 0, TWAGO_TELERUPTEUR, TWAGO_DIRECT, TWAGO_VOLET, TWAGO_VOLETIMPULSE, TWAGO_DALI,
       TWAGO_DALIGRP, TWAGO_TELERUPTEUR_KNX, TWAGO_DIRECT_KNX };

class WagoRuleCommand
{
        public:

                WagoRuleCommand(): input(0), type(TWAGO_NONE),
                               addr1(0), addr2(0),
                               sameas(-1), type_sent(false)
                { }

        int input;
        int type;
        int addr1;
        int addr2;
        int sameas;
        bool type_sent;
};

class WagoUploader : public QObject
{
        Q_OBJECT

        public:
                WagoUploader(QObject *parent = NULL);
                ~WagoUploader();

        public slots:
                void startUpload(QString ip);
                void stopUpload();

        signals:
                /* Signal emited when status of uploading updates */
                void statusUpdate(int status);

                /* update progess for uploading 0-100 */
                void progressUpdate(int percent);

        private:
                bool quit;
                QList<WagoRuleCommand> commands;
                int upload_num;
                QTimer *timer;
                QString wago_ip;

                void createConfig();
                void uploadConfig();

        private slots:
                void timerUpload();
};

#endif // WAGOUPLOADER_H
