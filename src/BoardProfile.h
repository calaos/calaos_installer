#ifndef BOARDPROFILE_H
#define BOARDPROFILE_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>

struct FlashConfig
{
    QString tool;
    QString toolUrl;                        // GitHub repo URL (when tool_url is a string)
    QMap<QString, QString> toolUrls;         // Per-OS direct URLs (when tool_url is an object)
    bool toolGithubRelease = false;
    bool requiresSerialPort = false;
    QStringList supportedOs;
    QString unsupportedOsDocUrl;

    // Returns the download URL for the current OS.
    // If toolGithubRelease, returns toolUrl (GitHub repo).
    // Otherwise returns the matching entry from toolUrls.
    QString resolvedToolUrl() const;

    // True when a direct (non-GitHub) download URL exists for the current OS
    bool hasDirectDownloadUrl() const;
};

struct BoardProfile
{
    QString id;
    QString name;
    QString platform;
    QString configPartitionName;
    QString firmwareRepo;
    FlashConfig flash;

    bool isFlashSupportedOnCurrentOs() const;

    static QList<BoardProfile> loadFromFile(const QString &path);
    static QString boardsFilePath();
};

#endif // BOARDPROFILE_H
