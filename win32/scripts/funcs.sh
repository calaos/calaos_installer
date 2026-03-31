#/bin/env bash

set -e

#Usage: get_version /path/to/repo
function get_version()
{
    repo=$1
    pushd $repo > /dev/null
    git describe --tags --abbrev=0
    popd > /dev/null
}

function wget_retry()
{
    count=0
    while [ $count -le 4 ]; do
        echo Downloading $@
        set +e
        wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 0 --continue $@
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

#signing of exe from server
#Usage sign_binary $Path/file.exe
function sign_binary()
{
    if [ ! -f /cert.p12 ]
    then
        echo "Ignoring codesign. Certificate is not installed."
        return 0
    fi

    set +e
    if [ -f $1 ]
    then
        echo "Signing binary file: $1"

        #sign SHA1
        osslsigncode sign \
            -pkcs12 /cert.p12 \
            -pass "$CODESIGN_WIN_PASS" \
            -h sha1 \
            -n "Moolticute" \
            -i "http://themooltipass.com" \
            -t http://timestamp.comodoca.com \
            -in "$1" -out "${1}_signed1"

        if [ ! $? -eq 0 ] ; then
            set -e
            rm ${1}_signed1
            echo "Failed to codesign SHA1"
            return 255
        fi

        #Append SHA256
        osslsigncode sign \
            -pkcs12 /cert.p12 \
            -pass "$CODESIGN_WIN_PASS" \
            -h sha256 \
            -n "Moolticute" \
            -i "http://themooltipass.com" \
            -ts http://timestamp.comodoca.com \
            -in "${1}_signed1" -out "${1}_signed2" \
            -nest

        if [ ! $? -eq 0 ] ; then
            set -e
            echo "Failed to codesign SHA256"
            return 255
        fi

        mv ${1}_signed2 $1
        rm ${1}_signed1
        fi
    set -e
}


#search for files in folder and sign all exe/dll
#Usage: find_and_sign $Path
function find_and_sign()
{
    Path=$1
    find $Path -iname "*.exe" -type f | while read file; do sign_binary "$file"; done
    find $Path -iname "*.dll" -type f | while read file; do sign_binary "$file"; done
}

function beginsWith()
{
    case $2 in "$1"*) true;; *) false;; esac;
}

function endsWith()
{
    case $2 in *"$1") true;; *) false;; esac;
}

function upload_file()
{
    FNAME=$1
    HASH=$2
    INSTALLPATH=$3

    curl -X POST \
        -H "Content-Type: multipart/form-data" \
        -F "upload_key=$UPLOAD_KEY" \
        -F "upload_folder=$INSTALLPATH" \
        -F "upload_sha256=$HASH" \
        -F "upload_file=@$FNAME" \
        https://calaos.fr/download/upload
}

