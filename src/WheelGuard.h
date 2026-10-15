#ifndef WHEELGUARD_H
#define WHEELGUARD_H

#include <QEvent>
#include <QObject>
#include <QWidget>

//A wheel over a scrolled form scrolls it, it does not change the value under the mouse.
class WheelGuard: public QObject
{
public:
    using QObject::QObject;

    static void install(QWidget *w)
    {
        w->setFocusPolicy(Qt::StrongFocus);
        w->installEventFilter(new WheelGuard(w));
    }

protected:
    bool eventFilter(QObject *o, QEvent *e) override
    {
        QWidget *w = qobject_cast<QWidget *>(o);
        if (e->type() == QEvent::Wheel && w && !w->hasFocus())
        {
            e->ignore();
            return true;
        }
        return QObject::eventFilter(o, e);
    }
};

#endif // WHEELGUARD_H
