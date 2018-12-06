#!/bin/bash
set -ev

docker run -t --name winbuilder -d \
    -v $(pwd):/calaos_installer \
    calaos/winbuilder

docker ps

