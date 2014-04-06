# -------------------------------------------------
# Project created by QtCreator 2009-04-27T17:51:09
# -------------------------------------------------
QT += core gui network widgets xml printsupport

#enable C++11 new syntax
CONFIG += c++11

TARGET = calaos_installer
TEMPLATE = app
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/common/base64.c \
    src/common/Params.cpp \
    src/common/Utils.cpp \
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
    src/FormActionStd.cpp \
    src/FormConditionStart.cpp \
    src/FormActionMail.cpp \
    src/DialogMailMessage.cpp \
    src/FormActionScript.cpp \
    src/DialogScriptEditor.cpp \
    src/CodeEditor.cpp \
    src/common/LuaScript/ScriptManager.cpp \
    src/common/LuaScript/ScriptBindings.cpp \
    src/FormActionTouchscreen.cpp \
    src/FormConditionScript.cpp \
    src/DialogInputEventScript.cpp \
    src/ConfigOptions.cpp \
    src/json.cpp \
    src/TempWizard.cpp \
    src/dialognewanalog.cpp \
    src/TwoLineItemDelegate.cpp \
    src/WagoModbus.cpp \
    src/DialogWagoFirmwareUpdate.cpp \
    src/RuleActionMenu.cpp \
    src/ftp/qurlinfo.cpp \
    src/ftp/qftp.cpp \
    src/dialognewonewiretemp.cpp \
    src/dialognewgpioinput.cpp \
    src/dialognewweboutputstring.cpp
HEADERS += src/mainwindow.h \
    src/common/Utils.h \
    src/common/base64.h \
    src/common/Params.h \
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
    src/FormActionStd.h \
    src/FormConditionStart.h \
    src/FormActionMail.h \
    src/DialogMailMessage.h \
    src/FormActionScript.h \
    src/DialogScriptEditor.h \
    src/CodeEditor.h \
    src/common/LuaScript/ScriptManager.h \
    src/common/LuaScript/ScriptBindings.h \
    src/common/LuaScript/Lunar.h \
    src/FormActionTouchscreen.h \
    src/FormConditionScript.h \
    src/DialogInputEventScript.h \
    src/ConfigOptions.h \
    src/json.h \
    src/TempWizard.h \
    src/dialognewanalog.h \
    src/TwoLineItemDelegate.h \
    src/version.h \
    src/WagoModbus.h \
    src/DialogWagoFirmwareUpdate.h \
    src/RuleActionMenu.h \
    src/common/FakeLogging.h \
    src/ftp/qurlinfo.h \
    src/ftp/qftp.h \
    src/dialognewonewiretemp.h \
    src/dialognewgpioinput.h \
    src/dialognewweboutputstring.h
FORMS += \
    data/FormConditionStd.ui \
    data/FormActionStd.ui \
    data/FormConditionStart.ui \
    data/FormActionMail.ui \
    data/DialogMailMessage.ui \
    data/FormActionScript.ui \
    data/DialogScriptEditor.ui \
    data/FormActionTouchscreen.ui \
    data/FormConditionScript.ui \
    data/DialogInputEventScript.ui \
    data/DialogWagoFirmwareUpdate.ui \
    data/DialogNewGpioInput.ui \
    data/DialogNewOneWireTemp.ui \
    data/DialogCameraView.ui \
    data/DialogConnect.ui \
    data/DialogDaliDeviceConfig.ui \
    data/DialogDetectSqueezebox.ui \
    data/DialogNewAnalog.ui \
    data/DialogNewAudio.ui \
    data/DialogNewCamera.ui \
    data/DialogNewDali.ui \
    data/DialogNewDaliRGB.ui \
    data/DialogNewInternal.ui \
    data/DialogNewRoom.ui \
    data/DialogNewRule.ui \
    data/DialogNewScenario.ui \
    data/DialogNewTemp.ui \
    data/DialogNewTime.ui \
    data/DialogNewVolet.ui \
    data/DialogNewWago.ui \
    data/DialogOpenOnline.ui \
    data/DialogPlageHoraire.ui \
    data/DialogPlageInput.ui \
    data/DialogSaveOnline.ui \
    data/MainWindow.ui \
    data/DialogIoList.ui \
    data/DialogIoProperties.ui \
    data/FormRules.ui \
    data/FormDali.ui \
    data/DialogNewWebOutputString.ui
RESOURCES += data/resources.qrc \
    data/textedit.qrc \
    data/wago_firmwares.qrc
