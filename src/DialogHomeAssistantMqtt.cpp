#include "DialogHomeAssistantMqtt.h"
#include "ConfigOptions.h"
#include "MqttPacket.h"

#include <ListeRoom.h>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTreeWidget>
#include <QVBoxLayout>

using namespace Calaos;

namespace
{

const int ENTITY_ROLE = Qt::UserRole + 1;

}

DialogHomeAssistantMqtt::DialogHomeAssistantMqtt(const QJsonArray &templates, Room *room, QWidget *parent):
    QDialog(parent),
    m_templates(templates),
    m_listener(new MqttListener(this))
{
    setWindowTitle(tr("Home Assistant MQTT discovery"));
    resize(820, 620);
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *intro = new QLabel(tr("Lists the entities that Home Assistant MQTT discovery announces on the broker "
                                  "(ESPHome, Zigbee2MQTT, Tasmota...). The checked ones are added to the project: "
                                  "an MQTT IO each, a climate device for a climate."), this);
    intro->setWordWrap(true);
    layout->addWidget(intro);

    //Written in the IOs: the broker as the Calaos server reaches it.
    QGroupBox *server = new QGroupBox(tr("Broker used by the Calaos server"), this);
    QFormLayout *serverForm = new QFormLayout(server);
    const MqttBroker project = MqttBroker::ofProject();
    m_serverHost = new QLineEdit(project.found ? project.host : QStringLiteral("127.0.0.1"), server);
    m_serverHost->setObjectName("serverHost");
    m_serverHost->setToolTip(tr("Written in the imported IOs. 127.0.0.1 is the Calaos box itself."));
    m_serverPort = new QSpinBox(server);
    m_serverPort->setObjectName("serverPort");
    m_serverPort->setRange(1, 65535);
    m_serverPort->setValue(project.port);
    m_serverUser = new QLineEdit(project.user, server);
    m_serverUser->setObjectName("serverUser");
    m_serverPassword = new QLineEdit(project.password, server);
    m_serverPassword->setObjectName("serverPassword");
    m_serverPassword->setEchoMode(QLineEdit::Password);
    serverForm->addRow(tr("Address:"), m_serverHost);
    serverForm->addRow(tr("Port:"), m_serverPort);
    serverForm->addRow(tr("User:"), m_serverUser);
    serverForm->addRow(tr("Password:"), m_serverPassword);
    layout->addWidget(server);

    QHBoxLayout *row = new QHBoxLayout();
    QLabel *probeLabel = new QLabel(tr("Broker address from this computer:"), this);
    m_probeHost = new QLineEdit(this);
    m_probeHost->setObjectName("probeHost");
    m_probeHost->setPlaceholderText(tr("Address of the Calaos box"));
    m_probeHost->setToolTip(tr("Port, user and password are the ones above; the user and the password of the "
                               "project's broker are only sent to that broker."));
    probeLabel->setBuddy(m_probeHost);
    QLabel *prefixLabel = new QLabel(tr("Discovery prefix:"), this);
    m_prefix = new QLineEdit(ConfigOptions::Instance().getHADiscoveryPrefix(), this);
    m_prefix->setObjectName("prefix");
    m_prefix->setPlaceholderText("homeassistant");
    prefixLabel->setBuddy(m_prefix);
    m_searchButton = new QPushButton(tr("Search"), this);
    m_searchButton->setObjectName("searchButton");
    m_searchButton->setAutoDefault(false);
    row->addWidget(probeLabel);
    row->addWidget(m_probeHost, 2);
    row->addWidget(prefixLabel);
    row->addWidget(m_prefix, 1);
    row->addWidget(m_searchButton);
    layout->addLayout(row);
    //Enter there searches; the dialog would take it for Import.
    m_probeHost->installEventFilter(this);
    m_prefix->installEventFilter(this);

    m_status = new QLabel(this);
    m_status->setObjectName("status");
    m_status->setWordWrap(true);
    layout->addWidget(m_status);
    setStatus(tr("Search the broker to list its entities."), false);

    m_list = new QTreeWidget(this);
    m_list->setObjectName("entityList");
    m_list->setHeaderLabels({ tr("Name"), tr("Home Assistant"), tr("Calaos"), tr("Status") });
    m_list->setAlternatingRowColors(true);
    m_list->header()->setStretchLastSection(true);
    m_list->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_list->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_list->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_list->hide();
    layout->addWidget(m_list, 1);

    QFormLayout *roomForm = new QFormLayout();
    m_room = new QComboBox(this);
    m_room->setObjectName("room");
    ListeRoom &lr = ListeRoom::Instance();
    for (int i = 0;i < lr.size();i++)
        m_room->addItem(QString::fromUtf8(lr.get_room(i)->get_name().c_str()), QVariant::fromValue(lr.get_room(i)));
    if (room && m_room->findData(QVariant::fromValue(room)) >= 0)
        m_room->setCurrentIndex(m_room->findData(QVariant::fromValue(room)));
    roomForm->addRow(tr("Import into the room:"), m_room);
    layout->addLayout(roomForm);

    m_error = new QLabel(this);
    m_error->setObjectName("labelError");
    m_error->setWordWrap(true);
    m_error->setStyleSheet("color: rgb(255, 85, 127);");
    m_error->hide();
    layout->addWidget(m_error);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_importButton = buttons->button(QDialogButtonBox::Ok);
    m_importButton->setText(tr("Import"));
    m_importButton->setEnabled(false);
    connect(buttons, &QDialogButtonBox::accepted, this, &DialogHomeAssistantMqtt::importChecked);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    connect(m_probeHost, &QLineEdit::textEdited, this, [this]() { m_probeTyped = true; });
    connect(m_serverHost, &QLineEdit::textChanged, this, &DialogHomeAssistantMqtt::updateProbeHost);
    connect(m_serverUser, &QLineEdit::textEdited, this, [this]() { m_credentialsTyped = true; });
    connect(m_serverPassword, &QLineEdit::textEdited, this, [this]() { m_credentialsTyped = true; });
    connect(m_searchButton, &QPushButton::clicked, this, &DialogHomeAssistantMqtt::search);
    connect(m_list, &QTreeWidget::itemChanged, this, &DialogHomeAssistantMqtt::updateImportButton);
    connect(m_listener, &MqttListener::message, this, [this](const QString &topic, const QByteArray &payload)
    {
        m_discovery.feed(topic, payload);
    });
    connect(m_listener, &MqttListener::listening, this, [this]()
    {
        setStatus(tr("Listening to the broker..."), false);
    });
    connect(m_listener, &MqttListener::finished, this, &DialogHomeAssistantMqtt::finished);

    updateProbeHost();
}

