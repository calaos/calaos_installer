#ifndef FIRMWAREMANIFEST_H
#define FIRMWAREMANIFEST_H

#include <QString>
#include <QList>
#include <QJsonObject>

struct EsptoolConfig
{
    QString chip;
    QString flashMode;
    QString flashSize;
    QString flashFreq;
    int baudrate = 921600;
    QString before;
    QString after;
};

struct BinaryEntry
{
    QString filename;
    QString offset;
    QString checksumSha256;
    QString checksumMd5;
};

struct FirmwareManifest
{
    int schemaVersion = 0;
    QString board;
    QString version;
    QString releaseDate;
    EsptoolConfig esptool;
    QList<BinaryEntry> binaries;

    static FirmwareManifest fromJson(const QJsonObject &obj);

    // Returns the offset for the config partition, or empty string if not found
    QString configBinaryOffset(const QString &configPartitionName) const;
};

#endif // FIRMWAREMANIFEST_H
