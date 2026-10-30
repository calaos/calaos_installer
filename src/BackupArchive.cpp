#include "BackupArchive.h"

#include <KZip>
#include <KZipFileEntry>

#include <QFileInfo>
#include <QHash>
#include <QList>
#include <QSaveFile>
#include <QStringList>

#include <memory>

namespace
{

const QString kIoFile = QStringLiteral("io.xml");
const QString kRulesFile = QStringLiteral("rules.xml");
const QString kLocalConfigFile = QStringLiteral("local_config.xml");

// Only fixed names are ever written, so this cannot escape the project folder: an archive
// that tries is simply not a backup.
bool isTraversal(const QString &path)
{
    const QString unixPath = QString(path).replace(QLatin1Char('\\'), QLatin1Char('/'));

    return unixPath.startsWith(QLatin1Char('/'))
           || (unixPath.size() >= 2 && unixPath.at(1) == QLatin1Char(':'))
           || unixPath.split(QLatin1Char('/')).contains(QStringLiteral(".."));
}

// The file name when path is at the root or under one folder, the layouts of a backup and
// of a zipped project. Empty for anything else, macOS' __MACOSX and dot files included.
QString projectFileName(const QString &path)
{
    QStringList parts = path.split(QLatin1Char('/'), Qt::SkipEmptyParts);
    parts.removeAll(QStringLiteral("."));

    if (parts.isEmpty() || parts.size() > 2 || parts.first() == QLatin1String("__MACOSX"))
        return QString();

    for (const QString &part : std::as_const(parts))
    {
        if (part.startsWith(QLatin1Char('.')))
            return QString();
    }

    return parts.last();
}

bool readEntry(const KZipFileEntry *file, QByteArray &data, QString &error)
{
    // Not KZipFileEntry::data(): it inflates everything, and size() is only what the zip claims.
    std::unique_ptr<QIODevice> device(file->createDevice());
    if (!device)
    {
        error = BackupArchive::tr("%1 cannot be read from this archive.").arg(file->path());
        return false;
    }

    data = device->read(BackupArchive::kMaxEntrySize + 1);

    if (data.size() > BackupArchive::kMaxEntrySize)
    {
        error = BackupArchive::tr("%1 is larger than the 2 MiB allowed in a backup.").arg(file->path());
        return false;
    }
    if (data.size() != file->size())
    {
        error = BackupArchive::tr("%1 is damaged in this archive.").arg(file->path());
        return false;
    }

    return true;
}

} // namespace

bool BackupArchive::read(const QString &zipPath, Project &project, QString &error)
{
    // KZip indexes the whole archive on open: bound that before it starts.
    if (QFileInfo(zipPath).size() > kMaxArchiveSize)
    {
        error = tr("This archive is larger than the 8 MiB allowed for a backup.");
        return false;
    }

    KZip zip(zipPath);
    if (!zip.open(QIODevice::ReadOnly))
    {
        error = tr("This file cannot be read as a zip archive.");
        return false;
    }

    QHash<QString, const KZipFileEntry *> found;
    QList<const KArchiveDirectory *> pending{zip.directory()};
    int entries = 0;

    while (!pending.isEmpty())
    {
        const KArchiveDirectory *directory = pending.takeLast();

        const QStringList names = directory->entries();
        for (const QString &name : names)
        {
            if (++entries > kMaxEntries)
            {
                error = tr("This archive holds more than the %1 entries allowed in a backup.")
                            .arg(kMaxEntries);
                return false;
            }

            const KArchiveEntry *entry = directory->entry(name);
            if (entry && entry->isDirectory())
            {
                if (isTraversal(name))
                {
                    error = tr("This archive is refused: it has a path outside its own folder (%1).")
                                .arg(name);
                    return false;
                }
                pending.append(static_cast<const KArchiveDirectory *>(entry));
                continue;
            }

            const auto *file = dynamic_cast<const KZipFileEntry *>(entry);
            if (!file)
                continue;

            // The tree holds names KZip has already tidied up: path() is the one in the zip.
            if (isTraversal(file->path()))
            {
                error = tr("This archive is refused: it has a path outside its own folder (%1).")
                            .arg(file->path());
                return false;
            }
            if (file->size() > kMaxEntrySize)
            {
                error = tr("%1 is larger than the 2 MiB allowed in a backup.").arg(file->path());
                return false;
            }

            const QString fileName = projectFileName(file->path());
            if (fileName != kIoFile && fileName != kRulesFile && fileName != kLocalConfigFile)
                continue;

            // Only across folders: KZip does not let a second entry of a path reach this tree.
            if (found.contains(fileName))
            {
                error = tr("This archive holds %1 twice: there is no telling which one to open.")
                            .arg(fileName);
                return false;
            }
            found.insert(fileName, file);
        }
    }

    QStringList missing;
    if (!found.contains(kIoFile))
        missing << kIoFile;
    if (!found.contains(kRulesFile))
        missing << kRulesFile;
    if (!missing.isEmpty())
    {
        error = tr("This archive is not a Calaos backup: %1 is missing.")
                    .arg(missing.join(QStringLiteral(", ")));
        return false;
    }

    Project opened;
    if (!readEntry(found.value(kIoFile), opened.io, error)
        || !readEntry(found.value(kRulesFile), opened.rules, error))
    {
        return false;
    }
    opened.hasLocalConfig = found.contains(kLocalConfigFile);

    project = opened;
    return true;
}

bool BackupArchive::write(const Project &project, const QString &dir, QString &error)
{
    QSaveFile io(dir + QLatin1Char('/') + kIoFile);
    QSaveFile rules(dir + QLatin1Char('/') + kRulesFile);

    const bool written = io.open(QIODevice::WriteOnly) && rules.open(QIODevice::WriteOnly)
                         && io.write(project.io) == project.io.size()
                         && rules.write(project.rules) == project.rules.size()
                         && io.commit() && rules.commit();

    if (!written)
        error = tr("The project could not be written to %1.").arg(dir);

    return written;
}
