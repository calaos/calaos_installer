#include "conditiontreewidget.h"
#include "mainwindow.h"
#include <ListeRoom.h>

ConditionTreeWidget::ConditionTreeWidget(QWidget *parent):
                QTreeWidget(parent)
{
        setAcceptDrops(true);
        setDragEnabled(false);
}

bool ConditionTreeWidget::dropMimeData(QTreeWidgetItem *, int, const QMimeData *data, Qt::DropAction)
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

                Input *input = ListeRoom::Instance().get_input(id);
                if (!input) return false;

                MainWindow *win = dynamic_cast<MainWindow *>(QApplication::activeWindow());

                Rule *rule = win->getFormRules()->getCurrentRule();
                if (!rule) return false;

                Condition *cond = new Condition(COND_STD);
                cond->Add(input);

                cond->get_operator().Add(id, "==");

                if (input->get_param("type") == "WIDigitalBP" || input->get_param("type") == "InputTime" ||
                    input->get_param("type") == "InputTimer" || input->get_param("type") == "Scenario" ||
                    input->get_param("type") == "scenario" || input->get_param("type") == "InPlageHoraire")
                      cond->get_params().Add(id, "true");
                else if (input->get_param("type") == "WIDigitalTriple")
                      cond->get_params().Add(id, "1");

                rule->AddCondition(cond);

                win->getFormRules()->addItemCondition(cond);
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
