#include "FlashToolManager.h"
#include "HttpClient.h"
#include "ConfigOptions.h"
#include "EsptoolRelease.h"
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <KTar>
#include <KZip>

FlashToolManager::FlashToolManager(QObject *parent)
    : QObject(parent)
    , m_http(new HttpClient(this))
{
    connect(m_http, &HttpClient::downloadProgress,
            this, &FlashToolManager::downloadProgress);
}

QString FlashToolManager::toolsCacheDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/tools";
}

QString FlashToolManager::cachedToolVersion(const QString &toolName) const
{
    QString versionFile = toolsCacheDir() + "/" + toolName + "/version.txt";
    QFile f(versionFile);
    if (f.open(QIODevice::ReadOnly))
        return QString::fromUtf8(f.readAll()).trimmed();
    return QString();
}

void FlashToolManager::saveCustomToolPath(const QString &toolName, const QString &path)
{
    ConfigOptions::Instance().setOption("tools/" + toolName + "_path", path);
}

QString FlashToolManager::loadCustomToolPath(const QString &toolName) const
{
    return ConfigOptions::Instance().getOption("tools/" + toolName + "_path").toString();
}

QString FlashToolManager::findInPath(const QString &toolName) const
{
    // Search system PATH
    QString pathEnv = qEnvironmentVariable("PATH");
#ifdef Q_OS_WIN
    QStringList dirs = pathEnv.split(';', Qt::SkipEmptyParts);
#else
    QStringList dirs = pathEnv.split(':', Qt::SkipEmptyParts);
#endif

    // Also search common locations
    dirs << "/usr/local/bin"
         << "/usr/bin"
         << QDir::homePath() + "/.local/bin";

#ifdef Q_OS_WIN
    QString exeName = toolName + ".exe";
#else
    QString exeName = toolName;
#endif

    for (const QString &dir : dirs)
    {
        QString candidate = dir + "/" + exeName;
        QFileInfo fi(candidate);
        if (fi.exists() && fi.isExecutable())
            return fi.absoluteFilePath();
    }

    return QString();
}

QString FlashToolManager::findTool(const FlashConfig &config) const
{
    QString toolName = config.tool;

#ifdef Q_OS_WIN
    QString exeName = toolName + ".exe";
#else
    QString exeName = toolName;
#endif

    // 1. Check saved custom path
    QString customPath = loadCustomToolPath(toolName);
    if (!customPath.isEmpty())
    {
        QFileInfo fi(customPath);
        if (fi.exists() && fi.isExecutable())
            return fi.absoluteFilePath();
    }

    // 2. Check cached download
    QString cachedPath = toolsCacheDir() + "/" + toolName + "/" + exeName;
    QFileInfo cachedFi(cachedPath);
    if (cachedFi.exists() && cachedFi.isExecutable())
        return cachedFi.absoluteFilePath();

    // 3. Search PATH and common locations
    QString pathResult = findInPath(toolName);
    if (!pathResult.isEmpty())
        return pathResult;

    return QString();
}

void FlashToolManager::downloadTool(const FlashConfig &config,
                                    std::function<void(bool, const QString &)> callback)
{
    if (config.toolGithubRelease)
    {
        downloadToolGithub(config, callback);
    }
    else if (config.hasDirectDownloadUrl())
    {
        downloadToolDirect(config, callback);
    }
    else
    {
        qWarning() << "No download URL available for tool:" << config.tool;
        callback(false, QString());
    }
}

void FlashToolManager::downloadToolGithub(const FlashConfig &config,
                                          std::function<void(bool, const QString &)> callback)
{
    fetchLatestAsset(config, [this, config, callback](const EsptoolAsset &asset, const QString &error)
    {
        if (!asset.isValid())
        {
            callback(false, error);
            return;
        }
        installAsset(config, asset, callback);
    });
}

