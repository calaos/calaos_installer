#!/bin/bash

set -e

export PROJECT_NAME=calaos_installer
TRAVIS_BUILD_DIR=$(pwd)
export TRAVIS_BUILD_DIR
BUILD_TAG=$(git tag --points-at=HEAD --sort version:refname | head -n 1)
export BUILD_TAG

#Usage: get_version /path/to/repo
# RELEASE_VERSION is set by the publish workflow: it builds before the tag exists.
function get_version()
{
    if [ -n "${RELEASE_VERSION:-}" ]; then
        echo "$RELEASE_VERSION"
        return
    fi
    pushd "$1" > /dev/null
    git describe --tags --abbrev=0
    popd > /dev/null
}

#Usage: make_version /path/to/repo
function make_version()
{
    local version
    version=$(get_version "$1")
    {
        echo "#ifndef VERSION__H"
        echo "#define VERSION__H"
        echo "#define APP_VERSION \"$version\""
        echo "const char *calaos_installer_version = APP_VERSION;"
        echo "#endif"
    } > "$1/src/version.h"
}

function wget_retry()
{
    count=0
    while [ $count -le 4 ]; do
        echo Downloading "$@"
        set +e
        wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 0 --continue "$@"
        ret=$?
        set -e
        if [ $ret = 0 ]
        then
            return 0
        fi
        sleep 1
        count=$((count+1))
        echo Download failed.
    done
    return 1
}

#Usage: upload_file <file> <sha256> <folder>, with UPLOAD_KEY in the environment
function upload_file()
{
    FNAME=$1
    HASH=$2
    INSTALLPATH=$3

    curl --fail-with-body -X POST \
        -H "Content-Type: multipart/form-data" \
        -F "upload_key=$UPLOAD_KEY" \
        -F "upload_folder=$INSTALLPATH" \
        -F "upload_sha256=$HASH" \
        -F "upload_file=@$FNAME" \
        https://dl-direct.raoulh.pw/download/upload
}
