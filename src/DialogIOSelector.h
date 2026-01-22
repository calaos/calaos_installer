#ifndef DIALOGIOSELECTOR_H
#define DIALOGIOSELECTOR_H

#include <QDialog>
#include <QTreeWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include "ClearLineEdit.h"
#include "IOBase.h"
#include "ListeRoom.h"
#include "qtreewidget_addition.h"

using namespace Calaos;

/**
 * @brief Dialog for selecting an IO from a hierarchical tree of Rooms and IOs.
 *
 * Features:
 * - Hierarchical tree: Rooms as parents, IOs as children
 * - ClearLineEdit filter for case-insensitive search by IO or Room name
 * - Preselection if an ioId is provided
 * - Double-click or Ok button to confirm selection
 */
class DialogIOSelector : public QDialog
{
    Q_OBJECT

public:
    explicit DialogIOSelector(const QString &currentIoId = QString(), QWidget *parent = nullptr);
    ~DialogIOSelector() override;

    /**
     * @brief Get the selected IO
     * @return Pointer to the selected IOBase, or nullptr if none selected
     */
    IOBase *getSelectedIO() const;

    /**
     * @brief Get the room containing the selected IO
     * @return Pointer to the Room, or nullptr if none selected
     */
    Room *getSelectedRoom() const;

private slots:
    void onFilterTextChanged(const QString &text);
    void onItemSelectionChanged();
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    void populateTree();
    void applyFilter(const QString &filter);
    void preselectIO(const QString &ioId);
    bool itemMatchesFilter(QTreeWidgetItem *item, const QString &filter) const;

    ClearLineEdit *m_filterEdit;
    QTreeWidget *m_treeWidget;
    QDialogButtonBox *m_buttonBox;

    QString m_currentIoId;
    IOBase *m_selectedIO = nullptr;
    Room *m_selectedRoom = nullptr;
};

#endif // DIALOGIOSELECTOR_H
