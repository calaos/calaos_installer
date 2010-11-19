#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

        if (!QApplication::setStyle("qtcurve"))
                qDebug("Error setting style...");

        MainWindow w;

        w.show();
        return a.exec();
}
