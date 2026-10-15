#!/bin/bash
#
# Refreshes data/doc (io_doc.json, io_doc.md, device_templates.json; en and fr)
# from calaos_base. The CI fails when they differ from the server's.
#
#   ./generate_iodoc.sh                downloads the io-doc artifact of the latest
#                                      green calaos_base master (needs gh; set
#                                      CALAOS_BASE_BRANCH to follow another branch)
#   ./generate_iodoc.sh <dir>          <dir>: the output of calaos_base/scripts/create_iodoc
#                                      or an unpacked io-doc artifact (en/, fr/)

set -euo pipefail

root=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ $# -ge 1 ]; then
    src=$1
else
    src=$(mktemp -d)
    trap 'rm -rf "$src"' EXIT
    "$root/scripts/fetch_iodoc_artifact.sh" "$src"
fi

for lang in en fr; do
    mkdir -p "$root/data/doc/$lang"
    for f in io_doc.json io_doc.md device_templates.json; do
        cp "$src/$lang/$f" "$root/data/doc/$lang/$f"
    done
done

"$root/scripts/check_iodoc_sync.py" "$src"
