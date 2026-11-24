"""Fetch only an authenticated state artifact for this checkout, with bounded input."""

from hashlib import file_digest
from http.client import HTTPException
import os
from pathlib import Path
import re
import shutil
import sys
import tempfile
from time import sleep
import urllib.error
import urllib.parse
import urllib.request
import zipfile

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "managed-check"))
from common import HEX40, HEX64, REPO, REPO_ID
from github_status import GitHub
from subprocess import CalledProcessError
from state_delta import predecessors
from state_evidence import LIMIT

WORKFLOW = ".github/workflows/state.yml"
RUN_ID = re.compile(r"[1-9][0-9]{0,19}\Z")


def verified_run(item, sha):
    return (item.get("path") == WORKFLOW and item.get("head_sha") == sha
            and item.get("head_branch") == "main" and item.get("status") == "completed"
            and item.get("event") in {"push", "workflow_dispatch", "repository_dispatch"}
            and item.get("conclusion") == "success"
            and (item.get("repository") or {}).get("id") == REPO_ID
            and RUN_ID.fullmatch(str(item.get("id", "")))
            and RUN_ID.fullmatch(str(item.get("run_attempt", ""))))


def select_run(api, sha):
    filters = urllib.parse.urlencode({"head_sha": sha, "branch": "main", "status": "success",
                                      "per_page": 10})
    runs = api.call("actions/workflows/state.yml/runs?" + filters)["workflow_runs"]
    return next(((row["id"], row["run_attempt"]) for row in runs if verified_run(row, sha)), None)


def select_ancestor_artifact(api, shas):
    filters = urllib.parse.urlencode({"branch": "main", "status": "success", "per_page": 100})
    runs = api.call("actions/workflows/state.yml/runs?" + filters)["workflow_runs"]
    by_base = {}
    allowed = set(shas)
    for row in runs:
        sha = row.get("head_sha")
        if sha in allowed and verified_run(row, sha):
            by_base.setdefault(sha, (row["id"], row["run_attempt"]))
    nearest = [by_base[sha] for sha in shas if sha in by_base]
    indexes = {}
    for run, attempt in nearest:
        if attempt not in indexes:
            indexes[attempt] = artifact_index(api, attempt)
        by_run, complete = indexes[attempt]
        if run in by_run:
            artifact = checked_artifact(by_run[run], attempt)
        elif complete:
            artifact = None
        else:
            artifact = select_artifact(api, run, attempt)
        if artifact:
            return artifact
    covered_runs = {run for run, _ in nearest}
    fallback_attempts = sorted({1} | set(indexes))
    for attempt_key in fallback_attempts:
        if attempt_key in indexes:
            by_run, _ = indexes[attempt_key]
        else:
            by_run, _ = artifact_index(api, attempt_key)
        unchecked = sorted(by_run.keys() - covered_runs)
        if not unchecked:
            continue
        sha_order = {sha: i for i, sha in enumerate(shas)}
        candidates = []
        for run_id in unchecked:
            try:
                run_data = api.call(f"actions/runs/{run_id}")
            except (LookupError, TypeError, ValueError):
                continue
            sha = run_data.get("head_sha")
            if sha in allowed and verified_run(run_data, sha):
                run_attempt = run_data.get("run_attempt")
                if run_attempt != attempt_key:
                    continue
                candidates.append((sha_order.get(sha, len(shas)), run_id, by_run[run_id]))
        for _, _, rows in sorted(candidates):
            artifact = checked_artifact(rows, attempt_key)
            if artifact:
                return artifact
    return None


def artifact_name(attempt):
    return f"bfme-exact-main-state-{attempt}"


MAX_ARTIFACT_PAGES = 5


def artifact_index(api, attempt):
    """Unexpired state artifacts of one attempt by run id, from bounded pagination, and whether that listing
    holds every such artifact. If the listing cannot be read, nothing is known and each run is asked on its own."""
    name = artifact_name(attempt)
    by_run = {}
    complete = True
    for page in range(1, MAX_ARTIFACT_PAGES + 1):
        filters = urllib.parse.urlencode({"name": name, "per_page": 100, "page": page})
        try:
            artifacts = api.call("actions/artifacts?" + filters)["artifacts"]
        except (LookupError, TypeError, ValueError):
            return {}, False
        for item in artifacts:
            wf_run = item.get("workflow_run") or {}
            run = wf_run.get("id")
            branch = wf_run.get("head_branch")
            if (item.get("name") == name and item.get("expired") is False
                    and RUN_ID.fullmatch(str(run))
                    and branch in (None, "main")):
                by_run.setdefault(int(run), []).append(item)
        if len(artifacts) < 100:
            break
        if page == MAX_ARTIFACT_PAGES:
            complete = False
    return by_run, complete


def select_artifact(api, run, attempt):
    name = artifact_name(attempt)
    artifacts = api.call(f"actions/runs/{run}/artifacts?name={name}&per_page=5")["artifacts"]
    return checked_artifact([item for item in artifacts
                             if item.get("name") == name and item.get("expired") is False], attempt)


