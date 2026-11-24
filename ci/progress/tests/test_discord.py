"""Offline unit tests for Discord progress posting (ci/progress/discord.py).

Run from the repository root:  python3 -B -m unittest discover -s ci/progress/tests -v
"""
from contextlib import redirect_stdout
import io
import json
import os
from pathlib import Path
import re
import sys
import tempfile
import unittest
import urllib.error
from unittest.mock import patch

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
sys.path.insert(0, str(HERE.parent))

import discord  # noqa: E402


SAMPLE_CARD = {
    "date": "2026-11-23T14:20:36Z",
    "drafts_in_use": 12,
    "files_measured": 3250,
    "finished": [11037, 41565],
    "linking": [6021, 9305],
    "linking_date": "2026-11-22",
    "linking_main": "4ae67eed04400d4cc8b4d80aa4521a89d1750bb2",
    "linking_unit": "names",
    "main": "cca4fc12cfe83ace89e550c3b7e19ad83c0cace4",
    "matched": [1751385, 6333616],
    "measured": "2026-11-23",
    "nightly": {
        "compiled": 3250,
        "files": 3250,
        "fully_matching": 526,
        "result": "partial",
    },
    "proven": [1229651, 6333616],
    "subsystems": {},
}

PREVIOUS_CARD = {
    "date": "2026-11-22T14:00:00Z",
    "drafts_in_use": 14,
    "finished": [11000, 41565],
    "linking": [6026, 9305],
    "linking_date": "2026-11-21",
    "linking_unit": "names",
    "main": "b011d73f96dcf5cf79290549d7dd8382eb4bbacb",
    "matched": [1743785, 6333616],
    "measured": "2026-11-22",
    "proven": [1229651, 6333616],
}


class FakeResponse:
    def __init__(self, body, status=200):
        self._body = body.encode("utf-8") if isinstance(body, str) else body
        self.status = status

    def read(self):
        return self._body

    def __enter__(self):
        return self

    def __exit__(self, *args):
        return False


