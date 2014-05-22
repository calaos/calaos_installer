#!/bin/bash

#Build script for Calaos Installer
#Start the script in an msysgit env with Qt5.3 installed in /c/Qt/Qt5.3.0/

set -e

MODE="$1"

BASEDIR=`pwd`
SRCDIR="$BASEDIR/.."
RELEASEDIR="$SRCDIR/../calaos_installer_release"

function build_bin()
{
    OLDPATH=$PATH
    export PATH="/c/Qt/Qt5.3.0/5.3/mingw482_32/bin:/c/Qt/Qt5.3.0/Tools/mingw482_32/bin:$PATH"
    ( cd $SRCDIR;
	  rm -fr build;
      mkdir build;
      cd build;
      qmake.exe ../calaos_installer.pro -r -spec win32-g++
      mingw32-make.exe -j8 -f Makefile.Release
    )
    export PATH=$OLDPATH

    #install binary
    cp $SRCDIR/build/release/calaos_installer.exe $RELEASEDIR
}

function build_setup()
{
    VERS=$1

    OLDPATH=$PATH
    export PATH="/c/Program Files (x86)/Inno Setup 5:$PATH"
    ( cd $BASEDIR;
	  rm -fr build
	  mkdir -p build
      echo "#define MyAppVersion \"$VERS\"" > build.iss
      cat installer.iss >> build.iss
      iscc build.iss
	  rm -f build.iss
    )
    export PATH=$OLDPATH
}

function build_all()
{
    #checkout the repo to the latest tag
    RELTAG="$(cd $SRCDIR; git describe --long --tags --always master)"

	echo "Building Calaos Installer version: $RELTAG"
    build_bin
    build_setup $RELTAG
}

build_all
