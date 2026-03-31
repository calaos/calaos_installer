#include "FlashToolManager.h"
#include "HttpClient.h"
#include "ConfigOptions.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <QSysInfo>
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
    // Convert GitHub repo URL to API URL for latest release
    // e.g. "https://github.com/espressif/esptool" ->
    //      "https://api.github.com/repos/espressif/esptool/releases/latest"
    QString repoUrl = config.toolUrl;
    QString apiUrl;

    if (repoUrl.contains("github.com"))
    {
        // Extract owner/repo from URL
        QUrl parsed(repoUrl);
        QString path = parsed.path(); // e.g. "/espressif/esptool"
        if (path.endsWith('/'))
            path.chop(1);
        apiUrl = "https://api.github.com/repos" + path + "/releases/latest";
    }
    else
    {
        qWarning() << "Tool URL is not a GitHub repo:" << repoUrl;
        callback(false, QString());
        return;
    }

    m_http->getJson(QUrl(apiUrl), [this, config, callback](bool success, const QJsonDocument &doc)
    {
        if (!success)
        {
            callback(false, QString());
            return;
        }

        QJsonObject release = doc.object();
        QString tagName = release.value("tag_name").toString();
        QJsonArray assets = release.value("assets").toArray();

        QString assetUrl = selectAssetUrl(assets);
        if (assetUrl.isEmpty())
        {
            qWarning() << "No matching asset found for current platform";
            callback(false, QString());
            return;
        }

        // Prepare destination directory
        QString toolDir = toolsCacheDir() + "/" + config.tool;
        QDir().mkpath(toolDir);

        // Download asset to temp file in tool directory
        QString assetFileName = QUrl(assetUrl).fileName();
        QString archivePath = toolDir + "/" + assetFileName;

        m_http->downloadFile(QUrl(assetUrl), archivePath,
                             [this, config, toolDir, archivePath, tagName, callback](bool dlSuccess)
        {
            if (!dlSuccess)
            {
                QFile::remove(archivePath);
                callback(false, QString());
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

            // Write version.txt
            QFile vf(toolDir + "/version.txt");
            if (vf.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                vf.write(tagName.toUtf8());
                vf.close();
            }

            callback(true, toolPath);
        });
    });
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
            callback(false, QString());
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

QString FlashToolManager::selectAssetUrl(const QJsonArray &assets) const
{
    QString osStr;
    QString archStr;

#if defined(Q_OS_LINUX)
    osStr = "linux";
#elif defined(Q_OS_MACOS)
    osStr = "macos";
#elif defined(Q_OS_WIN)
    osStr = "win";
#endif

    QString cpuArch = QSysInfo::currentCpuArchitecture();
    if (cpuArch == "x86_64")
        archStr = "amd64";
    else if (cpuArch == "arm64" || cpuArch == "aarch64")
        archStr = "arm64";
    else
        archStr = cpuArch;

#ifdef Q_OS_WIN
    // Windows assets use "win64" pattern
    QString pattern = osStr + "64";
#else
    QString pattern = osStr + "-" + archStr;
#endif

    for (const QJsonValue &val : assets)
    {
        QJsonObject asset = val.toObject();
        QString name = asset.value("name").toString();
        QString downloadUrl = asset.value("browser_download_url").toString();

        if (name.contains(pattern, Qt::CaseInsensitive))
            return downloadUrl;
    }

    return QString();
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
