#include "DialogFlashDevice.h"
#include "ui_DialogFlashDevice.h"
#include "ConfigImageBuilder.h"
#include "DialogFlashConsole.h"

#include <QSerialPortInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QStyle>

DialogFlashDevice::DialogFlashDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFlashDevice),
    m_firmwareManager(new FirmwareManager(this)),
    m_toolManager(new FlashToolManager(this)),
    m_serialRefreshTimer(new QTimer(this))
{
    ui->setupUi(this);

    // Password visibility toggle
    auto *togglePassAction = ui->editWifiPassword->addAction(
        QIcon::fromTheme("password-show-off",
                         style()->standardIcon(QStyle::SP_VistaShield)),
        QLineEdit::TrailingPosition);
    togglePassAction->setCheckable(true);
    togglePassAction->setToolTip(tr("Show password"));
    connect(togglePassAction, &QAction::toggled, this, [this, togglePassAction](bool visible)
    {
        ui->editWifiPassword->setEchoMode(visible ? QLineEdit::Normal : QLineEdit::Password);
        togglePassAction->setToolTip(visible ? tr("Hide password") : tr("Show password"));
    });

    // Board selection
    connect(ui->comboBoard, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogFlashDevice::onBoardChanged);

    // Firmware source
    connect(ui->radioDownload, &QRadioButton::toggled,
            this, &DialogFlashDevice::onFirmwareSourceChanged);
    connect(ui->radioLocalFile, &QRadioButton::toggled,
            this, &DialogFlashDevice::onFirmwareSourceChanged);
    connect(ui->buttonBrowseFirmware, &QPushButton::clicked,
            this, &DialogFlashDevice::onBrowseFirmware);

    // Network
    connect(ui->comboNetInterface, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogFlashDevice::onNetInterfaceChanged);
    connect(ui->comboIpConfig, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogFlashDevice::onIpConfigChanged);

    // Server
    connect(ui->checkAutoDiscovery, &QCheckBox::toggled,
            this, &DialogFlashDevice::onAutoDiscoveryToggled);

    // Serial port
    connect(ui->buttonRefreshPorts, &QPushButton::clicked,
            this, &DialogFlashDevice::onRefreshSerialPorts);
    m_serialRefreshTimer->setInterval(2000);
    connect(m_serialRefreshTimer, &QTimer::timeout,
            this, &DialogFlashDevice::refreshSerialPorts);

    // Tool
    connect(ui->buttonBrowseTool, &QPushButton::clicked,
            this, &DialogFlashDevice::onBrowseTool);

    // Action buttons
    connect(ui->buttonFlash, &QPushButton::clicked,
            this, &DialogFlashDevice::onFlashClicked);
    connect(ui->buttonSaveConfig, &QPushButton::clicked,
            this, &DialogFlashDevice::onSaveConfigClicked);
    connect(ui->buttonOpenInstructions, &QPushButton::clicked,
            this, &DialogFlashDevice::onOpenInstructions);
    connect(ui->buttonClose, &QPushButton::clicked,
            this, &DialogFlashDevice::reject);

    // Firmware download progress
    connect(m_firmwareManager, &FirmwareManager::downloadProgress,
            this, [this](qint64 received, qint64 total)
    {
        if (total > 0)
        {
            ui->progressFirmware->setMaximum(static_cast<int>(total));
            ui->progressFirmware->setValue(static_cast<int>(received));
        }
    });

    // Initial field visibility
    updateWifiFieldsVisibility();
    updateStaticFieldsVisibility();
    updateServerFieldsVisibility();

    loadBoards();
}

DialogFlashDevice::~DialogFlashDevice()
{
    delete m_extractDir;
    delete ui;
}

void DialogFlashDevice::loadBoards()
{
    m_boards = BoardProfile::loadFromFile(BoardProfile::boardsFilePath());

    ui->comboBoard->clear();
    for (const auto &board : m_boards)
        ui->comboBoard->addItem(board.name);

    if (!m_boards.isEmpty())
        onBoardChanged(0);
}

