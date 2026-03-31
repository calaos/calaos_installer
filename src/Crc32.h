#ifndef CRC32_H
#define CRC32_H

#include <QByteArray>
#include <cstdint>

class Crc32
{
public:
    static uint32_t compute(const QByteArray &data);
};

#endif // CRC32_H
