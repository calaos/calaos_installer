#ifndef DIALOGREMOTEUIEDITOR_H
#define DIALOGREMOTEUIEDITOR_H

#include <QDialog>
#include <QObject>
#include <QQuickWidget>
#include <QVariantMap>
#include "IOBase.h"
#include "RemoteUIModel.h"

class DialogRemoteUIEditor : public QDialog
{
    Q_OBJECT
public:
    DialogRemoteUIEditor(Calaos::IOBase *ioBase, QWidget *parent = nullptr);
    ~DialogRemoteUIEditor() override;

    /**
     * @brief Open the IO selector dialog and return the selected IO info
     * @param currentIoId The currently selected IO ID (for preselection)
     */
    Q_INVOKABLE void openIOSelector(const QString &currentIoId);

    /**
     * @brief Validate if an IO exists and return its info
     * @param ioId The IO ID to validate
     * @return Map with {exists, ioId, ioName, roomName, guiType, widgetType}
     */
    Q_INVOKABLE QVariantMap validateIO(const QString &ioId);

signals:
    /**
     * @brief Emitted when an IO is selected in the dialog
     * @param ioId The selected IO ID
     * @param ioName The selected IO name
     * @param roomName The room name containing the IO
     * @param widgetType The auto-detected widget type based on gui_type
     */
    void ioSelected(const QString &ioId, const QString &ioName, const QString &roomName, const QString &widgetType);

private slots:
    void acceptChanges();

private:
    Calaos::IOBase *ioBase;
    RemoteUIModel *m_model;
    QQuickWidget *m_quickWidget;
};

#endif // DIALOGREMOTEUIEDITOR_H
