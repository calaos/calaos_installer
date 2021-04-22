#include <QThread>
#include <QFileDialog>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrentRun>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "StorageDelegate.h"

#if defined Q_OS_WIN
#include <windows.h>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow),
    calaosApi(new CalaosApi(nullptr, this))
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(calaosApi, &CalaosApi::downloadProgress, this, &MainWindow::downloadProgress);

    ui->stackedWidget->setCurrentIndex(0);

    imageModel = new CalaosImageModel(this);
    filterImageModel = new FilterImageModel(this);
    filterImageModel->setSourceModel(imageModel);

    ui->versionCombo->setModel(filterImageModel);

    platform = new Platform(this);

    ui->listViewStorage->setModel(platform->getDiskModel());
    ui->listViewStorage->setItemDelegate(new StorageDelegate(ui->listViewStorage));
    ui->listViewStorage->setCurrentIndex(platform->getDiskModel()->lastIndex());

    calaosApi->loadImages([=](bool success, const QJsonArray &jarr)
    {
        if (!success)
        {
            QMessageBox::warning(this, tr("Error"), tr("Failed to download image list from calaos server"));
            return;
        }

        imageModel->loadJson(jarr);
        ui->machineCombo->clear();
        ui->machineCombo->addItems(imageModel->getMachines());
        ui->machineCombo->setCurrentIndex(0);
        filterImageModel->sort(0);
    });

    diskWriter = new DiskWriter(this);
    connect(diskWriter, &DiskWriter::progress, this, &MainWindow::writeProgress);
    connect(diskWriter, &DiskWriter::error, this, &MainWindow::writeError);
    connect(diskWriter, &DiskWriter::finished, this, &MainWindow::writeFinished);
    connect(diskWriter, &DiskWriter::syncing, this, &MainWindow::writeSync);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_machineCombo_currentIndexChanged(const QString &)
{
    filterImageModel->setFilterMachine(ui->machineCombo->currentText());
    ui->versionCombo->setCurrentIndex(0);
}

void MainWindow::on_radioButtonStable_clicked()
{
    filterImageModel->setFilterType(true);
    ui->versionCombo->setCurrentIndex(0);
}

void MainWindow::on_radioButtonAll_clicked()
{
    filterImageModel->setFilterType(false);
    ui->versionCombo->setCurrentIndex(0);
}

void MainWindow::on_continueButton_clicked()
{
    if (ui->stackedWidget->currentIndex() == MainWindow::Page_Start)
    {
        // File is not present on disk => donwload it
        if (!m_bFileFromDisk)
        {
            ui->stackedWidget->setCurrentIndex(MainWindow::Page_Download);

            auto im = imageModel->itemAt(filterImageModel->indexToSource(
                                             filterImageModel->index(ui->versionCombo->currentIndex(), 0)).row()
                                         );

            qDebug() << "Downloading " << im->get_url();
            startDownloadTime = QDateTime::currentDateTime();
            downloadTime.start();

            ui->downloadProgress->setMinimum(0);
            ui->downloadProgress->setMaximum(0);
            ui->downloadProgress->setValue(0);
            ui->timeDlLabel->setText(tr(""));
            ui->speedDlLabel->setText(tr(""));
            ui->downloadLabel->setText(tr("Checking download..."));

            ui->continueButton->hide();
            downloadReq = calaosApi->downloadImage(im->get_url(), im->get_checksum(), [=](bool success, QString filename)
            {
                if (!success)
                {
                    QMessageBox::warning(this, tr("Error"), tr("Failed to download image file"));

                    ui->downloadProgress->setMinimum(0);
                    ui->downloadProgress->setMaximum(0);
                    ui->downloadProgress->setValue(-1);
                    ui->timeDlLabel->setText(tr(""));
                    ui->speedDlLabel->setText(tr(""));
                    ui->downloadLabel->setText(tr("Download failed!"));

                    return;
                }

                qDebug() << "File downloaded to " << filename;
                m_decompressedFile = filename;
                ui->stackedWidget->setCurrentIndex(MainWindow::Page_Partition);
                ui->continueButton->show();
            });
        }
        // File is present on disk => flash it
        else
        {
            ui->stackedWidget->setCurrentIndex(MainWindow::Page_Partition);
        }

    }
    else if (ui->stackedWidget->currentIndex() == MainWindow::Page_Download)
    {
        ui->stackedWidget->setCurrentIndex(MainWindow::Page_Partition);
    }
    else if(ui->stackedWidget->currentIndex() == MainWindow::Page_Partition)
    {
        m_disk = platform->getDiskModel()->itemAt(ui->listViewStorage->currentIndex().row());
        if (!m_disk)
        {
            QMessageBox::warning(this, "Calaos", tr("No disk selected"));
            return;
        }

        ui->stackedWidget->setCurrentIndex(MainWindow::Page_Writing);
        ui->continueButton->hide();
        startWriteProcess();
    }
    else if(ui->stackedWidget->currentIndex() == MainWindow::Page_Final)
    {
        accept();
    }
}

