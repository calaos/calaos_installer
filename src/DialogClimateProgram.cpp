#include "DialogClimateProgram.h"
#include "DialogDeleteImpact.h"
#include "DialogEditTimeRange.h"
#include "WheelGuard.h"

#include <DeletionImpact.h>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <algorithm>
#include <cmath>

using namespace Calaos;
using ClimateProgram::Set;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

double number(const std::string &s, double def)
{
    bool ok = false;
    const double v = QString::fromStdString(s).trimmed().toDouble(&ok);
    return ok ? v : def;
}

//Bounds the server copied on the IO, those of an internal thermostat otherwise.
void setBounds(QDoubleSpinBox *box, IOBase *io)
{
    double min = 7, max = 30, step = 0.5;
    if (io)
    {
        min = number(io->get_param("value_min"), min);
        max = number(io->get_param("value_max"), max);
        step = number(io->get_param("step"), step);
    }
    box->setRange(std::min(min, max), std::max(min, max));
    box->setSingleStep(step > 0 ? step : 0.5);
    box->setDecimals(1);
    box->setSuffix(QStringLiteral(" °C"));
    WheelGuard::install(box);
}

bool same(double a, double b)
{
    return std::abs(a - b) < 1e-6;
}

QLabel *hint(const QString &text, QWidget *parent)
{
    QLabel *l = new QLabel(text, parent);
    l->setWordWrap(true);
    l->setForegroundRole(QPalette::PlaceholderText);
    return l;
}

}

DialogClimateProgram::DialogClimateProgram(Device *dev, bool readOnly, QWidget *parent):
    QDialog(parent),
    m_dev(dev),
    m_readOnly(readOnly)
{
    setWindowTitle(tr("Time program of \"%1\"").arg(u(dev->get_param("name"))));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumWidth(560);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(hint(tr("The server creates the IOs of new levels at its next start. "
                              "Lowering a count deletes the levels above it, with their time "
                              "ranges and the rules using them. Time ranges are saved as soon as "
                              "their own dialog is accepted, even if this one is cancelled."), this));

    m_heat.set = Set::Heat;
    m_cool.set = Set::Cool;
    QHBoxLayout *sets = new QHBoxLayout;
    sets->addWidget(buildSet(m_heat));
    sets->addWidget(buildSet(m_cool));
    layout->addLayout(sets);
    for (SetUi *ui: { &m_heat, &m_cool })
    {
        if (ClimateProgram::offers(dev, ui->set))
            continue;
        // The counter stays so levels the server ignores can still be brought down to 0.
        for (QWidget *w: ui->box->findChildren<QWidget *>())
            if (w != ui->count && !ui->count->isAncestorOf(w))
                w->setEnabled(false);
        ui->count->setMaximum(ui->initialCount);
        ui->box->setToolTip(tr("This climate does not offer this mode"));
    }

    QGroupBox *general = new QGroupBox(tr("Program"), this);
    QFormLayout *form = new QFormLayout(general);

    m_gap = new QDoubleSpinBox(general);
    m_gap->setRange(0, 10);
    m_gap->setSingleStep(0.5);
    m_gap->setDecimals(1);
    m_gap->setSuffix(QStringLiteral(" °C"));
    WheelGuard::install(m_gap);
    // What the box holds, not the raw param: an untouched value is not rewritten rounded.
    m_gap->setValue(number(ClimateProgram::param(dev, "program_min_gap", "2"), 2));
    m_initialGap = m_gap->value();
    m_gap->setToolTip(tr("Minimum gap between the low and high setpoints in auto mode"));
    form->addRow(tr("Minimum gap in auto mode"), m_gap);

    m_enabled = new QCheckBox(tr("Program enabled"), general);
    if (IOBase *io = ClimateProgram::ioForRole(dev, "program_enabled"))
        m_initialEnabled = ClimateProgram::ioBool(io, true);
    else
        m_enabled->setEnabled(false);
    m_enabled->setChecked(m_initialEnabled);
    form->addRow(QString(), m_enabled);
    layout->addWidget(general);

    QDialogButtonBox *buttons = new QDialogButtonBox(readOnly ? QDialogButtonBox::Close :
                                                                QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &DialogClimateProgram::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &DialogClimateProgram::reject);
    layout->addWidget(buttons);

    if (readOnly)
        for (QWidget *w: findChildren<QWidget *>())
            if (qobject_cast<QDoubleSpinBox *>(w) || qobject_cast<QSpinBox *>(w) ||
                qobject_cast<QCheckBox *>(w) || (qobject_cast<QPushButton *>(w) && w->parent() != buttons))
                w->setEnabled(false);
}

