#include "hometreewidget.h"
#include "HomeTree.h"
#include "HomeTreeMove.h"
#include <ListeRoom.h>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

using namespace Calaos;

namespace
{

QPoint dropPos(const QDropEvent *e)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return e->position().toPoint();
#else
    return e->pos();
#endif
}

}

HomeTreeWidget::HomeTreeWidget(QWidget *parent):
    QTreeWidget(parent)
{
    //Dragging is started by hand (mouseMoveEvent) and dropping is handled here:
    //QTreeWidget's own move would re-parent items without touching the model.
    setAcceptDrops(true);
    setDragEnabled(false);
}

bool HomeTreeWidget::dropIO(IOBase *io, const QPoint &pos)
{
    QTreeWidgetItemRoom *room = roomItemOf(itemAt(pos));
    if (!io || !room || !moveIOInHome(this, io, room->getRoom()))
        return false;

    emit ioMoved();
    return true;
}

void HomeTreeWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);

    if(event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();
}

void HomeTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (m_dragged)
        event->acceptProposedAction();
    else
        event->ignore();
}

void HomeTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QTreeWidgetItemRoom *room = roomItemOf(itemAt(dropPos(event)));
    const int from = m_dragged ? ListeRoom::Instance().searchIO(m_dragged) : -1;
    if (room && from >= 0 && ListeRoom::Instance().get_room(from) != room->getRoom())
        event->acceptProposedAction();
    else
        event->ignore();
}

void HomeTreeWidget::dropEvent(QDropEvent *event)
{
    if (m_dragged && dropIO(m_dragged, dropPos(event)))
        event->acceptProposedAction();
    else
        event->ignore();
}

IOBase *HomeTreeWidget::dragCandidate() const
{
    IOBase *io = ioOfItem(currentItem());
    return io && !io->getOwnerDevice() ? io : nullptr;
}

void HomeTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;
    if (currentItem() == NULL)
        return;

    if (this->itemAt(event->pos()) != currentItem())
        return;

    IOBase *io = dragCandidate();
    if (!io)
        return;

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(QString::fromUtf8(io->get_param("id").c_str()));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    m_dragged = io;
    drag->exec(Qt::MoveAction);
    m_dragged = nullptr;
}