void DialogFlashDevice::onBoardChanged(int index)
{
    if (index < 0 || index >= m_boards.size())
        return;

    const BoardProfile &board = m_boards.at(index);
    updateGuiForBoard(board);
    fetchFirmwareVersions();
    updateToolStatus();
}

void DialogFlashDevice::updateGuiForBoard(const BoardProfile &board)
{
    // Serial port section
    bool showSerial = board.flash.requiresSerialPort;
    ui->groupSerialPort->setVisible(showSerial);
    if (showSerial)
    {
        refreshSerialPorts();
        m_serialRefreshTimer->start();
    }
    else
    {
        m_serialRefreshTimer->stop();
    }

    // Check OS support
    bool flashSupported = board.isFlashSupportedOnCurrentOs();

    ui->buttonFlash->setVisible(flashSupported);
    ui->buttonSaveConfig->setVisible(!flashSupported);
    ui->buttonOpenInstructions->setVisible(!flashSupported);
    ui->labelUnsupportedOs->setVisible(!flashSupported);

    // Tool section: always visible so user can browse, but adapt messaging
    ui->groupToolStatus->setVisible(flashSupported);
}

void DialogFlashDevice::updateWifiFieldsVisibility()
{
    bool wifi = (ui->comboNetInterface->currentIndex() == 1);
    ui->labelWifiSsid->setVisible(wifi);
    ui->editWifiSsid->setVisible(wifi);
    ui->labelWifiPass->setVisible(wifi);
    ui->editWifiPassword->setVisible(wifi);
}

void DialogFlashDevice::updateStaticFieldsVisibility()
{
    bool isStatic = (ui->comboIpConfig->currentIndex() == 1);
    ui->labelStaticIp->setVisible(isStatic);
    ui->editStaticIp->setVisible(isStatic);
    ui->labelStaticMask->setVisible(isStatic);
    ui->editStaticMask->setVisible(isStatic);
    ui->labelStaticGw->setVisible(isStatic);
    ui->editStaticGateway->setVisible(isStatic);
    ui->labelStaticDns->setVisible(isStatic);
    ui->editStaticDns->setVisible(isStatic);
}

void DialogFlashDevice::updateServerFieldsVisibility()
{
    bool manual = !ui->checkAutoDiscovery->isChecked();
    ui->labelServerHost->setVisible(manual);
    ui->editServerHost->setVisible(manual);
    ui->labelServerPort->setVisible(manual);
    ui->spinServerPort->setVisible(manual);
    ui->checkSsl->setVisible(manual);
}

void DialogFlashDevice::updateToolStatus()
{
    int idx = ui->comboBoard->currentIndex();
    if (idx < 0 || idx >= m_boards.size())
        return;

    const BoardProfile &board = m_boards.at(idx);
    QString toolPath = m_toolManager->findTool(board.flash);

    if (!toolPath.isEmpty())
    {
        QString version = m_toolManager->cachedToolVersion(board.flash.tool);
        QString text = board.flash.tool;
        if (!version.isEmpty())
            text += " " + version;
        ui->labelToolStatus->setText(text);
        ui->labelToolStatus->setToolTip(toolPath);
        ui->labelToolStatus->setStyleSheet("color: green;");
    }
    else
    {
        ui->labelToolStatus->setText(tr("Tool not found: %1").arg(board.flash.tool));
        ui->labelToolStatus->setStyleSheet("color: #e65100;");
    }
}

void DialogFlashDevice::refreshSerialPorts()
{
    QString currentPort = ui->comboSerialPort->currentText();
    ui->comboSerialPort->clear();

    const auto ports = QSerialPortInfo::availablePorts();
    for (const auto &port : ports)
    {
        QString label = port.portName();
        if (!port.description().isEmpty())
            label += " — " + port.description();
        ui->comboSerialPort->addItem(label, port.systemLocation());
    }

    // Restore previous selection if still present
    if (!currentPort.isEmpty())
    {
        int idx = ui->comboSerialPort->findText(currentPort, Qt::MatchStartsWith);
        if (idx >= 0)
            ui->comboSerialPort->setCurrentIndex(idx);
    }
}

