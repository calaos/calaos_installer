#include "DialogRemoteUIEditor.h"
#include <QVBoxLayout>
#include <QQuickWidget>
#include <QDialogButtonBox>
#include <QQuickStyle>
#include <QQuickItem>

DialogRemoteUIEditor::DialogRemoteUIEditor(Calaos::IOBase *io, QWidget *parent):
    QDialog(parent),
    ioBase(io)
{
    setWindowTitle("Calaos Remote UI Editor");

    auto *rootLayout = new QVBoxLayout(this);

    QQuickStyle::setStyle("FluentWinUI3");

    auto *quickWidget = new QQuickWidget(this);
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    quickWidget->setSource(QUrl("qrc:/qml/main.qml"));

    // ButtonBox standard
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &DialogRemoteUIEditor::acceptChanges);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    rootLayout->addWidget(quickWidget);
    rootLayout->addWidget(buttonBox);

    connect(quickWidget, &QQuickWidget::statusChanged, this, [this, quickWidget](QQuickWidget::Status status)
    {
        if (status == QQuickWidget::Ready)
        {
            QQuickItem *root = quickWidget->rootObject();
            if (root) {
                int w = root->implicitWidth();
                int h = root->implicitHeight();
                quickWidget->setMinimumSize(200, 200);
                resize(w + layout()->contentsMargins().left() + layout()->contentsMargins().right(),
                       h + 50);
            }
        }
    });

}

void DialogRemoteUIEditor::acceptChanges()
{
    accept();
}
