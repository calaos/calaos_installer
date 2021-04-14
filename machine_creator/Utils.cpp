#include "Utils.h"

QString Utils::sizeHuman(qint64 sz)
{
    double num = sz;
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
     {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num, 'f', 2) + " " + unit;
}

QString Utils::calculateTimeRemaining(QDateTime startTime, qint64 received, qint64 total)
{
    uint difference = QDateTime::currentDateTime().toTime_t() - startTime.toTime_t();

    if (difference > 0)
    {
        QString timeString;
        qreal timeRemaining = total / (received / difference);

        if (timeRemaining > 7200)
        {
            timeRemaining /= 3600;
            int hours = static_cast<int>(timeRemaining + 0.5);

            if (hours > 1)
                timeString = QObject::tr("about %1 hours").arg (hours);
            else
                timeString = QObject::tr("about one hour");
        }
        else if (timeRemaining > 60)
        {
            timeRemaining /= 60;
            int minutes = static_cast<int>(timeRemaining + 0.5);

            if (minutes > 1)
                timeString = QObject::tr("%1 minutes").arg (minutes);
            else
                timeString = QObject::tr("1 minute");
        }
        else if (timeRemaining <= 60)
        {
            int seconds = static_cast<int>(timeRemaining + 0.5);

            if (seconds > 1)
                timeString = QObject::tr("%1 seconds").arg (seconds);
            else
                timeString = QObject::tr("1 second");
        }

        return timeString;
    }

    return QObject::tr("Unknown");
}