void DialogFlashDevice::fetchFirmwareVersions()
{
    int idx = ui->comboBoard->currentIndex();
    if (idx < 0 || idx >= m_boards.size())
        return;

    const BoardProfile &board = m_boards.at(idx);
    ui->comboFirmwareVersion->clear();
    ui->comboFirmwareVersion->addItem(tr("Loading..."));
    ui->comboFirmwareVersion->setEnabled(false);
    ui->labelFirmwareInfo->clear();
    m_releases.clear();

    m_firmwareManager->fetchReleases(board, [this](bool success, const QList<FirmwareRelease> &releases)
    {
        ui->comboFirmwareVersion->clear();
        ui->comboFirmwareVersion->setEnabled(true);

        if (!success || releases.isEmpty())
        {
            ui->comboFirmwareVersion->addItem(tr("No releases found"));
            ui->comboFirmwareVersion->setEnabled(false);
            return;
        }

        m_releases = releases;
        for (const auto &rel : releases)
        {
            QString label = rel.version;
            if (!rel.date.isEmpty())
                label += "  (" + rel.date + ")";
            ui->comboFirmwareVersion->addItem(label);
        }
    });
}

void DialogFlashDevice::onFirmwareSourceChanged()
{
    bool download = ui->radioDownload->isChecked();
    ui->comboFirmwareVersion->setEnabled(download);
    ui->buttonBrowseFirmware->setEnabled(!download);
    ui->labelFirmwareFile->setVisible(!download);
}

void DialogFlashDevice::onBrowseFirmware()
{
    QString path = QFileDialog::getOpenFileName(
        this, tr("Select firmware file"), QString(),
        tr("ZIP files (*.zip)"));

    if (path.isEmpty())
        return;

    m_localFirmwarePath = path;
    ui->labelFirmwareFile->setText(QFileInfo(path).fileName());
}

void DialogFlashDevice::onNetInterfaceChanged(int index)
{
    Q_UNUSED(index)
    updateWifiFieldsVisibility();
}

void DialogFlashDevice::onIpConfigChanged(int index)
{
    Q_UNUSED(index)
    updateStaticFieldsVisibility();
}

void DialogFlashDevice::onAutoDiscoveryToggled(bool checked)
{
    Q_UNUSED(checked)
    updateServerFieldsVisibility();
}

void DialogFlashDevice::onRefreshSerialPorts()
{
    refreshSerialPorts();
}

void DialogFlashDevice::onBrowseTool()
{
    int idx = ui->comboBoard->currentIndex();
    if (idx < 0 || idx >= m_boards.size())
        return;

    const BoardProfile &board = m_boards.at(idx);
    QString path = QFileDialog::getOpenFileName(
        this, tr("Locate %1").arg(board.flash.tool));

    if (path.isEmpty())
        return;

    QFileInfo fi(path);
    if (!fi.exists() || !fi.isExecutable())
    {
        QMessageBox::warning(this, tr("Invalid tool"),
                             tr("The selected file is not a valid executable."));
        return;
    }

    m_toolManager->saveCustomToolPath(board.flash.tool, path);
    updateToolStatus();
}

bool DialogFlashDevice::isValidIpv4(const QString &addr) const
{
    QStringList parts = addr.split('.');
    if (parts.size() != 4)
        return false;

    for (const auto &part : parts)
    {
        bool ok;
        int val = part.toInt(&ok);
        if (!ok || val < 0 || val > 255)
            return false;
    }
    return true;
}

