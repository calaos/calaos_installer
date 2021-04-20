#ifndef DISKWRITER_H
#define DISKWRITER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QFile>

#include "UsbDisk.h"

#if defined(Q_OS_WIN)
#include <qt_windows.h>
#endif

class PhysicalDevice : public QFile
{
    Q_OBJECT
public:
    PhysicalDevice(const QString &name);

    // Opens the selected device in WriteOnly mode, flags is ignored
    virtual bool open(OpenMode flags) override;

    void syncToDisk();

protected:
#if defined(Q_OS_WIN)
    HANDLE m_fileHandle;
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
    volatile bool isCancelled;

    QIODevice *createSourceDevice(QString filename);
};

#endif // DISKWRITER_H
