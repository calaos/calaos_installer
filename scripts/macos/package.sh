#!/bin/bash

set -ev

#
# creating the CalaosInstaller.dmg with Applications link
#

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $SCRIPTDIR/../funcs.sh

VERSION="$(get_version .)"

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
$QTDIR/bin/macdeployqt build/$APP.app -verbose=3 \
    -executable=build/$APP.app/Contents/MacOS/calaos_installer \
    -executable=build/$APP.app/Contents/MacOS/calaos_machinecreator
if [ "$?" -ne "0" ]; then
    echo "Failed to run macdeployqt"
    # remove keys
 #   security delete-keychain osx-build.keychain 
    exit 1
fi

#Call fix to change all rpath
#python3 ./scripts/macos/macdeployqtfix.py build/$APP.app/Contents/MacOS/calaos_installer /usr/local/Cellar/qt6/6.*/
#python3 ./scripts/macos/macdeployqtfix.py build/$APP.app/Contents/MacOS/calaos_machinecreator /usr/local/Cellar/qt6/6.*/

KEYCHAIN="build.keychain"

# Use first ID
security find-identity -v $KEYCHAIN
export ID=$(security find-identity -v $KEYCHAIN | grep "1)" | sed "s/^ *1) *\([^ ]*\).*/\1/")
echo "Using ID: $ID"

#Sign binaries!
codesign --deep --force --options runtime --verbose --sign $ID --keychain $KEYCHAIN build/$APP.app

echo "Verifying code signed app"
codesign --verify --verbose=4 build/$APP.app
spctl --assess --verbose=4 --raw build/$APP.app

#install appdmg https://github.com/LinusU/node-appdmg a tool to create awesome dmg !
echo "Creating .dmg"
npm install -g appdmg
appdmg macos/appdmg.json build/$APP-$VERSION.dmg

#sign dmg
echo "Sign dmg"
codesign --force --verify --verbose --sign "$ID" build/$APP-$VERSION.dmg

# export filename
echo "PACKAGE_FILE_NAME=build/$APP-$VERSION.dmg" >> $GITHUB_ENV
