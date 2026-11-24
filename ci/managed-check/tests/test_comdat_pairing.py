import copy
import hashlib
import importlib.util
import json
import os
import re
import subprocess
import struct
import sys
import tempfile
import types
import unittest
from unittest.mock import Mock, patch
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[3]
MANAGED_CHECK = REPO_ROOT / "ci" / "managed-check"
NT2_GEN = REPO_ROOT / "ci" / "managed-nt2" / "engine" / "gen"
NT2_REF = REPO_ROOT / "ci" / "managed-nt2" / "engine" / "reference"
TOOLS_JUDGE = REPO_ROOT / "tools" / "judge"

if str(MANAGED_CHECK) not in sys.path:
    sys.path.insert(0, str(MANAGED_CHECK))

import compare


def build_pe32(text_rva, text_bytes, image_base=0x00400000):
    """Build a minimal valid PE32 image with one .text section at text_rva."""
    dos = bytearray(0x80)
    dos[0:2] = b"MZ"
    struct.pack_into("<I", dos, 0x3C, 0x80)

    pe_sig = b"PE\0\0"
    coff_hdr = struct.pack("<HHIIIHH", 0x014C, 1, 0, 0, 0, 0xE0, 0x0102)
    opt_hdr = bytearray(0xE0)
    struct.pack_into("<H", opt_hdr, 0, 0x010B)          # PE32 magic
    struct.pack_into("<I", opt_hdr, 28, image_base)     # ImageBase
    struct.pack_into("<I", opt_hdr, 32, 0x1000)         # SectionAlignment
    struct.pack_into("<I", opt_hdr, 36, 0x200)          # FileAlignment
    struct.pack_into("<I", opt_hdr, 92, 16)             # NumberOfRvaAndSizes

    raw_offset = 0x200
    raw_size = ((len(text_bytes) + 0x1FF) // 0x200) * 0x200
    vsize = max(len(text_bytes), 0x1000)
    sec_hdr = struct.pack(
        "<8sIIIIIIHHI",
        b".text\0\0\0",
        vsize,
        text_rva,
        raw_size,
        raw_offset,
        0,
        0,
        0,
        0,
        0x60000020,
    )
    head = bytes(dos) + pe_sig + coff_hdr + bytes(opt_hdr) + sec_hdr
    assert len(head) <= raw_offset
    return head + b"\0" * (raw_offset - len(head)) + text_bytes + b"\0" * (raw_size - len(text_bytes))


def build_coff_obj(sections_spec, externs=()):
    """Build a valid i386 Win32 COFF .obj file with COMDAT sections and relocations.

    sections_spec: list of dicts:
        - name: COFF section name (default '.text')
        - symbol: defined code symbol name
        - data: bytes
        - selection: COMDAT selection (0=non-COMDAT, 1=NODUPLICATES, 2=ANY, 5=ASSOCIATIVE)
        - assoc_section: 1-based section index when selection == 5
        - storage: COFF storage class (default 2=EXTERNAL; use 3=STATIC for $L)
        - relocs: list of (offset, reloc_type, target_symbol_name)
    externs: iterable of undefined external symbol names referenced by relocations.
    """
    string_table = bytearray(4)

    def encode_name(name):
        raw = name.encode("ascii")
        if len(raw) <= 8:
            return raw.ljust(8, b"\0")
        offset = len(string_table)
        string_table.extend(raw + b"\0")
        return struct.pack("<II", 0, offset)

    sym_records = []
    sym_index = {}

    # Emit section + aux + defined symbol for each section
    for idx, spec in enumerate(sections_spec, 1):
        sec_name = spec.get("name", ".text")
        selection = spec.get("selection", 0)
        assoc_sec = spec.get("assoc_section", 0)
        data = spec["data"]
        relocs = spec.get("relocs", ())
        has_aux = selection > 0
        sym_records.append(struct.pack(
            "<8sIhHBB",
            encode_name(sec_name),
            0,
            idx,
            0,
            3,                      # IMAGE_SYM_CLASS_STATIC
            1 if has_aux else 0,
        ))
        if has_aux:
            aux = bytearray(18)
            struct.pack_into("<I", aux, 0, len(data))
            struct.pack_into("<H", aux, 4, len(relocs))
            struct.pack_into("<h", aux, 12, assoc_sec)
            aux[14] = selection
            sym_records.append(bytes(aux))
        sym_name = spec["symbol"]
        sym_index[sym_name] = len(sym_records)
        sym_records.append(struct.pack(
            "<8sIhHBB",
            encode_name(sym_name),
            0,
            idx,
            0x20,                   # function type
            spec.get("storage", 2), # EXTERNAL (2) or STATIC (3)
            0,
        ))

    for ext in externs:
        if ext not in sym_index:
            sym_index[ext] = len(sym_records)
            sym_records.append(struct.pack("<8sIhHBB", encode_name(ext), 0, 0, 0x20, 2, 0))

    n_sections = len(sections_spec)
    header_size = 20 + 40 * n_sections
    cursor = header_size
    section_headers = []
    body_blob = bytearray()

    for spec in sections_spec:
        sec_name = spec.get("name", ".text")
        data = spec["data"]
        relocs = spec.get("relocs", ())
        selection = spec.get("selection", 0)
        chars = 0x60000020 | (0x00001000 if selection > 0 else 0)
        raw_ptr = cursor
        body_blob.extend(data)
        cursor += len(data)
        rel_ptr = cursor if relocs else 0
        for off, kind, target in relocs:
            body_blob.extend(struct.pack("<IIH", off, sym_index[target], kind))
            cursor += 10
        section_headers.append(struct.pack(
            "<8sIIIIIIHHI",
            sec_name.encode("ascii")[:8].ljust(8, b"\0"),
            0,
            0,
            len(data),
            raw_ptr,
            rel_ptr,
            0,
            len(relocs),
            0,
            chars,
        ))

    sym_table_ptr = cursor
    struct.pack_into("<I", string_table, 0, len(string_table))
    coff_hdr = struct.pack("<HHIIIHH", 0x014C, n_sections, 0, sym_table_ptr, len(sym_records), 0, 0)
    return coff_hdr + b"".join(section_headers) + bytes(body_blob) + b"".join(sym_records) + bytes(string_table)


def load_judge_module(tmp_root, variant):
    """Load either the managed-nt2 or tools/judge judge.py module in an isolated namespace."""
    home = tmp_root / f"home_{variant}"
    vc_root = home / "vc71"
    (vc_root / "Vc7" / "bin").mkdir(parents=True, exist_ok=True)
    (vc_root / "Vc7" / "bin" / "cl.exe").write_bytes(b"")
    (vc_root / "Vc7" / "include").mkdir(parents=True, exist_ok=True)
    (vc_root / "Vc7" / "lib").mkdir(parents=True, exist_ok=True)
    (vc_root / "Common7" / "IDE").mkdir(parents=True, exist_ok=True)
    wine_z = home / "pilot" / "gen" / "wine" / "dosdevices"
    wine_z.mkdir(parents=True, exist_ok=True)
    if os.name != "nt" and not (wine_z / "z:").exists():
        (wine_z / "z:").symlink_to("/")
    runner_tools = home / "bfme2-runner" / "repo" / "tools"
    runner_tools.parent.mkdir(parents=True, exist_ok=True)
    if not runner_tools.exists():
        try:
            runner_tools.symlink_to(NT2_REF)
        except OSError:
            pass

    if variant == "nt2":
        code_root = NT2_GEN
        ref_root = NT2_REF
        judge_path = NT2_GEN / "bin" / "judge.py"
    else:
        code_root = TOOLS_JUDGE
        ref_root = TOOLS_JUDGE / "runner-tools"
        judge_path = TOOLS_JUDGE / "bin" / "judge.py"

    saved_env = dict(os.environ)
    saved_path = list(sys.path)
    saved_tempdir = tempfile.tempdir
    saved_mods = {}
    clear_names = {
        "checker_configuration", "checker_identity", "host_permits", "result_cache",
        "binary_model", "relocation_proof", "emission_policy", "identity_index",
        "compiler_support", "upstream_proof", "asm_policy", "asm_allowlist",
        "import_aliases", "source_constructs", "cpp", "build", "cl_flags", "stlport_overlay",
    }
    for name in clear_names:
        if name in sys.modules:
            saved_mods[name] = sys.modules.pop(name)
    added_fcntl = False
    try:
        try:
            import fcntl  # noqa: F401
        except ImportError:
            stub_fcntl = types.ModuleType("fcntl")
            stub_fcntl.LOCK_EX = 2
            stub_fcntl.LOCK_NB = 4
            stub_fcntl.LOCK_UN = 8
            stub_fcntl.flock = lambda *_args, **_kwargs: None
            sys.modules["fcntl"] = stub_fcntl
            added_fcntl = True
        os.environ["HOME"] = str(home)
        os.environ["USERPROFILE"] = str(home)
        os.environ["VC71_ROOT"] = str(vc_root)
        os.environ["PILOT_REMOTE"] = "0"
        if variant == "tools":
            os.environ["BFME_JUDGE_STATE"] = str(home / "pilot" / "gen")
            os.environ["BFME_JUDGE_WINEPREFIX"] = str(home / "pilot" / "gen" / "wine")
            os.environ["BFME_JUDGE_RUNNER"] = str(home / "bfme2-runner" / "repo")
        sys.path[:0] = [str(code_root), str(ref_root)]
        spec = importlib.util.spec_from_file_location(f"judge_{variant}", str(judge_path))
        mod = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(mod)
        mod.record_replay = lambda *_args, **_kwargs: None
        # The temporary HOME holding the reference helpers is restored after import.
        mod.checker_hash = lambda: mod.IMPORTED_CHECKER
        return mod
    finally:
        os.environ.clear()
        os.environ.update(saved_env)
        sys.path[:] = saved_path
        tempfile.tempdir = saved_tempdir
        if added_fcntl:
            sys.modules.pop("fcntl", None)
        for name in clear_names:
            sys.modules.pop(name, None)
        sys.modules.update(saved_mods)


def run_judge_case(judge, tmpdir, *, tu, object_id, rows, pe_bytes, obj_bytes, source_text, objects=None):
    """Stage a minimal job directory and run judge.judge_tu against prebuilt COFF + PE bytes."""
    job = tmpdir / f"job_{hashlib.sha256(obj_bytes + source_text.encode()).hexdigest()[:10]}"
    (job / "obj").mkdir(parents=True, exist_ok=True)
    src_path = job / tu
    src_path.parent.mkdir(parents=True, exist_ok=True)
    src_path.write_text(source_text, encoding="utf-8")

    pe_path = job / "game.dat"
    pe_path.write_bytes(pe_bytes)
    pe_sha = hashlib.sha256(pe_bytes).hexdigest()

    map_doc = {
        "snapshot": "test-snapshot-1",
        "objects": objects or [{"object_id": object_id, "tu": tu}],
        "rows": rows,
    }
    sym_doc = {
        "map_snapshot": "test-snapshot-1",
        "retail_sha256": pe_sha,
        "data_items": [],
    }
    (job / "config").mkdir(parents=True, exist_ok=True)
    (job / "config" / "map.json").write_text(json.dumps(map_doc), encoding="utf-8")
    (job / "config" / "symbols.json").write_text(json.dumps(sym_doc), encoding="utf-8")

    expected_rows = [r for r in rows if r.get("object") == object_id and r["role"] in judge.EMITTED]
    exp_path = job / "expected" / (tu[4:] + ".json")
    exp_path.parent.mkdir(parents=True, exist_ok=True)
    exp_path.write_text(json.dumps({"snapshot": "test-snapshot-1", "object": object_id, "rows": expected_rows}), encoding="utf-8")

    obj_path = job / "obj" / (tu.replace("/", "_") + ".obj")
    obj_path.write_bytes(obj_bytes)
    obj_path.with_suffix(".obj.includes.json").write_text("[]", encoding="utf-8")
    obj_path.with_suffix(".obj.compile.json").write_text(json.dumps({
        "source_sha256": hashlib.sha256(src_path.read_bytes()).hexdigest(),
        "object_sha256": hashlib.sha256(obj_bytes).hexdigest(),
        "headers": {},
        "compiler_contract": "test",
    }), encoding="utf-8")

    judge.RETAIL = pe_path
    judge.build.EXE = pe_path
    judge.build.exe_image.cache_clear()
    pe_model = judge.PE(pe_path)
    judge.retail_pe = lambda: pe_model
    judge._coff_snapshot.cache_clear()
    judge.MAP = map_doc
    judge.MAP_NAME_INDEX = judge.map_index(map_doc)["names"]
    judge._EXTENT_MAP = None
    judge.compile_tu = lambda _job, _tu: (obj_path, "", source_text)
    led = {int(r["rva"], 16): (r["size"], r["name"], "") for r in rows}
    return judge.judge_tu(job, tu, led), obj_path


def make_row(rva, size, role, name, obj_id, *, accepted=None, fold_group="", parent=None):
    return {
        "rva": f"0x{rva:08X}",
        "size": size,
        "section": ".text",
        "role": role,
        "name": name,
        "accepted_names": list(accepted) if accepted is not None else [name],
        "object": obj_id,
        "object_id": obj_id,
        "fold_group": fold_group,
        "parent": f"0x{parent:08X}" if parent is not None else None,
    }


def make_pe_layout(base_rva, chunks):
    """Pack (rva, bytes) into a single .text buffer starting at base_rva."""
    max_end = max(rva + len(data) for rva, data in chunks)
    buf = bytearray(max_end - base_rva)
    for rva, data in chunks:
        off = rva - base_rva
        buf[off:off + len(data)] = data
    return build_pe32(base_rva, bytes(buf))


class ComdatPairingTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls._tmp = tempfile.TemporaryDirectory(prefix="bfme-comdat-test-")
        cls.tmp = Path(cls._tmp.name)
        cls.judges = {
            "nt2": load_judge_module(cls.tmp, "nt2"),
            "tools": load_judge_module(cls.tmp, "tools"),
        }

    @classmethod
    def tearDownClass(cls):
        cls._tmp.cleanup()

    def test_state_records_emitted_provider_only_on_match(self):
        from state_evidence import native_record, validate_native
        outcome = {"object_verified": True, "gates": [], "rows": [],
                   "expected": [{"rva": "0x1000", "status": "MATCH"}],
                   "select_any_providers": [{"rva": 0x1000}]}
        saved = native_record(outcome)
        self.assertEqual(saved["rows"], [])
        self.assertEqual(saved["select_any_matches"], [0x1000])
        validate_native(saved, {"status": "compiled"})
        outcome["expected"][0]["status"] = "PARTIAL"
        self.assertNotIn("select_any_matches", native_record(outcome))

    def test_exact_state_provider_in_single_unit_pr(self):
        from provider_state import for_plan
        from state_evidence import header, native_record
        owner, loser = "src/a.cpp", "src/b.cpp"
        rows = [make_row(0x1000, 1, "owned", "_shared", "first"),
                make_row(0x1010, 1, "owned", "_other", "second")]
        pe = make_pe_layout(0x1000, [(0x1000, b"\xc3"), (0x1010, b"\xc3")])
        objects = [dict(tu=owner, object_id="first", link_index="1"),
                   dict(tu=loser, object_id="second", link_index="2")]
        base, tree = "a" * 40, "b" * 40
        git = Mock()
        git.value.return_value = tree
        for variant, judge in self.judges.items():
            def measure(tu, specs):
                i = int(tu == loser)
                return run_judge_case(judge, self.tmp, tu=tu, object_id=objects[i]["object_id"],
                                      rows=rows, pe_bytes=pe, obj_bytes=build_coff_obj(specs),
                                      objects=objects, source_text=f"// FUNCTION: BFME2 {rows[i]['rva']}\n")[0]
            keeper = measure(owner, [dict(symbol="_shared", data=b"\xc3", selection=2)])
            extra = measure(loser, [dict(symbol="_other", data=b"\xc3", selection=1),
                                    dict(symbol="_shared", data=b"\x90\xc3", selection=2)])
            native = native_record(keeper)
            identity = header(base, tree)
            artifact = self.tmp / "base-state.json"
            entry = {"native": native, "compiler": {"tu": owner, "status": "compiled",
                                                     "dependencies": [["include/a.h", "digest"]]}}
            def verdict(affected=(loser,), changed=(loser,), batch=None, **binding):
                artifact.write_text(json.dumps(identity | binding | {"evidence": {owner: entry}}))
                with patch.dict(os.environ, BFME_PROVIDER_STATE=str(artifact)):
                    plan = for_plan(git, base, affected, changed)
                    git.value.assert_called_with("rev-parse", base + "^{tree}")
                result = copy.deepcopy(extra)
                measured = {loser: result, **(batch or {})}
                with patch.object(compare, "_measure_pair", return_value=({}, measured)):
                    compare.measure_pair(None, judge, plan, None, None, None)
                self.assertEqual(result["rows"], extra["rows"])
                return result

            with self.subTest(variant=variant):
                accepted = verdict()
                self.assertTrue(accepted["object_verified"])
                self.assertIs(accepted["accepted_extras"][-1]["credited"], False)
                self.assertFalse(verdict(sha="0" * 40)["object_verified"])
                self.assertFalse(verdict(affected=(owner, loser))["object_verified"])
                self.assertFalse(verdict(batch={owner: {"object_verified": False}})["object_verified"])
                self.assertFalse(verdict(changed=("include/a.h",))["object_verified"])
                native["object_verified"] = False
                self.assertFalse(verdict()["object_verified"])
                native["object_verified"] = True
                native["rows"][0]["status"] = "PARTIAL"
                self.assertFalse(verdict()["object_verified"])
                native["rows"][0]["status"] = "MATCH"
                del native["select_any_matches"]
                self.assertFalse(verdict()["object_verified"])

    def test_verified_select_any_provider(self):
        owner, loser = "first", "second"
        a, b = "src/a.cpp", "src/b.cpp"
        key = "_shared"
        rows = [make_row(0x1000, 1, "owned", key, owner),
                make_row(0x1010, 1, "owned", "_other", loser)]
        pe = make_pe_layout(0x1000, [(0x1000, b"\xc3"), (0x1010, b"\xc3")])
        for variant, judge in self.judges.items():
            def run(objects, i, specs):
                path, oid, addr = [(a, owner, 0x1000), (b, loser, 0x1010)][i]
                return run_judge_case(judge, self.tmp, tu=path, object_id=oid, rows=rows,
                                      pe_bytes=pe, obj_bytes=build_coff_obj(specs), objects=objects,
                                      source_text=f"// FUNCTION: BFME2 0x{addr:08X}\n")[0]

            for selection, storage, order, data, present, expected in (
                    (2, 2, "1", b"\xc3", True, True),
                    (2, 2, "2", b"\xc3", True, False),
                    (2, 2, "3", b"\xc3", True, False),
                    (2, 2, "", b"\xc3", True, False),
                    (2, 2, "1", b"\xcc", True, False),
                    (2, 2, "1", b"\xc3", False, False),
                    (2, 3, "1", b"\xc3", True, False),
                    *((sel, 2, "1", b"\xc3", True, False) for sel in (1, 3, 4, 6))):
                with self.subTest(variant=variant, selection=selection, storage=storage,
                                  order=order, data=data, present=present):
                    objects = [dict(tu=a, object_id=owner, link_index=order),
                               dict(tu=b, object_id=loser, link_index="2")]
                    provider = run(objects, 0, [dict(symbol=key, data=data, selection=2)])
                    emitted = run(objects, 1, [dict(symbol="_other", data=b"\xc3", selection=1),
                                               dict(symbol=key, data=b"\x90\xc3", selection=selection,
                                                    storage=storage)])
                    self.assertEqual(emitted["unexpected_symbols"], [key])
                    results = {b: emitted}
                    if present:
                        results[a] = provider
                    rows_before = copy.deepcopy(emitted["rows"])
                    judge.resolve_batch_extras(results)
                    self.assertEqual(emitted["rows"], rows_before)
                    self.assertEqual(emitted["unexpected_symbols"], [] if expected else [key])
                    self.assertEqual(emitted["function_match"], 1)
                    self.assertEqual(emitted["object_verified"], expected)
                    if expected:
                        self.assertEqual(emitted["accepted_extras"][0]["provider_tu"], a)
                        self.assertFalse(emitted["accepted_extras"][0]["credited"])

    def test_wrong_provider_relocation_cannot_certify_extra(self):
        a, b = "src/keeper.cpp", "src/extra.cpp"
        objects = [dict(tu=a, object_id="keeper", link_index="1"),
                   dict(tu=b, object_id="extra", link_index="2")]
        rows = [make_row(0x1000, 6, "owned", "_shared", "keeper"),
                make_row(0x1010, 1, "owned", "_other", "extra"),
                make_row(0x1020, 1, "owned", "_right", "unknown"),
                make_row(0x1030, 1, "owned", "_wrong", "unknown")]
        call = b"\xe8" + struct.pack("<i", 0x1020 - 0x1005) + b"\xc3"
        pe = make_pe_layout(0x1000, [(0x1000, call), (0x1010, b"\xc3"),
                                     (0x1020, b"\xc3"), (0x1030, b"\xc3")])
        for variant, judge in self.judges.items():
            with self.subTest(variant=variant):
                a_obj = build_coff_obj([dict(symbol="_shared", data=b"\xe8\0\0\0\0\xc3", selection=2,
                                             relocs=[(1, 20, "_wrong")])], externs=["_wrong"])
                b_obj = build_coff_obj([dict(symbol="_other", data=b"\xc3", selection=1),
                                        dict(symbol="_shared", data=b"\x90\xc3", selection=2)])
                results = {}
                for tu, oid, code, addr in [(a, "keeper", a_obj, 0x1000), (b, "extra", b_obj, 0x1010)]:
                    results[tu], _ = run_judge_case(judge, self.tmp, tu=tu, object_id=oid, rows=rows,
                                                    pe_bytes=pe, obj_bytes=code, objects=objects,
                                                    source_text=f"// FUNCTION: BFME2 0x{addr:08X}\n")
                self.assertEqual(results[a]["rows"][0]["status"], "WRONG_REFERENCE")
                judge.resolve_batch_extras(results)
                self.assertEqual(results[b]["unexpected_symbols"], ["_shared"])

    @unittest.skipUnless(os.environ.get("VC71_CL") and os.environ.get("VC71_LINK"),
                         "set VC71_CL and VC71_LINK to the VC 7.1 executable paths")
    def test_vc71_link_select_any_order(self):
        cl, link = os.environ["VC71_CL"], os.environ["VC71_LINK"]
        judge = self.judges["nt2"]
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            def tool(exe, *args):
                return subprocess.run([exe, *args], cwd=root, capture_output=True,
                                      text=True, check=True).stdout
            for tag, constant in (("a", 7), ("b", 13)):
                (root / (tag + ".cpp")).write_text(
                    'extern "C" {\n'
                    f'int data_{tag} = {constant};\n'
                    f'inline int shared() {{ return data_{tag} + {constant}; }}\n'
                    f'int call_{tag}() {{ return shared(); }}\n}}\n')
                tool(cl, "/nologo", "/c", "/O1", "/Ob0", "/Gy", "/Zl", tag + ".cpp")
            for first, second in (("a", "b"), ("b", "a")):
                banner = tool(link, "/dll", "/noentry", "/nodefaultlib", "/incremental:no",
                     "/opt:noref", "/opt:noicf", "/map:out.map", "/out:out.dll",
                     first + ".obj", second + ".obj")
                self.assertRegex(banner, r"Version 7\.10\.")
                symbols = {n: (int(va, 16), obj) for n, va, obj in re.findall(
                    r"^\s*\w+:\w+\s+(\S+)\s+([0-9a-fA-F]+)\s+(?:f\s+)?(?:i\s+)?(\w+\.obj)\s*$",
                    (root / "out.map").read_text(), re.M)}
                address, kept = symbols["_shared"]
                self.assertEqual(kept, first + ".obj")
                pe = judge.PE(root / "out.dll")
                obj = judge.coff(root / kept)
                sec = obj.symbols[obj.index("_shared")]["section"]
                self.assertEqual(obj.comdat[sec], 2)
                body, relocs = obj.body("_shared")
                self.assertEqual(len(relocs), 1)
                off, kind, target = relocs[0]
                self.assertEqual((kind, target), (6, "_data_" + first))
                expected = bytearray(body)
                struct.pack_into("<I", expected, off, symbols[target][0] + struct.unpack_from("<I", body, off)[0])
                self.assertEqual(pe.read(address - pe.base, len(body)), expected)
                for tag in (first, second):
                    code, refs = judge.coff(root / (tag + ".obj")).body("_call_" + tag)
                    [(off, kind, target)] = refs
                    self.assertEqual((kind, target), (20, "_shared"))
                    caller = symbols["_call_" + tag][0]
                    displacement, = struct.unpack("<i", pe.read(caller - pe.base + off, 4))
                    self.assertEqual(caller + off + 4 + displacement, address)

    def _issue_42_fixture(self):
        tu = "src/Code/GameEngineDevice/Source/W3DDevice/GameLogic/W3DGhostObject.cpp"
        oid = "O00063415"
        base_rva = 0x00001000
        # Retail-accurate bodies (offsets shifted into a compact test PE section)
        rva_rva_ph_owned = 0x00001000      # mirrors 0x000635AB (owned, placeholder, 77 B)
        rva_geom_assign = 0x00001060       # mirrors 0x00063627 (icf_fold, 75 B)
        rva_peek_vmat = 0x000010C0         # mirrors 0x000636C9 (owned, 22 B)
        rva_comdat_ph = 0x000010E0         # mirrors 0x00063BE4 (comdat, placeholder, 79 B)
        rva_unwind = 0x00001140            # mirrors 0x0075E525 (unwind, 14 B)
        rva_ehhandler = 0x00001160         # mirrors 0x0075E575 (ehhandler, 10 B)
        rva_tex_dtor_fold = 0x00001200     # mirrors 0x0017098D (icf_fold, unknown, 12 B)
        rva_release_ref = 0x00001300       # mirrors 0x0061ED10 (icf_fold, unknown, 36 B)

        # Exact retail bytes of MaterialInfoClass::Peek_Vertex_Material (22 B)
        peek_bytes = bytes.fromhex("8b4424043b41187d088b490c8b0481eb0233c0c20400")
        # Exact retail bytes of ~TextureAsset fold: mov ecx,[ecx]; test ecx,ecx; jz +5; jmp rel32; ret
        rel32_to_release = (rva_release_ref - (rva_tex_dtor_fold + 7 + 4)) & 0xFFFFFFFF
        tex_dtor_retail = bytes.fromhex("8b0985c97405e9") + struct.pack("<I", rel32_to_release) + b"\xc3"
        tex_dtor_compiled = bytes.fromhex("8b0985c97405e900000000c3")
        release_bytes = b"\x90" * 35 + b"\xc3"

        # Deliberately high-similarity retail bodies for the unwritten rows so the old bug triggers strongly!
        vdtor_bytes = bytes([0x53, 0x56, 0x8B, 0xF1] + [(i * 7) & 0xFF for i in range(66)] + [0x5E, 0x5B, 0xC2, 0x04, 0x00])
        geom_retail = vdtor_bytes[:-5] + b"\x90\x90\x5E\x5B\xC3"
        ph_owned_retail = vdtor_bytes + b"\x90\x90"
        ph_comdat_retail = vdtor_bytes + b"\x90\x90\x90\x90"
        unwind_retail = bytes.fromhex("8b0985c97405e911223344c39090")
        eh_retail = bytes.fromhex("8b0985c974e911223344")

        pe = make_pe_layout(base_rva, [
            (rva_rva_ph_owned, ph_owned_retail),
            (rva_geom_assign, geom_retail),
            (rva_peek_vmat, peek_bytes),
            (rva_comdat_ph, ph_comdat_retail),
            (rva_unwind, unwind_retail),
            (rva_ehhandler, eh_retail),
            (rva_tex_dtor_fold, tex_dtor_retail),
            (rva_release_ref, release_bytes),
        ])
        rows = [
            make_row(rva_rva_ph_owned, len(ph_owned_retail), "owned",
                     "?rva000635AB@W3DGhostObjectManager@@QAEXHH@Z", oid,
                     accepted=["?rva000635AB@W3DGhostObjectManager@@QAEXHH@Z", "?rva000635AB@Rva000635AB@@QAEXHH@Z"]),
            make_row(rva_geom_assign, len(geom_retail), "icf_fold",
                     "??4GeometryShape@@QAEAAU0@ABU0@@Z", oid, fold_group="F00063627"),
            make_row(rva_peek_vmat, len(peek_bytes), "owned",
                     "?Peek_Vertex_Material@MaterialInfoClass@@QBEPAVVertexMaterialClass@@H@Z", oid),
            make_row(rva_comdat_ph, len(ph_comdat_retail), "comdat",
                     "??0BfmeStringRecord00063BE4@@QAE@ABU0@@Z", oid),
            make_row(rva_unwind, len(unwind_retail), "unwind", "uw_0075e525", oid),
            make_row(rva_ehhandler, len(eh_retail), "ehhandler",
                     "__ehhandler$?snapShot@W3DGhostObject@@UAEXH@Z", oid),
            make_row(rva_tex_dtor_fold, len(tex_dtor_retail), "icf_fold",
                     "??1?$RefCountPtr@VTextureClass@@@@QAE@XZ", "unknown",
                     accepted=[
                         "??1?$RefCountPtr@VTextureClass@@@@QAE@XZ",
                         "??1TextureClass@@QAE@XZ",
                         "??1TextureAsset@@QAE@XZ",
                     ], fold_group="F0017098D"),
            make_row(rva_release_ref, len(release_bytes), "icf_fold",
                     "?Release_Ref@TextureBaseClass@@QAEXXZ", "unknown",
                     accepted=["?Release_Ref@TextureBaseClass@@QAEXXZ", "?ReleaseRef@AssetFactoryBase@@QAEXXZ"],
                     fold_group="F0061ED10"),
        ]
        source = (
            f"// FUNCTION: BFME2 0x{rva_rva_ph_owned:08X} size={len(ph_owned_retail)}\n"
            f"// FUNCTION: BFME2 0x{rva_peek_vmat:08X} size={len(peek_bytes)}\n"
        )
        return {
            "tu": tu, "oid": oid, "pe": pe, "rows": rows, "source": source,
            "rva_peek_vmat": rva_peek_vmat, "rva_tex_dtor_fold": rva_tex_dtor_fold,
            "rva_rva_ph_owned": rva_rva_ph_owned, "rva_geom_assign": rva_geom_assign,
            "rva_unwind": rva_unwind, "rva_ehhandler": rva_ehhandler, "rva_comdat_ph": rva_comdat_ph,
            "peek_bytes": peek_bytes, "tex_dtor_compiled": tex_dtor_compiled,
            "vdtor_bytes": vdtor_bytes, "unwind_retail": unwind_retail, "eh_retail": eh_retail,
        }

    def test_issue_42_header_inline_comdats_not_falsely_paired(self):
        fx = self._issue_42_fixture()
        base_obj = build_coff_obj([])
        # Head: Peek_Vertex_Material (selection=1) + 3 unreferenced select-any COMDATs + associative EH on unreferenced COMDAT
        head_obj = build_coff_obj([
            {"symbol": "?Peek_Vertex_Material@MaterialInfoClass@@QBEPAVVertexMaterialClass@@H@Z",
             "data": fx["peek_bytes"], "selection": 1},
            {"symbol": "??1TextureAsset@@QAE@XZ", "data": fx["tex_dtor_compiled"], "selection": 2,
             "relocs": [(7, 20, "?ReleaseRef@AssetFactoryBase@@QAEXXZ")]},
            {"symbol": "??_ETextureAsset@@QAEPAXI@Z", "data": fx["vdtor_bytes"], "selection": 2},
            {"symbol": "??1TextureBaseClass@@QAE@XZ", "data": fx["tex_dtor_compiled"], "selection": 2,
             "relocs": [(7, 20, "?ReleaseRef@AssetFactoryBase@@QAEXXZ")]},
            # Associative unwind + ehhandler attached to unreferenced ??_ETextureAsset (section 3)
            {"name": ".text$x", "symbol": "$L9901", "data": fx["unwind_retail"], "selection": 5,
             "assoc_section": 3, "storage": 3},
            {"name": ".text$x", "symbol": "__ehhandler$??_ETextureAsset@@QAEPAXI@Z", "data": fx["eh_retail"],
             "selection": 5, "assoc_section": 3},
        ], externs=["?ReleaseRef@AssetFactoryBase@@QAEXXZ"])

        for variant, judge in self.judges.items():
            with self.subTest(variant=variant):
                base_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=base_obj, source_text=fx["source"])
                head_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=head_obj, source_text=fx["source"])

                owned_by_rva = {int(r["rva"], 16): r for r in head_res["rows"]}
                exp_by_rva = {int(r["rva"], 16): r for r in head_res["expected"]}

                self.assertEqual(owned_by_rva[fx["rva_peek_vmat"]]["status"], "MATCH")
                self.assertEqual(owned_by_rva[fx["rva_peek_vmat"]]["proof"], "proven")
                self.assertEqual(owned_by_rva[fx["rva_rva_ph_owned"]]["status"], "MISSING")
                for exp_rva in (fx["rva_geom_assign"], fx["rva_comdat_ph"], fx["rva_unwind"], fx["rva_ehhandler"]):
                    self.assertEqual(exp_by_rva[exp_rva]["status"], "MISSING",
                                     f"Expected row {hex(exp_rva)} falsely paired: {exp_by_rva[exp_rva]}")

                self.assertEqual(head_res["rejected_extras"], [])
                accepted_map = {e["symbol"]: e["classification"] for e in head_res["accepted_extras"]}
                self.assertEqual(accepted_map["??1TextureAsset@@QAE@XZ"], "extra_duplicate")
                self.assertEqual(accepted_map["??_ETextureAsset@@QAEPAXI@Z"], "extra_unreferenced")
                self.assertEqual(accepted_map["??1TextureBaseClass@@QAE@XZ"], "extra_unreferenced")
                self.assertEqual(accepted_map["$L9901"], "extra_unreferenced")
                self.assertEqual(accepted_map["__ehhandler$??_ETextureAsset@@QAEPAXI@Z"], "extra_unreferenced")
                self.assertNotIn("compiled owned function order differs from retail RVA order",
                                 head_res["verification_blockers"])

                counts, rules, hints = compare.reduction(
                    compare.index({fx["tu"]: base_res}),
                    compare.index({fx["tu"]: head_res}),
                )
                self.assertEqual(counts["gained"], 1)
                self.assertEqual(rules, set(), f"Unexpected refusing rules: {rules}, hints={hints}")

    def _issue_52_fixture(self):
        tu = "src/Code/GameEngine/Source/Common/MapObject.cpp"
        oid = "O0030D3D4"
        base_rva = 0x00002000
        rva_ph_before = 0x00002000     # mirrors 0x0030D6B1 (owned, placeholder, 22 B)
        rva_get_tt = 0x00002040        # mirrors 0x0030D833 (owned, 22 B)
        rva_ph_after = 0x00002080      # mirrors 0x0030DC73 (owned, placeholder, 18 B)
        rva_final_ovr = 0x00002100     # mirrors 0x001E35DF (owned in O001E3647, 14 B)

        # Exact retail bytes of Overridable::friend_getFinalOverride (14 B)
        final_ovr_bytes = bytes.fromhex("8bc1eb028bc28b500485d275f7c3")
        # Exact retail bytes of MapObject::getThingTemplate (22 B) tail-calling rva_final_ovr
        rel32_to_ovr = (rva_final_ovr - (rva_get_tt + 15 + 4)) & 0xFFFFFFFF
        get_tt_retail = bytes.fromhex("8b411885c0740c8b480485c97407e9") + struct.pack("<I", rel32_to_ovr) + bytes.fromhex("33c0c3")
        get_tt_compiled = bytes.fromhex("8b411885c0740c8b480485c97407e90000000033c0c3")

        # High-similarity unwritten placeholder rows
        helper34_bytes = bytes.fromhex("568b742408578d7908a5a5a5") + bytes(range(1, 18)) + bytes.fromhex("5f5ec20400")
        ph_before_retail = bytes.fromhex("568b742408578d7908a5a5a5e899ffffff5f5ec20400")
        ph_after_retail = bytes.fromhex("8bc1eb028bc28b500485d275f790909090c3")

        pe = make_pe_layout(base_rva, [
            (rva_ph_before, ph_before_retail),
            (rva_get_tt, get_tt_retail),
            (rva_ph_after, ph_after_retail),
            (rva_final_ovr, final_ovr_bytes),
        ])
        rows = [
            make_row(rva_ph_before, len(ph_before_retail), "owned",
                     "?bfmeGoBTA@BfmeThingBTA@@QAEXPBUBfmeVecBTA@@@Z", oid),
            make_row(rva_get_tt, len(get_tt_retail), "owned",
                     "?getThingTemplate@MapObject@@QBEPBVThingTemplate@@XZ", oid),
            make_row(rva_ph_after, len(ph_after_retail), "owned",
                     "?rva0030DC73@Rva0030DC73@@QBEABVAsciiString@@XZ", oid),
            make_row(rva_final_ovr, len(final_ovr_bytes), "owned",
                     "?friend_getFinalOverride@Overridable@@QAEPAV1@XZ", "O001E3647",
                     accepted=["?friend_getFinalOverride@Overridable@@QAEPAV1@XZ",
                               "?getFinalOverride@Overridable@@QBEPBV1@XZ"]),
        ]
        source = (
            f"// FUNCTION: BFME2 0x{rva_ph_before:08X} size={len(ph_before_retail)}\n"
            f"// FUNCTION: BFME2 0x{rva_get_tt:08X} size={len(get_tt_retail)}\n"
            f"// FUNCTION: BFME2 0x{rva_ph_after:08X} size={len(ph_after_retail)}\n"
        )
        return {
            "tu": tu, "oid": oid, "pe": pe, "rows": rows, "source": source,
            "rva_ph_before": rva_ph_before, "rva_get_tt": rva_get_tt,
            "rva_ph_after": rva_ph_after, "rva_final_ovr": rva_final_ovr,
            "get_tt_compiled": get_tt_compiled, "final_ovr_bytes": final_ovr_bytes,
            "helper34_bytes": helper34_bytes, "ph_after_retail": ph_after_retail,
            "ph_before_retail": ph_before_retail,
        }

    def test_issue_52_referenced_and_unreferenced_comdats_not_paired_onto_placeholder_owned_rows(self):
        fx = self._issue_52_fixture()
        base_obj = build_coff_obj([])
        head_obj = build_coff_obj([
            {"symbol": "?getThingTemplate@MapObject@@QBEPBVThingTemplate@@XZ",
             "data": fx["get_tt_compiled"], "selection": 1,
             "relocs": [(15, 20, "?friend_getFinalOverride@Overridable@@QAEPAV1@XZ")]},
            {"symbol": "?friend_getFinalOverride@Overridable@@QAEPAV1@XZ",
             "data": fx["final_ovr_bytes"], "selection": 2},
            {"symbol": "?helper34@ThingTemplate@@QAEHXZ",
             "data": fx["helper34_bytes"], "selection": 2},
        ])

        for variant, judge in self.judges.items():
            with self.subTest(variant=variant):
                base_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=base_obj, source_text=fx["source"])
                head_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=head_obj, source_text=fx["source"])

                owned_by_rva = {int(r["rva"], 16): r for r in head_res["rows"]}
                self.assertEqual(owned_by_rva[fx["rva_get_tt"]]["status"], "MATCH")
                self.assertEqual(owned_by_rva[fx["rva_get_tt"]]["proof"], "proven")
                self.assertEqual(owned_by_rva[fx["rva_ph_before"]]["status"], "MISSING")
                self.assertEqual(owned_by_rva[fx["rva_ph_after"]]["status"], "MISSING")

                self.assertEqual(head_res["rejected_extras"], [])
                accepted_map = {e["symbol"]: e["classification"] for e in head_res["accepted_extras"]}
                self.assertEqual(accepted_map["?friend_getFinalOverride@Overridable@@QAEPAV1@XZ"], "extra_duplicate")
                self.assertEqual(accepted_map["?helper34@ThingTemplate@@QAEHXZ"], "extra_unreferenced")

                counts, rules, hints = compare.reduction(
                    compare.index({fx["tu"]: base_res}),
                    compare.index({fx["tu"]: head_res}),
                )
                self.assertEqual(counts["gained"], 1)
                self.assertEqual(rules, set(), f"Unexpected refusing rules: {rules}, hints={hints}")

    def test_strictness_authored_partial_owned_body_still_refused(self):
        fx = self._issue_52_fixture()
        base_obj = build_coff_obj([])
        # Contributor writes a non-inline function (selection=1, own_body=True) for placeholder row rva_ph_after
        partial_owned = fx["ph_after_retail"][:-2] + b"\x90\x90\xc3"
        head_obj = build_coff_obj([
            {"symbol": "?getThingTemplate@MapObject@@QBEPBVThingTemplate@@XZ",
             "data": fx["get_tt_compiled"], "selection": 1,
             "relocs": [(15, 20, "?friend_getFinalOverride@Overridable@@QAEPAV1@XZ")]},
            {"symbol": "?getLabel@MapObject@@QBEABVAsciiString@@XZ",
             "data": partial_owned, "selection": 1},
            {"symbol": "?friend_getFinalOverride@Overridable@@QAEPAV1@XZ",
             "data": fx["final_ovr_bytes"], "selection": 2},
        ])
        for variant, judge in self.judges.items():
            with self.subTest(variant=variant):
                base_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=base_obj, source_text=fx["source"])
                head_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=head_obj, source_text=fx["source"])
                owned_by_rva = {int(r["rva"], 16): r for r in head_res["rows"]}
                self.assertEqual(owned_by_rva[fx["rva_ph_after"]]["status"], "PARTIAL")
                _, rules, _ = compare.reduction(compare.index({fx["tu"]: base_res}), compare.index({fx["tu"]: head_res}))
                self.assertIn("partial-body", rules)

    def test_strictness_corrupted_known_elsewhere_comdat_still_rejected(self):
        # Even unreferenced ??1TextureAsset (mapped at 0x0017098D) must fail if its body disagrees with retail!
        fx = self._issue_42_fixture()
        base_obj = build_coff_obj([])
        broken_dtor = bytes.fromhex("8b0985c97505e900000000c3")  # jnz instead of jz!
        head_obj = build_coff_obj([
            {"symbol": "?Peek_Vertex_Material@MaterialInfoClass@@QBEPAVVertexMaterialClass@@H@Z",
             "data": fx["peek_bytes"], "selection": 1},
            {"symbol": "??1TextureAsset@@QAE@XZ", "data": broken_dtor, "selection": 2,
             "relocs": [(7, 20, "?ReleaseRef@AssetFactoryBase@@QAEXXZ")]},
        ], externs=["?ReleaseRef@AssetFactoryBase@@QAEXXZ"])
        for variant, judge in self.judges.items():
            with self.subTest(variant=variant):
                base_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=base_obj, source_text=fx["source"])
                head_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=head_obj, source_text=fx["source"])
                self.assertEqual([e["symbol"] for e in head_res["rejected_extras"]], ["??1TextureAsset@@QAE@XZ"])
                _, rules, _ = compare.reduction(compare.index({fx["tu"]: base_res}), compare.index({fx["tu"]: head_res}))
                self.assertIn("unexpected-code", rules)

    def test_strictness_live_unmapped_comdat_rejected_not_mispaired(self):
        # A live (referenced) select-any COMDAT with no retail identity must NOT steal a placeholder owned row,
        # and MUST be rejected as unexpected-code by classify_extras.
        fx = self._issue_52_fixture()
        base_obj = build_coff_obj([])
        head_obj = build_coff_obj([
            {"symbol": "?getThingTemplate@MapObject@@QBEPBVThingTemplate@@XZ",
             "data": fx["get_tt_compiled"], "selection": 1,
             "relocs": [(15, 20, "?unknownInline@OtherClass@@QAEHXZ")]},
            {"symbol": "?unknownInline@OtherClass@@QAEHXZ",
             "data": fx["helper34_bytes"], "selection": 2},
        ])
        for variant, judge in self.judges.items():
            with self.subTest(variant=variant):
                base_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=base_obj, source_text=fx["source"])
                head_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=head_obj, source_text=fx["source"])
                owned_by_rva = {int(r["rva"], 16): r for r in head_res["rows"]}
                self.assertEqual(owned_by_rva[fx["rva_ph_before"]]["status"], "MISSING")
                self.assertEqual([e["symbol"] for e in head_res["rejected_extras"]], ["?unknownInline@OtherClass@@QAEHXZ"])
                _, rules, _ = compare.reduction(compare.index({fx["tu"]: base_res}), compare.index({fx["tu"]: head_res}))
                self.assertIn("unexpected-code", rules)

    def test_strictness_real_owned_order_violation_still_refused(self):
        fx = self._issue_52_fixture()
        base_obj = build_coff_obj([])
        # Emit rva_ph_after BEFORE rva_get_tt in COFF section order!
        head_obj = build_coff_obj([
            {"symbol": "?rva0030DC73@Rva0030DC73@@QBEABVAsciiString@@XZ",
             "data": fx["ph_after_retail"], "selection": 1},
            {"symbol": "?getThingTemplate@MapObject@@QBEPBVThingTemplate@@XZ",
             "data": fx["get_tt_compiled"], "selection": 1,
             "relocs": [(15, 20, "?friend_getFinalOverride@Overridable@@QAEPAV1@XZ")]},
            {"symbol": "?friend_getFinalOverride@Overridable@@QAEPAV1@XZ",
             "data": fx["final_ovr_bytes"], "selection": 2},
        ])
        for variant, judge in self.judges.items():
            with self.subTest(variant=variant):
                base_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=base_obj, source_text=fx["source"])
                head_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=head_obj, source_text=fx["source"])
                self.assertIn("compiled owned function order differs from retail RVA order",
                              head_res["verification_blockers"])
                _, rules, _ = compare.reduction(compare.index({fx["tu"]: base_res}), compare.index({fx["tu"]: head_res}))
                self.assertIn("owned-order", rules)

    def test_step2_exact_byte_collision_does_not_steal_unrelated_row(self):
        # An unreferenced select-any COMDAT whose bytes happen to equal an unwritten owned row
        # or a real-named comdat/icf_fold row must NOT steal that row as a bogus MATCH or MISNAMED.
        fx = self._issue_52_fixture()
        base_obj = build_coff_obj([])
        head_obj = build_coff_obj([
            {"symbol": "?getThingTemplate@MapObject@@QBEPBVThingTemplate@@XZ",
             "data": fx["get_tt_compiled"], "selection": 1,
             "relocs": [(15, 20, "?friend_getFinalOverride@Overridable@@QAEPAV1@XZ")]},
            {"symbol": "?friend_getFinalOverride@Overridable@@QAEPAV1@XZ",
             "data": fx["final_ovr_bytes"], "selection": 2},
            # Unreferenced header COMDAT whose bytes are byte-for-byte identical to unwritten rva_ph_after!
            {"symbol": "?unrelatedInline@OtherHeader@@QAEXXZ",
             "data": fx["ph_after_retail"], "selection": 2},
        ])
        for variant, judge in self.judges.items():
            with self.subTest(variant=variant):
                base_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=base_obj, source_text=fx["source"])
                head_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=head_obj, source_text=fx["source"])
                owned_by_rva = {int(r["rva"], 16): r for r in head_res["rows"]}
                self.assertEqual(owned_by_rva[fx["rva_ph_after"]]["status"], "MISSING")
                accepted_map = {e["symbol"]: e["classification"] for e in head_res["accepted_extras"]}
                self.assertEqual(accepted_map["?unrelatedInline@OtherHeader@@QAEXXZ"], "extra_unreferenced")
                _, rules, hints = compare.reduction(compare.index({fx["tu"]: base_res}), compare.index({fx["tu"]: head_res}))
                self.assertEqual(rules, set(), f"Unexpected refusing rules: {rules}, hints={hints}")

    def test_step2_exact_byte_collision_does_not_misname_expected_row(self):
        fx = self._issue_42_fixture()
        base_obj = build_coff_obj([])
        # Unreferenced header COMDAT whose bytes happen to match GeometryShape::operator= (icf_fold) exactly!
        geom_exact = fx["vdtor_bytes"][:-5] + b"\x90\x90\x5E\x5B\xC3"
        head_obj = build_coff_obj([
            {"symbol": "?Peek_Vertex_Material@MaterialInfoClass@@QBEPAVVertexMaterialClass@@H@Z",
             "data": fx["peek_bytes"], "selection": 1},
            {"symbol": "??_ETextureAsset@@QAEPAXI@Z",
             "data": geom_exact, "selection": 2},
        ])
        for variant, judge in self.judges.items():
            with self.subTest(variant=variant):
                base_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=base_obj, source_text=fx["source"])
                head_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=head_obj, source_text=fx["source"])
                exp_by_rva = {int(r["rva"], 16): r for r in head_res["expected"]}
                self.assertEqual(exp_by_rva[fx["rva_geom_assign"]]["status"], "MISSING")
                accepted_map = {e["symbol"]: e["classification"] for e in head_res["accepted_extras"]}
                self.assertEqual(accepted_map["??_ETextureAsset@@QAEPAXI@Z"], "extra_unreferenced")
                _, rules, hints = compare.reduction(compare.index({fx["tu"]: base_res}), compare.index({fx["tu"]: head_res}))
                self.assertEqual(rules, set(), f"Unexpected refusing rules: {rules}, hints={hints}")

    def test_strictness_broken_expected_comdat_in_same_tu_still_triggers_emitted_row(self):
        fx = self._issue_42_fixture()
        base_obj = build_coff_obj([])
        broken_geom = fx["vdtor_bytes"][:-10] + b"\x90" * 10
        head_obj = build_coff_obj([
            {"symbol": "?Peek_Vertex_Material@MaterialInfoClass@@QBEPAVVertexMaterialClass@@H@Z",
             "data": fx["peek_bytes"], "selection": 1,
             "relocs": [(0, 20, "??4GeometryShape@@QAEAAU0@ABU0@@Z")]},
            {"symbol": "??4GeometryShape@@QAEAAU0@ABU0@@Z",
             "data": broken_geom, "selection": 2},
        ])
        for variant, judge in self.judges.items():
            with self.subTest(variant=variant):
                base_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=base_obj, source_text=fx["source"])
                head_res, _ = run_judge_case(judge, self.tmp, tu=fx["tu"], object_id=fx["oid"],
                                             rows=fx["rows"], pe_bytes=fx["pe"], obj_bytes=head_obj, source_text=fx["source"])
                exp_by_rva = {int(r["rva"], 16): r for r in head_res["expected"]}
                self.assertEqual(exp_by_rva[fx["rva_geom_assign"]]["status"], "PARTIAL")
                _, rules, _ = compare.reduction(compare.index({fx["tu"]: base_res}), compare.index({fx["tu"]: head_res}))
                self.assertIn("emitted-row", rules)


if __name__ == "__main__":
    unittest.main()
