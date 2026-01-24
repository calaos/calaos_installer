#ifndef REMOTEUIMODEL_H
#define REMOTEUIMODEL_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QDomDocument>
#include <QQmlListProperty>
#include "PageModel.h"

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

    // Property setters
    void setGridW(int w);
    void setGridH(int h);
    void setCurrentPageIndex(int index);
    void setTitle(const QString &title);

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

    // Store the original document structure for preservation
    QDomDocument m_originalDoc;
};

#endif // REMOTEUIMODEL_H
