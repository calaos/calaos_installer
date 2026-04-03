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
#include <QMessageBox>

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

    // Load screen saver settings from IOBase params
    QString ssTimeout = QString::fromStdString(ioBase->get_param("screensaver_timeout"));
    if (!ssTimeout.isEmpty())
        m_model->setScreensaverTimeout(ssTimeout.toInt());
    QString ssDimming = QString::fromStdString(ioBase->get_param("screensaver_dimming"));
    if (!ssDimming.isEmpty())
        m_model->setScreensaverDimming(ssDimming.toInt());
    QString ssMode = QString::fromStdString(ioBase->get_param("screensaver_mode"));
    if (!ssMode.isEmpty())
        m_model->setScreensaverMode(ssMode);
    QString ssClockTzIana = QString::fromStdString(ioBase->get_param("screensaver_clock_timezone_iana"));
    if (!ssClockTzIana.isEmpty())
    {
        m_model->setScreensaverClockTimezone(ssClockTzIana);
    }
    else
    {
        QString ssClockTz = QString::fromStdString(ioBase->get_param("screensaver_clock_timezone"));
        if (!ssClockTz.isEmpty())
            m_model->setScreensaverClockTimezone(
                TimezoneHelper::posixTzToIana(TimezoneHelper::convertStoredTimezone(ssClockTz)));
    }
    QString ssClockFmt = QString::fromStdString(ioBase->get_param("screensaver_clock_format"));
    if (!ssClockFmt.isEmpty())
        m_model->setScreensaverClockFormat(ssClockFmt);
    QString ssClockShowDate = QString::fromStdString(ioBase->get_param("screensaver_clock_show_date"));
    if (!ssClockShowDate.isEmpty())
        m_model->setScreensaverClockShowDate(ssClockShowDate == "true");
    QString ssClockDateFmt = QString::fromStdString(ioBase->get_param("screensaver_clock_date_format"));
    if (!ssClockDateFmt.isEmpty())
        m_model->setScreensaverClockDateFormat(ssClockDateFmt);
    QString ssClockSeconds = QString::fromStdString(ioBase->get_param("screensaver_clock_seconds"));
    if (!ssClockSeconds.isEmpty())
        m_model->setScreensaverClockSeconds(ssClockSeconds == "true");

    // Load pages from IOBase XML content
    QString pagesXml = ioBase->getRemoteUIPagesXml();
    m_model->loadFromXml(pagesXml);
    m_model->resetModified();

    // Expose the model to QML
    m_quickWidget->rootContext()->setContextProperty("remoteUIModel", m_model);

    // Expose available widget types
    m_quickWidget->rootContext()->setContextProperty("availableWidgetTypes", WidgetModel::availableWidgetTypes());

    // Expose category system and Clock utilities to QML
    m_quickWidget->rootContext()->setContextProperty("availableCategories", WidgetModel::availableCategories());
    m_quickWidget->rootContext()->setContextProperty("availableTimezones", WidgetModel::availableTimezones());
    m_quickWidget->rootContext()->setContextProperty("availableDateFormats", WidgetModel::availableDateFormats());

    // Expose the editor itself for IO selection dialog
    m_quickWidget->rootContext()->setContextProperty("remoteUIEditor", this);

    // Initialize the title in the model
    QString ioName = QString::fromStdString(ioBase->get_param("name"));
    m_model->setTitle(ioName);

    // Connect BEFORE setSource: statusChanged can be emitted synchronously during setSource
    auto showQmlErrors = [this]()
    {
        QStringList errors;
        for (const QQmlError &err : m_quickWidget->errors())
            errors << err.toString();
        QMessageBox::critical(this, tr("QML Load Error"),
            tr("Failed to load QML:\n\n%1").arg(errors.join("\n")));
    };

    connect(m_quickWidget, &QQuickWidget::statusChanged, this, [this, showQmlErrors](QQuickWidget::Status status)
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
        else if (status == QQuickWidget::Error)
        {
            showQmlErrors();
        }
    });

    m_quickWidget->setSource(QUrl("qrc:/qml/main.qml"));

    // Fallback: check status immediately in case signal was already emitted before connect
    if (m_quickWidget->status() == QQuickWidget::Error)
        showQmlErrors();

    // ButtonBox standard
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &DialogRemoteUIEditor::acceptChanges);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    rootLayout->addWidget(m_quickWidget);
    rootLayout->addWidget(buttonBox);
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
    // Validate all IO widgets have an IO linked
    int pageIndex = 0, widgetX = 0, widgetY = 0;
    if (!m_model->validateWidgets(pageIndex, widgetX, widgetY))
    {
        QMessageBox::warning(this, tr("Validation Error"),
            tr("All IO widgets must have an IO linked.\n\n"
               "Page %1 contains a widget at position (%2, %3) without an IO.\n\n"
               "Please link an IO to all widgets before saving.")
            .arg(pageIndex + 1).arg(widgetX).arg(widgetY));

        // Highlight the invalid widget in QML
        QQuickItem *root = m_quickWidget->rootObject();
        if (root)
        {
            QMetaObject::invokeMethod(root, "highlightInvalidWidget",
                Q_ARG(QVariant, pageIndex),
                Q_ARG(QVariant, widgetX),
                Q_ARG(QVariant, widgetY));
        }
        return;
    }

    // Save the model back to IOBase
    QString pagesXml = m_model->toXml();
    ioBase->setRemoteUIPagesXml(pagesXml);

    // Update grid dimensions in IOBase params
    ioBase->set_param("grid_w", std::to_string(m_model->gridW()));
    ioBase->set_param("grid_h", std::to_string(m_model->gridH()));

    // Save screen saver settings to IOBase params
    ioBase->set_param("screensaver_timeout", std::to_string(m_model->screensaverTimeout()));
    ioBase->set_param("screensaver_dimming", std::to_string(m_model->screensaverDimming()));
    ioBase->set_param("screensaver_mode", m_model->screensaverMode().toStdString());
    ioBase->set_param("screensaver_clock_timezone",
                       TimezoneHelper::ianaToPosixTz(m_model->screensaverClockTimezone()).toStdString());
    ioBase->set_param("screensaver_clock_timezone_iana", m_model->screensaverClockTimezone().toStdString());
    ioBase->set_param("screensaver_clock_format", m_model->screensaverClockFormat().toStdString());
    ioBase->set_param("screensaver_clock_show_date", m_model->screensaverClockShowDate() ? "true" : "false");
    ioBase->set_param("screensaver_clock_date_format", m_model->screensaverClockDateFormat().toStdString());
    ioBase->set_param("screensaver_clock_seconds", m_model->screensaverClockSeconds() ? "true" : "false");

    // Save the title
    ioBase->set_param("name", m_model->title().toStdString());

    accept();
}