Room *DialogHomeAssistantMqtt::room() const
{
    return m_room->currentData().value<Room *>();
}

void DialogHomeAssistantMqtt::setTimings(int listenMs, int answerTimeoutMs)
{
    m_listenMs = listenMs;
    m_answerTimeoutMs = answerTimeoutMs;
}

MqttBroker DialogHomeAssistantMqtt::serverBroker() const
{
    MqttBroker b;
    b.host = m_serverHost->text().trimmed();
    b.port = m_serverPort->value();
    b.user = m_serverUser->text();
    b.password = m_serverPassword->text();
    return b;
}

void DialogHomeAssistantMqtt::updateProbeHost()
{
    if (!m_probeTyped)
        m_probeHost->setText(MqttBroker::probeHost(m_serverHost->text(), MqttBroker::reachedHost(),
                                                   MqttBroker::calaosHost()));
}

bool DialogHomeAssistantMqtt::eventFilter(QObject *watched, QEvent *event)
{
    if ((watched == m_probeHost || watched == m_prefix) && event->type() == QEvent::KeyPress)
    {
        const int key = static_cast<QKeyEvent *>(event)->key();
        if (key == Qt::Key_Return || key == Qt::Key_Enter)
        {
            if (m_searchButton->isEnabled())
                search();
            return true;
        }
    }
    return QDialog::eventFilter(watched, event);
}

void DialogHomeAssistantMqtt::search()
{
    MqttListener::Settings s;
    s.host = m_probeHost->text().trimmed();
    s.port = quint16(m_serverPort->value());
    if (s.host.isEmpty())
    {
        setStatus(tr("Type the address of the broker as this computer reaches it."), true);
        m_probeHost->setFocus();
        return;
    }
    const QString prefix = HomeAssistantDiscovery::normalizedPrefix(m_prefix->text());
    //The prefix starts topic names: a wildcard in it would also match other topics.
    if (prefix.contains('+') || prefix.contains('#') || !MqttPacket::isValidFilter(prefix + "/#"))
    {
        setStatus(tr("The discovery prefix %1 cannot contain + or #: it is the start of the topics, "
                     "homeassistant by default.").arg(prefix), true);
        m_prefix->setFocus();
        return;
    }
    //The credentials of the project's broker go to that broker only, unless typed for this search.
    if (m_credentialsTyped || MqttBroker::sameBroker(s.host, m_serverHost->text(), MqttBroker::calaosHost()))
    {
        s.user = m_serverUser->text();
        s.password = m_serverPassword->text();
    }

    m_discovery = HomeAssistantDiscovery(m_prefix->text());
    m_found.clear();
    m_list->clear();
    m_list->hide();
    m_error->hide();
    updateImportButton();
    m_searchButton->setEnabled(false);
    setStatus(tr("Connecting to %1:%2...").arg(s.host).arg(s.port), false);
    m_listener->start(s, m_discovery.filters(), m_listenMs, m_answerTimeoutMs);
}

