#include "DiskWriter.h"
#include "Utils.h"
#include "Platform.h"

#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QTime>
#include <QScopedPointer>
#include <QThread>

#include <KTar>
#include <KCompressionDevice>

#include <QRegularExpression>

#include <fcntl.h>
#ifndef Q_OS_WIN
#include <unistd.h>
#endif
#include <cerrno>
#include <cstring>

#ifdef Q_OS_WIN
#include <io.h> //_open_osfhandle()
#include <winioctl.h>
// IOCTL_DISK_SET_DISK_ATTRIBUTES may not be defined in older SDKs
#ifndef IOCTL_DISK_SET_DISK_ATTRIBUTES
#define IOCTL_DISK_SET_DISK_ATTRIBUTES CTL_CODE(IOCTL_DISK_BASE, 0x003D, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#endif
#ifndef DISK_ATTRIBUTE_OFFLINE
#define DISK_ATTRIBUTE_OFFLINE 0x0000000000000001
typedef struct _SET_DISK_ATTRIBUTES {
    DWORD Version;
    BOOLEAN Persist;
    BYTE Reserved1[3];
    DWORDLONG Attributes;
    DWORDLONG AttributesMask;
    DWORD Reserved2[4];
} SET_DISK_ATTRIBUTES, *PSET_DISK_ATTRIBUTES;
#endif
#endif

#ifdef Q_OS_LINUX
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusUnixFileDescriptor>
#endif

#ifdef Q_OS_MAC
#include <sys/socket.h>
#include <security/Authorization.h>
#endif

#include <karchive_version.h>

PhysicalDevice::PhysicalDevice(const QString &name):
    QFile(name)
{
}

PhysicalDevice::~PhysicalDevice()
{
    // ~QFileDevice() calls close() non-virtually during base destruction, so our
    // override would never run from there. Call it here so the Windows cleanup
    // (unlock volumes, set disk back online) is guaranteed on every exit path.
    // close() is idempotent, an earlier explicit close() is fine.
    close();
}

// Opens the selected device in WriteOnly mode
bool PhysicalDevice::open(OpenMode flags)
{
    flags = QIODevice::ReadWrite | QIODevice::Unbuffered;

#if defined(Q_OS_WIN)
    // In Windows QFile with write mode uses disposition OPEN_ALWAYS, but WinAPI
    // requires OPEN_EXISTING for physical devices. Therefore we have to use native API.
    m_fileHandle = CreateFile(
        reinterpret_cast<const wchar_t*>(fileName().utf16()),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        //Disable kernel caching of data
        FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING,
        NULL
    );

    if (m_fileHandle == INVALID_HANDLE_VALUE)
    {
        setErrorString(Utils::errorMessageFromCode(GetLastError()));
        return false;
    }

    // Construct QFile around the device handle; close() will now close the handle automatically
    // _O_BINARY is mandatory: without it the CRT fd inherits the default text
    // mode, and the verification pass (which reads back through QFile) would
    // mangle CRLF pairs and stop at the first 0x1A byte of the image.
    if (QFile::open(_open_osfhandle(reinterpret_cast<intptr_t>(m_fileHandle), _O_BINARY), flags, AutoCloseHandle))
    {
        // Volume locking is driven by the caller (relockVolumes() around the
        // diskpart step). Here we only try to set the disk offline so Windows
        // PnP ignores partitions appearing during the write. This is unreliable
        // on removable media, so a failure is logged but NOT fatal: the held
        // volume locks are the primary protection against auto-mounting.
        m_diskSetOffline = setDiskOffline(true);
        if (!m_diskSetOffline)
            qWarning() << "Could not set disk offline on" << fileName()
                       << "- relying on volume locks only";

        return true;
    }
    else
    {
        qWarning() << "QFile::open failed on physical drive handle for" << fileName() << "- closing handle";
        CloseHandle(m_fileHandle);
        m_fileHandle = INVALID_HANDLE_VALUE;
        return false;
    }
#elif defined(Q_OS_LINUX)

#ifdef DOES_NOT_WORK
    //This code does not work: when writing it stops with error: Invalid Arguments
    //Using fsync after every write() call instead
    auto ba = fileName().toLocal8Bit();

    //Open with O_DIRECT to disable kernel caching
    int fd;

    do {
        fd = ::open(ba.constData(), O_WRONLY | O_DIRECT | O_LARGEFILE, 0666);
    } while (fd == -1 && errno == EINTR);

    if (fd == -1)
    {
        setErrorString(QString::fromLocal8Bit(strerror(errno)));
        return false;
    }

    // Construct QFile around the device handle; close() will now close the handle automatically
    if (QFile::open(fd, flags, AutoCloseHandle))
        return true;
    else
    {
        ::close(fd);
        return false;
    }
#endif

    // Try direct open first (works when running as root / via pkexec)
    if (QFile::open(flags))
        return true;

    // Direct open failed (likely Flatpak sandbox). Try udisks2 D-Bus API.
    // udisks2 OpenForRestore returns a writable fd with polkit auth on the host.
    qDebug() << "Direct open failed for" << fileName() << "- trying udisks2 D-Bus...";

    // Convert /dev/sda → sda for udisks2 object path
    m_deviceName = fileName();
    if (m_deviceName.startsWith("/dev/"))
        m_deviceName = m_deviceName.mid(5);

    QString udisksPath = QStringLiteral("/org/freedesktop/UDisks2/block_devices/%1").arg(m_deviceName);

    QDBusConnection bus = QDBusConnection::systemBus();
    if (!bus.isConnected())
    {
        setErrorString("Cannot connect to system D-Bus");
        return false;
    }

    QDBusInterface blockIface("org.freedesktop.UDisks2",
                              udisksPath,
                              "org.freedesktop.UDisks2.Block",
                              bus);

    if (!blockIface.isValid())
    {
        setErrorString("udisks2 block interface not available for " + fileName());
        return false;
    }

    // OpenForRestore(a{sv} options) → returns a unix fd
    QVariantMap options;
    QDBusReply<QDBusUnixFileDescriptor> reply = blockIface.call("OpenForRestore", QVariant::fromValue(options));

    if (!reply.isValid())
    {
        setErrorString("udisks2 OpenForRestore failed: " + reply.error().message());
        return false;
    }

    int fd = ::dup(reply.value().fileDescriptor());
    if (fd < 0)
    {
        setErrorString("Failed to duplicate udisks2 file descriptor");
        return false;
    }

    if (QFile::open(fd, flags, AutoCloseHandle))
    {
        m_useUdisks = true;
        qDebug() << "Opened" << m_deviceName << "via udisks2 D-Bus (write)";
        return true;
    }
    else
    {
        ::close(fd);
        setErrorString("QFile::open failed on udisks2 fd for " + fileName());
        return false;
    }

#elif defined(Q_OS_MAC)
    //auto ret = QFile::open(flags);
    auto ret = authOpen(fileName().toUtf8());
    if (!ret)
    {
        setErrorString("authOpen failed");
        return false;
    }

    if (fcntl(handle(), F_NOCACHE, 1) != 0) //equivalent to Linux's O_DIRECT flag
        qWarning() << "Failed to set fcntl(F_NOCACHE)";

    return ret;
#else
    return false;
#endif
}

