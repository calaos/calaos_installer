#!/bin/bash
#
# Downloads the "io-doc" artifact of calaos_base (en/ and fr/: io_doc.json,
# io_doc.md, device_templates.json) into <outdir>.
#
# It comes from the latest green run of the CI on master. When CALAOS_BASE_BRANCH
# names an existing branch of calaos_base, that branch is used instead: a change
# that touches both repositories uses the same branch name in each, and the
# installer's CI can then be green before calaos_base is merged. Needs the gh CLI
# and GH_TOKEN (or a gh login).

set -euo pipefail

out=${1:?usage: fetch_iodoc_artifact.sh <outdir>}
repo=calaos/calaos_base
branch=${CALAOS_BASE_BRANCH:-master}

if [ "$branch" != master ] && ! gh api "repos/$repo/branches/$branch" --silent 2>/dev/null; then
    branch=master
fi

run=$(gh run list -R "$repo" -w "Build and Test" -b "$branch" -s success -L 1 \
        --json databaseId,headSha -q '.[0] | "\(.databaseId) \(.headSha)"')
if [ -z "$run" ]; then
    echo "no green run of 'Build and Test' on $repo@$branch" >&2
    exit 1
fi
echo "calaos_base: branch $branch, run ${run% *}, commit ${run#* }"

rm -rf "$out"
if ! gh run download "${run% *}" -R "$repo" -n io-doc -D "$out"; then
    echo "run ${run% *} has no io-doc artifact (expired, or older than DOC-3): re-run the CI of calaos_base $branch" >&2
    exit 1
fi
