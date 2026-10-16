#include "DialogNewDevice.h"
#include "DeviceTemplates.h"
#include "EsphomeDiscovery.h"
#include "MqttBroker.h"
#ifdef QT_MQTT_AVAILABLE
#include "WidgetEsphomeDiscovery.h"
#endif
#include "WheelGuard.h"

#include <ListeRoom.h>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QTextBrowser>
#include <QVBoxLayout>

using namespace Calaos;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

int decimalsOf(double step)
{
    const QString s = DeviceTemplates::formatNumber(step);
    const int dot = s.indexOf('.');
    return dot < 0 ? 0 : s.size() - dot - 1;
}

}

DialogNewDevice::DialogNewDevice(const QJsonArray &templates, Room *room, QWidget *parent):
    QDialog(parent),
    m_templates(templates),
    m_startRoom(room)
{
    setWindowTitle(tr("New device from a template"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    resize(620, 680);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *top = new QFormLayout();
    m_templateBox = new QComboBox(this);
    m_templateBox->setObjectName("templateBox");
    for (const QJsonValue &t: m_templates)
    {
        const QJsonObject tpl = t.toObject();
        m_templateBox->addItem(tpl["name"].toString(), tpl["id"].toString());
        m_templateBox->setItemData(m_templateBox->count() - 1, tpl["description"].toString(), Qt::ToolTipRole);
    }
    top->addRow(tr("Template:"), m_templateBox);
    layout->addLayout(top);

    m_description = new QLabel(this);
    m_description->setObjectName("templateDescription");
    m_description->setWordWrap(true);
    layout->addWidget(m_description);

#ifdef QT_MQTT_AVAILABLE
    m_discovery = new WidgetEsphomeDiscovery([this](const QString &host)
    {
        const QVariantMap v = values();
        MqttListener::Settings s;
        s.port = quint16(v.value("mqtt_port", 1883).toInt());
        //The credentials of the project's broker go to that broker only, unless the
        //user typed them for this search.
        const bool typed = m_changed.contains("mqtt_user") || m_changed.contains("mqtt_password");
        if (typed || MqttBroker::sameBroker(host, v.value("mqtt_host").toString(), MqttBroker::calaosHost()))
        {
            s.user = v.value("mqtt_user").toString();
            s.password = v.value("mqtt_password").toString();
        }
        return s;
    }, this);
    m_discovery->hide();
    connect(m_discovery, &WidgetEsphomeDiscovery::climateChosen, this, [this](const EsphomeDiscovery::Climate &c)
    {
        QStringList summary;
        setValues(EsphomeDiscovery::prefill(currentTemplate(), c, values(), &summary));
        m_discovery->showApplied(c, summary);
    });
    layout->addWidget(m_discovery);
#endif

    m_scroll = new QScrollArea(this);
    m_scroll->setWidgetResizable(true);
    m_scroll->setFrameShape(QFrame::NoFrame);
    layout->addWidget(m_scroll, 1);

    m_error = new QLabel(this);
    m_error->setObjectName("labelError");
    m_error->setWordWrap(true);
    QFont f = m_error->font();
    f.setItalic(true);
    m_error->setFont(f);
    m_error->setStyleSheet("color: rgb(255, 85, 127);");
    m_error->hide();
    layout->addWidget(m_error);

    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_buttons->button(QDialogButtonBox::Ok)->setText(tr("Create"));
    connect(m_buttons, &QDialogButtonBox::accepted, this, &DialogNewDevice::create);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(m_buttons);

    connect(m_templateBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        buildForm();
    });

    if (m_templates.isEmpty())
    {
        m_description->setText(tr("No device template is available in this installer."));
        m_buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
        buildForm();
}

bool DialogNewDevice::selectTemplate(const QString &id)
{
    const int i = m_templateBox->findData(id);
    if (i < 0)
        return false;
    m_templateBox->setCurrentIndex(i);
    return true;
}

QJsonObject DialogNewDevice::currentTemplate() const
{
    const int i = m_templateBox->currentIndex();
    return i < 0 ? QJsonObject() : m_templates.at(i).toObject();
}

QVariantMap DialogNewDevice::values() const
{
    QVariantMap all;
    for (const FieldEditor &f: m_fields)
        all[f.def["name"].toString()] = valueOf(f);
    return all;
}

void DialogNewDevice::buildForm()
{
    const QJsonObject tpl = currentTemplate();
    m_description->setText(tpl["description"].toString());
    m_error->hide();

    m_fields.clear();
    QWidget *form = new QWidget();
    form->setObjectName("templateForm");
    QFormLayout *layout = new QFormLayout(form);
    layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    //What the user set for another template is kept when this one asks for the same
    //field; a default left alone is this template's.
    QVariantMap start = DeviceTemplates::defaults(tpl, m_startRoom);
    const bool discoverable = EsphomeDiscovery::appliesTo(tpl);
    if (discoverable)
    {
        //The new IOs talk to the broker the project's MQTT IOs already use, password included.
        const MqttBroker b = MqttBroker::ofProject();
        if (b.found)
        {
            start["mqtt_host"] = b.host;
            start["mqtt_port"] = double(b.port);
            start["mqtt_user"] = b.user;
            start["mqtt_password"] = b.password;
        }
    }
    for (auto it = m_changed.constBegin();it != m_changed.constEnd();++it)
    {
        const QJsonObject def = DeviceTemplates::field(tpl, it.key());
        if (!def.isEmpty() && start.contains(it.key()) && start.value(it.key()).userType() == it.value().userType())
            start[it.key()] = it.value();
    }

    for (const QJsonValue &v: tpl["fields"].toArray())
    {
        FieldEditor f;
        f.def = v.toObject();
        const QString type = f.def["type"].toString();

        QWidget *cell = new QWidget(form);
        cell->setObjectName("row_" + f.def["name"].toString());
        QVBoxLayout *cellLayout = new QVBoxLayout(cell);
        cellLayout->setContentsMargins(0, 0, 0, 0);
        cellLayout->setSpacing(2);
        cellLayout->addWidget(createEditor(f, start.value(f.def["name"].toString())));

        QString help = f.def["description"].toString();
        if (type == "io" && qobject_cast<QComboBox *>(f.editor)->count() <= 1)
            help += (help.isEmpty() ? "" : "\n") + tr("No IO of this project fits: create it first.");
        //The link role or param the field feeds, as IODoc names it, with its id.
        for (const DeviceTemplates::Documented &d: DeviceTemplates::documentation(tpl, f.def["name"].toString()))
        {
            const QString what = d.label == d.id ? d.id : QStringLiteral("%1 (%2)").arg(d.label, d.id);
            help += (help.isEmpty() ? "" : "\n") +
                    (d.kind == DeviceTemplates::Documented::Link ? tr("Link: %1") : tr("Parameter: %1")).arg(what);
        }
        if (!help.isEmpty())
        {
            QLabel *hint = new QLabel(help, cell);
            hint->setObjectName("hint_" + f.def["name"].toString());
            hint->setWordWrap(true);
            hint->setForegroundRole(QPalette::PlaceholderText);
            QFont hf = hint->font();
            if (hf.pointSizeF() > 0)
                hf.setPointSizeF(hf.pointSizeF() * 0.9);
            hint->setFont(hf);
            cellLayout->addWidget(hint);
        }

        QLabel *label = new QLabel(type == "bool" ? QString() : tr("%1:").arg(f.def["label"].toString()), form);
        label->setBuddy(f.editor);
        layout->addRow(label, cell);
        m_fields.append(f);
    }

    QWidget *old = m_scroll->takeWidget();
    m_scroll->setWidget(form);
    delete old;

#ifdef QT_MQTT_AVAILABLE
    m_discovery->setVisible(discoverable);
    if (discoverable)
        updateDiscoveryHosts();
#endif
}

#ifdef QT_MQTT_AVAILABLE
void DialogNewDevice::updateDiscoveryHosts()
{
    m_discovery->setHosts(values().value("mqtt_host").toString(), MqttBroker::reachedHost(), MqttBroker::calaosHost());
}
#endif

void DialogNewDevice::setValues(const QVariantMap &v)
{
    for (FieldEditor &f: m_fields)
    {
        const QString name = f.def["name"].toString();
        if (!v.contains(name) || v.value(name) == valueOf(f))
            continue;
        const QVariant value = v.value(name);

        if (QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(f.editor))
            sb->setValue(value.toDouble());
        else if (QCheckBox *cb = qobject_cast<QCheckBox *>(f.editor))
            cb->setChecked(value.toBool());
        else if (QComboBox *box = qobject_cast<QComboBox *>(f.editor))
        {
            const int i = f.def["type"].toString() == "room" ? box->findData(value) : box->findData(value.toString());
            if (i >= 0)
                box->setCurrentIndex(i);
        }
        else if (QLineEdit *le = qobject_cast<QLineEdit *>(f.editor))
            le->setText(value.toString());
        else
        {
            const QStringList picked = value.toStringList();
            for (QCheckBox *choice: std::as_const(f.choices))
                choice->setChecked(picked.contains(choice->property("choice").toString()));
        }
    }
}

QWidget *DialogNewDevice::createEditor(FieldEditor &f, const QVariant &value)
{
    const QString name = f.def["name"].toString();
    const QString type = f.def["type"].toString();
    const QString tip = f.def["description"].toString();
    auto guarded = &WheelGuard::install;

    if (type == "number")
    {
        QDoubleSpinBox *sb = new QDoubleSpinBox();
        const double step = f.def.contains("step") ? f.def["step"].toDouble() : 0.01;
        sb->setDecimals(decimalsOf(step));
        sb->setSingleStep(step);
        sb->setRange(f.def.contains("min") ? f.def["min"].toDouble() : -1e9,
                     f.def.contains("max") ? f.def["max"].toDouble() : 1e9);
        if (!f.def["unit"].toString().isEmpty())
            sb->setSuffix(" " + f.def["unit"].toString());
        sb->setValue(value.toDouble());
        guarded(sb);
        connect(sb, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this, name]() { remember(name); });
        f.editor = sb;
    }
    else if (type == "bool")
    {
        QCheckBox *cb = new QCheckBox(f.def["label"].toString());
        cb->setChecked(value.toBool());
        connect(cb, &QCheckBox::toggled, this, [this, name]() { remember(name); });
        f.editor = cb;
    }
    else if (type == "enum" || type == "room" || type == "io")
    {
        QComboBox *box = new QComboBox();
        guarded(box);
        if (type == "enum")
        {
            for (const QJsonValue &o: f.def["values"].toArray())
                box->addItem(o.toObject()["label"].toString(), o.toObject()["value"].toString());
            box->setCurrentIndex(qMax(0, box->findData(value.toString())));
        }
        else if (type == "room")
        {
            for (int i = 0;i < ListeRoom::Instance().size();i++)
            {
                Room *room = ListeRoom::Instance().get_room(i);
                box->addItem(u(room->get_name()), QVariant::fromValue(room));
                if (room == value.value<Room *>())
                    box->setCurrentIndex(i);
            }
        }
        else
        {
            box->addItem(f.def["required"].toBool() ? tr("Choose an IO") : tr("None"), QString());
            for (IOBase *io: DeviceTemplates::candidates(currentTemplate(), f.def))
            {
                Room *room = ListeRoom::Instance().get_room(ListeRoom::Instance().searchIO(io));
                const QString id = u(io->get_param("id"));
                box->addItem(QStringLiteral("%1 (%2) - %3").arg(u(io->get_param("name")), id, u(room->get_name())), id);
            }
            box->setCurrentIndex(qMax(0, box->findData(value.toString())));
        }
        connect(box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, name]() { remember(name); });
        f.editor = box;
    }
    else if (type == "multi")
    {
        QWidget *w = new QWidget();
        QGridLayout *grid = new QGridLayout(w);
        grid->setContentsMargins(0, 0, 0, 0);
        const QStringList picked = value.toStringList();
        const QJsonArray options = f.def["values"].toArray();
        for (int i = 0;i < options.size();i++)
        {
            const QJsonObject o = options.at(i).toObject();
            QCheckBox *cb = new QCheckBox(o["label"].toString(), w);
            cb->setObjectName(QStringLiteral("field_%1_%2").arg(name, o["value"].toString()));
            cb->setProperty("choice", o["value"].toString());
            cb->setChecked(picked.contains(o["value"].toString()));
            connect(cb, &QCheckBox::toggled, this, [this, name]() { remember(name); });
            grid->addWidget(cb, i / 3, i % 3);
            f.choices.append(cb);
        }
        f.editor = w;
    }
    else
    {
        //string, and any type this installer does not know: validate() names the latter.
        QLineEdit *le = new QLineEdit(value.toString());
        if (f.def["secret"].toBool())
            le->setEchoMode(QLineEdit::Password);
        connect(le, &QLineEdit::textChanged, this, [this, name]() { remember(name); });
        f.editor = le;
    }

    f.editor->setObjectName("field_" + name);
    f.editor->setToolTip(tip);
    return f.editor;
}

