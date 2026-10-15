QT += core gui widgets xml network testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_devicecommand
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

# The shipped catalogue on purpose: this suite fails when a documented command
# field changes, which is the signal to look at the form.
RESOURCES += $$PWD/../tst_iodoccatalogue/iodoc_shipped.qrc $$PWD/../../data/resources.qrc

SOURCES += \
    $$PWD/../../src/IODoc.cpp \
    $$PWD/../../src/DeviceCommand.cpp \
    $$PWD/../../src/DeviceCommandForm.cpp \
    $$PWD/../../src/FormActionStd.cpp \
    $$PWD/../../src/RuleActionMenu.cpp \
    $$PWD/../../src/dialogiolist.cpp \
    tst_devicecommand.cpp

HEADERS += \
    $$PWD/../../src/IODoc.h \
    $$PWD/../../src/DeviceCommand.h \
    $$PWD/../../src/DeviceCommandForm.h \
    $$PWD/../../src/FormActionStd.h \
    $$PWD/../../src/RuleActionMenu.h \
    $$PWD/../../src/dialogiolist.h \
    $$PWD/../../src/qtreewidget_addition.h

FORMS += \
    $$PWD/../../data/FormActionStd.ui \
    $$PWD/../../data/DialogIoList.ui
TESTDATA += project.xml
