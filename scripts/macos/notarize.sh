#!/bin/bash

set -ev

# Submits the signed dmg built by package.sh to Apple notarization.
# Upload to dl-direct is done by the publish workflow, once every platform has built.

APP=calaos_installer

# The publish workflow builds before tagging: only the version it injects names the dmg.
if [ -z "${RELEASE_VERSION:-}" ]; then
    echo "RELEASE_VERSION is not set: run this from .github/workflows/publish.yml"
    exit 1
fi
DMG="build/$APP-$RELEASE_VERSION.dmg"
if [ ! -f "$DMG" ]; then
    echo "$DMG not found: package.sh did not build version $RELEASE_VERSION"
    exit 1
fi

xcrun notarytool submit "$DMG" \
    --apple-id "$MACOS_NOTARIZATION_USERNAME" \
    --password "$MACOS_NOTARIZATION_PASSWORD" \
    --team-id "$MACOS_NOTARIZATION_TEAMID" \
    --wait
