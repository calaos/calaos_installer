#ifndef STORAGEDELEGATE_H
#define STORAGEDELEGATE_H

#include <QListView>
#include <QStyledItemDelegate>
#include <QPainter>

class StorageDelegate: public QStyledItemDelegate
{
public:
    StorageDelegate(QListView *parent = 0);
    virtual ~StorageDelegate();

    enum datarole
    {
        headerTextRole = Qt::UserRole + 100,
        subHeaderTextRole = Qt::UserRole + 101,
    };

    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const override;

    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

private:
    QListView *view;
};

#endif // STORAGEDELEGATE_H
