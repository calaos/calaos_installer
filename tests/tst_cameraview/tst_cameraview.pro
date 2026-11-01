QT += core gui widgets xml network testlib printsupport quick qml quickwidgets quickcontrols2 serialport
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_cameraview
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

# Headers only: dialogcameraview.cpp includes mainwindow.h, which pulls the
# whole application's declarations (none of them is linked or used).
INCLUDEPATH += \
    $$PWD/../../src/common/LuaScript \
    $$PWD/../../src/common/LuaScript/lua-5.1.4/src \
    $$PWD/../../src/common/LuaScript/lua-5.1.4/etc
unix:!macx: INCLUDEPATH += /usr/include/KF6/KArchive

# DialogCameraView.cpp is compiled as is; the test needs only the dialog and
# the data model (the real camera preview, no stub).
SOURCES += \
    $$PWD/../../src/dialogcameraview.cpp \
    $$PWD/../../src/dialognewcamera.cpp \
    $$PWD/../../src/networkerrortext.cpp \
    tst_cameraview.cpp

HEADERS += \
    $$PWD/../../src/dialogcameraview.h \
    $$PWD/../../src/dialognewcamera.h \
    $$PWD/../../src/networkerrortext.h

FORMS += $$PWD/../../data/DialogCameraView.ui \
    $$PWD/../../data/DialogNewCamera.ui
