#include "DeviceCommandForm.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <cmath>

namespace
{

// A field that is not sent looks like a placeholder; the box on its left says it.
const char *s_unsetStyle = "QComboBox, QDoubleSpinBox { background: transparent; color: #bdbdbd;"
                           " border: 1px dashed #6e6e6e; padding: 2px; }";

// Same amber as the out-of-range values of the generic IO editor.
const char *s_flaggedStyle = "QComboBox, QDoubleSpinBox { background-color: #ffe8b3; color: black; }";

const char *s_noteStyle = "color: #bdbdbd;";

int decimalsNeeded(double v, int atLeast)
{
    for (int d = atLeast;d < 6;d++)
    {
        const double scaled = v * std::pow(10.0, d);
        if (std::fabs(scaled - std::round(scaled)) < 1e-9)
            return d;
    }
    return 6;
}

}

DeviceCommandForm::DeviceCommandForm(QWidget *parent) :
    QWidget(parent),
    m_grid(new QGridLayout),
    m_notice(new QLabel),
    m_keptLabel(new QLabel),
    m_rangeLabel(new QLabel)
{
    QVBoxLayout *main = new QVBoxLayout(this);
    main->setContentsMargins(0, 0, 0, 0);
    main->setSpacing(4);

    m_grid->setContentsMargins(0, 0, 0, 0);
    m_grid->setHorizontalSpacing(10);
    m_grid->setVerticalSpacing(5);
    m_grid->setColumnStretch(1, 1);
    main->addLayout(m_grid);

    for (QLabel *l : { m_notice, m_keptLabel, m_rangeLabel })
    {
        l->setWordWrap(true);
        l->setStyleSheet(s_noteStyle);
        l->hide();
        main->addWidget(l);
    }
}

QString DeviceCommandForm::labelOf(const QString &field)
{
    if (field == "mode") return tr("Mode");
    if (field == "target_temp") return tr("Target temperature");
    if (field == "target_temp_low") return tr("Low target");
    if (field == "target_temp_high") return tr("High target");
    if (field == "fan_mode") return tr("Fan mode");
    if (field == "swing_mode") return tr("Swing mode");

    QString s = field;
    s.replace('_', ' ');
    return s.left(1).toUpper() + s.mid(1);
}

QString DeviceCommandForm::valueLabel(const QString &value)
{
    if (value == "off") return tr("Off");
    if (value == "heat") return tr("Heat");
    if (value == "cool") return tr("Cool");
    if (value == "auto") return tr("Auto");
    if (value == "dry") return tr("Dry");
    if (value == "fan_only") return tr("Fan only");
    return value;
}

void DeviceCommandForm::clearRows()
{
    for (Row &r : m_rows)
    {
        delete r.check;
        delete r.editor;
    }
    m_rows.clear();
}

void DeviceCommandForm::setDevice(const QJsonObject &deviceDoc, const DeviceCommand::Instance &instance)
{
    m_doc = deviceDoc;
    m_instance = instance;
    m_offer = DeviceCommand::offer(deviceDoc, instance);
    m_kept = QJsonObject();

    clearRows();
    buildRows();
    updateLabels();
}

void DeviceCommandForm::buildRows()
{
    for (const DeviceCommand::Field &f : m_offer.fields)
    {
        Row r;
        r.field = f;

        r.check = new QCheckBox(labelOf(f.name));
        r.check->setToolTip(f.description.isEmpty() ? f.name : f.name + "\n" + f.description);

        if (f.type == DeviceCommand::Enum)
        {
            QComboBox *c = new QComboBox;
            for (const QString &v : f.values)
                c->addItem(valueLabel(v), v);
            r.editor = c;
        }
        else
        {
            QDoubleSpinBox *s = new QDoubleSpinBox;
            s->setDecimals(f.decimals);
            s->setRange(f.min, f.max);
            s->setSingleStep(f.step);
            if (!f.unit.isEmpty())
                s->setSuffix(" " + f.unit);
            s->setValue(f.min + std::round((f.max - f.min) / 2 / f.step) * f.step);
            r.editor = s;
        }
        r.editor->setToolTip(r.check->toolTip());
        r.editor->setMinimumWidth(130);

        const int index = m_rows.size();
        const int row = index;
        m_grid->addWidget(r.check, row, 0);
        m_grid->addWidget(r.editor, row, 1);

        connect(r.check, &QCheckBox::toggled, this, [this, index](bool on)
        {
            if (!on)
                m_rows[index].hasRaw = false;
            setRowSet(m_rows[index], on);
            if (!m_loading)
                emitChanged();
        });

        if (QComboBox *c = qobject_cast<QComboBox *>(r.editor))
        {
            connect(c, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, index]() { rowEdited(index); });
            // Choosing the value the list already shows changes no index, and still means "send this".
            connect(c, QOverload<int>::of(&QComboBox::activated), this, [this, index]()
            {
                if (!m_loading && !m_rows[index].check->isChecked())
                    m_rows[index].check->setChecked(true);
            });
        }
        else
        {
            connect(static_cast<QDoubleSpinBox *>(r.editor), QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                    [this, index]() { rowEdited(index); });
        }

        m_rows.append(r);
        setRowSet(m_rows.last(), false);
    }
}

