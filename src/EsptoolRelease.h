#ifndef ESPTOOLRELEASE_H
#define ESPTOOLRELEASE_H

#include <QString>

// Pinned esptool release: the installer downloads exactly this build and checks its SHA-256
// before extracting, so a moved or replaced asset can never be executed.
// Bump the tag and the hashes together (the GitHub release API exposes each asset's digest).
struct EsptoolAsset
{
    QString tag;
    QString name;
    QString url;
    QString sha256;

    bool isValid() const { return !url.isEmpty(); }
};

// os: "linux", "macos" or "windows"; cpu: QSysInfo::currentCpuArchitecture() value.
// Returns an invalid asset when no build exists for the combination.
EsptoolAsset esptoolAssetFor(const QString &os, const QString &cpu);

// Same, for the platform the installer runs on.
EsptoolAsset esptoolAssetForHost();

#endif // ESPTOOLRELEASE_H
