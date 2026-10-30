#ifndef BACKUPARCHIVE_H
#define BACKUPARCHIVE_H

#include <QByteArray>
#include <QCoreApplication>
#include <QString>

// Reads a calaos-backup/2 zip, or a zip of a project folder, as an installer project.
// The archive is not trusted: the limits are the ones of the web app's restore.
class BackupArchive
{
    Q_DECLARE_TR_FUNCTIONS(BackupArchive)

public:
    static constexpr qint64 kMaxArchiveSize = 8 * 1024 * 1024;
    static constexpr qint64 kMaxEntrySize = 2 * 1024 * 1024;
    static constexpr int kMaxEntries = 64;

    struct Project
    {
        QByteArray io;
        QByteArray rules;
        // Its content stays in the archive: the installer has no use for those secrets.
        bool hasLocalConfig = false;
    };

    static bool read(const QString &zipPath, Project &project, QString &error);

    // Both files or neither, as far as a rename allows: dir may hold the open project.
    static bool write(const Project &project, const QString &dir, QString &error);
};

#endif // BACKUPARCHIVE_H
