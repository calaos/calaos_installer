#!/bin/bash
# Run by run.sh inside the calaos_base container: starts calaos_server for a few seconds on
# each /work/xml/*.xml (or on the files given), keeps its log and the io.xml it saves on exit.
mkdir -p /work/out
for f in ${1:-/work/xml/*.xml}; do
  n=$(basename "$f" .xml)
  d=$(mktemp -d /tmp/cfg.XXXX); c=$(mktemp -d /tmp/cache.XXXX)
  cp "$f" "$d/io.xml"; cp /tools/rules.xml "$d/rules.xml"
  timeout -s INT 4 /src/src/bin/calaos_server/calaos_server --config "$d" --cache "$c" > "/work/out/$n.log" 2>&1
  cp "$d/io.xml" "/work/out/$n.saved.xml"
done
