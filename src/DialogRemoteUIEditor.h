#ifndef DIALOGREMOTEUIEDITOR_H
#define DIALOGREMOTEUIEDITOR_H

#include <QDialog>
#include <QObject>
#include "IOBase.h"

class DialogRemoteUIEditor : public QDialog
{
    Q_OBJECT
public:
    DialogRemoteUIEditor(Calaos::IOBase *ioBase, QWidget *parent = nullptr);

private slots:
    void acceptChanges();

private:
    Calaos::IOBase *ioBase;
};

#endif // DIALOGREMOTEUIEDITOR_H
