#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <version>"
	exit 1
fi

VERSION=$1

echo "Copy files..."
cp Info.plist ../../calaos_installer-build/calaos_installer.app/Contents/
cp icon.icns ../../calaos_installer-build/calaos_installer.app/Contents/Resources/

echo "Creating dmg package..."
macdeployqt ../../calaos_installer-build/calaos_installer.app -dmg
mv ../../calaos_installer-build/calaos_installer.dmg ../../CalaosInstaller_macosx_$VERSION.dmg

echo "Done."
echo "Package: ../../CalaosInstaller_macosx_$VERSION.dmg"
