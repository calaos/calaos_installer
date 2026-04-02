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

# Inject version into Flatpak metainfo
DATE="$(date +%Y-%m-%d)"
sed -i "s|<release version=\"0.0.0\" date=\"2024-01-01\">|<release version=\"${VERSION}\" date=\"${DATE}\">|" \
    flatpak/fr.calaos.CalaosInstaller.metainfo.xml
sed -i "s|<p>Development build</p>|<p>Calaos Installer ${VERSION}</p>|" \
    flatpak/fr.calaos.CalaosInstaller.metainfo.xml
echo "Metainfo updated with version $VERSION ($DATE)"

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
flatpak install --user -y --noninteractive flathub org.kde.Sdk//6.10 org.kde.Platform//6.10

# Build the Flatpak
echo "Building Flatpak..."
flatpak-builder \
    --user \
    --force-clean \
    --default-branch=stable \
    --install-deps-from=flathub \
    --repo=flatpak-repo \
    flatpak-build \
    flatpak/fr.calaos.CalaosInstaller.yml

# Create a single-file .flatpak bundle for distribution
echo "Creating Flatpak bundle..."
flatpak build-bundle \
    flatpak-repo \
    "calaos_installer-${VERSION}.flatpak" \
    fr.calaos.CalaosInstaller \
    stable

echo "Flatpak bundle created: calaos_installer-${VERSION}.flatpak"
echo "Bundle size: $(du -h "calaos_installer-${VERSION}.flatpak" | cut -f1)"