QSpinBox *DialogClimateProgram::levelCount(Set set) const
{
    return set == Set::Heat ? m_heat.count : m_cool.count;
}

QWidget *DialogClimateProgram::buildSet(SetUi &ui)
{
    const bool heat = ui.set == Set::Heat;
    QGroupBox *box = new QGroupBox(heat ? tr("Heating") : tr("Cooling"), this);
    ui.box = box;
    QVBoxLayout *v = new QVBoxLayout(box);
    QFormLayout *form = new QFormLayout;
    v->addLayout(form);

    ui.initialCount = ClimateProgram::levels(m_dev, ui.set);
    ui.count = new QSpinBox(box);
    ui.count->setObjectName(heat ? "heatLevels" : "coolLevels");
    ui.count->setRange(0, ClimateProgram::kMaxLevels);
    ui.count->setValue(ui.initialCount);
    WheelGuard::install(ui.count);
    form->addRow(tr("Levels"), ui.count);

    //Once the server created the IO, its value is what counts: the param is only
    //read when the IO is created.
    IOBase *defIo = ClimateProgram::ioForRole(m_dev, ClimateProgram::defaultRole(ui.set));
    const double defParam = number(ClimateProgram::param(m_dev, heat ? "heat_default_setpoint" : "cool_default_setpoint",
                                                         heat ? "17" : "26"), heat ? 17 : 26);
    ui.def = new QDoubleSpinBox(box);
    setBounds(ui.def, defIo);
    ui.def->setValue(defIo ? ClimateProgram::ioValue(defIo, defParam) : defParam);
    ui.initialDef = ui.def->value();
    ui.def->setToolTip(tr("Setpoint when no level is active"));
    form->addRow(tr("Default setpoint"), ui.def);

    if (!heat)
    {
        IOBase *offIo = ClimateProgram::ioForRole(m_dev, "cool_default_off");
        const bool offParam = ClimateProgram::param(m_dev, "cool_default_off", "false") == "true";
        ui.initialOff = offIo ? ClimateProgram::ioBool(offIo, offParam) : offParam;
        ui.off = new QCheckBox(tr("Cooling off when no level is active"), box);
        ui.off->setChecked(ui.initialOff);
        connect(ui.off, &QCheckBox::toggled, ui.def, [def = ui.def](bool off) { def->setEnabled(!off); });
        ui.def->setEnabled(!ui.initialOff);
        form->addRow(QString(), ui.off);
    }

    QGridLayout *grid = new QGridLayout;
    v->addLayout(grid);
    for (int n = 1;n <= ClimateProgram::kMaxLevels;n++)
    {
        LevelRow &row = ui.rows[n - 1];
        IOBase *sp = ClimateProgram::ioForRole(m_dev, ClimateProgram::setpointRole(ui.set, n));
        IOBase *range = ClimateProgram::ioForRole(m_dev, ClimateProgram::levelRole(ui.set, n));

        row.label = new QLabel(tr("Level %1").arg(n), box);
        row.setpoint = new QDoubleSpinBox(box);
        setBounds(row.setpoint, sp);
        row.setpoint->setValue(sp ? ClimateProgram::ioValue(sp, ui.initialDef) : ui.initialDef);
        ui.initialSetpoints[n - 1] = row.setpoint->value();
        row.ranges = new QPushButton(QIcon(":/img/icon_clock.png"), tr("Time ranges..."), box);
        if (range)
            connect(row.ranges, &QPushButton::clicked, this, [this, range]() { editRanges(range); });
        row.note = hint(QString(), box);

        grid->addWidget(row.label, n - 1, 0);
        grid->addWidget(row.setpoint, n - 1, 1);
        grid->addWidget(row.ranges, n - 1, 2);
        grid->addWidget(row.note, n - 1, 1, 1, 2);
    }
    v->addStretch();

    connect(ui.count, &QSpinBox::valueChanged, this, [this, &ui]() { refreshRows(ui); });
    refreshRows(ui);
    return box;
}

