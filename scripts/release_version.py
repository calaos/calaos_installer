#!/usr/bin/env python3
"""Next release tag of calaos_installer, for .github/workflows/publish.yml.

Same increments as calaos/action-bump-version (bump.py), which calaos_base and
calaos_mobile use: prerelease turns X.Y.Z into X.Y.(Z+1)-dev.0 and X.Y.Z-dev.N
into X.Y.Z-dev.(N+1); patch, minor and major drop any -dev suffix and bump
(so patch after 4.8.1-dev.2 gives 4.8.2, as there).

Differences, kept here so the shared action stays untouched:
- the installer tags carry a "v" and the output keeps it (v4.8.1);
- two-number tags (v4.8) count as X.Y.0, where bump.py raises ValueError;
- tags that are not vX.Y, vX.Y.Z or vX.Y.Z-dev.N (v2.0-beta1, ...) are ignored.

Prints key=value lines meant for $GITHUB_OUTPUT.
"""

import argparse
import re
import subprocess
import sys

BUMPS = ("prerelease", "patch", "minor", "major")

_TAG_RE = re.compile(r"^v?(\d+)\.(\d+)(?:\.(\d+)(?:-dev\.(\d+))?)?$")


class VersionError(Exception):
    pass


def parse_tag(tag):
    """(major, minor, patch, dev) with dev None for a final release, or None."""
    m = _TAG_RE.match(tag)
    if not m:
        return None
    major, minor, patch, dev = m.groups()
    return (int(major), int(minor), int(patch or 0), None if dev is None else int(dev))


def _sort_key(version):
    major, minor, patch, dev = version
    # A -dev.N sorts before the final X.Y.Z it leads to.
    return (major, minor, patch, 0 if dev is not None else 1, dev or 0)


def last_version(tags):
    versions = [v for v in (parse_tag(t.strip()) for t in tags) if v is not None]
    if not versions:
        raise VersionError("no vX.Y, vX.Y.Z or vX.Y.Z-dev.N tag found")
    return max(versions, key=_sort_key)


def bump(version, kind):
    major, minor, patch, dev = version
    if kind == "prerelease":
        if dev is None:
            return (major, minor, patch + 1, 0)
        return (major, minor, patch, dev + 1)
    if kind == "patch":
        return (major, minor, patch + 1, None)
    if kind == "minor":
        return (major, minor + 1, 0, None)
    if kind == "major":
        return (major + 1, 0, 0, None)
    raise VersionError(f"unknown bump {kind!r}, expected one of {', '.join(BUMPS)}")


def format_version(version, prefix="v", dev_sep="-"):
    major, minor, patch, dev = version
    text = f"{prefix}{major}.{minor}.{patch}"
    if dev is not None:
        text += f"{dev_sep}dev.{dev}"
    return text


def next_release(tags, kind):
    tags = [t.strip() for t in tags if t.strip()]
    previous = last_version(tags)
    new = bump(previous, kind)
    return {
        "previous": format_version(previous),
        "tag": format_version(new),
        "prerelease": "true" if new[3] is not None else "false",
        # AppStream compares like dpkg: "~" sorts before the final release, "-" after it.
        "appstream_version": format_version(new, prefix="", dev_sep="~"),
    }


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--bump", required=True, choices=BUMPS)
    parser.add_argument("--tags-file", help="one tag per line (default: git tag -l)")
    args = parser.parse_args(argv)

    if args.tags_file:
        with open(args.tags_file, encoding="utf-8") as f:
            tags = f.read().splitlines()
    else:
        tags = subprocess.run(["git", "tag", "-l"], check=True, capture_output=True,
                              text=True).stdout.splitlines()

    try:
        result = next_release(tags, args.bump)
    except VersionError as e:
        print(f"error: {e}", file=sys.stderr)
        return 1
    for key, value in result.items():
        print(f"{key}={value}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
