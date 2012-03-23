#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"
#include "ConfigOptions.h"

MainWindow::MainWindow(QWidget *parent):
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        wuploader(parent),
        messageBox(QMessageBox::Information, tr("Calaos Installer"), ""),
        teditor(this)
{
        ui->setupUi(this);

        ShowPage(PAGE_PROG);

        //Set version info
        ui->textEditAbout->setHtml(ui->textEditAbout->toHtml().arg(calaos_installer_version));

        ui->menubar->hide();

        //Add toolbar menus
        ui->btProject->setMenu(ui->menuProjet);
        ui->btWago->setMenu(ui->menuWago);
        ui->btServer->setMenu(ui->menuCentrale);

        connect(ui->btHelp, SIGNAL(pressed()), this, SLOT(actionAbout_triggered()));
        connect(ui->btQuit, SIGNAL(pressed()), this, SLOT(actionQuit_triggered()));
        connect(ui->btAboutQt, SIGNAL(pressed()), this, SLOT(actionAboutQt_triggered()));

        connect(ui->widgetRules, SIGNAL(projectModified(bool)), this, SLOT(projectChanged(bool)));

        //WagoConnect
        connect(&WagoConnect::Instance(), SIGNAL(connected(QString&,bool)), this, SLOT(wagoConnected(QString&,bool)));
        connect(&WagoConnect::Instance(), SIGNAL(disconnected()), this, SLOT(wagoDisconnected()));
        connect(&WagoConnect::Instance(), SIGNAL(updateNeeded(QString,QString)), this, SLOT(wagoUpdateNeeded(QString,QString)));
        connect(&WagoConnect::Instance(), SIGNAL(error(int)), this, SLOT(wagoError(int)));

        buttonStopProcess = new QPushButton(QIcon(":/img/process-stop_16x16.png"), "", this);
        statusBar()->addPermanentWidget(buttonStopProcess);
        buttonStopProcess->hide();

        progressBar = new QProgressBar(this);
        statusBar()->addPermanentWidget(progressBar);
        progressBar->hide();

        statusConnectText = new QLabel(QString::fromUtf8("Déconnecté."), this);
        statusBar()->addPermanentWidget(statusConnectText);

        statusConnectIcon = new QLabel(this);
        statusConnectIcon->setPixmap(QPixmap(":/img/user-invisible_16x16.png"));
        statusBar()->addPermanentWidget(statusConnectIcon);

        connect(&wuploader, SIGNAL(progressUpdate(int)), progressBar, SLOT(setValue(int)));
        connect(&wuploader, SIGNAL(statusUpdate(int)), this, SLOT(wagoStatusProgress(int)));

        connect(ui->widgetDali, SIGNAL(closeDaliForm()), this, SLOT(closeDaliForm_clicked()));

        project_path = "Nouveau";

        /* Search and create a new temp dir */
        tempDir.setPath(QDir::tempPath());
        int cpt = 0;

        do
        {
                tempDir.setPath(QDir::tempPath() + "/calaos_installer_" + QString::number(cpt));
                cpt++;
        } while (tempDir.exists());

        tempDir.mkdir(tempDir.path());

        const QStringList args = QCoreApplication::arguments();
        if(args.count() > 1)
        {
                QString path = args[1];

                Load(path);
        }
        else
        {
                if (ConfigOptions::Instance().optionExists("MainWindow/last_project"))
                {
                        QString last_project = ConfigOptions::Instance().getOption("MainWindow/last_project").toString();

                        if (!project_path.contains(tempDir.path()))
                                Load(last_project);
                }
                else
                {
                        on_actionNouveau_projet_triggered();
                }
        }

        if (ConfigOptions::Instance().optionExists("MainWindow/size"))
                resize(ConfigOptions::Instance().getOption("MainWindow/size").toSize());
        if (ConfigOptions::Instance().optionExists("MainWindow/pos"))
                move(ConfigOptions::Instance().getOption("MainWindow/pos").toPoint());
}

MainWindow::~MainWindow()
{
        tempDir.remove("io.xml");
        tempDir.remove("rules.xml");
        tempDir.remove("local_config.xml");
        tempDir.rmdir(tempDir.path());

        delete ui;
}

void MainWindow::ShowPage(int page)
{
        ui->Pages->setCurrentIndex(page);
}

void MainWindow::actionAboutQt_triggered()
{
        qApp->aboutQt();
}

