# Standalone qmake project: not included by calaos_installer.pro.
#   mkdir -p /tmp/installer_tests_build && cd /tmp/installer_tests_build
#   qmake6 <repo>/tests/tests.pro && make -j$(nproc) && make check
TEMPLATE = subdirs

SUBDIRS += tst_projectmanager tst_devices tst_widgetioproperties tst_analogbounds tst_iodoccatalogue tst_roomteardown tst_teardownorder tst_iomove tst_deleteimpact tst_hometree tst_servertypes tst_onlineconfig tst_devicecommand tst_devicetemplates tst_dialognewdevice tst_deviceedit tst_timezonehelper tst_builddirs tst_apptheme