bool DialogFlashDevice::validateForm(QString &error)
{
    // WiFi validation
    if (ui->comboNetInterface->currentIndex() == 1) // WiFi
    {
        if (ui->editWifiSsid->text().trimmed().isEmpty())
        {
            error = tr("WiFi SSID is required.");
            ui->editWifiSsid->setFocus();
            return false;
        }
    }

    // Static IP validation
    if (ui->comboIpConfig->currentIndex() == 1) // Static
    {
        if (!isValidIpv4(ui->editStaticIp->text().trimmed()))
        {
            error = tr("Invalid IP address.");
            ui->editStaticIp->setFocus();
            return false;
        }
        if (!isValidIpv4(ui->editStaticMask->text().trimmed()))
        {
            error = tr("Invalid subnet mask.");
            ui->editStaticMask->setFocus();
            return false;
        }
        if (!isValidIpv4(ui->editStaticGateway->text().trimmed()))
        {
            error = tr("Invalid gateway address.");
            ui->editStaticGateway->setFocus();
            return false;
        }
        QString dns = ui->editStaticDns->text().trimmed();
        if (!dns.isEmpty() && !isValidIpv4(dns))
        {
            error = tr("Invalid DNS address.");
            ui->editStaticDns->setFocus();
            return false;
        }
    }

    // Server validation
    if (!ui->checkAutoDiscovery->isChecked())
    {
        if (ui->editServerHost->text().trimmed().isEmpty())
        {
            error = tr("Server host is required when auto-discovery is disabled.");
            ui->editServerHost->setFocus();
            return false;
        }
    }

    // Serial port validation (if visible)
    if (ui->groupSerialPort->isVisible())
    {
        if (ui->comboSerialPort->count() == 0 ||
            ui->comboSerialPort->currentData().toString().isEmpty())
        {
            error = tr("Please select a serial port.");
            ui->comboSerialPort->setFocus();
            return false;
        }
    }

    // Firmware validation
    if (ui->radioDownload->isChecked())
    {
        if (m_releases.isEmpty())
        {
            error = tr("No firmware releases available. Check your internet connection.");
            return false;
        }
    }
    else
    {
        if (m_localFirmwarePath.isEmpty() || !QFile::exists(m_localFirmwarePath))
        {
            error = tr("Please select a valid firmware file.");
            return false;
        }
    }

    return true;
}

ProvisioningConfig DialogFlashDevice::buildProvisioningConfig() const
{
    ProvisioningConfig cfg;

    cfg.networkInterface = (ui->comboNetInterface->currentIndex() == 1) ? "wifi" : "ethernet";
    cfg.ipMode = (ui->comboIpConfig->currentIndex() == 1) ? "static" : "dhcp";

    if (cfg.networkInterface == "wifi")
    {
        cfg.wifiSsid = ui->editWifiSsid->text().trimmed();
        cfg.wifiPassword = ui->editWifiPassword->text();
    }

    if (cfg.ipMode == "static")
    {
        cfg.staticIp = ui->editStaticIp->text().trimmed();
        cfg.staticMask = ui->editStaticMask->text().trimmed();
        cfg.staticGateway = ui->editStaticGateway->text().trimmed();
        cfg.staticDns = ui->editStaticDns->text().trimmed();
    }

    cfg.autoDiscovery = ui->checkAutoDiscovery->isChecked();
    if (!cfg.autoDiscovery)
    {
        cfg.serverHost = ui->editServerHost->text().trimmed();
        cfg.serverPort = ui->spinServerPort->value();
        cfg.serverSsl = ui->checkSsl->isChecked();
    }

    return cfg;
}

void DialogFlashDevice::onFlashClicked()
{
    QString error;
    if (!validateForm(error))
    {
        QMessageBox::warning(this, tr("Validation Error"), error);
        return;
    }

    startFlashPreflight();
}

void DialogFlashDevice::onSaveConfigClicked()
{
    QString error;
    if (!validateForm(error))
    {
        QMessageBox::warning(this, tr("Validation Error"), error);
        return;
    }

    ProvisioningConfig cfg = buildProvisioningConfig();
    QByteArray image = ConfigImageBuilder::buildImage(cfg);

    QString path = QFileDialog::getSaveFileName(
        this, tr("Save config file"), "config.bin",
        tr("Binary files (*.bin)"));

    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("Cannot write to %1").arg(path));
        return;
    }
    file.write(image);
    file.close();

    QMessageBox::information(this, tr("Config saved"),
                             tr("Configuration file saved successfully."));
}