void FlashToolManager::updateCachedTool(const FlashConfig &config,
                                        std::function<bool(const QString &, const QString &)> confirm,
                                        std::function<void(bool, const QString &)> callback)
{
    const QString path = findTool(config);
    const QString cacheDir = QDir::fromNativeSeparators(toolsCacheDir()) + "/";
    if (!config.toolGithubRelease || path.isEmpty()
        || !QDir::fromNativeSeparators(path).startsWith(cacheDir))
    {
        callback(false, path);
        return;
    }

    const QString cachedVersion = cachedToolVersion(config.tool);
    fetchLatestAsset(config, [this, config, path, cachedVersion, confirm, callback](const EsptoolAsset &asset,
                                                                                    const QString &error)
    {
        // A failed check must not stop the user from flashing with what is already installed.
        if (!asset.isValid())
        {
            qWarning() << "Tool update check failed:" << error;
            callback(false, path);
            return;
        }
        if (!isNewerEsptoolVersion(asset.tag, cachedVersion))
        {
            callback(false, path);
            return;
        }
        if (!confirm(asset.tag, cachedVersion))
        {
            callback(false, path);
            return;
        }
        installAsset(config, asset, [path, callback](bool ok, const QString &newPath)
        {
            if (!ok)
                qWarning() << "Tool update failed:" << newPath;
            callback(ok, ok ? newPath : path);
        });
    });
}

void FlashToolManager::fetchLatestAsset(const FlashConfig &config,
                                        std::function<void(const EsptoolAsset &, const QString &)> callback)
{
    const QUrl repo(config.toolUrl);
    if (repo.host() != "github.com" || repo.path().count('/') < 2)
    {
        EsptoolAsset none;
        callback(none, tr("%1 is not a GitHub repository").arg(config.tool));
        return;
    }

    QString repoPath = repo.path();
    if (repoPath.endsWith('/'))
        repoPath.chop(1);
    // A few releases back, in case the newest one lacks a build for this platform.
    const QUrl apiUrl("https://api.github.com/repos" + repoPath + "/releases?per_page=5");

    m_http->getJson(apiUrl, [this, callback](bool ok, const QJsonDocument &doc)
    {
        EsptoolAsset none;
        if (!ok)
        {
            const int status = m_http->lastStatus();
            if (status == 403 || status == 429)
                callback(none, tr("GitHub refused the request (HTTP %1): the API rate limit is probably "
                                  "reached, try again later.").arg(status));
            else
                callback(none, tr("Cannot query GitHub releases: %1").arg(m_http->lastError()));
            return;
        }
        if (!doc.isArray())
        {
            callback(none, tr("Unexpected answer from the GitHub releases API."));
            return;
        }

        const EsptoolAsset asset = selectEsptoolAssetForHost(doc.array());
        callback(asset, asset.error);
    }, "application/vnd.github+json");
}

void FlashToolManager::installAsset(const FlashConfig &config, const EsptoolAsset &asset,
                                    std::function<void(bool, const QString &)> callback)
{
    // Extract beside the cache and swap on success: a failed update keeps the working tool, and
    // stale files of the previous version cannot shadow the new binary.
    const QString toolDir = toolsCacheDir() + "/" + config.tool;
    const QString stageDir = toolDir + ".new";
    QDir(stageDir).removeRecursively();
    QDir().mkpath(stageDir);
    const QString archivePath = stageDir + "/" + asset.name;

    m_http->downloadFile(QUrl(asset.url), archivePath,
                         [this, config, toolDir, stageDir, archivePath, asset, callback](bool dlSuccess)
    {
        auto fail = [&](const QString &reason)
        {
            QDir(stageDir).removeRecursively();
            callback(false, reason);
        };

        if (!dlSuccess)
        {
            fail(m_http->lastError());
            return;
        }

        if (fileSha256(archivePath) != asset.sha256)
        {
            fail(tr("Checksum mismatch for %1: the download does not match the digest "
                    "published by GitHub.").arg(asset.name));
            return;
        }

        const QString stagedTool = extractArchive(archivePath, stageDir, config.tool);
        QFile::remove(archivePath);
        if (stagedTool.isEmpty())
        {
            fail(tr("Cannot extract %1").arg(asset.name));
            return;
        }

        QFile vf(stageDir + "/version.txt");
        if (!vf.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            fail(tr("Cannot write the tool version file"));
            return;
        }
        vf.write(asset.tag.toUtf8());
        vf.close();

        QDir(toolDir).removeRecursively();
        if (!QDir().rename(stageDir, toolDir))
        {
            fail(tr("Cannot install the tool into %1").arg(toolDir));
            return;
        }

        callback(true, toolDir + stagedTool.mid(stageDir.size()));
    });
}

QString FlashToolManager::fileSha256(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return QString();
    QCryptographicHash h(QCryptographicHash::Sha256);
    if (!h.addData(&f))
        return QString();
    return QString::fromLatin1(h.result().toHex());
}

