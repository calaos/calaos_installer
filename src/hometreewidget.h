#ifndef HOMETREEWIDGET_H
#define HOMETREEWIDGET_H

#include <QTreeWidget>
#include <QDropEvent>

#include <IOBase.h>

class HomeTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    HomeTreeWidget(QWidget *parent = 0);

    //Moves io to the room whose item (or whose descendant's) is at pos.
    //False when there is none, or when the move is refused.
    bool dropIO(Calaos::IOBase *io, const QPoint &pos);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void ioMoved();

protected:
    QPoint dragStartPosition;
    //Only a drag started here is accepted: a file dropped from outside is not an IO.
    Calaos::IOBase *m_dragged = nullptr;

    //The IO the current item may be dragged as: rooms are not dragged, and an
    //owned IO moves with its device.
    Calaos::IOBase *dragCandidate() const;

    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *event);
};

#endif // HOMETREEWIDGET_H
