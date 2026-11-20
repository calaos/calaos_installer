#ifndef DIALOGCLIMATEPROGRAM_H
#define DIALOGCLIMATEPROGRAM_H

#include <QDialog>
#include <vector>

#include <ClimateProgram.h>

class QCheckBox;
class QDoubleSpinBox;
class QGridLayout;
class QLabel;
class QPushButton;

//Offline editor of a climate's time program: whether each set has one, default
//setpoints, level setpoints and, through DialogEditTimeRange, the level time ranges.
class DialogClimateProgram: public QDialog
{
    Q_OBJECT

public:
    DialogClimateProgram(Calaos::Device *dev, bool readOnly, QWidget *parent = nullptr);

    //The model changed, accepted or not: a time range is saved as soon as its
    //own dialog is accepted.
    bool changed() const { return m_changed; }
    //IOs deleted by a lower level count; only good for pointer comparison.
    const std::vector<Calaos::IOBase *> &removedIos() const { return m_removed; }

    //Checked: the set has a program, with the maximum number of levels.
    QCheckBox *programBox(Calaos::ClimateProgram::Set set) const;

public slots:
    void accept() override;

private:
    struct LevelRow
    {
        QLabel *label = nullptr;
        QDoubleSpinBox *setpoint = nullptr;
        QPushButton *ranges = nullptr;
        QLabel *note = nullptr;
    };

    struct SetUi
    {
        Calaos::ClimateProgram::Set set;
        QWidget *box = nullptr;
        int initialCount = 0;
        QCheckBox *program = nullptr;
        QDoubleSpinBox *def = nullptr;
        double initialDef = 0;
        QCheckBox *off = nullptr;
        bool initialOff = false;
        LevelRow rows[Calaos::ClimateProgram::kMaxLevels];
        double initialSetpoints[Calaos::ClimateProgram::kMaxLevels] = {};
    };

    QWidget *buildSet(SetUi &ui);
    void refreshRows(SetUi &ui);
    //Left as read unless the box was toggled: no silent migration of a 1-9 count.
    int targetCount(const SetUi &ui) const;
    void editRanges(Calaos::IOBase *range);
    bool writeValues();

    Calaos::Device *m_dev;
    bool m_readOnly;
    bool m_changed = false;
    std::vector<Calaos::IOBase *> m_removed;

    SetUi m_heat;
    SetUi m_cool;
    QDoubleSpinBox *m_gap = nullptr;
    double m_initialGap = 0;
    QCheckBox *m_enabled = nullptr;
    bool m_initialEnabled = true;
};

#endif // DIALOGCLIMATEPROGRAM_H
