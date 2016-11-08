#!/bin/bash
set -ev

docker pull centos:6
docker run --name buildvm centos:6 /bin/bash Recipe
docker cp buildvm:/calaos_instaler/calaos_installer.AppImage calaos_installer.AppImage

./script_win32.sh
