#!/bin/bash
set -ev

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $SCRIPTDIR/../funcs.sh

mkdir build
pushd build

export PATH=$HOME/mxe/usr/bin:$PATH
export MXE_BASE=$HOME/mxe

make_version ..

$MXE_BASE/usr/i686-w64-mingw32.shared.posix/qt5/bin/qmake ../calaos_installer.pro
make

mkdir machine_creator
cd machine_creator
$MXE_BASE/usr/i686-w64-mingw32.shared.posix/qt5/bin/qmake ../../machine_creator/machine_creator.pro
make

popd
