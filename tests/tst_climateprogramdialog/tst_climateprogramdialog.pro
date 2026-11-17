QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_climateprogramdialog
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

INCLUDEPATH += $$PWD/../../src

# The shipped catalogue: offers() reads the device's list_rule from it.
RESOURCES += $$PWD/../tst_iodoccatalogue/iodoc_shipped.qrc

SOURCES += \
    $$PWD/../../src/DialogClimateProgram.cpp \
    $$PWD/../../src/DialogEditTimeRange.cpp \
    $$PWD/../../src/DialogDeleteImpact.cpp \
    $$PWD/../../src/DialogIOSelector.cpp \
    $$PWD/../../src/ClearLineEdit.cpp \
    $$PWD/../../src/TwoLineItemDelegate.cpp \
    tst_climateprogramdialog.cpp

HEADERS += \
    $$PWD/../../src/DialogClimateProgram.h \
    $$PWD/../../src/DialogEditTimeRange.h \
    $$PWD/../../src/DialogDeleteImpact.h \
    $$PWD/../../src/DialogIOSelector.h \
    $$PWD/../../src/ClearLineEdit.h \
    $$PWD/../../src/TwoLineItemDelegate.h \
    $$PWD/../../src/WheelGuard.h

FORMS += $$PWD/../../data/DialogEditTimeRange.ui
