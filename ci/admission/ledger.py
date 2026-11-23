"""The admission ledger: durable, single-use reservations, each worth one compiler measurement.

One file, ledger.json, on the branch admission-ledger, written only by this repository's workflows through the
Contents API with compare-and-set on its blob sha. One snapshot holds both the daily counters and every
reservation key, so a limit and a key are always decided together:

  {"version": 2,
   "days": {"YYYY-MM-DD": {"total": n, "authors": {"<id>": n}, "wide_total": n, "wide_authors": {"<id>": n}}},
   "reservations": [{"day": "YYYY-MM-DD", "number": n, "author": id, "head": sha, "base": sha, "tree": sha,
                     "kind": "narrow|wide", "admitted_by": {"run": "id", "attempt": "n"}, "claimed_by": null}]}

reserve()        counts one measurement of an exact (pull request, head, base) against the day's limits of its kind;
                 the same tuple is never reserved twice, whatever day it was first reserved.
find()           returns that reservation.
claim()          marks it used by exactly one run attempt, once; another run or attempt gets "claimed-by-other",
                 another composed tree gets "tree-mismatch", and both must allocate nothing.
refresh_issuer() hands an unclaimed reservation to a newer admission run, when the first one stopped before its
                 dispatch, so the compiler check's issuer guard can accept the retry.
reissue_unmeasured() frees a used reservation for one more run when the run that used it finished without
                 measuring anything (a machine fault); at most MAX_REISSUES times, each counted against the day.

"tree" is GitHub's merge of exactly that head onto exactly that base (the pull request's merge preview commit).
Writes prune counters older than yesterday and reservations older than yesterday unless they are for the main
being reserved now: main never moves backwards, so an older base can never be measured again.
"""
import base64
import datetime
import json
import random
import time

LEDGER_BRANCH = "admission-ledger"
LEDGER_FILE = "ledger.json"
ATTEMPTS = 12
MAX_REISSUES = 4
_sleep = time.sleep


class LedgerError(Exception):
    pass


def days(clock=None):
    now = clock() if clock else datetime.datetime.now(datetime.timezone.utc)
    return [now.strftime("%Y-%m-%d"), (now - datetime.timedelta(days=1)).strftime("%Y-%m-%d")]


def _get(api, path):
    code, payload = api._send("GET", path, None)
    if code not in (200, 404):
        raise LedgerError(f"Expected HTTP 200 or 404 from the ledger. Found HTTP {code}.")
    return code, payload


def ensure(api):
    code, _ = _get(api, f"git/ref/heads/{LEDGER_BRANCH}")
    if code == 200:
        return
    note = "Admission reservations written by this repository's workflows (ledger.json).\n"
    code, tree = api.write("POST", "git/trees", {"tree": [{"path": "README.md", "mode": "100644", "type": "blob",
                                                            "content": note}]})
    if code != 201:
        raise LedgerError(f"Expected HTTP 201 creating the ledger tree. Found HTTP {code}.")
    code, commit = api.write("POST", "git/commits", {"message": "Start the admission ledger",
                                                      "tree": tree["sha"], "parents": []})
    if code != 201:
        raise LedgerError(f"Expected HTTP 201 creating the ledger commit. Found HTTP {code}.")
    code, _ = api.write("POST", "git/refs", {"ref": f"refs/heads/{LEDGER_BRANCH}", "sha": commit["sha"]})
    if code not in (201, 422):            # 422: another run created it first
        raise LedgerError(f"Expected HTTP 201 creating the ledger branch. Found HTTP {code}.")


def _read(api):
    code, payload = _get(api, f"contents/{LEDGER_FILE}?ref={LEDGER_BRANCH}")
    if code == 404:
        return {"version": 2, "days": {}, "reservations": []}, None
    try:
        data = json.loads(base64.b64decode(payload["content"]))
        if data.get("version") != 2 or not isinstance(data.get("days"), dict) \
                or not isinstance(data.get("reservations"), list):
            raise ValueError
        return data, payload["sha"]
    except (KeyError, TypeError, ValueError):
        raise LedgerError("Expected a readable ledger.json. Found an unreadable one.") from None