void MainWindow::on_Pages_currentChanged(int page)
{
        if (page == PAGE_PROG)
                onShowProg();
        else if (page == PAGE_DALI)
                onShowTransfert();
}

void MainWindow::onShowProg()
{

}

void MainWindow::onShowTransfert()
{

}

void MainWindow::projectChanged(bool changed)
{
        if (changed)
        {
                setWindowTitle(QString::fromUtf8("Calaos Installer: [modified] %1").arg(project_path));
                ui->labelProjectName->setText(QString::fromUtf8("[*] %1").arg(project_path));
        }
        else
        {
                setWindowTitle(QString::fromUtf8("Calaos Installer: %1").arg(project_path));
                ui->labelProjectName->setText(project_path);
        }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
        if (ui->widgetRules->projectChanged())
        {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Un projet est ouvert et modifié, voulez-vous l'enregistrer?"),
                                      QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                        on_actionSauvegarder_triggered();

                event->accept();
        }

        ConfigOptions::Instance().setOption("MainWindow/size", size());
        ConfigOptions::Instance().setOption("MainWindow/pos", pos());

        if (!project_path.contains(tempDir.path()))
                ConfigOptions::Instance().setOption("MainWindow/last_project", project_path);

        ConfigOptions::Instance().saveConfig();
}

void MainWindow::on_actionSauvegarder_un_projet_triggered()
{
        QString dir = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Choisir un dossier de projet..."),
                                                 QDir::homePath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

        project_path = dir;

        Save();
}

void MainWindow::on_actionNouveau_projet_triggered()
{
        if (ui->widgetRules->projectChanged())
        {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Un projet est ouvert et modifié, voulez-vous l'enregistrer?"),
                                      QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                        on_actionSauvegarder_triggered();
        }

        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();

        ui->widgetRules->ClearProject();

        project_path = "Nouveau";
        projectChanged(false);
}

void MainWindow::on_actionSauvegarder_triggered()
{
        if (project_path.isEmpty() || project_path == "Nouveau")
                on_actionSauvegarder_un_projet_triggered();

        Save();
}

void MainWindow::Save(QString path)
{
        if (!path.isEmpty())
                project_path = path;

        QString iofile = project_path + "/io.xml";
        QString rulefile = project_path + "/rules.xml";

        ProjectManager::saveIOsToFile(iofile);
        ProjectManager::saveRulesToFile(rulefile);

        ui->widgetRules->setProjectModified(false);

        statusBar()->showMessage(QString::fromUtf8("Projet sauvegardé: ") + project_path, 3000);
}

void MainWindow::Load(QString path)
{
        if (!path.isEmpty())
                project_path = path;

        QProgressDialog bar("Chargement en cours...", 0, 0, 0, this);
        bar.setWindowModality(Qt::ApplicationModal);
        bar.show();

        QString iofile = project_path + "/io.xml";
        QString rulefile = project_path + "/rules.xml";

        ProjectManager::loadIOsFromFile(iofile);
        QCoreApplication::processEvents();
        ProjectManager::loadRulesFromFile(rulefile);
        QCoreApplication::processEvents();

        ui->widgetRules->PopulateRoomsTree();
        ui->widgetRules->PopulateRulesTree();

        ui->widgetRules->setProjectModified(false);

        statusBar()->showMessage(QString::fromUtf8("Projet chargé: ") + project_path, 3000);
}

void MainWindow::on_actionCharger_un_projet_triggered()
{
        if (ui->widgetRules->projectChanged())
        {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Un projet est ouvert et modifié, voulez-vous l'enregistrer?"),
                                      QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                        on_actionSauvegarder_triggered();
        }

        QString dir = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Choisir un dossier de projet..."),
                                                 QDir::homePath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

        if (dir.isEmpty()) return;

        ListeRule::Instance().clear();
        ListeRoom::Instance().clear();

        ui->widgetRules->ClearProject();

        project_path = dir;

        Load();
}

void MainWindow::on_actionOuvrir_un_projet_en_ligne_triggered()
{
        if (ui->widgetRules->projectChanged())
        {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      QString::fromUtf8("Un projet est ouvert et modifié, voulez-vous l'enregistrer?"),
                                      QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                        on_actionSauvegarder_triggered();
        }

        DialogOpenOnline dopen(tempDir.path());

        if (dopen.exec() == QDialog::Accepted)
        {
                ListeRule::Instance().clear();
                ListeRoom::Instance().clear();

                ui->widgetRules->ClearProject();

                project_path = tempDir.path();

                Load();
        }
}

