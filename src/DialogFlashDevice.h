#ifndef DIALOGFLASHDEVICE_H
#define DIALOGFLASHDEVICE_H

#include <QDialog>
#include <QTimer>
#include <QTemporaryDir>
#include "BoardProfile.h"
#include "ConfigImageBuilder.h"
#include "FirmwareManifest.h"
#include "FirmwareManager.h"
#include "FlashToolManager.h"

namespace Ui {
class DialogFlashDevice;
}

class DialogFlashDevice : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFlashDevice(QWidget *parent = nullptr);
    ~DialogFlashDevice();

private slots:
    void onBoardChanged(int index);
    void onFirmwareSourceChanged();
    void onBrowseFirmware();
    void onNetInterfaceChanged(int index);
    void onIpConfigChanged(int index);
    void onAutoDiscoveryToggled(bool checked);
    void onRefreshSerialPorts();
    void onBrowseTool();
    void onFlashClicked();
    void onSaveConfigClicked();
    void onOpenInstructions();

private:
    void loadBoards();
    void updateGuiForBoard(const BoardProfile &board);
    void updateWifiFieldsVisibility();
    void updateStaticFieldsVisibility();
    void updateServerFieldsVisibility();
    void updateToolStatus();
    void refreshSerialPorts();
    void fetchFirmwareVersions();
    bool validateForm(QString &error);
    bool isValidIpv4(const QString &addr) const;
    ProvisioningConfig buildProvisioningConfig() const;

    // Flash pre-flight: download/validate firmware, generate config, launch flash
    void startFlashPreflight();
    void proceedWithFirmwareZip(const QString &zipPath);
    void launchFlash(const QString &firmwareDir, const FirmwareManifest &manifest);
    void setFlashUiBusy(bool busy);

    Ui::DialogFlashDevice *ui;
    QList<BoardProfile> m_boards;
    QList<FirmwareRelease> m_releases;
    QString m_localFirmwarePath;

    FirmwareManager *m_firmwareManager;
    FlashToolManager *m_toolManager;
    QTimer *m_serialRefreshTimer;

    // Keeps extracted firmware alive during flash
    QTemporaryDir *m_extractDir = nullptr;
};

#endif // DIALOGFLASHDEVICE_H
