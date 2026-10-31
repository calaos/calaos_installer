#include "BackupArchive.h"

#include <KZip>
#include <KZipFileEntry>

#include <QDir>
#include <QFileInfo>
#include <QHash>
#include <QList>
#include <QSaveFile>
#include <QStringList>
#include <QXmlStreamReader>

#include <memory>

namespace
{

const QString kIoFile = QStringLiteral("io.xml");
const QString kRulesFile = QStringLiteral("rules.xml");
const QString kLocalConfigFile = QStringLiteral("local_config.xml");

QString toUnixPath(const QString &path)
{
    return QString(path).replace(QLatin1Char('\\'), QLatin1Char('/'));
}

// Only fixed names are ever written, so this cannot escape the project folder: an archive
// that tries is simply not a backup.
bool isTraversal(const QString &path)
{
    const QString unixPath = toUnixPath(path);

    return unixPath.startsWith(QLatin1Char('/'))
           || (unixPath.size() >= 2 && unixPath.at(1) == QLatin1Char(':'))
           || unixPath.split(QLatin1Char('/')).contains(QStringLiteral(".."));
}

struct ProjectPath
{
    QString folder;
    QString fileName;
};

// A file name when path is at the root or under one folder, the layouts of a backup and of a
// zipped project. Empty for anything else, macOS' __MACOSX and dot files included.
ProjectPath projectPath(const QString &path)
{
    QStringList parts = toUnixPath(path).split(QLatin1Char('/'), Qt::SkipEmptyParts);
    parts.removeAll(QStringLiteral("."));

    if (parts.isEmpty() || parts.size() > 2 || parts.first() == QLatin1String("__MACOSX"))
        return {};

    for (const QString &part : std::as_const(parts))
    {
        if (part.startsWith(QLatin1Char('.')))
            return {};
    }

    return {parts.size() == 2 ? parts.first() : QString(), parts.last()};
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

// What Load() needs to see, as it cannot be undone once it runs: its reader of rules.xml gives
// no rules at all for another root, and the server only knows these prefixed names.
bool isProjectXml(const QByteArray &data, const QString &root, const QString &onlyChild = QString())
{
    QXmlStreamReader xml(data);
    int depth = 0;

    while (!xml.atEnd())
    {
        xml.readNext();

        if (xml.isEndElement())
            --depth;
        if (!xml.isStartElement())
            continue;

        ++depth;
        if ((depth == 1 && xml.qualifiedName() != root)
            || (depth == 2 && !onlyChild.isEmpty() && xml.qualifiedName() != onlyChild))
        {
            return false;
        }
    }

    return !xml.hasError();
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
    QString folder;
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

            const ProjectPath projectFile = projectPath(file->path());
            const QString &fileName = projectFile.fileName;
            if (fileName != kIoFile && fileName != kRulesFile && fileName != kLocalConfigFile)
                continue;

            // Only across folders: of two entries with one cleaned path, KZip lets at most the
            // first reach this tree.
            if (found.contains(fileName))
            {
                error = tr("This archive holds %1 twice: there is no telling which one to open.")
                            .arg(fileName);
                return false;
            }
            // Mixing them would send half of a project and half of another to the box.
            if (!found.isEmpty() && projectFile.folder != folder)
            {
                error = tr("This archive holds two project folders: there is no telling which "
                           "one to open.");
                return false;
            }
            folder = projectFile.folder;
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

    const bool ioValid = isProjectXml(opened.io, QStringLiteral("calaos:ioconfig"),
                                      QStringLiteral("calaos:home"));
    if (!ioValid || !isProjectXml(opened.rules, QStringLiteral("calaos:rules")))
    {
        error = tr("%1 in this archive is not a valid Calaos project file.")
                    .arg(ioValid ? kRulesFile : kIoFile);
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

void BackupSaveGuard::backupOpened(const QString &tempFolder)
{
    m_tempFolder = tempFolder;
    m_temp = Temp::HoldsTheArchive;
}

bool BackupSaveGuard::openArchive(const QString &zipPath, const QString &tempFolder,
                                  BackupArchive::Project &project, QString &error)
{
    // The temp folder may hold the open project: only touch it once the archive is known good.
    if (!BackupArchive::read(zipPath, project, error))
        return false;

    if (!BackupArchive::write(project, tempFolder, error))
    {
        if (m_temp == Temp::HoldsTheArchive)
            m_temp = Temp::HoldsSomethingElse;
        return false;
    }

    backupOpened(tempFolder);
    return true;
}

void BackupSaveGuard::onlineProjectOpened()
{
    m_temp = Temp::NotGuarded;
}

void BackupSaveGuard::release()
{
    if (m_temp == Temp::NotGuarded)
        return;

    QDir temp(m_tempFolder);
    temp.remove(kIoFile);
    temp.remove(kRulesFile);
    m_temp = Temp::NotGuarded;
}

void BackupSaveGuard::projectOpened(const QString &folder)
{
    if (!isTempFolder(folder))
        release();
    else if (m_temp == Temp::NotGuarded)
        m_temp = Temp::HoldsSomethingElse;
}

BackupSaveGuard::Step BackupSaveGuard::step(const QString &folder) const
{
    if (m_temp == Temp::NotGuarded)
        return Step::Write;
    if (folder.isEmpty())
        return Step::ChooseFolder;

    return isTempFolder(folder) ? Step::RefuseFolder : Step::Write;
}

bool BackupSaveGuard::sendsAsIs(bool modified, bool readOnly) const
{
    return m_temp == Temp::HoldsTheArchive && !modified && !readOnly;
}

bool BackupSaveGuard::isTempFolder(const QString &folder) const
{
    const QString canonical = QFileInfo(folder).canonicalFilePath();
    return canonical == QFileInfo(m_tempFolder).canonicalFilePath() && !canonical.isEmpty();
}
