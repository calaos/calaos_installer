QT += core gui widgets xml testlib quick quickwidgets qml quickcontrols2
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_apptheme
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

# The QML of the Remote UI editor, for the screenshots.
RESOURCES += $$PWD/../../qml/qml.qrc $$PWD/../../data/resources.qrc

INCLUDEPATH += $$PWD/../../src

SOURCES += \
    $$PWD/../../src/AppTheme.cpp \
    $$PWD/../../src/DialogRemoteUIEditor.cpp \
    $$PWD/../../src/DialogIOSelector.cpp \
    $$PWD/../../src/ClearLineEdit.cpp \
    $$PWD/../../src/RemoteUIModel.cpp \
    $$PWD/../../src/PageModel.cpp \
    $$PWD/../../src/WidgetModel.cpp \
    $$PWD/../../src/TimezoneHelper.cpp \
    tst_apptheme.cpp

HEADERS += \
    $$PWD/../../src/AppTheme.h \
    $$PWD/../../src/DialogRemoteUIEditor.h \
    $$PWD/../../src/DialogIOSelector.h \
    $$PWD/../../src/ClearLineEdit.h \
    $$PWD/../../src/RemoteUIModel.h \
    $$PWD/../../src/PageModel.h \
    $$PWD/../../src/WidgetModel.h \
    $$PWD/../../src/TimezoneHelper.h
