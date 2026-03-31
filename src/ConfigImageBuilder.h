#ifndef CONFIGIMAGEBUILDER_H
#define CONFIGIMAGEBUILDER_H

#include <QByteArray>
#include <QString>
#include <cstdint>

struct ProvisioningConfig
{
    QString networkInterface; // "ethernet" or "wifi"
    QString ipMode;           // "dhcp" or "static"
    QString staticIp;
    QString staticMask;
    QString staticGateway;
    QString staticDns;
    QString wifiSsid;
    QString wifiPassword;
    QString serverHost;
    int serverPort = 5454;
    bool serverSsl = false;
    bool autoDiscovery = true;
};

class ConfigImageBuilder
{
public:
    // Build minified JSON payload from config, omitting default values
    static QByteArray buildJson(const ProvisioningConfig &cfg);

    // Build full binary image: header + JSON + 0xFF padding
    static QByteArray buildImage(const ProvisioningConfig &cfg, int paddingSize = 65536);

    static constexpr char MAGIC[4] = { 'C', 'A', 'L', 'O' };
    static constexpr uint8_t FORMAT_VERSION = 0x01;
};

#endif // CONFIGIMAGEBUILDER_H
