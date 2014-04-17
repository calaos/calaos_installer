#include "DialogOptions.h"
#include "ui_DialogOptions.h"
#include <QMessageBox>
#include "ConfigOptions.h"

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOptions)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    //Fill the language combobox from available *.qm files in the ressource
    ui->comboLang->addItem(tr("System default language"), QStringLiteral(""));
    ui->comboLang->addItem(QLocale::languageToString(QLocale::English), QStringLiteral("en"));

    if (ConfigOptions::Instance().optionExists("ui/lang") &&
        ConfigOptions::Instance().getOption("ui/lang").toString() == "en")
    {
        ui->comboLang->setCurrentIndex(ui->comboLang->count() - 1);
    }

    /*
      Languages:
      the files contained into the .qrc ressources a automatically parsed to populate the UI.
      The filename of the translation files should have correct ISO 639 codes like:
          calaos_installer_CODE.qm
      for chinese, we need traditional chinese and simplified chinese. We need to use the following codes:
        calaos_installer_zh_HANS.qm for (generic) simplified Chinese characters
        calaos_installer_zh_HANT.qm for traditional Chinese characters
      other languages like french or german would only be:
        calaos_installer_fr.qm
        calaos_installer_de.qm
    */

    QDirIterator it(":/lang", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        //calaos_installer_LANG.qm
        QString fname = it.next();
        if (!fname.startsWith(":/lang/calaos_installer_")) continue;
        QString lang = fname.section('_', 2).section('.', 0, 0);
        QLocale locale(lang);

        QString ln;
        if (locale.language() == QLocale::Chinese)
            ln = QString("%1 (%2)")
                    .arg(QLocale::languageToString(locale.language()))
                    .arg(QLocale::scriptToString(locale.script()));
        else
            ln = QLocale::languageToString(locale.language());

        ui->comboLang->addItem(ln, lang);

        if (ConfigOptions::Instance().optionExists("ui/lang") &&
            ConfigOptions::Instance().getOption("ui/lang").toString() == lang)
        {
            ui->comboLang->setCurrentIndex(ui->comboLang->count() - 1);
        }
    }
}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::applyConfig()
{
    bool needRestart = false;
    bool config_changed = false;

    QString lang = ui->comboLang->itemData(ui->comboLang->currentIndex()).toString();

    if (!ConfigOptions::Instance().optionExists("ui/lang") ||
        ConfigOptions::Instance().getOption("ui/lang").toString() != lang)
    {
        ConfigOptions::Instance().setOption("ui/lang", lang);
        config_changed = true;
        needRestart = true;
    }

    /* Save other config options here */

    if (config_changed)
        ConfigOptions::Instance().saveConfig();

    if (needRestart)
    {
        QMessageBox box;
        box.setText(tr("Relaunch required"));
        box.setInformativeText(tr("Calaos Installer need to be restarted for the new settings to take effect. Do it now or later after saving your work."));
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
    }
}
