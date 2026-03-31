#include "Crc32.h"

// ISO 3309 / ITU-T V.42 CRC-32 table computed from polynomial 0xEDB88320
static const struct Crc32Table
{
    uint32_t entries[256];
    constexpr Crc32Table() : entries{}
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            uint32_t crc = i;
            for (int j = 0; j < 8; j++)
                crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;
            entries[i] = crc;
        }
    }
} s_crc32Table;

uint32_t Crc32::compute(const QByteArray &data)
{
    uint32_t crc = 0xFFFFFFFF;
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(data.constData());
    for (int i = 0;i < data.size();i++)
        crc = s_crc32Table.entries[(crc ^ bytes[i]) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFF;
}
