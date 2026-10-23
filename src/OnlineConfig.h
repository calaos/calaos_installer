#ifndef ONLINECONFIG_H
#define ONLINECONFIG_H

#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QUrl>

// JSON side of "config get" / "config put" against calaos_server, kept free of widgets and network.
namespace OnlineConfig
{

// Newest io.xml format the installer reads and writes; declared in every put.
const int kPutIoFormat = 2;
// First format a server must announce to receive devices.
const int kDevicesMinFormat = 2;
// Marks a server whose format was never read in this session.
const int kFormatUnknown = 0;

// An empty adminSession leaves the key out, so a server without an admin sees today's request.
QByteArray buildGetRequest(const QString &user, const QString &pass, const QString &adminSession = QString());
QByteArray buildPutRequest(const QString &user, const QString &pass,
                           const QString &ioXml, const QString &rulesXml,
                           const QString &adminSession = QString());
QByteArray buildUnlockRequest(const QString &user, const QString &pass, const QString &adminPass);

// Error code of a refusal ({"success":"false","error":<code>}), empty for a success or a non-JSON body.
QString refusalCode(const QByteArray &body);
// Session of a successful admin_unlock answer, empty otherwise.
QString parseUnlockResponse(const QByteArray &body);

// Readable text of a refusal: the admin codes of calaos_server are translated, any other
// error is the server's own text, empty when it gave none.
QString refusalText(const QByteArray &body);

// Server address as typed in the dialogs: a bare host means https://host/api.php.
QUrl serverUrl(const QString &host);
// https://host/api.php (httpsPort < 0: the default port), or an empty URL when that is
// already the URL tried: a refusal there would only come back.
QUrl httpsRetryUrl(const QUrl &url, int httpsPort = -1);
// https_required, or an admin session refused on the clear port 5454, which can never
// carry one. Other refusals are not retried: an old server has no https to retry on.
bool shouldRetryOverHttps(const QUrl &url, const QString &code);

struct GetResult
{
    bool ok = false;
    QString ioXml, rulesXml;
    // Older servers omit it; that is format 1, not "unknown".
    int ioFormat = 1;
    bool hasFiles = false;
    QString error;
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

// Ids of the <calaos:device> elements of an io.xml text.
QStringList deviceIds(const QString &ioXml);

// Ids the server holds that the io.xml about to be sent does not: a put declaring format 2
// is trusted by the server, so these would be deleted without any refusal.
QStringList devicesLostByPut(const QString &serverIoXml, const QString &sentIoXml);

}

#endif