void PhysicalDevice::close()
{
#ifdef Q_OS_WIN
    // Order matters: flush pending data, let Windows re-read the freshly written
    // partition table (IOCTL_DISK_UPDATE_PROPERTIES) while the volumes are still
    // locked and the disk still offline, then release the volume locks, bring the
    // disk back online so the mount manager mounts the new volumes, and only then
    // close the physical handle.
    if (m_fileHandle != INVALID_HANDLE_VALUE)
    {
        if (!FlushFileBuffers(m_fileHandle))
        {
            DWORD errorCode = GetLastError();
            qWarning() << "FlushFileBuffers on close failed, error" << errorCode
                       << Utils::errorMessageFromCode(errorCode);
        }

        DWORD bytesRet = 0;
        if (!DeviceIoControl(m_fileHandle, IOCTL_DISK_UPDATE_PROPERTIES,
                             NULL, 0, NULL, 0, &bytesRet, NULL))
        {
            DWORD errorCode = GetLastError();
            qWarning() << "IOCTL_DISK_UPDATE_PROPERTIES failed, error" << errorCode
                       << Utils::errorMessageFromCode(errorCode);
        }

        unlockAndCloseVolumes();

        if (m_diskSetOffline)
        {
            // Failing to bring the disk back online is nasty for the user:
            // once the handle is closed just below there is no way left to
            // recover, so retry a few times before giving up.
            bool backOnline = false;
            for (int attempt = 0;attempt < 3 && !backOnline;attempt++)
            {
                if (attempt > 0)
                    QThread::msleep(500);
                backOnline = setDiskOffline(false);
            }

            if (backOnline)
                m_diskSetOffline = false;
            else
                qWarning() << "Could not set the disk back online after 3 attempts:"
                           << "the disk will stay OFFLINE and unusable until it is"
                           << "unplugged and plugged back in";
        }
    }
    else
    {
        // The physical handle was never opened (or is already closed): still
        // release any volume locks taken before diskpart so no handle leaks.
        unlockAndCloseVolumes();
    }
#endif

    QFile::close();

#ifdef Q_OS_WIN
    // QFile was opened with AutoCloseHandle on the fd from _open_osfhandle, so
    // QFile::close() just closed the underlying Win32 handle. Reset it to avoid
    // any use-after-close in syncToDisk()/DeviceIoControl.
    m_fileHandle = INVALID_HANDLE_VALUE;
#endif
}

bool PhysicalDevice::syncToDisk()
{
    if (isOpen())
    {
#if defined(Q_OS_WIN)
        return m_fileHandle != INVALID_HANDLE_VALUE && FlushFileBuffers(m_fileHandle);
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
        return fsync(handle()) == 0;
#endif
    }

    return false;
}

bool PhysicalDevice::reopenForRead()
{
#if defined(Q_OS_LINUX)
    if (m_useUdisks)
    {
        // Close the write fd from OpenForRestore
        close();

        QString udisksPath = QStringLiteral("/org/freedesktop/UDisks2/block_devices/%1").arg(m_deviceName);

        QDBusConnection bus = QDBusConnection::systemBus();
        if (!bus.isConnected())
        {
            setErrorString("Cannot connect to system D-Bus");
            return false;
        }

        QDBusInterface blockIface("org.freedesktop.UDisks2",
                                  udisksPath,
                                  "org.freedesktop.UDisks2.Block",
                                  bus);

        if (!blockIface.isValid())
        {
            setErrorString("udisks2 block interface not available for " + m_deviceName);
            return false;
        }

        // OpenForBackup(a{sv} options) → returns a readable fd
        QVariantMap options;
        QDBusReply<QDBusUnixFileDescriptor> reply = blockIface.call("OpenForBackup", QVariant::fromValue(options));

        if (!reply.isValid())
        {
            setErrorString("udisks2 OpenForBackup failed: " + reply.error().message());
            return false;
        }

        int fd = ::dup(reply.value().fileDescriptor());
        if (fd < 0)
        {
            setErrorString("Failed to duplicate udisks2 read file descriptor");
            return false;
        }

        if (QFile::open(fd, QIODevice::ReadOnly | QIODevice::Unbuffered, AutoCloseHandle))
        {
            qDebug() << "Reopened" << m_deviceName << "via udisks2 D-Bus (read)";
            return true;
        }
        else
        {
            ::close(fd);
            setErrorString("QFile::open failed on udisks2 read fd");
            return false;
        }
    }
#endif

    // Non-udisks2 path: seek back to 0 (device already open as ReadWrite)
    return seek(0);
}

#if defined(Q_OS_WIN)

