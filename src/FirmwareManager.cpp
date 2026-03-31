#include "FirmwareManager.h"
#include "HttpClient.h"
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QUrl>
#include <KZip>

FirmwareManager::FirmwareManager(QObject *parent)
    : QObject(parent)
    , m_http(new HttpClient(this))
{
    connect(m_http, &HttpClient::downloadProgress,
            this, &FirmwareManager::downloadProgress);
}

QString FirmwareManager::firmwareCacheDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/firmware";
}

void FirmwareManager::fetchReleases(const BoardProfile &board,
                                    std::function<void(bool, const QList<FirmwareRelease> &)> callback)
{
    // Convert firmware_repo URL to GitHub API
    // e.g. "https://github.com/calaos/calaos_remote_ui" ->
    //      "https://api.github.com/repos/calaos/calaos_remote_ui/releases"
    QUrl repoUrl(board.firmwareRepo);
    QString path = repoUrl.path();
    if (path.endsWith('/'))
        path.chop(1);
    QString apiUrl = "https://api.github.com/repos" + path + "/releases";

    m_http->getJson(QUrl(apiUrl), [board, callback](bool success, const QJsonDocument &doc)
    {
        if (!success)
        {
            callback(false, {});
            return;
        }

        QList<FirmwareRelease> releases;
        QJsonArray arr = doc.array();

        for (const QJsonValue &val : arr)
        {
            QJsonObject obj = val.toObject();
            QString tagName = obj.value("tag_name").toString();

            // Filter: tag must start with board id
            if (!tagName.startsWith(board.id))
                continue;

            // Extract version from tag: "waveshare-86-panel-0.0.1-dev.10" -> "0.0.1-dev.10"
            QString version = tagName.mid(board.id.length() + 1); // skip "boardid-"

            // Find the flash ZIP asset
            QString assetUrl;
            QJsonArray assets = obj.value("assets").toArray();
            for (const QJsonValue &aVal : assets)
            {
                QJsonObject asset = aVal.toObject();
                QString name = asset.value("name").toString();
                if (name.endsWith("-flash.zip"))
                {
                    assetUrl = asset.value("browser_download_url").toString();
                    break;
                }
            }

            if (assetUrl.isEmpty())
                continue;

            FirmwareRelease rel;
            rel.tagName = tagName;
            rel.version = version;
            rel.date = obj.value("published_at").toString();
            rel.assetUrl = assetUrl;
            releases.append(rel);
        }

        // Already sorted newest first by GitHub API
        callback(true, releases);
    });
}

void FirmwareManager::downloadFirmware(const FirmwareRelease &release,
                                       std::function<void(bool, const QString &)> callback)
{
    QString cacheDir = firmwareCacheDir();
    QDir().mkpath(cacheDir);

    // Use tag name as filename: e.g. "waveshare-86-panel-0.0.1-dev.10-flash.zip"
    QString fileName = release.tagName + "-flash.zip";
    QString filePath = cacheDir + "/" + fileName;

    // If already cached, reuse
    if (QFile::exists(filePath))
    {
        callback(true, filePath);
        return;
    }

    m_http->downloadFile(QUrl(release.assetUrl), filePath,
                         [filePath, callback](bool success)
    {
        if (!success)
        {
            QFile::remove(filePath);
            callback(false, QString());
            return;
        }
        callback(true, filePath);
    });
}

bool FirmwareManager::extractAndValidate(const QString &zipPath, const BoardProfile &board,
                                         FirmwareManifest &manifest, QTemporaryDir &tempDir,
                                         QString &error)
{
    if (!tempDir.isValid())
    {
        error = tr("Cannot create temporary directory.");
        return false;
    }

    // Extract ZIP
    KZip zip(zipPath);
    if (!zip.open(QIODevice::ReadOnly))
    {
        error = tr("Cannot open firmware ZIP file.");
        return false;
    }

    const KArchiveDirectory *root = zip.directory();
    root->copyTo(tempDir.path(), true);
    zip.close();

    // Read manifest.json
    QString manifestPath = tempDir.path() + "/manifest.json";
    QFile mf(manifestPath);
    if (!mf.open(QIODevice::ReadOnly))
    {
        error = tr("Firmware ZIP does not contain manifest.json.");
        return false;
    }

    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(mf.readAll(), &parseErr);
    mf.close();

    if (parseErr.error != QJsonParseError::NoError)
    {
        error = tr("manifest.json parse error: %1").arg(parseErr.errorString());
        return false;
    }

    manifest = FirmwareManifest::fromJson(doc.object());

    // Validate schema version
    if (manifest.schemaVersion != 1)
    {
        error = tr("Unsupported firmware manifest version: %1").arg(manifest.schemaVersion);
        return false;
    }

    // Validate board match
    if (manifest.board != board.id)
    {
        error = tr("This firmware is for board \"%1\", but you selected board \"%2\".")
                    .arg(manifest.board, board.id);
        return false;
    }

    // Verify SHA-256 checksums (skip config partition)
    for (const BinaryEntry &be : manifest.binaries)
    {
        if (be.filename == board.configPartitionName)
            continue;

        if (be.checksumSha256.isEmpty())
            continue;

        QString binPath = tempDir.path() + "/" + be.filename;
        QFile bf(binPath);
        if (!bf.open(QIODevice::ReadOnly))
        {
            error = tr("Missing binary file: %1").arg(be.filename);
            return false;
        }

        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(&bf);
        bf.close();

        QString computed = hash.result().toHex();
        if (computed != be.checksumSha256)
        {
            error = tr("Checksum mismatch for %1. Firmware may be corrupted.")
                        .arg(be.filename);
            return false;
        }
    }

    return true;
}

void FirmwareManager::abort()
{
    m_http->abort();
}
