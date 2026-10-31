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

// A backup opens in the temp folder, which is deleted on exit: saving there would lose the
// project without a word, so it has to reach a folder of the user's first.
class BackupSaveGuard
{
public:
    enum class Step
    {
        Write,
        ChooseFolder,
        RefuseFolder,
    };

    void backupOpened(const QString &tempFolder);

    // The project was written elsewhere, or another one replaced it. An online project lives in
    // the same temp folder: it is reloaded from the server, so the guard does not apply.
    void release();

    // folder: where the save was asked to go, empty for the folder of the project.
    Step step(const QString &folder) const;

private:
    QString m_tempFolder;
};

#endif // BACKUPARCHIVE_H
