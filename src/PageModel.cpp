#include "PageModel.h"

const QStringList PageModel::s_knownAttributes = {
    "name", "type"  // Known attributes we handle explicitly
};

PageModel::PageModel(QObject *parent)
    : QObject(parent)
    , m_name("Page")
    , m_pageType("default")
{
}

PageModel::~PageModel()
{
    qDeleteAll(m_widgets);
    m_widgets.clear();
}

void PageModel::loadFromXmlElement(const QDomElement &element)
{
    // Clear existing widgets
    qDeleteAll(m_widgets);
    m_widgets.clear();

    // Read known attributes
    m_name = element.attribute("name", "Page");
    m_pageType = element.attribute("type", "default");

    // Store unknown attributes for preservation
    m_unknownAttrs.clear();
    QDomNamedNodeMap attrs = element.attributes();
    for (int i = 0; i < attrs.count(); ++i)
    {
        QDomAttr attr = attrs.item(i).toAttr();
        QString name = attr.name();
        if (!s_knownAttributes.contains(name))
        {
            m_unknownAttrs.insert(name, attr.value());
        }
    }

    // Load widgets from child elements
    QDomElement widgetElement = element.firstChildElement();
    while (!widgetElement.isNull())
    {
        // Check for widget element (handle namespace)
        QString localName = widgetElement.localName();
        if (localName.isEmpty())
            localName = widgetElement.tagName();

        if (localName == "widget")
        {
            auto *widget = new WidgetModel(this);
            widget->loadFromXmlElement(widgetElement);
            m_widgets.append(widget);

            // Connect widget changes to page changes
            connect(widget, &WidgetModel::modelChanged, this, &PageModel::modelChanged);
        }
        widgetElement = widgetElement.nextSiblingElement();
    }

    emit widgetsChanged();
}

QDomElement PageModel::toXmlElement(QDomDocument &doc) const
{
    QDomElement element = doc.createElementNS("http://www.calaos.fr", "calaos:page");

    // Write known attributes
    element.setAttribute("name", m_name);
    element.setAttribute("type", m_pageType);

    // Write unknown/preserved attributes
    for (auto it = m_unknownAttrs.constBegin(); it != m_unknownAttrs.constEnd(); ++it)
    {
        element.setAttribute(it.key(), it.value());
    }

    // Write widgets
    for (WidgetModel *widget : m_widgets)
    {
        element.appendChild(widget->toXmlElement(doc));
    }

    return element;
}

void PageModel::setName(const QString &name)
{
    if (m_name != name)
    {
        m_name = name;
        emit nameChanged();
        emit modelChanged();
    }
}

void PageModel::setPageType(const QString &type)
{
    if (m_pageType != type)
    {
        m_pageType = type;
        emit pageTypeChanged();
        emit modelChanged();
    }
}

QQmlListProperty<WidgetModel> PageModel::widgets()
{
    return QQmlListProperty<WidgetModel>(this, nullptr,
                                          &PageModel::appendWidget,
                                          &PageModel::widgetCount,
                                          &PageModel::widget,
                                          &PageModel::clearWidgets);
}

WidgetModel* PageModel::widgetAt(int index) const
{
    if (index >= 0 && index < m_widgets.count())
        return m_widgets.at(index);
    return nullptr;
}

void PageModel::addWidget(WidgetModel *widget)
{
    if (widget)
    {
        widget->setParent(this);
        m_widgets.append(widget);
        connect(widget, &WidgetModel::modelChanged, this, &PageModel::modelChanged);
        emit widgetsChanged();
        emit modelChanged();
    }
}

WidgetModel* PageModel::createWidget(const QString &ioId, const QString &type, int x, int y, int w, int h)
{
    auto *widget = new WidgetModel(this);
    widget->setIoId(ioId);
    widget->setType(type);
    widget->setX(x);
    widget->setY(y);
    widget->setW(w);
    widget->setH(h);

    m_widgets.append(widget);
    connect(widget, &WidgetModel::modelChanged, this, &PageModel::modelChanged);
    emit widgetsChanged();
    emit modelChanged();

    return widget;
}

void PageModel::removeWidget(int index)
{
    if (index >= 0 && index < m_widgets.count())
    {
        WidgetModel *widget = m_widgets.takeAt(index);
        widget->deleteLater();
        emit widgetsChanged();
        emit modelChanged();
    }
}

void PageModel::removeWidgetObject(WidgetModel *widget)
{
    int index = m_widgets.indexOf(widget);
    if (index >= 0)
    {
        removeWidget(index);
    }
}

void PageModel::moveWidget(int fromIndex, int toIndex)
{
    if (fromIndex >= 0 && fromIndex < m_widgets.count() &&
        toIndex >= 0 && toIndex < m_widgets.count() &&
        fromIndex != toIndex)
    {
        m_widgets.move(fromIndex, toIndex);
        emit widgetsChanged();
        emit modelChanged();
    }
}

void PageModel::clearWidgets()
{
    if (!m_widgets.isEmpty())
    {
        qDeleteAll(m_widgets);
        m_widgets.clear();
        emit widgetsChanged();
        emit modelChanged();
    }
}

void PageModel::setUnknownAttribute(const QString &key, const QString &value)
{
    m_unknownAttrs.insert(key, value);
    emit modelChanged();
}

// QQmlListProperty callbacks
void PageModel::appendWidget(QQmlListProperty<WidgetModel> *list, WidgetModel *widget)
{
    auto *page = qobject_cast<PageModel*>(list->object);
    if (page)
    {
        page->addWidget(widget);
    }
}

qsizetype PageModel::widgetCount(QQmlListProperty<WidgetModel> *list)
{
    auto *page = qobject_cast<PageModel*>(list->object);
    if (page)
        return page->m_widgets.count();
    return 0;
}

WidgetModel* PageModel::widget(QQmlListProperty<WidgetModel> *list, qsizetype index)
{
    auto *page = qobject_cast<PageModel*>(list->object);
    if (page)
        return page->widgetAt(static_cast<int>(index));
    return nullptr;
}

void PageModel::clearWidgets(QQmlListProperty<WidgetModel> *list)
{
    auto *page = qobject_cast<PageModel*>(list->object);
    if (page)
    {
        page->clearWidgets();
    }
}
