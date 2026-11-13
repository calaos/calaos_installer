#!/bin/bash
set -ev

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
# shellcheck source=scripts/funcs.sh
source "$SCRIPTDIR/../funcs.sh"

rm -fr build && mkdir -p build
pushd build

QTDIR="${QT_ROOT_DIR:-$(brew --prefix qt6)}"
PATH="$QTDIR/bin:$PATH"

make_version ..
# Without RELEASE_VERSION, calaos_installer.pro runs git describe itself.
QMAKE_ARGS=()
if [ -n "${RELEASE_VERSION:-}" ]; then
    QMAKE_ARGS+=("REVISION=$RELEASE_VERSION")
fi
qmake ../calaos_installer.pro "${QMAKE_ARGS[@]}"
make -j"$(sysctl -n hw.physicalcpu)"
mkdir machine_creator
cd machine_creator
qmake ../../machine_creator/machine_creator.pro
make -j"$(sysctl -n hw.physicalcpu)"

popd
