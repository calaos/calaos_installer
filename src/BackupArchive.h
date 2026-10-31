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

    // read then write, and backupOpened() on success. A write that fails may leave files of two
    // archives in tempFolder, while the open project stays: it can no longer be sent from there.
    bool openArchive(const QString &zipPath, const QString &tempFolder,
                     BackupArchive::Project &project, QString &error);

    // A project of the server took the temp folder over: it is reloaded from the server, so the
    // guard does not apply, and its files stay there to be sent.
    void onlineProjectOpened();

    // The project was written elsewhere, or another one replaced it. Its files leave the temp
    // folder, which keeps nothing that could pass for the project. Does nothing when not guarded.
    void release();

    // The temp folder stays guarded whatever it holds: a save there would still be lost on exit.
    void projectOpened(const QString &folder);

    // folder: where the save was asked to go, empty for the folder of the project.
    Step step(const QString &folder) const;

    // An untouched backup is the archive itself: it can be sent from the temp folder while the
    // files there are that archive. Anything else goes through Save first, which writes it or
    // refuses it.
    bool sendsAsIs(bool modified, bool readOnly) const;

private:
    enum class Temp
    {
        NotGuarded,
        HoldsTheArchive,
        HoldsSomethingElse,
    };

    bool isTempFolder(const QString &folder) const;

    QString m_tempFolder;
    Temp m_temp = Temp::NotGuarded;
};

#endif // BACKUPARCHIVE_H