void DialogNewDevice::remember(const QString &name)
{
    for (const FieldEditor &f: m_fields)
        if (f.def["name"].toString() == name)
            m_changed[name] = valueOf(f);
#ifdef QT_MQTT_AVAILABLE
    if (name == "mqtt_host" && m_discovery->isVisibleTo(this))
        updateDiscoveryHosts();
#endif
}

QVariant DialogNewDevice::valueOf(const FieldEditor &f) const
{
    const QString type = f.def["type"].toString();

    if (QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(f.editor))
        return sb->value();
    if (QCheckBox *cb = qobject_cast<QCheckBox *>(f.editor))
        return cb->isChecked();
    if (QComboBox *box = qobject_cast<QComboBox *>(f.editor))
        return type == "room" ? box->currentData() : QVariant(box->currentData().toString());
    if (QLineEdit *le = qobject_cast<QLineEdit *>(f.editor))
        return f.def["secret"].toBool() ? le->text() : le->text().trimmed();

    QStringList picked;
    for (QCheckBox *cb: f.choices)
        if (cb->isChecked())
            picked << cb->property("choice").toString();
    return picked;
}

void DialogNewDevice::create()
{
    const QJsonObject tpl = currentTemplate();
    const QVariantMap v = values();

    const QList<DeviceTemplates::Problem> problems = DeviceTemplates::validate(tpl, v);
    if (!problems.isEmpty())
    {
        QStringList lines;
        for (const DeviceTemplates::Problem &p: problems)
            lines << p.message;
        m_error->setText(lines.join('\n'));
        m_error->show();

        for (const FieldEditor &f: m_fields)
        {
            if (f.def["name"].toString() != problems.first().field)
                continue;
            QWidget *target = f.choices.isEmpty() ? f.editor : f.choices.first();
            m_scroll->ensureWidgetVisible(target);
            target->setFocus();
        }
        return;
    }

    m_device = DeviceTemplates::create(tpl, v, &m_notes);
    if (!m_device)
    {
        m_error->setText(tr("The device could not be created."));
        m_error->show();
        return;
    }
    m_room = DeviceTemplates::roomOf(tpl, v);

    accept();
}

QDialog *DialogNewDevice::createNotesDialog(const QString &deviceName, const QStringList &notes, QWidget *parent)
{
    QDialog *d = new QDialog(parent);
    d->setWindowTitle(tr("Device created"));
    d->setWindowFlags(d->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    d->resize(640, 520);

    QVBoxLayout *layout = new QVBoxLayout(d);
    QLabel *intro = new QLabel(tr("%1 is created. Its setup needs a few more steps:").arg(deviceName), d);
    intro->setWordWrap(true);
    layout->addWidget(intro);

    QTextBrowser *text = new QTextBrowser(d);
    text->setObjectName("notesText");
    text->setOpenExternalLinks(true);
    text->setMarkdown(notes.join("\n\n"));
    layout->addWidget(text, 1);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, d);
    connect(buttons, &QDialogButtonBox::rejected, d, &QDialog::reject);
    layout->addWidget(buttons);

    return d;
}
