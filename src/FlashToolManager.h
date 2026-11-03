#ifndef FLASHTOOLMANAGER_H
#define FLASHTOOLMANAGER_H

#include <QObject>
#include <QString>
#include <functional>
#include "BoardProfile.h"
#include "EsptoolRelease.h"

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
    // Dispatches to GitHub release flow or direct download based on config.
    // On failure the callback's string is a user-facing reason (no URL).
    void downloadTool(const FlashConfig &config,
                      std::function<void(bool success, const QString &toolPath)> callback);

    // If findTool() resolves to a download made by us and a newer release exists, install it.
    // The callback gets (true, new path) after an update, else (false, current path); a failed
    // check or update keeps the current tool.
    void updateCachedTool(const FlashConfig &config,
                          std::function<void(bool updated, const QString &toolPath)> callback);

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

    // Ask the GitHub releases API for the newest asset fitting this host; on failure the
    // asset is invalid and the string is the reason.
    void fetchLatestAsset(const FlashConfig &config,
                          std::function<void(const EsptoolAsset &, const QString &)> callback);

    // Download, verify the SHA-256, extract and install an asset into the tool cache
    void installAsset(const FlashConfig &config, const EsptoolAsset &asset,
                      std::function<void(bool, const QString &)> callback);

    // Download tool from a direct URL (non-GitHub)
    void downloadToolDirect(const FlashConfig &config,
                            std::function<void(bool, const QString &)> callback);

    // Lowercase hex SHA-256 of a file, empty if unreadable
    static QString fileSha256(const QString &path);

    // Extract archive (tar.gz or zip) into destDir, returns path to tool binary
    QString extractArchive(const QString &archivePath, const QString &destDir,
                           const QString &toolName) const;

    // Search for executable in PATH and common locations
    QString findInPath(const QString &toolName) const;

    HttpClient *m_http;
};

#endif // FLASHTOOLMANAGER_H
