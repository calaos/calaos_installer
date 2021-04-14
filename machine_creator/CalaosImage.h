#ifndef CALAOSIMAGE_H
#define CALAOSIMAGE_H

#include <QtCore>
#include "QmlHelpers/qqmlhelpers.h"

class FilterImageModel: public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FilterImageModel(QObject *parent = 0);

    void setFilterType(bool onlyStable);
    void setFilterMachine(QString machine);

    Q_INVOKABLE QModelIndex indexToSource(const QModelIndex &idx) const;
    Q_INVOKABLE QModelIndex indexFromSource(const QModelIndex &idx) const;

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    bool filter_all = false;
    QString filter_machine;
};

class CalaosImage;
class CalaosImageModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    CalaosImageModel(QObject *parent = nullptr);

    void loadJson(const QJsonArray &arr);
    const CalaosImage *itemAt(int rowidx) const;
    CalaosImage *itemAt(int rowidx);
    void clear();

    QStringList getMachines();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    QVector<CalaosImage *> m_items;
    QStringList machines;
};

class CalaosImage: public QObject
{
    Q_OBJECT

    QML_WRITABLE_PROPERTY(QString, url)
    QML_WRITABLE_PROPERTY(QString, version)
    QML_WRITABLE_PROPERTY(QString, type)
    QML_WRITABLE_PROPERTY(qint64, filesize)
    QML_WRITABLE_PROPERTY(QString, machine)
    QML_WRITABLE_PROPERTY(QDateTime, releaseDate)
    QML_WRITABLE_PROPERTY(QString, checksum)

public:
    CalaosImage();

    void load(const QJsonObject obj);
};

#endif // CALAOSIMAGE_H
