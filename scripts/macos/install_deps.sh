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
# as $(brew --prefix qt6)/lib (e.g. /opt/homebrew/opt/qt/lib).  The compiler's
# -I flag points to QT_INSTALL_LIBS/QtMqtt.framework/Headers, so the framework
# must be reachable from QT_INSTALL_LIBS.
#
# Two symlinks are needed:
# 1. Framework-level: QT_INSTALL_LIBS/QtMqtt.framework -> actual install location
#    (mirrors what Homebrew does for its own Qt frameworks)
# 2. Inner Headers:   QtMqtt.framework/Headers/QtMqtt -> .
#    (so #include <QtMqtt/QMqttClient> resolves inside the framework)
QT_PREFIX="$(brew --prefix qt6)"
QT_LIBS="$(qmake -query QT_INSTALL_LIBS)"

# Step 1: If QT_INSTALL_LIBS differs from the install prefix, create a
# framework-level symlink so the compiler can find QtMqtt.framework
if [ "$QT_LIBS" != "$QT_PREFIX/lib" ] && [ ! -e "$QT_LIBS/QtMqtt.framework" ]; then
    ln -sf "$QT_PREFIX/lib/QtMqtt.framework" "$QT_LIBS/QtMqtt.framework"
    echo "Created framework symlink $QT_LIBS/QtMqtt.framework -> $QT_PREFIX/lib/QtMqtt.framework"
fi

# Step 2: Create the inner Headers/QtMqtt -> . symlink at both paths
for libdir in "$QT_PREFIX/lib" "$QT_LIBS"; do
    if [ -d "$libdir/QtMqtt.framework/Headers" ]; then
        ln -sf . "$libdir/QtMqtt.framework/Headers/QtMqtt"
        echo "Created Headers/QtMqtt symlink at $libdir/QtMqtt.framework/"
    fi
done

cd ..
