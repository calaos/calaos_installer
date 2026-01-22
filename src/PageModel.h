#ifndef PAGEMODEL_H
#define PAGEMODEL_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QDomElement>
#include <QQmlListProperty>
#include "WidgetModel.h"

class PageModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<WidgetModel> widgets READ widgets NOTIFY widgetsChanged)
    Q_PROPERTY(int widgetCount READ widgetCount NOTIFY widgetsChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString pageType READ pageType WRITE setPageType NOTIFY pageTypeChanged)

public:
    explicit PageModel(QObject *parent = nullptr);
    ~PageModel() override;

    // Load from XML element, preserving unknown attributes
    void loadFromXmlElement(const QDomElement &element);

    // Serialize to XML element, including unknown attributes
    QDomElement toXmlElement(QDomDocument &doc) const;

    // QML list property
    QQmlListProperty<WidgetModel> widgets();
    int widgetCount() const { return m_widgets.count(); }

    // Page properties
    QString name() const { return m_name; }
    void setName(const QString &name);
    QString pageType() const { return m_pageType; }
    void setPageType(const QString &type);

    // Widget management (callable from QML)
    Q_INVOKABLE WidgetModel* widgetAt(int index) const;
    Q_INVOKABLE void addWidget(WidgetModel *widget);
    Q_INVOKABLE WidgetModel* createWidget(const QString &ioId, const QString &type, int x, int y, int w = 1, int h = 1);
    Q_INVOKABLE void removeWidget(int index);
    Q_INVOKABLE void removeWidgetObject(WidgetModel *widget);
    Q_INVOKABLE void moveWidget(int fromIndex, int toIndex);
    Q_INVOKABLE void clearWidgets();

    // Access to unknown attributes for preservation
    QMap<QString, QString> unknownAttributes() const { return m_unknownAttrs; }
    void setUnknownAttribute(const QString &key, const QString &value);

    // Internal widget list access
    QList<WidgetModel*> widgetList() const { return m_widgets; }

signals:
    void widgetsChanged();
    void modelChanged();
    void nameChanged();
    void pageTypeChanged();

private:
    // QQmlListProperty callbacks
    static void appendWidget(QQmlListProperty<WidgetModel> *list, WidgetModel *widget);
    static qsizetype widgetCount(QQmlListProperty<WidgetModel> *list);
    static WidgetModel* widget(QQmlListProperty<WidgetModel> *list, qsizetype index);
    static void clearWidgets(QQmlListProperty<WidgetModel> *list);

    QList<WidgetModel*> m_widgets;
    QString m_name;
    QString m_pageType;

    // Store unknown/future attributes to preserve them during roundtrip
    QMap<QString, QString> m_unknownAttrs;

    // List of known attributes that we handle explicitly
    static const QStringList s_knownAttributes;
};

#endif // PAGEMODEL_H
