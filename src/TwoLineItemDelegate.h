#ifndef CONDITIONITEMDELEGATE_H
#define CONDITIONITEMDELEGATE_H

#include <QtWidgets>
#include <QStyledItemDelegate>

class TwoLineItemDelegate : public QStyledItemDelegate
{
        public:
                TwoLineItemDelegate();

                enum datarole { headerTextRole = Qt::UserRole + 100, subHeaderTextRole = Qt::UserRole + 101};

                void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

                QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};

#endif // CONDITIONITEMDELEGATE_H
