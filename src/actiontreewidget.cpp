#include "actiontreewidget.h"
#include "mainwindow.h"
#include "TwoLineItemDelegate.h"
#include <ListeRoom.h>

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
        QFileInfo info;
        QString fName;

        urlList = data->urls(); // retrieve list of urls

        foreach(QUrl url, urlList) // iterate over list
        {
                fName = url.toLocalFile();
                info.setFile( fName );

                string id = info.fileName().toUtf8().data();

                Output *output = ListeRoom::Instance().get_output(id);
                if (!output) return false;

                MainWindow *win = dynamic_cast<MainWindow *>(QApplication::activeWindow());

                Rule *rule = win->getFormRules()->getCurrentRule();
                if (!rule) return false;

                Action *action = new Action(ACTION_STD);

                action->Add(output);

                if (output->get_param("type") == "WODigital" || output->get_param("type") == "WODali" ||
                    output->get_param("type") == "WODaliRVB" || output->get_param("type") == "WONeon" ||
                    output->get_param("type") == "WOVolet" || output->get_param("type") == "WOVoletSmart")
                      action->get_params().Add(id, "toggle");
                else if (output->get_param("type") == "scenario" || output->get_param("type") == "InputTimer")
                      action->get_params().Add(id, "true");

                rule->AddAction(action);

                win->getFormRules()->addItemAction(action);
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
