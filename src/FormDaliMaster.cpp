#include "FormDaliMaster.h"
#include "ui_FormDaliMaster.h"
#include <QMessageBox>
#include "DialogDaliMasterItem.h"
#include "DialogWagoFirmwareUpdate.h"
#include "wagoconnect.h"
#include <QDebug>
#include <QFileDialog>

#define CALAOS_CSV_FILE     "calaos_dali_master.csv"

FormDaliMaster::FormDaliMaster(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDaliMaster)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    connect(ui->pushButtonQuit, &QPushButton::clicked, this, &FormDaliMaster::closeDaliForm);

    ui->treeWidget->setHeaderLabels(QStringList() <<
                                    tr("Device Type") <<
                                    tr("Addresses") <<
                                    tr("Off Timeout"));
}

FormDaliMaster::~FormDaliMaster()
{
    if (ftp) delete ftp;
    delete ui;
}

void FormDaliMaster::on_pushButtonAdd_clicked()
{
    DialogDaliMasterItem d(ui->treeWidget);
    d.exec();
    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
}

void FormDaliMaster::on_pushButtonEdit_clicked()
{
    if (ui->treeWidget->currentItem())
    {
        DialogDaliMasterItem d(ui->treeWidget, ui->treeWidget->currentItem());
        d.exec();
        ui->treeWidget->resizeColumnToContents(0);
        ui->treeWidget->resizeColumnToContents(1);
        ui->treeWidget->resizeColumnToContents(2);
    }
}

void FormDaliMaster::on_pushButtonDel_clicked()
{
    if (ui->treeWidget->currentItem())
    {
        if (QMessageBox::question(this,
                                  tr("Calaos Installer"),
                                  tr("Do you want to remove selected item?"),
                                  QMessageBox::Yes,
                                  QMessageBox::No) == QMessageBox::Yes)
        {
            delete ui->treeWidget->currentItem();
        }
    }
}

void FormDaliMaster::loadCsv(QStringList csvFile)
{
    ui->treeWidget->clear();

    foreach (const QString &line, csvFile)
    {
        QStringList cols = line.split(';');

        if (cols.at(0) == "DaliEcg" && cols.count() >= 2)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            DialogDaliMasterItem::updateItem(item, cols.at(0), cols.at(1).toInt());
        }
        else if (cols.at(0) == "DaliGroup" && cols.count() >= 2)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            DialogDaliMasterItem::updateItem(item, cols.at(0), cols.at(1).toInt());
        }
        else if (cols.at(0) == "MultiSensorType1" && cols.count() >= 4)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            DialogDaliMasterItem::updateItem(item, cols.at(0), cols.at(1).toInt(), cols.at(2).toInt(), cols.at(3).toInt());
        }
        else if (cols.at(0) == "PushbuttonSensorType1" && cols.count() >= 5)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            DialogDaliMasterItem::updateItem(item, cols.at(0), cols.at(1).toInt(), cols.at(2).toInt(), cols.at(3).toInt(), cols.at(4).toInt());
        }
        else if (cols.at(0) == "MultiSensorType2" && cols.count() >= 5)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            DialogDaliMasterItem::updateItem(item, cols.at(0), cols.at(1).toInt(), cols.at(2).toInt(), cols.at(3).toInt(), cols.at(4).toInt());
        }
        else if (cols.at(0) == "PushbuttonSensorType2" && cols.count() >= 2)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            DialogDaliMasterItem::updateItem(item, cols.at(0), cols.at(1).toInt());
        }
    }

    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
}