void MainWindow::on_actionSauvegarder_un_projet_en_ligne_triggered()
{
        DialogSaveOnline dsave(project_path);

        Save(); //Save the project before sending it

        if (dsave.exec() == QDialog::Accepted)
        {
                statusBar()->showMessage(QString::fromUtf8("Projet envoyé sur la centrale..."), 3000);
        }
}

void MainWindow::on_actionProgrammer_l_automate_triggered()
{
        ui->actionProgrammer_l_automate->setEnabled(false);
        buttonStopProcess->show();
        progressBar->show();
        connect(buttonStopProcess, SIGNAL(clicked()), this, SLOT(button_wagostop_clicked()));

        wuploader.startUpload(WagoConnect::Instance().getWagoIP());
}

void MainWindow::button_wagostop_clicked()
{
        wuploader.stopUpload();
        ui->actionProgrammer_l_automate->setEnabled(true);
        buttonStopProcess->hide();
        progressBar->hide();

        disconnect(buttonStopProcess, SIGNAL(clicked()), this, SLOT(button_wagostop_clicked()));
}

void MainWindow::wagoStatusProgress(int status)
{
        if (status == WAGOST_START)
        {
                progressBar->setFormat(QString::fromUtf8("Veuillez patienter..."));
        }
        else if (status == WAGOST_CREATING)
        {
                progressBar->setFormat(QString::fromUtf8("Création %p%"));
        }
        else if (status == WAGOST_UPLOADING)
        {
                progressBar->setFormat(QString::fromUtf8("Chargement automate %p%"));
        }
        else if (status == WAGOST_ABORTED)
        {
                progressBar->setFormat(QString::fromUtf8("Annulé"));
                progressBar->setValue(0);
        }
        else if (status == WAGOST_DONE)
        {
                progressBar->setFormat("%p%");
                progressBar->setValue(100);

                ui->actionProgrammer_l_automate->setEnabled(true);
                buttonStopProcess->hide();
                progressBar->hide();
        }
}

void MainWindow::on_actionSe_connecter_triggered()
{
        DialogConnect dconnect;
        dconnect.exec();
}

void MainWindow::on_actionSe_d_connecter_triggered()
{
        WagoConnect::Instance().Disconnect();
}

void MainWindow::wagoConnected(QString &, bool)
{
        ui->actionSe_connecter->setEnabled(false);
        ui->actionSe_d_connecter->setEnabled(true);
        ui->actionProgrammer_l_automate->setEnabled(true);
        ui->actionDALI->setEnabled(true);
        ui->actionMise_jour_Automate->setEnabled(true);

        statusConnectText->setText(QString::fromUtf8("Connecté (") + WagoConnect::Instance().getWagoVersion() + ")");
        statusConnectIcon->setPixmap(QPixmap(":/img/user-online_16x16.png"));
}

void MainWindow::wagoDisconnected()
{
        ui->actionSe_connecter->setEnabled(true);
        ui->actionSe_d_connecter->setEnabled(false);
        ui->actionProgrammer_l_automate->setEnabled(false);
        ui->actionDALI->setEnabled(false);
        ui->actionMise_jour_Automate->setEnabled(false);

        statusConnectText->setText(QString::fromUtf8("Déconnecté."));
        statusConnectIcon->setPixmap(QPixmap(":/img/user-invisible_16x16.png"));
}

void MainWindow::wagoUpdateNeeded(QString version, QString new_version)
{
        if (WagoConnect::Instance().getWagoType() == "750-841" ||
            WagoConnect::Instance().getWagoType() == "750-849")
        {
                if (QMessageBox::question(NULL, tr("Calaos Installer"),
                                      QString::fromUtf8("L'automate doit être mis à jour.\n\nIl est actuellement en version %1, la dernière version est la %2.\nVous pouvez le faire maintenant, et l'automate sera réinitialisé.\n\nVoulez-vous le faire maintenant?").arg(version, new_version),
                                      QMessageBox::Yes,
                                      QMessageBox::No) == QMessageBox::Yes)
                {
                        WagoConnect::Instance().updateWago();
                }
        }
        /* Disable this, user can't update the 842 by themselves...
        else
        {
                messageBox.setText(QString::fromUtf8("L'automate doit être mis à jour.\n\nIl est actuellement en version %1, la dernière version est la %2.").arg(version, new_version));
                messageBox.show();
        }*/
}

