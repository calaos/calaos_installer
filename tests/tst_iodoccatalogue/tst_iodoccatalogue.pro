QT += core gui widgets xml network testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_iodoccatalogue
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

# Unlike tst_widgetioproperties, this suite checks the catalogue shipped in data/doc.
RESOURCES += iodoc_shipped.qrc

SOURCES += \
    $$PWD/../../src/IODoc.cpp \
    $$PWD/../../src/WidgetIOProperties.cpp \
    $$PWD/../../src/BalloonTip.cpp \
    $$PWD/../../src/dialogioproperties.cpp \
    $$PWD/../../src/qanimationlabel.cpp \
    tst_iodoccatalogue.cpp

HEADERS += \
    $$PWD/../../src/IODoc.h \
    $$PWD/../../src/WidgetIOProperties.h \
    $$PWD/../../src/BalloonTip.h \
    $$PWD/../../src/dialogioproperties.h \
    $$PWD/../../src/qanimationlabel.h

FORMS += \
    $$PWD/../../data/WidgetIOProperties.ui $$PWD/../../data/DialogIoProperties.ui
