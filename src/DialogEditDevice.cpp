#include "DialogEditDevice.h"
#include "DeviceTemplates.h"
#include "WheelGuard.h"

#include <ListeRoom.h>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

using namespace Calaos;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

QString ioLabel(IOBase *io)
{
    const int r = ListeRoom::Instance().searchIO(io);
    const QString room = r < 0 ? QString() : u(ListeRoom::Instance().get_room(r)->get_name());
    return QStringLiteral("%1 (%2) - %3").arg(u(io->get_param("name")), u(io->get_param("id")), room);
}

//The editor and the description under it, as in the "new device" dialog.
QWidget *withHint(QWidget *editor, const QString &hint, QWidget *parent)
{
    QWidget *cell = new QWidget(parent);
    QVBoxLayout *l = new QVBoxLayout(cell);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(2);
    l->addWidget(editor);
    if (!hint.isEmpty())
    {
        QLabel *h = new QLabel(hint, cell);
        h->setWordWrap(true);
        h->setForegroundRole(QPalette::PlaceholderText);
        QFont f = h->font();
        if (f.pointSizeF() > 0)
            f.setPointSizeF(f.pointSizeF() * 0.9);
        h->setFont(f);
        l->addWidget(h);
    }
    return cell;
}

}

DialogEditDevice::DialogEditDevice(Device *device, QWidget *parent):
    QDialog(parent),
    m_device(device)
{
    setWindowTitle(tr("Device: %1").arg(u(device->get_param("name"))));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    resize(640, 700);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *top = new QFormLayout();
    top->addRow(tr("Id:"), new QLabel(u(device->get_param("id")), this));
    top->addRow(tr("Family:"), new QLabel(QStringLiteral("%1 / %2").arg(u(device->family()), u(device->impl())), this));
    layout->addLayout(top);

    const DeviceEdit::State start = DeviceEdit::current(device);
    QWidget *body = new QWidget();
    QVBoxLayout *bodyLayout = new QVBoxLayout(body);
    bodyLayout->addWidget(buildLinks(start));
    bodyLayout->addWidget(buildParams(start));
    if (!device->ownedIos().empty())
        bodyLayout->addWidget(buildOwnedIos());
    bodyLayout->addStretch(1);

    m_scroll = new QScrollArea(this);
    m_scroll->setWidgetResizable(true);
    m_scroll->setFrameShape(QFrame::NoFrame);
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scroll->setWidget(body);
    layout->addWidget(m_scroll, 1);

    m_warning = new QLabel(this);
    m_warning->setObjectName("labelWarning");
    m_warning->setWordWrap(true);
    layout->addWidget(m_warning);

    m_error = new QLabel(this);
    m_error->setObjectName("labelError");
    m_error->setWordWrap(true);
    QFont f = m_error->font();
    f.setItalic(true);
    m_error->setFont(f);
    m_error->setStyleSheet("color: rgb(255, 85, 127);");
    m_error->hide();
    layout->addWidget(m_error);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &DialogEditDevice::save);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    refresh();
}

QWidget *DialogEditDevice::buildLinks(const DeviceEdit::State &start)
{
    QGroupBox *group = new QGroupBox(tr("Links"));
    QFormLayout *form = new QFormLayout(group);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    for (const DeviceEdit::Role &role: DeviceEdit::roles(m_device))
    {
        LinkEditor e;
        e.role = role;
        e.label = new QLabel(group);
        QWidget *editor;

        if (!start.links.contains(role.name))
        {
            //Its IO is the device's own: it goes and comes with the device.
            IOBase *io = nullptr;
            for (const DeviceLink &l: m_device->links())
                if (u(l.name) == role.name)
                    io = DeviceTemplates::ioById(l.io);
            QLabel *owned = new QLabel(tr("%1, owned by this device").arg(io ? ioLabel(io) : QString()), group);
            owned->setWordWrap(true);
            editor = owned;
        }
        else
        {
            e.box = new QComboBox(group);
            WheelGuard::install(e.box);
            //Long IO names must not widen the form past the dialog.
            e.box->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
            e.box->setMinimumContentsLength(20);
            e.box->addItem(tr("None"), QString());
            for (IOBase *io: DeviceEdit::candidates(role, m_device))
                e.box->addItem(ioLabel(io), u(io->get_param("id")));

            const QString current = start.links.value(role.name);
            if (!current.isEmpty() && e.box->findData(current) < 0)
            {
                //Kept as it is in the file until the user picks another.
                IOBase *io = DeviceTemplates::ioById(current.toStdString());
                e.box->addItem(io ? tr("%1, of another kind").arg(ioLabel(io)) : tr("%1 (not found)").arg(current), current);
            }
            e.box->setCurrentIndex(qMax(0, e.box->findData(current)));
            connect(e.box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DialogEditDevice::refresh);
            editor = e.box;
        }

        editor->setObjectName("link_" + role.name);
        form->addRow(e.label, withHint(editor, role.description, group));
        m_links.append(e);
    }

    if (m_links.isEmpty())
        form->addRow(new QLabel(tr("IODoc documents no link for this device."), group));
    return group;
}

