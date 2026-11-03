#include "networkerrortext.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

QString networkErrorText(QNetworkReply *reply)
{
    const int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (code >= 400)
        return QString("HTTP %1 %2").arg(code).arg(
            reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()).trimmed();

    // file:// and "Cannot open %1" errors embed the URL or the local path: cut them out.
    QString text = reply->errorString();
    const QUrl url = reply->url();
    const QStringList leaks = { url.toString(), url.toString(QUrl::PreferLocalFile),
                                url.toDisplayString(), url.toLocalFile(),
                                QString::fromUtf8(url.toEncoded()) };
    for (const QString &leak : leaks)
    {
        if (!leak.isEmpty())
            text.replace(leak, QStringLiteral("<url>"));
    }
    return text;
}
