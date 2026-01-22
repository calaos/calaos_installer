#include "WidgetModel.h"

const QStringList WidgetModel::s_knownAttributes = {
    "io_id", "type", "x", "y", "w", "h"
};

WidgetModel::WidgetModel(QObject *parent)
    : QObject(parent)
{
}

void WidgetModel::loadFromXmlElement(const QDomElement &element)
{
    // Load known attributes
    m_ioId = element.attribute("io_id");
    m_type = element.attribute("type");
    m_x = element.attribute("x", "0").toInt();
    m_y = element.attribute("y", "0").toInt();
    m_w = element.attribute("w", "1").toInt();
    m_h = element.attribute("h", "1").toInt();

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
}

QDomElement WidgetModel::toXmlElement(QDomDocument &doc) const
{
    QDomElement element = doc.createElementNS("http://www.calaos.fr", "calaos:widget");

    // Write known attributes
    element.setAttribute("io_id", m_ioId);
    element.setAttribute("type", m_type);
    element.setAttribute("x", m_x);
    element.setAttribute("y", m_y);
    element.setAttribute("w", m_w);
    element.setAttribute("h", m_h);

    // Write unknown/preserved attributes
    for (auto it = m_unknownAttrs.constBegin(); it != m_unknownAttrs.constEnd(); ++it)
    {
        element.setAttribute(it.key(), it.value());
    }

    return element;
}

void WidgetModel::setIoId(const QString &ioId)
{
    if (m_ioId != ioId)
    {
        m_ioId = ioId;
        emit ioIdChanged();
        emit modelChanged();
    }
}

void WidgetModel::setType(const QString &type)
{
    if (m_type != type)
    {
        m_type = type;
        emit typeChanged();
        emit modelChanged();
    }
}

void WidgetModel::setX(int x)
{
    if (m_x != x)
    {
        m_x = x;
        emit xChanged();
        emit modelChanged();
    }
}

void WidgetModel::setY(int y)
{
    if (m_y != y)
    {
        m_y = y;
        emit yChanged();
        emit modelChanged();
    }
}

void WidgetModel::setW(int w)
{
    if (m_w != w)
    {
        m_w = w;
        emit wChanged();
        emit modelChanged();
    }
}

void WidgetModel::setH(int h)
{
    if (m_h != h)
    {
        m_h = h;
        emit hChanged();
        emit modelChanged();
    }
}

void WidgetModel::setUnknownAttribute(const QString &key, const QString &value)
{
    m_unknownAttrs.insert(key, value);
    emit modelChanged();
}

QStringList WidgetModel::availableWidgetTypes()
{
    return QStringList{
        "LightSwitch",
        "Temperature",
        "Scenario",
    };
}

QString WidgetModel::widgetTypeForGuiType(const QString &guiType)
{
    // Mapping from IO gui_type to widget type
    static const QMap<QString, QString> mapping = {
        // Lights and switches
        {"light", "LightSwitch"},
        {"switch", "LightSwitch"},
        {"switch_long", "LightSwitch"},
        {"switch3", "LightSwitch"},
        {"var_bool", "LightSwitch"},

        // Dimmers
        {"light_dimmer", "LightSwitch"},
        {"light_rgb", "LightSwitch"},

        // Temperature
        {"temp", "Temperature"},

        // Scenarios
        {"scenario", "Scenario"},

        // Shutters (future)
        {"shutter", "Shutter"},
        {"shutter_smart", "Shutter"},

        // Analog (future)
        {"analog_in", "Gauge"},
        {"analog_out", "Gauge"},
        {"var_int", "Gauge"},

        // Time (future)
        {"time", "Timer"},
        {"timer", "Timer"},
        {"time_range", "TimeRange"},

        // Text/String (future)
        {"string_in", "Text"},
        {"string_out", "Text"},
        {"var_string", "Text"},

        // Media (future)
        {"audio", "Audio"},
        {"camera", "Camera"},
        {"avreceiver", "AVReceiver"},
    };

    return mapping.value(guiType, "Generic");
}
