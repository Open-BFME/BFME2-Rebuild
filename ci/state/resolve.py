"""Decide which run of the main state workflow to publish, and say why.

Writes `action`, `run_id`, `attempt` and `sha` to GITHUB_OUTPUT. Actions:

  publish         a successful measurement of the commit exists; publish it
  wait            the measurement of the newest main is queued or running
  measure-failed  the newest measurement of that commit failed
  stale           main has had no measurement for longer than STALE_HOURS
  ignore          the triggering run is not a main state run (wrong branch or event)
"""
import datetime
import json
import os
import re
import subprocess
import sys
import time
import urllib.error
import urllib.request

WORKFLOW = ".github/workflows/state.yml"
STALE_HOURS = 3
EVENTS = {"push", "workflow_dispatch", "repository_dispatch"}
OID = re.compile(r"[0-9a-f]{40}\Z")
NUMBER = re.compile(r"[1-9][0-9]{0,19}\Z")


class Api:
    def __init__(self, repository, token):
        self.base = f"https://api.github.com/repos/{repository}/"
        self.token = token

    def get(self, path):
        request = urllib.request.Request(self.base + path, headers={
            "Authorization": "Bearer " + self.token, "Accept": "application/vnd.github+json",
            "X-GitHub-Api-Version": "2022-11-28"})
        with urllib.request.urlopen(request, timeout=30) as response:
            return json.loads(response.read(8 * 1024**2))


def from_workflow_run(env):
    sha, conclusion = env.get("RUN_SHA", ""), env.get("RUN_CONCLUSION", "")
    if (env.get("RUN_BRANCH") != "main" or env.get("RUN_EVENT") not in EVENTS
            or env.get("RUN_PATH") != WORKFLOW or not OID.fullmatch(sha)
            or not NUMBER.fullmatch(env.get("RUN_ID", "")) or not NUMBER.fullmatch(env.get("RUN_ATTEMPT", ""))):
        return {"action": "ignore", "why": "expected a main state run on branch main from push, dispatch or landing, "
                f"found branch {env.get('RUN_BRANCH')!r} event {env.get('RUN_EVENT')!r} path {env.get('RUN_PATH')!r}"}
    result = {"run_id": env["RUN_ID"], "attempt": env["RUN_ATTEMPT"], "sha": sha}
    if conclusion == "success":
        return result | {"action": "publish", "why": f"measurement run {env['RUN_ID']} of {sha} succeeded"}
    if conclusion in {"cancelled", "skipped"}:
        return result | {"action": "wait", "why": f"measurement run of {sha} was {conclusion}; a newer one owns main"}
    return result | {"action": "measure-failed",
                     "why": f"expected measurement run {env['RUN_ID']} of {sha} to succeed, found {conclusion!r}"}


def from_main(api, main, age_seconds, input_run=""):
    if input_run:
        run = api.get(f"actions/runs/{input_run}")
        runs = [run]
    else:
        try:
            runs = api.get(f"actions/workflows/state.yml/runs?branch=main&head_sha={main}&per_page=20")["workflow_runs"]
        except urllib.error.HTTPError as error:
            if error.code != 404:
                raise
            runs = []  # the measuring workflow is not installed yet
    runs = [r for r in runs if r.get("path") == WORKFLOW and r.get("head_branch") == "main"
            and r.get("event") in EVENTS and (input_run or r.get("head_sha") == main)]
    runs.sort(key=lambda r: (r.get("created_at", ""), r.get("id", 0)), reverse=True)
    for run in runs:
        if run.get("status") != "completed":
            return {"action": "wait", "why": f"measurement run {run['id']} of {run['head_sha']} is {run.get('status')}"}
        if run.get("conclusion") == "success":
            return {"action": "publish", "run_id": str(run["id"]), "attempt": str(run["run_attempt"]),
                    "sha": run["head_sha"], "why": f"measurement run {run['id']} of {run['head_sha']} succeeded"}
        if run.get("conclusion") in {"cancelled", "skipped"}:
            continue
        return {"action": "measure-failed", "run_id": str(run["id"]), "attempt": str(run["run_attempt"]),
                "sha": run["head_sha"],
                "why": f"expected measurement run {run['id']} of {run['head_sha']} to succeed, found {run.get('conclusion')!r}"}
    if age_seconds > STALE_HOURS * 3600:
        return {"action": "stale", "sha": main,
                "why": f"expected a measurement of main {main} within {STALE_HOURS} h of its commit, "
                       f"found none after {age_seconds // 3600} h"}
    return {"action": "wait", "sha": main, "why": f"no measurement of main {main} yet; it is {age_seconds // 60} min old"}


