#!/bin/bash

#Build script for Calaos Installer
#Start the script on a linux machine with win-builds installed and Qt5.4

set -e

BASEDIR=`pwd`
SRCDIR="$BASEDIR/.."
RELEASEDIR="$HOME/.wine/drive_c/calaos_installer_release"

BUILD_TYPE=$1

function build_bin()
{
    (
    set +e
    source /opt/cross_toolchain_32/bin/win-builds-switch 32
    set -e
    cd $SRCDIR
    rm -fr build
    mkdir build
    cd build
    qmake ../calaos_installer.pro
    make -j16
    )

    #install binary
    cp $SRCDIR/build/release/calaos_installer.exe $RELEASEDIR
}

function build_setup()
{
    VERS=$1

    for f in /opt/cross_toolchain_32/i686-w64-mingw32/lib/libgcc_s_sjlj-1.dll \
             /opt/cross_toolchain_32/i686-w64-mingw32/lib/libstdc++-6.dll \
             /opt/cross_toolchain_32/bin/libwinpthread-1.dll \
             /opt/windows_32/bin/libicudt51.dll \
             /opt/windows_32/bin/libicuin51.dll \
             /opt/windows_32/bin/libicuuc51.dll \
             /opt/windows_32/bin/libjpeg-9.dll \
             /opt/windows_32/bin/libz-1.dll \
             /opt/windows_32/bin/libpcre16-0.dll \
             /opt/windows_32/bin/Qt5Concurrent.dll \
             /opt/windows_32/bin/Qt5Core.dll \
             /opt/windows_32/bin/Qt5Gui.dll \
             /opt/windows_32/bin/Qt5Network.dll \
             /opt/windows_32/bin/Qt5PrintSupport.dll \
             /opt/windows_32/bin/Qt5Svg.dll \
             /opt/windows_32/bin/Qt5Widgets.dll \
             /opt/windows_32/bin/Qt5Xml.dll \
             /opt/windows_32/bin/Qt5XmlPatterns.dll \
             /opt/windows_32/plugins/iconengines \
             /opt/windows_32/plugins/imageformats \
             /opt/windows_32/plugins/platforms \
             /opt/windows_32/plugins/printsupport \
             $SRCDIR/win32/psvince.dll
    do
        cp -R $f $RELEASEDIR
    done

    mkdir $RELEASEDIR/redist
    for f in vcredist_sp1_x86.exe Win32OpenSSL_Light-1_0_1L.exe
    do
        wget https://calaos.fr/download/misc/redist/$f -O $RELEASEDIR/redist/$f
    done

    ( cd $SRCDIR/win32
      echo "#define MyAppVersion \"$VERS\"" > build.iss
      cat installer.iss >> build.iss
      iscc build.iss
    )

    echo "Done. Setup is : $SRCDIR/win32/build/calaos_installer_setup_${VERS}.exe"
}

function build_all()
{
    #checkout the repo to the latest tag
    RELTAG="$(cd $SRCDIR; git describe --long --tags --always master)"

	echo "Building Calaos Installer version: $RELTAG"
	rm -fr $RELEASEDIR
	mkdir -p $RELEASEDIR
    build_bin
    build_setup $RELTAG

    #upload
    type=experimental
    [ "$BUILD_TYPE" = "TESTING" ] && type=testing
    [ "$BUILD_TYPE" = "STABLE" ] && type=stable

    scp $SRCDIR/win32/build/calaos_installer_setup_${VERS}.exe uploader@calaos.fr:/home/raoul/www/download.calaos.fr/$type/calaos_installer/
}

build_all
