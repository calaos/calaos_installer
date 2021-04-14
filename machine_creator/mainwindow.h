#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QList>
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
#include <QProcess>
#include <KCompressionDevice>

#include "DiskWriter.h"
#include "CalaosApi.h"
#include "CalaosImage.h"
#include "Utils.h"

/* read/write buffer sizes */
#define IN_BUF_MAX  409600
#define OUT_BUF_MAX 409600
/* analogous to xz CLI options: -0 to -9 */
#define COMPRESSION_LEVEL 7

/* boolean setting, analogous to xz CLI option: -e */
#define COMPRESSION_EXTREME true

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
    void downloadFinished(QNetworkReply* repl);
    void writingFinished();
    void writingError(QString error);

signals:
    void finished(QString filename);
    void proceedToWriteImageToDevice(const QString& image, const QString& device);

private:
    Ui::MainWindow *ui;

    enum {
        Page_Start = 0,
        Page_Download = 1,
        Page_Partition = 2,
        Page_Writing = 3,
        Page_Final = 4
    };

    QNetworkReply *reply;
    QNetworkAccessManager *manager;

    CalaosImageModel *imageModel = nullptr;
    FilterImageModel *filterImageModel = nullptr;

    KCompressionDevice *compDevice;

    short state;

    uint8_t *in_buf;
    uint8_t out_buf [OUT_BUF_MAX];
    size_t in_len;  /* length of useful data in in_buf */
    size_t out_len; /* length of useful data in out_buf */

    DiskWriter *diskWriter;
    QThread* diskWriterThread;

    bool m_bFileFromDisk;
    QString path;

    unsigned long total_upd_size;
    QString m_decompressedFile;

    QDateTime startDownloadTime;
    QElapsedTimer downloadTime;

    CalaosApi *calaosApi = nullptr;

    QStringList getUserFriendlyNames(const QStringList &devices) const;
};

#endif // MAINWINDOW_H
