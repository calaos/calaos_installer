#include "DiskWriter.h"
#include "Utils.h"

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

#include <KF5/karchive_version.h>

PhysicalDevice::PhysicalDevice(const QString &name):
    QFile(name)
{
}

// Opens the selected device in WriteOnly mode
bool PhysicalDevice::open(OpenMode flags)
{
    flags = QIODevice::ReadWrite | QIODevice::Unbuffered;

#if defined(Q_OS_WIN)
    DWORD bret;

    // In Windows QFile with write mode uses disposition OPEN_ALWAYS, but WinAPI
    // requires OPEN_EXISTING for physical devices. Therefore we have to use native API.
    m_fileHandle = CreateFile(
        reinterpret_cast<const wchar_t*>(fileName().utf16()),
        GENERIC_WRITE,
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

    // Lock the opened device
    if (!DeviceIoControl(m_fileHandle, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bret, NULL))
    {
        setErrorString(Utils::formatErrorMessageFromCode("Could not acquire lock:", GetLastError()));
        return false;
    }

    // Construct QFile around the device handle; close() will now close the handle automatically
    if (QFile::open(_open_osfhandle(reinterpret_cast<intptr_t>(m_fileHandle), 0), flags, AutoCloseHandle))
        return true;
    else
    {
        CloseHandle(m_fileHandle);
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

    return QFile::open(flags);

#elif defined(Q_OS_MAC)
    auto ret = QFile::open(flags);
    if (fcntl(handle(), F_NOCACHE, 1) != 0) //equivalent to Linux's O_DIRECT flag
        qWarning() << "Failed to set fcntl(F_NOCACHE)";

    return ret;
#else
    return false;
#endif
}

void PhysicalDevice::syncToDisk()
{
    if (isOpen())
    {
#if defined(Q_OS_WIN)
        FlushFileBuffers(m_fileHandle);
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
        fsync(handle());
#endif
    }
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

    //TODO: rescan disk if mount points have changed

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
    for (QString vol: disk->get_volumes())
    {
        QProcess unmount;
        unmount.start("/bin/umount", QStringList() << vol);
        unmount.waitForStarted();
        unmount.waitForFinished();
        qDebug() << unmount.readAll();
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
    //2s seems enough
    QThread::sleep(2);
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

    emit progress("Writing", writtenBytes, totalBytes, timer.elapsed());

    qDebug() << "Write...";

    while (!srcDevice->atEnd() && !isCancelled)
    {
        auto r = srcDevice->read(static_cast<char*>(buffer->buffer), BLOCK_SIZE);
        if (r <= 0)
            break;

        auto n = phyDev.write(static_cast<char*>(buffer->buffer), r);
//        qDebug() << n << " bytes written out of " << r << " bytes";
        if (r != n)
        {
            qDebug() << "Failed to write " << r << "bytes, got " << n << ": " << phyDev.errorString();
            emit error("Failed to write to " + disk->get_physicalDevice() + ": " + phyDev.errorString());
            return;
        }

        writtenBytes += n;

        emit progress("Writing", writtenBytes, totalBytes, timer.elapsed());

#ifdef Q_OS_LINUX
        phyDev.syncToDisk();
#endif
    }

    if (writtenBytes != totalBytes)
    {
        emit error("Writing failed");
        return;
    }

    qDebug() << "Sync...";
    emit syncing();
    phyDev.syncToDisk();

    //TODO: Add verification step that reads back data from disk and compare hash

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

#if KARCHIVE_VERSION_MINOR > 81
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
            return nullptr;
        }

        return stdFile;
    }
}
