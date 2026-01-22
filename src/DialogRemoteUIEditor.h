#ifndef DIALOGREMOTEUIEDITOR_H
#define DIALOGREMOTEUIEDITOR_H

#include <QDialog>
#include <QObject>
#include <QQuickWidget>
#include "IOBase.h"
#include "RemoteUIModel.h"

class DialogRemoteUIEditor : public QDialog
{
    Q_OBJECT
public:
    DialogRemoteUIEditor(Calaos::IOBase *ioBase, QWidget *parent = nullptr);
    ~DialogRemoteUIEditor() override;

private slots:
    void acceptChanges();

private:
    Calaos::IOBase *ioBase;
    RemoteUIModel *m_model;
    QQuickWidget *m_quickWidget;
};

#endif // DIALOGREMOTEUIEDITOR_H
