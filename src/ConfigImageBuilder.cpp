#include "ConfigImageBuilder.h"
#include "Crc32.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <cstring>

QByteArray ConfigImageBuilder::buildJson(const ProvisioningConfig &cfg)
{
    QJsonObject obj;

    // Only include fields that differ from defaults
    if (!cfg.networkInterface.isEmpty() && cfg.networkInterface != "ethernet")
        obj["network_interface"] = cfg.networkInterface;

    if (!cfg.ipMode.isEmpty() && cfg.ipMode != "dhcp")
        obj["ip_mode"] = cfg.ipMode;

    if (!cfg.staticIp.isEmpty())
        obj["static_ip"] = cfg.staticIp;
    if (!cfg.staticMask.isEmpty())
        obj["static_mask"] = cfg.staticMask;
    if (!cfg.staticGateway.isEmpty())
        obj["static_gateway"] = cfg.staticGateway;
    if (!cfg.staticDns.isEmpty())
        obj["static_dns"] = cfg.staticDns;

    if (!cfg.wifiSsid.isEmpty())
        obj["wifi_ssid"] = cfg.wifiSsid;
    if (!cfg.wifiPassword.isEmpty())
        obj["wifi_password"] = cfg.wifiPassword;

    if (!cfg.autoDiscovery && !cfg.serverHost.isEmpty())
        obj["server_host"] = cfg.serverHost;
    if (!cfg.autoDiscovery && cfg.serverPort != 5454)
        obj["server_port"] = cfg.serverPort;
    if (!cfg.autoDiscovery && cfg.serverSsl)
        obj["server_ssl"] = true;

    QJsonDocument doc(obj);
    return doc.toJson(QJsonDocument::Compact);
}

QByteArray ConfigImageBuilder::buildImage(const ProvisioningConfig &cfg, int paddingSize)
{
    QByteArray json = buildJson(cfg);
    uint32_t crc = Crc32::compute(json);
    uint16_t jsonLen = static_cast<uint16_t>(json.size());

    // Header: 12 bytes
    // [0..3]  Magic "CALO"
    // [4]     Format version
    // [5]     Flags (reserved)
    // [6..7]  JSON payload length (uint16 LE)
    // [8..11] CRC-32 of JSON payload (uint32 LE)
    QByteArray image;
    image.reserve(paddingSize);

    // Magic
    image.append(MAGIC, 4);
    // Format version
    image.append(static_cast<char>(FORMAT_VERSION));
    // Flags (reserved)
    image.append(static_cast<char>(0x00));
    // JSON length (little-endian)
    image.append(static_cast<char>(jsonLen & 0xFF));
    image.append(static_cast<char>((jsonLen >> 8) & 0xFF));
    // CRC-32 (little-endian)
    image.append(static_cast<char>(crc & 0xFF));
    image.append(static_cast<char>((crc >> 8) & 0xFF));
    image.append(static_cast<char>((crc >> 16) & 0xFF));
    image.append(static_cast<char>((crc >> 24) & 0xFF));
    // JSON payload
    image.append(json);

    // Pad with 0xFF to target size
    int remaining = paddingSize - image.size();
    if (remaining > 0)
        image.append(QByteArray(remaining, static_cast<char>(0xFF)));

    return image;
}