void DialogHomeAssistantMqtt::finished(const QString &error)
{
    m_searchButton->setEnabled(true);
    m_found = m_discovery.entities();
    fillList();

    QString text;
    if (!error.isEmpty())
        text = m_found.isEmpty() ? error : tr("%1 What was read before is listed.").arg(error);
    else if (m_found.isEmpty())
        text = tr("No Home Assistant discovery config under %1/ on this broker. Check the discovery prefix, and "
                  "that the devices have discovery on.").arg(m_discovery.prefix());
    else
        text = tr("Entities found: %1. Check the ones to import.").arg(m_found.size());
    const int unreadable = int(m_discovery.unreadable().size());
    if (unreadable > 0)
        text += " " + tr("Discovery configs that could not be read: %1.").arg(unreadable);
    setStatus(text, !error.isEmpty() || m_found.isEmpty());

    emit searchFinished();
}

void DialogHomeAssistantMqtt::fillList()
{
    QSignalBlocker block(m_list);
    m_list->clear();
    if (m_found.isEmpty())
    {
        m_list->hide();
        return;
    }

    const QSet<QString> inProject = HomeAssistantDiscovery::projectKeys();
    const MqttBroker broker = serverBroker();
    const QColor grey = palette().color(QPalette::Disabled, QPalette::Text);
    QTreeWidgetItem *device = nullptr;
    QString deviceKey;
    for (int i = 0;i < m_found.size();i++)
    {
        const HomeAssistantDiscovery::Entity &e = m_found.at(i);
        if (!device || e.deviceKey != deviceKey)
        {
            deviceKey = e.deviceKey;
            device = new QTreeWidgetItem(m_list, { e.deviceName.isEmpty() ? e.deviceKey : e.deviceName });
            QFont f = device->font(0);
            f.setBold(true);
            device->setFont(0, f);
            device->setFlags(Qt::ItemIsEnabled);
            device->setExpanded(true);
        }

        const HomeAssistantDiscovery::Mapping m = HomeAssistantDiscovery::mappingOf(e, m_templates);
        const bool already = m.ok() && inProject.contains(HomeAssistantDiscovery::keyOf(e, broker));
        QTreeWidgetItem *item = new QTreeWidgetItem(device, { e.ownName(), e.component, m.ok() ? m.what : QString() });
        item->setData(0, ENTITY_ROLE, i);
        item->setToolTip(0, e.topic.section('#', 0, 0));

        QString status;
        if (!m.ok())
            status = tr("not imported: %1").arg(m.reason);
        else if (already)
            status = tr("already in the project");
        else if (e.availability == HomeAssistantDiscovery::OFFLINE)
            status = tr("offline");
        else if (e.availability == HomeAssistantDiscovery::ONLINE)
            status = tr("online");
        item->setText(3, status);
        item->setToolTip(3, status);

        if (m.ok() && !already)
        {
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            item->setCheckState(0, Qt::Unchecked);
            device->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
            device->setCheckState(0, Qt::Unchecked);
        }
        else
            item->setFlags(Qt::ItemIsEnabled);
        if (!m.ok() || already || e.availability == HomeAssistantDiscovery::OFFLINE)
            for (int col = 0;col < m_list->columnCount();col++)
                item->setForeground(col, grey);
    }
    m_list->show();
}

QList<HomeAssistantDiscovery::Entity> DialogHomeAssistantMqtt::checkedEntities() const
{
    QList<HomeAssistantDiscovery::Entity> l;
    for (int d = 0;d < m_list->topLevelItemCount();d++)
    {
        QTreeWidgetItem *device = m_list->topLevelItem(d);
        for (int c = 0;c < device->childCount();c++)
        {
            QTreeWidgetItem *item = device->child(c);
            const int i = item->data(0, ENTITY_ROLE).toInt();
            if ((item->flags() & Qt::ItemIsUserCheckable) && item->checkState(0) == Qt::Checked && i < m_found.size())
                l << m_found.at(i);
        }
    }
    return l;
}

void DialogHomeAssistantMqtt::updateImportButton()
{
    m_importButton->setEnabled(!checkedEntities().isEmpty() && room());
}

void DialogHomeAssistantMqtt::importChecked()
{
    const QList<HomeAssistantDiscovery::Entity> l = checkedEntities();
    if (l.isEmpty() || !room())
        return;

    m_imported = HomeAssistantDiscovery::import(l, m_templates, serverBroker(), room());
    ConfigOptions::Instance().setHADiscoveryPrefix(m_discovery.prefix());
    if (m_imported.ios.empty() && m_imported.devices.empty())
    {
        m_error->setText(m_imported.notes.join('\n'));
        m_error->show();
        return;
    }
    accept();
}

void DialogHomeAssistantMqtt::setStatus(const QString &text, bool error)
{
    m_status->setText(text);
    m_status->setStyleSheet(error ? QStringLiteral("color: rgb(255, 85, 127);") : QString());
    m_status->setForegroundRole(error ? QPalette::WindowText : QPalette::PlaceholderText);
}
