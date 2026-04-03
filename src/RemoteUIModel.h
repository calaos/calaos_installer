#ifndef REMOTEUIMODEL_H
#define REMOTEUIMODEL_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QDomDocument>
#include <QQmlListProperty>
#include "PageModel.h"
#include "TimezoneHelper.h"

class RemoteUIModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<PageModel> pages READ pages NOTIFY pagesChanged)
    Q_PROPERTY(int pageCount READ pageCount NOTIFY pagesChanged)
    Q_PROPERTY(int gridW READ gridW WRITE setGridW NOTIFY gridWChanged)
    Q_PROPERTY(int gridH READ gridH WRITE setGridH NOTIFY gridHChanged)
    Q_PROPERTY(bool modified READ isModified NOTIFY modifiedChanged)
    Q_PROPERTY(int currentPageIndex READ currentPageIndex WRITE setCurrentPageIndex NOTIFY currentPageIndexChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

    // Screen saver properties
    Q_PROPERTY(int screensaverTimeout READ screensaverTimeout WRITE setScreensaverTimeout NOTIFY screensaverTimeoutChanged)
    Q_PROPERTY(int screensaverDimming READ screensaverDimming WRITE setScreensaverDimming NOTIFY screensaverDimmingChanged)
    Q_PROPERTY(QString screensaverMode READ screensaverMode WRITE setScreensaverMode NOTIFY screensaverModeChanged)
    Q_PROPERTY(QString screensaverClockTimezone READ screensaverClockTimezone WRITE setScreensaverClockTimezone NOTIFY screensaverClockTimezoneChanged)
    Q_PROPERTY(QString screensaverClockFormat READ screensaverClockFormat WRITE setScreensaverClockFormat NOTIFY screensaverClockFormatChanged)
    Q_PROPERTY(bool screensaverClockShowDate READ screensaverClockShowDate WRITE setScreensaverClockShowDate NOTIFY screensaverClockShowDateChanged)
    Q_PROPERTY(QString screensaverClockDateFormat READ screensaverClockDateFormat WRITE setScreensaverClockDateFormat NOTIFY screensaverClockDateFormatChanged)
    Q_PROPERTY(bool screensaverClockSeconds READ screensaverClockSeconds WRITE setScreensaverClockSeconds NOTIFY screensaverClockSecondsChanged)

public:
    explicit RemoteUIModel(QObject *parent = nullptr);
    ~RemoteUIModel() override;

    // Load pages from XML string (content of <calaos:pages>)
    bool loadFromXml(const QString &xml);

    // Serialize pages to XML string
    QString toXml() const;

    // Set grid dimensions (from IOBase params)
    void setGridDimensions(int w, int h);

    // QML list property
    QQmlListProperty<PageModel> pages();
    int pageCount() const { return m_pages.count(); }

    // Property getters
    int gridW() const { return m_gridW; }
    int gridH() const { return m_gridH; }
    bool isModified() const { return m_modified; }
    int currentPageIndex() const { return m_currentPageIndex; }
    QString title() const { return m_title; }

    // Screen saver getters
    int screensaverTimeout() const { return m_screensaverTimeout; }
    int screensaverDimming() const { return m_screensaverDimming; }
    QString screensaverMode() const { return m_screensaverMode; }
    QString screensaverClockTimezone() const { return m_screensaverClockTimezone; }
    QString screensaverClockFormat() const { return m_screensaverClockFormat; }
    bool screensaverClockShowDate() const { return m_screensaverClockShowDate; }
    QString screensaverClockDateFormat() const { return m_screensaverClockDateFormat; }
    bool screensaverClockSeconds() const { return m_screensaverClockSeconds; }

    // Property setters
    void setGridW(int w);
    void setGridH(int h);
    void setCurrentPageIndex(int index);
    void setTitle(const QString &title);

    // Screen saver setters
    void setScreensaverTimeout(int timeout);
    void setScreensaverDimming(int dimming);
    void setScreensaverMode(const QString &mode);
    void setScreensaverClockTimezone(const QString &tz);
    void setScreensaverClockFormat(const QString &format);
    void setScreensaverClockShowDate(bool show);
    void setScreensaverClockDateFormat(const QString &format);
    void setScreensaverClockSeconds(bool seconds);

    // Validation - returns true if all widgets have ioId, false otherwise
    // If invalid, outPageIndex, outWidgetX, outWidgetY contain first invalid widget info
    Q_INVOKABLE bool validateWidgets(int &outPageIndex, int &outWidgetX, int &outWidgetY) const;

    // Page management (callable from QML)
    Q_INVOKABLE PageModel* pageAt(int index) const;
    Q_INVOKABLE PageModel* currentPage() const;
    Q_INVOKABLE PageModel* addPage();
    Q_INVOKABLE void removePage(int index);
    Q_INVOKABLE void movePage(int fromIndex, int toIndex);
    Q_INVOKABLE void clearPages();

    // Mark as modified/saved
    void setModified(bool modified);
    void resetModified() { setModified(false); }

    // Access to internal page list
    QList<PageModel*> pageList() const { return m_pages; }

signals:
    void pagesChanged();
    void gridWChanged();
    void gridHChanged();
    void modifiedChanged();
    void currentPageIndexChanged();
    void modelChanged();
    void titleChanged();

    // Screen saver signals
    void screensaverTimeoutChanged();
    void screensaverDimmingChanged();
    void screensaverModeChanged();
    void screensaverClockTimezoneChanged();
    void screensaverClockFormatChanged();
    void screensaverClockShowDateChanged();
    void screensaverClockDateFormatChanged();
    void screensaverClockSecondsChanged();

private slots:
    void onPageModelChanged();

private:
    // QQmlListProperty callbacks
    static void appendPage(QQmlListProperty<PageModel> *list, PageModel *page);
    static qsizetype pageCount(QQmlListProperty<PageModel> *list);
    static PageModel* page(QQmlListProperty<PageModel> *list, qsizetype index);
    static void clearPages(QQmlListProperty<PageModel> *list);

    QList<PageModel*> m_pages;
    int m_gridW = 3;
    int m_gridH = 3;
    bool m_modified = false;
    int m_currentPageIndex = 0;
    QString m_title;

    // Screen saver settings
    int m_screensaverTimeout = 0;          // seconds, 0 = disabled
    int m_screensaverDimming = 0;          // 0-100%, 0 = screen off
    QString m_screensaverMode = "black";   // "black" or "clock"
    QString m_screensaverClockTimezone = "UTC";
    QString m_screensaverClockFormat = "24";
    bool m_screensaverClockShowDate = true;
    QString m_screensaverClockDateFormat = "DD/MM/YYYY";
    bool m_screensaverClockSeconds = false;

    // Store the original document structure for preservation
    QDomDocument m_originalDoc;
};

#endif // REMOTEUIMODEL_H