void DeviceCommandForm::setRowSet(Row &row, bool set)
{
    // Dropping a flagged value discards it: nothing is left to warn about.
    if (row.flagged && set)
        return;

    row.flagged = false;
    row.editor->setToolTip(row.check->toolTip());
    row.editor->setStyleSheet(set ? QString() : QString(s_unsetStyle));
}

void DeviceCommandForm::rowEdited(int index)
{
    if (m_loading)
        return;

    Row &row = m_rows[index];
    row.hasRaw = false;
    // Editing a flagged value is the way to correct it.
    if (row.flagged)
    {
        row.flagged = false;
        row.editor->setToolTip(row.check->toolTip());
    }

    if (!row.check->isChecked())
        row.check->setChecked(true); // emits through toggled
    else
    {
        setRowSet(row, true);
        emitChanged();
    }
}

void DeviceCommandForm::flag(Row &row, const QString &why)
{
    row.flagged = true;
    row.editor->setStyleSheet(s_flaggedStyle);
    row.editor->setToolTip(why);
}

void DeviceCommandForm::setValue(const QString &text)
{
    m_loading = true;

    QJsonObject obj;
    if (DeviceCommand::classify(text, m_doc) == DeviceCommand::PlainMode)
        obj.insert("mode", text.trimmed());
    else
        DeviceCommand::parse(text, obj);

    m_kept = obj;

    for (Row &r : m_rows)
    {
        r.flagged = false;
        r.hasRaw = false;
        r.editor->setToolTip(r.check->toolTip());

        bool consumed = false;
        const QJsonValue v = obj.value(r.field.name);

        if (r.field.type == DeviceCommand::Enum)
        {
            QComboBox *c = static_cast<QComboBox *>(r.editor);
            // Back to the offered list, then the stored value if it is outside.
            while (c->count() > r.field.values.size())
                c->removeItem(c->count() - 1);

            if (v.isString())
            {
                consumed = true;
                int i = c->findData(v.toString());
                if (i < 0)
                {
                    c->addItem(v.toString(), v.toString());
                    i = c->count() - 1;
                    flag(r, whyOutside(r, v.toString()));
                }
                c->setCurrentIndex(i);
            }
        }
        else
        {
            QDoubleSpinBox *s = static_cast<QDoubleSpinBox *>(r.editor);
            s->setRange(r.field.min, r.field.max);
            s->setDecimals(r.field.decimals);

            if (v.isDouble())
            {
                consumed = true;
                const double d = v.toDouble();
                r.hasRaw = true;
                r.raw = d;
                // QDoubleSpinBox clamps and rounds silently, which would rewrite the stored number.
                s->setDecimals(decimalsNeeded(d, r.field.decimals));
                s->setRange(qMin(r.field.min, d), qMax(r.field.max, d));
                s->setValue(d);
                if (d < r.field.min || d > r.field.max)
                {
                    flag(r, tr("The value %1 is outside the bounds of this device (%2 to %3). "
                               "It is kept as is unless you change it.")
                         .arg(d).arg(r.field.min).arg(r.field.max));
                }
            }
        }

        if (consumed)
            m_kept.remove(r.field.name);

        r.check->setChecked(consumed);
        if (!r.flagged)
            setRowSet(r, consumed);
    }

    m_loading = false;
    updateLabels();
}

double DeviceCommandForm::numberOf(const Row &row)
{
    return row.hasRaw ? row.raw : static_cast<QDoubleSpinBox *>(row.editor)->value();
}

