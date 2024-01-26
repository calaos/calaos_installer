#!/bin/bash
set -ev

echo "install.sh"

brew update > /dev/null
brew install qt6

#get extra-cmake-modules and build it
git clone https://invent.kde.org/frameworks/extra-cmake-modules.git
cd extra-cmake-modules
git checkout 5.114.0
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX="$(brew --prefix qt6)" -DCMAKE_PREFIX_PATH="$(brew --prefix qt6)"
make -j$(sysctl -n hw.physicalcpu)
make install
cd ../..

#get karchive and build it
git clone https://invent.kde.org/frameworks/karchive.git
cd karchive
git checkout 5.114.0
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX="$(brew --prefix qt6)" -DCMAKE_PREFIX_PATH="$(brew --prefix qt6)" \
        -DBUILD_TESTING=0 \
		-DBUILD_WITH_QT6=ON \
        -DQT_MAJOR_VERSION=6
make -j$(sysctl -n hw.physicalcpu)
make install
cd ../..

#get qtmqtt and build it
git clone https://github.com/qt/qtmqtt.git
cd qtmqtt
git checkout 6.6.1
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX="$(brew --prefix qt6)" -DCMAKE_PREFIX_PATH="$(brew --prefix qt6)" -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.physicalcpu)
make install
cd ../..
