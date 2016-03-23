#ifndef DIALOGDALIMASTERITEM_H
#define DIALOGDALIMASTERITEM_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>

namespace Ui {
class DialogDaliMasterItem;
}

class DialogDaliMasterItem : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDaliMasterItem(QTreeWidget *w, QTreeWidgetItem *item = nullptr, QWidget *parent = nullptr);
    ~DialogDaliMasterItem();

    enum
    {
        RoleType = Qt::UserRole + 1,
        RoleAddr1,
        RoleAddr2,
        RoleAddr3,
        RoleAddr4,
    };

    static void updateItem(QTreeWidgetItem *item, QString type, int addr1, int addr2 = 0, int addr3 = 0, int addr4 = 0);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogDaliMasterItem *ui;

    QTreeWidgetItem *editItem;
    QTreeWidget *treeWidget;
};

#endif // DIALOGDALIMASTERITEM_H
