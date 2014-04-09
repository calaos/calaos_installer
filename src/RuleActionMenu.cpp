#include "RuleActionMenu.h"

RuleActionMenu::RuleActionMenu(QWidget *parent, QString &title, QString &help, QString &cmd) :
    QWidgetAction(parent),
    helpLabel(NULL),
    command(cmd)
{
    QWidget *pWidget = new QWidget(NULL);
    QVBoxLayout *pLayout = new QVBoxLayout();

    pWidget->setProperty("class", QVariant("MenuItem"));

    actionLabel = new QLabel(title);
    pLayout->addWidget(actionLabel);

    if (!help.isEmpty())
    {
        helpLabel = new QLabel(help);
        pLayout->addWidget(helpLabel, 0, Qt::AlignRight);
    }

    pWidget->setLayout (pLayout);
    setDefaultWidget(pWidget);

    connect(this, SIGNAL(triggered()), this, SLOT(emitSignal()));
}

void RuleActionMenu::emitSignal()
{
    emit triggered(this);
}
