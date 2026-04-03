#ifndef TIMEZONEHELPER_H
#define TIMEZONEHELPER_H

#include <QString>
#include <QStringList>

class TimezoneHelper
{
public:
    // Returns ~150 filtered IANA timezone IDs suitable for display
    static QStringList filteredIanaTimezones();

    // Converts IANA timezone ID to POSIX TZ string
    // e.g. "Europe/Paris" -> "CET-1CEST,M3.5.0/2,M10.5.0/3"
    static QString ianaToPosixTz(const QString &ianaId);

    // Converts POSIX TZ string back to IANA timezone ID
    // Returns "UTC" if no match found
    static QString posixTzToIana(const QString &posixTz);

    // Converts legacy "UTC+1", "UTC-5:30" format to POSIX TZ
    // e.g. "UTC+1" -> "UTC-1", "UTC-5" -> "UTC5"
    static QString legacyUtcOffsetToPosixTz(const QString &utcOffset);

    // Auto-detects stored format and converts to POSIX TZ
    // Handles both legacy UTC offset and already-POSIX strings
    static QString convertStoredTimezone(const QString &stored);
};

#endif // TIMEZONEHELPER_H