INCLUDEPATH += src/common/ \
    src/common/LuaScript \
    Calaos/ \
    src/
DEFINES += CALAOS_INSTALLER
OTHER_FILES += \
    calaos_installer_en.ts

win32|mac {
SOURCES += src/common/LuaScript/lua-5.1.4/src/linit.c \
    src/common/LuaScript/lua-5.1.4/src/ltablib.c \
    src/common/LuaScript/lua-5.1.4/src/lstrlib.c \
    src/common/LuaScript/lua-5.1.4/src/loslib.c \
    src/common/LuaScript/lua-5.1.4/src/loadlib.c \
    src/common/LuaScript/lua-5.1.4/src/lmathlib.c \
    src/common/LuaScript/lua-5.1.4/src/liolib.c \
    src/common/LuaScript/lua-5.1.4/src/ldblib.c \
    src/common/LuaScript/lua-5.1.4/src/lbaselib.c \
    src/common/LuaScript/lua-5.1.4/src/lauxlib.c \
    src/common/LuaScript/lua-5.1.4/src/lzio.c \
    src/common/LuaScript/lua-5.1.4/src/lvm.c \
    src/common/LuaScript/lua-5.1.4/src/lundump.c \
    src/common/LuaScript/lua-5.1.4/src/ltm.c \
    src/common/LuaScript/lua-5.1.4/src/ltable.c \
    src/common/LuaScript/lua-5.1.4/src/lstring.c \
    src/common/LuaScript/lua-5.1.4/src/lstate.c \
    src/common/LuaScript/lua-5.1.4/src/lparser.c \
    src/common/LuaScript/lua-5.1.4/src/lopcodes.c \
    src/common/LuaScript/lua-5.1.4/src/lobject.c \
    src/common/LuaScript/lua-5.1.4/src/lmem.c \
    src/common/LuaScript/lua-5.1.4/src/llex.c \
    src/common/LuaScript/lua-5.1.4/src/lgc.c \
    src/common/LuaScript/lua-5.1.4/src/lfunc.c \
    src/common/LuaScript/lua-5.1.4/src/ldump.c \
    src/common/LuaScript/lua-5.1.4/src/ldo.c \
    src/common/LuaScript/lua-5.1.4/src/ldebug.c \
    src/common/LuaScript/lua-5.1.4/src/lcode.c \
    src/common/LuaScript/lua-5.1.4/src/lapi.c

HEADERS += src/common/LuaScript/lua-5.1.4/src/luaconf.h \
    src/common/LuaScript/lua-5.1.4/src/lualib.h \
    src/common/LuaScript/lua-5.1.4/src/llimits.h \
    src/common/LuaScript/lua-5.1.4/src/lauxlib.h \
    src/common/LuaScript/lua-5.1.4/src/lzio.h \
    src/common/LuaScript/lua-5.1.4/src/lvm.h \
    src/common/LuaScript/lua-5.1.4/src/lundump.h \
    src/common/LuaScript/lua-5.1.4/src/ltm.h \
    src/common/LuaScript/lua-5.1.4/src/ltable.h \
    src/common/LuaScript/lua-5.1.4/src/lstring.h \
    src/common/LuaScript/lua-5.1.4/src/lstate.h \
    src/common/LuaScript/lua-5.1.4/src/lparser.h \
    src/common/LuaScript/lua-5.1.4/src/lopcodes.h \
    src/common/LuaScript/lua-5.1.4/src/lobject.h \
    src/common/LuaScript/lua-5.1.4/src/lmem.h \
    src/common/LuaScript/lua-5.1.4/src/llex.h \
    src/common/LuaScript/lua-5.1.4/src/lgc.h \
    src/common/LuaScript/lua-5.1.4/src/lfunc.h \
    src/common/LuaScript/lua-5.1.4/src/ldo.h \
    src/common/LuaScript/lua-5.1.4/src/ldebug.h \
    src/common/LuaScript/lua-5.1.4/src/lcode.h \
    src/common/LuaScript/lua-5.1.4/src/lapi.h \
    src/common/LuaScript/lua-5.1.4/etc/lua.hpp

INCLUDEPATH += src/common/LuaScript/lua-5.1.4/src/ \
    src/common/LuaScript/lua-5.1.4/etc/
}

win32 {
    RC_FILE = win32/windows_res.rc
}

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += lua5.1
}

 TRANSLATIONS = calaos_installer_en.ts
