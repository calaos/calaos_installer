#ifndef DIALOGLISTPROPERTIES_H
#define DIALOGLISTPROPERTIES_H

#include <QDialog>
#include <QInputDialog>
#include <QTreeWidgetItem>
#include <QMessageBox>

#include <Params.h>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
class DialogListProperties;
}

class DialogListProperties : public QDialog
{
    Q_OBJECT

public:
    explicit DialogListProperties(const Params &p, int type, QWidget *parent = 0);
    ~DialogListProperties();

    enum { OBJ_NONE = 0, OBJ_INPUT, OBJ_OUTPUT, OBJ_ROOM, OBJ_RULE };

    Params &getParams() { return params; }

private:
    Ui::DialogListProperties *ui;

    Params params;
    int type;
    QTreeWidgetItem *current_item;
    bool modified;

private slots:
    void on_treeProperties_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_buttonBox_accepted();
    void on_modifyButton_clicked();
    void on_treeProperties_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void on_delButton_clicked();
    void on_addButton_clicked();
};

#endif // DIALOGLISTPROPERTIES_H
