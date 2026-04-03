#include "TimezoneHelper.h"

#include <QTimeZone>
#include <QDateTime>
#include <QMap>
#include <QRegularExpression>

QStringList TimezoneHelper::filteredIanaTimezones()
{
    static QStringList s_cached;
    if (!s_cached.isEmpty())
        return s_cached;

    // Prefixes to exclude: deprecated aliases and non-geographic zones
    static const QStringList excludePrefixes = {
        "Etc/", "SystemV/", "US/", "Canada/", "Mexico/",
        "Brazil/", "Chile/", "Cuba", "Egypt", "Eire",
        "GB", "GMT", "Hongkong", "Iceland", "Iran", "Israel",
        "Jamaica", "Japan", "Kwajalein", "Libya", "Navajo",
        "NZ", "Poland", "Portugal", "PRC", "ROC", "ROK",
        "Singapore", "Turkey", "UCT", "W-SU", "Zulu",
        "CST6CDT", "EST5EDT", "MST7MDT", "PST8PDT",
        "posix/", "right/", "EST", "MST", "HST",
    };

    // Valid geographic region prefixes (IANA canonical zones)
    static const QStringList allowedPrefixes = {
        "Africa/", "America/", "Antarctica/", "Arctic/",
        "Asia/", "Atlantic/", "Australia/", "Europe/",
        "Indian/", "Pacific/",
    };

    QList<QByteArray> allZones = QTimeZone::availableTimeZoneIds();
    QStringList result;

    for (const QByteArray &zoneId : allZones)
    {
        QString id = QString::fromUtf8(zoneId);

        // Keep "UTC" itself
        if (id == "UTC")
        {
            result.append(id);
            continue;
        }

        // Must contain '/' to be a canonical Area/City entry
        if (!id.contains('/'))
            continue;

        // Only allow known geographic region prefixes
        bool allowed = false;
        for (const QString &prefix : allowedPrefixes)
        {
            if (id.startsWith(prefix))
            {
                allowed = true;
                break;
            }
        }
        if (!allowed)
            continue;

        // Exclude deprecated prefixes
        bool excluded = false;
        for (const QString &prefix : excludePrefixes)
        {
            if (id.startsWith(prefix))
            {
                excluded = true;
                break;
            }
        }
        if (excluded)
            continue;

        result.append(id);
    }

    result.sort();
    s_cached = result;
    return s_cached;
}

// Helper: format an offset in seconds to POSIX offset string.
// POSIX convention: sign is INVERTED vs UTC offset.
// UTC+1 (3600s) -> "-1", UTC-5 (-18000s) -> "5"
static QString formatPosixOffset(int offsetSeconds)
{
    // Invert sign for POSIX
    int totalSeconds = -offsetSeconds;
    bool negative = totalSeconds < 0;
    int abs = qAbs(totalSeconds);

    int hours = abs / 3600;
    int minutes = (abs % 3600) / 60;
    int seconds = abs % 60;

    QString result;
    if (negative)
        result = QStringLiteral("-");

    result += QString::number(hours);

    if (minutes > 0 || seconds > 0)
    {
        result += QStringLiteral(":") + QString::number(minutes).rightJustified(2, '0');
        if (seconds > 0)
            result += QStringLiteral(":") + QString::number(seconds).rightJustified(2, '0');
    }

    return result;
}

// Helper: compute the M.w.d rule from a QDateTime transition date.
// Returns e.g. "M3.5.0" for "last Sunday of March"
static QString transitionToMwd(const QDateTime &dt)
{
    QDate d = dt.date();
    int month = d.month();
    int dayOfWeek = d.dayOfWeek() % 7; // Qt: Mon=1..Sun=7 -> POSIX: Sun=0..Sat=6
    int dayOfMonth = d.day();

    // Compute which week: POSIX uses week 1-4 for specific occurrences,
    // week 5 means "last occurrence in the month"
    int week = (dayOfMonth - 1) / 7 + 1;

    // Check if this is the last occurrence of this weekday in the month
    int daysInMonth = d.daysInMonth();
    if (dayOfMonth + 7 > daysInMonth)
        week = 5; // last occurrence

    return QStringLiteral("M%1.%2.%3").arg(month).arg(week).arg(dayOfWeek);
}

// Helper: format time part of transition rule
// Default transition time is 02:00, only append if different
static QString transitionTimeStr(const QDateTime &dt)
{
    QTime t = dt.time();
    if (t.hour() == 2 && t.minute() == 0 && t.second() == 0)
        return QString(); // 02:00 is the POSIX default, omit it

    QString result = QStringLiteral("/") + QString::number(t.hour());
    if (t.minute() > 0 || t.second() > 0)
    {
        result += QStringLiteral(":") + QString::number(t.minute()).rightJustified(2, '0');
        if (t.second() > 0)
            result += QStringLiteral(":") + QString::number(t.second()).rightJustified(2, '0');
    }
    return result;
}

// Helper: wrap abbreviation in <> if it starts with +, -, or a digit (POSIX requirement)
static QString posixAbbrev(const QString &abbrev)
{
    if (abbrev.isEmpty())
        return abbrev;
    QChar first = abbrev.at(0);
    if (first == '+' || first == '-' || first.isDigit())
        return QStringLiteral("<") + abbrev + QStringLiteral(">");
    return abbrev;
}

