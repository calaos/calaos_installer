# -------------------------------------------------
# Project created by QtCreator 2009-04-27T17:51:09
# -------------------------------------------------
QT += core gui network widgets xml printsupport

isEmpty(REVISION) {
REVISION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --long --tags --always master)
}
DEFINES += CALAOS_VERSION=\\\"$$REVISION\\\"

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
    src/dialognewwebio.cpp \
    src/dialogcameraview.cpp \
    src/wagoconnect.cpp \
    src/dialogconnect.cpp \
    src/dialogopenonline.cpp \
    src/qanimationlabel.cpp \
    src/dialogsaveonline.cpp \
    src/formpost.cpp \
    src/dialogiolist.cpp \
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
    Calaos/IOBase.cpp \
    src/DialogEditTimeRange.cpp \
    Calaos/TimeRange.cpp \
    src/DialogNewAVReceiver.cpp \
    src/DialogNewGpioLight.cpp \
    src/DialogNewGpioShutter.cpp \
    src/DialogNewX10.cpp \
    src/DialogNewZibase.cpp \
    src/DialogOptions.cpp \
    src/dialognewwebioshutter.cpp \
    src/formanalogproperties.cpp \
    src/DialogNewZibaseOutput.cpp \
    src/dialogautodetect.cpp \
    src/DialogNewMySensors.cpp \
    src/DialogOla.cpp \
    src/DialogNewPing.cpp \
    src/DialogNewWOL.cpp \
    src/DialogCreateNewImage.cpp \
    src/DialogListProperties.cpp \
    src/BalloonTip.cpp

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
    src/DialogEditTimeRange.h \
    Calaos/TimeRange.h \
    src/DialogNewAVReceiver.h \
    src/dialognewwebio.h \
    src/DialogNewGpioLight.h \
    src/DialogNewGpioShutter.h \
    src/DialogNewX10.h \
    src/DialogNewZibase.h \
    src/DialogOptions.h \
    src/dialognewwebioshutter.h \
    src/formanalogproperties.h \
    src/DialogNewZibaseOutput.h \
    src/dialogautodetect.h \
    src/DialogNewMySensors.h \
    src/DialogOla.h \
    src/DialogNewPing.h \
    src/DialogNewWOL.h \
    src/DialogCreateNewImage.h \
    src/DialogListProperties.h \
    src/BalloonTip.h

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
    data/DialogEditTimeRange.ui \
    data/DialogNewAVReceiver.ui \
    data/DialogNewWebIO.ui \
    data/DialogNewGpioLight.ui \
    data/DialogNewGpioShutter.ui \
    data/DialogNewX10.ui \
    data/DialogNewZibase.ui \
    data/DialogOptions.ui \
    data/DialogNewWebIoShutter.ui \
    data/formanalogproperties.ui \
    data/DialogNewZibaseOutput.ui \
    data/dialogautodetect.ui \
    data/DialogNewMySensors.ui \
    data/DialogOla.ui \
    data/DialogNewPing.ui \
    data/DialogNewWOL.ui \
    src/DialogCreateNewImage.ui \
    src/DialogListProperties.ui

RESOURCES += data/resources.qrc \
    data/textedit.qrc \
    data/wago_firmwares.qrc \
    lang/lang_ressources.qrc \
    data/iodoc.qrc

win32 {
#on windows to deploy we need to add Qt own language files
#to have all Qt dialogs/strings translated as well
RESOURCES += lang/lang_ressources_qt.qrc
}

INCLUDEPATH += src/common/ \
    src/common/LuaScript \
    Calaos/ \
    src/
DEFINES += CALAOS_INSTALLER

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

win32 {
    RC_FILE = win32/windows_res.rc
}

TRANSLATIONS = \
    lang/calaos_installer_fr.ts \
    lang/calaos_installer_de.ts \
    lang/calaos_installer_es.ts \ 
    lang/calaos_installer_nb.ts \
    lang/calaos_installer_hi.ts

#Build *.qm translation files automatically

isEmpty(QMAKE_LRELEASE) {
    QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += updateqm
PRE_TARGETDEPS += compiler_updateqm_make_all

