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
#    return 0
fi

QTDIR="/usr/local/opt/qt5"
APP=calaos_installer
# this directory name will also be shown in the title when the DMG is mounted
TEMPDIR=build/$APP
SIGNATURE="Raoul Hecky"
NAME=`uname`

if [ "$NAME" != "Darwin" ]; then
    echo "This is not a Mac"
    exit 1
fi

# copy icon
cp macos/icon.icns build/$APP.app/Contents/Resources/

# copy Info.plist
cp macos/Info.plist build/$APP.app/Contents/Info.plist

# removing backup plist
rm -f build/$APP.app/Contents/Info.plist-e

# Copy machine_creator to bundle
cp build/machine_creator/calaos_machinecreator.app/Contents/MacOS/calaos_machinecreator build/$APP.app/Contents/MacOS/
# use macdeployqt to deploy the application
#echo "Calling macdeployqt and code signing application"
#$QTDIR/bin/macdeployqt ./$APP.app -codesign="$DEVELOPER_NAME"
echo "Calling macdeployqt"
$QTDIR/bin/macdeployqt build/$APP.app
if [ "$?" -ne "0" ]; then
    echo "Failed to run macdeployqt"
    # remove keys
 #   security delete-keychain osx-build.keychain 
    exit 1
fi

#Call fix to change all rpath
wget https://raw.githubusercontent.com/aurelien-rainone/macdeployqtfix/master/macdeployqtfix.py
python macdeployqtfix.py build/$APP.app/Contents/MacOS/calaos_installer /usr/local/Cellar/qt5/5.6.1-1
python macdeployqtfix.py build/$APP.app/Contents/MacOS/calaos_machinecreator /usr/local/Cellar/qt5/5.6.1-1

#install appdmg https://github.com/LinusU/node-appdmg a tool to create awesome dmg !
npm install -g appdmg
appdmg macos/appdmg.json build/$APP-$VERSION.dmg


upload_file build/$APP-$VERSION.dmg $(shasum -a 256 build/$APP-$VERSION.dmg | cut -d' ' -f1) "macos"

exit 0

