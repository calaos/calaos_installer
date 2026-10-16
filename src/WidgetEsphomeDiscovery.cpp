#include "WidgetEsphomeDiscovery.h"
#include "MqttBroker.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <algorithm>

namespace
{

QString publishes(const EsphomeDiscovery::Climate &c)
{
    QStringList l;
    if (c.topics.contains("target_temperature_low") || c.topics.contains("target_temperature_high"))
        l << WidgetEsphomeDiscovery::tr("two setpoints");
    else if (c.topics.contains("target_temperature"))
        l << WidgetEsphomeDiscovery::tr("one setpoint");
    if (c.topics.contains("fan_mode"))
        l << WidgetEsphomeDiscovery::tr("fan");
    if (c.topics.contains("swing_mode"))
        l << WidgetEsphomeDiscovery::tr("swing");
    if (c.topics.contains("action"))
        l << WidgetEsphomeDiscovery::tr("action");
    if (!c.states.value("mode").isEmpty())
        l << WidgetEsphomeDiscovery::tr("now %1").arg(c.states.value("mode"));
    return l.join(", ");
}

}

WidgetEsphomeDiscovery::WidgetEsphomeDiscovery(std::function<MqttListener::Settings(const QString &host)> settings,
                                               QWidget *parent):
    QGroupBox(tr("Find it on the MQTT broker"), parent),
    m_settings(std::move(settings)),
    m_listener(new MqttListener(this))
{
    setObjectName("discoveryBox");
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *row = new QHBoxLayout();
    QLabel *hostLabel = new QLabel(tr("Broker address from this computer:"), this);
    m_host = new QLineEdit(this);
    m_host->setObjectName("discoveryHost");
    m_host->setPlaceholderText(tr("Address of the Calaos box"));
    m_host->setToolTip(tr("The form's broker address is the one the Calaos server uses; 127.0.0.1 there "
                          "is the box itself. Port, user and password are the form's."));
    hostLabel->setBuddy(m_host);
    //Enter there searches; the dialog would take it for its default button, Create.
    m_host->installEventFilter(this);
    m_button = new QPushButton(tr("Search"), this);
    m_button->setObjectName("discoveryButton");
    m_button->setDefault(false);
    m_button->setAutoDefault(false);
    row->addWidget(hostLabel);
    row->addWidget(m_host, 1);
    row->addWidget(m_button);
    layout->addLayout(row);

    m_status = new QLabel(tr("Lists the ESPHome climates the broker knows; choosing one fills in the form."), this);
    m_status->setObjectName("discoveryStatus");
    m_status->setWordWrap(true);
    m_status->setForegroundRole(QPalette::PlaceholderText);
    layout->addWidget(m_status);

    m_list = new QTreeWidget(this);
    m_list->setObjectName("discoveryList");
    m_list->setHeaderLabels({ tr("Name"), tr("Topic prefix"), tr("Object id"), tr("Publishes"), tr("Status") });
    m_list->setRootIsDecorated(false);
    m_list->setAlternatingRowColors(true);
    //Long "publishes" texts are elided (full text in the tooltip): a horizontal
    //scroll bar would hide the rows of a list sized to them.
    m_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_list->header()->setStretchLastSection(false);
    m_list->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_list->header()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_list->hide();
    layout->addWidget(m_list);

    m_applied = new QLabel(this);
    m_applied->setObjectName("discoveryApplied");
    m_applied->setWordWrap(true);
    m_applied->hide();
    layout->addWidget(m_applied);

    connect(m_host, &QLineEdit::textEdited, this, [this]() { m_hostTyped = true; });
    connect(m_button, &QPushButton::clicked, this, &WidgetEsphomeDiscovery::search);
    connect(m_listener, &MqttListener::message, this, [this](const QString &topic, const QByteArray &payload)
    {
        m_discovery.feed(topic, payload);
    });
    connect(m_listener, &MqttListener::listening, this, [this]()
    {
        setStatus(tr("Listening to the broker..."), false);
    });
    connect(m_listener, &MqttListener::finished, this, &WidgetEsphomeDiscovery::finished);
    auto choose = [this](QTreeWidgetItem *item)
    {
        const int i = item ? m_list->indexOfTopLevelItem(item) : -1;
        if (i >= 0 && i < m_found.size())
            emit climateChosen(m_found.at(i));
    };
    connect(m_list, &QTreeWidget::currentItemChanged, this, choose);
    //Again after the form was changed by hand.
    connect(m_list, &QTreeWidget::itemActivated, this, choose);
}

