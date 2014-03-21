#ifndef TEMPWIZARD_H
#define TEMPWIZARD_H

#include <QtWidgets>
#include <QWizard>

class TempWizard : public QWizard
{
        Q_OBJECT

        public:
                explicit TempWizard(QWidget *parent = 0);
};

class IntroPage : public QWizardPage
{
        Q_OBJECT

        protected:
                QLineEdit *consName;

                void initializePage();

        public:
                IntroPage(QWidget *parent = 0);

};

class RulesPage : public QWizardPage
{
        Q_OBJECT

        public:
                RulesPage(QWidget *parent = 0);
};

#endif // TEMPWIZARD_H
