#include "WidgetModel.h"

const QStringList WidgetModel::s_knownAttributes = {
    "io_id", "type", "name", "x", "y", "w", "h",
    "clock_timezone", "clock_format", "clock_show_date", "clock_date_format", "clock_seconds"
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
    m_name = element.attribute("name");
    m_x = element.attribute("x", "0").toInt();
    m_y = element.attribute("y", "0").toInt();
    m_w = element.attribute("w", "1").toInt();
    m_h = element.attribute("h", "1").toInt();

    // Load clock-specific attributes
    if (categoryForType(m_type) == "Clock")
    {
        m_clockTimezone = element.attribute("clock_timezone", "UTC");
        m_clockFormat = element.attribute("clock_format", "24");
        m_clockShowDate = element.attribute("clock_show_date", "true") == "true";
        m_clockDateFormat = element.attribute("clock_date_format", "DD/MM/YYYY");
        m_clockSeconds = element.attribute("clock_seconds", "false") == "true";
    }

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

    QString cat = categoryForType(m_type);

    // Write io_id only for IO widgets
    if (cat == "IO")
        element.setAttribute("io_id", m_ioId);

    element.setAttribute("type", m_type);
    if (!m_name.isEmpty())
        element.setAttribute("name", m_name);
    element.setAttribute("x", m_x);
    element.setAttribute("y", m_y);
    element.setAttribute("w", m_w);
    element.setAttribute("h", m_h);

    // Write clock-specific attributes only for Clock widgets
    if (cat == "Clock")
    {
        element.setAttribute("clock_timezone", m_clockTimezone);
        element.setAttribute("clock_format", m_clockFormat);
        element.setAttribute("clock_show_date", m_clockShowDate ? "true" : "false");
        element.setAttribute("clock_date_format", m_clockDateFormat);
        element.setAttribute("clock_seconds", m_clockSeconds ? "true" : "false");
    }

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

void WidgetModel::setName(const QString &name)
{
    if (m_name != name)
    {
        m_name = name;
        emit nameChanged();
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

// --- Clock property setters ---

void WidgetModel::setClockTimezone(const QString &tz)
{
    if (m_clockTimezone != tz)
    {
        m_clockTimezone = tz;
        emit clockTimezoneChanged();
        emit modelChanged();
    }
}

void WidgetModel::setClockFormat(const QString &format)
{
    if (m_clockFormat != format)
    {
        m_clockFormat = format;
        emit clockFormatChanged();
        emit modelChanged();
    }
}

void WidgetModel::setClockShowDate(bool show)
{
    if (m_clockShowDate != show)
    {
        m_clockShowDate = show;
        emit clockShowDateChanged();
        emit modelChanged();
    }
}

void WidgetModel::setClockDateFormat(const QString &format)
{
    if (m_clockDateFormat != format)
    {
        m_clockDateFormat = format;
        emit clockDateFormatChanged();
        emit modelChanged();
    }
}

void WidgetModel::setClockSeconds(bool seconds)
{
    if (m_clockSeconds != seconds)
    {
        m_clockSeconds = seconds;
        emit clockSecondsChanged();
        emit modelChanged();
    }
}

// --- Category system ---

QString WidgetModel::categoryForType(const QString &type)
{
    if (type == "Clock")
        return QStringLiteral("Clock");
    // All other types (including IO-bound widget types) are in the IO category
    return QStringLiteral("IO");
}

QStringList WidgetModel::availableCategories()
{
    return QStringList{ "IO", "Clock" };
}

QStringList WidgetModel::typesForCategory(const QString &category)
{
    if (category == "Clock")
        return QStringList{ "Clock" };
    // IO category: return available IO widget types
    return availableWidgetTypes();
}

bool WidgetModel::categoryRequiresIO(const QString &category)
{
    return category == "IO";
}

// --- Clock utilities ---

QStringList WidgetModel::availableTimezones()
{
    return QStringList{
        "UTC-12",
        "UTC-11",
        "UTC-10",
        "UTC-9:30",
        "UTC-9",
        "UTC-8",
        "UTC-7",
        "UTC-6",
        "UTC-5",
        "UTC-4",
        "UTC-3:30",
        "UTC-3",
        "UTC-2",
        "UTC-1",
        "UTC",
        "UTC+1",
        "UTC+2",
        "UTC+3",
        "UTC+3:30",
        "UTC+4",
        "UTC+4:30",
        "UTC+5",
        "UTC+5:30",
        "UTC+5:45",
        "UTC+6",
        "UTC+6:30",
        "UTC+7",
        "UTC+8",
        "UTC+8:45",
        "UTC+9",
        "UTC+9:30",
        "UTC+10",
        "UTC+10:30",
        "UTC+11",
        "UTC+12",
        "UTC+12:45",
        "UTC+13",
        "UTC+14",
    };
}

QStringList WidgetModel::availableDateFormats()
{
    return QStringList{
        "DD/MM/YYYY",
        "MM/DD/YYYY",
        "YYYY-MM-DD",
        "DD MMM YYYY",
        "MMM DD YYYY",
    };
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
