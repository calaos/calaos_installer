#include <QtWidgets>
#include "mainwindow.h"
#include "ConfigOptions.h"

static void echoVersion()
{
    cout << "Calaos Version: \n\t" CALAOS_VERSION << endl;
}

static void echoUsage(char **argv)
{
    echoVersion();
    cout << "Usage:\n\t" << argv[0] << " [options]" << endl;
    cout << endl << "\tOptions:\n";
    cout << "\t-h, --help\tDisplay this help.\n";
    cout << "\t-v, --version\tDisplay current version and exit.\n";
    cout << endl;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Calaos");
    QCoreApplication::setOrganizationDomain("calaos.fr");
    QCoreApplication::setApplicationName("CalaosInstaller");

    QStringList p;
    p << QCoreApplication::applicationDirPath();
    QCoreApplication::setLibraryPaths(p);

    if (!QApplication::setStyle("fusion"))
    {
        qDebug("Error setting style...");

        qDebug("Styles avalaible:");
        for (int i = 0;i < QStyleFactory::keys().size();i++)
            qDebug() << QStyleFactory::keys().at(i);
    }

    QStringList cmdline_args = QCoreApplication::arguments();
    if (cmdline_args.contains("--version") || cmdline_args.contains("-v"))
    {
        echoVersion();
        exit(0);
    }
    else if (cmdline_args.contains("--help") || cmdline_args.contains("-h"))
    {
        echoUsage(argv);
        exit(0);
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
