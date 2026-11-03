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
#include <KArchiveDirectory>
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
    // The release selection is written for esptool's archives only.
    if (config.tool != "esptool")
    {
        callback(false, tr("Automatic download is not supported for %1").arg(config.tool));
        return;
    }
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
                                        std::function<void(bool, const QString &, const QString &)> callback)
{
    const QString path = findTool(config);
    const QString cacheDir = QDir::fromNativeSeparators(toolsCacheDir()) + "/";
    if (!config.toolGithubRelease || config.tool != "esptool" || path.isEmpty()
        || !QDir::fromNativeSeparators(path).startsWith(cacheDir))
    {
        callback(false, path, QString());
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
            callback(false, path, QString());
            return;
        }
        const QString declinedKey = "tools/" + config.tool + "_declined_version";
        if (!isNewerEsptoolVersion(asset.tag, cachedVersion)
            || ConfigOptions::Instance().getOption(declinedKey).toString() == asset.tag)
        {
            callback(false, path, QString());
            return;
        }
        if (!confirm(asset.tag, cachedVersion))
        {
            ConfigOptions::Instance().setOption(declinedKey, asset.tag);
            callback(false, path, QString());
            return;
        }
        installAsset(config, asset, [path, callback](bool ok, const QString &newPath)
        {
            if (!ok)
                qWarning() << "Tool update failed:" << newPath;
            callback(ok, ok ? newPath : path, ok ? QString() : newPath);
        });
    });
}

void FlashToolManager::abort()
{
    m_http->abort();
}

bool FlashToolManager::renameDir(const QString &from, const QString &to)
{
    return QDir().rename(from, to);
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
    const QUrl apiUrl(m_apiBase + "/repos" + repoPath + "/releases?per_page=5");

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
    if (!isSafeAssetName(asset.name))
    {
        callback(false, tr("Refusing the unsafe asset name %1").arg(asset.name));
        return;
    }
    const QString toolDir = toolsCacheDir() + "/" + config.tool;
    const QString stageDir = toolDir + ".new";
    // A leftover would mix with the new files and could shadow the binary.
    QDir(stageDir).removeRecursively();
    if (QFileInfo::exists(stageDir))
    {
        callback(false, tr("Cannot clear the staging folder %1").arg(stageDir));
        return;
    }
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

        // Park the old install instead of deleting it, so a failed swap can put it back.
        const QString oldDir = toolDir + ".old";
        QDir(oldDir).removeRecursively();
        const bool hadOld = QFileInfo::exists(toolDir);
        if (hadOld && !renameDir(toolDir, oldDir))
        {
            fail(tr("Cannot replace the installed tool in %1").arg(toolDir));
            return;
        }
        if (!renameDir(stageDir, toolDir))
        {
            if (hadOld && !renameDir(oldDir, toolDir))
            {
                fail(tr("Cannot install the tool into %1, and the previous one could not be "
                        "put back: it is in %2.").arg(toolDir, oldDir));
                return;
            }
            fail(tr("Cannot install the tool into %1").arg(toolDir));
            return;
        }
        QDir(oldDir).removeRecursively();

        callback(true, toolDir + stagedTool.mid(stageDir.size()));
    }, asset.size);
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

        // upgrade_tool's 32-hex filename suffix is not the zip's MD5, so there is
        // no digest to verify here; only the size bound below applies.
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
    }, kMaxToolArchiveBytes);
}

bool FlashToolManager::archiveIsSafe(const KArchiveDirectory *dir, qint64 &budget)
{
    for (const QString &name : dir->entries())
    {
        const KArchiveEntry *e = dir->entry(name);
        if (!e || name.isEmpty() || name == ".." || name == "." || name.contains('/')
            || name.contains('\\') || name.contains(':') || !e->symLinkTarget().isEmpty())
            return false;
        if (e->isFile())
        {
            budget -= static_cast<const KArchiveFile *>(e)->size();
            if (budget < 0)
                return false;
        }
        if (e->isDirectory() && !archiveIsSafe(static_cast<const KArchiveDirectory *>(e), budget))
            return false;
    }
    return true;
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
        qint64 budget = m_maxExtractedBytes;
        if (!archiveIsSafe(root, budget))
        {
            qWarning() << "Unsafe entry in archive:" << archivePath;
            return QString();
        }
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
        qint64 budget = m_maxExtractedBytes;
        if (!archiveIsSafe(root, budget))
        {
            qWarning() << "Unsafe entry in archive:" << archivePath;
            return QString();
        }
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
