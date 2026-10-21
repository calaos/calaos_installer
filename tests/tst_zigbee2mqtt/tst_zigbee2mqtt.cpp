#include <QtTest>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QTemporaryDir>
#include <QTreeView>

#include "ConfigOptions.h"
#include "DialogZigbee2mqtt.h"
#include "../tst_esphomediscovery/FakeMqttBroker.h"

namespace
{

QByteArray devices(const QStringList &names)
{
    QJsonArray a;
    a.append(QJsonObject{ { "type", "Coordinator" }, { "friendly_name", "Coordinator" } });
    for (const QString &n: names)
    {
        const QJsonObject expose{ { "type", "binary" }, { "name", "occupancy" }, { "label", "Occupancy" } };
        const QJsonObject def{ { "vendor", "Aqara" }, { "description", "Motion sensor" },
                               { "exposes", QJsonArray{ expose } } };
        a.append(QJsonObject{ { "type", "EndDevice" }, { "friendly_name", n }, { "definition", def } });
    }
    return QJsonDocument(a).toJson(QJsonDocument::Compact);
}

}

class tst_Zigbee2mqtt: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void theDevicesOfTheBridgeAreListed();
    void aBrokerErrorIsShown();
    void theBrokerIsRememberedOnceItTakesTheUser();

private:
    QTemporaryDir m_settingsDir;

    void fill(DialogZigbee2mqtt &d, quint16 port, const QString &user, const QString &password);
};

void tst_Zigbee2mqtt::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName("calaos-installer-tests");
    QCoreApplication::setApplicationName("tst_zigbee2mqtt");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_settingsDir.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_settingsDir.path());
}

void tst_Zigbee2mqtt::fill(DialogZigbee2mqtt &d, quint16 port, const QString &user, const QString &password)
{
    d.findChild<QLineEdit *>("lineEditHost")->setText("127.0.0.1");
    d.findChild<QSpinBox *>("spinBoxPort")->setValue(port);
    d.findChild<QLineEdit *>("lineEditUsername")->setText(user);
    d.findChild<QLineEdit *>("lineEditPassword")->setText(password);
}

void tst_Zigbee2mqtt::theDevicesOfTheBridgeAreListed()
{
    FakeMqttBroker broker;
    broker.requiredUser = "calaos";
    broker.requiredPassword = "s3cret";
    broker.retained << qMakePair(QString("zigbee2mqtt/bridge/devices"), devices({ "salon_motion" }));
    const quint16 port = broker.start();

    DialogZigbee2mqtt d;
    d.show();
    fill(d, port, "calaos", "s3cret");
    QPushButton *button = d.findChild<QPushButton *>("pushButtonConnect");
    auto *model = qobject_cast<QStandardItemModel *>(d.findChild<QTreeView *>("treeDevices")->model());
    QVERIFY(model);

    QTest::mouseClick(button, Qt::LeftButton);
    QVERIFY(!button->isEnabled());
    QVERIFY(!d.findChild<QLineEdit *>("lineEditHost")->isEnabled());
    QTRY_COMPARE(model->rowCount(), 1);
    QCOMPARE(model->item(0)->text(), QString("salon_motion [Aqara - Motion sensor]"));
    QCOMPARE(model->item(0)->child(0, 1)->text(), QString("Switch"));
    QCOMPARE(button->text(), QString("Disconnect"));
    QVERIFY(button->isEnabled());
    QCOMPARE(broker.filters, QStringList({ "zigbee2mqtt/bridge/devices" }));
    QCOMPARE(ConfigOptions::Instance().getMqttBrokerHost(), QString("127.0.0.1"));
    QCOMPARE(ConfigOptions::Instance().getMqttBrokerPort(), int(port));

    //The dialog stays connected: a device paired later shows up.
    QTest::qWait(300);
    broker.publish("zigbee2mqtt/bridge/devices", devices({ "salon_motion", "cuisine_motion" }));
    QTRY_COMPARE(model->rowCount(), 2);

    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(button->text(), QString("Connect"));
    QVERIFY(d.findChild<QLineEdit *>("lineEditHost")->isEnabled());
    QTRY_COMPARE(broker.disconnects, 1);
    QCOMPARE(model->rowCount(), 2);
}

void tst_Zigbee2mqtt::aBrokerErrorIsShown()
{
    FakeMqttBroker broker;
    broker.requiredUser = "calaos";
    broker.requiredPassword = "s3cret";
    const quint16 port = broker.start();

    DialogZigbee2mqtt d;
    d.show();
    fill(d, port, "calaos", "wrong");
    QString shown;
    QTimer closer;
    connect(&closer, &QTimer::timeout, this, [&]()
    {
        if (auto *box = qobject_cast<QMessageBox *>(QApplication::activeModalWidget()))
        {
            shown = box->text();
            box->close();
        }
    });
    closer.start(20);
    QPushButton *button = d.findChild<QPushButton *>("pushButtonConnect");
    QTest::mouseClick(button, Qt::LeftButton);
    QTRY_COMPARE(shown, QString("The broker at 127.0.0.1:%1 refused the user name or the password.").arg(port));
    QCOMPARE(button->text(), QString("Connect"));
    QVERIFY(button->isEnabled());
    QVERIFY(d.findChild<QLineEdit *>("lineEditPassword")->isEnabled());
}

void tst_Zigbee2mqtt::theBrokerIsRememberedOnceItTakesTheUser()
{
    //Refused topics: the broker and the user are right all the same.
    FakeMqttBroker broker;
    broker.refuseSubscriptions = true;
    const quint16 port = broker.start();
    ConfigOptions::Instance().setMqttBrokerHost("elsewhere");

    DialogZigbee2mqtt d;
    d.show();
    fill(d, port, "zb", "pw");
    QString shown;
    QTimer closer;
    connect(&closer, &QTimer::timeout, this, [&]()
    {
        if (auto *box = qobject_cast<QMessageBox *>(QApplication::activeModalWidget()))
        {
            shown = box->text();
            box->close();
        }
    });
    closer.start(20);
    QTest::mouseClick(d.findChild<QPushButton *>("pushButtonConnect"), Qt::LeftButton);
    QTRY_COMPARE(shown, QString("The broker at 127.0.0.1:%1 does not let this user read those topics.").arg(port));
    QCOMPARE(ConfigOptions::Instance().getMqttBrokerHost(), QString("127.0.0.1"));
    QCOMPARE(ConfigOptions::Instance().getMqttBrokerUser(), QString("zb"));
}

QTEST_MAIN(tst_Zigbee2mqtt)

#include "tst_zigbee2mqtt.moc"
