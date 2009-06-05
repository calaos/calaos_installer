#ifndef DIALOGNEWRULE_H
#define DIALOGNEWRULE_H

#include <QtGui/QDialog>
#include <iostream>

#include <ListeRule.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
        class DialogNewRule;
}

class DialogNewRule : public QDialog
{
                Q_OBJECT
                Q_DISABLE_COPY(DialogNewRule)
        public:
                explicit DialogNewRule(string name, string type, QWidget *parent = 0);
                virtual ~DialogNewRule();

                Rule *getRule() { return rule; }

        protected:
                virtual void changeEvent(QEvent *e);

        private:
                Ui::DialogNewRule *ui;
                Rule *rule;

        private slots:
                void on_buttonBox_accepted();
};

#endif // DIALOGNEWRULE_H
