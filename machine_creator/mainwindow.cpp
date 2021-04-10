#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

#if defined Q_OS_WIN
#include <windows.h>
#endif

#include "DiskWriter_unix.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow),
    calaosApi(new CalaosApi(nullptr, this))
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    QStringList names;
    QStringList friendlyNames;
#if defined Q_OS_MAC
    QProcess lsblk;
    lsblk.start("diskutil list", QIODevice::ReadOnly);
    lsblk.waitForStarted();
    lsblk.waitForFinished();

    QString device = lsblk.readLine();
    while (!lsblk.atEnd()) {
        device = device.trimmed(); // Odd trailing whitespace

        if (device.startsWith("/dev/disk")) {
            QString name = device.split(QRegExp("\\s+")).first();
            names << name;
        }
        device = lsblk.readLine();

    }
#elif defined Q_OS_UNIX

    QDir currentDir("/sys/block");
    currentDir.setFilter(QDir::Dirs);

    QStringList entries = currentDir.entryList();
    foreach (QString device, entries) {
        // Skip "." and ".." dir entries
        if (device == "." || device == "..") {
            continue;
        }

        if (device.startsWith("mmcblk") || device.startsWith("sd")) {
            names << device;
        }
    }

#elif defined Q_OS_WIN
    WCHAR *szDriveLetters;
    WCHAR szDriveInformation[1024];

    GetLogicalDriveStrings(1024, szDriveInformation);

    szDriveLetters = szDriveInformation;
    while (*szDriveLetters != '\0') {
        if (GetDriveType(szDriveLetters) == DRIVE_REMOVABLE) {
            names << QString::fromWCharArray(szDriveLetters);
        }
        szDriveLetters = &szDriveLetters[wcslen(szDriveLetters) + 1];
    }
#endif

    friendlyNames = getUserFriendlyNames(names);
    ui->targetCombo->clear();
    for (int i = 0; i < names.size(); ++i)
    {
        ui->targetCombo->addItem(friendlyNames[i] + " " + names[i], names[i]);
    }
    ui->targetCombo->setCurrentIndex(ui->targetCombo->count() - 1);

    calaosApi->loadImages([=](bool success, const QJsonArray &jarr)
    {
        qDebug() << jarr.size();

        foreach (const QJsonValue & value, jarr)
        {
            QJsonObject obj = value.toObject();
            CalaosImage *im = new CalaosImage();
            im->type = obj["type"].toString();
            im->url = obj["url"].toString();
            im->version = obj["version"].toString();
            images[obj["machine"].toString()].append(im);
        }

        ui->machineCombo->clear();
        for(auto e : images.keys())
        {
            ui->machineCombo->addItem(e, e);
        }
        ui->machineCombo->setCurrentIndex(ui->machineCombo->count() - 1);
    });

    diskWriter = new DiskWriter_unix();

    diskWriterThread = new QThread(this);
    diskWriter->moveToThread(diskWriterThread);
    connect(diskWriterThread, SIGNAL(finished()), diskWriter, SLOT(deleteLater()));
    connect(this, SIGNAL(proceedToWriteImageToDevice(QString,QString)),
            diskWriter, SLOT(writeCompressedImageToRemovableDevice(QString,QString)));
    connect(diskWriter, SIGNAL(bytesWritten(int)), ui->writeProgress, SLOT(setValue(int)));
    connect(diskWriter, SIGNAL(finished()), this, SLOT(writingFinished()));
    connect(diskWriter, SIGNAL(error(QString)), this, SLOT(writingError(QString)));
    diskWriterThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateVersions(const QString &arg1)
{
    ui->versionCombo->clear();
    for(auto it : images[arg1])
    {
        if (ui->radioButton_2->isChecked() && it->type == "stable")
            ui->versionCombo->addItem(it->version, it->version);
        else if (ui->radioButton->isChecked())
            ui->versionCombo->addItem(it->version, it->version);
    }
    ui->versionCombo->setCurrentIndex(ui->versionCombo->count() - 1);
}

void MainWindow::on_machineCombo_currentIndexChanged(const QString &arg1)
{
    updateVersions(arg1);
}


void MainWindow::on_radioButton_2_clicked()
{
    updateVersions(ui->machineCombo->currentText());
}

void MainWindow::on_radioButton_clicked()
{
    updateVersions(ui->machineCombo->currentText());
}

