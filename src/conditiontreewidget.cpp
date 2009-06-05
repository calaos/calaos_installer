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

                string id = info.fileName().toLocal8Bit().data();

                Input *input = ListeRoom::Instance().get_input(id);
                if (!input) return false;

                MainWindow *win = dynamic_cast<MainWindow *>(QApplication::activeWindow());

                Rule *rule = win->getCurrentRule();
                if (!rule) return false;

                if (rule->get_size_conds() <= 0)
                        rule->AddCondition(new Condition());

                rule->get_condition(0)->Add(input);
                rule->get_condition(0)->get_operator().Add(input->get_param("id"), "==");

                if (input->get_param("type") == "WIDigitalBP" || input->get_param("type") == "InputTime" ||
                    input->get_param("type") == "InputTimer" || input->get_param("type") == "Scenario" ||
                    input->get_param("type") == "scenario" || input->get_param("type") == "InPlageHoraire")
                      rule->get_condition(0)->get_params().Add(input->get_param("id"), "true");
                else if (input->get_param("type") == "WIDigitalTriple")
                      rule->get_condition(0)->get_params().Add(input->get_param("id"), "1");

                win->addItemCondition(rule->get_condition(0), input);
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
