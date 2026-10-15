#ifndef DEVICECOMMANDFORM_H
#define DEVICECOMMANDFORM_H

#include <QWidget>

#include "DeviceCommand.h"

class QCheckBox;
class QGridLayout;
class QLabel;

// Form of the JSON command of a device. A field is sent only when its box is
// checked; editing a field checks it.
class DeviceCommandForm : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceCommandForm(QWidget *parent = nullptr);

    void setDevice(const QJsonObject &deviceDoc, const DeviceCommand::Instance &instance);

    // Fills the form without emitting valueChanged. What the form does not
    // offer (no link, unknown key, unexpected type) is kept and sent back as is.
    void setValue(const QString &text);
    QString value() const;

    int fieldCount() const { return m_rows.size(); }
    // Boxes to tick, editors and the line listing what is kept, for the tests.
    QCheckBox *checkBox(const QString &field) const;
    QWidget *editor(const QString &field) const;
    QLabel *notice() const { return m_notice; }
    QLabel *kept() const { return m_keptLabel; }
    QLabel *rangeNotice() const { return m_rangeLabel; }

signals:
    void valueChanged(const QString &value);

private:
    struct Row
    {
        DeviceCommand::Field field;
        QCheckBox *check = nullptr;
        QWidget *editor = nullptr;
        bool flagged = false;
    };

    void buildRows();
    void clearRows();
    void rowEdited(int index);
    void setRowSet(Row &row, bool set);
    void flag(Row &row, const QString &why);
    void emitChanged();
    void updateLabels();

    static QString labelOf(const QString &field);
    static QString valueLabel(const QString &value);

    QJsonObject m_doc;
    DeviceCommand::Instance m_instance;
    DeviceCommand::Offer m_offer;
    QList<Row> m_rows;
    QJsonObject m_kept;
    bool m_loading = false;

    QGridLayout *m_grid;
    QLabel *m_notice;
    QLabel *m_keptLabel;
    QLabel *m_rangeLabel;
};

#endif // DEVICECOMMANDFORM_H
