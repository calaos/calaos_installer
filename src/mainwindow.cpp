#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "version.h"
#include "ConfigOptions.h"
#include "DialogOptions.h"
#include "dialogautodetect.h"
#include "DialogCreateNewImage.h"
#include "WidgetIPAddr.h"

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

    statusConnectText = new QLabel(tr("Disconnected."), this);
    statusBar()->addPermanentWidget(statusConnectText);

    statusConnectIcon = new QLabel(this);
    statusConnectIcon->setPixmap(QPixmap(":/img/user-invisible_16x16.png"));
    statusBar()->addPermanentWidget(statusConnectIcon);

    connect(&wuploader, SIGNAL(progressUpdate(int)), progressBar, SLOT(setValue(int)));
    connect(&wuploader, SIGNAL(statusUpdate(int)), this, SLOT(wagoStatusProgress(int)));

    connect(ui->widgetDali, SIGNAL(closeDaliForm()), this, SLOT(closeDaliForm_clicked()));
    connect(ui->widgetDaliMaster, SIGNAL(closeDaliForm()), this, SLOT(closeDaliForm_clicked()));

    project_path = tr("New");

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
        //Disable this for now. It makes confusion when your last opened project was online,
        //because the project is loaded from the old temp folder, and thus not up to date from calaos_server.
        /*if (ConfigOptions::Instance().optionExists("MainWindow/last_project"))
        {
            QString last_project = ConfigOptions::Instance().getOption("MainWindow/last_project").toString();

            if (!project_path.contains(tempDir.path()))
                Load(last_project);
        }
        else*/
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
        setWindowTitle(QString::fromUtf8("Calaos Installer: [*] %1").arg(project_path));
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
                                      tr("The project was modified, do you want to save it?"),
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
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a project folder..."),
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
                                      tr("The project was modified, do you want to save it?"),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
            on_actionSauvegarder_triggered();
    }

    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();

    ui->widgetRules->ClearProject();

    project_path = tr("Nouveau");
    projectChanged(false);
}

void MainWindow::on_actionSauvegarder_triggered()
{
    if (project_path.isEmpty() || project_path == "new")
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

    statusBar()->showMessage(QString(tr("Project saved: %1")).arg(project_path), 3000);
}

void MainWindow::Load(QString path)
{
    if (!path.isEmpty())
        project_path = path;

    QProgressDialog bar(tr("Loading in progress..."), 0, 0, 0, this);
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

    statusBar()->showMessage(QString(tr("Project loaded: %1")).arg(project_path), 3000);
}

void MainWindow::on_actionCharger_un_projet_triggered()
{
    if (ui->widgetRules->projectChanged())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      tr("The project was modified, do you want to save it?"),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
            on_actionSauvegarder_triggered();
    }

    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a project folder..."),
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
                                      tr("The project was modified, do you want to save it?"),
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
        progressBar->setFormat(tr("Please wait..."));
    }
    else if (status == WAGOST_CREATING)
    {
        progressBar->setFormat(QString(tr("Creating %p%")));
    }
    else if (status == WAGOST_UPLOADING)
    {
        progressBar->setFormat(QString(tr("Loading PLC %p%")));
    }
    else if (status == WAGOST_ABORTED)
    {
        progressBar->setFormat(tr("Canceled"));
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
    ui->actionSet_DMX4ALL_IP_Address->setEnabled(true);
    ui->actionUploadDaliMasterCSV->setEnabled(true);

    statusConnectText->setText(QString(tr("Connected (%1)")).arg(WagoConnect::Instance().getWagoVersion()));
    statusConnectIcon->setPixmap(QPixmap(":/img/user-online_16x16.png"));
}

void MainWindow::wagoDisconnected()
{
    ui->actionSe_connecter->setEnabled(true);
    ui->actionSe_d_connecter->setEnabled(false);
    ui->actionProgrammer_l_automate->setEnabled(false);
    ui->actionDALI->setEnabled(false);
    ui->actionMise_jour_Automate->setEnabled(false);
    ui->actionSet_DMX4ALL_IP_Address->setEnabled(false);
    ui->actionUploadDaliMasterCSV->setEnabled(false);

    statusConnectText->setText(tr("Disconnected."));
    statusConnectIcon->setPixmap(QPixmap(":/img/user-invisible_16x16.png"));
}

