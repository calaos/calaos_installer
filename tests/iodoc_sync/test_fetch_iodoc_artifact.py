#!/usr/bin/env python3
"""scripts/fetch_iodoc_artifact.sh against a fake gh: which run it trusts, and how."""
import json
import os
import subprocess
import tempfile
import unittest

SCRIPT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "scripts", "fetch_iodoc_artifact.sh")

# gh as the script uses it: `gh api` (does the branch exist), `gh run list` (canned
# runs, the arguments are logged), `gh run download` (writes a marker in -D).
FAKE_GH = r"""#!/bin/bash
echo "$*" >> "$FAKE_GH_LOG"
case "$1 $2" in
"api repos/calaos/calaos_base/branches/"*)
    b=${2#repos/calaos/calaos_base/branches/}
    grep -qx "$b" "$FAKE_GH_BRANCHES"
    exit $? ;;
"run list") cat "$FAKE_GH_RUNS" ;;
"run download")
    [ -z "${FAKE_GH_NO_ARTIFACT:-}" ] || exit 1
    while [ $# -gt 0 ]; do [ "$1" = -D ] && mkdir -p "$2" && echo "$3" > "$2/marker"; shift; done ;;
*) exit 2 ;;
esac
"""


def run(id_, event, sha="a" * 40):
    return {"databaseId": id_, "headSha": sha, "event": event}


class FetchIodocArtifact(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        t = self._tmp.name
        os.makedirs(os.path.join(t, "bin"))
        gh = os.path.join(t, "bin", "gh")
        with open(gh, "w") as f:
            f.write(FAKE_GH)
        os.chmod(gh, 0o755)
        self.log = os.path.join(t, "gh.log")
        self.runs = os.path.join(t, "runs.json")
        self.branches = os.path.join(t, "branches")
        self.out = os.path.join(t, "out")
        open(self.log, "w").close()
        with open(self.branches, "w") as f:
            f.write("master\nfeat/devices-doc-3\n")

    def tearDown(self):
        self._tmp.cleanup()

    def fetch(self, runs, branch=None, no_artifact=False):
        with open(self.runs, "w") as f:
            json.dump(runs, f)
        env = dict(os.environ, PATH=os.path.join(self._tmp.name, "bin") + os.pathsep + os.environ["PATH"],
                   FAKE_GH_LOG=self.log, FAKE_GH_RUNS=self.runs, FAKE_GH_BRANCHES=self.branches)
        env.pop("CALAOS_BASE_BRANCH", None)
        if branch:
            env["CALAOS_BASE_BRANCH"] = branch
        if no_artifact:
            env["FAKE_GH_NO_ARTIFACT"] = "1"
        r = subprocess.run(["bash", SCRIPT, self.out], env=env, capture_output=True, text=True)
        return r.returncode, r.stdout + r.stderr

    def calls(self):
        with open(self.log) as f:
            return f.read()

    def test_the_first_push_run_is_downloaded(self):
        rc, out = self.fetch([run(11, "push"), run(10, "push")])
        self.assertEqual(rc, 0, out)
        self.assertIn("run download 11 ", self.calls())
        self.assertNotIn("run download 10 ", self.calls())

    def test_a_pull_request_run_is_never_trusted(self):
        # The most recent green run is a fork's pull request named like the branch.
        rc, out = self.fetch([run(12, "pull_request"), run(11, "push")])
        self.assertEqual(rc, 0, out)
        self.assertIn("run download 11 ", self.calls())
        self.assertNotIn("run download 12 ", self.calls())

    def test_only_pull_request_runs_is_a_failure_without_download(self):
        rc, out = self.fetch([run(12, "pull_request"), run(13, "pull_request")])
        self.assertEqual(rc, 1, out)
        self.assertNotIn("run download", self.calls())

    def test_the_monthly_and_manual_runs_count(self):
        rc, out = self.fetch([run(12, "pull_request"), run(9, "schedule"), run(8, "push")])
        self.assertEqual(rc, 0, out)
        self.assertIn("run download 9 ", self.calls())
        rc, out = self.fetch([run(7, "workflow_dispatch")])
        self.assertEqual(rc, 0, out)
        self.assertIn("run download 7 ", self.calls())

    def test_no_green_run_fails(self):
        rc, out = self.fetch([])
        self.assertEqual(rc, 1, out)
        self.assertNotIn("run download", self.calls())

    def test_master_by_default_and_only_green_runs(self):
        self.fetch([run(1, "push")])
        self.assertIn("-b master", self.calls())
        self.assertIn("-s success", self.calls())
        self.assertIn("-w Build and Test", self.calls())

    def test_a_branch_of_the_same_name_is_used_when_it_exists(self):
        rc, out = self.fetch([run(1, "push")], branch="feat/devices-doc-3")
        self.assertEqual(rc, 0, out)
        self.assertIn("-b feat/devices-doc-3", self.calls())

    def test_an_unknown_branch_falls_back_to_master(self):
        rc, out = self.fetch([run(1, "push")], branch="someone/else")
        self.assertEqual(rc, 0, out)
        self.assertIn("-b master", self.calls())
        self.assertNotIn("-b someone/else", self.calls())

    def test_a_run_without_the_artifact_fails_with_an_explanation(self):
        rc, out = self.fetch([run(1, "push")], no_artifact=True)
        self.assertEqual(rc, 1, out)
        self.assertIn("no io-doc artifact", out)


if __name__ == "__main__":
    unittest.main()
