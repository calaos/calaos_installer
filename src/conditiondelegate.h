#ifndef CONDITIONDELEGATE_H
#define CONDITIONDELEGATE_H

#include <QtGui>
#include <QStyledItemDelegate>
#include <Condition.h>
#include <IOBase.h>
#include <ioeditorselection.h>
#include <dialogiolist.h>

using namespace Calaos;
using namespace std;

class ConditionDelegate : public QStyledItemDelegate
{
        Q_OBJECT

        public:
                ConditionDelegate(Condition *condition, Input *input, QObject *parent = 0);

                QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

                void setEditorData(QWidget *editor, const QModelIndex &index) const;
                void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

//                void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

        private slots:
                void editMoreClick(QWidget *editor);

        private:
                Condition *condition;
                Input *input;

};

#endif // CONDITIONDELEGATE_H
