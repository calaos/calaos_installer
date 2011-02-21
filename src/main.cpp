#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

        QStringList p;
        p << QCoreApplication::applicationDirPath();
        QCoreApplication::setLibraryPaths(p);

        if (!QApplication::setStyle("qtcurve"))
                qDebug("Error setting style...");

        MainWindow w;

        w.show();
        return a.exec();
}
