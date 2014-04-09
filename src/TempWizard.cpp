#include "TempWizard.h"

TempWizard::TempWizard(QWidget *parent) :
    QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new RulesPage);

    setWindowTitle(QString::fromUtf8("Assistant de chauffage"));

    resize(680, 455);
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Assistant de chauffage"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/img/watermark.png"));

    QGridLayout *glayout = new QGridLayout;

    QLabel *label = new QLabel(QString::fromUtf8("Cet assistant va vous aider à créer les règles pour "
                                                 "une gestion de chauffage par consigne. Il va "
                                                 "lier la sonde de température avec une consigne "
                                                 "et ainsi créer les règles qui sont associés. "
                                                 "Ces règles permettront de régler la consigne de chauffage "
                                                 "directement sur une interface Calaos."));
    label->setWordWrap(true);

    glayout->addWidget(label, 0, 0, 1, 2);

    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    glayout->addWidget(line, 1, 0, 1, 2);

    label = new QLabel(QString::fromUtf8("Nom de la consigne:"));
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
    consName->setText(QString::fromUtf8("Consigne"));
}

RulesPage::RulesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(QString::fromUtf8("Règles"));
    setSubTitle(QString::fromUtf8("Est-ce que les règles doivent être crées automatiquement ?"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/img/watermark.png"));

    QCheckBox *checkRules = new QCheckBox(QString::fromUtf8("Créer les règles"));
    checkRules->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(checkRules);

    QLabel *label = new QLabel(QString::fromUtf8("Attention!\nL'assistant va créer les 2 règles qui vont "
                                                 "démarrer ou arrêter le chauffage en fonction de la "
                                                 "consigne. Ces règles ne comportent aucune action, "
                                                 "vous devrez les ajouter par vous même."));

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
