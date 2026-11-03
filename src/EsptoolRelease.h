#ifndef ESPTOOLRELEASE_H
#define ESPTOOLRELEASE_H

#include <QJsonArray>
#include <QString>

// Picks the esptool asset for a platform out of GitHub "releases" API answers. Nothing is
// pinned: the SHA-256 comes from the `digest` field the API gives for each asset, and the
// installer compares it with the downloaded file before extracting.
// Upper bound on an announced archive size: esptool builds weigh a few tens of MB.
constexpr qint64 kMaxToolArchiveBytes = 256LL * 1024 * 1024;
// Upper bound on the files of an archive once unpacked (a compressed bomb stays small on the wire).
constexpr qint64 kMaxExtractedBytes = 1024LL * 1024 * 1024;

struct EsptoolAsset
{
    QString tag;
    QString name;
    QString url;
    QString sha256; // lowercase hex, from the API digest
    qint64 size = 0; // bytes announced by the API; the download may not exceed it
    QString error;  // user-facing reason when no usable asset was found

    bool isValid() const { return error.isEmpty() && !url.isEmpty() && !sha256.isEmpty() && size > 0; }
};

// releases: the array answered by /repos/<owner>/<repo>/releases, in any order.
// Drafts and prereleases are skipped; the highest version (leading "v" ignored) holding an asset
// for the platform wins, older versions serve as fallback in decreasing order.
// os: "linux", "macos" or "windows"; cpu: QSysInfo::currentCpuArchitecture() value.
EsptoolAsset selectEsptoolAsset(const QJsonArray &releases, const QString &os, const QString &cpu);

// Same, for the platform the installer runs on.
EsptoolAsset selectEsptoolAssetForHost(const QJsonArray &releases);

// True when `name` can be used as a plain file name inside a directory: no separator, no "..",
// no drive or stream colon, no control character.
bool isSafeAssetName(const QString &name);

// True when `candidate` is a newer version than `cached`. An unknown or unparsable cached
// version (empty, "latest") counts as older; an unparsable candidate never wins.
bool isNewerEsptoolVersion(const QString &candidate, const QString &cached);

#endif // ESPTOOLRELEASE_H
