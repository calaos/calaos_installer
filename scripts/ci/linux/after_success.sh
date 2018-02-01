#!/bin/bash
set -ev

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $SCRIPTDIR/../funcs.sh

MXE_BIN=$HOME/mxe/usr/i686-w64-mingw32.shared.posix
WDIR=$HOME/.wine/drive_c/calaos_installer_build

VERSION="$(get_version .)"

FILENAME=calaos_installer_setup_$VERSION

#Only build if the commit we are building is for the last tag
if [ "$(git rev-list -n 1 $VERSION)" != "$(cat .git/HEAD)"  ]; then
    echo "Not uploading package"
    return 0
fi

mkdir -p $WDIR/redist

#Get 3rd party tools
wget_retry https://calaos.fr/download/misc/redist/Win32OpenSSL_Light-1_0_2L.exe -O $WDIR/redist/Win32OpenSSL_Light-1_0_2L.exe
wget_retry https://calaos.fr/download/misc/redist/vcredist_sp1_x86.exe -O $WDIR/redist/vcredist_sp1_x86.exe

for f in $MXE_BIN/bin/libgcc_s_sjlj-1.dll \
         $MXE_BIN/bin/libstdc++-6.dll \
         $MXE_BIN/bin/icudt56.dll \
         $MXE_BIN/bin/icuin56.dll \
         $MXE_BIN/bin/icuuc56.dll \
         $MXE_BIN/bin/libwinpthread-1.dll \
         $MXE_BIN/bin/libwebp-5.dll \
         $MXE_BIN/bin/zlib1.dll \
         $MXE_BIN/qt5/bin/Qt5Core.dll \
         $MXE_BIN/qt5/bin/Qt5Gui.dll \
         $MXE_BIN/qt5/bin/Qt5Network.dll \
         $MXE_BIN/qt5/bin/Qt5Widgets.dll \
         $MXE_BIN/qt5/bin/Qt5WebSockets.dll \
         $MXE_BIN/qt5/plugins/imageformats \
         $MXE_BIN/qt5/plugins/platforms \
         $HOME/build/calaos/calaos_installer/build/release/calaos_installer.exe \
         $HOME/build/calaos/calaos_installer/build/machine_creator/release/calaos_machinecreator.exe
do
    cp -R $f $WDIR
done

pwd
ls -al

pushd win32

echo "#define MyAppVersion \"$VERSION\"" > build.iss
cat installer.iss >> build.iss
chmod +x iscc
./iscc build.iss

upload_file build/$FILENAME.exe $(sha256sum build/$FILENAME.exe | cut -d' ' -f1) "experimental/calaos_installer/windows"

popd
