#ifndef DIALOGDELETEIMPACT_H
#define DIALOGDELETEIMPACT_H

#include <QDialog>
#include <QString>

#include <DeletionImpact.h>

class QDialogButtonBox;
class QTreeWidget;

// Asks before a deletion, and says what it does to the rest of the project:
// the sections are built from the model's DeletionImpact, computed before
// anything is deleted. The default button is Cancel.
class DialogDeleteImpact: public QDialog
{
    Q_OBJECT

public:
    //subject: the thing deleted ("device \"Salon AC\""). listOwnedIos: also
    //list the IOs that go with it (a device's own), minus the first of impact.ios.
    DialogDeleteImpact(const QString &subject, const Calaos::DeletionImpact &impact,
                       bool listOwnedIos, QWidget *parent = nullptr);

    static QString ioLabel(Calaos::IOBase *io);
    static QString ruleLabel(Calaos::Rule *rule);

private:
    QTreeWidget *m_tree;
    QDialogButtonBox *m_buttons;

    void addSection(const QString &title, const QString &hint, const QStringList &entries,
                    const QIcon &icon, bool warning);
};

#endif // DIALOGDELETEIMPACT_H
