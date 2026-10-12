# Standalone qmake project: not included by calaos_installer.pro.
#   mkdir -p /tmp/installer_tests_build && cd /tmp/installer_tests_build
#   qmake6 <repo>/tests/tests.pro && make -j$(nproc) && make check
TEMPLATE = subdirs

SUBDIRS += tst_projectmanager tst_devices tst_widgetioproperties tst_roomteardown tst_teardownorder