void MainWindow::wagoUpdateNeeded(QString version, QString new_version)
{
    if (WagoConnect::Instance().getWagoType() == "750-841" ||
        WagoConnect::Instance().getWagoType() == "750-849" ||
        WagoConnect::Instance().getWagoType() == "750-881")
    {
        if (QMessageBox::question(NULL, tr("Calaos Installer"),
                                  QString(tr("The WAGO PLC have to be upgraded.\n\nIt is actually running the version %1, the last version is %2.\nYou can do it now, but the PLC memory would be cleared.\n\nDo you want to do it now?")).arg(version).arg(new_version),
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
        QMessageBox::critical(this, tr("Calaos Installer"), tr("Connection failed!"));
        break;
    case WERROR_NOTCONNECTED:
        QMessageBox::critical(this, tr("Calaos Installer"), tr("Wago PLC is not connected!"));
        break;
    case WERROR_TIMEOUT:
        QMessageBox::critical(this, tr("Calaos Installer"), tr("Wait timeout reached, error in communication!"));
        break;
    default:
        QMessageBox::critical(this, tr("Calaos Installer"), tr("Unknown error!"));
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
            WagoConnect::Instance().getWagoType() == "750-849" ||
            WagoConnect::Instance().getWagoType() == "750-880" ||
            WagoConnect::Instance().getWagoType() == "750-881")
        {
            ShowPage(PAGE_DALI);
            ui->widgetDali->Init();
        }
        else
        {
            QMessageBox::critical(this, tr("Calaos Installer"), tr("DALI configuration tool is not available on your PLC!"));
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
            WagoConnect::Instance().getWagoType() == "750-849" ||
            WagoConnect::Instance().getWagoType() == "750-881" ||
            WagoConnect::Instance().getWagoType() == "750-889")
        {
            if (QMessageBox::question(NULL, tr("Calaos Installer"),
                                      tr("Your PLC can be upgraded to the latest version right now, do it?"),
                                      QMessageBox::Yes,
                                      QMessageBox::No) == QMessageBox::Yes)
            {
                WagoConnect::Instance().updateWago();
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Calaos Installer"), tr("Wago PLC upgrade not supported on your PLC!"));
        }
    }
    else
    {
        WagoConnect::Instance().SendCommand("WAGO_DALI_ADDRESSING_STATUS");
    }
}

void MainWindow::on_btConfigure_clicked()
{
    DialogOptions d;

    if (d.exec() == QDialog::Accepted)
        d.applyConfig();

}

void MainWindow::on_btHelp_clicked()
{

}

void MainWindow::on_btAutodetect_clicked()
{
    DialogAutoDetect d;

    if (d.exec() == QDialog::Accepted)
    {
        QString host = d.getHost();
        ConfigOptions::Instance().setHost(host);
        on_actionOuvrir_un_projet_en_ligne_triggered();
    }
}

void MainWindow::on_actionCreateNewImage_triggered()
{
    QProcess *createImageProcess = new QProcess();
    createImageProcess->start("./calaos_machinecreator.sh");
    createImageProcess->waitForFinished();
    delete createImageProcess;
}


void MainWindow::on_actionSet_DMX4ALL_IP_Address_triggered()
{
    if (WagoConnect::Instance().getConnectionStatus() == WAGO_CONNECTED)
    {
        if (WagoConnect::Instance().getWagoType() == "750-841" ||
            WagoConnect::Instance().getWagoType() == "750-849" ||
            WagoConnect::Instance().getWagoType() == "750-880" ||
            WagoConnect::Instance().getWagoType() == "750-881" ||
            WagoConnect::Instance().getWagoType() == "750-889")
        {
            QDialog *d = new QDialog(this);
            d->setWindowFlags(d->windowFlags() & ~Qt::WindowContextHelpButtonHint);

            QVBoxLayout *vLayout = new QVBoxLayout(d);
            QLabel *label = new QLabel();
            label->setText(tr("Please enter the IP address of the DMX4ALL:"));
            vLayout->addWidget(label);

            WidgetIPAddr *wip = new WidgetIPAddr(0);
            vLayout->addWidget(wip);

            QDialogButtonBox *bbox = new QDialogButtonBox(0);
            bbox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
            vLayout->addWidget(bbox);
            connect(bbox, SIGNAL(rejected()), d, SLOT(reject()));
            connect(bbox, SIGNAL(accepted()), d, SLOT(accept()));

            d->layout()->setSizeConstraint(QLayout::SetFixedSize);

            if (d->exec() == QDialog::Accepted)
            {
                QHostAddress address(wip->getIPAddress());
                if (QAbstractSocket::IPv4Protocol == address.protocol())
                {
                    WagoConnect::Instance().SendCommand(QString("WAGO_SET_DMX_IP %1").arg(wip->getIPAddress()));
                    QMessageBox::information(this, tr("Calaos Installer"), tr("IP Address changed successfully"));
                }
                else
                    QMessageBox::critical(this, tr("Calaos Installer"), tr("Wrong IP Address!"));
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Calaos Installer"), tr("DMX4ALL configuration is not available on your PLC!"));
        }
    }
}

void MainWindow::on_actionUploadDaliMasterCSV_triggered()
{
    if (WagoConnect::Instance().getConnectionStatus() == WAGO_CONNECTED)
    {
        if (WagoConnect::Instance().getWagoType() != "750-842")
        {
            ShowPage(PAGE_DALIMASTER);
        }
        else
            QMessageBox::critical(this, tr("Calaos Installer"), tr("DALI configuration tool is not available on your PLC!"));
    }
}
