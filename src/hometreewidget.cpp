#include "hometreewidget.h"
#include "mainwindow.h"
#include "formrules.h"
#include "HomeTreeMove.h"
#include <ListeRoom.h>
#include <QApplication>
#include <QDrag>

HomeTreeWidget::HomeTreeWidget(QWidget *parent):
    QTreeWidget(parent)
{
    setAcceptDrops(false);
    setDragEnabled(false);
}

bool HomeTreeWidget::dropMimeData(QTreeWidgetItem *parent, int, const QMimeData *data, Qt::DropAction)
{
    QList<QUrl> urlList;
    QFileInfo info;
    QString fName;

    urlList = data->urls(); // retrieve list of urls

    foreach(QUrl url, urlList) // iterate over list
    {
        fName = url.toLocalFile();
        info.setFile( fName );

        string id = info.fileName().toUtf8().data();

        if (parent != NULL)
        {
            QTreeWidgetItemRoom *pitem = dynamic_cast<QTreeWidgetItemRoom *>(parent);

            if (!pitem) //search the parent room if we drop on IOs
            {
                QTreeWidgetItemInput *oitem = dynamic_cast<QTreeWidgetItemInput *>(parent);
                if (oitem)
                {
                    pitem = dynamic_cast<QTreeWidgetItemRoom *>(oitem->parent());
                }

                QTreeWidgetItemOutput *iitem = dynamic_cast<QTreeWidgetItemOutput *>(parent);
                if (iitem && !pitem)
                {
                    pitem = dynamic_cast<QTreeWidgetItemRoom *>(iitem->parent());
                }
            }

            if (pitem)
            {
                IOBase *io = ListeRoom::Instance().get_output(id);
                if (!io)
                    io = ListeRoom::Instance().get_input(id);

                if (io)
                {
                    if (!moveIOInHome(this, io, pitem->getRoom()))
                        return false;

                    MainWindow *win = dynamic_cast<MainWindow *>(QApplication::activeWindow());
                    win->getFormRules()->setProjectModified(true);

                    return true;
                }
            }
        }
    }

    return false;
}

QStringList HomeTreeWidget::mimeTypes () const
{
    QStringList l;

    // list of accepted mime types for drop
    l.append("text/uri-list");

    return l;
}

Qt::DropActions HomeTreeWidget::supportedDropActions () const
{
    return Qt::CopyAction | Qt::MoveAction;
}

void HomeTreeWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);

    if(event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();
}

void HomeTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void HomeTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;
    if (currentItem() == NULL)
        return;

    if (this->itemAt(QPoint(event->x(), event->y())) != currentItem())
        return;

    //check if current item is an Input/Output and not a room
    QTreeWidgetItemRoom *item = dynamic_cast<QTreeWidgetItemRoom *>(currentItem());
    if (item)
        return;

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(currentItem());
    if (itinput)
    {
        string id = itinput->getInput()->get_param("id");
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        QList<QUrl> list;
        QString qs = QString::fromUtf8(id.c_str());
        qDebug() << "drag item: " << id.c_str();
        list.append(QUrl(qs));

        // mime stuff
        mimeData->setUrls(list);
        drag->setMimeData(mimeData);

        // start drag
        drag->exec(Qt::CopyAction | Qt::MoveAction);

        return;
    }

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(currentItem());
    if (itoutput)
    {
        string id = itoutput->getOutput()->get_param("id");
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        QList<QUrl> list;
        QString qs = QString::fromUtf8(id.c_str());
        list.append(QUrl(qs));

        // mime stuff
        mimeData->setUrls(list);
        drag->setMimeData(mimeData);

        // start drag
        drag->exec(Qt::CopyAction | Qt::MoveAction);

        return;
    }
}
