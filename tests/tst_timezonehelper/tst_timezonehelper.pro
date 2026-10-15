QT += core gui widgets xml quick qml quickwidgets quickcontrols2 testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_timezonehelper
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

RESOURCES += $$PWD/../../qml/qml.qrc

SOURCES += \
    $$PWD/../../src/TimezoneHelper.cpp \
    $$PWD/../../src/DialogRemoteUIEditor.cpp \
    $$PWD/../../src/DialogIOSelector.cpp \
    $$PWD/../../src/ClearLineEdit.cpp \
    $$PWD/../../src/RemoteUIModel.cpp \
    $$PWD/../../src/PageModel.cpp \
    $$PWD/../../src/WidgetModel.cpp \
    tst_timezonehelper.cpp

HEADERS += \
    $$PWD/../../src/TimezoneHelper.h \
    $$PWD/../../src/DialogRemoteUIEditor.h \
    $$PWD/../../src/DialogIOSelector.h \
    $$PWD/../../src/ClearLineEdit.h \
    $$PWD/../../src/RemoteUIModel.h \
    $$PWD/../../src/PageModel.h \
    $$PWD/../../src/WidgetModel.h
