#include "DiskWriter.h"
#include <qDebug>
#include <QCoreApplication>
#include <QFile>
#include <QTime>

void DiskWriter::cancelWrite()
{
    isCancelled = true;
}

void DiskWriter::writeCompressedImageToRemovableDevice(const QString &filename, const QString &device)
{
    int r;
    qint64 n;
    QByteArray buf(8*1024*1024*sizeof(char), 0);
    qint64 totaBytes = 0;
    isCancelled = false;

    QTime timer;
    timer.start();

    qDebug() << "Opengin device : " << device;
    if (!open(device)) {
        emit error("Couldn't open " + device);
        return;
    }


    QFile src(filename);
    qDebug() << "src " << filename << " as input";
    // Open source
    if (!src.open(QFile::ReadOnly)) {
        emit error("Couldn't open " + filename);
        this->close();
        return;
    }

    r = src.read(buf.data(), buf.length());
    while (r > 0 && ! isCancelled) {
        // TODO: Sanity check

        n = this->write(buf);
        if (n == -1) {
            emit error("Failed to write to " + device + "! " + this->errorString());
            qDebug() << "Failed to write to " + device + "! " + this->errorString();
            src.close();
            this->close();
            return;
        }
        this->sync();
        totaBytes += n;
        emit bytesWritten(totaBytes);
        qDebug() << "Write " << n;
        r = src.read(buf.data(), buf.length());
    }

    if (r < 0) {
        emit error("Failed to read from " + filename + "! " + src.errorString());
        src.close();
        this->close();
        return;
    }

    qDebug() << "Syncing";
    emit syncing();
    src.close();
    this->sync();
    this->close();

    // do something..
    int nMilliseconds = timer.elapsed();
    qDebug() << "time ellapsed : " << nMilliseconds / 1000 << " seconds";

    if (isCancelled) {
        emit bytesWritten(0);
    }
    else {
        emit finished();
    }
    isCancelled = false;
}


