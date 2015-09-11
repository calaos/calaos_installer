#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QSignalMapper>
#include <QMessageBox>
#include <QTreeWidgetItem>

#include "textedit.h"

#include <dialogconnect.h>
#include <dialogopenonline.h>
#include <dialogsaveonline.h>

#include <formrules.h>
#include <projectmanager.h>
#include <wagouploader.h>

using namespace Utils;
using namespace Calaos;

namespace Ui
{
class MainWindow;
}

enum { PAGE_PROG=0, PAGE_DALI=1, PAGE_ABOUT=2 };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void ShowPage(int page = PAGE_ABOUT);

    void Save(QString path = QString());
    void Load(QString path = QString());

    FormRules *getFormRules();

private:
    Ui::MainWindow *ui;

    WagoUploader wuploader;

    QString project_path;

    QMessageBox messageBox;
    QLabel *statusConnectIcon;
    QLabel *statusConnectText;
    QProgressBar *progressBar;
    QPushButton *buttonStopProcess;

    QDir tempDir;

    TextEdit teditor;

    void onShowProg();
    void onShowTransfert();

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void wagoStatusProgress(int status);

    void wagoConnected(QString &ip, bool proxy);
    void wagoDisconnected();
    void wagoUpdateNeeded(QString version, QString new_version);
    void wagoError(int error);

    void projectChanged(bool changed);

private slots:
    void onActionAboutTriggered();
    void onActionQuitTriggered();
    void onActionAboutQtTriggered();

    void onActionDALITriggered();
    void onPushButtonBackClicked();
    void onActionSaveOnlineProject();
    void onActionOpenOnlineProject();
    void onActionDisconnectTriggered();
    void onActionPLCProgrammingTriggered();
    void onActionConnectingTrigerred();
    void onBtWagoStopClicked();
    void onLoadProjectTriggered();
    void onActionSaveTriggered();
    void onActionNewProjectTriggered();
    void onActionSaveProjectTriggered();
    void onPagesCurrentChanged(int );
    void onCloseDaliFormClicked();
    void onActionByRoomTriggered();
    void onActionByInputOutputTriggered();
    void onActionUpdatePLC();
    void onBtConfigureClicked();
    void onBtHelpClicked();
    void onBtAutodetectClicked();
    void onActionCreateNewImageTriggered();
};

#endif // MAINWINDOW_H
