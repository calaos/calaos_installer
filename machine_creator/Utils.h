#ifndef UTILS_H
#define UTILS_H

#include <QtCore>

class Utils
{
public:
    static QString sizeHuman(qint64 sz);
    static QString calculateTimeRemaining(QDateTime startTime, qint64 received, qint64 total);
};

#endif // UTILS_H
