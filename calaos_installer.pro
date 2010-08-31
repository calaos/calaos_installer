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
    src/ioeditorselection.cpp \
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
    src/dialogiolist.cpp \
    src/dialogioplagehoraire.cpp \
    src/dialogplageinput.cpp \
    src/formdali.cpp \
    src/dialogdalideviceconfig.cpp \
    src/dialogdetectsqueezebox.cpp \
    src/textedit.cpp \
    src/ClearLineEdit.cpp \
    src/formrules.cpp \
    src/FormConditionStd.cpp \
    src/FormActionStd.cpp
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
    src/ioeditorselection.h \
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
    src/dialogiolist.h \
    src/qtreewidget_addition.h \
    src/dialogioplagehoraire.h \
    src/dialogplageinput.h \
    src/formdali.h \
    src/dialogdalideviceconfig.h \
    src/dialogdetectsqueezebox.h \
    src/textedit.h \
    src/ClearLineEdit.h \
    src/formrules.h \
    src/FormConditionStd.h \
    src/FormActionStd.h
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
    data/dialogiolist.ui \
    data/dialogplagehoraire.ui \
    data/dialogplageinput.ui \
    data/formdali.ui \
    data/dialogdalideviceconfig.ui \
    data/dialogdetectsqueezebox.ui \
    data/formrules.ui \
    data/FormConditionStd.ui \
    data/FormActionStd.ui
RESOURCES += data/resources.qrc \
    data/textedit.qrc
INCLUDEPATH += ../calaos_common/ \
    Calaos/ \
    src/
DEFINES += CALAOS_INSTALLER
OTHER_FILES += 