def checked_artifact(rows, expected_attempt=None):
    if len(rows) != 1:
        return None
    row = rows[0]
    size = row.get("size_in_bytes")
    digest = (row.get("digest") or "").removeprefix("sha256:")
    if (type(size) is not int or not 0 < size <= LIMIT or not HEX64.fullmatch(digest)
            or not RUN_ID.fullmatch(str(row.get("id", "")))):
        return None
    if expected_attempt is not None:
        name = row.get("name", "")
        if not name.endswith(f"-{expected_attempt}"):
            return None
    wf_run = row.get("workflow_run") or {}
    if wf_run:
        if wf_run.get("head_branch") not in (None, "main"):
            return None
        repo_id = wf_run.get("repository_id")
        if repo_id is not None and repo_id != REPO_ID:
            return None
    return row["id"], size, digest


def retry(operation):
    for pause in (1, 3, 9, None):
        try:
            return operation()
        except urllib.error.HTTPError as error:
            if pause is None or error.code not in {404, 408, 429, 500, 502, 503, 504}:
                raise ValueError("GitHub artifact transfer failed.") from None
        except (HTTPException, OSError):
            if pause is None:
                raise ValueError("GitHub artifact transfer failed.") from None
        except ValueError as error:
            temporary = re.fullmatch(r"The GitHub API returned HTTP (408|429|50[0234])\.", str(error))
            if not temporary or pause is None:
                raise
        sleep(pause)


def download(api, artifact, archive_size, digest, output, *, members=None):
    allowed = members if members is not None else {"base-state.json": LIMIT}
    if not 0 < archive_size <= LIMIT:
        raise ValueError("The state archive exceeds its allowance.")
    url = f"https://api.github.com/repos/{REPO}/actions/artifacts/{artifact}/zip"
    req = urllib.request.Request(url, headers={"Authorization": "Bearer " + api.token})
    try:
        with api.api.open(req, timeout=15):
            raise ValueError("Expected a GitHub artifact storage redirect.")
    except urllib.error.HTTPError as error:
        if error.code != 302:
            raise
        url = error.headers["Location"]
        error.close()
    parts = urllib.parse.urlsplit(url)
    host = parts.hostname or ""
    if (parts.scheme != "https" or parts.username or parts.password or parts.port not in (443, None)
            or not host.endswith((".blob.core.windows.net", ".actions.githubusercontent.com"))):
        raise ValueError("GitHub returned an unexpected artifact storage host.")
    # No GitHub token is sent to artifact storage, and redirects are disabled.
    with tempfile.TemporaryFile() as archive:
        with api.api.open(urllib.request.Request(url), timeout=20) as response:
            left = LIMIT
            while left:
                block = response.read(min(65536, left))
                if not block:
                    break
                archive.write(block)
                left -= len(block)
            if left == LIMIT or response.read(1):
                raise ValueError("Unexpected state package size.")
        archive.seek(0)
        if file_digest(archive, "sha256").hexdigest() != digest:
            raise ValueError("GitHub artifact digest did not match.")
        with zipfile.ZipFile(archive) as bundle:
            files = bundle.infolist()
            if (len(files) != len(allowed) or {file.filename for file in files} != allowed.keys()
                    or any(not 0 < file.file_size <= allowed[file.filename] for file in files)):
                raise ValueError("The state package must contain exactly the expected bounded JSON files.")
            for file in files:
                destination = output / file.filename if members is not None else output
                with bundle.open(file) as source, destination.open("xb") as target:
                    shutil.copyfileobj(source, target, 65536)



def main(output, *, parent=False, merge_base=False):
    sha = os.environ["BFME_MQ_MAIN_SHA" if merge_base else "GITHUB_SHA"]
    ref = os.environ["GITHUB_REF"]
    protected = ref == "refs/heads/main"
    if merge_base:
        protected = (not parent and os.environ.get("GITHUB_EVENT_NAME") == "merge_group"
                     and ref.startswith("refs/heads/gh-readonly-queue/main/"))
    if (os.environ["GITHUB_REPOSITORY"] != REPO or os.environ["GITHUB_REPOSITORY_ID"] != str(REPO_ID)
            or not protected or not HEX40.fullmatch(sha)):
        raise ValueError("Use the exact protected repository and main.")
    if parent:
        if os.environ.get("GITHUB_EVENT_NAME") not in {"push", "workflow_dispatch", "repository_dispatch"}:
            raise ValueError("Only the trusted state workflow may carry the parent.")
        try:
            shas = predecessors(Path.cwd(), sha)
        except (OSError, CalledProcessError):
            print("The protected ancestry is unavailable.")
            return
        if not shas:
            print("Main has no predecessor in the state checkout.")
            return
    temporary = output.with_name(output.name + ".partial")
    try:
        api = GitHub()
        if parent:
            ref = select_ancestor_artifact(api, shas)
        else:
            run = select_run(api, sha)
            ref = select_artifact(api, *run) if run else None
        if ref:
            download(api, *ref, temporary)
            temporary.replace(output)
            print("Downloaded state from a protected ancestor." if parent
                  else "Downloaded state produced for this exact protected revision.")
            return
    except (OSError, LookupError, TypeError, ValueError, zipfile.BadZipFile):
        # Storage URLs are signed, so never include exceptions or URLs in logs.
        print("Protected main state could not be fetched.")
    finally:
        temporary.unlink(missing_ok=True)
    print("No authenticated protected state available for this request.")


if __name__ == "__main__":
    os.umask(0o077)
    main(Path(sys.argv[1]), parent=sys.argv[2:] == ["--parent"], merge_base=sys.argv[2:] == ["--merge-base"])

