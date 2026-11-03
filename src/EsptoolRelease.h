#ifndef ESPTOOLRELEASE_H
#define ESPTOOLRELEASE_H

#include <QJsonArray>
#include <QString>

// Picks the esptool asset for a platform out of GitHub "releases" API answers. Nothing is
// pinned: the SHA-256 comes from the `digest` field the API gives for each asset, and the
// installer compares it with the downloaded file before extracting.
struct EsptoolAsset
{
    QString tag;
    QString name;
    QString url;
    QString sha256; // lowercase hex, from the API digest
    QString error;  // user-facing reason when no usable asset was found

    bool isValid() const { return error.isEmpty() && !url.isEmpty() && !sha256.isEmpty(); }
};

// releases: the array answered by /repos/<owner>/<repo>/releases, newest first.
// Drafts and prereleases are skipped; the first release holding an asset for the platform wins.
// os: "linux", "macos" or "windows"; cpu: QSysInfo::currentCpuArchitecture() value.
EsptoolAsset selectEsptoolAsset(const QJsonArray &releases, const QString &os, const QString &cpu);

// Same, for the platform the installer runs on.
EsptoolAsset selectEsptoolAssetForHost(const QJsonArray &releases);

// True when `candidate` is a newer version than `cached`. An unknown or unparsable cached
// version (empty, "latest") counts as older; an unparsable candidate never wins.
bool isNewerEsptoolVersion(const QString &candidate, const QString &cached);

#endif // ESPTOOLRELEASE_H