void MainWindow::wagoError(int error)
{
        switch (error)
        {
//          case WERROR_CONNECT_FAILED:
//                if (QMessageBox::question(this, tr("Calaos Installer"),
//                                          QString::fromUtf8("La connection a échoué !\nVoulez-vous essayer à nouveau?"),
//                                          QMessageBox::Ok,
//                                          QMessageBox::Cancel
//                        ) == QMessageBox::Ok)
//                        on_actionSe_connecter_triggered();
//                break;
//          case WERROR_NOTCONNECTED:
//                if (QMessageBox::question(this, tr("Calaos Installer"),
//                                          QString::fromUtf8("L'automate n'est pas connecté !\nVoulez-vous effectuer une connexion maintenant?"),
//                                          QMessageBox::Ok,
//                                          QMessageBox::Cancel
//                        ) == QMessageBox::Ok)
//                        on_actionSe_connecter_triggered();
//                break;
//          case WERROR_TIMEOUT:
//                if (QMessageBox::question(this, tr("Calaos Installer"),
//                                          QString::fromUtf8("Le délai d'attente de la réponse est dépassé !\nVoulez-vous essayer à nouveau?"),
//                                          QMessageBox::Ok,
//                                          QMessageBox::Cancel
//                        ) == QMessageBox::Ok)
//                        on_actionSe_connecter_triggered();
//                break;
          case WERROR_CONNECT_FAILED:
                QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("La connection a échoué !"));
                break;
          case WERROR_NOTCONNECTED:
                QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("L'automate n'est pas connecté !"));
                break;
          case WERROR_TIMEOUT:
                QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Le délai d'attente de la réponse est dépassé ! Erreur dans la réponse."));
                break;
          default:
                QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("Une erreur inconnue est survenue !"));
                break;
        }
}

void MainWindow::on_pushButtonBack_clicked()
{
        ShowPage(PAGE_PROG);
}

void MainWindow::on_actionDALI_triggered()
{
        if (WagoConnect::Instance().getConnectionStatus() == WAGO_CONNECTED)
        {
                if (WagoConnect::Instance().getWagoType() == "750-841" ||
                    WagoConnect::Instance().getWagoType() == "750-849")
                {
                        ShowPage(PAGE_DALI);
                        ui->widgetDali->Init();
                }
                else
                {
                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("L'adressage DALI n'est supporté que sur les contrôleurs\nWago 750-841 et 750-849!"));
                }
        }
        else
        {
                WagoConnect::Instance().SendCommand("WAGO_DALI_ADDRESSING_STATUS");
        }
}

FormRules *MainWindow::getFormRules()
{
        if (ui)
                return ui->widgetRules;

        return NULL;
}

void MainWindow::closeDaliForm_clicked()
{
        ShowPage(PAGE_PROG);
}

void MainWindow::actionQuit_triggered()
{
        close();
}

void MainWindow::actionAbout_triggered()
{
        ShowPage(PAGE_ABOUT);
}

void MainWindow::on_actionPar_pi_ce_triggered()
{
        teditor.resize(700, 700);
        teditor.show();
        teditor.loadRooms();
}

void MainWindow::on_actionPar_Entr_es_Sorties_triggered()
{
        teditor.resize(700, 700);
        teditor.show();
        teditor.loadIOList();
}

void MainWindow::on_actionMise_jour_Automate_triggered()
{
        if (WagoConnect::Instance().getConnectionStatus() == WAGO_CONNECTED)
        {
                if (WagoConnect::Instance().getWagoType() == "750-841" ||
                    WagoConnect::Instance().getWagoType() == "750-849")
                {
                        if (QMessageBox::question(NULL, tr("Calaos Installer"),
                                              QString::fromUtf8("L'automate peut être mis à jour à la dernière version.\n\nVoulez-vous le faire maintenant?"),
                                              QMessageBox::Yes,
                                              QMessageBox::No) == QMessageBox::Yes)
                        {
                                WagoConnect::Instance().updateWago();
                        }
                }
                else
                {
                        QMessageBox::critical(this, tr("Calaos Installer"), QString::fromUtf8("La mise à jour n'est supporté que sur les contrôleurs\nWago 750-841 et 750-849!"));
                }
        }
        else
        {
                WagoConnect::Instance().SendCommand("WAGO_DALI_ADDRESSING_STATUS");
        }
}
