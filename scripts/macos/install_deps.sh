#!/bin/bash
set -ev

echo "install.sh"

brew update > /dev/null
brew install qt6 cmake pkg-config xz zstd

#get extra-cmake-modules and build it
git clone https://invent.kde.org/frameworks/extra-cmake-modules.git
cd extra-cmake-modules
git checkout v6.5.0
cmake -B build -S . \
        -DCMAKE_INSTALL_PREFIX="$(brew --prefix qt6)" \
	-DCMAKE_INSTALL_LIBDIR="$(brew --prefix qt6)" \
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
git checkout v6.5.0
cmake -B build -S . \
        -DCMAKE_INSTALL_PREFIX="$(brew --prefix qt6)" \
	-DCMAKE_INSTALL_LIBDIR="$(brew --prefix qt6)" \
        -DBUILD_TESTING=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_FIND_FRAMEWORK=LAST \
        -Wno-dev \
        -DBUILD_WITH_QT6=ON \
        -DQT_MAJOR_VERSION=6
cmake --build build
cmake --install build
cd ..

#get qtmqtt and build it
git clone https://github.com/qt/qtmqtt.git
cd qtmqtt
git checkout 6.7.3
cmake -B build -S . \
        -DCMAKE_INSTALL_PREFIX="$(brew --prefix qt6)" \
	-DCMAKE_INSTALL_LIBDIR="$(brew --prefix qt6)" \
        -DBUILD_TESTING=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_FIND_FRAMEWORK=LAST \
        -Wno-dev
cmake --build build
cmake --install build
cd ..
