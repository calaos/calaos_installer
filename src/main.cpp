#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

        for (int i = 0;i < QStyleFactory::keys().size();i++)
                cout << "Style: " << QStyleFactory::keys().value(i).toAscii().data() << endl;

        if (!QApplication::setStyle("qtcurve"))
                cout << "Error setting style..." << endl;

        MainWindow w;

        w.show();
        return a.exec();
}
