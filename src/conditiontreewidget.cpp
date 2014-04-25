#include "conditiontreewidget.h"
#include "TwoLineItemDelegate.h"
#include "mainwindow.h"
#include <ListeRoom.h>

ConditionTreeWidget::ConditionTreeWidget(QWidget *parent):
    QTreeWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(false);

    setItemDelegate(new TwoLineItemDelegate());
}

bool ConditionTreeWidget::dropMimeData(QTreeWidgetItem *, int, const QMimeData *data, Qt::DropAction)
{
    QList<QUrl> urlList;

    urlList = data->urls(); // retrieve list of urls

    foreach(QUrl url, urlList) // iterate over list
    {
        string id = url.toString().toUtf8().data();

        IOBase *input = ListeRoom::Instance().get_input(id);
        IOBase *output = NULL;
        if (!input)
        {
            //In case id is a Audio/Cam output id
            output = ListeRoom::Instance().get_output(id);

            if (output)
            {
                if (output->get_gui_type() == "audio" ||
                    output->get_gui_type() == "camera")
                    input = ListeRoom::Instance().get_input(output->get_param("iid"));
            }

            if (!input && !output)
                return false;
        }

        MainWindow *win = dynamic_cast<MainWindow *>(QApplication::activeWindow());

        if (input)
            win->getFormRules()->addCondition(COND_STD);
        else
            win->getFormRules()->addCondition(COND_OUTPUT);
    }

    return true;
}

QStringList ConditionTreeWidget::mimeTypes () const
{
    QStringList l;

    // list of accepted mime types for drop
    l.append("text/uri-list");

    return l;
}

Qt::DropActions ConditionTreeWidget::supportedDropActions () const
{
    return Qt::CopyAction | Qt::MoveAction;
}

void ConditionTreeWidget::clear_all()
{
    for (int i = 0;i < invisibleRootItem()->childCount();i++)
    {
        delete itemDelegateForRow(i);
    }

    clear();
}