// Enumerate volumes that belong to the physical drive, lock and dismount every one
// not already held, and keep the locked handles open: holding an FSCTL_LOCK_VOLUME
// handle open is what actually prevents the mount manager from (re)mounting a
// volume — dismounting alone does not. Called before diskpart (pre-existing
// volumes), after diskpart clean/rescan (volumes that re-appeared), and from
// writeData() when a write fails with ERROR_ACCESS_DENIED because a partition of
// the partly-written image got auto-mounted under us.
// FSCTL_LOCK_VOLUME and FSCTL_DISMOUNT_VOLUME only work on VOLUME handles
// (e.g. \\.\Volume{...}), NOT on physical drive handles (e.g. \\.\PhysicalDrive3).
bool PhysicalDevice::relockVolumes(int *volumesOnDiskOut)
{
    if (volumesOnDiskOut)
        *volumesOnDiskOut = 0;

    // Extract disk number from \\.\PhysicalDriveN
    QRegularExpression re("PHYSICALDRIVE(\\d+)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch m = re.match(fileName());
    if (!m.hasMatch())
    {
        qWarning() << "Cannot parse disk number from" << fileName();
        return true; // no volumes to lock
    }
    DWORD targetDiskNumber = m.captured(1).toULong();

    // Enumerate all volumes and find those on our physical disk
    WCHAR volumeName[MAX_PATH] = {};
    HANDLE hFind = FindFirstVolumeW(volumeName, ARRAYSIZE(volumeName));
    if (hFind == INVALID_HANDLE_VALUE)
    {
        DWORD errorCode = GetLastError();
        qWarning() << "FindFirstVolume failed, error" << errorCode << Utils::errorMessageFromCode(errorCode);
        return true;
    }

    do
    {
        // Remove trailing backslash for CreateFile
        size_t len = wcslen(volumeName);
        if (len > 0 && volumeName[len - 1] == L'\\')
            volumeName[len - 1] = L'\0';

        QString volName = QString::fromWCharArray(volumeName);

        // Already locked and held from a previous call. Held handles are on
        // our disk by construction, so this volume counts as present.
        if (m_volumeHandles.contains(volName))
        {
            if (volumesOnDiskOut)
                (*volumesOnDiskOut)++;
            continue;
        }

        HANDLE hVol = CreateFileW(
            volumeName,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hVol == INVALID_HANDLE_VALUE)
        {
            DWORD errorCode = GetLastError();
            qDebug() << "Cannot open volume" << volName << "for inspection, error"
                     << errorCode << Utils::errorMessageFromCode(errorCode);
            continue;
        }

        // Check which physical disk this volume belongs to. The initial buffer
        // holds 8 extents; a volume can span more (spanned/dynamic volumes), in
        // which case the ioctl fails with ERROR_MORE_DATA and NumberOfDiskExtents
        // holds the required count: grow the buffer and retry.
        DWORD bytesRet = 0;
        DWORD extentIoctlError = ERROR_SUCCESS;
        BOOL ok = FALSE;
        QByteArray extentsBuffer(int(sizeof(VOLUME_DISK_EXTENTS) + sizeof(DISK_EXTENT) * 8), '\0');

        for (int attempt = 0;attempt < 3;attempt++)
        {
            ok = DeviceIoControl(
                hVol,
                IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
                NULL, 0,
                extentsBuffer.data(), static_cast<DWORD>(extentsBuffer.size()),
                &bytesRet, NULL
            );

            if (ok)
                break;

            extentIoctlError = GetLastError();
            if (extentIoctlError != ERROR_MORE_DATA)
                break;

            DWORD needed = reinterpret_cast<VOLUME_DISK_EXTENTS *>(extentsBuffer.data())->NumberOfDiskExtents;
            qDebug() << "Volume" << volName << "has" << needed << "disk extents, growing buffer";
            extentsBuffer.resize(int(sizeof(VOLUME_DISK_EXTENTS) + sizeof(DISK_EXTENT) * qMax<DWORD>(needed, 1)));
        }

        if (!ok)
        {
            // Never skip a volume silently: an uninspected volume stays
            // unlocked, which is exactly the failure mode the held locks
            // are meant to prevent.
            qWarning() << "Cannot determine disk extents of volume" << volName
                       << "error" << extentIoctlError << Utils::errorMessageFromCode(extentIoctlError)
                       << "- volume left unlocked";
            CloseHandle(hVol);
            continue;
        }

        auto *extents = reinterpret_cast<VOLUME_DISK_EXTENTS *>(extentsBuffer.data());
        bool belongsToUs = false;
        for (DWORD i = 0;i < extents->NumberOfDiskExtents;i++)
        {
            if (extents->Extents[i].DiskNumber == targetDiskNumber)
            {
                belongsToUs = true;
                break;
            }
        }

        if (!belongsToUs)
        {
            CloseHandle(hVol);
            continue;
        }

        if (volumesOnDiskOut)
            (*volumesOnDiskOut)++;

        qDebug() << "Found volume" << volName << "on disk" << targetDiskNumber << "- locking and dismounting";

        // Allow extended DASD I/O on this volume
        DeviceIoControl(hVol, FSCTL_ALLOW_EXTENDED_DASD_IO, NULL, 0, NULL, 0, &bytesRet, NULL);

        // Lock the volume with retries
        bool locked = false;
        DWORD lockError = 0;
        for (int attempt = 0;attempt < 20;attempt++)
        {
            if (DeviceIoControl(hVol, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesRet, NULL))
            {
                locked = true;
                qDebug() << "Locked volume" << volName;
                break;
            }
            lockError = GetLastError();
            qDebug() << "FSCTL_LOCK_VOLUME retry" << attempt + 1 << "on" << volName;
            QThread::msleep(100);
        }

        if (!locked)
        {
            // Volume may be genuinely busy: log which one and why, proceed anyway
            qWarning() << "Failed to lock volume" << volName << "error" << lockError
                       << Utils::errorMessageFromCode(lockError) << "- proceeding anyway";
        }

        // Dismount the volume
        for (int attempt = 0;attempt < 20;attempt++)
        {
            if (DeviceIoControl(hVol, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &bytesRet, NULL))
            {
                qDebug() << "Dismounted volume" << volName;
                break;
            }
            qDebug() << "FSCTL_DISMOUNT_VOLUME retry" << attempt + 1 << "on" << volName;
            QThread::msleep(100);
        }

        // Keep handle open (locked) to prevent Windows from re-mounting
        m_volumeHandles.insert(volName, hVol);

    } while (FindNextVolumeW(hFind, volumeName, ARRAYSIZE(volumeName)));

    FindVolumeClose(hFind);
    return true;
}

void PhysicalDevice::unlockAndCloseVolumes()
{
    DWORD bytesRet;
    for (HANDLE hVol : m_volumeHandles)
    {
        DeviceIoControl(hVol, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &bytesRet, NULL);
        CloseHandle(hVol);
    }
    m_volumeHandles.clear();
}

bool PhysicalDevice::setDiskOffline(bool offline)
{
    if (m_fileHandle == INVALID_HANDLE_VALUE)
    {
        qWarning() << "setDiskOffline(" << offline << ") called without an open physical drive handle";
        return false;
    }

    // SET_DISK_ATTRIBUTES to mark disk offline prevents Windows PnP from
    // auto-mounting partitions as they appear during image writing.
    // Available since Windows Vista / Server 2008.
    // Use the SDK layout: Attributes/AttributesMask are 64 bit and Attributes
    // comes first. A hand-rolled struct with 32 bit fields makes sizeof() (and
    // thus Version) wrong, and disk.sys then rejects every call with
    // ERROR_INVALID_PARAMETER.
    SET_DISK_ATTRIBUTES attrs = {};

    attrs.Version = sizeof(attrs);
    attrs.Persist = FALSE;  // Don't persist across reboots
    attrs.AttributesMask = DISK_ATTRIBUTE_OFFLINE;
    attrs.Attributes = offline ? DISK_ATTRIBUTE_OFFLINE : 0ULL;

    DWORD bytesRet;
    if (!DeviceIoControl(m_fileHandle, IOCTL_DISK_SET_DISK_ATTRIBUTES,
                         &attrs, sizeof(attrs), NULL, 0, &bytesRet, NULL))
    {
        DWORD errorCode = GetLastError();
        qWarning() << "IOCTL_DISK_SET_DISK_ATTRIBUTES" << (offline ? "offline" : "online")
                   << "failed, error" << errorCode << Utils::errorMessageFromCode(errorCode);
        return false;
    }
    qDebug() << "Disk set" << (offline ? "offline" : "online");
    return true;
}

qint64 PhysicalDevice::writeData(const char *data, qint64 len)
{
    if (m_fileHandle == INVALID_HANDLE_VALUE)
    {
        qWarning() << "writeData: invalid file handle";
        return -1;
    }

    const char* dataPtr = data;
    qint64 totalBytesWritten = 0;
    qint64 bytesLeft = len;
    DWORD bytesWritten = 0;
    BOOL result;
    int retryCount = 0;
    const int maxRetries = 10;
    const DWORD retryDelay = 200;    // base delay in ms
    const DWORD maxRetryDelay = 600; // per-attempt cap; total retry budget ~5.4s
    const qint64 startOffset = pos();

    m_lastWriteError = 0;

    while (bytesLeft > 0 && retryCount < maxRetries)
    {
        result = WriteFile(m_fileHandle, dataPtr, static_cast<DWORD>(bytesLeft), &bytesWritten, NULL);
        if (result && bytesWritten > 0)
        {
            totalBytesWritten += bytesWritten;
            dataPtr += bytesWritten;
            bytesLeft -= bytesWritten;
            retryCount = 0;
        }
        else if (result)
        {
            // WriteFile "succeeded" but wrote 0 bytes: without bounding this the
            // loop would spin forever with zero progress. Retry a bounded number
            // of times, then fail.
            retryCount++;
            qWarning() << "WriteFile wrote 0 bytes at offset" << (startOffset + totalBytesWritten)
                       << "- retry" << retryCount << "/" << maxRetries;
            QThread::msleep(qMin<DWORD>(retryDelay * retryCount, maxRetryDelay));
        }
        else
        {
            DWORD errorCode = GetLastError();
            m_lastWriteError = errorCode;
            qWarning() << "WriteFile failed at offset" << (startOffset + totalBytesWritten)
                       << "error" << errorCode << Utils::errorMessageFromCode(errorCode);

            // Retry on transient errors.
            // ERROR_INVALID_PARAMETER (87) can happen when Windows PnP
            // briefly interferes with the physical drive.
            // ERROR_ACCESS_DENIED (5), ERROR_SHARING_VIOLATION (32) and
            // ERROR_MEDIA_CHANGED happen when the mount manager auto-mounts a
            // partition of the partly-written image while we are still writing.
            if (errorCode == ERROR_IO_PENDING ||
                errorCode == ERROR_LOCK_VIOLATION ||
                errorCode == ERROR_NOT_READY ||
                errorCode == ERROR_INVALID_PARAMETER ||
                errorCode == ERROR_ACCESS_DENIED ||
                errorCode == ERROR_SHARING_VIOLATION ||
                errorCode == ERROR_MEDIA_CHANGED)
            {
                retryCount++;

                // ACCESS_DENIED means a volume got re-mounted under us:
                // re-lock it to reclaim exclusive access before retrying
                if (errorCode == ERROR_ACCESS_DENIED)
                    relockVolumes();

                qWarning() << "Retrying write at offset" << (startOffset + totalBytesWritten)
                           << "(" << retryCount << "/" << maxRetries << ")";
                QThread::msleep(qMin<DWORD>(retryDelay * retryCount, maxRetryDelay));
            }
            else
            {
                qWarning() << "Non-retryable write error" << errorCode
                           << "at offset" << (startOffset + totalBytesWritten);
                return -1;
            }
        }
    }

    if (bytesLeft > 0)
    {
        qWarning() << "Failed to write all data after" << maxRetries << "retries at offset"
                   << (startOffset + totalBytesWritten) << "- last error" << m_lastWriteError;
        return -1;
    }

    return totalBytesWritten;
}

#endif

#if defined(Q_OS_MAC)
//Code from https://github.com/raspberrypi/rpi-imager/blob/qml/mac/macfile.cpp
bool PhysicalDevice::authOpen(const QByteArray &filename)
{
    int fd = -1;

    QByteArray right = "sys.openfile.readwrite."+filename;
    AuthorizationItem item = {right, 0, nullptr, 0};
    AuthorizationRights rights = {1, &item};
    AuthorizationFlags flags = kAuthorizationFlagInteractionAllowed |
            kAuthorizationFlagExtendRights |
            kAuthorizationFlagPreAuthorize;
    AuthorizationRef authRef;
    if (AuthorizationCreate(&rights, nullptr, flags, &authRef) != 0)
        return false;

    AuthorizationExternalForm externalForm;
    if (AuthorizationMakeExternalForm(authRef, &externalForm) != 0)
    {
        AuthorizationFree(authRef, 0);
        return false;
    }

    const char *cmd = "/usr/libexec/authopen";
    QByteArray mode = QByteArray::number(O_RDWR);
    int pipe[2];
    int stdinpipe[2];
    ::socketpair(AF_UNIX, SOCK_STREAM, 0, pipe);
    ::pipe(stdinpipe);
    pid_t pid = ::fork();
    if (pid == 0)
    {
        // child
        ::close(pipe[0]);
        ::close(stdinpipe[1]);
        ::dup2(pipe[1], STDOUT_FILENO);
        ::dup2(stdinpipe[0], STDIN_FILENO);
        ::execl(cmd, cmd, "-stdoutpipe", "-extauth", "-o", mode.data(), filename.data(), NULL);
        ::exit(-1);
    }
    else
    {
        ::close(pipe[1]);
        ::close(stdinpipe[0]);
        ::write(stdinpipe[1], externalForm.bytes, sizeof(externalForm.bytes));
        ::close(stdinpipe[1]);

        const size_t bufSize = CMSG_SPACE(sizeof(int));
        char buf[bufSize];
        struct iovec io_vec[1];
        io_vec[0].iov_base = buf;
        io_vec[0].iov_len = bufSize;
        const size_t cmsgSize = CMSG_SPACE(sizeof(int));
        char cmsg[cmsgSize];

        struct msghdr msg = {0};
        msg.msg_iov = io_vec;
        msg.msg_iovlen = 1;
        msg.msg_control = cmsg;
        msg.msg_controllen = cmsgSize;

        ssize_t size;
        do {
            size = recvmsg(pipe[0], &msg, 0);
        } while (size == -1 && errno == EINTR);

        qDebug() << "RECEIVED SIZE:" << size;

        if (size > 0) {
            struct cmsghdr *chdr = CMSG_FIRSTHDR(&msg);
            if (chdr && chdr->cmsg_type == SCM_RIGHTS) {
                qDebug() << "SCMRIGHTS";
                fd = *( (int*) (CMSG_DATA(chdr)) );
            }
            else
            {
                qDebug() << "NOT SCMRIGHTS";
            }
        }

        pid_t wpid;
        int status;

        do {
            wpid = ::waitpid(pid, &status, 0);
        } while (wpid == -1 && errno == EINTR);

        if (wpid == -1)
        {
            qDebug() << "waitpid() failed executing authopen";
            return false;
        }
        if (WEXITSTATUS(status))
        {
            qDebug() << "authopen returned failure code" << WEXITSTATUS(status);
            return false;
        }

        qDebug() << "fd received:" << fd;
    }
    AuthorizationFree(authRef, 0);

    return QFile::open(fd, QFile::ReadWrite | QFile::ExistingOnly | QFile::Unbuffered, QFileDevice::AutoCloseHandle);
}
#endif

#if defined(Q_OS_WIN)
// Alignment required by FILE_FLAG_NO_BUFFERING: every transfer length must be
// a multiple of the device sector size. Use the sector size reported by
// drivelist, but fall back to 4096 when it is missing, not a power of two or
// larger than the transfer buffer. Never go below 4096: a multiple of 4096 is
// always a multiple of 512, so 4096 is a safe over-alignment for
// 512-byte-sector drives.
static qint64 sectorAlignment(quint32 sectorSize, qint64 blockSize)
{
    qint64 align = sectorSize;

    if (align <= 0 || (align & (align - 1)) != 0 || align > blockSize)
        align = 4096;

    if (align < 4096)
        align = 4096;

    return align;
}

// diskpart prints to the console using the OEM codepage (CP850/CP437/...),
// not the ANSI codepage QString::fromLocal8Bit() assumes: decode with the
// console codepage so localised output is not mojibake in the log and in the
// user-facing error dialog on non-English Windows.
static QString fromOemCodepage(const QByteArray &ba)
{
    if (ba.isEmpty())
        return QString();

    UINT cp = GetOEMCP();
    int wlen = MultiByteToWideChar(cp, 0, ba.constData(), ba.size(), NULL, 0);
    if (wlen <= 0)
        return QString::fromLocal8Bit(ba);

    QString result(wlen, QChar());
    if (MultiByteToWideChar(cp, 0, ba.constData(), ba.size(),
                            reinterpret_cast<wchar_t *>(result.data()), wlen) <= 0)
        return QString::fromLocal8Bit(ba);
    return result;
}
#endif

// Flush the device with a bounded retry: a flaky USB bridge can fail a single
// FlushFileBuffers/fsync transiently, which must not abort a whole flash. A
// genuinely persistent failure is still reported to the caller as fatal.
// On failure errorOut contains the human-readable message of the last error.
static bool flushWithRetry(PhysicalDevice &phyDev, qint64 offset, QString &errorOut)
{
    const int maxFlushRetries = 3;
    const unsigned long flushRetryDelayMs = 500;

    for (int attempt = 1;attempt <= maxFlushRetries;attempt++)
    {
        if (phyDev.syncToDisk())
            return true;

        // syncToDisk() returns false without making any system call when the
        // device is not open: GetLastError()/errno were not set by it, so
        // reporting them would be stale noise from some earlier call.
        if (!phyDev.isOpen())
        {
            errorOut = QObject::tr("device is not open");
            qWarning() << "Flush failed at offset" << offset << "- device is not open";
            return false;
        }

#if defined(Q_OS_WIN)
        DWORD errorCode = GetLastError();
        errorOut = Utils::errorMessageFromCode(errorCode);
#else
        int errorCode = errno;
        errorOut = QString::fromLocal8Bit(strerror(errorCode));
#endif
        qWarning() << "Flush failed at offset" << offset << "error" << errorCode
                   << errorOut << "- attempt" << attempt << "/" << maxFlushRetries;

        if (attempt < maxFlushRetries)
            QThread::msleep(flushRetryDelayMs * attempt);
    }

    return false;
}

DiskWriter::DiskWriter(QObject *parent):
    QObject(parent)
{
}

DiskWriter::~DiskWriter()
{
}

void DiskWriter::writeToRemovableDevice(const QString &filename, UsbDisk *d)
{
    // Reset the cancel flag before anything else: a cancelWrite() issued
    // during the (long) Windows prep phase below must not be wiped out by a
    // later reset, which would let the write proceed after the user cancelled.
    isCancelled = false;

    QScopedPointer<UsbDisk> disk(new UsbDisk(d));

    // This code needs to run in a thread and is a blocking process
    emit progress(tr("Checking image file..."), 0, 0, 0);

    QScopedPointer<QIODevice> srcDevice(createSourceDevice(filename));

    if (srcDevice.isNull())
    {
        emit error("Failed to open image file " + filename);
        return;
    }

    qDebug() << "QIODevice created";

    //Unmount disk if needed
    emit progress(tr("Unmounting volumes..."), 0, 0, 0);

    //Rescan disks in case mount point changes when opening archives
    {
        auto disks = Platform::enumUsbDisk();
        for (UsbDisk *udisk: disks)
        {
            if (udisk->get_physicalDevice() == disk->get_physicalDevice())
                disk.reset(new UsbDisk(udisk));
        }
        qDeleteAll(disks);
    }

    //Make sure the image can fit on the target disk before destroying anything.
    //A source size of 0 means the image is a compressed stream and the final
    //size is unknown: skip the check, the write loop detects device overflow.
    if (srcDevice->size() > 0 && disk->get_size() > 0 &&
        srcDevice->size() > disk->get_size())
    {
        emit error(tr("Image is too large for this device: image size is %1, device size is %2")
                   .arg(Utils::sizeHuman(srcDevice->size()),
                        Utils::sizeHuman(disk->get_size())));
        return;
    }

    qDebug() << "Unmounting device...";
#if defined(Q_OS_MAC)
    for (QString vol: disk->get_volumes())
    {
        QProcess unmount;
        unmount.start("diskutil", QStringList() << "unmountDisk" << disk->get_physicalDevice());
        unmount.waitForStarted();
        unmount.waitForFinished();
        qDebug() << unmount.readAll();
    }
#elif defined(Q_OS_LINUX)
    // Try direct umount first (works as root / via pkexec)
    bool umountOk = true;
    for (const QString &vol: disk->get_volumes())
    {
        QProcess unmount;
        unmount.start("/bin/umount", QStringList() << vol);
        unmount.waitForStarted();
        unmount.waitForFinished();
        qDebug() << unmount.readAll();
        if (unmount.exitCode() != 0)
            umountOk = false;
    }

    // If direct umount failed, try udisks2 D-Bus (Flatpak sandbox)
    if (!umountOk && !disk->get_volumes().isEmpty())
    {
        qDebug() << "Direct umount failed, trying udisks2 D-Bus...";
        QDBusConnection bus = QDBusConnection::systemBus();
        if (bus.isConnected())
        {
            // Enumerate udisks2 block devices for this drive's partitions
            QString physDev = disk->get_physicalDevice();
            QString devBase = physDev;
            if (devBase.startsWith("/dev/"))
                devBase = devBase.mid(5);

            // Try partitions: sda1, sda2, ... sda9
            for (int i = 1;i <= 9;i++)
            {
                QString partName = devBase + QString::number(i);
                QString partPath = QStringLiteral("/org/freedesktop/UDisks2/block_devices/%1").arg(partName);

                QDBusInterface fsIface("org.freedesktop.UDisks2",
                                       partPath,
                                       "org.freedesktop.UDisks2.Filesystem",
                                       bus);

                if (!fsIface.isValid())
                    continue;

                QVariantMap options;
                QDBusReply<void> reply = fsIface.call("Unmount", QVariant::fromValue(options));
                if (reply.isValid())
                    qDebug() << "Unmounted" << partName << "via udisks2";
                else
                    qDebug() << "udisks2 unmount" << partName << ":" << reply.error().message();
            }
        }
    }
#endif

    PhysicalDevice phyDev(disk->get_physicalDevice());

#if defined(Q_OS_WIN)
    // Lock and dismount every volume currently on the target disk BEFORE diskpart
    // destroys them, and keep the locked handles open for the whole write: a held
    // FSCTL_LOCK_VOLUME handle is what prevents the mount manager from
    // auto-mounting the partitions of the image while it is being written.
    emit progress(tr("Locking volumes..."), 0, 0, 0);
    phyDev.relockVolumes();

    if (isCancelled)
    {
        qDebug() << "Write cancelled by user during preparation";
        emit error(tr("Write cancelled."));
        return;
    }

    emit progress(tr("Formatting drive..."), 0, 0, 0);

    qDebug() << "Format...";

    QRegularExpression windriveregex("\\\\\\\\.\\\\PHYSICALDRIVE([0-9]+)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch m = windriveregex.match(disk->get_physicalDevice());

    if (!m.hasMatch())
    {
        emit error("Invalid disk: " + disk->get_physicalDevice());
        return;
    }

    QByteArray nr = m.captured(1).toLocal8Bit();
    qDebug() << "Formatting Windows drive #" << nr << "(" << disk->get_physicalDevice() << ")";

    // Deliberately drop our own volume locks right before running diskpart:
    // diskpart/VDS takes its own FSCTL_LOCK_VOLUME to clean the volumes, and
    // that lock only succeeds when no other handle to the volume exists, so
    // holding ours here can make "clean" fail with "Access is denied". The
    // lock+dismount above already served its purpose (dismounting forces
    // other applications' handles closed); the anti-remount protection during
    // the write is the post-diskpart relock below plus the
    // relock-on-ACCESS_DENIED retry in writeData().
    phyDev.unlockAndCloseVolumes();

    QProcess proc;
    QByteArray diskpartCmds = "select disk " + nr + "\r\n"
                            "clean\r\n"
                            "rescan\r\n";
    proc.start("diskpart", QStringList());
    if (!proc.waitForStarted(10000))
    {
        qWarning() << "diskpart failed to start:" << proc.errorString();
        emit error(tr("Failed to start diskpart: %1").arg(proc.errorString()));
        return;
    }
    proc.write(diskpartCmds);
    proc.closeWriteChannel();
    if (!proc.waitForFinished(120000))
    {
        qWarning() << "diskpart did not finish within timeout, killing it";
        proc.kill();
        proc.waitForFinished(5000);
        emit error(tr("diskpart timed out while cleaning the disk"));
        return;
    }

    // diskpart reports its errors on STDOUT (not stderr), and its exit code is
    // not reliable when driven through stdin: capture both channels and scan
    // for the error marker as well.
    QString diskpartOutput = fromOemCodepage(proc.readAllStandardOutput()) +
                             fromOemCodepage(proc.readAllStandardError());
    qDebug().noquote() << diskpartOutput;
    qDebug() << "Done running diskpart. Exit status code =" << proc.exitCode();

    if (proc.exitStatus() != QProcess::NormalExit ||
        proc.exitCode() != 0 ||
        diskpartOutput.contains(QLatin1String("DiskPart has encountered an error"), Qt::CaseInsensitive))
    {
        emit error(tr("Error partitioning disk.\ndiskpart output:\n%1").arg(diskpartOutput));
        return;
    }

    // The markers above are localised (non-English Windows prints translated
    // messages), so their absence is only suspicious, not proof either way.
    if (!diskpartOutput.contains(QLatin1String("DiskPart succeeded"), Qt::CaseInsensitive))
        qWarning() << "diskpart output has no recognizable success marker (localised Windows?)";

    //Windows needs some time to reflect the changes after diskpart has run.
    //6s seems enough. Sleep in short slices so a user cancel is noticed
    //within ~100ms instead of only after the full delay.
    for (int i = 0;i < 60 && !isCancelled;i++)
        QThread::msleep(100);

    if (isCancelled)
    {
        qDebug() << "Write cancelled by user during preparation";
        emit error(tr("Write cancelled."));
        return;
    }

    // diskpart clean/rescan invalidated the pre-existing volumes; re-enumerate and
    // lock anything that (re)appeared on the target disk before writing starts,
    // and keep those handles held for the whole write.
    int volumesLeft = 0;
    phyDev.relockVolumes(&volumesLeft);

    if (volumesLeft > 0)
    {
        // NOT fatal: after "clean" the Windows superfloppy heuristic routinely
        // re-surfaces a removable disk as a single RAW volume within the settle
        // delay above (the familiar "You need to format the disk in drive X:"
        // popup), so finding a volume here is expected on a perfectly healthy
        // flash. The relock above locked it, which is the actual protection
        // against the mount manager interfering, and the image write
        // overwrites the partition table anyway.
        qWarning() << "Found" << volumesLeft << "volume(s) on the disk after diskpart clean"
                   << "- volume(s) locked, continuing with the write";
    }
#endif

    const qint64 BLOCK_SIZE = 1 * 1024 * 1024;

    struct BufferDeleter
    {
        void *buffer = nullptr;

        static inline void cleanup(BufferDeleter *b)
        {
#if defined(Q_OS_WIN)
            // MEM_RELEASE requires dwSize == 0 and cannot be combined with
            // MEM_DECOMMIT, otherwise VirtualFree fails with ERROR_INVALID_PARAMETER
            // and the buffer leaks
            if (b->buffer)
                VirtualFree(b->buffer, 0, MEM_RELEASE);
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
            qFreeAligned(b->buffer);
#endif
            delete b;
        }
    };

    QScopedPointer<BufferDeleter, BufferDeleter> buffer(new BufferDeleter());

#if defined(Q_OS_WIN)
    // Using VirtualAlloc so that the buffer was properly aligned (required for
    // direct access to devices and for unbuffered reading/writing)

    buffer->buffer = VirtualAlloc(NULL, BLOCK_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (buffer->buffer == nullptr)
    {
        emit error(Utils::formatErrorMessageFromCode(tr("Failed to allocate memory for buffer:"), GetLastError()));
        return;
    }
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    buffer->buffer = qMallocAligned(BLOCK_SIZE, 4096);
    if (buffer->buffer == nullptr)
    {
        emit error("Failed to allocate memory for buffer.");
        return;
    }
#endif

    if (isCancelled)
    {
        // Cancelled during the prep phase: nothing was opened yet, and
        // ~PhysicalDevice() releases any volume locks taken before diskpart.
        qDebug() << "Write cancelled by user during preparation";
        emit error(tr("Write cancelled."));
        return;
    }

    qDebug() << "Opening physical device " << disk->get_physicalDevice();
    if (!phyDev.open(QIODevice::WriteOnly))
    {
        emit error("Unable to open storage device " + disk->get_physicalDevice() + ". " + phyDev.errorString());

        return;
    }

    QElapsedTimer timer;
    timer.start();

    qint64 totalBytes = srcDevice->size();
    qint64 writtenBytes = 0; //logical image bytes, excludes any sector padding

    //Flush every 128 blocks (128 MB with 1 MB blocks) instead of every block
    const qint64 FLUSH_INTERVAL_BLOCKS = 128;
    qint64 blocksSinceFlush = 0;

#ifdef Q_OS_WIN
    //Physical bytes written to the device, including the sector padding of the
    //final block. Only used to detect writing past the end of the device.
    qint64 paddedBytes = 0;
    const qint64 alignment = sectorAlignment(disk->get_sectorSize(), BLOCK_SIZE);

    // The device's TRUE sector size, validated. 4096 stays the regular
    // transfer alignment, but a device size is only guaranteed to be a
    // multiple of the true sector size (often 512), so the very last write
    // may have to be aligned down to it: otherwise an image that legitimately
    // ends inside the device's last partial 4 KiB would be falsely rejected
    // as too large. Falls back to the transfer alignment (no down-alignment
    // possible) when the reported sector size is unusable.
    qint64 deviceSectorSize = disk->get_sectorSize();
    if (deviceSectorSize <= 0 ||
        (deviceSectorSize & (deviceSectorSize - 1)) != 0 ||
        deviceSectorSize > alignment)
        deviceSectorSize = alignment;

    qDebug() << "Using write alignment of" << alignment
             << "bytes (device sector size:" << disk->get_sectorSize() << ")";
#endif

    //BLAKE2b checksum of source
    QString checksumSrc;

    const auto hashLen = 256 / 8; //Blake2b256
    blake2b_state blake2bContext;

    new (&blake2bContext) blake2b_state;
    blake2b_init(&blake2bContext, hashLen);

    emit progress(tr("Writing"), writtenBytes, totalBytes, timer.elapsed());

    qDebug() << "Write...";

    while (!srcDevice->atEnd() && !isCancelled)
    {
        // Fill the block completely before writing: KCompressionDevice and
        // KLimitedIODevice can legitimately return short reads mid-stream, and
        // padding a short block in the middle of the device would corrupt the
        // image. Only a true EOF may leave the block partially filled.
        qint64 r = 0;
        while (r < BLOCK_SIZE)
        {
            auto chunk = srcDevice->read(static_cast<char *>(buffer->buffer) + r, BLOCK_SIZE - r);
            if (chunk < 0)
            {
                qDebug() << "Failed to read from source: " << srcDevice->errorString();
                emit error(tr("Failed to read image file: %1").arg(srcDevice->errorString()));
                return;
            }
            if (chunk == 0) //EOF
                break;

            r += chunk;
        }

        if (r == 0)
            break;

        blake2b_update(&blake2bContext, reinterpret_cast<const uint8_t *>(buffer->buffer), r);

#ifdef Q_OS_WIN
        // Align the write size to a multiple of the device sector size
        // (required by FILE_FLAG_NO_BUFFERING). The zero padding is physical
        // only, it is not part of the image data.
        qint64 alignedSize = (r + alignment - 1) & ~(alignment - 1);

        // Detect running past the end of the device before WriteFile fails
        // with a cryptic Win32 error (the padded tail can extend past the end
        // of an almost-full disk, and an unknown-size compressed stream can be
        // larger than the disk)
        if (disk->get_size() > 0 && paddedBytes + alignedSize > disk->get_size())
        {
            // The 4 KiB-padded tail overshoots the device end, but the image
            // itself may still fit: the device size is only guaranteed to be
            // a multiple of its true sector size (often 512). Align the final
            // write down to the true sector size instead — still a valid
            // transfer length for FILE_FLAG_NO_BUFFERING — and only fail when
            // even that does not fit.
            qint64 sectorAlignedSize = (r + deviceSectorSize - 1) & ~(deviceSectorSize - 1);
            if (paddedBytes + sectorAlignedSize <= disk->get_size())
            {
                qDebug() << "Aligning final write down to the device sector size:"
                         << sectorAlignedSize << "bytes instead of" << alignedSize;
                alignedSize = sectorAlignedSize;
            }
            else
            {
                qDebug() << "Write of" << sectorAlignedSize << "bytes at offset" << paddedBytes
                         << "would exceed device size" << disk->get_size();
                emit error(tr("Image is too large for this device (device size is %1)")
                           .arg(Utils::sizeHuman(disk->get_size())));
                return;
            }
        }

        // Zero out the buffer beyond the read size
        if (alignedSize != r)
            memset(static_cast<char *>(buffer->buffer) + r, 0, alignedSize - r);
#else
        qint64 alignedSize = r;
#endif

        auto n = phyDev.write(static_cast<char*>(buffer->buffer), alignedSize);
//        qDebug() << n << " bytes written out of " << r << " bytes";
        if (n != alignedSize)
        {
#ifdef Q_OS_WIN
            // Use the error code saved by writeData() — GetLastError() is already
            // clobbered by qWarning/FormatMessage calls inside writeData().
            QString err = Utils::errorMessageFromCode(phyDev.m_lastWriteError);
#else
            QString err = phyDev.errorString();
#endif
            qDebug() << "Failed to write " << alignedSize << "bytes, got " << n << ": " << err;
            qDebug() << "writtenBytes = " << writtenBytes << ", totalBytes = " << totalBytes << ", timer = " << timer.elapsed();
            emit error("Failed to write to " + disk->get_physicalDevice() + ": " + err);
            return;
        }

        writtenBytes += r; //count only real image bytes, not the padding
#ifdef Q_OS_WIN
        paddedBytes += alignedSize;
#endif

        emit progress(tr("Writing"), writtenBytes, totalBytes, timer.elapsed());

        // The device is opened unbuffered (FILE_FLAG_WRITE_THROUGH |
        // FILE_FLAG_NO_BUFFERING on Windows, fsync-per-flush elsewhere), so a
        // flush per 1 MB block only issues a needless SYNCHRONIZE CACHE per MB
        // and kills USB throughput. Flush periodically instead; the mandatory
        // final flush after the loop covers the tail.
        blocksSinceFlush++;
        if (blocksSinceFlush >= FLUSH_INTERVAL_BLOCKS)
        {
            QString flushError;
            if (!flushWithRetry(phyDev, writtenBytes, flushError))
            {
#if defined(Q_OS_MAC)
                // On macOS a flush failure must NOT abort the write: fsync()
                // on the authopen'd raw-device fd has never been exercised
                // for failure here (historically macOS had no flush at all),
                // and a deterministic error such as ENOTSUP would hard-fail
                // every flash after perfectly good writes. The verification
                // pass reads the data back anyway.
                qWarning() << "Ignoring flush failure on macOS:" << flushError;
#else
                emit error(tr("Failed to flush the disk: %1").arg(flushError));
                return;
#endif
            }
            blocksSinceFlush = 0;
        }
    }

    if (isCancelled)
    {
        // Deliberate user cancel: don't fall through to the completion check
        // below, which would misreport it as "Writing failed". Device cleanup
        // (flush, unlock volumes, disk back online) runs in ~PhysicalDevice()
        // on this early return, like on every other exit path.
        qDebug() << "Write cancelled by user";
        emit error(tr("Write cancelled."));
        return;
    }

    if (writtenBytes != totalBytes &&
        totalBytes > 0) //when totalBytes is 0, it can be because the
                        // source is compressed and KArchive can't know the final size.
                        // Do not fail with error in that case
    {
        emit error("Writing failed");
        return;
    }

    qDebug() << "Sync...";
    emit syncing();
    {
        QString flushError;
        if (!flushWithRetry(phyDev, writtenBytes, flushError))
        {
#if defined(Q_OS_MAC)
            // See the periodic flush above: fsync() failure on the authopen'd
            // raw device is unexercised territory on macOS and must not fail
            // an otherwise fully successful write. Verification follows.
            qWarning() << "Ignoring final flush failure on macOS:" << flushError;
#else
            emit error(tr("Failed to flush the disk: %1").arg(flushError));
            return;
#endif
        }
    }

    //Compute Blake2b checksum from source
    QByteArray result;
    result.resize(hashLen);
    blake2b_final(&blake2bContext, reinterpret_cast<uint8_t *>(result.data()), hashLen);
    checksumSrc = result.toHex();

    //Verification step

    qDebug() << "Verify...";
    if (totalBytes == 0)
        totalBytes = writtenBytes; //logical image size, sector padding excluded
    writtenBytes = 0;
    timer.restart();
    new (&blake2bContext) blake2b_state;
    blake2b_init(&blake2bContext, hashLen);

    emit progress(tr("Verify"), writtenBytes, totalBytes, timer.elapsed());

    // Reopen for reading (udisks2: close write fd, open read fd via OpenForBackup)
    if (!phyDev.reopenForRead())
    {
        qDebug() << "reopenForRead failed:" << phyDev.errorString();
        emit error("Failed to reopen drive for verification: " + phyDev.errorString());
        return;
    }

#ifdef Q_OS_LINUX
    /* Make sure we are reading from the drive and not from cache */
    //fcntl(phyDev.handle(), F_SETFL, O_DIRECT | fcntl(phyDev.handle(), F_GETFL));
    posix_fadvise(phyDev.handle(), 0, 0, POSIX_FADV_DONTNEED);
#endif

    while (writtenBytes < totalBytes && !isCancelled)
    {
        qint64 logicalLen = qMin((qint64) BLOCK_SIZE, (qint64) (totalBytes - writtenBytes));

#ifdef Q_OS_WIN
        // The handle is still open with FILE_FLAG_NO_BUFFERING, which requires
        // every read length to be a multiple of the sector size. Round the last
        // partial chunk up (the buffer always has room), but hash and count
        // only the logical bytes so the padding never enters the checksum.
        qint64 lenToRead = (logicalLen + alignment - 1) & ~(alignment - 1);

        // Mirror the end-of-device handling of the write loop: when the
        // 4 KiB-rounded read would run past the device end (the image ends
        // inside the device's last partial 4 KiB), round down to the true
        // sector size instead so the read cannot fail with a device error.
        if (disk->get_size() > 0 && writtenBytes + lenToRead > disk->get_size())
            lenToRead = (logicalLen + deviceSectorSize - 1) & ~(deviceSectorSize - 1);
#else
        qint64 lenToRead = logicalLen;
#endif

        qint64 lenRead = phyDev.read(static_cast<char *>(buffer->buffer), lenToRead);
        if (lenRead <= 0)
        {
            qDebug() << "Read failed " << phyDev.errorString();
            emit error("Failed to read physical drive (Broken device?). " + phyDev.errorString());
            return;
        }

        qint64 lenUsed = qMin(lenRead, logicalLen);

        blake2b_update(&blake2bContext, reinterpret_cast<const uint8_t *>(buffer->buffer), lenUsed);

        writtenBytes += lenUsed;

        emit progress(tr("Verify"), writtenBytes, totalBytes, timer.elapsed());
    }

    if (isCancelled)
    {
        // Cancelling mid-verification leaves a partial device hash; comparing
        // it would misreport a checksum mismatch ("Is your USB drive broken?").
        qDebug() << "Verification cancelled by user";
        emit error(tr("Write cancelled."));
        return;
    }

    //Compute Blake2b checksum from device
    result = QByteArray();
    result.resize(hashLen);
    blake2b_final(&blake2bContext, reinterpret_cast<uint8_t *>(result.data()), hashLen);

    qDebug() << "Checksums: src(" << checksumSrc << ") drive(" << result.toHex() << ")";

    if (checksumSrc != result.toHex())
    {
        qDebug() << "Cheksum does not match!";
        emit error("Written data verification failed. Is your USB drive broken?");
        return;
    }

    // On Windows this flushes, refreshes the OS view of the new partition table,
    // releases the volume locks and puts the disk back online (see close()).
    // Error paths rely on ~PhysicalDevice() running the same idempotent cleanup.
    qDebug() << "Closing device...";
    phyDev.close();

    emit finished();
}

void DiskWriter::cancelWrite()
{
    isCancelled = true;
}

QIODevice *DiskWriter::createSourceDevice(QString filename)
{
    //depending of image extension type, we may need to Untar or only uncompress the archive.
    //The file can also be uncompressed

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filename, QMimeDatabase::MatchExtension);

    if (mime.inherits(QStringLiteral("application/x-compressed-tar")) ||
        mime.inherits(QStringLiteral("application/x-bzip-compressed-tar")) ||
        mime.inherits(QStringLiteral("application/x-lzma-compressed-tar")) ||
        mime.inherits(QStringLiteral("application/x-xz-compressed-tar")) ||
        mime.inherits(QStringLiteral("application/x-zstd-compressed-tar")) ||
        mime.inherits(QStringLiteral("application/x-tar")))
    {
        //compressed tar
        auto tarFile = new KTar(filename);

        qDebug() << "Open archive: " << filename;
        if (!tarFile->open(QIODevice::ReadOnly))
        {
            emit error("Failed to open image archive " + filename);
            qDebug() << "Error: " << tarFile->errorString();
            return nullptr;
        }

        qDebug() << "Archive contains:";
        //search for the image file inside the tar
        auto dir = tarFile->directory();

        qDebug() << dir->entries();

        //get the first matching file from the archive
        QString compfname;
        if (dir->entries().count() == 1)
            compfname = dir->entries().at(0);
        else
        {
            for (const auto &entry: dir->entries())
            {
                if (entry.endsWith(".hddimg") ||
                    entry.endsWith(".rpi-sdimg") ||
                    entry.endsWith("sdimg") ||
                    entry.endsWith(".img") ||
                    entry.endsWith(".direct") ||
                    entry.endsWith(".wic"))
                {
                    compfname = entry;
                    break;
                }
            }
        }

        qDebug() << "Using archive entry: " << compfname;

        auto kfile = dir->file(compfname);
        if (!kfile)
        {
            emit error("Unable to find a valid image file in archive " + filename);
            return nullptr;
        }

        //Get the corresponding compressed QIODevice to read from
        return kfile->createDevice();
    }
    else if (mime.inherits(QStringLiteral("application/x-gzip")) ||
             mime.inherits(QStringLiteral("application/x-bzip")) ||
             mime.inherits(QStringLiteral("application/x-lzma")) ||
             mime.inherits(QStringLiteral("application/x-xz")) ||
             mime.inherits(QStringLiteral("application/zstd")) ||
             mime.inherits(QStringLiteral("application/x-zstd")) ||
             filename.endsWith(".zst") ||
             filename.endsWith(".zstd"))
    {
        auto comp = KCompressionDevice::None;
        if (mime.inherits(QStringLiteral("application/x-gzip")))
            comp = KCompressionDevice::GZip;
        else if (mime.inherits(QStringLiteral("application/x-bzip")))
            comp = KCompressionDevice::BZip2;
        else if (mime.inherits(QStringLiteral("application/x-xz")) ||
                 mime.inherits(QStringLiteral("application/x-lzma")))
            comp = KCompressionDevice::Xz;

#if KARCHIVE_VERSION >= ((5<<16)|(82<<8)|(0))
        else if (mime.inherits(QStringLiteral("application/x-zstd")) ||
                 mime.inherits(QStringLiteral("application/zstd")) ||
                 filename.endsWith(".zst") ||
                 filename.endsWith(".zstd"))
            comp = KCompressionDevice::Zstd;
#endif

        //non tar file, only compressed
        auto compDevice = new KCompressionDevice(filename, comp);

        qDebug() << "Open archive: " << filename;
        if (!compDevice->open(QIODevice::ReadOnly))
        {
            emit error("Failed to open image archive " + filename);
            qDebug() << "Failed:" << compDevice->errorString();
            return nullptr;
        }

        return compDevice;
    }
    else
    {
        //Raw image
        auto stdFile = new QFile(filename);

        qDebug() << "Open archive: " << filename;
        if (!stdFile->open(QIODevice::ReadOnly))
        {
            emit error("Failed to open image file " + filename);
            qDebug() << "Failed:" << stdFile->errorString();
            return nullptr;
        }

        return stdFile;
    }
}
