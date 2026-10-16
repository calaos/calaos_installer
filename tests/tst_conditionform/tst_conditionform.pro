QT += core gui widgets xml network testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_conditionform
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

RESOURCES += $$PWD/../../data/resources.qrc

SOURCES += \
    $$PWD/../../src/FormConditionStd.cpp \
    $$PWD/../../src/RuleActionMenu.cpp \
    $$PWD/../../src/dialogiolist.cpp \
    tst_conditionform.cpp

HEADERS += \
    $$PWD/../../src/FormConditionStd.h \
    $$PWD/../../src/RuleActionMenu.h \
    $$PWD/../../src/dialogiolist.h \
    $$PWD/../../src/qtreewidget_addition.h

FORMS += \
    $$PWD/../../data/FormConditionStd.ui \
    $$PWD/../../data/DialogIoList.ui