QString TimezoneHelper::ianaToPosixTz(const QString &ianaId)
{
    if (ianaId.isEmpty() || ianaId == "UTC")
        return QStringLiteral("UTC0");

    QTimeZone tz(ianaId.toUtf8());
    if (!tz.isValid())
        return QStringLiteral("UTC0");

    // Use January 15 of current year for standard time info
    int year = QDate::currentDate().year();
    QDateTime winterDt(QDate(year, 1, 15), QTime(12, 0), QTimeZone::UTC);

    QString stdAbbrev = tz.abbreviation(winterDt);
    int stdOffset = tz.standardTimeOffset(winterDt);

    // Find transitions for the current year
    QTimeZone::OffsetDataList transitions = tz.transitions(
        QDateTime(QDate(year, 1, 1), QTime(0, 0), QTimeZone::UTC),
        QDateTime(QDate(year, 12, 31), QTime(23, 59, 59), QTimeZone::UTC));

    // Build standard part: abbrev + offset
    QString posix = posixAbbrev(stdAbbrev) + formatPosixOffset(stdOffset);

    // Check for actual DST transitions this year (not just hasDaylightTime()
    // which returns true for historically-DST zones like Asia/Tokyo)
    if (transitions.size() < 2)
        return posix;

    // Get DST info from a summer date
    QDateTime summerDt(QDate(year, 7, 15), QTime(12, 0), QTimeZone::UTC);
    QString dstAbbrev = tz.abbreviation(summerDt);
    int dstOffset = tz.offsetFromUtc(summerDt);

    posix += posixAbbrev(dstAbbrev);

    // If DST offset is not the standard +1h from standard, specify it explicitly
    if (dstOffset != stdOffset + 3600)
        posix += formatPosixOffset(dstOffset);

    // Find the spring (to DST) and autumn (to STD) transitions
    // Wall-clock time must use the pre-transition offset:
    //   spring (entering DST): we're in standard time before -> use stdOffset
    //   autumn (leaving DST): we're in DST before -> use dstOffset
    QDateTime springTransition, autumnTransition;

    for (const QTimeZone::OffsetData &od : transitions)
    {
        if (od.daylightTimeOffset != 0)
        {
            // Transition TO DST: wall clock = UTC + standard offset (pre-transition)
            springTransition = od.atUtc.addSecs(stdOffset);
        }
        else
        {
            // Transition TO standard: wall clock = UTC + DST offset (pre-transition)
            autumnTransition = od.atUtc.addSecs(dstOffset);
        }
    }

    if (springTransition.isValid() && autumnTransition.isValid())
    {
        posix += QStringLiteral(",");
        posix += transitionToMwd(springTransition);
        posix += transitionTimeStr(springTransition);
        posix += QStringLiteral(",");
        posix += transitionToMwd(autumnTransition);
        posix += transitionTimeStr(autumnTransition);
    }

    return posix;
}

QString TimezoneHelper::posixTzToIana(const QString &posixTz)
{
    if (posixTz.isEmpty() || posixTz == "UTC0" || posixTz == "UTC")
        return QStringLiteral("UTC");

    // Build reverse map lazily: POSIX TZ -> IANA ID
    static QMap<QString, QString> s_reverseMap;
    static bool s_built = false;

    if (!s_built)
    {
        QStringList zones = filteredIanaTimezones();
        for (const QString &ianaId : zones)
        {
            QString posix = ianaToPosixTz(ianaId);
            // First match wins (alphabetical order, since list is sorted)
            if (!s_reverseMap.contains(posix))
                s_reverseMap.insert(posix, ianaId);
        }
        s_built = true;
    }

    if (s_reverseMap.contains(posixTz))
        return s_reverseMap.value(posixTz);

    // Fallback: if no exact match, return UTC
    return QStringLiteral("UTC");
}

QString TimezoneHelper::legacyUtcOffsetToPosixTz(const QString &utcOffset)
{
    // Input: "UTC", "UTC+1", "UTC-5", "UTC+5:30", "UTC-9:30", etc.
    // Output: POSIX TZ with inverted sign: "UTC0", "UTC-1", "UTC5", "UTC-5:30", "UTC9:30"

    if (utcOffset == "UTC")
        return QStringLiteral("UTC0");

    QString rest = utcOffset.mid(3); // Remove "UTC" prefix
    if (rest.isEmpty())
        return QStringLiteral("UTC0");

    // Invert the sign for POSIX convention
    QString posixOffset;
    if (rest.startsWith('+'))
        posixOffset = QStringLiteral("-") + rest.mid(1);
    else if (rest.startsWith('-'))
        posixOffset = rest.mid(1); // Remove minus = positive in POSIX
    else
        posixOffset = rest;

    return QStringLiteral("UTC") + posixOffset;
}

QString TimezoneHelper::convertStoredTimezone(const QString &stored)
{
    if (stored.isEmpty())
        return QStringLiteral("UTC0");

    // Detect legacy UTC offset format: "UTC", "UTC+N", "UTC-N", "UTC+N:MM", "UTC-N:MM"
    static QRegularExpression legacyPattern(
        QStringLiteral("^UTC([+-]\\d{1,2}(:\\d{2})?)?$"));

    if (legacyPattern.match(stored).hasMatch())
        return legacyUtcOffsetToPosixTz(stored);

    // Already in POSIX TZ format (contains alphabetic abbreviation beyond just "UTC")
    return stored;
}
