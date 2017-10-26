#ifndef FORMDALIMASTER_H
#define FORMDALIMASTER_H

#include <QWidget>
#include <QTemporaryFile>
#include "ftp/qftp.h"
#include <QProgressDialog>
#include <QBuffer>

namespace Ui {
class FormDaliMaster;
}

class FormDaliMaster : public QWidget
{
    Q_OBJECT

public:
    explicit FormDaliMaster(QWidget *parent = 0);
    ~FormDaliMaster();

signals:
    void closeDaliForm();

private slots:
    void on_pushButtonAdd_clicked();
    void on_pushButtonDel_clicked();
    void on_pushButtonLoad_clicked();
    void on_pushButtonSend_clicked();
    void on_pushButtonEdit_clicked();

    void updateDataTransferProgress(qint64 done, qint64 total);
    void onCommandFinished(int id, bool error);
    void onCommandStarted(int id);

    void resetDone();

    void on_pushButtonLoadFile_clicked();

    void on_pushButtonExportFile_clicked();

private:
    Ui::FormDaliMaster *ui;

    QFtp *ftp = nullptr;

    QTemporaryFile *csvTempFile = nullptr;
    QByteArray csvData;
    QBuffer csvIoBuffer;

    QProgressDialog *progress = nullptr;

    int cmd_connect;
    int cmd_login;
    int cmd_cd;
    int cmd_file_get;
    int cmd_file_set;
    int cmd_del;

    void loadCsv(QStringList csvFile);
    void writeCsv();
};

#endif // FORMDALIMASTER_H
