#include "DialogCreateNewImage.h"
#include "ui_DialogCreateNewImage.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

#if defined Q_OS_WIN
#include <windows.h>
#endif

#define CALAOS_UPDATE_VERSION_URL "http://calaos.fr/update?versions=all"

#include <QProcess>

DialogCreateNewImage::DialogCreateNewImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCreateNewImage)
{
    ui->setupUi(this);

    QStringList names;
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
    QStringList names;

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

    ui->targetCombo->clear();
    for (int i = 0; i < names.size(); ++i)
    {
        ui->targetCombo->addItem(names[i], names[i]);
    }
    ui->targetCombo->setCurrentIndex(ui->targetCombo->count() - 1);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,[=](QNetworkReply* reply)
    {
        QJsonDocument j = QJsonDocument::fromJson(reply->readAll());
        QJsonArray jarr = j.array();

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

    manager->get(QNetworkRequest(QUrl(CALAOS_UPDATE_VERSION_URL)));

}

DialogCreateNewImage::~DialogCreateNewImage()
{
    delete ui;
}


void DialogCreateNewImage::updateVersions(const QString &arg1)
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

void DialogCreateNewImage::on_machineCombo_currentIndexChanged(const QString &arg1)
{
    updateVersions(arg1);
}


void DialogCreateNewImage::on_radioButton_2_clicked()
{
    updateVersions(ui->machineCombo->currentText());
}

void DialogCreateNewImage::on_radioButton_clicked()
{
     updateVersions(ui->machineCombo->currentText());
}

void DialogCreateNewImage::on_downloadButton_clicked()
{
    for (auto it : images[ui->machineCombo->currentText()])
    {
        if (it->version == ui->versionCombo->currentText())
        {
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            QUrl url = it->url;

            QFileInfo fileInfo(url.path());

            QString downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
            if (downloadsFolder.isEmpty())
                downloadsFolder = QDir::homePath();

            QString fileName = downloadsFolder + "/" + fileInfo.fileName();
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
            QFile *file = new QFile(fileName);
            if (!file->open(QIODevice::WriteOnly))
            {
                QMessageBox::information(this, tr("Image download"),
                                         tr("Unable to save the file %1: %2.")
                                         .arg(fileName).arg(file->errorString()));
                delete file;
                file = nullptr;
                return;
            }
            ui->statusLabel->setText(tr("Downloading %1.").arg(fileName));
            ui->downloadButton->setEnabled(false);
            ui->writeButton->setEnabled(false);
            QNetworkReply *reply = manager->get(QNetworkRequest(url));
            connect(reply, &QNetworkReply::readyRead, [=]{
                file->write(reply->readAll());
            });
            connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesRead, qint64 totalBytes){
                ui->downloadProgress->setMaximum(totalBytes);
                ui->downloadProgress->setValue(bytesRead);
            });

            connect(reply, &QNetworkReply::finished, [=]{
                ui->imageFilenameLabel->setText(fileName);
                ui->writeButton->setEnabled(true);
                ui->downloadButton->setEnabled(true);
            });
        }
    }
}

void DialogCreateNewImage::on_selectImageButton_clicked()
{
    QString downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (downloadsFolder.isEmpty())
        downloadsFolder = QDir::homePath();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Calaos Image"), downloadsFolder);
    ui->imageFilenameLabel->setText(fileName);
    ui->writeButton->setEnabled(true);
}
