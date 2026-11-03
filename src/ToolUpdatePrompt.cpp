#include "ToolUpdatePrompt.h"

#include <QCoreApplication>
#include <QMessageBox>

namespace
{
// A named context lets lupdate extract the messages below.
struct Tr
{
    Q_DECLARE_TR_FUNCTIONS(ToolUpdatePrompt)
};
} // namespace

QMessageBox *makeToolUpdateQuestion(const QString &tool, const QString &newVersion,
                                    const QString &current, QWidget *parent)
{
    auto *box = new QMessageBox(QMessageBox::Question, Tr::tr("Update %1").arg(tool),
                                Tr::tr("A newer %1 is available: %2 (installed: %3).\n\nDownload and use it?\n"
                                       "If you decline, the installed one is used.").arg(tool, newVersion, current),
                                QMessageBox::Yes | QMessageBox::No, parent);
    box->setTextFormat(Qt::PlainText);
    return box;
}

QMessageBox *makeToolUpdateFailedBox(const QString &error, QWidget *parent)
{
    auto *box = new QMessageBox(QMessageBox::Warning, Tr::tr("Update failed"),
                                Tr::tr("The tool was not updated; the installed one is used.") + "\n" + error,
                                QMessageBox::Ok, parent);
    box->setTextFormat(Qt::PlainText);
    return box;
}
