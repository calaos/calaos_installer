QT       += core gui network widgets
QT_CONFIG -= no-pkg-config
CONFIG += c++11 link_pkgconfig

TARGET = calaos_machinecreator
TEMPLATE = app

SOURCES += main.cpp\
    BLAKE2/ref/blake2b-ref.c \
    CalaosApi.cpp \
    CalaosImage.cpp \
    NetworkRequest.cpp \
    Platform.cpp \
    StorageDelegate.cpp \
    UsbDisk.cpp \
    Utils.cpp \
    mainwindow.cpp \
    DiskWriter.cpp \
    QmlHelpers/qqmlhelpers.cpp \
    QmlHelpers/qqmlobjectlistmodel.cpp \
    QmlHelpers/qqmlvariantlistmodel.cpp

HEADERS  += mainwindow.h \
    BLAKE2/ref/blake2-impl.h \
    BLAKE2/ref/blake2.h \
    CalaosApi.h \
    CalaosImage.h \
    DiskWriter.h \
    NetworkRequest.h \
    QmlHelpers/qqmlhelpers.h \
    QmlHelpers/qqmlmodels.h \
    QmlHelpers/qqmlobjectlistmodel.h \
    QmlHelpers/qqmlvariantlistmodel.h \
    QmlHelpers/qqmlvariantlistmodel_p.h \
    StorageDelegate.h \
    UsbDisk.h \
    Utils.h \
    Platform.h \
    drivelist/src/drivelist.hpp

FORMS    += mainwindow.ui

RESOURCES += \
    img.qrc

LIBS += -lKF5Archive

linux {
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += libudev
    QT += KArchive

    LIBS += -ldl

    SOURCES += UsbMonitor_linux.cpp \
        drivelist_linux/linuxdrivelist.cpp
    HEADERS += UsbMonitor_linux.h

    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

    # install the binary
    target.path = $$PREFIX/bin
    INSTALLS += target

    # install the elevation policy
    policy.files = fr.calaos.pkexec.calaos_machinecreator.policy
    policy.commands = mkdir -p /usr/share/polkit-1/actions
    policy.path = /usr/share/polkit-1/actions
    INSTALLS += policy
}

mac {
    SOURCES += UsbMonitor_mac.mm \
        drivelist/src/darwin/list.mm \
        drivelist/src/darwin/REDiskList.m
    HEADERS += UsbMonitor_mac.h \
        drivelist/src/darwin/REDiskList.h

    LIBS += -framework IOKit -framework Cocoa -framework Security

    #KArchive
    INCLUDEPATH += /usr/local/opt/karchive/include/KF5/KArchive
    INCLUDEPATH += /opt/homebrew/opt/qt/include/KF6/KArchive
    INCLUDEPATH += /usr/local/opt/karchive/include/KF6/KArchive
    INCLUDEPATH += /usr/local/opt/qt/include/KF6/KArchive
    LIBS += -L/usr/local/opt/karchive/lib
    LIBS += -L/opt/homebrew/opt/qt
    LIBS += -L/usr/local/opt/qt/
}

win32 {
    RC_FILE = windows_res.rc

    SOURCES += UsbMonitor_win.cpp \
        drivelist/src/windows/list.cpp
    HEADERS += UsbMonitor_win.h \
        drivelist/src/windows/list.hpp

    LIBS += -lole32 -loleaut32 -luuid -lsetupapi

    #KArchive
    INCLUDEPATH += C:/kderoot/include/KF5/KArchive \
        /mxe/usr/i686-w64-mingw32.shared.posix/include/KF5/KArchive \
        /mxe/usr/i686-w64-mingw32.shared.posix/include/KF6/KArchive
    LIBS += -LC:/kderoot/lib \
        -LC:/kderoot/bin \
        -L/mxe/usr/i686-w64-mingw32.shared.posix/lib
}

