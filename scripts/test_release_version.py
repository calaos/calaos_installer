#!/usr/bin/env python3
"""Offline tests of release_version.py: python3 -m unittest scripts/test_release_version.py"""

import contextlib
import io
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import release_version as rv  # noqa: E402

# Shapes found in the repository today: vX.Y, old -beta/-alpha/-rc tags, a few without "v".
LEGACY_TAGS = ["v2.0-beta1", "v2.0-rc1", "v3.0-alpha1", "3.4", "3.10", "v3.10", "3.23", "v3.23",
               "v4.6", "v4.7"]


def tags_with(*extra):
    return LEGACY_TAGS + list(extra)


class NextTag(unittest.TestCase):
    def check(self, tags, expected):
        for kind, tag in expected.items():
            with self.subTest(bump=kind):
                self.assertEqual(rv.next_release(tags, kind)["tag"], tag)

    def test_two_number_tag_counts_as_x_y_0(self):
        self.check(tags_with("v4.8"), {
            "prerelease": "v4.8.1-dev.0",
            "patch": "v4.8.1",
            "minor": "v4.9.0",
            "major": "v5.0.0",
        })

    def test_after_a_final_release(self):
        self.check(tags_with("v4.8", "v4.8.1"), {
            "prerelease": "v4.8.2-dev.0",
            "patch": "v4.8.2",
            "minor": "v4.9.0",
            "major": "v5.0.0",
        })

    def test_after_a_prerelease_same_as_action_bump_version(self):
        self.check(tags_with("v4.8", "v4.8.1-dev.0", "v4.8.1-dev.1", "v4.8.1-dev.2"), {
            "prerelease": "v4.8.1-dev.3",
            "patch": "v4.8.2",
            "minor": "v4.9.0",
            "major": "v5.0.0",
        })

    def test_final_release_wins_over_its_prereleases(self):
        tags = tags_with("v4.8.1-dev.2", "v4.8.1", "v4.8.1-dev.9")
        self.assertEqual(rv.next_release(tags, "prerelease")["previous"], "v4.8.1")

    def test_dev_numbers_compare_as_numbers(self):
        tags = tags_with("v4.8.1-dev.9", "v4.8.1-dev.10")
        self.assertEqual(rv.next_release(tags, "prerelease")["tag"], "v4.8.1-dev.11")

    def test_numbers_compare_as_numbers_not_strings(self):
        self.assertEqual(rv.next_release(["v4.9.0", "v4.10.0"], "patch")["tag"], "v4.10.1")

    def test_tags_without_v_count_and_output_keeps_v(self):
        self.assertEqual(rv.next_release(tags_with("4.9.0"), "patch")["tag"], "v4.9.1")

    def test_v_x_y_and_v_x_y_0_are_the_same_version(self):
        self.assertEqual(rv.next_release(["v4.8.0", "v4.8"], "patch")["tag"], "v4.8.1")
        self.assertEqual(rv.next_release(["v4.8", "v4.8.0"], "patch")["tag"], "v4.8.1")

    def test_unrecognised_tags_are_ignored(self):
        tags = ["v4.8", "v9.0-beta1", "latest", "v5.0.0-rc.1", "v5", "release-5.0.0", "v5.0.0-dev.x"]
        self.assertEqual(rv.next_release(tags, "patch")["tag"], "v4.8.1")

    def test_blank_lines_and_spaces_are_ignored(self):
        self.assertEqual(rv.next_release(["", "  v4.8.1 ", "\n"], "patch")["tag"], "v4.8.2")


class Outputs(unittest.TestCase):
    def test_stable(self):
        self.assertEqual(rv.next_release(["v4.8"], "patch"), {
            "previous": "v4.8.0",
            "tag": "v4.8.1",
            "prerelease": "false",
            "appstream_version": "4.8.1",
        })

    def test_prerelease(self):
        self.assertEqual(rv.next_release(["v4.8.1-dev.2"], "prerelease"), {
            "previous": "v4.8.1-dev.2",
            "tag": "v4.8.1-dev.3",
            "prerelease": "true",
            "appstream_version": "4.8.1~dev.3",
        })


class Errors(unittest.TestCase):
    def test_no_usable_tag(self):
        with self.assertRaises(rv.VersionError):
            rv.next_release(["v2.0-beta1", "latest"], "patch")

    def test_unknown_bump(self):
        with self.assertRaises(rv.VersionError):
            rv.next_release(["v4.8"], "finalize")


class Cli(unittest.TestCase):
    def run_cli(self, tags, *args):
        with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False) as f:
            f.write("\n".join(tags) + "\n")
        self.addCleanup(os.unlink, f.name)
        out, err = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            code = rv.main(["--tags-file", f.name, *args])
        return code, out.getvalue(), err.getvalue()

    def test_prints_github_output_lines(self):
        code, out, _ = self.run_cli(tags_with("v4.8"), "--bump", "prerelease")
        self.assertEqual(code, 0)
        self.assertEqual(out.splitlines(), [
            "previous=v4.8.0",
            "tag=v4.8.1-dev.0",
            "prerelease=true",
            "appstream_version=4.8.1~dev.0",
        ])

    def test_error_exits_non_zero_without_output(self):
        code, out, err = self.run_cli(["latest"], "--bump", "patch")
        self.assertEqual(code, 1)
        self.assertEqual(out, "")
        self.assertIn("no vX.Y", err)

    def test_bump_is_restricted(self):
        with contextlib.redirect_stderr(io.StringIO()), self.assertRaises(SystemExit):
            rv.main(["--bump", "finalize", "--tags-file", os.devnull])


if __name__ == "__main__":
    unittest.main()
