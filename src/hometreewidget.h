#ifndef HOMETREEWIDGET_H
#define HOMETREEWIDGET_H

#include <QTreeWidget>
#include <QDropEvent>
#include <QtWidgets>

class HomeTreeWidget : public QTreeWidget
{
public:
    HomeTreeWidget(QWidget *parent = 0);

    bool dropMimeData(QTreeWidgetItem *parent, int index,
                      const QMimeData *data, Qt::DropAction action);

    //                QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const;
    QStringList mimeTypes() const;
    Qt::DropActions supportedDropActions () const;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    QPoint dragStartPosition;

    void dragEnterEvent(QDragEnterEvent *e);
    //void dragMoveEvent(QDragMoveEvent *e);
    //void dropEvent(QDropEvent *event);

};

#endif // HOMETREEWIDGET_H
