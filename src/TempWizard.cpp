#include "TempWizard.h"

TempWizard::TempWizard(QWidget *parent) :
    QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new RulesPage);

    setWindowTitle(tr("Heating setpoint wizard"));

    resize(680, 455);
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Heating setpoint wizard"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/img/watermark.png"));

    QGridLayout *glayout = new QGridLayout;

    QLabel *label = new QLabel(QObject::tr("This wizard will help you create rules "
                                           "for managing you heating system with a "
                                           "setpoint. It will link the temperature "
                                           "sensor with the setpoint variable and "
                                           "create all assiociated rules. Those rules "
                                           "will let user change the setpoint directly "
                                           "on the Calaos user interface.");
    label->setWordWrap(true);

    glayout->addWidget(label, 0, 0, 1, 2);

    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    glayout->addWidget(line, 1, 0, 1, 2);

    label = new QLabel(tr("Setpoint name:"));
    consName = new QLineEdit;
    label->setBuddy(consName);

    glayout->addWidget(label, 2, 0);
    glayout->addWidget(consName, 2, 1);
    glayout->setVerticalSpacing(20);

    setLayout(glayout);

    registerField("consigneName*", consName);
}

void IntroPage::initializePage()
{
    consName->setText(QObject::tr("Setpoint"));
}

RulesPage::RulesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(QObject::tr("Rules"));
    setSubTitle(QObject::tr("Do you want the rules to be automatically created?"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/img/watermark.png"));

    QCheckBox *checkRules = new QCheckBox(QObject::tr("Create the rules"));
    checkRules->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(checkRules);

    QLabel *label = new QLabel(QObject::tr("Warning!\nThe wizard is going to create 2 rules that would "
                                           "start or stop the heating system from the setpoint value. Those "
                                           "rules will not have any actions, you need to add them yourself."));

    label->setWordWrap(true);

    QHBoxLayout *hbox = new QHBoxLayout;

    QLabel *icon = new QLabel;
    icon->setPixmap(QPixmap(":/img/messagebox_info.png"));

    hbox->addWidget(icon);
    hbox->addWidget(label, 1);

    layout->addLayout(hbox);
    layout->setSpacing(20);

    setLayout(layout);

    registerField("checkRules", checkRules);
}
