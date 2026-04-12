#include "DiskWriter.h"
#include "Utils.h"
#include "Platform.h"

#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QTime>
#include <QScopedPointer>

#include <KTar>
#include <KCompressionDevice>

#include <QRegularExpression>

#include <fcntl.h>
#include <unistd.h>

#ifdef Q_OS_WIN
#include <winioctl.h>
// IOCTL_DISK_SET_DISK_ATTRIBUTES may not be defined in older SDKs
#ifndef IOCTL_DISK_SET_DISK_ATTRIBUTES
#define IOCTL_DISK_SET_DISK_ATTRIBUTES CTL_CODE(IOCTL_DISK_BASE, 0x003D, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
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
    if (QFile::open(_open_osfhandle(reinterpret_cast<intptr_t>(m_fileHandle), 0), flags, AutoCloseHandle))
    {
        // Lock and dismount all volumes on this physical disk.
        // FSCTL_LOCK_VOLUME / FSCTL_DISMOUNT_VOLUME only work on volume handles,
        // NOT on the physical drive handle.
        if (!lockAndDismountVolumes())
        {
            qWarning() << "Failed to lock/dismount volumes on device. Closing handle :(";
            unlockAndCloseVolumes();
            CloseHandle(m_fileHandle);
            m_fileHandle = INVALID_HANDLE_VALUE;
            return false;
        }

        // Set disk offline to prevent Windows PnP from auto-mounting
        // partitions as they appear during the write
        setDiskOffline(true);

        return true;
    }
    else
    {
        qWarning() << "Closing handle :(";
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
    setDiskOffline(false);
    unlockAndCloseVolumes();
#endif

    QFile::close();
}

