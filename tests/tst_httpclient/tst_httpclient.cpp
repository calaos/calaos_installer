#include <QtTest>
#include <QFile>
#include <QTemporaryDir>
#include <QUrl>

#include "HttpClient.h"

// file:// URLs keep these tests off the network.
class TstHttpClient : public QObject
{
    Q_OBJECT

private slots:
    void unwritableDestinationSetsError();
    void successClearsPreviousError();
};

void TstHttpClient::unwritableDestinationSetsError()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QFile src(dir.filePath("src.bin"));
    QVERIFY(src.open(QIODevice::WriteOnly));
    src.write("data");
    src.close();

    HttpClient http;
    int calls = 0;
    bool ok = true;
    http.downloadFile(QUrl::fromLocalFile(src.fileName()),
                      dir.filePath("missing_dir/dest.bin"),
                      [&](bool success) { ++calls; ok = success; });

    QCOMPARE(calls, 1);
    QVERIFY(!ok);
    QVERIFY(!http.lastError().isEmpty());
}

void TstHttpClient::successClearsPreviousError()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QFile src(dir.filePath("src.bin"));
    QVERIFY(src.open(QIODevice::WriteOnly));
    src.write("data");
    src.close();

    HttpClient http;
    http.downloadFile(QUrl::fromLocalFile(src.fileName()),
                      dir.filePath("missing_dir/dest.bin"), [](bool) {});
    QVERIFY(!http.lastError().isEmpty());

    bool done = false;
    bool ok = false;
    http.downloadFile(QUrl::fromLocalFile(src.fileName()), dir.filePath("dest.bin"),
                      [&](bool success) { done = true; ok = success; });
    QTRY_VERIFY_WITH_TIMEOUT(done, 5000);
    QVERIFY(ok);
    QVERIFY(http.lastError().isEmpty());
}

QTEST_MAIN(TstHttpClient)
#include "tst_httpclient.moc"
