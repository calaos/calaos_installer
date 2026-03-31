#!/bin/env bash

set -ev

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $SCRIPTDIR/funcs.sh

upload_token=$1
reltype=$2

PKDIR=/calaos_installer/packages
mkdir -p $PKDIR

if [ -z "$upload_token" ]
then
    echo "No upload token"
    exit 1
fi

if [ -z "$reltype" ]
then
    reltype="experimental"
fi

export UPLOAD_KEY=$upload_token

cd /calaos_installer

upload_file $PKDIR/*.exe $(sha256sum $PKDIR/*.exe | cut -d' ' -f1) "$reltype/calaos_installer/windows"

