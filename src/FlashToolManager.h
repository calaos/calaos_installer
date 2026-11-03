#ifndef FLASHTOOLMANAGER_H
#define FLASHTOOLMANAGER_H

#include <QObject>
#include <QString>
#include <functional>
#include "BoardProfile.h"
#include "EsptoolRelease.h"

class HttpClient;
class KArchiveDirectory;

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

    // If findTool() resolves to a download made by us and a newer release exists, install it
    // once confirm(newVersion, cachedVersion) agrees; cachedVersion is empty or "latest" when
    // unknown. A refused version is remembered and not proposed again. The callback gets
    // (true, new path, "") after an update, else (false, current path, error); error is set
    // only when the user agreed and the update failed. A failed check, a refusal or a failed
    // update keeps the current tool.
    void updateCachedTool(const FlashConfig &config,
                          std::function<bool(const QString &newVersion, const QString &cachedVersion)> confirm,
                          std::function<void(bool updated, const QString &toolPath, const QString &error)> callback);

    // Cancel the check or download in progress; its callback reports "Cancelled"
    void abort();

    // Download, verify the SHA-256, extract and install an asset into the tool cache. The
    // previous install stays in place unless the new one is complete.
    void installAsset(const FlashConfig &config, const EsptoolAsset &asset,
                      std::function<void(bool, const QString &)> callback);

    // Replace the GitHub API root (tests only)
    void setApiBaseUrl(const QString &url) { m_apiBase = url; }

    // Save/load custom tool path in QSettings
    void saveCustomToolPath(const QString &toolName, const QString &path);
    QString loadCustomToolPath(const QString &toolName) const;

    // Base directory for cached tools: <AppData>/tools/
    static QString toolsCacheDir();

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

protected:
    // Seam for tests: rename a directory
    virtual bool renameDir(const QString &from, const QString &to);

private:
    // Download tool from GitHub releases API
    void downloadToolGithub(const FlashConfig &config,
                            std::function<void(bool, const QString &)> callback);

    // Ask the GitHub releases API for the newest asset fitting this host; on failure the
    // asset is invalid and the string is the reason.
    void fetchLatestAsset(const FlashConfig &config,
                          std::function<void(const EsptoolAsset &, const QString &)> callback);

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

    // False if an entry could write outside the destination (".." or separator in a name,
    // symlink)
    static bool archiveIsSafe(const KArchiveDirectory *dir);

    HttpClient *m_http;
    QString m_apiBase = "https://api.github.com";
};

#endif // FLASHTOOLMANAGER_H