void FlashToolManager::downloadToolDirect(const FlashConfig &config,
                                          std::function<void(bool, const QString &)> callback)
{
    QString url = config.resolvedToolUrl();
    if (url.isEmpty())
    {
        qWarning() << "No direct download URL for tool:" << config.tool;
        callback(false, QString());
        return;
    }

    QString toolDir = toolsCacheDir() + "/" + config.tool;
    QDir().mkpath(toolDir);

    QString archiveFileName = QUrl(url).fileName();
    QString archivePath = toolDir + "/" + archiveFileName;

    m_http->downloadFile(QUrl(url), archivePath,
                         [this, config, toolDir, archivePath, url, callback](bool dlSuccess)
    {
        if (!dlSuccess)
        {
            QFile::remove(archivePath);
            callback(false, m_http->lastError());
            return;
        }

        // Extract archive
        QString toolPath = extractArchive(archivePath, toolDir, config.tool);

        // Clean up archive
        QFile::remove(archivePath);

        if (toolPath.isEmpty())
        {
            callback(false, QString());
            return;
        }

        // Write version.txt with the URL as cache identifier
        QFile vf(toolDir + "/version.txt");
        if (vf.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            vf.write(url.toUtf8());
            vf.close();
        }

        callback(true, toolPath);
    });
}

QString FlashToolManager::extractArchive(const QString &archivePath, const QString &destDir,
                                         const QString &toolName) const
{
#ifdef Q_OS_WIN
    QString exeName = toolName + ".exe";
#else
    QString exeName = toolName;
#endif

    // Try tar.gz first (Linux/macOS esptool releases)
    if (archivePath.endsWith(".tar.gz") || archivePath.endsWith(".tgz"))
    {
        KTar tar(archivePath);
        if (!tar.open(QIODevice::ReadOnly))
        {
            qWarning() << "Cannot open tar archive:" << archivePath;
            return QString();
        }

        const KArchiveDirectory *root = tar.directory();
        root->copyTo(destDir, true);
        tar.close();
    }
    else if (archivePath.endsWith(".zip"))
    {
        KZip zip(archivePath);
        if (!zip.open(QIODevice::ReadOnly))
        {
            qWarning() << "Cannot open zip archive:" << archivePath;
            return QString();
        }

        const KArchiveDirectory *root = zip.directory();
        root->copyTo(destDir, true);
        zip.close();
    }
    else
    {
        qWarning() << "Unsupported archive format:" << archivePath;
        return QString();
    }

    // Find the tool binary in the extracted files
    // Archives may have a subfolder (e.g. esptool-v5.2.0/esptool, upgrade_tool_v2.17_for_linux/upgrade_tool)
    // Note: KZip/KTar copyTo() may not preserve executable permissions, so search
    // by name only (QDir::Files) and set +x afterwards.
    QDir dir(destDir);
    QStringList candidates = dir.entryList(QStringList() << exeName,
                                           QDir::Files,
                                           QDir::Name);
    if (!candidates.isEmpty())
    {
        QString path = dir.absoluteFilePath(candidates.first());
#ifndef Q_OS_WIN
        QFile::setPermissions(path, QFile::permissions(path)
                              | QFileDevice::ExeOwner | QFileDevice::ExeGroup | QFileDevice::ExeOther);
#endif
        return path;
    }

    // Search in subdirectories (one level deep)
    QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &subdir : subdirs)
    {
        QDir sub(dir.absoluteFilePath(subdir));
        QStringList subCandidates = sub.entryList(QStringList() << exeName,
                                                   QDir::Files,
                                                   QDir::Name);
        if (!subCandidates.isEmpty())
        {
            // Move tool binary (and LICENSE if present) up to destDir
            QString srcPath = sub.absoluteFilePath(subCandidates.first());
            QString dstPath = dir.absoluteFilePath(exeName);
            QFile::rename(srcPath, dstPath);

            // Copy LICENSE if present
            if (sub.exists("LICENSE"))
                QFile::copy(sub.absoluteFilePath("LICENSE"), dir.absoluteFilePath("LICENSE"));

#ifndef Q_OS_WIN
            // Ensure executable permission
            QFile::setPermissions(dstPath, QFile::permissions(dstPath)
                                  | QFileDevice::ExeOwner | QFileDevice::ExeGroup | QFileDevice::ExeOther);
#endif

            return dstPath;
        }
    }

    qWarning() << "Tool binary not found after extraction:" << exeName;
    return QString();
}
