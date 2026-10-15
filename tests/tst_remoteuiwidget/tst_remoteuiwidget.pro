QT += core gui widgets xml qml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_remoteuiwidget
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

RESOURCES += $$PWD/../tst_devicetemplates/devicetemplates.qrc

SOURCES += \
    $$PWD/../../src/RemoteUIModel.cpp \
    $$PWD/../../src/PageModel.cpp \
    $$PWD/../../src/WidgetModel.cpp \
    $$PWD/../../src/TimezoneHelper.cpp \
    tst_remoteuiwidget.cpp

HEADERS += \
    $$PWD/../tst_devicetemplates/fixture.h \
    $$PWD/../../src/RemoteUIModel.h \
    $$PWD/../../src/PageModel.h \
    $$PWD/../../src/WidgetModel.h \
    $$PWD/../../src/TimezoneHelper.h
