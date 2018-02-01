#!/bin/bash
set -ev

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $SCRIPTDIR/../funcs.sh

mkdir -p build
pushd build

QTDIR="$(brew --prefix qt@5.10)"
PATH="$QTDIR/bin:$PATH"
LDFLAGS=-L$QTDIR/lib
CPPFLAGS=-I$QTDIR/include

make_version ..
qmake ../calaos_installer.pro
make
mkdir machine_creator
cd machine_creator
qmake ../../machine_creator/machine_creator.pro
make

popd
