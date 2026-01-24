#include "RemoteUIModel.h"
#include <QDebug>

RemoteUIModel::RemoteUIModel(QObject *parent)
    : QObject(parent)
{
}

RemoteUIModel::~RemoteUIModel()
{
    qDeleteAll(m_pages);
    m_pages.clear();
}

bool RemoteUIModel::loadFromXml(const QString &xml)
{
    // Clear existing pages
    qDeleteAll(m_pages);
    m_pages.clear();

    if (xml.isEmpty())
    {
        // No pages yet, create a default empty page
        auto *page = new PageModel(this);
        m_pages.append(page);
        connect(page, &PageModel::modelChanged, this, &RemoteUIModel::onPageModelChanged);
        emit pagesChanged();
        return true;
    }

    // Wrap in root element if needed for parsing
    QString wrappedXml = xml;
    if (!xml.trimmed().startsWith("<?xml"))
    {
        wrappedXml = QString("<?xml version=\"1.0\"?>"
                             "<root xmlns:calaos=\"http://www.calaos.fr\">%1</root>").arg(xml);
    }

    QDomDocument::ParseResult result = m_originalDoc.setContent(QAnyStringView(wrappedXml), QDomDocument::ParseOption::UseNamespaceProcessing);
    if (!result)
    {
        qWarning() << "RemoteUIModel: Failed to parse XML:" << result.errorMessage
                   << "at line" << result.errorLine << "column" << result.errorColumn;
        return false;
    }

    // Find the pages element
    QDomElement root = m_originalDoc.documentElement();
    QDomElement pagesElement = root.firstChildElement();

    // Handle case where root is the pages element directly
    QString localName = pagesElement.localName();
    if (localName.isEmpty())
        localName = pagesElement.tagName();

    if (localName == "pages")
    {
        // Iterate over page elements
        QDomElement pageElement = pagesElement.firstChildElement();
        while (!pageElement.isNull())
        {
            QString pageLocalName = pageElement.localName();
            if (pageLocalName.isEmpty())
                pageLocalName = pageElement.tagName();

            if (pageLocalName == "page")
            {
                auto *page = new PageModel(this);
                page->loadFromXmlElement(pageElement);
                m_pages.append(page);
                connect(page, &PageModel::modelChanged, this, &RemoteUIModel::onPageModelChanged);
            }
            pageElement = pageElement.nextSiblingElement();
        }
    }

    // If no pages found, create a default one
    if (m_pages.isEmpty())
    {
        auto *page = new PageModel(this);
        m_pages.append(page);
        connect(page, &PageModel::modelChanged, this, &RemoteUIModel::onPageModelChanged);
    }

    m_currentPageIndex = 0;
    emit pagesChanged();
    emit currentPageIndexChanged();

    return true;
}

QString RemoteUIModel::toXml() const
{
    QDomDocument doc;

    // Create pages element
    QDomElement pagesElement = doc.createElementNS("http://www.calaos.fr", "calaos:pages");
    doc.appendChild(pagesElement);

    // Add each page
    for (PageModel *page : m_pages)
    {
        pagesElement.appendChild(page->toXmlElement(doc));
    }

    // Return the XML with proper formatting
    QString xml = doc.toString(1);

    // Remove the XML declaration if present (we only want the content)
    if (xml.startsWith("<?xml"))
    {
        int endDecl = xml.indexOf("?>");
        if (endDecl != -1)
        {
            xml = xml.mid(endDecl + 2).trimmed();
        }
    }

    return xml;
}

void RemoteUIModel::setGridDimensions(int w, int h)
{
    bool changed = false;
    if (m_gridW != w)
    {
        m_gridW = w;
        emit gridWChanged();
        changed = true;
    }
    if (m_gridH != h)
    {
        m_gridH = h;
        emit gridHChanged();
        changed = true;
    }
    if (changed)
    {
        emit modelChanged();
    }
}

QQmlListProperty<PageModel> RemoteUIModel::pages()
{
    return QQmlListProperty<PageModel>(this, nullptr,
                                        &RemoteUIModel::appendPage,
                                        &RemoteUIModel::pageCount,
                                        &RemoteUIModel::page,
                                        &RemoteUIModel::clearPages);
}

void RemoteUIModel::setGridW(int w)
{
    if (m_gridW != w)
    {
        m_gridW = w;
        emit gridWChanged();
        setModified(true);
    }
}

void RemoteUIModel::setGridH(int h)
{
    if (m_gridH != h)
    {
        m_gridH = h;
        emit gridHChanged();
        setModified(true);
    }
}

