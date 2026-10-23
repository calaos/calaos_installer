#include <QtTest>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFile>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslServer>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTimer>

#include "ConfigOptions.h"
#include "dialogopenonline.h"
#include "dialogsaveonline.h"
#include "FakeApi.h"

// The online dialogs driven offscreen against a fake /api.php that has an admin password.
class TstOnlineDialogs: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void openFirstConnectionToNewHost();
    void saveFirstConnectionToNewHost();
    void pickingAnotherHostClearsAdminPassword();

private:
    QSslServer *newHttpsServer();
    void fill(QDialog &d, const QString &host, const QString &adminPass);
    void clickOk(QDialog &d);

    QTemporaryDir m_settingsDir;
    QSslConfiguration m_tls;
    QTimer m_boxAnswerer;
};

void TstOnlineDialogs::initTestCase()
{
    QVERIFY(m_settingsDir.isValid());
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_onlinedialogs");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());

    QFile cert(":/test_cert.pem"), key(":/test_key.pem");
    QVERIFY(cert.open(QIODevice::ReadOnly));
    QVERIFY(key.open(QIODevice::ReadOnly));
    m_tls = QSslConfiguration::defaultConfiguration();
    m_tls.setLocalCertificate(QSslCertificate(cert.readAll()));
    m_tls.setPrivateKey(QSslKey(key.readAll(), QSsl::Ec));

    // "Upload to ...?" and "successfully uploaded" are modal: answer them as the user would.
    connect(&m_boxAnswerer, &QTimer::timeout, this, []()
    {
        auto *box = qobject_cast<QMessageBox *>(QApplication::activeModalWidget());
        if (!box)
            return;
        if (QAbstractButton *b = box->button(QMessageBox::Yes))
            b->click();
        else if (QAbstractButton *b = box->button(QMessageBox::Ok))
            b->click();
    });
    m_boxAnswerer.start(50);
}

void TstOnlineDialogs::init()
{
    for (const CalaosAuth &a : ConfigOptions::Instance().getAuthList())
        ConfigOptions::Instance().deleteAuth(a.host);
}

QSslServer *TstOnlineDialogs::newHttpsServer()
{
    auto *s = new QSslServer(this);
    s->setSslConfiguration(m_tls);
    return s;
}

void TstOnlineDialogs::fill(QDialog &d, const QString &host, const QString &adminPass)
{
    d.findChild<QComboBox *>("comboIP")->setEditText(host);
    d.findChild<QLineEdit *>("editUsername")->setText("user");
    d.findChild<QLineEdit *>("editPass")->setText("pass");
    d.findChild<QLineEdit *>("editAdminPass")->setText(adminPass);
}

void TstOnlineDialogs::clickOk(QDialog &d)
{
    d.findChild<QDialogButtonBox *>("buttonBox")->button(QDialogButtonBox::Ok)->click();
}

void TstOnlineDialogs::openFirstConnectionToNewHost()
{
    FakeApi api(newHttpsServer(), adminServer);
    QVERIFY(api.listen());
    QTemporaryDir dir;

    DialogOpenOnline d(dir.path());
    fill(d, QString("https://127.0.0.1:%1/api.php").arg(api.port()), "correct horse battery");
    clickOk(d);

    QTRY_COMPARE_WITH_TIMEOUT(d.result(), int(QDialog::Accepted), 10000);
    QCOMPARE(api.count("admin_unlock"), 1);
    QCOMPARE(api.requests[1].body.value("admin_pass").toString(), QString("correct horse battery"));
    QVERIFY(QFile::exists(dir.filePath("io.xml")));
    // Kept for another attempt from the same window.
    QCOMPARE(d.findChild<QLineEdit *>("editAdminPass")->text(), QString("correct horse battery"));
}

void TstOnlineDialogs::saveFirstConnectionToNewHost()
{
    FakeApi api(newHttpsServer(), adminServer);
    QVERIFY(api.listen());
    QTemporaryDir dir;
    for (const char *name : { "io.xml", "rules.xml" })
    {
        QFile f(dir.filePath(name));
        QVERIFY(f.open(QIODevice::WriteOnly));
        f.write("<io/>");
    }

    DialogSaveOnline d(dir.path());
    fill(d, QString("https://127.0.0.1:%1/api.php").arg(api.port()), "correct horse battery");
    clickOk(d);

    QTRY_COMPARE_WITH_TIMEOUT(d.result(), int(QDialog::Accepted), 10000);
    QCOMPARE(api.count("admin_unlock"), 1);
    QCOMPARE(api.count("config", "put"), 1);
}

void TstOnlineDialogs::pickingAnotherHostClearsAdminPassword()
{
    ConfigOptions::Instance().addAuth({ "user", "pass", "10.0.0.1" });
    ConfigOptions::Instance().addAuth({ "user", "pass", "10.0.0.2" });
    QTemporaryDir dir;

    DialogOpenOnline d(dir.path());
    auto *combo = d.findChild<QComboBox *>("comboIP");
    auto *admin = d.findChild<QLineEdit *>("editAdminPass");
    admin->setText("secret of box A");
    combo->setCurrentIndex(combo->currentIndex() == 0 ? 1 : 0);
    QVERIFY(admin->text().isEmpty());
}

QTEST_MAIN(TstOnlineDialogs)
#include "tst_onlinedialogs.moc"