void FormDaliMaster::writeCsv()
{
    for (int i = 0;i < ui->treeWidget->topLevelItemCount();i++)
    {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);

        QString t = item->data(0, DialogDaliMasterItem::RoleType).toString();
        QString line;

        if (t == "DaliEcg" ||
            t == "DaliGroup" ||
            t == "PushbuttonSensorType2")
        {
            line = QString("%1;%2").arg(t)
                        .arg(item->data(0, DialogDaliMasterItem::RoleAddr1).toString());
        }
        else if (t == "MultiSensorType1")
        {
            line = QString("%1;%2;%3;%4").arg(t)
                        .arg(item->data(0, DialogDaliMasterItem::RoleAddr1).toString())
                        .arg(item->data(0, DialogDaliMasterItem::RoleAddr2).toString())
                        .arg(item->data(0, DialogDaliMasterItem::RoleAddr3).toString());
        }
        else if (t == "PushbuttonSensorType1" ||
                 t == "MultiSensorType2")
        {
            line = QString("%1;%2;%3;%4;%5").arg(t)
                        .arg(item->data(0, DialogDaliMasterItem::RoleAddr1).toString())
                        .arg(item->data(0, DialogDaliMasterItem::RoleAddr2).toString())
                        .arg(item->data(0, DialogDaliMasterItem::RoleAddr3).toString())
                        .arg(item->data(0, DialogDaliMasterItem::RoleAddr4).toString());
        }
        csvIoBuffer.write(line.toLocal8Bit());
        if (i < ui->treeWidget->topLevelItemCount() - 1)
            csvIoBuffer.write("\r\n");
    }
}

void FormDaliMaster::on_pushButtonLoad_clicked()
{
    if (ftp) return;

    progress = new QProgressDialog(this);
    progress->setCancelButton(nullptr);
    progress->setWindowModality(Qt::WindowModal);
    progress->setAutoClose(false);
    progress->setAutoReset(false);
    progress->show();

    /* Start the FTP upload process */
    ftp = new QFtp(this);

    connect(ftp, &QFtp::dataTransferProgress,
            this, &FormDaliMaster::updateDataTransferProgress);
    connect(ftp, &QFtp::commandFinished,
            this, &FormDaliMaster::onCommandFinished);
    connect(ftp, &QFtp::commandStarted,
            this, &FormDaliMaster::onCommandStarted);

    cmd_connect = ftp->connectToHost(ConfigOptions::Instance().getWagoHost());
    cmd_login = ftp->login(DialogWagoFirmwareUpdate::wago_ftp_login, DialogWagoFirmwareUpdate::wago_ftp_password);
    QString dir = "PLC";
    cmd_cd = ftp->cd(dir);

    csvData.clear();
    csvIoBuffer.close();
    csvIoBuffer.setBuffer(&csvData);
    csvIoBuffer.open(QBuffer::ReadWrite);

    cmd_file_get = ftp->get(CALAOS_CSV_FILE, &csvIoBuffer);
}

void FormDaliMaster::on_pushButtonSend_clicked()
{
    if (ftp) return;

    /* Start the FTP upload process */
    ftp = new QFtp(this);

    progress = new QProgressDialog(this);
    progress->setCancelButton(nullptr);
    progress->setWindowModality(Qt::WindowModal);
    progress->setAutoClose(false);
    progress->setAutoReset(false);
    progress->show();

    connect(ftp, &QFtp::dataTransferProgress,
            this, &FormDaliMaster::updateDataTransferProgress);
    connect(ftp, &QFtp::commandFinished,
            this, &FormDaliMaster::onCommandFinished);
    connect(ftp, &QFtp::commandStarted,
            this, &FormDaliMaster::onCommandStarted);

    cmd_connect = ftp->connectToHost(ConfigOptions::Instance().getWagoHost());
    cmd_login = ftp->login(DialogWagoFirmwareUpdate::wago_ftp_login, DialogWagoFirmwareUpdate::wago_ftp_password);
    QString dir = "PLC";
    cmd_cd = ftp->cd(dir);

    csvData.clear();
    csvIoBuffer.close();
    csvIoBuffer.setBuffer(&csvData);
    csvIoBuffer.open(QBuffer::ReadWrite);

    writeCsv();
    csvIoBuffer.close();

    cmd_file_set = ftp->put(csvData, CALAOS_CSV_FILE);
}

void FormDaliMaster::updateDataTransferProgress(qint64 done, qint64 total)
{
    if (!progress) return;

    if (total <= 0) total = 1;
    progress->setValue(done * 100 / total);
}

void FormDaliMaster::onCommandStarted(int id)
{
    if (!progress) return;

    if (id == cmd_connect)
        progress->setLabelText(tr("Connecting to PLC..."));
    else if (id == cmd_login)
        progress->setLabelText(tr("Login into PLC..."));
    else if (id == cmd_cd)
        progress->setLabelText(tr("Selecting context..."));
    else if (id == cmd_file_get)
        progress->setLabelText(tr("Downloading configuration from PLC..."));
    else if (id == cmd_file_set)
        progress->setLabelText(tr("Uploading configuration to PLC..."));
}