void RemoteUIModel::setCurrentPageIndex(int index)
{
    if (index >= 0 && index < m_pages.count() && m_currentPageIndex != index)
    {
        m_currentPageIndex = index;
        emit currentPageIndexChanged();
    }
}

void RemoteUIModel::setTitle(const QString &title)
{
    if (m_title != title)
    {
        m_title = title;
        emit titleChanged();
        setModified(true);
    }
}

bool RemoteUIModel::validateWidgets(int &outPageIndex, int &outWidgetX, int &outWidgetY) const
{
    for (int p = 0; p < m_pages.count(); ++p)
    {
        PageModel *page = m_pages.at(p);
        if (!page) continue;

        const auto &widgets = page->widgetList();
        for (int w = 0; w < widgets.count(); ++w)
        {
            WidgetModel *widget = widgets.at(w);
            if (widget && widget->ioId().isEmpty())
            {
                outPageIndex = p;
                outWidgetX = widget->x();
                outWidgetY = widget->y();
                return false;
            }
        }
    }
    return true;
}

PageModel* RemoteUIModel::pageAt(int index) const
{
    if (index >= 0 && index < m_pages.count())
        return m_pages.at(index);
    return nullptr;
}

PageModel* RemoteUIModel::currentPage() const
{
    return pageAt(m_currentPageIndex);
}

PageModel* RemoteUIModel::addPage()
{
    auto *page = new PageModel(this);
    m_pages.append(page);
    connect(page, &PageModel::modelChanged, this, &RemoteUIModel::onPageModelChanged);
    emit pagesChanged();
    setModified(true);
    return page;
}

void RemoteUIModel::removePage(int index)
{
    if (index >= 0 && index < m_pages.count())
    {
        // Don't allow removing the last page
        if (m_pages.count() <= 1)
        {
            qWarning() << "RemoteUIModel: Cannot remove the last page";
            return;
        }

        PageModel *page = m_pages.takeAt(index);
        page->deleteLater();

        // Adjust current page index if needed
        if (m_currentPageIndex >= m_pages.count())
        {
            m_currentPageIndex = m_pages.count() - 1;
            emit currentPageIndexChanged();
        }

        emit pagesChanged();
        setModified(true);
    }
}

void RemoteUIModel::movePage(int fromIndex, int toIndex)
{
    if (fromIndex >= 0 && fromIndex < m_pages.count() &&
        toIndex >= 0 && toIndex < m_pages.count() &&
        fromIndex != toIndex)
    {
        m_pages.move(fromIndex, toIndex);

        // Adjust current page index
        if (m_currentPageIndex == fromIndex)
        {
            m_currentPageIndex = toIndex;
            emit currentPageIndexChanged();
        }

        emit pagesChanged();
        setModified(true);
    }
}

void RemoteUIModel::clearPages()
{
    if (!m_pages.isEmpty())
    {
        qDeleteAll(m_pages);
        m_pages.clear();

        // Always keep at least one page
        auto *page = new PageModel(this);
        m_pages.append(page);
        connect(page, &PageModel::modelChanged, this, &RemoteUIModel::onPageModelChanged);

        m_currentPageIndex = 0;
        emit currentPageIndexChanged();
        emit pagesChanged();
        setModified(true);
    }
}

void RemoteUIModel::setModified(bool modified)
{
    if (m_modified != modified)
    {
        m_modified = modified;
        emit modifiedChanged();
    }
}

void RemoteUIModel::onPageModelChanged()
{
    setModified(true);
    emit modelChanged();
}

// QQmlListProperty callbacks
void RemoteUIModel::appendPage(QQmlListProperty<PageModel> *list, PageModel *page)
{
    auto *model = qobject_cast<RemoteUIModel*>(list->object);
    if (model && page)
    {
        page->setParent(model);
        model->m_pages.append(page);
        QObject::connect(page, &PageModel::modelChanged, model, &RemoteUIModel::onPageModelChanged);
        emit model->pagesChanged();
        model->setModified(true);
    }
}

qsizetype RemoteUIModel::pageCount(QQmlListProperty<PageModel> *list)
{
    auto *model = qobject_cast<RemoteUIModel*>(list->object);
    if (model)
        return model->m_pages.count();
    return 0;
}

PageModel* RemoteUIModel::page(QQmlListProperty<PageModel> *list, qsizetype index)
{
    auto *model = qobject_cast<RemoteUIModel*>(list->object);
    if (model)
        return model->pageAt(static_cast<int>(index));
    return nullptr;
}

void RemoteUIModel::clearPages(QQmlListProperty<PageModel> *list)
{
    auto *model = qobject_cast<RemoteUIModel*>(list->object);
    if (model)
    {
        model->clearPages();
    }
}
