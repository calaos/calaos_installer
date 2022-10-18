/******************************************************************************
**  Copyright (c) 2006-2011, Calaos. All Rights Reserved.
**
**  This file is part of Calaos Home.
**
**  Calaos Home is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 3 of the License, or
**  (at your option) any later version.
**
**  Calaos Home is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Calaos; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
******************************************************************************/

#ifndef DIALOGWAGOFIRMWAREUPDATE_H
#define DIALOGWAGOFIRMWAREUPDATE_H

#include <QDialog>
#include <qanimationlabel.h>
#include "ftp/qftp.h"
#include <QFile>

namespace Ui
{
class DialogWagoFirmwareUpdate;
}

class DialogWagoFirmwareUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWagoFirmwareUpdate(QString whost, QString wtype, QString wversion, QString prgFile, QString chkFile, QWidget *parent = 0);
    ~DialogWagoFirmwareUpdate();

    static const QString wago_ftp_login;
    static const QString wago_ftp_password;

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private slots:
    void on_buttonStart_clicked();

    void updateDataTransferProgress(qint64 done, qint64 total);
    void onCommandFinished(int id, bool error);
    void onCommandStarted(int id);

    void resetDone();

private:
    Ui::DialogWagoFirmwareUpdate *ui;

    QPointer<QAnimationLabel> spinner;

    QFtp *ftp;

    QFile *file_chk;
    QFile *file_prg;

    QString wago_host;
    QString wago_type;
    QString wago_version;

    int cmd_connect;
    int cmd_login;
    int cmd_cd;
    int cmd_file_chk;
    int cmd_file_prg;
    int cmd_del;

    //For custom fw files
    QString prgFile;
    QString chkFile;

signals:
    void updateFirmwareDone();
};

#endif // DIALOGWAGOFIRMWAREUPDATE_H
