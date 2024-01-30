#include <QApplication>
#include "StorageDelegate.h"
#include "UsbDisk.h"
#include "Utils.h"

StorageDelegate::StorageDelegate(QListView *parent):
    QStyledItemDelegate(parent),
    view(parent)
{
}

StorageDelegate::~StorageDelegate()
{
}

void StorageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if (!index.data(headerTextRole).isValid())
        return;

    painter->save();

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QString headerText = qvariant_cast<QString>(index.data(headerTextRole));
    QString subText = qvariant_cast<QString>(index.data(subHeaderTextRole));

    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QSize iconsize = icon.actualSize(option.decorationSize);

    // draw correct background
    opt.text = "";
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    QRect rect = opt.rect;
    QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
        cg = QPalette::Inactive;

    // set pen color
    if (opt.state & QStyle::State_Selected)
        painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
    else
        painter->setPen(opt.palette.color(cg, QPalette::Text));

    QFont font = painter->font();
    QFont subFont = painter->font();
    //font.setBold(true);
    //subFont.setWeight(subFont.weight() - 4);
    subFont.setPointSize(subFont.pointSize() - 1);
    subFont.setItalic(true);
    QFontMetrics fm(font);

    // draw 2 lines of text
    painter->setFont(font);
    painter->drawText(QRect(rect.left() + iconsize.width() + 8, rect.top(), rect.width() - iconsize.width() - 8, rect.height()/2),
                      opt.displayAlignment, headerText);
    painter->setFont(subFont);

    if (!(opt.state & QStyle::State_Selected))
        painter->setPen(QColor::fromString("#A0A0A0"));

    painter->drawText(QRect(rect.left() + iconsize.width() + 8, rect.top()+rect.height()/2, rect.width() - iconsize.width() - 8, rect.height()/2),
                      opt.displayAlignment, subText);

    painter->restore();

/*
    QRect r = option.rect;

    QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);

    if(option.state & QStyle::State_Selected)
    {
        painter->setBrush(Qt::cyan);
        painter->drawRect(r);

    }
    else
    {
        //BACKGROUND ALTERNATING COLORS
        painter->setBrush( (index.row() % 2) ? Qt::white : QColor(252,252,252) );
        painter->drawRect(r);
    }

    painter->setPen(fontPen);

    //GET TITLE, DESCRIPTION AND ICON
    UsbDiskModel *model = dynamic_cast<UsbDiskModel *>(view->model());
    UsbDisk *d = model->itemAt(index.row());

    QString icres = ":/img/ic_storage_40px.svg";
    if (d->get_isSD())
        icres = ":/img/ic_sd_storage_40px.svg";
    else if (d->get_isUsb())
        icres = ":/img/ic_usb_40px.svg";

    QIcon ic = QIcon(icres);
    QString title = QStringLiteral("%1 [%2]").arg(d->get_name(),
                                                  Utils::sizeHuman(d->get_size()));
    QString description;
    if (!d->get_volumes().isEmpty())
        description = d->get_volumes().join(", ");

    int imageSpace = 10;
    if (!ic.isNull())
    {
        //ICON
        r = option.rect.adjusted(5, 10, -10, -10);
        ic.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
        imageSpace = 55;
    }

    //TITLE
    r = option.rect.adjusted(imageSpace, 0, -10, -30);
    painter->setFont( QFont( "Lucida Grande", 6, QFont::Normal ) );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, title, &r);

    //DESCRIPTION
    r = option.rect.adjusted(imageSpace, 30, -10, 0);
    painter->setFont( QFont( "Lucida Grande", 5, QFont::Normal ) );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, description, &r);
*/
}

QSize StorageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.data(headerTextRole).isValid())
        return QStyledItemDelegate::sizeHint(option, index);

    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QSize iconsize = icon.actualSize(option.decorationSize);

    QFont font = QApplication::font();
    QFont subFont = QApplication::font();

    subFont.setPointSize(subFont.pointSize() - 1);
    subFont.setItalic(true);

    QFontMetrics fm(font), subfm(subFont);

    QString headerText = qvariant_cast<QString>(index.data(headerTextRole));
    QString subText = qvariant_cast<QString>(index.data(subHeaderTextRole));

    int textWidth = fm.horizontalAdvance(headerText);
    int subWidth = subfm.horizontalAdvance(subText);
    if (textWidth < subWidth) textWidth = subWidth;

    QSize sz(iconsize.width() + textWidth + 10, fm.height() + subfm.height() + 8);
    if (sz.width() < 180)
        sz.setWidth(180);

    return sz;
}
