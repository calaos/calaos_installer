#ifndef DIALOGIOPROPERTIES_H
#define DIALOGIOPROPERTIES_H

#include <QtWidgets>
#include <QDialog>
#include <QInputDialog>
#include <QTreeWidgetItem>
#include <QMessageBox>

#include <Params.h>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui
{
class DialogIOProperties;
}

enum { OBJ_NONE = 0, OBJ_INPUT, OBJ_OUTPUT, OBJ_ROOM, OBJ_RULE };

class DialogIOProperties : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogIOProperties)
public:
    explicit DialogIOProperties(const Params &p, int type, QWidget *parent = 0);
    virtual ~DialogIOProperties();

    Params &getParams() { return params; }

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogIOProperties *ui;

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

#endif // DIALOGIOPROPERTIES_H
