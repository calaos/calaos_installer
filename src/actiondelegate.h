#ifndef ACTIONDELEGATE_H
#define ACTIONDELEGATE_H

#include <QtGui>
#include <QStyledItemDelegate>
#include <Action.h>
#include <IOBase.h>
#include <ioeditorselection.h>
#include <dialogiolist.h>

using namespace Calaos;
using namespace std;

class ActionDelegate : public QStyledItemDelegate
{
        Q_OBJECT

        public:
                ActionDelegate(Action *action, Output *output, QObject *parent = 0);

                QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

                void setEditorData(QWidget *editor, const QModelIndex &index) const;
                void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

//                void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

        private slots:
                void editMoreClick();

        private:
                Action *action;
                Output *output;

};

#endif // ACTIONDELEGATE_H
