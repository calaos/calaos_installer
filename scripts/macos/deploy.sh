#!/bin/bash

set -ev

#
# creating the CalaosInstaller.dmg with Applications link
#

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $SCRIPTDIR/../funcs.sh

VERSION="$(get_version .)"

#Only build if the commit we are building is for the last tag
if [ "$(git rev-list -n 1 $VERSION)" != "$(cat .git/HEAD)"  ]; then
    echo "Not uploading package"
    #exit 0
fi

QTDIR="$(brew --prefix qt6)"
APP=calaos_installer

# copy icon
mkdir -p build/$APP.app/Contents/Resources/
cp macos/icon.icns build/$APP.app/Contents/Resources/

# copy Info.plist
cp macos/Info.plist build/$APP.app/Contents/Info.plist

# removing backup plist
rm -f build/$APP.app/Contents/Info.plist-e

# Copy machine_creator to bundle
cp build/machine_creator/calaos_machinecreator.app/Contents/MacOS/calaos_machinecreator build/$APP.app/Contents/MacOS/

cp macos/calaos_machinecreator.sh build/$APP.app/Contents/MacOS/
cp macos/main.scpt build/$APP.app/Contents/MacOS/

# use macdeployqt to deploy the application
#echo "Calling macdeployqt and code signing application"
#$QTDIR/bin/macdeployqt ./$APP.app -codesign="$DEVELOPER_NAME"
echo "Calling macdeployqt"
$QTDIR/bin/macdeployqt build/$APP.app \
    -executable=build/$APP.app/Contents/MacOS/calaos_installer \
    -executable=build/$APP.app/Contents/MacOS/calaos_machinecreator
if [ "$?" -ne "0" ]; then
    echo "Failed to run macdeployqt"
    # remove keys
 #   security delete-keychain osx-build.keychain 
    exit 1
fi

#Call fix to change all rpath
wget_retry https://raw.githubusercontent.com/aurelien-rainone/macdeployqtfix/master/macdeployqtfix.py

python macdeployqtfix.py build/$APP.app/Contents/MacOS/calaos_installer /usr/local/Cellar/qt5/5.*/
python macdeployqtfix.py build/$APP.app/Contents/MacOS/calaos_machinecreator /usr/local/Cellar/qt5/5.*/

#install appdmg https://github.com/LinusU/node-appdmg a tool to create awesome dmg !
npm install -g appdmg
appdmg macos/appdmg.json build/$APP-$VERSION.dmg

#upload_file build/$APP-$VERSION.dmg $(shasum -a 256 build/$APP-$VERSION.dmg | cut -d' ' -f1) "experimental/calaos_installer/osx"