void MainWindow::on_selectImageButton_clicked()
{
    QString downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (downloadsFolder.isEmpty())
        downloadsFolder = QDir::homePath();

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Calaos Image"),
                                                    downloadsFolder);
    if (!fileName.isEmpty())
    {
        ui->imageFilenameLabel->setText(fileName);
        m_decompressedFile = fileName;
        m_bFileFromDisk = true;
    }
}

void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0)
    {
        ui->downloadProgress->setMinimum (0);
        ui->downloadProgress->setMaximum (100);
        ui->downloadProgress->setValue((bytesReceived * 100) / bytesTotal);

        ui->downloadLabel->setText(tr("Downloading %1 of %2")
                                   .arg(Utils::sizeHuman(bytesReceived), Utils::sizeHuman(bytesTotal)));

        //calculate remaining time
        QString remain = Utils::calculateTimeRemaining(startDownloadTime, bytesReceived, bytesTotal);
        ui->timeDlLabel->setText(tr("Time remaining: %1").arg(remain));
        double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
        ui->speedDlLabel->setText(tr("Speed: %1/s").arg(Utils::sizeHuman(speed)));
    }
    else
    {
        ui->downloadProgress->setMinimum(0);
        ui->downloadProgress->setMaximum(0);
        ui->downloadProgress->setValue(-1);
        ui->timeDlLabel->setText(tr("Time remaining: Unknown"));
        ui->speedDlLabel->setText(tr("Speed: Unknown"));
    }
}

void MainWindow::writeFinished()
{
    ui->stackedWidget->setCurrentIndex(MainWindow::Page_Final);
    ui->finalLabelIcon->setPixmap(QPixmap(":/img/dialog-ok.png"));

    ui->continueButton->show();
    ui->cancelButton->hide();
    ui->continueButton->setText(tr("Close"));
}

void MainWindow::writeSync()
{
    ui->writeProgress->setMinimum (0);
    ui->writeProgress->setMaximum (0);
    ui->writeProgress->setValue(-1);
    ui->writeLabel->setText(tr("Syncing..."));
    ui->timeWriteLabel->setText(tr("Time remaining: Unknown"));
    ui->speedWriteLabel->setText(tr("Speed: Unknown"));
}

void MainWindow::writeError(QString error)
{
    ui->stackedWidget->setCurrentIndex(MainWindow::Page_Final);
    ui->finalLabelTitle->setText(tr("Failed to write image !"));
    ui->finalLabel->setText(error);
    ui->finalLabelIcon->setPixmap(QPixmap(":/img/dialog-cancel.png"));

    ui->continueButton->show();
    ui->cancelButton->hide();
    ui->continueButton->setText(tr("Close"));
}

void MainWindow::writeProgress(QString status, qint64 bytesReceived, qint64 bytesTotal, qint64 elapsedMs)
{
    if (bytesTotal > 0)
    {
        ui->writeProgress->setMinimum (0);
        ui->writeProgress->setMaximum (100);
        ui->writeProgress->setValue((bytesReceived * 100) / bytesTotal);

        ui->writeLabel->setText(tr("%1 %2 of %3")
                                   .arg(status, Utils::sizeHuman(bytesReceived), Utils::sizeHuman(bytesTotal)));

        //calculate remaining time
        QString remain = Utils::calculateTimeRemaining(startDownloadTime, bytesReceived, bytesTotal);
        ui->timeWriteLabel->setText(tr("Time remaining: %1").arg(remain));
        double speed = bytesReceived * 1000.0 / elapsedMs;
        ui->speedWriteLabel->setText(tr("Speed: %1/s").arg(Utils::sizeHuman(speed)));
    }
    else
    {
        ui->writeProgress->setMinimum(0);
        ui->writeProgress->setMaximum(0);
        ui->writeProgress->setValue(-1);
        ui->writeLabel->setText(status);
        ui->timeWriteLabel->setText(tr("Time remaining: Unknown"));
        ui->speedWriteLabel->setText(tr("Speed: Unknown"));
    }
}

void MainWindow::startWriteProcess()
{
    startDownloadTime = QDateTime::currentDateTime();

    QtConcurrent::run([=]()
    {
        diskWriter->writeToRemovableDevice(m_decompressedFile, m_disk);
    });
}

void MainWindow::on_cancelButton_clicked()
{
    if (ui->stackedWidget->currentIndex() == MainWindow::Page_Download && downloadReq)
    {
        if (downloadReq && QMessageBox::question(this, "Calaos", tr("Cancel download ?")) == QMessageBox::Yes)
        {
            downloadReq->cancel();
            downloadReq = nullptr;
        }
        return;
    }
    else if (ui->stackedWidget->currentIndex() == MainWindow::Page_Writing)
    {
        if (QMessageBox::question(this, "Calaos", tr("Cancel write ?")) == QMessageBox::Yes)
            diskWriter->cancelWrite();
        return;
    }

    reject();
}
