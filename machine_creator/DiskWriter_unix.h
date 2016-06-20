#ifndef DISKWRITER_UNIX_H
#define DISKWRITER_UNIX_H

#include "DiskWriter.h"
#include <QFile>

class DiskWriter_unix : public DiskWriter
{
    Q_OBJECT
public:
    explicit DiskWriter_unix(QObject *parent = 0);
    ~DiskWriter_unix();

private:
    QFile dev;

    bool open(const QString &device);
    void close();
    void sync();
    bool isOpen();
    qint64 write(const QByteArray &data);
    QString errorString();
};

#endif // DISKWRITER_UNIX_H
