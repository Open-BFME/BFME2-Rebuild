"""Post the BFME2 progress card metrics to Discord via an incoming webhook.

    python3 -B ci/progress/discord.py --card progress-data/card.json \
        --previous last-post/discord-last.json --save-state last-post/discord-last.json

Reads the webhook URL from the `DISCORD_PROGRESS_WEBHOOK` environment variable
(not needed with `--dry-run`) and never prints the webhook URL on error.
"""
import argparse
from datetime import datetime, timezone
import json
import os
from pathlib import Path
import re
import sys
import urllib.error
import urllib.parse
import urllib.request

REPO_URL = "https://github.com/Open-BFME/BFME2-Rebuild"
EMBED_TITLE = "BFME 2 Rebuild · Progress"
EMBED_COLOR = 0x2EA043
USER_AGENT = "OpenBFME-Progress/1.0"
TIMEOUT_SECONDS = 30
BAR_WIDTH = 10
WEBHOOK_PREFIXES = (
    "https://discord.com/api/webhooks/",
    "https://discordapp.com/api/webhooks/",
)
ISO_UTC_RE = re.compile(r"^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z$")


def valid_pair(pair):
    return (
        isinstance(pair, (list, tuple))
        and len(pair) == 2
        and all(type(n) is int for n in pair)
        and pair[0] >= 0
        and pair[1] > 0
    )


def pct(pair):
    if not valid_pair(pair):
        return None
    return 100.0 * pair[0] / pair[1]


def emoji_bar(value, filled_emoji, empty_emoji="⬛", width=BAR_WIDTH):
    """Render a fixed-width emoji progress bar."""
    if value is None:
        return empty_emoji * width
    clamped = max(0.0, min(100.0, float(value)))
    filled = max(0, min(width, int(clamped / 100.0 * width + 0.5)))
    return filled_emoji * filled + empty_emoji * (width - filled)


def format_delta(current_pct, previous_pct):
    """Format percentage-point change vs the previous post, or '' when unchanged/unavailable."""
    if current_pct is None or previous_pct is None:
        return ""
    diff = round(current_pct - previous_pct, 2)
    if diff > 0:
        return f"▲ +{diff:.2f}"
    if diff < 0:
        return f"▼ {diff:.2f}"
    return ""


def validate_webhook_url(url):
    """Validate a Discord incoming webhook URL without ever exposing it in errors."""
    if not isinstance(url, str):
        raise ValueError("DISCORD_PROGRESS_WEBHOOK is not set")
    cleaned = url.strip()
    if not cleaned:
        raise ValueError("DISCORD_PROGRESS_WEBHOOK is empty")
    if any(ch.isspace() or ord(ch) < 32 for ch in cleaned):
        raise ValueError("DISCORD_PROGRESS_WEBHOOK contains invalid characters")
    if not cleaned.startswith(WEBHOOK_PREFIXES):
        raise ValueError(
            "DISCORD_PROGRESS_WEBHOOK must start with "
            "https://discord.com/api/webhooks/ or https://discordapp.com/api/webhooks/"
        )
    parts = urllib.parse.urlsplit(cleaned)
    if parts.scheme != "https" or parts.netloc not in ("discord.com", "discordapp.com"):
        raise ValueError("DISCORD_PROGRESS_WEBHOOK has an invalid host or scheme")
    prefix = "/api/webhooks/"
    if not parts.path.startswith(prefix):
        raise ValueError("DISCORD_PROGRESS_WEBHOOK has an invalid webhook path")
    segments = [seg for seg in parts.path[len(prefix):].split("/") if seg]
    if len(segments) < 2:
        raise ValueError("DISCORD_PROGRESS_WEBHOOK must include webhook id and token")
    return cleaned


def webhook_post_url(url):
    """Append wait=true to a validated webhook URL so Discord returns the created message."""
    parts = urllib.parse.urlsplit(url)
    query = [(k, v) for k, v in urllib.parse.parse_qsl(parts.query, keep_blank_values=True) if k != "wait"]
    query.append(("wait", "true"))
    return urllib.parse.urlunsplit((parts.scheme, parts.netloc, parts.path, urllib.parse.urlencode(query), ""))


