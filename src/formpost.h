#ifndef __FORMPOST_H__
#define __FORMPOST_H__

#include <QtPlugin>
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QProgressBar>
#include <QDateTime>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FormPost: public QWidget
{
    Q_OBJECT

private:
    QByteArray data;
    QByteArray strToEnc(QString s);

    QString encodingS;
    QString userAgentS;
    QString refererS;

    QStringList fieldNames;
    QStringList fieldValues;
    QStringList fileFieldNames;
    QStringList fileNames;
    QStringList fileMimes;

    QList<QByteArray> files;

public:
    FormPost();

    QString userAgent();
    void setUserAgent(QString agent);

    QString referer();
    void setReferer(QString ref);

    QString encoding();
    void setEncoding(QString enc);

    void addField(QString name, QString value);

    void addFile(QString fieldName, QByteArray file, QString name, QString mime);
    void addFile(QString fieldName, QString fileName, QString mime);

    QNetworkReply *postData(QString url);
    QByteArray &response();

signals:
    void uploadDone(QByteArray &reply);


private slots:
    void readData(QNetworkReply * r);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
};

#endif
