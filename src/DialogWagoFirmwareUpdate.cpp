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

#include "DialogWagoFirmwareUpdate.h"
#include "ui_DialogWagoFirmwareUpdate.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>

const QString DialogWagoFirmwareUpdate::wago_ftp_login = "admin";
const QString DialogWagoFirmwareUpdate::wago_ftp_password = "wago";

DialogWagoFirmwareUpdate::DialogWagoFirmwareUpdate(QString whost, QString wtype, QString wversion, QString _prgFile, QString _chkFile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWagoFirmwareUpdate),
    ftp(NULL),
    wago_host(whost),
    wago_type(wtype),
    wago_version(wversion),
    prgFile(_prgFile),
    chkFile(_chkFile)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ui->progressBar->setValue(0);

    spinner = new QAnimationLabel(":/img/loader.gif", this);
    ui->vbox_display->addWidget(spinner, 0, Qt::AlignCenter);
}

DialogWagoFirmwareUpdate::~DialogWagoFirmwareUpdate()
{
    if (ftp) delete ftp;
    delete spinner;
    delete ui;
}

void DialogWagoFirmwareUpdate::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogWagoFirmwareUpdate::on_buttonStart_clicked()
{
    ui->buttonStart->setDisabled(true);
    ui->buttonBox->setDisabled(true);

    /* Start the FTP upload process */
    ftp = new QFtp(this);

    connect(ftp, &QFtp::dataTransferProgress,
            this, &DialogWagoFirmwareUpdate::updateDataTransferProgress);
    connect(ftp, &QFtp::commandFinished,
            this, &DialogWagoFirmwareUpdate::onCommandFinished);
    connect(ftp, &QFtp::commandStarted,
            this, &DialogWagoFirmwareUpdate::onCommandStarted);

    cmd_connect = ftp->connectToHost(wago_host);
    cmd_login = ftp->login(wago_ftp_login, wago_ftp_password);
    QString dir = "PLC";
    cmd_cd = ftp->cd(dir);

    if (prgFile.isEmpty())
    {
        if (wago_type == "750-841")
        {
            file_chk = new QFile(":/wago_firmwares/750-841/DEFAULT.CHK");
            file_prg = new QFile(":/wago_firmwares/750-841/DEFAULT.PRG");
        }
        else if (wago_type == "750-849")
        {
            file_chk = new QFile(":/wago_firmwares/750-849/DEFAULT.CHK");
            file_prg = new QFile(":/wago_firmwares/750-849/DEFAULT.PRG");
        }
        else if (wago_type == "750-881")
        {
            file_chk = new QFile(":/wago_firmwares/750-881/DEFAULT.CHK");
            file_prg = new QFile(":/wago_firmwares/750-881/DEFAULT.PRG");
        }
    }
    else
    {
        file_chk = new QFile(chkFile);
        file_prg = new QFile(prgFile);
    }

    QString fname_chk = "DEFAULT.CHK";
    QString fname_prg = "DEFAULT.PRG";
    cmd_file_chk = ftp->put(file_chk, fname_chk);
    cmd_file_prg = ftp->put(file_prg, fname_prg);

    QString fname_rm = "persist.dat";
    cmd_del = ftp->remove(fname_rm);

    spinner->start();
}

void DialogWagoFirmwareUpdate::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void DialogWagoFirmwareUpdate::updateDataTransferProgress(qint64 done, qint64 total)
{
    ui->progressBar->setValue(done * 100 / total);
}

void DialogWagoFirmwareUpdate::onCommandStarted(int id)
{
    if (id == cmd_connect)
    {
        QString fmt = QString::fromUtf8("Connection: %p%");
        ui->progressBar->setFormat(fmt);
    }
    else if (id == cmd_login)
    {
        QString fmt = QString::fromUtf8("Authentification: %p%");
        ui->progressBar->setFormat(fmt);
    }
    else if (id == cmd_cd)
    {
        QString fmt = QString::fromUtf8("Sélection du contexte: %p%");
        ui->progressBar->setFormat(fmt);
    }
    else if (id == cmd_file_chk || id == cmd_file_prg)
    {
        QString fmt = QString::fromUtf8("Mise à jour: %p%");
        ui->progressBar->setFormat(fmt);
    }
    else if (id == cmd_del)
    {
        QString fmt = QString::fromUtf8("Nettoyage: %p%");
        ui->progressBar->setFormat(fmt);
    }
}

void DialogWagoFirmwareUpdate::onCommandFinished(int id, bool error)
{
    if (id == cmd_connect)
    {
        if (error)
        {
            QMessageBox::warning(this, QString::fromUtf8("Calaos Installer"),
                                 QString::fromUtf8("Erreur lors de la connexion à l'automate !"));

            spinner->stop();
            ui->buttonStart->setDisabled(false);
            ui->buttonBox->setDisabled(false);
            ftp->clearPendingCommands();
            ftp->deleteLater();
            ftp = NULL;
        }
    }
    else if (id == cmd_login)
    {
        if (error)
        {
            QMessageBox::warning(this, QString::fromUtf8("Calaos Installer"),
                                 QString::fromUtf8("Erreur lors de l'authentification à l'automate !"));

            spinner->stop();
            ui->buttonStart->setDisabled(false);
            ui->buttonBox->setDisabled(false);
            ftp->clearPendingCommands();
            ftp->deleteLater();
            ftp = NULL;
        }
    }
    else if (id == cmd_cd)
    {
        if (error)
        {
            QMessageBox::warning(this, QString::fromUtf8("Calaos Installer"),
                                 QString::fromUtf8("Erreur lors de contexte !"));

            spinner->stop();
            ui->buttonStart->setDisabled(false);
            ui->buttonBox->setDisabled(false);
            ftp->clearPendingCommands();
            ftp->deleteLater();
            ftp = NULL;
        }
    }
    else if (id == cmd_file_chk || id == cmd_file_prg)
    {
        if (error)
        {
            QMessageBox::warning(this, QString::fromUtf8("Calaos Installer"),
                                 QString::fromUtf8("Erreur lors de la mise à jour de l'automate !%1\n\nAttention, l'automate peut ne plus fonctionner maintenant, veuillez relancer le processus de mis à jour.").arg(ftp->errorString()));

            spinner->stop();
            ui->buttonStart->setDisabled(false);
            ui->buttonBox->setDisabled(false);
            ftp->clearPendingCommands();
            ftp->deleteLater();
            ftp = NULL;
        }
    }
    else if (id == cmd_del)
    {
        QString fmt = QString::fromUtf8("Redémarrage en cours...");
        ui->progressBar->setFormat(fmt);
        ui->progressBar->setValue(100);

        ftp->close();
        ftp->deleteLater();
        ftp = NULL;

        emit updateFirmwareDone();

        QTimer::singleShot(15000, this, SLOT(resetDone()));
    }
}

void DialogWagoFirmwareUpdate::resetDone()
{
    QString fmt = QString::fromUtf8("Mise à jour réussie.");
    ui->progressBar->setFormat(fmt);
    ui->progressBar->setValue(0);
    spinner->stop();
    ui->buttonBox->setDisabled(false);
}
