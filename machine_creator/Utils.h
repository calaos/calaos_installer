#ifndef UTILS_H
#define UTILS_H

#include <QtCore>

#if defined(Q_OS_WIN)
#include <qt_windows.h>
#endif

class Utils
{
public:
    static QString sizeHuman(qint64 sz);
    static QString calculateTimeRemaining(QDateTime startTime, qint64 received, qint64 total);

#if defined(Q_OS_WIN)
    static QString errorMessageFromCode(DWORD errorCode);
    static QString formatErrorMessageFromCode(QString prefixMessage, DWORD errorCode);
#endif
};

#endif // UTILS_H
