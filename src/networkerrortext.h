#ifndef NETWORKERRORTEXT_H
#define NETWORKERRORTEXT_H

#include <QString>

class QNetworkReply;

// Strips the URL from a failed HTTP(S) reply: Qt's errorString() embeds it,
// query included, and some camera and NAS URLs carry a password there.
// Other schemes (file://, "Cannot open %1") fall back to errorString() and may leak it.
QString networkErrorText(QNetworkReply *reply);

#endif // NETWORKERRORTEXT_H