QWidget *DialogEditDevice::buildParams(const DeviceEdit::State &start)
{
    QGroupBox *group = new QGroupBox(tr("Parameters"));
    QFormLayout *form = new QFormLayout(group);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    for (const DeviceEdit::Param &param: DeviceEdit::params(m_device))
    {
        ParamEditor e;
        e.param = param;
        e.wasSet = start.params.contains(param.name);
        const QString value = start.params.value(param.name);
        QWidget *editor;

        if (param.type == "bool")
        {
            e.check = new QCheckBox(group);
            e.check->setChecked((e.wasSet ? value : param.defaultValue) == "true");
            editor = e.check;
        }
        else if (param.type == "list" && !param.listValues.isEmpty())
        {
            e.list = new QComboBox(group);
            WheelGuard::install(e.list);
            e.list->addItem(param.defaultValue.isEmpty() ? tr("Not set") : tr("Default (%1)").arg(param.defaultValue));
            for (auto it = param.listValues.constBegin();it != param.listValues.constEnd();++it)
                e.list->addItem(it.value().toString(), it.key());
            if (e.wasSet && e.list->findData(value) < 0)
                e.list->addItem(value, value);
            e.list->setCurrentIndex(e.wasSet ? e.list->findData(value) : 0);
            editor = e.list;
        }
        else
        {
            e.line = new QLineEdit(value, group);
            if (!param.mandatory)
                e.line->setPlaceholderText(param.defaultValue.isEmpty() ? tr("Not set")
                                                                        : tr("Default: %1").arg(param.defaultValue));
            editor = e.line;
        }

        editor->setObjectName("param_" + param.name);
        QString hint = param.description;
        if (!param.min.isEmpty() || !param.max.isEmpty())
            hint += (hint.isEmpty() ? "" : " ") + tr("(%1 to %2)").arg(param.min.isEmpty() ? "-" : param.min,
                                                                         param.max.isEmpty() ? "-" : param.max);
        form->addRow(param.name + ":", withHint(editor, hint, group));
        e.raw = value;
        e.initial = editorValue(e);
        if (e.check)
            connect(e.check, &QCheckBox::toggled, this, &DialogEditDevice::refresh);
        else if (e.list)
            connect(e.list, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DialogEditDevice::refresh);
        else
            connect(e.line, &QLineEdit::textChanged, this, &DialogEditDevice::refresh);
        m_params.append(e);
    }
    return group;
}

QWidget *DialogEditDevice::buildOwnedIos()
{
    QGroupBox *group = new QGroupBox(tr("Owned IOs"));
    QVBoxLayout *l = new QVBoxLayout(group);
    QLabel *note = new QLabel(tr("Created with the device and deleted with it; edit each one from the tree."), group);
    note->setWordWrap(true);
    l->addWidget(note);
    for (IOBase *io: m_device->ownedIos())
        l->addWidget(new QLabel(QStringLiteral("%1 (%2, %3)").arg(u(io->get_param("name")), u(io->get_param("type")),
                                                                  u(io->get_param("id"))), group));
    return group;
}

DeviceEdit::State DialogEditDevice::state() const
{
    DeviceEdit::State s;
    for (const LinkEditor &e: m_links)
        if (e.box)
            s.links[e.role.name] = e.box->currentData().toString();

    for (const ParamEditor &e: m_params)
    {
        //An editor left as it opened keeps the file's text, whatever it is (" 0.5", "", "1").
        if (editorValue(e) == e.initial)
        {
            if (e.wasSet)
                s.params[e.param.name] = e.raw;
        }
        else if (e.check)
            s.params[e.param.name] = e.check->isChecked() ? "true" : "false";
        else if (e.list)
        {
            if (!e.list->currentData().toString().isEmpty())
                s.params[e.param.name] = e.list->currentData().toString();
        }
        else
        {
            const QString v = e.line->text().trimmed();
            if (!v.isEmpty() || e.param.mandatory)
                s.params[e.param.name] = v;
        }
    }
    return s;
}

void DialogEditDevice::refresh()
{
    const DeviceEdit::State s = state();
    for (LinkEditor &e: m_links)
        e.label->setText(DeviceEdit::isRequired(m_device, e.role, s) ? tr("%1 (required):").arg(e.role.name)
                                                                      : tr("%1:").arg(e.role.name));

    const QStringList missing = DeviceEdit::missingRequired(m_device, s);
    m_warning->setText(missing.isEmpty() ? QString()
                                         : tr("No IO for %1: the device loads without what this link gives.")
                                               .arg(missing.join(", ")));
    m_warning->setVisible(!missing.isEmpty());
}

QString DialogEditDevice::editorValue(const ParamEditor &e)
{
    if (e.check)
        return e.check->isChecked() ? "true" : "false";
    if (e.list)
        return QString::number(e.list->currentIndex());
    return e.line->text();
}

void DialogEditDevice::save()
{
    const DeviceEdit::State s = state();
    if (DeviceEdit::isUnchanged(m_device, s))
    {
        accept();
        return;
    }

    const QList<DeviceEdit::Problem> problems = DeviceEdit::validate(m_device, s);
    if (!problems.isEmpty())
    {
        QStringList lines;
        for (const DeviceEdit::Problem &p: problems)
            lines << p.message;
        m_error->setText(lines.join('\n'));
        m_error->show();

        if (QWidget *w = findChild<QWidget *>("link_" + problems.first().field))
            w->setFocus();
        else if (QWidget *w = findChild<QWidget *>("param_" + problems.first().field))
            w->setFocus();
        if (QWidget *w = focusWidget())
            m_scroll->ensureWidgetVisible(w);
        return;
    }

    DeviceEdit::apply(m_device, s);
    m_changed = true;
    accept();
}
