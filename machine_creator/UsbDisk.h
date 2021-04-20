#ifndef USBDISK_H
#define USBDISK_H

#include <QtCore>
#include "QmlHelpers/qqmlhelpers.h"

class UsbDisk;
class UsbDiskModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    UsbDiskModel(QObject *parent = nullptr);

    void loadModel(const QVector<UsbDisk *> lst);
    const UsbDisk *itemAt(int rowidx) const;
    UsbDisk *itemAt(int rowidx);
    void clear();

    QModelIndex lastIndex();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    QVector<UsbDisk *> m_items;
};

class UsbDisk: public QObject
{
    Q_OBJECT

    QML_WRITABLE_PROPERTY(QString, name)
    QML_WRITABLE_PROPERTY(QStringList, volumes) //list of mounted volumes
    QML_WRITABLE_PROPERTY(qint64, size)
    QML_WRITABLE_PROPERTY(quint32, sectorSize)
    QML_WRITABLE_PROPERTY(QString, physicalDevice)
    QML_WRITABLE_PROPERTY(bool, isRemovable)
    QML_WRITABLE_PROPERTY(bool, isSystem)
    QML_WRITABLE_PROPERTY(bool, isUsb)
    QML_WRITABLE_PROPERTY(bool, isSD)

public:
    UsbDisk();
    UsbDisk(UsbDisk *d);
};
#endif // USBDISK_H