void MainWindow::on_continueButton_clicked()
{
    if (ui->stackedWidget->currentIndex() == MainWindow::Page_Start)
    {
        // File is not present on disk => donwload it
        if (!m_bFileOnDisk)
        {
            ui->stackedWidget->setCurrentIndex(MainWindow::Page_Download);
            for (auto it : images[ui->machineCombo->currentText()])
            {
                if (it->version == ui->versionCombo->currentText())
                {
                    manager = new QNetworkAccessManager(this);
                    QUrl url = it->url;
                    qDebug() << "Downloading url " << it->url;
                    QFileInfo fileInfo(url.path());

                    QString downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
                    if (downloadsFolder.isEmpty())
                        downloadsFolder = QDir::homePath();

                    QString fileName = downloadsFolder + "/" + fileInfo.fileName();
                    fileName.remove(".xz");
                    if (QFile::exists(fileName))
                    {
                        if (QMessageBox::question(this, tr("Image download"),
                                                  tr("There already exists a file called %1 in "
                                                     "the current directory. Overwrite?").arg(fileName),
                                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
                                == QMessageBox::No)
                            return;
                        QFile::remove(fileName);
                    }
                    path = fileName;
                    qDebug() << "Downloading to " << fileName;
                    //ui->statusLabel->setText(tr("Downloading %1.").arg(fileName));
                    ui->continueButton->setEnabled(false);
                    m_decompressedFile = fileName;

                    reply = manager->get(QNetworkRequest(url));
                    connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesRead, qint64 totalBytes){
                        ui->downloadProgress->setMaximum(totalBytes);
                        ui->downloadProgress->setValue(bytesRead);
                    });

                    connect(reply, &QNetworkReply::finished, [=]{
                        ui->imageFilenameLabel->setText(fileName);
                        ui->continueButton->setEnabled(true);
                    });

                    memset(&strm, 0, sizeof(strm));
                    int ret_xz = LZMA.lzma_stream_decoder (&strm, UINT64_MAX, LZMA_TELL_UNSUPPORTED_CHECK | LZMA_CONCATENATED);
                    if (ret_xz == LZMA_OK)
                    {
                        connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadNewData(qint64,qint64)));
                        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
                    }
                }
            }
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
        QFileInfo f(m_decompressedFile);

        qDebug() << "Write Image " << m_decompressedFile << " on " << ui->targetCombo->currentData().toString();
        ui->writeProgress->setValue(0);
        ui->writeProgress->setMaximum(f.size());
        emit proceedToWriteImageToDevice(m_decompressedFile, ui->targetCombo->currentData().toString());
        ui->stackedWidget->setCurrentIndex(MainWindow::Page_Writting);
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Calaos Image"), downloadsFolder);
    ui->imageFilenameLabel->setText(fileName);
    m_decompressedFile = fileName;
    m_bFileOnDisk = true;
}


QStringList MainWindow::getUserFriendlyNames(const QStringList &devices) const
{
    QStringList returnList;

    foreach (QString s, devices) {
#ifdef Q_OS_LINUX
        bool ok;
        QFile file("/sys/block/" + s +"/size");
        file.open(QIODevice::ReadOnly);
        QString str = file.readAll();
        quint64 size = str.toULongLong(&ok, 10);

        QDir sysDir("/sys/block/" + s);
        sysDir.setFilter(QDir::Dirs);
        QString partitions;
        QStringList entries = sysDir.entryList();
        foreach (QString device, entries) {
            if (device.startsWith(s)) {
                partitions += device + ", ";
            }
        }
        partitions.chop(2);

        QTextStream friendlyName(&s);
        friendlyName.setRealNumberNotation(QTextStream::FixedNotation);
        friendlyName.setRealNumberPrecision(2);
        friendlyName << " (";
        friendlyName << size/(1024*1024*1024.0) << " GB";

        if (partitions.length() > 0)
            friendlyName << ",  partitions: " << partitions;

        friendlyName << ")";

        returnList.append(s);
#else
        QProcess lsblk;
        lsblk.start(QString("diskutil info %1").arg(s), QIODevice::ReadOnly);
        lsblk.waitForStarted();
        lsblk.waitForFinished();

        QString output = lsblk.readLine();
        QStringList iddata;
        QString item = "";
        while (!lsblk.atEnd())
        {
            output = output.trimmed(); // Odd trailing whitespace
            if (output.contains("Device / Media Name:"))
            { // We want the volume name of this device
                output.replace("Device / Media Name:      ","");
                iddata.append(output);
            }
            else if (output.contains("Device Identifier:"))
            { // We want the volume name of this device
                output.replace("Device Identifier:        ","");
                iddata.append(output);
            }
            else if (output.contains("Total Size:"))
            { // We want the volume name of this device
                output.replace("Total Size:               ","");
                QStringList tokens = output.split(" ");
                iddata.append( "("+tokens[0]+tokens[1]+")");
            }

            output = lsblk.readLine();
        }

        foreach(QString each,iddata)
        {
            item += each+": ";
        }

        returnList.append(item);
#endif
    }

    return returnList;
}

void MainWindow::downloadNewData(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesReceived);
    Q_UNUSED(bytesTotal);

    if(reply->error() != QNetworkReply::NoError)
    {//TODO: handle error here
        QFile(path).remove();
        QFile(path + ".tmp").rename(path);
        return;
    }

    total_upd_size -= reply->bytesAvailable();
    QByteArray data = reply->readAll();
    in_len = data.size();
    in_buf = (uint8_t*)data.data();
    strm.next_in = in_buf;
    strm.avail_in = in_len;

    do {
        strm.next_out = out_buf;
        strm.avail_out = OUT_BUF_MAX;
        (void)LZMA.lzma_code (&strm, LZMA_RUN);
        out_len = OUT_BUF_MAX - strm.avail_out;

        QFile file(path);
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            file.write(QByteArray((char*)out_buf, (int)out_len));
            file.close();
        }
        out_buf[0] = 0;
    } while (strm.avail_out == 0);
}


void MainWindow::downloadFinished(QNetworkReply* repl)
{
    Q_UNUSED(repl)
    ui->stackedWidget->setCurrentIndex(MainWindow::Page_Partition);
}

void MainWindow::writingFinished()
{
    ui->stackedWidget->setCurrentIndex(MainWindow::Page_Final);
}

void MainWindow::writingError(QString error)
{
    ui->stackedWidget->setCurrentIndex(MainWindow::Page_Final);
    ui->finalLabel->setText(error);
}

void MainWindow::on_cancelButton_clicked()
{
    reject();
}
