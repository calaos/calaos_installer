#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Calaos");
    QCoreApplication::setOrganizationDomain("calaos.fr");
    QCoreApplication::setApplicationName("CalaosMachineCreator");

    LzmaLoader::Instance().load();

    if (!LzmaLoader::Instance().load())
    {
        QMessageBox box;
        box.setText(QObject::tr("Missing component!"));
        box.setInformativeText(QObject::tr("liblzma is not found on the system. Please install a copy of lzma"));
        box.setStandardButtons(QMessageBox::Ok);
        return box.exec();
    }

    MainWindow w;
    w.show();

    return a.exec();
}
