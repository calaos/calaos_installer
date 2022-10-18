#include "ClearLineEdit.h"
#include <QStyle>

ClearLineEdit::ClearLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    clearButton = new QToolButton(this);

    QPixmap pixmap(":/img/clear_left.png");
    clearButton->setIcon(QIcon(pixmap));
    clearButton->setIconSize(pixmap.size());
    clearButton->setCursor(Qt::ArrowCursor);
    clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    clearButton->setToolTip(tr("Clear text"));
    clearButton->hide();

    label = new QLabel(tr("Filter..."), this);
    label->resize(size());
    label->setStyleSheet("QLabel { font-style: italic; color: #aaa; margin-left: 5px; }");

    connect(clearButton, &QToolButton::clicked, this, &ClearLineEdit::clear);
    connect(this, &ClearLineEdit::textChanged, this, &ClearLineEdit::updateCloseButton);

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

    setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(clearButton->sizeHint().width() + frameWidth + 1));

    QSize msz = minimumSizeHint();
    setMinimumSize(qMax(msz.width(), clearButton->sizeHint().height() + frameWidth * 2 + 2),
                   qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
}

void ClearLineEdit::resizeEvent(QResizeEvent *)
{
    QSize sz = clearButton->sizeHint();

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

    clearButton->move(rect().right() - frameWidth - sz.width(),
                      (rect().bottom() + 2 - sz.height()) / 2);
}

void ClearLineEdit::updateCloseButton(const QString& text)
{
    clearButton->setVisible(!text.isEmpty());
    label->setVisible(text.isEmpty());
}
