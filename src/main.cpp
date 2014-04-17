#include <QtWidgets>
#include "mainwindow.h"
#include "ConfigOptions.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Calaos");
    QCoreApplication::setOrganizationDomain("calaos.fr");
    QCoreApplication::setApplicationName("CalaosInstaller");

    QStringList p;
    p << QCoreApplication::applicationDirPath();
    QCoreApplication::setLibraryPaths(p);

    if (!QApplication::setStyle("qtcurve"))
    {
        qDebug("Error setting style...");

        qDebug("Styles avalaible:");
        for (int i = 0;i < QStyleFactory::keys().size();i++)
            qDebug() << QStyleFactory::keys().at(i);
    }

    QString locale;
    {
        if (ConfigOptions::Instance().optionExists("ui/lang"))
        {
            //set language from config
            locale = ConfigOptions::Instance().getOption("ui/lang").toString();
        }
        else
        {
            //set default system language
            locale = QLocale::system().name().section('_', 0, 0);
        }
    }

    //Set language
    QString langfile = QString(":/lang/calaos_installer_%1.qm").arg(locale);
    QTranslator translator;
    qDebug() << "Trying to set language: " << langfile;
#ifdef Q_OS_WIN
    QTranslator translator_qt;
    if (QFile::exists(langfile))
    {
        translator_qt.load(QString(":/lang/qt_%1.qm").arg(locale));
        app.installTranslator(&translator_qt);
        translator.load(langfile);
        app.installTranslator(&translator);
    }
#else
    translator.load(langfile);
    app.installTranslator(&translator);
#endif

    MainWindow w;

    w.show();
    return app.exec();
}