void DialogFlashDevice::onOpenInstructions()
{
    int idx = ui->comboBoard->currentIndex();
    if (idx < 0 || idx >= m_boards.size())
        return;

    const BoardProfile &board = m_boards.at(idx);
    if (!board.flash.unsupportedOsDocUrl.isEmpty())
        QDesktopServices::openUrl(QUrl(board.flash.unsupportedOsDocUrl));
}

void DialogFlashDevice::setFlashUiBusy(bool busy)
{
    ui->buttonFlash->setEnabled(!busy);
    ui->buttonSaveConfig->setEnabled(!busy);
    ui->comboBoard->setEnabled(!busy);
    ui->progressFirmware->setVisible(busy);
    if (!busy)
        ui->progressFirmware->setValue(0);
}

void DialogFlashDevice::startFlashPreflight()
{
    int idx = ui->comboBoard->currentIndex();
    if (idx < 0 || idx >= m_boards.size())
        return;

    const BoardProfile &board = m_boards.at(idx);

    // Check tool availability first
    QString toolPath = m_toolManager->findTool(board.flash);
    if (toolPath.isEmpty())
    {
        if (board.flash.toolGithubRelease || board.flash.hasDirectDownloadUrl())
        {
            // Offer to auto-download
            auto answer = QMessageBox::question(
                this, tr("Download required"),
                tr("%1 is required for flashing. Download now?").arg(board.flash.tool),
                QMessageBox::Yes | QMessageBox::No);

            if (answer != QMessageBox::Yes)
                return;

            setFlashUiBusy(true);
            m_toolManager->downloadTool(board.flash,
                [this](bool success, const QString &path)
            {
                if (!success || path.isEmpty())
                {
                    setFlashUiBusy(false);
                    QMessageBox::critical(this, tr("Download failed"),
                                          tr("Failed to download the flash tool. Check your internet connection."));
                    return;
                }
                updateToolStatus();
                // Continue pre-flight after tool download
                startFlashPreflight();
            });
            return;
        }
        else
        {
            QMessageBox::warning(this, tr("Tool not found"),
                tr("%1 was not found. Use the Browse button to locate it, or visit the download page.")
                    .arg(board.flash.tool));
            return;
        }
    }

    // Get firmware ZIP path
    if (ui->radioDownload->isChecked())
    {
        int relIdx = ui->comboFirmwareVersion->currentIndex();
        if (relIdx < 0 || relIdx >= m_releases.size())
            return;

        const FirmwareRelease &release = m_releases.at(relIdx);
        setFlashUiBusy(true);

        m_firmwareManager->downloadFirmware(release,
            [this](bool success, const QString &zipPath)
        {
            if (!success)
            {
                setFlashUiBusy(false);
                QMessageBox::critical(this, tr("Download failed"),
                                      tr("Failed to download firmware. Check your internet connection."));
                return;
            }
            proceedWithFirmwareZip(zipPath);
        });
    }
    else
    {
        setFlashUiBusy(true);
        proceedWithFirmwareZip(m_localFirmwarePath);
    }
}

void DialogFlashDevice::proceedWithFirmwareZip(const QString &zipPath)
{
    int idx = ui->comboBoard->currentIndex();
    if (idx < 0 || idx >= m_boards.size())
    {
        setFlashUiBusy(false);
        return;
    }

    const BoardProfile &board = m_boards.at(idx);

    // Clean up previous extraction
    delete m_extractDir;
    m_extractDir = new QTemporaryDir();
    if (!m_extractDir->isValid())
    {
        setFlashUiBusy(false);
        QMessageBox::critical(this, tr("Error"),
                              tr("Failed to create temporary directory."));
        return;
    }

    FirmwareManifest manifest;
    QString error;
    if (!m_firmwareManager->extractAndValidate(zipPath, board, manifest, *m_extractDir, error))
    {
        setFlashUiBusy(false);
        QMessageBox::critical(this, tr("Firmware error"), error);
        return;
    }

    // Generate config.bin and replace default in extracted dir
    ProvisioningConfig cfg = buildProvisioningConfig();
    QByteArray configImage = ConfigImageBuilder::buildImage(cfg);

    QString configPath = m_extractDir->path() + "/" + board.configPartitionName;
    QFile configFile(configPath);
    if (!configFile.open(QIODevice::WriteOnly))
    {
        setFlashUiBusy(false);
        QMessageBox::critical(this, tr("Error"),
                              tr("Failed to write config image."));
        return;
    }
    configFile.write(configImage);
    configFile.close();

    launchFlash(m_extractDir->path(), manifest);
}

