# -------------------------------------------------
# Project created by QtCreator 2009-04-27T17:51:09
# -------------------------------------------------
QT += network \
    xml
TARGET = calaos_installer
TEMPLATE = app
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    ../calaos_common/base64.c \
    ../calaos_common/Params.cpp \
    ../calaos_common/Utils.cpp \
    Calaos/Rule.cpp \
    Calaos/Room.cpp \
    Calaos/ListeRule.cpp \
    Calaos/ListeRoom.cpp \
    Calaos/IOFactory.cpp \
    Calaos/Condition.cpp \
    Calaos/Action.cpp \
    src/dialognewroom.cpp \
    src/dialognewwago.cpp \
    src/dialogioproperties.cpp \
    src/dialognewvolet.cpp \
    src/hometreewidget.cpp \
    src/detectip.cpp \
    src/projectmanager.cpp \
    src/dialognewrule.cpp \
    src/conditiontreewidget.cpp \
    src/actiontreewidget.cpp \
    src/conditiondelegate.cpp \
    src/ioeditorselection.cpp \
    src/actiondelegate.cpp \
    src/dialognewdali.cpp \
    src/wagouploader.cpp \
    src/dialognewdalirgb.cpp \
    src/dialognewtemp.cpp \
    src/dialognewcamera.cpp \
    src/dialognewaudio.cpp \
    src/dialognewinternal.cpp \
    src/dialognewscenario.cpp \
    src/dialognewtime.cpp \
    src/dialogcameraview.cpp \
    src/wagoconnect.cpp \
    src/dialogconnect.cpp \
    src/dialogopenonline.cpp \
    src/qanimationlabel.cpp \
    src/dialogsaveonline.cpp \
    src/formpost.cpp \
    src/dialogiolist.cpp
HEADERS += src/mainwindow.h \
    ../calaos_common/Utils.h \
    ../calaos_common/base64.h \
    ../calaos_common/Params.h \
    Calaos/IOBase.h \
    Calaos/Rule.h \
    Calaos/Room.h \
    Calaos/ListeRule.h \
    Calaos/ListeRoom.h \
    Calaos/IOFactory.h \
    Calaos/IOBase.h \
    Calaos/Condition.h \
    Calaos/Action.h \
    src/dialognewroom.h \
    src/dialognewwago.h \
    src/dialogioproperties.h \
    src/dialognewvolet.h \
    src/hometreewidget.h \
    src/detectip.h \
    src/projectmanager.h \
    src/dialognewrule.h \
    src/conditiontreewidget.h \
    src/actiontreewidget.h \
    src/conditiondelegate.h \
    src/ioeditorselection.h \
    src/actiondelegate.h \
    src/dialognewdali.h \
    src/wagouploader.h \
    src/dialognewdalirgb.h \
    src/dialognewtemp.h \
    src/dialognewcamera.h \
    src/dialognewaudio.h \
    src/dialognewinternal.h \
    src/dialognewscenario.h \
    src/dialognewtime.h \
    src/dialogcameraview.h \
    src/wagoconnect.h \
    src/dialogconnect.h \
    src/dialogopenonline.h \
    src/qanimationlabel.h \
    src/dialogsaveonline.h \
    src/formpost.h \
    src/dialogiolist.h
FORMS += data/mainwindow.ui \
    data/dialognewroom.ui \
    data/dialognewwago.ui \
    data/dialogioproperties.ui \
    data/dialognewvolet.ui \
    data/dialognewrule.ui \
    data/dialognewdali.ui \
    data/dialognewdalirgb.ui \
    data/dialognewtemp.ui \
    data/dialognewcamera.ui \
    data/dialognewaudio.ui \
    data/dialognewinternal.ui \
    data/dialognewscenario.ui \
    data/dialognewtime.ui \
    data/dialogcameraview.ui \
    data/dialogconnect.ui \
    data/dialogopenonline.ui \
    data/dialogsaveonline.ui \
    data/dialogiolist.ui
RESOURCES += data/resources.qrc
INCLUDEPATH += ../calaos_common/ \
    Calaos/ \
    src/
DEFINES += CALAOS_INSTALLER
