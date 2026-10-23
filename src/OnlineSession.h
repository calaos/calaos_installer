#ifndef ONLINESESSION_H
#define ONLINESESSION_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QUrl>

class QNetworkReply;

// "config get" / "config put" against calaos_server for one dialog: unlocks the admin session
// when the server asks for it, keeps it for the next requests, and retries over https once.
class OnlineSession: public QObject
{
    Q_OBJECT
public:
    explicit OnlineSession(QObject *parent = nullptr);

    // Another host drops the admin session and the https rewrite of the previous one.
    void setServer(const QString &host);
    void setCredentials(const QString &user, const QString &pass, const QString &adminPass);
    // The https retry goes to the default port; a test server cannot listen there.
    void setHttpsPort(int port) { m_httpsPort = port; }

    void getConfig();
    void putConfig(const QString &ioXml, const QString &rulesXml);

    QUrl url() const { return m_url; }

    static const int kUnlockTimeoutMs = 5000;

signals:
    // reached: body is the server's answer to config, or a refusal in its place (admin_unlock
    // refused, or refused by the installer itself). Otherwise networkError says why no answer came.
    void getFinished(bool reached, const QByteArray &body, const QString &networkError);
    void putFinished(bool reached, const QByteArray &body, const QString &networkError);

private:
    enum Step { STEP_CONFIG, STEP_UNLOCK };

    void start();
    void send(Step step);
    void replied(QNetworkReply *reply, Step step);
    bool moveToHttps();
    void finish(bool reached, const QByteArray &body, const QString &networkError = QString());

    QNetworkAccessManager m_network;
    QString m_host, m_user, m_pass, m_adminPass;
    QString m_session;
    QUrl m_url;
    int m_httpsPort = -1;

    bool m_put = false;
    QString m_ioXml, m_rulesXml;

    bool m_unlockTried = false;
    bool m_httpsRetried = false;
    // Shown instead of the transport error when the https retry cannot connect at all.
    QByteArray m_refusalBeforeRetry;
};

#endif // ONLINESESSION_H