void FormDaliMaster::onCommandFinished(int id, bool error)
{
    if (id == cmd_connect)
    {
        if (error)
        {
            QMessageBox::warning(this, tr("Calaos Installer"),
                                 tr("Error connecting to PLC"));
            ftp->clearPendingCommands();
            ftp->deleteLater();
            ftp = nullptr;
            progress->hide();
            progress->deleteLater();
            progress = nullptr;
        }
        qDebug() << "FTP connect ok";
    }
    else if (id == cmd_login)
    {
        if (error)
        {
            QMessageBox::warning(this, tr("Calaos Installer"),
                                 tr("Failed to login into PLC"));
            ftp->clearPendingCommands();
            ftp->deleteLater();
            ftp = nullptr;
            progress->hide();
            progress->deleteLater();
            progress = nullptr;
        }
        qDebug() << "FTP login ok";
    }
    else if (id == cmd_cd)
    {
        if (error)
        {
            QMessageBox::warning(this, tr("Calaos Installer"),
                                 tr("Failed to switch context on PLC"));
            ftp->clearPendingCommands();
            ftp->deleteLater();
            ftp = nullptr;
            progress->hide();
            progress->deleteLater();
            progress = nullptr;
        }
        qDebug() << "FTP cd ok";
    }
    else if (id == cmd_file_get)
    {
        if (!error)
        {
            csvIoBuffer.reset();
            QStringList csv;
            while (!csvIoBuffer.atEnd())
            {
                QByteArray line = csvIoBuffer.readLine();
                QString fline(line);
                csv << fline;
            }
            loadCsv(csv);
        }

        qDebug() << "FTP file get ok";

        ftp->clearPendingCommands();
        ftp->deleteLater();
        ftp = nullptr;
        progress->hide();
        progress->deleteLater();
        progress = nullptr;
    }
    else if (id == cmd_file_set)
    {
        if (error)
        {
            QMessageBox::warning(this, tr("Calaos Installer"),
                                 tr("Failed to send configuration to PLC"));
            ftp->clearPendingCommands();
            ftp->deleteLater();
            ftp = nullptr;
            progress->hide();
            progress->deleteLater();
            progress = nullptr;
        }
        else
        {
            qDebug() << "FTP file put ok";
            progress->setLabelText(tr("Rebooting PLC... Please wait."));
            WagoConnect::Instance().ResetWago();
            QTimer::singleShot(15000, this, SLOT(resetDone()));
        }
    }
}

void FormDaliMaster::resetDone()
{
    qDebug() << "Reset done, reconnecting...";
    WagoConnect::Instance().Connect(ConfigOptions::Instance().getWagoHost());

    ftp->clearPendingCommands();
    ftp->deleteLater();
    ftp = nullptr;
    progress->hide();
    progress->deleteLater();
    progress = nullptr;
}

void FormDaliMaster::on_pushButtonLoadFile_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, tr("Open wago DALI Master file"), CALAOS_CSV_FILE, "CSV File (*.csv)");
    if (fname.isEmpty())
        return;

    QFile file(fname);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this, "Calaos Installer",
                             tr("Unable to open file %1 with read access").arg(fname));
        return;
    }

    QStringList csv;
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString fline(line);
        csv << fline;
    }
    file.close();
    loadCsv(csv);
}

void FormDaliMaster::on_pushButtonExportFile_clicked()
{
    QString fname = QFileDialog::getSaveFileName(this, tr("Save to wago DALI Master file"), CALAOS_CSV_FILE, "CSV File (*.csv)");
    if (fname.isEmpty())
        return;

    QFile file(fname);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(this, "Calaos Installer",
                             tr("Unable to open file %1 with write access").arg(fname));
        return;
    }

    csvData.clear();
    csvIoBuffer.close();
    csvIoBuffer.setBuffer(&csvData);
    csvIoBuffer.open(QBuffer::ReadWrite);

    writeCsv();
    csvIoBuffer.close();

    file.write(csvData);
    file.close();
}
