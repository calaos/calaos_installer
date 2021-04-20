#include "UsbDisk.h"
#include "Utils.h"
#include "StorageDelegate.h"

UsbDiskModel::UsbDiskModel(QObject *parent):
    QAbstractTableModel(parent)
{
}

void UsbDiskModel::loadModel(const QVector<UsbDisk *> lst)
{
    beginResetModel();
    qDeleteAll(m_items);
    m_items.clear();
    m_items.append(lst);
    endResetModel();
}

const UsbDisk *UsbDiskModel::itemAt(int rowidx) const
{
    if (rowidx < 0 || rowidx >= m_items.size())
            return nullptr;
        return m_items.at(rowidx);
}

UsbDisk *UsbDiskModel::itemAt(int rowidx)
{
    if (rowidx < 0 || rowidx >= m_items.size())
            return nullptr;
    return m_items.at(rowidx);
}

void UsbDiskModel::clear()
{
    beginResetModel();
    qDeleteAll(m_items);
    m_items.clear();
    endResetModel();
}

QModelIndex UsbDiskModel::lastIndex()
{
    return index(rowCount() - 1, 0);
}

int UsbDiskModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int UsbDiskModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant UsbDiskModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= m_items.size())
        return {};

    auto *im = m_items.at(index.row());

    if (role == Qt::DisplayRole ||
        role == Qt::EditRole)
    {
//        return QStringLiteral("%1 - %2 (%3)")
//                .arg(im->get_volumes().isEmpty()? "": im->get_volumes().join(", "),
//                     im->get_name(),
//                     Utils::sizeHuman(im->get_size()));
    }
    else if (role == StorageDelegate::headerTextRole)
    {
        return QStringLiteral("%1 [%2]").arg(im->get_name(),
                                             Utils::sizeHuman(im->get_size()));
    }
    else if (role == StorageDelegate::subHeaderTextRole)
    {
        if (!im->get_volumes().isEmpty())
            return im->get_volumes().join(", ");
    }
    else if (role == Qt::DecorationRole)
    {
        QString icres = ":/img/ic_storage_40px.svg";
        if (im->get_isSD())
            icres = ":/img/ic_sd_storage_40px.svg";
        else if (im->get_isUsb())
            icres = ":/img/ic_usb_40px.svg";

        return QIcon(icres);
    }

    return {};
}

QVariant UsbDiskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return tr("Removable disks");
    return {};
}

UsbDisk::UsbDisk():
    QObject()
{
}

UsbDisk::UsbDisk(UsbDisk *d)
{
    set_name(d->get_name());
    set_volumes(d->get_volumes());
    set_size(d->get_size());
    set_sectorSize(d->get_sectorSize());
    set_physicalDevice(d->get_physicalDevice());
    set_isRemovable(d->get_isRemovable());
    set_isSystem(d->get_isSystem());
    set_isUsb(d->get_isUsb());
    set_isSD(d->get_isSD());
}
