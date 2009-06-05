#include <QtGui/QApplication>
#include "mainwindow.h"
#include "detectip.h"

int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

        DetectIP::Instance();

        MainWindow w;

        w.show();
        return a.exec();
}