void DialogFlashDevice::launchFlash(const QString &firmwareDir, const FirmwareManifest &manifest)
{
    int idx = ui->comboBoard->currentIndex();
    if (idx < 0 || idx >= m_boards.size())
    {
        setFlashUiBusy(false);
        return;
    }

    const BoardProfile &board = m_boards.at(idx);
    QString toolPath = m_toolManager->findTool(board.flash);

    if (toolPath.isEmpty())
    {
        setFlashUiBusy(false);
        QMessageBox::critical(this, tr("Error"),
                              tr("Flash tool not found."));
        return;
    }

    DialogFlashConsole *console = new DialogFlashConsole(this);

    if (board.platform == "esp32")
    {
        // Build esptool write_flash command from manifest
        QStringList writeArgs;
        writeArgs << "--chip" << manifest.esptool.chip;
        writeArgs << "--port" << ui->comboSerialPort->currentData().toString();
        writeArgs << "--baud" << QString::number(manifest.esptool.baudrate);
        writeArgs << "--before" << manifest.esptool.before;
        writeArgs << "--after" << manifest.esptool.after;
        writeArgs << "write_flash";
        writeArgs << "--flash_mode" << manifest.esptool.flashMode;
        writeArgs << "--flash_freq" << manifest.esptool.flashFreq;
        writeArgs << "--flash_size" << manifest.esptool.flashSize;

        for (const auto &bin : manifest.binaries)
        {
            writeArgs << bin.offset;
            writeArgs << firmwareDir + "/" + bin.filename;
        }

        if (ui->checkEraseFlash->isChecked())
        {
            // Erase entire flash first, then write firmware
            QStringList eraseArgs;
            eraseArgs << "--chip" << manifest.esptool.chip;
            eraseArgs << "--port" << ui->comboSerialPort->currentData().toString();
            eraseArgs << "--baud" << QString::number(manifest.esptool.baudrate);
            eraseArgs << "--before" << manifest.esptool.before;
            eraseArgs << "--after" << "no_reset";
            eraseArgs << "erase_flash";

            QList<QPair<QString, QStringList>> commands;
            commands.append(qMakePair(toolPath, eraseArgs));
            commands.append(qMakePair(toolPath, writeArgs));
            console->startFlashSequence(commands, board.name);
        }
        else
        {
            console->startFlash(toolPath, writeArgs, board.name);
        }
    }
    else if (board.platform == "linux")
    {
        // Rockchip: two sequential commands via pkexec
        QList<QPair<QString, QStringList>> commands;

        // Find update.img in the firmware dir
        QString updateImg;
        QDir dir(firmwareDir);
        QStringList imgFiles = dir.entryList(QStringList() << "*.img", QDir::Files);
        if (!imgFiles.isEmpty())
            updateImg = firmwareDir + "/" + imgFiles.first();

        if (updateImg.isEmpty())
        {
            setFlashUiBusy(false);
            delete console;
            QMessageBox::critical(this, tr("Error"),
                                  tr("No update.img found in firmware package."));
            return;
        }

        QString configBin = firmwareDir + "/" + board.configPartitionName;

        // Command 1: flash full firmware
        commands.append(qMakePair(
            QStringLiteral("pkexec"),
            QStringList() << toolPath << "uf" << updateImg));

        // Command 2: flash config partition
        commands.append(qMakePair(
            QStringLiteral("pkexec"),
            QStringList() << toolPath << "di" << "-p"
                          << board.configPartitionName << configBin));

        console->startFlashSequence(commands, board.name);
    }

    connect(console, &DialogFlashConsole::flashFinished,
            this, [this](bool success)
    {
        Q_UNUSED(success)
        setFlashUiBusy(false);
    });

    console->exec();
    console->deleteLater();
    setFlashUiBusy(false);
}