def _published_state_age(api, now=None):
    """Age in seconds of the latest commit on the published `state` branch, or None if unreadable."""
    try:
        commit = api.get("commits/state")
        stamp = ((commit.get("commit") or {}).get("committer") or {}).get("date") or (
            ((commit.get("commit") or {}).get("author") or {}).get("date")
        )
        if not isinstance(stamp, str) or not stamp:
            return None
        when = datetime.datetime.fromisoformat(stamp.replace("Z", "+00:00"))
        current = time.time() if now is None else now
        return max(0, int(current - when.timestamp()))
    except (urllib.error.URLError, KeyError, TypeError, ValueError, AttributeError):
        return None


def require_artifact(api, result, age_seconds=0, *, now=None):
    """A successful run whose jobs were all skipped has no measurement; raise stale if published state is old."""
    name = f"bfme-numeric-main-state-{result['attempt']}"
    found = api.get(f"actions/runs/{result['run_id']}/artifacts?name={name}&per_page=5")["artifacts"]
    if any(a.get("name") == name and not a.get("expired") for a in found):
        return result
    published_age = _published_state_age(api, now=now)
    effective_age = published_age if published_age is not None else age_seconds
    if effective_age > STALE_HOURS * 3600:
        return {"action": "stale", "sha": result["sha"],
                "why": f"run {result['run_id']} finished without artifact {name} and main {result['sha']} "
                       f"has had no published measurement for {effective_age // 3600} h (threshold {STALE_HOURS} h)"}
    return {"action": "wait", "sha": result["sha"],
            "why": f"run {result['run_id']} finished without artifact {name} (its jobs were skipped or it expired); "
                   "nothing to publish"}


def main():
    env = os.environ
    age = 0
    if env.get("EVENT") == "workflow_run":
        result = from_workflow_run(env)
    else:
        main_sha = env.get("MAIN_SHA", "")
        if not OID.fullmatch(main_sha):
            print(f"::error::resolve: expected the 40-hex main commit, found {main_sha!r}")
            return 1
        stamp = subprocess.run(["git", "show", "-s", "--format=%ct", main_sha], capture_output=True, text=True)
        if stamp.returncode:
            print(f"::error::resolve: expected {main_sha} in the checkout, found it missing")
            return 1
        api = Api(env["GITHUB_REPOSITORY"], env.pop("READ_TOKEN"))
        run = env.get("INPUT_RUN_ID", "")
        if run and not NUMBER.fullmatch(run):
            print(f"::error::resolve: expected a numeric run id, found {run!r}")
            return 1
        age = int(time.time()) - int(stamp.stdout)
        result = from_main(api, main_sha, age, run)
    if result["action"] == "publish":
        api = api if env.get("EVENT") != "workflow_run" else Api(env["GITHUB_REPOSITORY"], env.pop("READ_TOKEN"))
        result = require_artifact(api, result, age_seconds=age)
    print(f"Decision: {result['action']}: {result['why']}")
    with open(env["GITHUB_OUTPUT"], "a") as stream:
        for key in ("action", "run_id", "attempt", "sha"):
            stream.write(f"{key}={result.get(key, '')}\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
