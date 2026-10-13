#ifndef ONLINECONFIG_H
#define ONLINECONFIG_H

#include <QByteArray>
#include <QString>

// JSON side of "config get" / "config put" against calaos_server, kept free of widgets and network.
namespace OnlineConfig
{

// Format the installer declares in every put (see ListeRoom::IO_FORMAT_SUPPORTED).
const int kPutIoFormat = 2;
// First format a server must announce to receive devices.
const int kDevicesMinFormat = 2;
// Marks a server whose format was never read in this session.
const int kFormatUnknown = 0;

QByteArray buildGetRequest(const QString &user, const QString &pass);
QByteArray buildPutRequest(const QString &user, const QString &pass,
                           const QString &ioXml, const QString &rulesXml);

struct GetResult
{
    bool ok = false;
    QString ioXml, rulesXml;
    // Servers older than CORE-3 omit it; that is format 1, not "unknown".
    int ioFormat = 1;
    bool hasFiles = false;
};
GetResult parseGetResponse(const QByteArray &body);

struct PutResult
{
    bool ok = false;
    QString error; // the server's own text on a refusal, empty when it gave none
};
PutResult parsePutResponse(const QByteArray &body);

// Formats are remembered per server address for the session, so a project opened from
// server A is never judged by what server B announced.
void rememberServerFormat(const QString &host, int format);
int serverFormat(const QString &host);

bool serverAcceptsDevices(int format);

// True when a put must wait for a fresh config get: devices are about to be sent and the
// last announced format would not take them (never read, or since upgraded).
bool mustProbeBeforePut(bool projectHasDevices, const QString &host);

}

#endif
