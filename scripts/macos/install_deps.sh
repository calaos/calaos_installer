#!/bin/bash
set -ev

echo "install.sh"

brew update > /dev/null
brew install cmake pkg-config xz zstd

# The CI pins Qt with aqt (install-qt-action sets QT_ROOT_DIR): Homebrew's qt6
# moves to every new release. Without it, a local build takes Homebrew's.
if [ -z "$QT_ROOT_DIR" ]; then
    brew install qt6
    QT_ROOT_DIR="$(brew --prefix qt6)"
fi

#get extra-cmake-modules and build it
git clone https://invent.kde.org/frameworks/extra-cmake-modules.git
cd extra-cmake-modules
git checkout v6.24.0
cmake -B build -S . \
        -DCMAKE_INSTALL_PREFIX="$QT_ROOT_DIR" \
        -DCMAKE_PREFIX_PATH="$QT_ROOT_DIR" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DBUILD_TESTING=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_FIND_FRAMEWORK=LAST \
        -Wno-dev
cmake --build build
cmake --install build
cd ..

#get karchive and build it
git clone https://invent.kde.org/frameworks/karchive.git
cd karchive
git checkout v6.24.0
cmake -B build -S . \
        -DCMAKE_INSTALL_PREFIX="$QT_ROOT_DIR" \
        -DCMAKE_PREFIX_PATH="$QT_ROOT_DIR" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DBUILD_TESTING=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_FIND_FRAMEWORK=LAST \
        -Wno-dev
cmake --build build
cmake --install build
cd ..
