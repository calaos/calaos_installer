#ifndef DISKWRITER_H
#define DISKWRITER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QFile>
#include <QHash>

#include <atomic>

#include "UsbDisk.h"

#include "BLAKE2/ref/blake2.h"

#if defined(Q_OS_WIN)
#include <qt_windows.h>
#endif

class PhysicalDevice : public QFile
{
    Q_OBJECT
public:
    PhysicalDevice(const QString &name);
    virtual ~PhysicalDevice();

    // Opens the selected device in WriteOnly mode, flags is ignored
    virtual bool open(OpenMode flags) override;
    void close() override;

    bool syncToDisk();

    // Reopen the device for reading (used after writing for verification).
    // On Linux with udisks2, this closes the write fd and opens via OpenForBackup.
    bool reopenForRead();

#if defined(Q_OS_WIN)
    // Last WriteFile error code, saved for caller to retrieve
    DWORD m_lastWriteError = 0;

    // Enumerate volumes belonging to this physical disk and lock/dismount any
    // that are not already held. Keeps every locked volume handle open until
    // close(). Safe to call repeatedly (before diskpart, after diskpart
    // clean/rescan, and mid-write when a volume gets re-mounted under us).
    // If volumesOnDiskOut is non-null it receives the number of volumes found
    // on the disk (newly locked + already held).
    bool relockVolumes(int *volumesOnDiskOut = nullptr);

    // Release and close every held volume lock. Called from close(), and by
    // DiskWriter right before running diskpart: FSCTL_LOCK_VOLUME (which
    // diskpart/VDS takes itself to clean a volume) only succeeds when no
    // other handle to the volume exists, so our own held locks would make
    // "clean" fail.
    void unlockAndCloseVolumes();
#endif

protected:
#if defined(Q_OS_LINUX)
    bool m_useUdisks = false;
    QString m_deviceName; // e.g. "sda"
#endif
#if defined(Q_OS_WIN)
    HANDLE m_fileHandle = INVALID_HANDLE_VALUE;
    // Locked volume handles, keyed by volume GUID path so relockVolumes() can
    // tell which volumes are already held
    QHash<QString, HANDLE> m_volumeHandles;
    // Whether IOCTL_DISK_SET_DISK_ATTRIBUTES offline succeeded (it is unreliable
    // on removable media; the volume locks are the primary protection)
    bool m_diskSetOffline = false;

    bool setDiskOffline(bool offline);

    virtual qint64 writeData(const char *data, qint64 len) override;
#endif

#if defined(Q_OS_MAC)
    /* Prevent that Qt thinks /dev/rdisk does not permit seeks because it does not report size */
    virtual bool isSequential() const override { return false; }

    bool authOpen(const QByteArray &filename);
#endif
};

class DiskWriter: public QObject
{
    Q_OBJECT

public:
    DiskWriter(QObject *parent = 0);
    virtual ~DiskWriter();

public slots:
    void writeToRemovableDevice(const QString &filename, UsbDisk *disk);
    void cancelWrite();

signals:
    void progress(QString status, qint64 bytesWritten, qint64 bytesTotal, qint64 elapsedMs);
    void syncing();
    void finished();
    void error(const QString &message);

protected:
    std::atomic<bool> isCancelled {false};

    QIODevice *createSourceDevice(QString filename);
};

#endif // DISKWRITER_H
