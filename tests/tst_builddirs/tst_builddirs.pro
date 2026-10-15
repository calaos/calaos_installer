QT += core testlib

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_builddirs
TEMPLATE = app

DEFINES += CALAOS_SOURCE_DIR=\\\"$$clean_path($$PWD/../..)\\\"

SOURCES += tst_builddirs.cpp
