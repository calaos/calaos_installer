#-------------------------------------------------
#
# Project created by QtCreator 2015-12-15T12:02:55
#
#-------------------------------------------------

QT       += core gui network widgets
QT_CONFIG -= no-pkg-config
CONFIG += c++11 link_pkgconfig

#QT += KArchive

#####
INCLUDEPATH += C:/kderoot/include/KF5/KArchive
LIBS += -LC:/kderoot/lib -LC:/kderoot/bin
#####

TARGET = calaos_machinecreator
TEMPLATE = app

win32 {
    RC_FILE = ../win32/windows_res.rc
}

SOURCES += main.cpp\
    BLAKE2/ref/blake2b-ref.c \
    CalaosApi.cpp \
    CalaosImage.cpp \
    NetworkRequest.cpp \
    Utils.cpp \
    mainwindow.cpp \
    DiskWriter.cpp \
    DiskWriter_unix.cpp \
    QmlHelpers/qqmlhelpers.cpp \
    QmlHelpers/qqmlobjectlistmodel.cpp \
    QmlHelpers/qqmlvariantlistmodel.cpp

HEADERS  += mainwindow.h \
    BLAKE2/ref/blake2-impl.h \
    BLAKE2/ref/blake2.h \
    CalaosApi.h \
    CalaosImage.h \
    DiskWriter.h \
    DiskWriter_unix.h \
    NetworkRequest.h \
    QmlHelpers/qqmlhelpers.h \
    QmlHelpers/qqmlmodels.h \
    QmlHelpers/qqmlobjectlistmodel.h \
    QmlHelpers/qqmlvariantlistmodel.h \
    QmlHelpers/qqmlvariantlistmodel_p.h \
    Utils.h

FORMS    += mainwindow.ui

RESOURCES += \
    ../data/resources.qrc
