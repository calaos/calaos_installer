#ifndef RULEACTIONMENU_H
#define RULEACTIONMENU_H

#include <QtWidgets>

const QString RuleActionTpl = "<span style=\"font-style:italic; color: rgb(150, 150, 150);\">%1</span>";

class RuleActionMenu : public QWidgetAction
{
        Q_OBJECT
        public:
                explicit RuleActionMenu(QWidget *parent, QString &title, QString &help, QString &command);
                
                QLabel *getActionLabel() { return actionLabel; }
                QLabel *getHelpLabel() { return helpLabel; }

                QString &getCommand() { return command; }

        private:
                QLabel *actionLabel;
                QLabel *helpLabel;
                QString command;

        signals:
                void triggered(RuleActionMenu *action);
                
        private slots:
                void emitSignal();
                
};

#endif // RULEACTIONMENU_H
