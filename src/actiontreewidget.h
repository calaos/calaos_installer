#ifndef ACTIONTREEWIDGET_H
#define ACTIONTREEWIDGET_H

#include <QTreeWidget>
#include <QDropEvent>

class ActionTreeWidget : public QTreeWidget
{
public:
    ActionTreeWidget(QWidget *parent = 0);

    bool dropMimeData(QTreeWidgetItem *parent, int index,
                      const QMimeData *data, Qt::DropAction action);

    //                QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const;
    QStringList mimeTypes() const;
    Qt::DropActions supportedDropActions () const;

    void clear_all();
};

#endif // ACTIONTREEWIDGET_H
