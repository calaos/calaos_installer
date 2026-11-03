#include <QtTest>
#include <QMessageBox>
#include <memory>

#include "ToolUpdatePrompt.h"

class TstToolUpdatePrompt : public QObject
{
    Q_OBJECT

private slots:
    void questionShowsNetworkTagAsPlainText();
    void failureShowsNetworkErrorAsPlainText();
};

void TstToolUpdatePrompt::questionShowsNetworkTagAsPlainText()
{
    const QString tag = "<img src='http://evil.example/x.png'>v9";
    std::unique_ptr<QMessageBox> box(makeToolUpdateQuestion("esptool", tag, "v5.0.0", nullptr));

    QCOMPARE(box->textFormat(), Qt::PlainText);
    QVERIFY(box->text().contains(tag));
    QVERIFY(box->text().contains("v5.0.0"));
    QVERIFY(box->standardButtons().testFlag(QMessageBox::Yes));
    QVERIFY(box->standardButtons().testFlag(QMessageBox::No));
}

void TstToolUpdatePrompt::failureShowsNetworkErrorAsPlainText()
{
    const QString error = "<a href='http://evil.example'>click</a>";
    std::unique_ptr<QMessageBox> box(makeToolUpdateFailedBox(error, nullptr));

    QCOMPARE(box->textFormat(), Qt::PlainText);
    QVERIFY(box->text().contains(error));
}

QTEST_MAIN(TstToolUpdatePrompt)
#include "tst_toolupdateprompt.moc"
