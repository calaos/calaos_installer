#!/bin/bash
set -ev

echo "install.sh"

brew update > /dev/null
brew install qt6 cmake pkg-config xz zstd

#get extra-cmake-modules and build it
git clone https://invent.kde.org/frameworks/extra-cmake-modules.git
cd extra-cmake-modules
git checkout v6.24.0
cmake -B build -S . \
        -DCMAKE_INSTALL_PREFIX="$(brew --prefix qt6)" \
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
        -DCMAKE_INSTALL_PREFIX="$(brew --prefix qt6)" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DBUILD_TESTING=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_FIND_FRAMEWORK=LAST \
        -Wno-dev
cmake --build build
cmake --install build
cd ..

#get qtmqtt and build it
git clone https://github.com/qt/qtmqtt.git
cd qtmqtt
git checkout v6.10.2
cmake -B build -S . \
        -DCMAKE_INSTALL_PREFIX="$(brew --prefix qt6)" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DFEATURE_framework=OFF \
        -DBUILD_TESTING=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_FIND_FRAMEWORK=LAST \
        -Wno-dev
cmake --build build
cmake --install build

# Workaround: qtmqtt ignores -DFEATURE_framework=OFF and still installs as a
# macOS framework.  The generated .pri file uses QT_MODULE_LIB_BASE which qmake
# resolves to QT_INSTALL_LIBS (e.g. /opt/homebrew/lib), NOT necessarily the same
# as $(brew --prefix qt6)/lib (e.g. /opt/homebrew/opt/qt/lib).  The -I flag in
# the compile command points to QT_INSTALL_LIBS/QtMqtt.framework/Headers, so the
# symlink must exist there.  We create it at both paths to be safe.
QT_PREFIX="$(brew --prefix qt6)"
QT_LIBS="$(qmake -query QT_INSTALL_LIBS)"

for libdir in "$QT_PREFIX/lib" "$QT_LIBS"; do
    if [ -d "$libdir/QtMqtt.framework/Headers" ]; then
        ln -sf . "$libdir/QtMqtt.framework/Headers/QtMqtt"
        echo "Created symlink workaround at $libdir/QtMqtt.framework/Headers/QtMqtt"
    fi
done

cd ..
