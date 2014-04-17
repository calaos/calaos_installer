#include "TwoLineItemDelegate.h"

TwoLineItemDelegate::TwoLineItemDelegate():
    QStyledItemDelegate()
{
}

QSize TwoLineItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
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

    int textWidth = fm.width(headerText);
    int subWidth = subfm.width(subText);
    if (textWidth < subWidth) textWidth = subWidth;

    QSize sz(iconsize.width() + textWidth + 10, fm.height() + subfm.height() + 8);
    if (sz.width() < 180)
        sz.setWidth(180);

    return sz;
}

void TwoLineItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    if (!index.data(headerTextRole).isValid())
        return;

    painter->save();

    QStyleOptionViewItemV4 opt = option;
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
    {
        QColor penColor = painter->pen().color();
        penColor.setNamedColor("#A0A0A0");
        painter->setPen(penColor);
    }

    painter->drawText(QRect(rect.left() + iconsize.width() + 8, rect.top()+rect.height()/2, rect.width() - iconsize.width() - 8, rect.height()/2),
                      opt.displayAlignment, subText);

    painter->restore();
}
