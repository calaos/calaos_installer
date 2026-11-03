#ifndef TOOLUPDATEPROMPT_H
#define TOOLUPDATEPROMPT_H

#include <QString>

class QMessageBox;
class QWidget;

// The texts embed strings from the network (release tag, error reason): both boxes are plain
// text so they can never be read as markup. The caller runs them with exec().
QMessageBox *makeToolUpdateQuestion(const QString &tool, const QString &newVersion,
                                    const QString &current, QWidget *parent);
QMessageBox *makeToolUpdateFailedBox(const QString &error, QWidget *parent);

#endif // TOOLUPDATEPROMPT_H
