#include "CalaosImage.h"
#include "Utils.h"

FilterImageModel::FilterImageModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

void FilterImageModel::setFilterType(bool onlystable)
{
    filter_all = !onlystable;
    invalidate();
}

void FilterImageModel::setFilterMachine(QString machine)
{
    filter_machine = machine;
    invalidate();
}

QModelIndex FilterImageModel::indexToSource(const QModelIndex &idx) const
{
    return mapToSource(idx);
}

QModelIndex FilterImageModel::indexFromSource(const QModelIndex &idx) const
{
    return mapFromSource(idx);
}

bool FilterImageModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)

    if (filter_all && filter_machine.isEmpty())
        return true;

    CalaosImageModel *model = qobject_cast<CalaosImageModel *>(sourceModel());
    CalaosImage *item = model->itemAt(source_row);
    if (!item)
        return false;

    if (!filter_all && item->get_type() != "stable")
        return false;

    if (!filter_machine.isEmpty() && filter_machine != item->get_machine())
        return false;

    return true;
}

bool FilterImageModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    CalaosImageModel *model = qobject_cast<CalaosImageModel *>(sourceModel());

    if (left.row() < 0 ||
        left.row() >= model->rowCount() ||
        right.row() < 0 ||
        right.row() >= model->rowCount())
        return false;

    const auto &l = model->itemAt(left.row());
    const auto &r = model->itemAt(right.row());

    return l->get_releaseDate() > r->get_releaseDate();
}

CalaosImageModel::CalaosImageModel(QObject *parent):
    QAbstractTableModel(parent)
{
}

void CalaosImageModel::loadJson(const QJsonArray &arr)
{
    beginResetModel();
    qDeleteAll(m_items);
    m_items.clear();

    for (const QJsonValue &value: arr)
    {
        auto it = new CalaosImage();
        it->load(value.toObject());
        m_items.append(it);
        machines.append(it->get_machine());
    }

    machines.removeDuplicates();
    endResetModel();
}

const CalaosImage *CalaosImageModel::itemAt(int rowidx) const
{
    if (rowidx < 0 || rowidx >= m_items.size())
            return nullptr;
        return m_items.at(rowidx);
}

CalaosImage *CalaosImageModel::itemAt(int rowidx)
{
    if (rowidx < 0 || rowidx >= m_items.size())
            return nullptr;
    return m_items.at(rowidx);
}

void CalaosImageModel::clear()
{
    beginResetModel();
    qDeleteAll(m_items);
    m_items.clear();
    endResetModel();
}

QStringList CalaosImageModel::getMachines()
{
    return machines;
}

int CalaosImageModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int CalaosImageModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant CalaosImageModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= m_items.size())
        return {};

    auto *im = m_items.at(index.row());

    if (role == Qt::DisplayRole ||
        role == Qt::EditRole)
    {
        return QStringLiteral("%1 [%2] (%3)")
                .arg(im->get_version())
                .arg(im->get_releaseDate().toString("dd/MM/yyyy"))
                .arg(Utils::sizeHuman(im->get_filesize()));
    }

    return {};
}

QVariant CalaosImageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return tr("Image version");
    return {};
}

CalaosImage::CalaosImage():
    QObject()
{
}

void CalaosImage::load(const QJsonObject obj)
{
    set_type(obj["release_type"].toString());
    set_machine(obj["machine"].toString());
    set_url(obj["url"].toString());
    set_version(obj["version"].toString());
    set_releaseDate(QDateTime::fromString(obj["release_date"].toString(), Qt::ISODate));
    set_filesize(obj["filesize"].toDouble());
    set_checksum(obj["hash_blake2b"].toString());
}