def _write(api, data, sha, message):
    body = {"message": message, "branch": LEDGER_BRANCH,
            "content": base64.b64encode((json.dumps(data, sort_keys=True, indent=1) + "\n").encode()).decode()}
    if sha:
        body["sha"] = sha
    code, _ = api.write("PUT", f"contents/{LEDGER_FILE}", body)
    if code in (200, 201):
        return True
    if code in (409, 422):
        return False                       # someone wrote first: read again
    raise LedgerError(f"Expected HTTP 200 or 201 for the ledger update. Found HTTP {code}.")


def _match(data, number, head, base):
    for item in data["reservations"]:
        if item.get("number") == number and item.get("head") == head and item.get("base") == base:
            return item
    return None


def _prune(data, yesterday, base):
    data["days"] = {day: counts for day, counts in data["days"].items() if day >= yesterday}
    data["reservations"] = [item for item in data["reservations"]
                            if item.get("day", "") >= yesterday or item.get("base") == base]


def reserve(api, limits, author, number, head, base, run, attempt, clock=None, tree=None, kind="narrow"):
    """'reserved', 'already' (this exact tuple is in the ledger, any day) or 'over-budget'."""
    if kind not in ("narrow", "wide", "trusted-narrow", "trusted-wide"):
        raise LedgerError(f"Expected kind narrow, wide, trusted-narrow or trusted-wide. Found {kind!r}.")
    total_key, authors_key = ("total", "authors") if kind == "narrow" else ("wide_total", "wide_authors")
    ensure(api)
    today, yesterday = days(clock)
    for attempt_idx in range(ATTEMPTS):
        data, sha = _read(api)
        item = _match(data, number, head, base)
        if item:
            exact = item.get("tree") == tree and item.get("kind") == kind and item.get("author") == author
            return "already" if exact else "binding-mismatch"
        already_paid_today = any(
            r.get("day") == today
            and r.get("number") == number and r.get("head") == head and r.get("kind") == kind
            and r.get("author") == author
            for r in data["reservations"]
        )
        if kind not in ("trusted-narrow", "trusted-wide") and not already_paid_today:
            counts = data["days"].setdefault(today, {"total": 0, "authors": {}, "wide_total": 0, "wide_authors": {}})
            key = str(author)
            if counts[total_key] >= limits["daily_total"] or counts[authors_key].get(key, 0) >= limits["daily_per_author"]:
                return "over-budget"
            counts[total_key] += 1
            counts[authors_key][key] = counts[authors_key].get(key, 0) + 1
        data["reservations"].append({"day": today, "number": number, "author": author,
                                     "head": head, "base": base, "tree": tree, "kind": kind,
                                     "admitted_by": {"run": str(run), "attempt": str(attempt)}, "claimed_by": None})
        _prune(data, yesterday, base)
        if _write(api, data, sha, f"Reserve #{number} {head[:12]} on {base[:12]}"):
            return "reserved"
        if attempt_idx + 1 < ATTEMPTS and getattr(api, "live", False) and hasattr(api, "opener"):
            _sleep(0.15 * (attempt_idx + 1) + random.uniform(0, 0.15))
    raise LedgerError(f"Expected a ledger update within {ATTEMPTS} attempts. Found repeated concurrent changes.")


def find(api, number, head, base, clock=None):
    data, _ = _read(api)
    return _match(data, number, head, base)


