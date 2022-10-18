#include "actiontreewidget.h"
#include "mainwindow.h"
#include "TwoLineItemDelegate.h"
#include <ListeRoom.h>
#include <QApplication>

ActionTreeWidget::ActionTreeWidget(QWidget *parent):
    QTreeWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(false);

    setItemDelegate(new TwoLineItemDelegate());
}

bool ActionTreeWidget::dropMimeData(QTreeWidgetItem *, int, const QMimeData *data, Qt::DropAction)
{
    QList<QUrl> urlList;

    urlList = data->urls(); // retrieve list of urls

    foreach(QUrl url, urlList) // iterate over list
    {
        string id = url.toString().toUtf8().data();

        IOBase *output = ListeRoom::Instance().get_output(id);
        if (!output) return false;

        MainWindow *win = dynamic_cast<MainWindow *>(QApplication::activeWindow());

        win->getFormRules()->addAction(ACTION_STD);
    }

    return true;
}

QStringList ActionTreeWidget::mimeTypes () const
{
    QStringList l;

    // list of accepted mime types for drop
    l.append("text/uri-list");

    return l;
}

Qt::DropActions ActionTreeWidget::supportedDropActions () const
{
    return Qt::CopyAction | Qt::MoveAction;
}

void ActionTreeWidget::clear_all()
{
    for (int i = 0;i < invisibleRootItem()->childCount();i++)
    {
        delete itemDelegateForRow(i);
    }

    clear();
}
