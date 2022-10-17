#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QProcess>

#include "DiskWriter.h"
#include "CalaosApi.h"
#include "CalaosImage.h"
#include "Utils.h"
#include "Platform.h"
#include "UsbDisk.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_machineCombo_currentIndexChanged(const QString &);
    void on_radioButtonStable_clicked();
    void on_radioButtonAll_clicked();

    void on_continueButton_clicked();
    void on_cancelButton_clicked();

    void on_selectImageButton_clicked();

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void writeFinished();
    void writeSync();
    void writeError(QString error);

    void writeProgress(QString status, qint64 bytesReceived, qint64 bytesTotal, qint64 elapsedMs);

    void on_restartButton_clicked();

signals:
    void finished(QString filename);
    void proceedToWriteImageToDevice(const QString &image, UsbDisk *device);

private:
    Ui::MainWindow *ui;

    enum {
        Page_Start = 0,
        Page_Download = 1,
        Page_Partition = 2,
        Page_Writing = 3,
        Page_Final = 4,
    };

    QNetworkReply *reply;
    QNetworkAccessManager *manager;

    CalaosImageModel *imageModel = nullptr;
    FilterImageModel *filterImageModel = nullptr;

    Platform *platform = nullptr;

    DiskWriter *diskWriter;
    QThread* diskWriterThread;

    bool m_bFileFromDisk = false;
    QString path;

    unsigned long total_upd_size;
    QString m_decompressedFile;
    UsbDisk *m_disk = nullptr;

    QDateTime startDownloadTime;
    QElapsedTimer downloadTime;

    CalaosApi *calaosApi = nullptr;
    NetworkRequest *downloadReq = nullptr;

    void startWriteProcess();
};

#endif // MAINWINDOW_H
