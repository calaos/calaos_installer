#!/bin/bash
set -ev

echo "install.sh"

brew update > /dev/null
brew install qt6
brew install --HEAD kde-karchive

#get qtmqtt and build it
git clone https://github.com/qt/qtmqtt.git
cd qtmqtt
git checkout 6.6.1
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX="$(brew --prefix qt6)" -DCMAKE_PREFIX_PATH="$(brew --prefix qt6)" -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.physicalcpu)
make install
cd ../..
