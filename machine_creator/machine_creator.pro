#-------------------------------------------------
#
# Project created by QtCreator 2015-12-15T12:02:55
#
#-------------------------------------------------

QT       += core gui network
QT_CONFIG -= no-pkg-config
CONFIG += c++11 link_pkgconfig

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calaos_machinecreator
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           DiskWriter.cpp \
           DiskWriter_unix.cpp \
    LzmaLoader.cpp

HEADERS  += mainwindow.h \
            DiskWriter.h \
            DiskWriter_unix.h \
    LzmaLoader.h

FORMS    += mainwindow.ui
