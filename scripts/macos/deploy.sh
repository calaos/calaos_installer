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

APP=calaos_installer

echo "Staple notarized app"
xcrun notarytool submit build/$APP-$VERSION.dmg \
    --apple-id "$MACOS_NOTARIZATION_USERNAME" \
    --password "$MACOS_NOTARIZATION_PASSWORD" \
    --team-id "$MACOS_NOTARIZATION_TEAMID" \
    --wait

if [ -z "$REL_TYPE" ]; then
    REL_TYPE="experimental"
fi

upload_file build/$APP-$VERSION.dmg $(shasum -a 256 build/$APP-$VERSION.dmg | cut -d' ' -f1) "$REL_TYPE/calaos_installer/osx"