//A level shows its setpoint and ranges once the server created them, a note otherwise.
void DialogClimateProgram::refreshRows(SetUi &ui)
{
    const int count = ui.count->value();
    for (int n = 1;n <= ClimateProgram::kMaxLevels;n++)
    {
        LevelRow &row = ui.rows[n - 1];
        const bool exists = ClimateProgram::ioForRole(m_dev, ClimateProgram::setpointRole(ui.set, n)) ||
                            ClimateProgram::ioForRole(m_dev, ClimateProgram::levelRole(ui.set, n));
        const bool editable = exists && n <= count;
        const bool shown = n <= std::max(count, ui.initialCount);

        row.label->setVisible(shown);
        row.setpoint->setVisible(shown && editable);
        row.ranges->setVisible(shown && editable);
        row.setpoint->setEnabled(!m_readOnly && ClimateProgram::ioForRole(m_dev, ClimateProgram::setpointRole(ui.set, n)));
        row.ranges->setEnabled(!m_readOnly && ClimateProgram::ioForRole(m_dev, ClimateProgram::levelRole(ui.set, n)));

        if (n > count && exists)
            row.note->setText(tr("Deleted with its time ranges"));
        else if (!exists)
            row.note->setText(tr("Created by the server at its next start"));
        row.note->setVisible(shown && !editable);
    }
}

void DialogClimateProgram::editRanges(IOBase *range)
{
    DialogEditTimeRange d(range);
    if (d.exec() == QDialog::Accepted)
    {
        d.save();
        m_changed = true;
    }
}

bool DialogClimateProgram::writeValues()
{
    bool changed = false;
    auto writeNumber = [&](IOBase *io, double initial, double value) {
        if (io && !same(initial, value))
        {
            ClimateProgram::setIoValue(io, value);
            changed = true;
        }
    };

    for (SetUi *ui: { &m_heat, &m_cool })
    {
        const bool heat = ui->set == Set::Heat;
        const double def = ui->def->value();
        if (!same(def, ui->initialDef))
        {
            //The param too: it seeds the IO if the server ever creates it again.
            ClimateProgram::setParam(m_dev, heat ? "heat_default_setpoint" : "cool_default_setpoint",
                                     QString::number(def).toStdString());
            writeNumber(ClimateProgram::ioForRole(m_dev, ClimateProgram::defaultRole(ui->set)), ui->initialDef, def);
            changed = true;
        }

        if (ui->off && ui->off->isChecked() != ui->initialOff)
        {
            ClimateProgram::setParam(m_dev, "cool_default_off", ui->off->isChecked() ? "true" : "false");
            if (IOBase *io = ClimateProgram::ioForRole(m_dev, "cool_default_off"))
                ClimateProgram::setIoBool(io, ui->off->isChecked());
            changed = true;
        }

        for (int n = 1;n <= std::min(ui->count->value(), ClimateProgram::kMaxLevels);n++)
            writeNumber(ClimateProgram::ioForRole(m_dev, ClimateProgram::setpointRole(ui->set, n)),
                        ui->initialSetpoints[n - 1], ui->rows[n - 1].setpoint->value());
    }

    if (!same(m_gap->value(), m_initialGap))
    {
        ClimateProgram::setParam(m_dev, "program_min_gap", QString::number(m_gap->value()).toStdString());
        changed = true;
    }

    IOBase *enabled = ClimateProgram::ioForRole(m_dev, "program_enabled");
    if (enabled && m_enabled->isChecked() != m_initialEnabled)
    {
        ClimateProgram::setIoBool(enabled, m_enabled->isChecked());
        changed = true;
    }
    return changed;
}

void DialogClimateProgram::accept()
{
    if (m_readOnly)
    {
        QDialog::accept();
        return;
    }

    const int heat = m_heat.count->value();
    const int cool = m_cool.count->value();
    const std::vector<IOBase *> gone = ClimateProgram::iosRemovedBy(m_dev, heat, cool);
    if (!gone.empty())
    {
        DeletionImpact impact = deletionImpact(gone);
        //The climate drops these links itself.
        impact.brokenLinks.erase(std::remove_if(impact.brokenLinks.begin(), impact.brokenLinks.end(),
                                                [this](const BrokenLink &l) { return l.device == m_dev; }),
                                 impact.brokenLinks.end());
        DialogDeleteImpact confirm(tr("%n program IO(s) of \"%1\"", nullptr, int(gone.size()))
                                       .arg(u(m_dev->get_param("name"))),
                                   impact, DialogDeleteImpact::ListNone, this);
        if (confirm.exec() != QDialog::Accepted)
            return;
    }

    if (writeValues())
        m_changed = true;

    if (heat != m_heat.initialCount || cool != m_cool.initialCount)
    {
        m_removed = ClimateProgram::applyLevels(m_dev, heat, cool);
        m_changed = true;
    }

    QDialog::accept();
}
