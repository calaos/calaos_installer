#ifndef CONDITIONTREEWIDGET_H
#define CONDITIONTREEWIDGET_H

#include <QTreeWidget>
#include <QDropEvent>

class ConditionTreeWidget : public QTreeWidget
{
public:
    ConditionTreeWidget(QWidget *parent = 0);

    bool dropMimeData(QTreeWidgetItem *parent, int index,
                      const QMimeData *data, Qt::DropAction action);

    //                QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const;
    QStringList mimeTypes() const;
    Qt::DropActions supportedDropActions () const;

    void clear_all();
};

#endif // CONDITIONTREEWIDGET_H
