#include "hometreewidget.h"
#include "mainwindow.h"
#include "formrules.h"
#include <ListeRoom.h>

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
                                IOBase *output = ListeRoom::Instance().get_output(id);
                                if (output)
                                {
                                        int _i = ListeRoom::Instance().searchIO(output);
                                        if (_i < 0) return false;
                                        Room *old_room = ListeRoom::Instance().get_room(_i);

                                        if (pitem->getRoom() == old_room) return false;

                                        //delete old item
                                        old_room->RemoveOutput(output);

                                        QTreeWidgetItemIterator it(this);
                                        while (*it)
                                        {
                                                QTreeWidgetItemOutput *item = dynamic_cast<QTreeWidgetItemOutput *>(*it);
                                                if (item && item->getOutput() == output)
                                                {
                                                        delete item;
                                                        break;
                                                }
                                                ++it;
                                        }

                                        MainWindow *win = dynamic_cast<MainWindow *>(QApplication::activeWindow());
                                        win->getFormRules()->addItemOutput(output, pitem);
                                        pitem->getRoom()->AddOutput(output);

                                        return true;
                                }

                                IOBase *input = ListeRoom::Instance().get_input(id);
                                if (input)
                                {
                                        int _i = ListeRoom::Instance().searchIO(input);
                                        if (_i < 0) return false;
                                        Room *old_room = ListeRoom::Instance().get_room(_i);

                                        if (pitem->getRoom() == old_room) return false;

                                        //delete old item
                                        old_room->RemoveInput(input);

                                        QTreeWidgetItemIterator it(this);
                                        while (*it)
                                        {
                                                QTreeWidgetItemInput *item = dynamic_cast<QTreeWidgetItemInput *>(*it);
                                                if (item && item->getInput() == input)
                                                {
                                                        delete item;
                                                        break;
                                                }

                                                /*QTreeWidgetItemOutput *oitem = dynamic_cast<QTreeWidgetItemOutput *>(*it);
                                                if (oitem && oitem->getOutput() == output)
                                                {
                                                        delete oitem;
                                                        //break;
                                                }*/
                                                ++it;
                                        }

                                        //create new item
                                        MainWindow *win = dynamic_cast<MainWindow *>(QApplication::activeWindow());
                                        win->getFormRules()->addItemInput(input, pitem);
                                        pitem->getRoom()->AddInput(input);

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
                if (itinput->getInput()->get_gui_type() == "audio" ||
                    itinput->getInput()->get_gui_type() == "camera")
                        id = itinput->getInput()->get_param("iid");

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
                if (itoutput->getOutput()->get_gui_type() == "audio" ||
                    itoutput->getOutput()->get_gui_type() == "camera")
                        id = itoutput->getOutput()->get_param("oid");

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
