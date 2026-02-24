#ifndef WIDGETMODEL_H
#define WIDGETMODEL_H

#include <QObject>
#include <QMap>
#include <QDomElement>

class WidgetModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ioId READ ioId WRITE setIoId NOTIFY ioIdChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int w READ w WRITE setW NOTIFY wChanged)
    Q_PROPERTY(int h READ h WRITE setH NOTIFY hChanged)

    // Category is derived from type — not stored in XML
    Q_PROPERTY(QString category READ category NOTIFY typeChanged)

    // Clock-specific properties (stored as clock_* XML attributes)
    Q_PROPERTY(QString clockTimezone READ clockTimezone WRITE setClockTimezone NOTIFY clockTimezoneChanged)
    Q_PROPERTY(QString clockFormat READ clockFormat WRITE setClockFormat NOTIFY clockFormatChanged)
    Q_PROPERTY(bool clockShowDate READ clockShowDate WRITE setClockShowDate NOTIFY clockShowDateChanged)
    Q_PROPERTY(QString clockDateFormat READ clockDateFormat WRITE setClockDateFormat NOTIFY clockDateFormatChanged)
    Q_PROPERTY(bool clockSeconds READ clockSeconds WRITE setClockSeconds NOTIFY clockSecondsChanged)

public:
    explicit WidgetModel(QObject *parent = nullptr);
    ~WidgetModel() override = default;

    // Load from XML element, preserving unknown attributes
    void loadFromXmlElement(const QDomElement &element);

    // Serialize to XML element, including unknown attributes
    QDomElement toXmlElement(QDomDocument &doc) const;

    // Property getters
    QString ioId() const { return m_ioId; }
    QString type() const { return m_type; }
    QString name() const { return m_name; }
    int x() const { return m_x; }
    int y() const { return m_y; }
    int w() const { return m_w; }
    int h() const { return m_h; }

    // Category derived from type
    QString category() const { return categoryForType(m_type); }

    // Clock property getters
    QString clockTimezone() const { return m_clockTimezone; }
    QString clockFormat() const { return m_clockFormat; }
    bool clockShowDate() const { return m_clockShowDate; }
    QString clockDateFormat() const { return m_clockDateFormat; }
    bool clockSeconds() const { return m_clockSeconds; }

    // Property setters
    void setIoId(const QString &ioId);
    void setType(const QString &type);
    void setName(const QString &name);
    void setX(int x);
    void setY(int y);
    void setW(int w);
    void setH(int h);

    // Clock property setters
    void setClockTimezone(const QString &tz);
    void setClockFormat(const QString &format);
    void setClockShowDate(bool show);
    void setClockDateFormat(const QString &format);
    void setClockSeconds(bool seconds);

    // Get list of available IO widget types
    Q_INVOKABLE static QStringList availableWidgetTypes();

    // Get widget type for a given gui_type
    Q_INVOKABLE static QString widgetTypeForGuiType(const QString &guiType);

    // Category system — derives category from type
    Q_INVOKABLE static QString categoryForType(const QString &type);
    Q_INVOKABLE static QStringList availableCategories();
    Q_INVOKABLE static QStringList typesForCategory(const QString &category);
    Q_INVOKABLE static bool categoryRequiresIO(const QString &category);

    // Clock utilities
    Q_INVOKABLE static QStringList availableTimezones();
    Q_INVOKABLE static QStringList availableDateFormats();

    // Access to unknown attributes for preservation
    QMap<QString, QString> unknownAttributes() const { return m_unknownAttrs; }
    void setUnknownAttribute(const QString &key, const QString &value);

signals:
    void ioIdChanged();
    void typeChanged();
    void nameChanged();
    void xChanged();
    void yChanged();
    void wChanged();
    void hChanged();
    void modelChanged();
    void clockTimezoneChanged();
    void clockFormatChanged();
    void clockShowDateChanged();
    void clockDateFormatChanged();
    void clockSecondsChanged();

private:
    QString m_ioId;
    QString m_type;
    QString m_name;
    int m_x = 0;
    int m_y = 0;
    int m_w = 1;
    int m_h = 1;

    // Clock-specific properties
    QString m_clockTimezone = QStringLiteral("UTC");
    QString m_clockFormat = QStringLiteral("24");
    bool m_clockShowDate = true;
    QString m_clockDateFormat = QStringLiteral("DD/MM/YYYY");
    bool m_clockSeconds = false;

    // Store unknown/future attributes to preserve them during roundtrip
    QMap<QString, QString> m_unknownAttrs;

    // List of known attributes that we handle explicitly
    static const QStringList s_knownAttributes;
};

#endif // WIDGETMODEL_H
