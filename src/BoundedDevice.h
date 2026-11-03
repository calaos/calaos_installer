#ifndef BOUNDEDDEVICE_H
#define BOUNDEDDEVICE_H

#include <QIODevice>

// Read-only view of another device that fails once more than `limit` bytes were
// read from it. Put over a decompressor, it stops a compressed bomb while it is inflated
// instead of after everything was written out.
class BoundedDevice : public QIODevice
{
public:
    BoundedDevice(QIODevice *inner, qint64 limit, QObject *parent = nullptr)
        : QIODevice(parent), m_inner(inner), m_limit(limit) {}

    bool exceeded() const { return m_exceeded; }

    bool open(OpenMode mode) override
    {
        // Unbuffered: this device and the inner one then always sit at the same offset.
        return mode == QIODevice::ReadOnly && m_inner->isOpen()
            && QIODevice::open(mode | QIODevice::Unbuffered);
    }
    qint64 size() const override { return m_inner->size(); }

    bool seek(qint64 pos) override
    {
        // KCompressionDevice skips forward by inflating without any bound: do it here.
        if (pos < 0 || (pos < m_inner->pos() && !m_inner->seek(pos)))
            return false;
        char buf[4096];
        while (m_inner->pos() < pos)
            if (readData(buf, qMin<qint64>(pos - m_inner->pos(), sizeof buf)) <= 0)
                return false;
        return QIODevice::seek(pos);
    }

protected:
    qint64 readData(char *data, qint64 maxlen) override
    {
        const qint64 room = m_limit - m_inner->pos();
        if (room <= 0)
        {
            // Past the limit only a clean end of stream is acceptable.
            char c;
            if (m_inner->read(&c, 1) == 0 && m_inner->atEnd())
                return 0;
            m_exceeded = true;
            return -1;
        }
        return m_inner->read(data, qMin(maxlen, room));
    }
    qint64 writeData(const char *, qint64) override { return -1; }

private:
    QIODevice *m_inner;
    qint64 m_limit;
    bool m_exceeded = false;
};

#endif // BOUNDEDDEVICE_H
