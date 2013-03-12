#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

        QTranslator myappTranslator;
        myappTranslator.load("calaos_installer_" + QLocale::system().name());
        a.installTranslator(&myappTranslator);

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

        MainWindow w;

        w.show();
        return a.exec();
}
