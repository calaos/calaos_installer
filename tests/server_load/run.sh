#!/bin/bash
# Loads the devices the installer generates in a real calaos_server. Nothing is installed:
# the server runs from a calaos_base checkout built in its dev container.
#
#   CALAOS_DEVICE_XML_DIR=<dir>/xml tst_devicetemplates everyVariantBuildsAValidDevice
#   tests/server_load/run.sh <dir> <calaos_base checkout with src/bin/calaos_server built> <its dev image>
set -e
work=$(realpath "$1"); base=$(realpath "$2"); image=$3
tools=$(dirname "$(realpath "$0")")
docker run --rm --network none -u "$(id -u):$(id -g)" \
    -v "$base:/src:ro" -v "$work:/work" -v "$tools:/tools:ro" "$image" /tools/load-all.sh
python3 -I "$tools/check.py" "$work"
