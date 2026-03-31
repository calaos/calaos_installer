#include "FirmwareManifest.h"
#include <QJsonArray>

FirmwareManifest FirmwareManifest::fromJson(const QJsonObject &obj)
{
    FirmwareManifest m;
    m.schemaVersion = obj.value("schema_version").toInt(0);
    m.board = obj.value("board").toString();
    m.version = obj.value("version").toString();
    m.releaseDate = obj.value("release_date").toString();

    QJsonObject et = obj.value("esptool").toObject();
    m.esptool.chip = et.value("chip").toString();
    m.esptool.flashMode = et.value("flash_mode").toString();
    m.esptool.flashSize = et.value("flash_size").toString();
    m.esptool.flashFreq = et.value("flash_freq").toString();
    m.esptool.baudrate = et.value("baudrate").toInt(921600);
    m.esptool.before = et.value("before").toString();
    m.esptool.after = et.value("after").toString();

    QJsonArray bins = obj.value("binaries").toArray();
    for (const QJsonValue &val : bins)
    {
        QJsonObject bo = val.toObject();
        BinaryEntry be;
        be.filename = bo.value("filename").toString();
        be.offset = bo.value("offset").toString();
        be.checksumSha256 = bo.value("checksum_sha256").toString();
        be.checksumMd5 = bo.value("checksum_md5").toString();
        m.binaries.append(be);
    }

    return m;
}

QString FirmwareManifest::configBinaryOffset(const QString &configPartitionName) const
{
    for (const BinaryEntry &be : binaries)
    {
        if (be.filename == configPartitionName)
            return be.offset;
    }
    return QString();
}
