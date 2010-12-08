#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
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

enum { PAGE_PROG=0, PAGE_DALI, PAGE_ABOUT };

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
                void wagoUpdateNeeded(QString &version);
                void wagoError(int error);

                void projectChanged(bool changed);

        private slots:
                void on_actionG_n_rer_fichier_d_impression_triggered();
                void on_actionDALI_triggered();
                void on_pushButtonBack_clicked();
                void on_actionSauvegarder_un_projet_en_ligne_triggered();
                void on_actionOuvrir_un_projet_en_ligne_triggered();
                void on_actionSe_d_connecter_triggered();
                void on_actionProgrammer_l_automate_triggered();
                void on_actionSe_connecter_triggered();
                void button_wagostop_clicked();
                void on_actionCharger_un_projet_triggered();
                void on_actionSauvegarder_triggered();
                void on_actionNouveau_projet_triggered();
                void on_actionSauvegarder_un_projet_triggered();
                void on_Pages_currentChanged(int );
                void on_actionA_propos_de_Qt_triggered();
                void on_actionAbout_activated();
                void on_actionQuit_activated();
                void closeDaliForm_clicked();
};

#endif // MAINWINDOW_H
