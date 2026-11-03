#include "EsptoolRelease.h"

#include <QSysInfo>

namespace {

const char *const kTag = "v5.6.0";

struct Entry
{
    const char *key; // "<os>-<arch>", as in the asset file name
    const char *ext;
    const char *sha256;
};

const Entry kEntries[] = {
    { "linux-amd64",   "tar.gz", "1590723cfa9675983f49d0bf2895b8507f8d2879082e51bb7ef02a8bca88d354" },
    { "linux-aarch64", "tar.gz", "7723120429394b4d6d974d694e5cf378e00259381a7f8856c2c88492041b9f59" },
    { "linux-armv7",   "tar.gz", "65e2f91fba2e79a75243454ded44da57532c74e242e17f9890aa7fe6cd588ee4" },
    { "macos-amd64",   "tar.gz", "23921776a7df75f2e6f3ca10a604d74db4661e1b8a0ebb89bdddcd2d26f37055" },
    { "macos-arm64",   "tar.gz", "08a58829a3f14bb08f2e39f4cad48dcb14b7000384268e7f06689836332d6e25" },
    { "windows-amd64", "zip",    "d55a96fbeb97fd4cb05bd93f201e5c8f6518b7bfcde2fc6966fd0190fd891629" },
};

// Espressif names the Linux arm64 build "aarch64" and the macOS one "arm64".
QString archFor(const QString &os, const QString &cpu)
{
    if (cpu == "x86_64")
        return "amd64";
    if (cpu == "arm64" || cpu == "aarch64")
        return os == "macos" ? "arm64" : (os == "windows" ? "amd64" : "aarch64");
    if (cpu == "arm" && os == "linux")
        return "armv7";
    return QString();
}

} // namespace

EsptoolAsset esptoolAssetFor(const QString &os, const QString &cpu)
{
    const QString arch = archFor(os, cpu);
    if (arch.isEmpty())
        return EsptoolAsset();

    const QString key = os + "-" + arch;
    for (const Entry &e : kEntries)
    {
        if (key != QLatin1String(e.key))
            continue;

        EsptoolAsset a;
        a.tag = kTag;
        a.name = QString("esptool-%1-%2.%3").arg(kTag, key, e.ext);
        a.url = QString("https://github.com/espressif/esptool/releases/download/%1/%2")
                    .arg(kTag, a.name);
        a.sha256 = e.sha256;
        return a;
    }
    return EsptoolAsset();
}

EsptoolAsset esptoolAssetForHost()
{
#if defined(Q_OS_WIN)
    const QString os = "windows";
#elif defined(Q_OS_MACOS)
    const QString os = "macos";
#else
    const QString os = "linux";
#endif
    return esptoolAssetFor(os, QSysInfo::currentCpuArchitecture());
}
