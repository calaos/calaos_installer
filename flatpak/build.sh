#!/bin/bash
set -ev

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
REPODIR="$SCRIPTDIR/.."
source "$REPODIR/scripts/funcs.sh"

echo "=== Calaos Installer Flatpak Build ==="

cd "$REPODIR"

# Generate version header
make_version .

VERSION="$(get_version .)"
echo "Building version: $VERSION"

# Install flatpak and flatpak-builder if not already present
if ! command -v flatpak-builder &> /dev/null; then
    echo "Installing flatpak-builder..."
    sudo apt-get update -qq
    sudo apt-get install -y -qq flatpak flatpak-builder
fi

# Add Flathub remote for KDE SDK/runtime dependencies
flatpak remote-add --user --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo

# Install the required runtime and SDK
echo "Installing KDE SDK and Platform runtime..."
flatpak install --user -y --noninteractive flathub org.kde.Sdk//6.8 org.kde.Platform//6.8

# Build the Flatpak
echo "Building Flatpak..."
flatpak-builder \
    --user \
    --force-clean \
    --install-deps-from=flathub \
    --repo=flatpak-repo \
    flatpak-build \
    flatpak/fr.calaos.CalaosInstaller.yml

# Create a single-file .flatpak bundle for distribution
echo "Creating Flatpak bundle..."
flatpak build-bundle \
    flatpak-repo \
    "calaos_installer-${VERSION}.flatpak" \
    fr.calaos.CalaosInstaller

echo "Flatpak bundle created: calaos_installer-${VERSION}.flatpak"
echo "Bundle size: $(du -h "calaos_installer-${VERSION}.flatpak" | cut -f1)"
