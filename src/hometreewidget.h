#ifndef HOMETREEWIDGET_H
#define HOMETREEWIDGET_H

#include <QTreeWidget>
#include <QDropEvent>
#include <QMimeData>

#include <IOBase.h>

class HomeTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    HomeTreeWidget(QWidget *parent = 0);

    //What the condition and action trees read a dragged IO from: its id as a url.
    static QMimeData *ioMimeData(Calaos::IOBase *io);

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

    //The IO the current item is dragged as; rooms are not dragged. Any IO may
    //be dropped into a rule, only a free one onto a room (see dropIO).
    Calaos::IOBase *dragCandidate() const;

    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *event);
};

#endif // HOMETREEWIDGET_H
