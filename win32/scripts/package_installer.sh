#!/bin/env bash

set -ev

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $SCRIPTDIR/funcs.sh

build_dir=/calaos_installer/build_win
build_mc_dir=/calaos_installer/build_win/mc
MXE_BIN=/mxe/usr/i686-w64-mingw32.shared.posix
WDIR=$HOME/.wine/drive_c/calaos_installer_build
PKDIR=/calaos_installer/packages
mkdir -p $PKDIR

cd /calaos_installer
VERSION="$(get_version .)"
FILENAME=calaos_installer_setup_$VERSION

mkdir -p $WDIR
cd $build_dir/..

for f in $MXE_BIN/bin/libgcc_s_sjlj-1.dll \
         $MXE_BIN/bin/libstdc++-6.dll \
         $MXE_BIN/bin/libwinpthread-1.dll \
         $MXE_BIN/bin/zlib1.dll \
         $MXE_BIN/bin/libcrypto-3.dll \
         $MXE_BIN/bin/libssl-3.dll \
         $MXE_BIN/bin/icudt74.dll \
         $MXE_BIN/bin/icuin74.dll \
         $MXE_BIN/bin/icuuc74.dll \
         $MXE_BIN/bin/libzstd.dll \
         $MXE_BIN/bin/liblzma-5.dll \
         $MXE_BIN/bin/libbz2.dll \
         $MXE_BIN/bin/libpng16-16.dll \
         $MXE_BIN/bin/libjpeg-9.dll \
         $MXE_BIN/bin/libfreetype-6.dll \
         $MXE_BIN/bin/libbrotlidec.dll \
         $MXE_BIN/bin/libbrotlicommon.dll \
         $MXE_BIN/bin/libKF5Archive.dll \
         $MXE_BIN/qt6/bin/Qt6Core.dll \
         $MXE_BIN/qt6/bin/Qt6Gui.dll \
         $MXE_BIN/qt6/bin/Qt6Network.dll \
         $MXE_BIN/qt6/bin/Qt6Widgets.dll \
         $MXE_BIN/qt6/bin/Qt6WebSockets.dll \
         $MXE_BIN/qt6/bin/Qt6Xml.dll \
         $MXE_BIN/qt6/bin/Qt6PrintSupport.dll \
         $MXE_BIN/qt6/bin/Qt6Core5Compat.dll \
         $MXE_BIN/qt6/bin/Qt6Mqtt.dll \
         $MXE_BIN/qt6/bin/Qt6Quick.dll \
         $MXE_BIN/qt6/bin/Qt6Qml.dll \
         $MXE_BIN/qt6/bin/Qt6QmlModels.dll \
         $MXE_BIN/qt6/bin/Qt6OpenGL.dll \
         $MXE_BIN/qt6/bin/Qt6QuickControls2.dll \
         $MXE_BIN/qt6/bin/Qt6QuickTemplates2.dll \
         $MXE_BIN/qt6/bin/Qt6QuickWidgets.dll \
         $MXE_BIN/qt6/bin/Qt6SerialPort.dll \
         $MXE_BIN/qt6/bin/Qt6QmlWorkerScript.dll \
         $MXE_BIN/qt6/bin/Qt6QuickControls2Impl.dll \
         $MXE_BIN/qt6/bin/Qt6QuickLayouts.dll \
         $MXE_BIN/qt6/plugins/imageformats \
         $MXE_BIN/qt6/plugins/platforms \
         $MXE_BIN/qt6/plugins/styles \
         $MXE_BIN/qt6/plugins/tls \
         $MXE_BIN/qt6/qml/QtQml  \
	     $MXE_BIN/qt6/qml/QtCore \
	     $MXE_BIN/qt6/qml/QtQuick \
         $build_dir/release/calaos_installer.exe \
         $build_mc_dir/release/calaos_machinecreator.exe
do
    cp -R $f $WDIR
done

pushd win32

echo "#define MyAppVersion \"$VERSION\"" > build.iss
cat installer.iss >> build.iss
iscc build.iss

mv build/$FILENAME.exe $PKDIR

popd

