#include "DialogRemoteUIEditor.h"
#include "DialogIOSelector.h"
#include "WidgetModel.h"
#include "ListeRoom.h"
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

    // Expose available widget types
    m_quickWidget->rootContext()->setContextProperty("availableWidgetTypes", WidgetModel::availableWidgetTypes());

    // Expose the editor itself for IO selection dialog
    m_quickWidget->rootContext()->setContextProperty("remoteUIEditor", this);

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

void DialogRemoteUIEditor::openIOSelector(const QString &currentIoId)
{
    DialogIOSelector dialog(currentIoId, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        Calaos::IOBase *io = dialog.getSelectedIO();
        Calaos::Room *room = dialog.getSelectedRoom();

        if (io && room)
        {
            QString ioId = QString::fromStdString(io->get_param("id"));
            QString ioName = QString::fromStdString(io->get_param("name"));
            QString roomName = QString::fromStdString(room->get_name());
            QString guiType = QString::fromStdString(io->get_gui_type());
            QString widgetType = WidgetModel::widgetTypeForGuiType(guiType);

            emit ioSelected(ioId, ioName, roomName, widgetType);
        }
    }
}

QVariantMap DialogRemoteUIEditor::validateIO(const QString &ioId)
{
    QVariantMap result;
    result["exists"] = false;
    result["ioId"] = ioId;
    result["ioName"] = QString();
    result["roomName"] = QString();
    result["guiType"] = QString();
    result["widgetType"] = QString();

    if (ioId.isEmpty())
        return result;

    std::string idStr = ioId.toStdString();

    // Search in inputs
    Calaos::IOBase *io = Calaos::ListeRoom::Instance().get_input(idStr);
    if (!io)
    {
        // Search in outputs
        io = Calaos::ListeRoom::Instance().get_output(idStr);
    }

    if (io)
    {
        // Find the room containing this IO
        int roomIndex = Calaos::ListeRoom::Instance().searchIO(io);
        if (roomIndex >= 0)
        {
            Calaos::Room *room = Calaos::ListeRoom::Instance().get_room(roomIndex);

            result["exists"] = true;
            result["ioName"] = QString::fromStdString(io->get_param("name"));
            result["roomName"] = QString::fromStdString(room->get_name());
            result["guiType"] = QString::fromStdString(io->get_gui_type());
            result["widgetType"] = WidgetModel::widgetTypeForGuiType(result["guiType"].toString());
        }
    }

    return result;
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
