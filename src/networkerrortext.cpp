#include "networkerrortext.h"

#include <QNetworkReply>
#include <QNetworkRequest>

QString networkErrorText(QNetworkReply *reply)
{
    const int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (code >= 400)
        return QString("HTTP %1 %2").arg(code).arg(
            reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()).trimmed();
    // Errors without an HTTP status (TLS, connection, redirect policy) do not carry the URL.
    return reply->errorString();
}
