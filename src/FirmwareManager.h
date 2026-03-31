#ifndef FIRMWAREMANAGER_H
#define FIRMWAREMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QTemporaryDir>
#include <functional>
#include "BoardProfile.h"
#include "FirmwareManifest.h"

class HttpClient;

struct FirmwareRelease
{
    QString tagName;
    QString version;
    QString date;
    QString assetUrl;
};

class FirmwareManager : public QObject
{
    Q_OBJECT

public:
    explicit FirmwareManager(QObject *parent = nullptr);

    // Fetch available releases from GitHub for a given board
    void fetchReleases(const BoardProfile &board,
                       std::function<void(bool success, const QList<FirmwareRelease> &releases)> callback);

    // Download firmware ZIP to cache directory
    // Returns cached path if already downloaded
    void downloadFirmware(const FirmwareRelease &release,
                          std::function<void(bool success, const QString &zipPath)> callback);

    // Extract ZIP to temp dir, validate manifest + checksums
    // On success, sets manifest and returns path to extracted directory
    // tempDir must outlive the flash operation
    bool extractAndValidate(const QString &zipPath, const BoardProfile &board,
                            FirmwareManifest &manifest, QTemporaryDir &tempDir,
                            QString &error);

    // Abort ongoing download
    void abort();

    // Cache directory for firmware downloads
    static QString firmwareCacheDir();

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    HttpClient *m_http;
};

#endif // FIRMWAREMANAGER_H