void WidgetEsphomeDiscovery::setHosts(const QString &brokerHost, const QString &reachedBroker, const QString &calaosHost)
{
    if (!m_hostTyped)
        m_host->setText(MqttBroker::probeHost(brokerHost, reachedBroker, calaosHost));
}

bool WidgetEsphomeDiscovery::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_host && event->type() == QEvent::KeyPress)
    {
        const int key = static_cast<QKeyEvent *>(event)->key();
        if (key == Qt::Key_Return || key == Qt::Key_Enter)
        {
            if (m_button->isEnabled())
                search();
            return true;
        }
    }
    return QGroupBox::eventFilter(watched, event);
}

void WidgetEsphomeDiscovery::setTimings(int listenMs, int answerTimeoutMs)
{
    m_listenMs = listenMs;
    m_answerTimeoutMs = answerTimeoutMs;
}

void WidgetEsphomeDiscovery::search()
{
    const QString host = m_host->text().trimmed();
    MqttListener::Settings s = m_settings(host);
    s.host = host;
    if (s.host.isEmpty())
    {
        setStatus(tr("Type the address of the broker as this computer reaches it."), true);
        m_host->setFocus();
        return;
    }

    m_discovery.clear();
    m_found.clear();
    m_list->clear();
    m_list->hide();
    m_applied->hide();
    m_button->setEnabled(false);
    setStatus(tr("Connecting to %1:%2...").arg(s.host).arg(s.port), false);
    m_listener->start(s, EsphomeDiscovery::filters(), m_listenMs, m_answerTimeoutMs);
}

void WidgetEsphomeDiscovery::finished(const QString &error)
{
    m_button->setEnabled(true);
    m_found = m_discovery.climates();

    if (!m_found.isEmpty())
    {
        QSignalBlocker block(m_list);
        for (const EsphomeDiscovery::Climate &c: std::as_const(m_found))
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(m_list, { c.displayName(), c.prefix, c.objectId, publishes(c) });
            item->setToolTip(0, c.ip.isEmpty() ? c.key() : QStringLiteral("%1 (%2)").arg(c.key(), c.ip));
            item->setToolTip(3, item->text(3));
            if (c.stale)
            {
                item->setText(4, tr("old or offline"));
                item->setToolTip(4, tr("Neither discovered nor online: likely retained messages of a former "
                                       "object id."));
                for (int col = 0;col < m_list->columnCount();col++)
                    item->setForeground(col, palette().color(QPalette::Disabled, QPalette::Text));
            }
            else if (c.status == "online")
                item->setText(4, tr("online"));
            else if (c.status == "offline")
                item->setText(4, tr("offline"));
        }
        m_list->setCurrentItem(nullptr);
        //As high as its rows, up to five: the form below needs the room.
        const int rows = std::min(int(m_found.size()), 5);
        m_list->setFixedHeight(m_list->header()->sizeHint().height() + rows * m_list->sizeHintForRow(0)
                               + 2 * m_list->frameWidth() + 2);
        m_list->show();
    }

    if (!error.isEmpty())
        setStatus(m_found.isEmpty() ? error : tr("%1 What was read before is listed.").arg(error), true);
    else if (m_found.isEmpty())
        setStatus(tr("No ESPHome climate on this broker. Check that the node has an mqtt: block and is "
                     "online, or fill in the form by hand."), true);
    else
        setStatus(m_found.size() == 1 ? tr("One climate found: choose it to fill in the form.")
                                      : tr("%1 climates found: choose the one to fill in the form with.").arg(m_found.size()),
                  false);

    emit searchFinished();
}

void WidgetEsphomeDiscovery::showApplied(const EsphomeDiscovery::Climate &c, const QStringList &summary)
{
    m_applied->setText(tr("Form filled in from %1.").arg(c.key()) + " " + summary.join(' '));
    m_applied->show();
}

void WidgetEsphomeDiscovery::setStatus(const QString &text, bool error)
{
    m_status->setText(text);
    m_status->setStyleSheet(error ? QStringLiteral("color: rgb(255, 85, 127);") : QString());
    m_status->setForegroundRole(error ? QPalette::WindowText : QPalette::PlaceholderText);
}