bool PhysicalDevice::syncToDisk()
{
    if (isOpen())
    {
#if defined(Q_OS_WIN)
        return FlushFileBuffers(m_fileHandle);
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

// Enumerate volume handles that belong to the physical drive, lock and dismount each one.
// FSCTL_LOCK_VOLUME and FSCTL_DISMOUNT_VOLUME only work on VOLUME handles (e.g. \\.\E:),
// NOT on physical drive handles (e.g. \\.\PhysicalDrive3).
bool PhysicalDevice::lockAndDismountVolumes()
{
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
        qWarning() << "FindFirstVolume failed:" << Utils::errorMessageFromCode(GetLastError());
        return true;
    }

    do
    {
        // Remove trailing backslash for CreateFile
        size_t len = wcslen(volumeName);
        if (len > 0 && volumeName[len - 1] == L'\\')
            volumeName[len - 1] = L'\0';

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
            continue;

        // Check which physical disk this volume belongs to
        BYTE buffer[sizeof(VOLUME_DISK_EXTENTS) + sizeof(DISK_EXTENT) * 8] = {};
        DWORD bytesRet = 0;
        BOOL ok = DeviceIoControl(
            hVol,
            IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
            NULL, 0,
            buffer, sizeof(buffer),
            &bytesRet, NULL
        );

        if (!ok)
        {
            CloseHandle(hVol);
            continue;
        }

        auto *extents = reinterpret_cast<VOLUME_DISK_EXTENTS *>(buffer);
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

        qDebug() << "Found volume on disk" << targetDiskNumber << "- locking and dismounting";

        // Allow extended DASD I/O on this volume
        DeviceIoControl(hVol, FSCTL_ALLOW_EXTENDED_DASD_IO, NULL, 0, NULL, 0, &bytesRet, NULL);

        // Lock the volume with retries
        bool locked = false;
        for (int attempt = 0;attempt < 20;attempt++)
        {
            if (DeviceIoControl(hVol, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesRet, NULL))
            {
                locked = true;
                qDebug() << "Locked volume";
                break;
            }
            qDebug() << "FSCTL_LOCK_VOLUME retry" << attempt + 1;
            QThread::msleep(100);
        }

        if (!locked)
        {
            qWarning() << "Failed to lock volume, proceeding anyway";
        }

        // Dismount the volume
        for (int attempt = 0;attempt < 20;attempt++)
        {
            if (DeviceIoControl(hVol, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &bytesRet, NULL))
            {
                qDebug() << "Dismounted volume";
                break;
            }
            qDebug() << "FSCTL_DISMOUNT_VOLUME retry" << attempt + 1;
            QThread::msleep(100);
        }

        // Keep handle open (locked) to prevent Windows from re-mounting
        m_volumeHandles.append(hVol);

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
    // SET_DISK_ATTRIBUTES to mark disk offline prevents Windows PnP from
    // auto-mounting partitions as they appear during image writing.
    // Available since Windows Vista / Server 2008.
    struct {
        DWORD Version;
        BOOLEAN Persist;
        BYTE Reserved1[3];
        DWORD AttributesMask;
        DWORD Attributes;
        DWORD Reserved2[4];
    } attrs = {};

    attrs.Version = sizeof(attrs);
    attrs.Persist = FALSE;  // Don't persist across reboots
    attrs.AttributesMask = 0x00000001; // DISK_ATTRIBUTE_OFFLINE
    attrs.Attributes = offline ? 0x00000001 : 0x00000000;

    DWORD bytesRet;
    if (!DeviceIoControl(m_fileHandle, IOCTL_DISK_SET_DISK_ATTRIBUTES,
                         &attrs, sizeof(attrs), NULL, 0, &bytesRet, NULL))
    {
        qWarning() << "IOCTL_DISK_SET_DISK_ATTRIBUTES" << (offline ? "offline" : "online")
                   << "failed:" << Utils::errorMessageFromCode(GetLastError());
        return false;
    }
    qDebug() << "Disk set" << (offline ? "offline" : "online");
    return true;
}

qint64 PhysicalDevice::writeData(const char *data, qint64 len)
{
    if (m_fileHandle == INVALID_HANDLE_VALUE)
    {
        qWarning() << "Invalid file handle";
        return -1;
    }

    const char* dataPtr = data;
    qint64 totalBytesWritten = 0;
    qint64 bytesLeft = len;
    DWORD bytesWritten = 0;
    BOOL result;
    int retryCount = 0;
    const int maxRetries = 5;
    const DWORD retryDelay = 200;  // 200 ms

    m_lastWriteError = 0;

    while (bytesLeft > 0 && retryCount < maxRetries)
    {
        result = WriteFile(m_fileHandle, dataPtr, static_cast<DWORD>(bytesLeft), &bytesWritten, NULL);
        if (result)
        {
            totalBytesWritten += bytesWritten;
            dataPtr += bytesWritten;
            bytesLeft -= bytesWritten;
            retryCount = 0;
        }
        else
        {
            DWORD errorCode = GetLastError();
            m_lastWriteError = errorCode;
            qWarning() << "WriteFile failed with error code:" << errorCode << Utils::errorMessageFromCode(errorCode);

            // Retry on transient errors.
            // ERROR_INVALID_PARAMETER (87) can happen when Windows PnP
            // briefly interferes with the physical drive.
            if (errorCode == ERROR_IO_PENDING ||
                errorCode == ERROR_LOCK_VIOLATION ||
                errorCode == ERROR_NOT_READY ||
                errorCode == ERROR_INVALID_PARAMETER)
            {
                retryCount++;
                qWarning() << "Retrying write (" << retryCount << "/" << maxRetries << ")";
                QThread::msleep(retryDelay * retryCount); // exponential backoff
            }
            else
            {
                return -1;
            }
        }
    }

    if (bytesLeft > 0)
    {
        qWarning() << "Failed to write all data after" << maxRetries << "retries";
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

DiskWriter::DiskWriter(QObject *parent):
    QObject(parent)
{
}

DiskWriter::~DiskWriter()
{
}

void DiskWriter::writeToRemovableDevice(const QString &filename, UsbDisk *d)
{
    QScopedPointer<UsbDisk> disk(new UsbDisk(d));

    // This code needs to run in a thread and is a blocking process
    emit progress("Checking image file...", 0, 0, 0);

    QScopedPointer<QIODevice> srcDevice(createSourceDevice(filename));

    if (srcDevice.isNull())
    {
        emit error("Failed to open image file " + filename);
        return;
    }

    qDebug() << "QIODevice created";

    //Unmount disk if needed
    emit progress("Unmounting volumes...", 0, 0, 0);

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

#if defined(Q_OS_WIN)
    /*
         * Does not work well
         *
        for (int i = 0; i < disk->get_volumes().size(); i++)
        {
            QString vol = disk->get_volumes().at(i);
            while (vol.endsWith('\\'))
                vol.remove(vol.count() - 1, 1);
            qDebug() << "Unmounting volume: " << vol;

            DWORD bret;
            HANDLE volume = CreateFile(
                        reinterpret_cast<const wchar_t*>(("\\\\.\\" + vol).utf16()),
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL
                        );
            if (volume == INVALID_HANDLE_VALUE)
            {
                qDebug() << Utils::formatErrorMessageFromCode(tr("Failed to open the drive") + " " + disk->get_volumes().at(i), GetLastError());
                continue;
            }
            // Trying to lock the volume but ignore if we failed (such call seems to be required for
            // dismounting the volume on WinXP)
            DeviceIoControl(volume, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bret, NULL);
            if (!DeviceIoControl(volume, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &bret, NULL))
                qDebug() << Utils::formatErrorMessageFromCode(tr("Failed to unmount the drive") + " " + disk->get_volumes().at(i), GetLastError());
            CloseHandle(volume);
        }
        */
    emit progress("Formating drive...", 0, 0, 0);

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

    QProcess proc;
    QByteArray diskpartCmds = "select disk " + nr + "\r\n"
                            "clean\r\n"
                            "rescan\r\n";
    proc.start("diskpart", QStringList());
    proc.waitForStarted();
    proc.write(diskpartCmds);
    proc.closeWriteChannel();
    proc.waitForFinished();

    QByteArray output = proc.readAllStandardError();
    qDebug() << output;
    qDebug() << "Done running diskpart. Exit status code =" << proc.exitCode();

    if (proc.exitCode())
    {
        emit error(tr("Error partitioning: %1").arg(QString(output)));
        return;
    }

    //Windows needs some time to reflect the changes after diskpart has run.
    //6s seems enough
    QThread::sleep(6);
#endif

    const qint64 BLOCK_SIZE = 1 * 1024 * 1024;

    struct BufferDeleter
    {
        void *buffer = nullptr;

        static inline void cleanup(BufferDeleter *b)
        {
#if defined(Q_OS_WIN)
            VirtualFree(b->buffer, BLOCK_SIZE, MEM_DECOMMIT | MEM_RELEASE);
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

    PhysicalDevice phyDev(disk->get_physicalDevice());

    qDebug() << "Opening physical device " << disk->get_physicalDevice();
    if (!phyDev.open(QIODevice::WriteOnly))
    {
        emit error("Unable to open storage device " + disk->get_physicalDevice() + ". " + phyDev.errorString());

        return;
    }

    QElapsedTimer timer;
    timer.start();
    isCancelled = false;

    qint64 totalBytes = srcDevice->size();
    qint64 writtenBytes = 0;

    //BLAKE2b checksum of source
    QString checksumSrc;

    const auto hashLen = 256 / 8; //Blake2b256
    blake2b_state blake2bContext;

    new (&blake2bContext) blake2b_state;
    blake2b_init(&blake2bContext, hashLen);

    emit progress("Writing", writtenBytes, totalBytes, timer.elapsed());

    qDebug() << "Write...";

    while (!srcDevice->atEnd() && !isCancelled)
    {
        auto r = srcDevice->read(static_cast<char *>(buffer->buffer), BLOCK_SIZE);
        if (r <= 0)
            break;

        blake2b_update(&blake2bContext, reinterpret_cast<const uint8_t *>(buffer->buffer), r);

#ifdef Q_OS_WIN
        // Align the size of the read buffer to a multiple of 4096
        qint64 alignedSize = (r + 4095) & ~4095;

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

        writtenBytes += n;

        emit progress("Writing", writtenBytes, totalBytes, timer.elapsed());

#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
        if (!phyDev.syncToDisk())
        {
            QString err;
#ifdef Q_OS_WIN
            err = Utils::errorMessageFromCode(GetLastError());
#else
            err = strerror(errno);
#endif
            qDebug() << "Failed to flush the disk: " << err;
            emit error("Failed to flush the disk: " + err);
            return;
        }
#endif
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
    phyDev.syncToDisk();

    //Compute Blake2b checksum from source
    QByteArray result;
    result.resize(hashLen);
    blake2b_final(&blake2bContext, reinterpret_cast<uint8_t *>(result.data()), hashLen);
    checksumSrc = result.toHex();

    //Verification step

    qDebug() << "Verify...";
    if (totalBytes == 0)
        totalBytes = writtenBytes;
    writtenBytes = 0;
    timer.restart();
    new (&blake2bContext) blake2b_state;
    blake2b_init(&blake2bContext, hashLen);

    emit progress("Verify", writtenBytes, totalBytes, timer.elapsed());

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
        qint64 lenRead = phyDev.read(static_cast<char *>(buffer->buffer),
                                     qMin((qint64) BLOCK_SIZE, (qint64) (totalBytes - writtenBytes) ));
        if (lenRead == -1)
        {
            qDebug() << "Read failed " << phyDev.errorString();
            emit error("Failed to read physical drive (Broken device?). " + phyDev.errorString());
            return;
        }

        blake2b_update(&blake2bContext, reinterpret_cast<const uint8_t *>(buffer->buffer), lenRead);

        writtenBytes += lenRead;

        emit progress("Verify", writtenBytes, totalBytes, timer.elapsed());
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
