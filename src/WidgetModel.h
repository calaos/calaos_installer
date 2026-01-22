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
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int w READ w WRITE setW NOTIFY wChanged)
    Q_PROPERTY(int h READ h WRITE setH NOTIFY hChanged)

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
    int x() const { return m_x; }
    int y() const { return m_y; }
    int w() const { return m_w; }
    int h() const { return m_h; }

    // Property setters
    void setIoId(const QString &ioId);
    void setType(const QString &type);
    void setX(int x);
    void setY(int y);
    void setW(int w);
    void setH(int h);

    // Access to unknown attributes for preservation
    QMap<QString, QString> unknownAttributes() const { return m_unknownAttrs; }
    void setUnknownAttribute(const QString &key, const QString &value);

signals:
    void ioIdChanged();
    void typeChanged();
    void xChanged();
    void yChanged();
    void wChanged();
    void hChanged();
    void modelChanged();

private:
    QString m_ioId;
    QString m_type;
    int m_x = 0;
    int m_y = 0;
    int m_w = 1;
    int m_h = 1;

    // Store unknown/future attributes to preserve them during roundtrip
    QMap<QString, QString> m_unknownAttrs;

    // List of known attributes that we handle explicitly
    static const QStringList s_knownAttributes;
};

#endif // WIDGETMODEL_H
