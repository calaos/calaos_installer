QT += core gui widgets xml network testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_widgetioproperties
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

# The catalogue shipped in data/doc is deliberately not used: the suite owns the
# bound it tests, so it does not depend on which io_doc.json is checked in.
RESOURCES += iodoc_fixture.qrc

SOURCES += \
    $$PWD/../../src/WidgetIOProperties.cpp \
    $$PWD/../../src/BalloonTip.cpp \
    $$PWD/../../src/qanimationlabel.cpp \
    tst_widgetioproperties.cpp

HEADERS += \
    $$PWD/../../src/WidgetIOProperties.h \
    $$PWD/../../src/BalloonTip.h \
    $$PWD/../../src/qanimationlabel.h

FORMS += \
    $$PWD/../../data/WidgetIOProperties.ui
