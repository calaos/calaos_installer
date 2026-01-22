#include "DialogRemoteUIEditor.h"
#include <QVBoxLayout>
#include <QQuickWidget>
#include <QDialogButtonBox>
#include <QQuickStyle>
#include <QQuickItem>
#include <QQmlContext>
#include <QQmlEngine>

DialogRemoteUIEditor::DialogRemoteUIEditor(Calaos::IOBase *io, QWidget *parent):
    QDialog(parent),
    ioBase(io),
    m_model(new RemoteUIModel(this)),
    m_quickWidget(nullptr)
{
    setWindowTitle("Calaos Remote UI Editor");
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    auto *rootLayout = new QVBoxLayout(this);

    QQuickStyle::setStyle("FluentWinUI3");

    m_quickWidget = new QQuickWidget(this);
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // Register types for QML
    qmlRegisterType<RemoteUIModel>("Calaos.RemoteUI", 1, 0, "RemoteUIModel");
    qmlRegisterType<PageModel>("Calaos.RemoteUI", 1, 0, "PageModel");
    qmlRegisterType<WidgetModel>("Calaos.RemoteUI", 1, 0, "WidgetModel");

    // Load grid dimensions from IOBase params
    int gridW = QString::fromStdString(ioBase->get_param("grid_w")).toInt();
    int gridH = QString::fromStdString(ioBase->get_param("grid_h")).toInt();
    if (gridW <= 0) gridW = 3;
    if (gridH <= 0) gridH = 3;
    m_model->setGridDimensions(gridW, gridH);

    // Load pages from IOBase XML content
    QString pagesXml = ioBase->getRemoteUIPagesXml();
    m_model->loadFromXml(pagesXml);
    m_model->resetModified();

    // Expose the model to QML
    m_quickWidget->rootContext()->setContextProperty("remoteUIModel", m_model);

    // Also expose the IOBase name for the title
    QString ioName = QString::fromStdString(ioBase->get_param("name"));
    m_quickWidget->rootContext()->setContextProperty("remoteUIName", ioName);

    m_quickWidget->setSource(QUrl("qrc:/qml/main.qml"));

    // ButtonBox standard
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &DialogRemoteUIEditor::acceptChanges);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    rootLayout->addWidget(m_quickWidget);
    rootLayout->addWidget(buttonBox);

    connect(m_quickWidget, &QQuickWidget::statusChanged, this, [this](QQuickWidget::Status status)
    {
        if (status == QQuickWidget::Ready)
        {
            QQuickItem *root = m_quickWidget->rootObject();
            if (root) {
                int w = root->implicitWidth();
                int h = root->implicitHeight();
                m_quickWidget->setMinimumSize(200, 200);
                resize(w + layout()->contentsMargins().left() + layout()->contentsMargins().right(),
                       h + 50);
            }
        }
    });
}

DialogRemoteUIEditor::~DialogRemoteUIEditor()
{
}

void DialogRemoteUIEditor::acceptChanges()
{
    // Save the model back to IOBase
    QString pagesXml = m_model->toXml();
    ioBase->setRemoteUIPagesXml(pagesXml);

    // Update grid dimensions in IOBase params
    ioBase->set_param("grid_w", std::to_string(m_model->gridW()));
    ioBase->set_param("grid_h", std::to_string(m_model->gridH()));

    accept();
}
