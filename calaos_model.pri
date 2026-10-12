# Data model and io.xml/rules.xml (de)serialisation, shared by the app and tests/.
# No GUI dependency beyond QMessageBox in projectmanager.cpp (QT += widgets).

SOURCES += \
    $$PWD/src/common/base64.c \
    $$PWD/src/common/Params.cpp \
    $$PWD/src/common/Utils.cpp \
    $$PWD/src/SimpleCrypt.cpp \
    $$PWD/src/ConfigOptions.cpp \
    $$PWD/src/projectmanager.cpp \
    $$PWD/Calaos/Rule.cpp \
    $$PWD/Calaos/Room.cpp \
    $$PWD/Calaos/ListeRule.cpp \
    $$PWD/Calaos/ListeRoom.cpp \
    $$PWD/Calaos/IOFactory.cpp \
    $$PWD/Calaos/Condition.cpp \
    $$PWD/Calaos/Action.cpp \
    $$PWD/Calaos/IOBase.cpp \
    $$PWD/Calaos/TimeRange.cpp

HEADERS += \
    $$PWD/src/SimpleCrypt.h \
    $$PWD/src/ConfigOptions.h \
    $$PWD/src/projectmanager.h \
    $$PWD/src/common/Utils.h \
    $$PWD/src/common/base64.h \
    $$PWD/src/common/Params.h \
    $$PWD/Calaos/IOBase.h \
    $$PWD/Calaos/Rule.h \
    $$PWD/Calaos/Room.h \
    $$PWD/Calaos/ListeRule.h \
    $$PWD/Calaos/ListeRoom.h \
    $$PWD/Calaos/IOFactory.h \
    $$PWD/Calaos/Condition.h \
    $$PWD/Calaos/Action.h \
    $$PWD/Calaos/TimeRange.h

INCLUDEPATH += \
    $$PWD/src/common \
    $$PWD/Calaos \
    $$PWD/src
