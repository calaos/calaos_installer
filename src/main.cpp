#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

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
