#ifndef FLASHTOOLMANAGER_H
#define FLASHTOOLMANAGER_H

#include <QObject>
#include <QString>
#include <functional>
#include "BoardProfile.h"

class HttpClient;

class FlashToolManager : public QObject
{
    Q_OBJECT

public:
    explicit FlashToolManager(QObject *parent = nullptr);

    // Find tool on disk: saved path, PATH, common locations, cached download
    // Returns full path to executable, or empty string if not found
    QString findTool(const FlashConfig &config) const;

    // Read cached tool version from version.txt, or empty string
    QString cachedToolVersion(const QString &toolName) const;

    // Auto-download tool from GitHub releases or direct URL
    // Dispatches to GitHub release flow or direct download based on config
    void downloadTool(const FlashConfig &config,
                      std::function<void(bool success, const QString &toolPath)> callback);

    // Save/load custom tool path in QSettings
    void saveCustomToolPath(const QString &toolName, const QString &path);
    QString loadCustomToolPath(const QString &toolName) const;

    // Base directory for cached tools: <AppData>/tools/
    static QString toolsCacheDir();

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    // Download tool from GitHub releases API
    void downloadToolGithub(const FlashConfig &config,
                            std::function<void(bool, const QString &)> callback);

    // Download tool from a direct URL (non-GitHub)
    void downloadToolDirect(const FlashConfig &config,
                            std::function<void(bool, const QString &)> callback);

    // Select the correct release asset for current platform
    QString selectAssetUrl(const QJsonArray &assets) const;

    // Extract archive (tar.gz or zip) into destDir, returns path to tool binary
    QString extractArchive(const QString &archivePath, const QString &destDir,
                           const QString &toolName) const;

    // Search for executable in PATH and common locations
    QString findInPath(const QString &toolName) const;

    HttpClient *m_http;
};

#endif // FLASHTOOLMANAGER_H