class FormattingTests(unittest.TestCase):
    def test_emoji_bar_widths_and_bounds(self):
        self.assertEqual(discord.emoji_bar(None, "🟩"), "⬛" * 10)
        self.assertEqual(discord.emoji_bar(0.0, "🟩"), "⬛" * 10)
        self.assertEqual(discord.emoji_bar(100.0, "🟩"), "🟩" * 10)
        self.assertEqual(discord.emoji_bar(27.65, "🟩"), "🟩🟩🟩⬛⬛⬛⬛⬛⬛⬛")
        self.assertEqual(discord.emoji_bar(64.71, "🟨"), "🟨🟨🟨🟨🟨🟨⬛⬛⬛⬛")
        self.assertEqual(discord.emoji_bar(19.41, "🟦"), "🟦🟦⬛⬛⬛⬛⬛⬛⬛⬛")
        self.assertEqual(discord.emoji_bar(26.55, "🟪"), "🟪🟪🟪⬛⬛⬛⬛⬛⬛⬛")
        self.assertEqual(discord.emoji_bar(-5.0, "🟩"), "⬛" * 10)
        self.assertEqual(discord.emoji_bar(150.0, "🟩"), "🟩" * 10)
        self.assertEqual(len(discord.emoji_bar(50.0, "🟦")), 10)

    def test_format_delta(self):
        self.assertEqual(discord.format_delta(27.65, 27.53), "▲ +0.12")
        self.assertEqual(discord.format_delta(64.66, 64.71), "▼ -0.05")
        self.assertEqual(discord.format_delta(19.41, 19.41), "")
        self.assertEqual(discord.format_delta(19.412, 19.411), "")
        self.assertEqual(discord.format_delta(None, 10.0), "")
        self.assertEqual(discord.format_delta(10.0, None), "")

    def test_payload_without_previous(self):
        payload = discord.build_payload(SAMPLE_CARD)
        self.assertEqual(payload["allowed_mentions"], {"parse": []})
        self.assertEqual(len(payload["embeds"]), 1)
        embed = payload["embeds"][0]
        self.assertEqual(embed["title"], "BFME 2 Rebuild · Progress")
        self.assertEqual(embed["color"], 0x2EA043)
        self.assertEqual(embed["timestamp"], "2026-11-23T14:20:36Z")
        self.assertEqual(embed["footer"], {"text": "Measured 2026-11-23"})

        desc = embed["description"]
        self.assertIn("*Clean C++ rebuild of game.dat (1.06) compiled with Visual C++ 7.1*", desc)
        self.assertIn("**Does it work?**", desc)
        self.assertIn("**Byte match:** `27.65%`\n🟩🟩🟩⬛⬛⬛⬛⬛⬛⬛\n1,751,385 / 6,333,616 bytes compiled byte-for-byte", desc)
        self.assertIn("**Linking:** `64.71%`\n🟨🟨🟨🟨🟨🟨⬛⬛⬛⬛\n6,021 / 9,305 names provided; measured 2026-11-22", desc)
        self.assertIn("> Daily build: 3,250 of 3,250 source files compile, 526 match completely", desc)
        self.assertIn("**Clean progress**", desc)
        self.assertIn("🟦🟦⬛⬛⬛⬛⬛⬛⬛⬛ **19.41%** Code proven (matched + all references verified)", desc)
        self.assertIn("🟪🟪🟪⬛⬛⬛⬛⬛⬛⬛ **26.55%** Finished functions (11,037 / 41,565, named file + final header)", desc)
        self.assertIn("Draft headers still in use: 12", desc)
        self.assertIn("Main `cca4fc12cfe8` · [Live progress card & repo](https://github.com/Open-BFME/BFME2-Rebuild)", desc)
        self.assertNotIn("▲", desc)
        self.assertNotIn("▼", desc)

    def test_payload_with_previous_deltas(self):
        payload = discord.build_payload(SAMPLE_CARD, PREVIOUS_CARD)
        desc = payload["embeds"][0]["description"]
        # matched: 27.65% - 27.53% = +0.12
        self.assertIn("**Byte match:** `27.65%` `▲ +0.12`", desc)
        # linking: 64.71% - 64.76% = -0.05
        self.assertIn("**Linking:** `64.71%` `▼ -0.05`", desc)
        # proven unchanged -> no delta marker on proven line
        proven_line = [line for line in desc.splitlines() if "Code proven" in line][0]
        self.assertNotIn("▲", proven_line)
        self.assertNotIn("▼", proven_line)
        # finished: 26.55% - 26.46% = +0.09
        self.assertIn("Finished functions (11,037 / 41,565, named file + final header) `▲ +0.09`", desc)

    def test_missing_linking_nightly_and_drafts(self):
        minimal = {
            "main": "1234567890abcdef1234567890abcdef12345678",
            "matched": [100, 1000],
            "linking": None,
            "proven": [50, 1000],
            "finished": [5, 100],
            "nightly": None,
        }
        payload = discord.build_payload(minimal)
        embed = payload["embeds"][0]
        self.assertNotIn("timestamp", embed)
        self.assertNotIn("footer", embed)
        desc = embed["description"]
        self.assertIn("**Linking:** —\n⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛\nnot measured yet", desc)
        self.assertNotIn("Daily build:", desc)
        self.assertNotIn("Draft headers still in use", desc)
        self.assertIn("Main `1234567890ab` · [Live progress card & repo]", desc)

    def test_nightly_without_fully_matching_and_functions_linking_unit(self):
        custom = dict(
            SAMPLE_CARD,
            linking_unit="functions",
            linking_date="",
            nightly={"compiled": 3000, "files": 3250, "fully_matching": None},
        )
        desc = discord.build_description(custom)
        self.assertIn("6,021 / 9,305 functions provided\n", desc)
        self.assertIn("> Daily build: 3,000 of 3,250 source files compile\n", desc)


