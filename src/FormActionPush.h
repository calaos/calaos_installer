#ifndef FORMACTIONPUSH_H
#define FORMACTIONPUSH_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <ListeRule.h>
#include <ListeRoom.h>

using namespace Calaos;

namespace Ui {
class FormActionPush;
}

class FormActionPush : public QWidget
{
    Q_OBJECT

public:
    explicit FormActionPush(QWidget *parent = 0);
    ~FormActionPush();

    void setAction(QTreeWidgetItem *item, Rule *rule, Action *action);

private:
    Ui::FormActionPush *ui;

    Rule *rule = nullptr;
    Action *action = nullptr;
    QTreeWidgetItem *qitem = nullptr;

    bool setDone;

private slots:
    void on_btEditMsg_clicked();
    void on_comboCamera_currentIndexChanged(int index);
};

#endif // FORMACTIONPUSH_H
