#include "EsptoolRelease.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QJsonObject>
#include <QRegularExpression>
#include <QStringList>
#include <QSysInfo>

#include <algorithm>
#include <vector>

namespace {

enum class Arch { Unknown, X64, Arm64, Armv7 };

QString osOfToken(const QString &t)
{
    if (t == "win" || t == "windows" || t == "win64")
        return "windows";
    if (t == "linux")
        return "linux";
    if (t == "macos" || t == "darwin" || t == "osx")
        return "macos";
    return QString();
}

Arch archOfToken(const QString &t)
{
    // "win64" names the x64 build on its own.
    if (t == "amd64" || t == "x64" || t == "win64")
        return Arch::X64;
    if (t == "arm64" || t == "aarch64")
        return Arch::Arm64;
    if (t == "armv7" || t == "armv7l" || t == "armhf" || t == "arm32")
        return Arch::Armv7;
    return Arch::Unknown;
}

// Architectures usable on the host, best first.
QList<Arch> wantedArchs(const QString &os, const QString &cpu)
{
    if (cpu == "x86_64" || cpu == "amd64")
        return { Arch::X64 };
    if (cpu == "arm64" || cpu == "aarch64")
    {
        // Windows on ARM runs the x64 build under emulation.
        if (os == "windows")
            return { Arch::Arm64, Arch::X64 };
        return { Arch::Arm64 };
    }
    if (cpu == "arm" || cpu == "armv7")
        return os == "linux" ? QList<Arch>{ Arch::Armv7 } : QList<Arch>{};
    return {};
}

bool isArchive(const QString &lowerName)
{
    return lowerName.endsWith(".tar.gz") || lowerName.endsWith(".tgz") || lowerName.endsWith(".zip");
}

// Returns the rank of the asset for the platform (0 is best), or -1 when it does not fit.
int rankAsset(const QString &name, const QString &os, const QList<Arch> &archs)
{
    const QString lower = name.toLower();
    if (!isArchive(lower) || !isSafeAssetName(name))
        return -1;

    QString normalized = lower;
    normalized.replace("x86-64", "x64");
    normalized.replace("x86_64", "x64");
    const QStringList tokens = normalized.split(QRegularExpression("[-._]"), Qt::SkipEmptyParts);

    QString assetOs;
    Arch assetArch = Arch::Unknown;
    for (const QString &t : tokens)
    {
        if (assetOs.isEmpty())
            assetOs = osOfToken(t);
        const Arch a = archOfToken(t);
        if (assetArch == Arch::Unknown && a != Arch::Unknown)
            assetArch = a;
    }
    if (assetOs != os)
        return -1;

    // Old releases ship a single universal "macos" zip with no architecture in the name.
    if (assetArch == Arch::Unknown)
        return os == "macos" ? archs.size() : -1;
    return archs.contains(assetArch) ? archs.indexOf(assetArch) : -1;
}

// A named context lets lupdate extract the messages below.
struct Tr
{
    Q_DECLARE_TR_FUNCTIONS(EsptoolRelease)
};

// "sha256:<64 hex>" -> hex, else empty.
QString parseDigest(const QString &digest)
{
    static const QRegularExpression re("^sha256:([0-9a-fA-F]{64})$");
    const QRegularExpressionMatch m = re.match(digest);
    return m.hasMatch() ? m.captured(1).toLower() : QString();
}

QList<int> versionParts(const QString &v)
{
    static const QRegularExpression re("^v?(\\d+)\\.(\\d+)(?:\\.(\\d+))?");
    const QRegularExpressionMatch m = re.match(v.trimmed());
    if (!m.hasMatch())
        return {};
    return { m.captured(1).toInt(), m.captured(2).toInt(), m.captured(3).toInt() };
}

} // namespace

bool isSafeAssetName(const QString &name)
{
    if (name.isEmpty() || name == "." || name.contains("..") || name != QFileInfo(name).fileName())
        return false;
    for (const QChar c : name)
    {
        if (c == '/' || c == '\\' || c == ':' || c.unicode() < 0x20)
            return false;
    }
    return true;
}

EsptoolAsset selectEsptoolAsset(const QJsonArray &releases, const QString &os, const QString &cpu)
{
    EsptoolAsset result;
    const QList<Arch> archs = wantedArchs(os, cpu);
    if (archs.isEmpty())
    {
        result.error = Tr::tr("No esptool build exists for this platform (%1).").arg(cpu);
        return result;
    }

    // The API lists by creation date: a 4.x maintenance release can follow a 5.x one.
    std::vector<QJsonObject> eligible;
    for (const QJsonValue &rv : releases)
    {
        const QJsonObject rel = rv.toObject();
        if (!rel.value("draft").toBool() && !rel.value("prerelease").toBool())
            eligible.push_back(rel);
    }
    std::stable_sort(eligible.begin(), eligible.end(), [](const QJsonObject &a, const QJsonObject &b)
    {
        return versionParts(a.value("tag_name").toString()) > versionParts(b.value("tag_name").toString());
    });

    for (const QJsonObject &rel : eligible)
    {
        int bestRank = -1;
        QJsonObject best;
        for (const QJsonValue &av : rel.value("assets").toArray())
        {
            const QJsonObject asset = av.toObject();
            const int rank = rankAsset(asset.value("name").toString(), os, archs);
            if (rank >= 0 && (bestRank < 0 || rank < bestRank))
            {
                bestRank = rank;
                best = asset;
            }
        }
        if (bestRank < 0)
            continue;

        result.tag = rel.value("tag_name").toString();
        result.name = best.value("name").toString();
        const QString url = best.value("browser_download_url").toString();
        if (!url.startsWith("https://"))
        {
            result.error = Tr::tr("Release %1 gives no HTTPS download URL for %2.").arg(result.tag, result.name);
            return result;
        }

        result.sha256 = parseDigest(best.value("digest").toString());
        if (result.sha256.isEmpty())
        {
            result.error = Tr::tr("GitHub gives no SHA-256 digest for %1 (release %2): "
                              "refusing to download an unverifiable file.").arg(result.name, result.tag);
            return result;
        }
        const qint64 size = static_cast<qint64>(best.value("size").toDouble());
        if (size <= 0 || size > kMaxToolArchiveBytes)
        {
            result.error = Tr::tr("GitHub gives an unexpected size for %1 (release %2): "
                                  "refusing to download it.").arg(result.name, result.tag);
            return result;
        }
        result.size = size;

        result.url = url;
        return result;
    }

    result.error = Tr::tr("No esptool build for this platform (%1) in the latest releases.").arg(cpu);
    return result;
}

EsptoolAsset selectEsptoolAssetForHost(const QJsonArray &releases)
{
#if defined(Q_OS_WIN)
    const QString os = "windows";
#elif defined(Q_OS_MACOS)
    const QString os = "macos";
#else
    const QString os = "linux";
#endif
    return selectEsptoolAsset(releases, os, QSysInfo::currentCpuArchitecture());
}

bool isNewerEsptoolVersion(const QString &candidate, const QString &cached)
{
    const QList<int> c = versionParts(candidate);
    if (c.isEmpty())
        return false;
    const QList<int> k = versionParts(cached);
    return k.isEmpty() || c > k;
}
