#!/bin/bash
set -ev

docker pull centos:6
docker run -i -t --name buildvm -d centos:6 /bin/bash
docker cp Recipe buildvm:Recipe
docker exec -ti buildvm /Recipe

docker cp buildvm:/calaos_instaler/calaos_installer.AppImage calaos_installer.AppImage

./script_win32.sh