def claim(api, number, head, base, run, attempt, clock=None, tree=None, *, kind=None, admitted_by=None):
    """Claim only the exact binding the caller inspected and leave a mismatched reservation intact.

    A missing binding returns 'missing'. Mismatches return 'tree-mismatch', 'kind-mismatch' or
    'issuer-mismatch'. A reservation use returns 'claimed', 'already-claimed-by-this-run' or
    'claimed-by-other'.
    """
    mine = {"run": str(run), "attempt": str(attempt)}
    for attempt_idx in range(ATTEMPTS):
        data, sha = _read(api)
        item = _match(data, number, head, base)
        if not item:
            return "missing"
        if tree is not None and item.get("tree") != tree:
            return "tree-mismatch"
        if kind is not None and item.get("kind") != kind:
            return "kind-mismatch"
        if admitted_by is not None and item.get("admitted_by") != admitted_by:
            return "issuer-mismatch"
        if item.get("claimed_by") == mine:
            return "already-claimed-by-this-run"
        if item.get("claimed_by"):
            return "claimed-by-other"
        item["claimed_by"] = mine
        if _write(api, data, sha, f"Claim #{number} {head[:12]} by run {run}/{attempt}"):
            return "claimed"
        if attempt_idx + 1 < ATTEMPTS and getattr(api, "live", False) and hasattr(api, "opener"):
            _sleep(0.15 * (attempt_idx + 1) + random.uniform(0, 0.15))
    raise LedgerError(f"Expected a ledger claim within {ATTEMPTS} attempts. Found repeated concurrent changes.")


def refresh_issuer(api, number, head, base, run, attempt):
    """Give an unclaimed reservation to this admission run. 'refreshed', 'claimed' (leave it) or 'missing'."""
    mine = {"run": str(run), "attempt": str(attempt)}
    for attempt_idx in range(ATTEMPTS):
        data, sha = _read(api)
        item = _match(data, number, head, base)
        if not item:
            return "missing"
        if item.get("claimed_by"):
            return "claimed"
        if item.get("admitted_by") == mine:
            return "refreshed"
        item["admitted_by"] = mine
        if _write(api, data, sha, f"Re-issue #{number} {head[:12]} to run {run}/{attempt}"):
            return "refreshed"
        if attempt_idx + 1 < ATTEMPTS and getattr(api, "live", False) and hasattr(api, "opener"):
            _sleep(0.15 * (attempt_idx + 1) + random.uniform(0, 0.15))
    raise LedgerError(f"Expected a ledger update within {ATTEMPTS} attempts. Found repeated concurrent changes.")


def reissue_unmeasured(api, limits, number, head, base, claimed_by, run, attempt, clock=None):
    """Free a used reservation for one more run, when the run that used it ended without measuring anything.

    The caller has established that the run named in `claimed_by` is finished and left no compiler verdict: a
    machine fault, not a result. Only that exact claim is released (compare-and-set on `claimed_by`), at most
    MAX_REISSUES times for one reservation, and each release counts against the day's limits like a new
    measurement. Returns 'reissued', 'exhausted', 'over-budget', 'changed' (the claim is no longer the one the
    caller inspected) or 'missing'.
    """
    mine = {"run": str(run), "attempt": str(attempt)}
    today, _ = days(clock)
    for attempt_idx in range(ATTEMPTS):
        data, sha = _read(api)
        item = _match(data, number, head, base)
        if not item:
            return "missing"
        if not isinstance(claimed_by, dict) or not claimed_by or item.get("claimed_by") != claimed_by:
            return "changed"
        used = item.get("reissues", 0)
        if type(used) is not int or not 0 <= used < MAX_REISSUES:
            return "exhausted"
        kind = item.get("kind")
        if kind not in ("narrow", "wide", "trusted-narrow", "trusted-wide"):
            raise LedgerError(f"Expected kind narrow, wide, trusted-narrow or trusted-wide. Found {kind!r}.")
        if kind not in ("trusted-narrow", "trusted-wide") and not limits:
            return "over-budget"
        # Infrastructure faults never double-charge the daily quota already paid on initial reserve().
        item["reissues"] = used + 1
        item["claimed_by"] = None
        item["admitted_by"] = mine
        if _write(api, data, sha, f"Re-issue #{number} {head[:12]} after an unmeasured run ({used + 1} of {MAX_REISSUES})"):
            return "reissued"
        if attempt_idx + 1 < ATTEMPTS and getattr(api, "live", False) and hasattr(api, "opener"):
            _sleep(0.15 * (attempt_idx + 1) + random.uniform(0, 0.15))
    raise LedgerError(f"Expected a ledger update within {ATTEMPTS} attempts. Found repeated concurrent changes.")
