"""Small GitHub REST client for the landing job.

Reads always work. Writes (merge, statuses, comments, dispatches) are refused
unless the job runs in live mode, so a dry run cannot change anything.
"""
import json
import re
import sys
import threading
import time
import urllib.error
import urllib.request

LIMIT = 4 * 1024**2
RETRY_STATUSES = {500, 502, 503, 504}
GET_ATTEMPTS = 3


class ApiError(Exception):
    pass


class TransientApiError(ApiError):
    pass


class ReadOnly(Exception):
    pass


class NoRedirect(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, req, fp, code, msg, headers, newurl):
        return None


def printable(text, limit=200):
    text = "".join(ch if ch.isprintable() else " " for ch in str(text))
    return text[:limit]


class Api:
    thread_safe_reads = False
    ratelimit_remaining = None
    ratelimit_reset = None

    def __init__(self, repo, token, live):
        if not token or not re.fullmatch(r"[A-Za-z0-9_.-]{15,2048}", token):
            raise ApiError("Expected the landing job token in BFME_LANDING_TOKEN. Found none or a malformed value.")
        self._token = token
        self.live = live
        self.root = f"https://api.github.com/repos/{repo}/"
        self.graphql = "https://api.github.com/graphql"
        self._local = threading.local()
        self.opener = self._opener()
        self.thread_safe_reads = True
        self.writes = []
        self.last_headers = {}
        self.ratelimit_remaining = None
        self.ratelimit_reset = None

    def _record_headers(self, resp_headers):
        if not resp_headers:
            return {}
        wanted = ("X-Accepted-GitHub-Permissions", "X-GitHub-Request-Id",
                  "X-RateLimit-Remaining", "X-RateLimit-Reset")
        headers = {k.lower(): resp_headers.get(k) for k in wanted if resp_headers.get(k) is not None}
        rem = resp_headers.get("X-RateLimit-Remaining")
        if rem is not None:
            try:
                self.ratelimit_remaining = int(rem)
            except (TypeError, ValueError):
                pass
        rst = resp_headers.get("X-RateLimit-Reset")
        if rst is not None:
            try:
                self.ratelimit_reset = int(rst)
            except (TypeError, ValueError):
                pass
        return headers

    def _opener(self):
        local = getattr(self, "_local", None)
        if local is None:
            return self.opener
        opener = getattr(local, "opener", None)
        if opener is None:
            opener = urllib.request.build_opener(urllib.request.ProxyHandler({}), NoRedirect())
            local.opener = opener
        return opener

    def _send(self, method, path, body):
        data = json.dumps(body, sort_keys=True).encode("utf-8") if body is not None else None
        url = self.graphql if path == "graphql" else self.root + path
        request = urllib.request.Request(url, data=data, method=method, headers={
            "Authorization": "Bearer " + self._token,
            "Accept": "application/vnd.github+json",
            "X-GitHub-Api-Version": "2022-11-28",
            "Content-Type": "application/json"})
        headers = {}
        try:
            with self._opener().open(request, timeout=30) as response:
                code, raw = response.status, response.read(LIMIT + 1)
                headers = self._record_headers(response.headers)
        except urllib.error.HTTPError as exc:
            code, raw = exc.code, exc.read(LIMIT + 1)
            headers = self._record_headers(exc.headers)
            if code == 403 and (self.ratelimit_remaining is not None or self.ratelimit_reset is not None):
                print(
                    f"GitHub API HTTP 403 on {method} {path.split('?')[0]} "
                    f"(X-RateLimit-Remaining={self.ratelimit_remaining}, X-RateLimit-Reset={self.ratelimit_reset})",
                    file=sys.stderr,
                )
        except (urllib.error.URLError, OSError) as exc:
            raise TransientApiError(f"Expected a response from GitHub for {method} {path.split('?')[0]}. "
                                    f"Found a connection failure ({type(exc).__name__}).") from None
        if len(raw) > LIMIT:
            raise ApiError(f"Expected at most {LIMIT} bytes from {path.split('?')[0]}. Found more.")
        try:
            payload = json.loads(raw) if raw.strip() else None
        except ValueError:
            payload = None
        self.last_headers = headers
        return code, payload

    _sleep = staticmethod(time.sleep)

    def call(self, path):
        for attempt in range(GET_ATTEMPTS):
            try:
                code, payload = self._send("GET", path, None)
            except TransientApiError:
                if attempt + 1 >= GET_ATTEMPTS:
                    raise
                self._sleep(1 << attempt)
                continue
            if code == 200:
                return payload
            if code not in RETRY_STATUSES or attempt + 1 >= GET_ATTEMPTS:
                raise ApiError(f"Expected HTTP 200 from GET {path.split('?')[0]}. Found HTTP {code}.")
            self._sleep(1 << attempt)
        raise ApiError(f"Expected HTTP 200 from GET {path.split('?')[0]}. Found no response.")

    def write(self, method, path, body):
        if not self.live:
            raise ReadOnly(f"Expected no writes in dry-run mode. Found an attempted {method} {path}.")
        self.writes.append((method, path, body))
        return self._send(method, path, body)

    def query(self, query, variables=None):
        """Read-only GraphQL query (allowed in both dry-run and live mode) with transient retry."""
        if not isinstance(query, str) or not query.lstrip().startswith("query"):
            raise ApiError("Expected a read-only GraphQL query starting with 'query'.")
        body = {"query": query, "variables": variables or {}}
        for attempt in range(GET_ATTEMPTS):
            try:
                code, payload = self._send("POST", "graphql", body)
            except TransientApiError:
                if attempt + 1 >= GET_ATTEMPTS:
                    raise
                self._sleep(1 << attempt)
                continue
            if code == 200 and isinstance(payload, dict) and not payload.get("errors"):
                return payload.get("data") or {}
            if code not in RETRY_STATUSES or attempt + 1 >= GET_ATTEMPTS:
                if code == 200 and isinstance(payload, dict) and payload.get("errors"):
                    msg = printable((payload["errors"][0] or {}).get("message", ""), 160)
                    raise ApiError(f"Expected GraphQL query to succeed. Found error: {msg}")
                raise ApiError(f"Expected HTTP 200 from GraphQL. Found HTTP {code}.")
            self._sleep(1 << attempt)

    def mutate(self, query, variables):
        """A GraphQL mutation (writes, so live only). Returns the data object or raises with GitHub's message."""
        if not self.live:
            raise ReadOnly("Expected no writes in dry-run mode. Found an attempted GraphQL mutation.")
        self.writes.append(("POST", "graphql", {"query": query, "variables": variables}))
        code, payload = self._send("POST", "graphql", {"query": query, "variables": variables})
        errors = (payload or {}).get("errors") if isinstance(payload, dict) else None
        if code != 200 or errors or not isinstance((payload or {}).get("data"), dict):
            detail = printable((errors or [{}])[0].get("message", "") if errors else "", 160)
            raise ApiError(f"Expected HTTP 200 and data from the GraphQL mutation. Found HTTP {code} {detail}".rstrip() + ".")
        return payload["data"]