def load_previous_card(path, current_main=None):
    """Load the previous posted card from a saved state file, a card.json, or a history.json."""
    if not path:
        return None
    target = Path(path)
    if not target.is_file():
        return None
    try:
        data = json.loads(target.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return None
    if isinstance(data, dict):
        if isinstance(data.get("card"), dict):
            return data["card"]
        if any(k in data for k in ("matched", "linking", "proven", "finished")):
            return data
        return None
    if isinstance(data, list):
        rows = [row for row in data if isinstance(row, dict)]
        if not rows:
            return None
        if current_main:
            earlier = [row for row in rows if row.get("main") != current_main]
            if earlier:
                return earlier[-1]
        return rows[-1]
    return None


def _nightly_line(card):
    nightly = card.get("nightly")
    if not isinstance(nightly, dict):
        return None
    compiled = nightly.get("compiled")
    files = nightly.get("files")
    if type(compiled) is not int or type(files) is not int or compiled < 0 or files <= 0:
        return None
    fully = nightly.get("fully_matching")
    if type(fully) is int and fully >= 0:
        return f"> Daily build: {compiled:,} of {files:,} source files compile, {fully:,} match completely"
    return f"> Daily build: {compiled:,} of {files:,} source files compile"


def build_description(card, previous=None):
    """Format the Discord embed description markdown for one card snapshot."""
    prev = previous if isinstance(previous, dict) else {}

    matched = card.get("matched")
    matched_pct = pct(matched)
    matched_delta = format_delta(matched_pct, pct(prev.get("matched")))

    linking = card.get("linking")
    linking_pct = pct(linking)
    linking_delta = format_delta(linking_pct, pct(prev.get("linking")))
    linking_unit = card.get("linking_unit") if card.get("linking_unit") in ("names", "functions") else "names"
    linking_date = str(card.get("linking_date") or "")[:10]

    proven = card.get("proven")
    proven_pct = pct(proven)
    proven_delta = format_delta(proven_pct, pct(prev.get("proven")))

    finished = card.get("finished")
    finished_pct = pct(finished)
    finished_delta = format_delta(finished_pct, pct(prev.get("finished")))

    lines = [
        "*Clean C++ rebuild of game.dat (1.06) compiled with Visual C++ 7.1*",
        "",
        "**Does it work?**",
    ]

    if matched_pct is not None:
        delta_suffix = f" `{matched_delta}`" if matched_delta else ""
        lines.append(f"**Byte match:** `{matched_pct:.2f}%`{delta_suffix}")
        lines.append(emoji_bar(matched_pct, "🟩"))
        lines.append(f"{matched[0]:,} / {matched[1]:,} bytes compiled byte-for-byte")
    else:
        lines.append("**Byte match:** —")
        lines.append(emoji_bar(None, "🟩"))
        lines.append("not measured yet")

    lines.append("")

    if linking_pct is not None:
        delta_suffix = f" `{linking_delta}`" if linking_delta else ""
        date_suffix = f"; measured {linking_date}" if linking_date else ""
        lines.append(f"**Linking:** `{linking_pct:.2f}%`{delta_suffix}")
        lines.append(emoji_bar(linking_pct, "🟨"))
        lines.append(f"{linking[0]:,} / {linking[1]:,} {linking_unit} provided{date_suffix}")
    else:
        lines.append("**Linking:** —")
        lines.append(emoji_bar(None, "🟨"))
        lines.append("not measured yet")

    nightly_quote = _nightly_line(card)
    if nightly_quote:
        lines.append("")
        lines.append(nightly_quote)

    lines.append("")
    lines.append("**Clean progress**")

    if proven_pct is not None:
        delta_suffix = f" `{proven_delta}`" if proven_delta else ""
        lines.append(
            f"{emoji_bar(proven_pct, '🟦')} **{proven_pct:.2f}%** Code proven "
            f"(matched + all references verified){delta_suffix}"
        )
    else:
        lines.append(f"{emoji_bar(None, '🟦')} **—** Code proven (matched + all references verified)")

    if finished_pct is not None:
        delta_suffix = f" `{finished_delta}`" if finished_delta else ""
        lines.append(
            f"{emoji_bar(finished_pct, '🟪')} **{finished_pct:.2f}%** Finished functions "
            f"({finished[0]:,} / {finished[1]:,}, named file + final header){delta_suffix}"
        )
    else:
        lines.append(f"{emoji_bar(None, '🟪')} **—** Finished functions (named file + final header)")

    drafts = card.get("drafts_in_use")
    if type(drafts) is int and drafts >= 0:
        lines.append(f"Draft headers still in use: {drafts:,}")

    lines.append("")
    main_sha = str(card.get("main") or "")
    if main_sha:
        lines.append(f"Main `{main_sha[:12]}` · [Live progress card & repo]({REPO_URL})")
    else:
        lines.append(f"[Live progress card & repo]({REPO_URL})")

    return "\n".join(lines)


def build_payload(card, previous=None):
    """Build the complete Discord webhook JSON payload."""
    embed = {
        "title": EMBED_TITLE,
        "url": REPO_URL,
        "color": EMBED_COLOR,
        "description": build_description(card, previous),
    }
    measured = card.get("measured")
    if isinstance(measured, str) and measured.strip():
        embed["footer"] = {"text": f"Measured {measured.strip()}"}
    stamp = card.get("date")
    if isinstance(stamp, str) and ISO_UTC_RE.match(stamp):
        embed["timestamp"] = stamp
    return {
        "allowed_mentions": {"parse": []},
        "embeds": [embed],
    }


def post_webhook(webhook_url, payload, timeout=TIMEOUT_SECONDS, opener=None):
    """POST the payload to Discord with ?wait=true, never leaking the webhook URL on error."""
    cleaned = validate_webhook_url(webhook_url)
    target_url = webhook_post_url(cleaned)
    body = json.dumps(payload, ensure_ascii=False).encode("utf-8")
    req = urllib.request.Request(
        target_url,
        data=body,
        method="POST",
        headers={
            "Content-Type": "application/json",
            "User-Agent": USER_AGENT,
        },
    )
    open_fn = opener or urllib.request.urlopen
    try:
        with open_fn(req, timeout=timeout) as response:
            raw = response.read()
    except urllib.error.HTTPError as err:
        raise RuntimeError(f"Discord webhook returned HTTP {err.code}") from None
    except urllib.error.URLError as err:
        reason = str(err.reason)
        if cleaned in reason or "webhooks/" in reason:
            reason = "network error"
        raise RuntimeError(f"Discord webhook request failed: {reason}") from None
    except OSError as err:
        raise RuntimeError(f"Discord webhook request failed: {type(err).__name__}") from None

    if not raw:
        return {}
    try:
        parsed = json.loads(raw.decode("utf-8"))
    except (UnicodeDecodeError, ValueError):
        return {}
    return parsed if isinstance(parsed, dict) else {}


def save_state(path, card, message_id=None, posted_at=None, run_id=None):
    """Persist the posted card snapshot and delivery metadata for the next run's deltas."""
    target = Path(path)
    target.parent.mkdir(parents=True, exist_ok=True)
    record = {
        "card": card,
        "message_id": message_id,
        "posted_at": posted_at or datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "run_id": run_id,
    }
    data = json.dumps(record, indent=2, sort_keys=True, ensure_ascii=False) + "\n"
    target.write_bytes(data.encode("utf-8"))
    return record


def main(argv=None):
    parser = argparse.ArgumentParser(description="Post BFME2 progress metrics to Discord.")
    parser.add_argument("--card", required=True, help="Path to card.json from the progress branch")
    parser.add_argument("--previous", default="", help="Optional path to previous discord-last.json or history.json")
    parser.add_argument("--save-state", default="", help="Optional path to save the posted state JSON")
    parser.add_argument("--dry-run", action="store_true", help="Print the webhook JSON payload without posting")
    args = parser.parse_args(argv)

    try:
        card_data = json.loads(Path(args.card).read_text(encoding="utf-8"))
    except (OSError, ValueError) as err:
        print(f"::error::discord progress: failed to read card JSON: {err}")
        return 1
    if not isinstance(card_data, dict):
        print("::error::discord progress: expected card JSON to be an object")
        return 1

    previous = load_previous_card(args.previous, current_main=card_data.get("main"))
    payload = build_payload(card_data, previous)

    if args.dry_run:
        print(json.dumps(payload, indent=2, ensure_ascii=False))
        return 0

    webhook = os.environ.get("DISCORD_PROGRESS_WEBHOOK", "")
    try:
        response = post_webhook(webhook, payload)
    except (ValueError, RuntimeError) as err:
        print(f"::error::discord progress: {err}")
        return 1

    raw_id = response.get("id")
    message_id = str(raw_id) if raw_id is not None else None
    run_id = os.environ.get("GITHUB_RUN_ID") or None

    if args.save_state:
        try:
            save_state(args.save_state, card_data, message_id=message_id, run_id=run_id)
        except OSError as err:
            print(f"::error::discord progress: posted to Discord but failed to save state: {err}")
            return 1

    main_short = str(card_data.get("main") or "unknown")[:12]
    print(f"Posted BFME2 progress to Discord (main {main_short}, message {message_id or 'unknown'}).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
