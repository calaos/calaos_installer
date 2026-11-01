#ifndef NETWORKERRORTEXT_H
#define NETWORKERRORTEXT_H

#include <QString>

class QNetworkReply;

// Text safe to show or log for a failed reply. Qt's own errorString() embeds
// the request URL for HTTP errors, query included, and some camera and NAS
// URLs carry a password there.
QString networkErrorText(QNetworkReply *reply);

#endif // NETWORKERRORTEXT_H
