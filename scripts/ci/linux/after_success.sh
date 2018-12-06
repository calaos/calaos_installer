#!/bin/bash
set -ev

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $SCRIPTDIR/../funcs.sh

VERSION="$(get_version .)"
FILENAME=calaos_installer_setup_$VERSION

#Only build if the commit we are building is for the last tag
if [ "$(git rev-list -n 1 $VERSION)" != "$(cat .git/HEAD)"  ]; then
    echo "Not uploading package"
    return 0
fi

docker exec winbuilder /bin/bash /scripts/package_installer.sh

upload_file packages/$FILENAME.exe $(sha256sum build/$FILENAME.exe | cut -d' ' -f1) "experimental/calaos_installer/windows"

popd