QString DeviceCommandForm::whyOutside(const Row &row, const QString &value) const
{
    const QString keep = tr("It is kept as is unless you change it.");
    const DeviceCommand::Field &f = row.field;

    if (f.excludedByParam.contains(value))
        return tr("%1 is not in the %2 param of this device.").arg(value, f.excludedByParam.value(value)) + " " + keep;
    if (f.excludedByDefault.contains(value))
        return tr("%1 is not offered by default with the %2 link and no modes param.")
                   .arg(value, f.excludedByDefault.value(value)) + " " + keep;
    if (f.excludedByNeeds.contains(value))
        return tr("%1 needs %2, which this device does not offer.")
                   .arg(value, f.excludedByNeeds.value(value).join(tr(" and "))) + " " + keep;
    if (f.excludedByLinks.contains(value))
        return tr("%1 needs the link %2, which this device lacks or cannot write.")
            .arg(value, f.excludedByLinks.value(value).join(", ")) + " " + keep;
    if (!f.valuesParam.isEmpty())
        return tr("%1 is not in the %2 param of this device.").arg(value, f.valuesParam) + " " + keep;

    return tr("%1 is not a documented value of %2.").arg(value, f.name) + " " + keep;
}

QString DeviceCommandForm::value() const
{
    QHash<QString, QJsonValue> values;
    for (const Row &r : m_rows)
    {
        if (!r.check->isChecked())
            continue;

        if (r.field.type == DeviceCommand::Enum)
            values.insert(r.field.name, static_cast<QComboBox *>(r.editor)->currentData().toString());
        else
            values.insert(r.field.name, numberOf(r));
    }

    QList<DeviceCommand::Field> fields;
    for (const Row &r : m_rows)
        fields << r.field;

    return DeviceCommand::compose(fields, values, m_kept);
}

void DeviceCommandForm::emitChanged()
{
    updateLabels();
    emit valueChanged(value());
}

void DeviceCommandForm::updateLabels()
{
    if (m_rows.isEmpty())
    {
        m_notice->setText(tr("No field to offer: this device has no linked IO to command yet."));
        m_notice->setToolTip(QString());
        m_notice->show();
    }
    else if (!m_offer.hidden.isEmpty())
    {
        QStringList names;
        for (const QString &h : m_offer.hidden)
        {
            names << (m_offer.unwritable.contains(h) ? tr("%1 (link not writable)").arg(labelOf(h))
                                                     : labelOf(h));
        }
        m_notice->setText(tr("%n more field(s) need a link on this device", "", m_offer.hidden.size()));
        m_notice->setToolTip(names.join(", "));
        m_notice->show();
    }
    else
    {
        m_notice->hide();
    }

    if (m_kept.isEmpty())
    {
        m_keptLabel->hide();
    }
    else
    {
        m_keptLabel->setText(tr("Sent as is: %1").arg(m_kept.keys().join(", ")));
        m_keptLabel->setToolTip(tr("These keys of the stored command have no field in this form."));
        m_keptLabel->show();
    }

    updateRangeFlag();
}

// The server refuses a pair closer than min_range, or inverted, as a whole.
void DeviceCommandForm::updateRangeFlag()
{
    Row *low = nullptr, *high = nullptr;
    for (Row &r : m_rows)
    {
        if (r.field.name == "target_temp_low")
            low = &r;
        else if (r.field.name == "target_temp_high")
            high = &r;
    }

    bool bad = false;
    double gap = 0;
    if (low && high && low->check->isChecked() && high->check->isChecked())
    {
        bool ok = false;
        gap = m_instance.params.value("min_range").toDouble(&ok);
        if (!ok || gap < 0)
            gap = 0;
        const double l = numberOf(*low), h = numberOf(*high);
        bad = h - l < gap - 1e-9; // also true when inverted
    }

    for (Row *r : { low, high })
    {
        if (!r || r->flagged)
            continue;
        r->editor->setStyleSheet(bad ? QString(s_flaggedStyle)
                                     : (r->check->isChecked() ? QString() : QString(s_unsetStyle)));
    }

    if (bad)
    {
        m_rangeLabel->setText(tr("Low and high must be at least %1 apart, low below high: the server refuses the pair otherwise.")
                              .arg(gap));
        m_rangeLabel->show();
    }
    else
    {
        m_rangeLabel->hide();
    }
}

QCheckBox *DeviceCommandForm::checkBox(const QString &field) const
{
    for (const Row &r : m_rows)
    {
        if (r.field.name == field)
            return r.check;
    }
    return nullptr;
}

QWidget *DeviceCommandForm::editor(const QString &field) const
{
    for (const Row &r : m_rows)
    {
        if (r.field.name == field)
            return r.editor;
    }
    return nullptr;
}