class WebhookValidationTests(unittest.TestCase):
    def test_valid_discord_and_discordapp_urls(self):
        for url in (
            "https://discord.com/api/webhooks/123456789/secret-token",
            "https://discordapp.com/api/webhooks/123456789/secret-token",
            "  https://discord.com/api/webhooks/123/tok?thread_id=999  ",
        ):
            cleaned = discord.validate_webhook_url(url)
            self.assertEqual(cleaned, url.strip())
        self.assertEqual(
            discord.webhook_post_url("https://discord.com/api/webhooks/123/tok"),
            "https://discord.com/api/webhooks/123/tok?wait=true",
        )
        self.assertEqual(
            discord.webhook_post_url("https://discord.com/api/webhooks/123/tok?thread_id=999"),
            "https://discord.com/api/webhooks/123/tok?thread_id=999&wait=true",
        )

    def test_invalid_webhook_urls_rejected_without_leaking_secret(self):
        secret = "SUPER_SECRET_TOKEN_XYZ"
        bad_urls = [
            "",
            "   ",
            f"http://discord.com/api/webhooks/123/{secret}",
            f"https://example.com/api/webhooks/123/{secret}",
            f"https://discord.com/api/webhooks/{secret}",
            f"https://discord.com/api/webhooks/123/{secret} extra",
        ]
        for bad in bad_urls:
            with self.assertRaises(ValueError) as ctx:
                discord.validate_webhook_url(bad)
            self.assertNotIn(secret, str(ctx.exception))


class PreviousLoaderAndCliTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.card_path = self.root / "card.json"
        self.card_path.write_text(json.dumps(SAMPLE_CARD), encoding="utf-8")

    def tearDown(self):
        self.tmp.cleanup()

    def test_load_previous_from_saved_state_card_and_history(self):
        self.assertIsNone(discord.load_previous_card(str(self.root / "missing.json")))

        saved_path = self.root / "discord-last.json"
        discord.save_state(saved_path, PREVIOUS_CARD, message_id="999", posted_at="2026-11-22T13:37:00Z", run_id="42")
        self.assertEqual(discord.load_previous_card(str(saved_path)), PREVIOUS_CARD)

        raw_path = self.root / "raw-card.json"
        raw_path.write_text(json.dumps(PREVIOUS_CARD), encoding="utf-8")
        self.assertEqual(discord.load_previous_card(str(raw_path)), PREVIOUS_CARD)

        hist_path = self.root / "history.json"
        hist_path.write_text(json.dumps([PREVIOUS_CARD, SAMPLE_CARD]), encoding="utf-8")
        self.assertEqual(discord.load_previous_card(str(hist_path), current_main=SAMPLE_CARD["main"]), PREVIOUS_CARD)

        corrupt = self.root / "corrupt.json"
        corrupt.write_text("{not json", encoding="utf-8")
        self.assertIsNone(discord.load_previous_card(str(corrupt)))

    def test_dry_run_prints_json_without_network_or_state_write(self):
        state_path = self.root / "out" / "discord-last.json"
        prev_path = self.root / "prev.json"
        prev_path.write_text(json.dumps({"card": PREVIOUS_CARD}), encoding="utf-8")
        buf = io.StringIO()
        with patch.dict(os.environ, {}, clear=True), redirect_stdout(buf):
            rc = discord.main([
                "--card", str(self.card_path),
                "--previous", str(prev_path),
                "--save-state", str(state_path),
                "--dry-run",
            ])
        self.assertEqual(rc, 0)
        parsed = json.loads(buf.getvalue())
        self.assertEqual(parsed["embeds"][0]["title"], "BFME 2 Rebuild · Progress")
        self.assertIn("`▲ +0.12`", parsed["embeds"][0]["description"])
        self.assertFalse(state_path.exists())

    def test_mocked_post_and_state_saving(self):
        state_path = self.root / "last-post" / "discord-last.json"
        captured = {}

        def fake_urlopen(req, timeout=None):
            captured["url"] = req.full_url
            captured["method"] = req.get_method()
            captured["headers"] = {k.lower(): v for k, v in req.header_items()}
            captured["body"] = json.loads(req.data.decode("utf-8"))
            captured["timeout"] = timeout
            return FakeResponse(json.dumps({"id": "1310000000000000000"}))

        webhook = "https://discord.com/api/webhooks/1234567890/top-secret-token"
        buf = io.StringIO()
        with (
            patch.dict(os.environ, {"DISCORD_PROGRESS_WEBHOOK": webhook, "GITHUB_RUN_ID": "987654"}, clear=True),
            patch.object(discord.urllib.request, "urlopen", side_effect=fake_urlopen),
            redirect_stdout(buf),
        ):
            rc = discord.main([
                "--card", str(self.card_path),
                "--save-state", str(state_path),
            ])

        self.assertEqual(rc, 0)
        self.assertEqual(captured["url"], webhook + "?wait=true")
        self.assertEqual(captured["method"], "POST")
        self.assertEqual(captured["timeout"], 30)
        self.assertEqual(captured["headers"]["user-agent"], "OpenBFME-Progress/1.0")
        self.assertEqual(captured["headers"]["content-type"], "application/json")
        self.assertEqual(captured["body"]["allowed_mentions"], {"parse": []})
        self.assertNotIn("top-secret-token", buf.getvalue())
        self.assertIn("1310000000000000000", buf.getvalue())

        saved = json.loads(state_path.read_text(encoding="utf-8"))
        self.assertEqual(saved["card"], SAMPLE_CARD)
        self.assertEqual(saved["message_id"], "1310000000000000000")
        self.assertEqual(saved["run_id"], "987654")
        self.assertRegex(saved["posted_at"], r"^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z$")

    def test_http_error_never_leaks_webhook_url(self):
        webhook = "https://discord.com/api/webhooks/1234567890/very-secret-webhook-token"
        err = urllib.error.HTTPError(webhook + "?wait=true", 401, "Unauthorized", {}, None)
        buf = io.StringIO()
        with (
            patch.dict(os.environ, {"DISCORD_PROGRESS_WEBHOOK": webhook}, clear=True),
            patch.object(discord.urllib.request, "urlopen", side_effect=err),
            redirect_stdout(buf),
        ):
            rc = discord.main(["--card", str(self.card_path)])
        self.assertEqual(rc, 1)
        out = buf.getvalue()
        self.assertIn("HTTP 401", out)
        self.assertNotIn("very-secret-webhook-token", out)
        self.assertNotIn("webhooks/1234567890", out)

    def test_url_error_never_leaks_webhook_url(self):
        webhook = "https://discord.com/api/webhooks/1234567890/very-secret-webhook-token"
        err = urllib.error.URLError(f"connection reset for {webhook}")
        buf = io.StringIO()
        with (
            patch.dict(os.environ, {"DISCORD_PROGRESS_WEBHOOK": webhook}, clear=True),
            patch.object(discord.urllib.request, "urlopen", side_effect=err),
            redirect_stdout(buf),
        ):
            rc = discord.main(["--card", str(self.card_path)])
        self.assertEqual(rc, 1)
        out = buf.getvalue()
        self.assertIn("network error", out)
        self.assertNotIn("very-secret-webhook-token", out)
        self.assertNotIn("webhooks/1234567890", out)


class DiscordWorkflowRulesTests(unittest.TestCase):
    def test_workflow_invariants(self):
        text = (ROOT / ".github/workflows/discord-progress.yml").read_text(encoding="utf-8")
        self.assertRegex(text, r"(?m)^permissions: \{\}$")
        self.assertIn("cron: '37 13 * * *'", text)
        self.assertIn("workflow_dispatch:", text)
        self.assertIn("group: bfme-discord-progress", text)
        self.assertIn("cancel-in-progress: false", text)
        self.assertIn("contents: read", text)
        self.assertIn("sha256sum -c --quiet bundle.sha256", text)
        self.assertIn("DISCORD_PROGRESS_WEBHOOK", text)
        self.assertEqual(set(re.findall(r"persist-credentials: (\S+)", text)), {"false"})
        for ref in re.findall(r"uses: (\S+)", text):
            self.assertRegex(ref, r"@[0-9a-f]{40}$")


if __name__ == "__main__":
    unittest.main()
