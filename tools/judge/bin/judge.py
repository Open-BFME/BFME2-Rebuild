#!/usr/bin/env python3
"""Pilot judge: compile TUs of a bfme2-next revision and compare every annotated function to retail.

usage: judge.py REV TU [TU ...] [--diff 0xRVA] [--json]
       judge.py REV --all [--json]
  REV    a branch or commit in the local checker data
  TU     paths under src/
  --all  every src/**/*.cpp and *.c; an empty file list is refused, never read as "every file"
Statuses: MATCH (masked bytes equal, same length), PARTIAL n% (best compiled symbol), MISSING, COMPILE_ERROR.
"""
import hashlib
import concurrent.futures, csv, difflib, json, os, re, shutil, subprocess, sys, tarfile, io, tempfile, time, uuid, pickle
from pathlib import Path

CODE_ROOT = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path.insert(0, str(CODE_ROOT))
from checker_configuration import FARM, REPO, RETAIL, RUNNER, RUNNER_MODULES, STATE, WINE_PREFIX
HOME = Path.home()
PILOT = STATE
PILOT.mkdir(parents=True, exist_ok=True)
os.environ["WINEPREFIX"] = str(WINE_PREFIX)
os.environ["TMPDIR"] = str(PILOT / "tmp")
(PILOT / "tmp").mkdir(exist_ok=True)
tempfile.tempdir = str(PILOT / "tmp")
BARE = REPO
sys.path.insert(1, str(RUNNER_MODULES))
sys.path.append(str(RUNNER / "tools"))
from checker_identity import checker_digest,verdict_digest
from host_permits import Permit,host_slots
CODE_ROOT=Path(__file__).resolve().parents[1]
# The result-cache key and the CHECKER_CHANGED guards use the verdict identity: operational edits (permits, dispatch,
# workers, logging) neither empty the cache nor void in-flight checks. Logs and receipts keep the full digest.
IMPORTED_CHECKER=verdict_digest(CODE_ROOT)
IMPORTED_FULL=checker_digest(CODE_ROOT)
from result_cache import GitInputs,ResultCache,namespace,digest_file,forget_namespaces,remembered_namespace
from binary_model import COFF,PE
from relocation_proof import Resolver
from emission_policy import folded_identity,classify_extras,compiler_local,GENERATED_DESCRIPTIONS
from identity_index import map_index,symbol_index,function_span
import compiler_support
from upstream_proof import pristine_c
import asm_policy
from functools import lru_cache
import build  # noqa: E402  (read-only use: retail bytes, COFF parsing, toolchain paths)

build.EXE = RETAIL
VC = build.vc71_root()
CL = str(VC / "Vc7" / "bin" / "cl.exe")
WINE = shutil.which("wine")
BASE = ["-nologo", "-c", "-O2", "-GR-", "-EHsc-", "-O1", "-arch:SSE", "-G7", "-DNDEBUG", "-MD", "-EHsc", "-Gy"]
ALLOWED = re.compile(r"^[-/](O[12bdgistxy][0-9-]*|Ob[0-2]|Oy-?|G[3-7BRSdrsyFfhz]-?|GS-?|Gy-?|EH[sca-]+|arch:SSE2?|Zc:[a-zA-Z]+-?|D[A-Za-z_][A-Za-z0-9_]*(=[A-Za-z0-9_]*)?|U[A-Za-z_]+|Zp[1248]?|J|vmg|vm[bsmv]|GX-?|GF|Gm-?|MD|MT|ML)$")
def wine_path(path):
    # This private prefix has a verified Z: mapping to /. Avoid a Wine process for pure path conversion.
    if (WINE_PREFIX/'dosdevices/z:').resolve()!=Path('/'):raise ValueError('gen Wine Z: mapping changed')
    return 'Z:'+str(Path(path).resolve()).replace('/','\\')
# Preserve the reference environment builder, substituting only its pure path conversion locally.
_REFERENCE_WINE_PATH=build.wine_path
build.wine_path=wine_path
ENV = dict(build.compiler_environment(VC, None))
ENV["WINEDEBUG"] = "-all"
ANNOT = re.compile(r"//\s*FUNCTION:\s*BFME2\s+0x([0-9A-Fa-f]+)")
CLASS_IN_CPP = re.compile(r"^\s*(class|struct)\s+[A-Za-z_]\w*\s*(:[^;{]*)?\{", re.M)
BANNED = re.compile(r"__asm|__declspec\s*\(\s*naked|__emit|#\s*pragma\s+optimize|alternatename", re.I)
# Source shapes that buy bytes instead of recovering source: raw this-offset member access, inlining
# overrides, and per-TU macro switches that reshape shared headers.
SHORTCUTS = [
    (re.compile(r"\(\s*(?:const\s+)?(?:unsigned\s+)?char\s*\*\s*\)\s*this\s*\)?\s*\+"), "raw (char*)this+offset member access"),
    (re.compile(r"__declspec\s*\(\s*noinline\s*\)"), "__declspec(noinline)"),
    (re.compile(r"\b__assume\s*\("), "__assume (compiler-shaping hint)"),
    (re.compile(r"^\s*#\s*define\s+BFME_\w+", re.M), "per-TU BFME_ macro switch"),
]
PLACEHOLDER = re.compile(r"[Rr]va[0-9A-Fa-f]{6,8}|Bfme|^\$L\d+$|^uw_|Gen_t_|_0x[0-9A-Fa-f]{6}")
ANON = re.compile(r"\?A0x[0-9A-Fa-f]{8}")
LOG = PILOT / "log" / "judge.jsonl"


def checker_hash():return verdict_digest(CODE_ROOT)

def remote_judge(argv,misses,cache_only=False,keep_job=False,diff=None):
    # Local hits and under-lock cache-only probes never wait for a sandbox.
    # --diff runs on the pool too (its compile and comparison are the same; the sandbox prints the disassembly diff and the
    # farm returns its stdout); only --keep-job, which needs the job dir here, and cache-only probes stay local.
    # A run left to this host with work to do (misses, or a --diff) first takes one of its local-run slots and waits for
    # memory (host_permits.local_run, held until exit), before main loads config/map.json and forks workers: runs queued
    # here then hold ~30 MB, not the parsed map (2026-11-18: the checker host at 64 GB, 325 judge processes needed 111 GB).
    def here():
        if (misses or diff) and not cache_only:
            import host_permits
            remote_judge.held=host_permits.local_run()
        return None
    if not misses or cache_only or keep_job or os.environ.get('PILOT_REMOTE')=='0':return here()
    farm=Path(os.environ['PILOT_FARM_DIR']) if 'PILOT_FARM_DIR' in os.environ else FARM
    if farm is None:return here()
    if os.environ.get('PILOT_REMOTE')!='1' and not (farm/'run/remote.on').exists():return here()
    sys.path.append(str(farm))
    try:
        import farm_client
        rc=farm_client.judge_remote(argv,PILOT,BARE,HOME)
    except Exception as error:
        print('[farm] remote judge unavailable ('+str(error)+'); judging locally on the checker host',file=sys.stderr)
        return here()
    return rc if rc is not None else here()


def ledger_names():
    return map_index(MAP)['accepted_names']


def identity(symbol, names, pair):
    """Why a byte-exact body may not be credited, or None. `names` are the ledger's names at the address;
    `pair` maps an EH parent's ledger name to the symbol it matched as."""
    norm = lambda s: ANON.sub("?A0xHASH", s)
    if compiler_local(symbol):
        # A compiled _$E<n> (dynamic initializer or atexit terminator) is numbered per object: no name confirms or
        # refutes it, and the caller binds it by position (.CRT$XCU slot, atexit operand). Only a real name at the
        # address, one neither placeholder, nor _$E<n>, nor a ??__E/??__F description of the same role, contradicts it.
        real = [n for n in names if not PLACEHOLDER.search(n) and not compiler_local(n) and not n.startswith(GENERATED_DESCRIPTIONS)]
        return "compiled compiler-generated %s; the ledger's evidenced name is %s" % (symbol, sorted(real)[0]) if real else None
    if norm(symbol) in {norm(n) for n in names}:
        return None
    # A map row's _$E<n> is not evidence of a name either (every object restarts the count).
    real = [n for n in names if not PLACEHOLDER.search(n) and not compiler_local(n)]
    if any(n.startswith("__ehhandler$") for n in names):
        # a row may carry several handler names (a real parent and a placeholder alias: 148 rows); the old
        # next() over this frozenset chose one by Python's per-process string hash, so the verdict changed run to run.
        # The real parents decide, in sorted order; a placeholder parent only when no real one is listed.
        parents = sorted(n[len("__ehhandler$"):] for n in names if n.startswith("__ehhandler$"))
        real = [x for x in parents if not PLACEHOLDER.search(x)]
        got = symbol[len("__ehhandler$"):] if symbol.startswith("__ehhandler$") else None
        if got is not None and any(got == pair.get(x, x if not PLACEHOLDER.search(x) else got) for x in (real or parents)):
            return None
        parent = (real or parents)[0]
        return "EH handler credited to the wrong parent (expected parent %s, compiled %s)" % (pair.get(parent, parent), symbol)
    if any("?$" in n for n in names) and "?$" not in symbol:
        return "ledger name is a template instantiation; compiled symbol %s is a hand-written stand-in" % symbol
    if real:
        return "compiled as %s; the ledger's evidenced name is %s" % (symbol, sorted(real)[0])
    return None             # every ledger name is a placeholder: a rename is allowed (recorded)


MAP = None
MAP_NAME_INDEX = {}

def check_configuration(job):
    # Source branches cannot mint boundary evidence by deleting rows or rewriting symbol proof.
    for name in ('map.json','header_manifest.json','symbols.json'):
        expected=subprocess.run(['git','--git-dir',str(BARE),'show','main:config/'+name],capture_output=True,check=True).stdout
        actual=(job/'config'/name).read_bytes()
        if actual!=expected:raise ValueError('candidate config/'+name+' differs from generated main; rebase/regenerate before judging')

def ledger(job):
    global MAP, MAP_NAME_INDEX
    check_configuration(job)
    MAP = json.loads((job / 'config/map.json').read_text())
    # Warm shared immutable metadata before worker threads start.
    for name in ('symbols.json','header_manifest.json'):
        if (job/'config'/name).exists():config_json(job/'config'/name)
    MAP_NAME_INDEX=map_index(MAP)['names']
    if (job/'config/symbols.json').exists():symbol_index(config_json(job/'config/symbols.json'))
    return {int(r['rva'],16):(r['size'],r['name'],'') for r in MAP['rows']}


def export(rev, dest):
    data = subprocess.run(["git", "--git-dir", str(BARE), "archive", rev], check=True, capture_output=True).stdout
    with tarfile.open(fileobj=io.BytesIO(data)) as t:
        t.extractall(dest, filter="data")
    sha = subprocess.run(["git", "--git-dir", str(BARE), "rev-parse", rev], check=True, capture_output=True, text=True).stdout.strip()
    return sha


def masked(body, relocs, n):
    m = bytearray(body[:n])
    for off, _t, _s in relocs:
        if _t==0:continue
        for i in range(off, min(off + 4, n)):
            m[i] = 0
    return bytes(m)


def mask_like(target, compiled_masked, relocs, n):
    t = bytearray(target[:n])
    for off, _t, _s in relocs:
        if _t==0:continue
        for i in range(off, min(off + 4, n)):
            t[i] = 0
    return bytes(t)


@lru_cache(maxsize=64)
def _coff_snapshot(path,mtime,size):
    return COFF(Path(path),build)
def coff(obj):
    st=obj.stat();return _coff_snapshot(str(obj),st.st_mtime_ns,st.st_size)
def read_body(obj,name,detail=False):return coff(obj).body(name,detail)
def code_symbols(obj):return coff(obj).code_symbols()
@lru_cache(maxsize=1)
def retail_pe():
    return PE(RETAIL)
# Content digests are required: this host can preserve both timestamps across rapid same-size writes.
_CONFIG_CACHE={}
_DRAFT_CACHE={}
import threading
_CONFIG_LOCK=threading.RLock()
def _config_read(path):
    payload=path.read_bytes();digest=hashlib.sha256(payload).digest();key=(str(path),digest)
    with _CONFIG_LOCK:
        if key not in _CONFIG_CACHE:
            if len(_CONFIG_CACHE)>=16:_CONFIG_CACHE.pop(next(iter(_CONFIG_CACHE)))
            _CONFIG_CACHE[key]=json.loads(payload)
        return _CONFIG_CACHE[key],digest
def config_json(path):return _config_read(path)[0]
def draft_index(job,path):
    document,digest=_config_read(path);key=(str(job),str(path),digest)
    with _CONFIG_LOCK:
        if key not in _DRAFT_CACHE:
            if len(_DRAFT_CACHE)>=8:_DRAFT_CACHE.pop(next(iter(_DRAFT_CACHE)))
            _DRAFT_CACHE[key]={(job/d['path']).resolve():d for d in document.get('drafts',[])}
        return _DRAFT_CACHE[key]

def relocation_checker(job,obj,paired,unit=None):
    symbols=config_json(job/'config/symbols.json') if (job/'config/symbols.json').exists() else {}
    if symbols and symbols['map_snapshot']!=MAP['snapshot']:raise ValueError('symbols.json snapshot differs from map')
    if symbols and symbols['retail_sha256']!=retail_pe().sha256:raise ValueError('retail image differs from symbol evidence')
    return Resolver(coff(obj),retail_pe(),MAP,symbols,identity,paired,unit)


def existing_include(path):
    if path.is_file():return path.resolve()
    current=Path(path.anchor)
    try:
        for part in path.parts[1:]:
            direct=current/part
            if direct.exists():current=direct;continue
            matches=[p for p in current.iterdir() if p.name.casefold()==part.casefold()]
            if len(matches)!=1:return None
            current=matches[0]
        return current.resolve() if current.is_file() else None
    except OSError:return None

def compiler_base(text):
    if re.search(r'^// cl-profile: upstream[ \t]*$',text,re.M):return ['-nologo','-c','-Gy']
    # `// cl-arch: x87`: retail's object had no /arch:SSE (x87 code and branches where SSE gives cmov); VC7.1 has no
    # flag that turns /arch:SSE off once given, so the file asks for BASE without it.
    if re.search(r'^// cl-arch: x87[ \t]*$',text,re.M):return [f for f in BASE if f!='-arch:SSE']
    return BASE

# Per-process memos for the cache lookup (each check used to verify the STLport overlay and re-hash the toolchain and
# retail image twice). A tool digest is reused only while size, mtime, inode and ctime are all unchanged; any write
# changes ctime, which a restored mtime cannot hide. The include-namespace walk of a cache lookup is remembered per root
# until the end-of-run recheck (result_cache.remembered_namespace); cache_context's walk is never remembered.
_OVERLAY=None
_TOOL_DIGESTS={}
def stlport46_overlay():
    global _OVERLAY
    import stlport_overlay
    stlport_overlay.ROOT = PILOT
    if _OVERLAY is None:
        _OVERLAY = stlport_overlay.materialize()
    return _OVERLAY

def tool_digest(path):
    st = os.stat(path); key = (str(path), st.st_size, st.st_mtime_ns, st.st_ino, st.st_ctime_ns)
    if key not in _TOOL_DIGESTS:
        _TOOL_DIGESTS[key] = digest_file(path)
    return _TOOL_DIGESTS[key]

# STLport 4.6 is retail's STL, so it is the default for game C++: a .cpp under src/ that names no STL compiles exactly
# as if it ended with these lines (the lines a file used to carry by hand). Exempt: .c files, the pristine C trees,
# and a file that says `// msvcstl` (MSVC's own STL). A file with `// stlport46` or `// stlport` keeps its own lines.
STL_DEFAULT_LINES='\n// stlport46\n// cl: /D_STLP_USE_STATIC_LIB /D_STLP_USE_MALLOC /D_BFME_RETAIL_TREE_INSERT_LAYOUT\n'
STL_DEFAULT_EXEMPT=('src/zlib-1.1.4/','src/lua-4.0.1/')
def effective_source(tu,text):
    if not tu.startswith('src/') or not tu.lower().endswith('.cpp') or tu.startswith(STL_DEFAULT_EXEMPT):return text
    if any(compiler_support.marker(text,m) for m in ('stlport46','stlport','msvcstl')):return text
    return text+STL_DEFAULT_LINES

def compile_settings(text):
    flags = [];roots=[VC/"Vc7/include"]
    for line in text.splitlines():
        if line.startswith("// cl:"):
            for tok in line[6:].split():
                tok = tok.replace("/", "-", 1) if tok.startswith("/") else tok
                if tok==compiler_support.ALLOC_TOKEN:flags.append(tok);continue
                if re.fullmatch(r'-W[0-4]',tok):flags.append(tok);continue
                if tok.startswith('-I') and tok[2:].startswith(('include/','src/')) and '..' not in Path(tok[2:]).parts and '\\' not in tok[2:]:flags.append(tok);continue
                if not ALLOWED.match(tok):
                    raise ValueError(f"flag refused: {tok}")
                flags.append(tok)
    allocation=compiler_support.allocator_requested(text,flags)
    flags=[x for x in flags if x!=compiler_support.ALLOC_TOKEN]
    if allocation and not (compiler_support.marker(text,'stlport') or compiler_support.marker(text,'stlport46')):raise ValueError('bfmealloc_alloconly requires // stlport or // stlport46')
    # STLport opt-in exactly as build.py: `// stlport` (4.5.3) or `// stlport46` (4.6 + BFME patch layer)
    # puts STLport ahead of the VC headers; without a marker the TU sees MSVC's own STL.
    env = dict(build.compiler_environment(VC, None))
    env["WINEDEBUG"] = "-all"
    if re.search(r'^// stlport46[ \t]*$', text, re.M):
        import stlport_overlay
        # All materialized overlay files and locks stay in gen; pristine TREE and PATCHES are read-only.
        stlport_overlay.ROOT = PILOT
        overlay = stlport46_overlay()
        if allocation:
            directory=overlay/'bfmealloc_alloconly';flags.append('-I'+wine_path(directory));roots.append(directory)
        flags.extend(['-I' + build.wine_path(overlay / 'base'), '-I' + build.wine_path(stlport_overlay.TREE / 'stlport')]);roots.extend([overlay/'base',stlport_overlay.TREE/'stlport'])
    elif re.search(r'^// stlport[ \t]*$', text, re.M):
        if allocation:
            directory=RUNNER/'reference/shims/bfmealloc_alloconly';flags.append('-I'+wine_path(directory));roots.append(directory)
        flags.append('-I' + build.wine_path(RUNNER / 'vendor/stlport'));roots.append(RUNNER/'vendor/stlport')
    if allocation:flags.extend(compiler_support.allocator_static_flags(flags))
    sdk_flags,sdk_roots=compiler_support.sdk_options(text,VC,RUNNER,wine_path,build)
    return sdk_flags+flags,env,roots+sdk_roots

def compilation_contract(flags,env,base=None):
    value={"flags":(BASE if base is None else base)+["-showIncludes"]+flags+["-Iinclude"],"path_mode":"relative-source-and-project-include-v1","compiler":tool_digest(CL),"environment":{k:env.get(k,"") for k in ("INCLUDE","LIB","WINEPATH","WINEPREFIX","LANG","LC_ALL","WINEDLLOVERRIDES")}}
    return hashlib.sha256(json.dumps(value,sort_keys=True).encode()).hexdigest()

def compile_tu(job, tu):
    src = job / tu
    on_disk = src.read_text(encoding="utf-8-sig", errors="replace")
    text = effective_source(tu, on_disk)
    try:flags,env,roots=compile_settings(text)
    except ValueError as error:return None,str(error),text
    base=compiler_base(text);contract=compilation_contract(flags,env,base)
    obj = job / "obj" / (tu.replace("/", "_") + ".obj")
    obj.parent.mkdir(parents=True, exist_ok=True)
    for old in (obj,obj.with_suffix(obj.suffix+".compile.json")):
        old.unlink(missing_ok=True)
    cmd = [WINE, CL] + base + ["-showIncludes"] + flags + ["-Iinclude", "-Fo" + build.wine_path(obj), tu.replace("/","\\")]
    with tempfile.NamedTemporaryFile("w+", errors="replace") as log:
        try:
            p = subprocess.run(cmd, cwd=job, env=env, stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT, timeout=420)
            code = p.returncode
        except subprocess.TimeoutExpired:
            code = -9
        log.seek(0)
        out = log.read()
    included=[]
    for raw in re.findall(r'Note: including file:\s*(.+)',out):
        raw=raw.strip().replace('\\','/')
        if re.match(r'^[A-Za-z]:/',raw):
            drive=Path(env['WINEPREFIX'])/'dosdevices'/raw[:2].lower()
            native=drive.resolve()/raw[3:]
        else:native=job/raw
        resolved=existing_include(native)
        if resolved is not None:included.append(str(resolved))
        else:included.append('!UNRESOLVED! '+raw)
    obj.with_suffix(obj.suffix+'.includes.json').write_text(json.dumps(included))
    if code != 0 or not obj.exists():
        lines = [l for l in out.splitlines() if "error" in l.lower() or "warning C4" in l][:25]
        return None, "\n".join(lines) or out[-2000:], text
    receipt={'compiler_contract':contract,'include_capture':'msvc-showIncludes-v1','source_sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'headers':{path:hashlib.sha256(Path(path).read_bytes()).hexdigest() for path in included if not path.startswith('!UNRESOLVED! ')}}
    if src.read_text(encoding='utf-8-sig',errors='replace')!=on_disk:return None,'source changed during compilation',text
    obj.with_suffix(obj.suffix+'.compile.json').write_text(json.dumps(receipt))
    return obj, "", text


def caller_bound_targets(obj, bodies, placed, info, occupants=None):
    """{compiled symbol: retail addresses} from the references of bodies whose place is known. PLACED maps a
    compiled body to the rows it can be: one row when step 1 paired it by name, or every row it is byte-identical to;
    OCCUPANTS maps each such row to every body that could take it. A reference to another code symbol of this object
    (REL32/DIR32, not compiler-local, inside the row) counts only when every body that could occupy every candidate row
    makes that same reference at that offset, and the retail operand there, less the compiled addend, is one address in
    all those rows: whatever the final pairing, the symbol called there lives at that address."""
    occupants = occupants or {}
    out = {}
    for s, rvas in placed.items():
        if s not in bodies or not rvas or any(r not in info for r in rvas):
            continue
        data, relocs = bodies[s]
        # a caller whose own references contradict the map where it sits is no witness for its callees.
        # CastleSystem.cpp: CastleBehavior's ctor 0x3983D4 (paired by name; it calls ~vector<void*> where the map has
        # it elsewhere, WRONG) calls set<ObjectID>::clear where retail calls 0x2EE9B7, another instantiation's copy,
        # and b9_4 then barred the clear from 0x39611E, its proven row under a2c0bdcf.
        if any(reference_agreement(obj, bodies[s], r, info[r][0]) < 0 for r in rvas):
            if os.environ.get('GEN5_DEBUG'): print('CALLER-DISTRUSTED', s[:90], [hex(r) for r in rvas], file=sys.stderr)
            continue
        for off, kind, name in relocs:
            if kind not in (6, 20) or name not in bodies or name == s or compiler_local(name):
                continue
            if any((off, kind, name) not in set(bodies[o][1]) for r in rvas for o in occupants.get(r, [s]) if o in bodies):
                continue
            starts = set()
            for rva in rvas:
                if off + 4 > min(info[rva][0], len(data)):
                    starts.add(None); break
                try:
                    operand = int.from_bytes(build.read_target_bytes(rva + off, 4), 'little')
                except Exception:
                    starts.add(None); break
                addend = int.from_bytes(data[off:off + 4], 'little', signed=(kind == 20))
                if kind == 6: starts.add((operand - retail_pe().base - addend) & 0xFFFFFFFF)
                else: starts.add((rva + off + 4 + int.from_bytes(operand.to_bytes(4, 'little'), 'little', signed=True) - addend) & 0xFFFFFFFF)
            if len(starts) == 1 and None not in starts:
                out.setdefault(name, set()).update(starts)
    return out


def lands_elsewhere(body, rva, size, elsewhere):
    """True when one of BODY's references, read at retail RVA, lands where ELSEWHERE says the referenced symbol
    is not (the symbol is placed at another address): this body cannot be the one at RVA (s05: set<int>'s destructor,
    jmp clear, against the thunk 0x2B5FEC that jumps to 0x2B54F9 while clear is placed at 0x72FE6)."""
    data, relocs = body
    for off, kind, name in relocs:
        if name not in elsewhere or kind not in (6, 20) or off + 4 > min(size, len(data)):
            continue
        try:
            operand = int.from_bytes(build.read_target_bytes(rva + off, 4), 'little')
        except Exception:
            continue
        addend = int.from_bytes(data[off:off + 4], 'little', signed=(kind == 20))
        if kind == 6: start = (operand - retail_pe().base - addend) & 0xFFFFFFFF
        else: start = (rva + off + 4 + int.from_bytes(operand.to_bytes(4, 'little'), 'little', signed=True) - addend) & 0xFFFFFFFF
        if start not in elsewhere[name]:
            return True
    return False


def reference_agreement(obj, body, rva, size):
    """How many of a compiled body's references land, in retail at RVA, where the compiled reference says: a mapped
    name at its mapped address, or compiled literal/data bytes equal to retail's (+1 each; -1 for a contradiction).
    Only used to tell byte-identical candidates apart; verification still decides credit."""
    data, relocs = body
    model = coff(obj); score = 0
    for off, kind, name in relocs:
        if kind not in (6, 7, 20) or off + 4 > min(size, len(data)):
            continue
        try:
            operand = int.from_bytes(build.read_target_bytes(rva + off, 4), 'little')
            addend = int.from_bytes(data[off:off+4], 'little', signed=(kind == 20))
            if kind == 6: start = (operand - retail_pe().base - addend) & 0xFFFFFFFF
            elif kind == 7: start = (operand - addend) & 0xFFFFFFFF
            else: start = (rva + off + 4 + int.from_bytes(operand.to_bytes(4, 'little'), 'little', signed=True)) & 0xFFFFFFFF
            known = None if compiler_local(name) else map_index(MAP)['names'].get(name)
            if known:
                score += 1 if start in known else -1
                continue
            defined = [i for i in model.names.get(name, []) if model.symbols[i]['section'] > 0]
            if len(defined) != 1 or model.sections[model.symbols[defined[0]]['section'] - 1]['characteristics'] & 0x20:
                continue                                  # unmapped local code: identity is the verifier's job
            i = defined[0]; n = model.extent(i)
            if n <= 0:
                continue
            score += 1 if model.bytes(model.loc(i), n) == build.read_target_bytes(start, n) else -1
        except Exception:
            continue
    return score


def xcu_initializers(model, tu):
    """{retail initializer rva: compiled symbol} by .CRT$XCU position: the k-th function pointer of the object's
    non-COMDAT .CRT$XCU sections (section order, then offset: the linker keeps one object's slots in that order) against
    the k-th retail slot the map lists for the TU's object (xcu_slots). COMDAT slots are left out (template statics:
    retail kept one slot for all objects). {} unless both lists exist, every slot is one exact DIR32 pointer, and the
    counts agree: a count that differs proves no position."""
    retail = [int(x, 16) for x in (object_for(tu).get('xcu_slots') or '').split(';') if x.strip()]
    ours = []
    for sec, s in enumerate(model.sections, 1):
        if not s['name'].startswith('.CRT$XCU') or s['characteristics'] & 0x1000:
            continue
        rel = sorted(model.relocs((sec, 0), s['raw_size']))
        if s['raw_size'] % 4 or [o for o, _k, _j in rel] != list(range(0, s['raw_size'], 4)) or any(k != 6 for _o, k, _j in rel):
            return {}
        if any(model.bytes((sec, 0), s['raw_size'])):
            return {}
        ours.extend(model.symbols[j]['name'] for _o, _k, j in rel)
    if not ours or len(ours) != len(retail):
        return {}
    base = retail_pe().base
    return {(int.from_bytes(build.read_target_bytes(slot, 4), 'little') - base) & 0xFFFFFFFF: name for name, slot in zip(ours, retail)}


def generated_operands(body, rva, generated):
    """(retail target, compiled symbol) for each reference an exactly paired body at RVA makes to a compiler-generated
    function: retail's operand at the same offset, read as reference_agreement reads it."""
    data, relocs = body; out = []
    for off, kind, name in relocs:
        if name not in generated or kind not in (6, 20) or off + 4 > len(data):
            continue
        operand = int.from_bytes(build.read_target_bytes(rva + off, 4), 'little')
        if kind == 6:
            target = operand - retail_pe().base - int.from_bytes(data[off:off+4], 'little')
        else:
            target = rva + off + 4 + int.from_bytes(operand.to_bytes(4, 'little'), 'little', signed=True)
        out.append((target & 0xFFFFFFFF, name))
    return out


VENDORED_TU = re.compile(r'^src/[A-Za-z][A-Za-z0-9_.+]*-[0-9][A-Za-z0-9_.]*/')
_LOCAL_SCAN = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/|//[^\n]*|[{};]', re.S)
def _code_depths(text):
    """For each '{' / '}' in TEXT (strings, chars and comments skipped): the stack of opener kinds, 'ns' (namespace,
    extern "C"), 'type' (class/struct/union/enum) or 'code' (anything else: a function body, an initializer).
    Returns a list of (offset, inside_code) for every CLASS_IN_CPP match position check."""
    events=[];stack=[];last=0
    for m in _LOCAL_SCAN.finditer(text):
        tok=m.group(0)
        if tok=='{':
            head=text[last:m.start()]
            if re.search(r'\bnamespace\b[^;{}]*$|\bextern\s*"C"\s*$',head):kind='ns'
            elif re.search(r'\b(class|struct|union|enum)\b[^;{}()]*$',head):kind='type'
            else:kind='code'
            stack.append(kind);events.append((m.start(),'code' in stack[:-1] or False));last=m.end()
        elif tok=='}':
            if stack:stack.pop()
            last=m.end()
        elif tok==';':last=m.end()
    return events
def class_in_cpp(text,tu):
    """CLASS_IN_CPP over TEXT with // comments removed, except that in a vendored library unit a type whose body
    opens inside a function body (or an initializer) is upstream's own local declaration and allowed."""
    code=re.sub(r"//.*", "", text)
    hits=list(CLASS_IN_CPP.finditer(code))
    if not hits:return False
    if not VENDORED_TU.match(tu):return True
    opens=dict(_code_depths(code))
    for h in hits:
        brace=code.find('{',h.start(),h.end())
        if not opens.get(brace,False):return True
    return False


def pilot_judge_tu(job, tu, led):
    t0 = time.time()
    obj, err, text = compile_tu(job, tu)
    rvas = active_rvas(tu)
    gates = []
    if class_in_cpp(text,tu) and not pristine_c(text,tu):
        gates.append("class/struct with a body defined in a .cpp (move it to include/)")
    # Inline assembly only through a named allowlist entry with its evidence comment (asm_policy.py).
    asm_policy.set_job(job)   # the revision's config/asm_allowlist.json entries apply too
    asm_free, asm_problems = asm_policy.strip_allowed(text, tu)
    if BANNED.search(asm_free):
        gates.append("banned construct (asm/naked/emit/pragma optimize/alternatename)")
    gates.extend("inline assembly allowance: " + p for p in asm_problems)
    code_only = re.sub(r"//.*", "", text)
    for pattern, why in SHORTCUTS:
        if pattern.search(code_only):
            gates.append("shortcut: " + why)
    res = {"tu": tu, "functions": len(rvas), "gates": gates, "compile_s": None, "rows": []}
    if obj is None:
        res["error"] = err
        res["rows"] = [{"rva": hex(r), "status": "COMPILE_ERROR"} for r in rvas]
        res["compile_s"] = round(time.time() - t0, 1)
        return res
    res["compile_s"] = round(time.time() - t0, 1)
    syms = code_symbols(obj)
    bodies = {}
    for s in syms:
        try:
            b, rel = read_body(obj, s)
            bodies[s] = (b, rel)
        except Exception:
            pass
    # Pair annotations with compiled symbols globally, never first-come: 1) exact masked matches,
    # 2) the ledger's own symbol name, 3) best similarity over all remaining (annotation, symbol) pairs.
    info, exact_of, scores = {}, {}, []
    for rva in rvas:
        size, oldname, _ = led.get(rva, (None, None, None))
        if size is None:
            continue
        size=function_span(MAP,rva)['size']
        info[rva] = (size, oldname, build.read_target_bytes(rva, size))
    for rva, (size, oldname, target) in info.items():
        for s, (b, rel) in bodies.items():
            body = b[:size] if len(b)>size and all(x==0xcc for x in b[size:]) else b
            mb = masked(b, rel, max(len(body), size))
            mt = mask_like(target, mb, rel, size)
            if len(body) == size and mb[:size] == mt:
                exact_of.setdefault(rva, []).append(s)
            elif not (len(body) > 4 * size + 64 or size > 4 * len(body) + 64):
                scores.append((difflib.SequenceMatcher(None, mb[:len(body)], mt, autojunk=False).ratio(), rva, s))
    used, pair = set(), {}
    # 0) Compiler-generated initializers and terminators (_$E<n>, numbered afresh in every object) pair by position
    #    only, never by name, order or similarity: an initializer by its .CRT$XCU slot against the map object's slot
    #    list (here), a terminator by the atexit operand of a body paired exactly (after step 2). Left unpaired otherwise.
    generated = {s for s in bodies if compiler_local(s)}
    positional = set()
    def pair_by_position(rva, s):
        if rva not in info or rva in pair or s in used or s not in bodies:
            return
        if map_index(MAP)['rows'][rva]['role'] not in ('initializer', 'terminator'):
            return
        if s in exact_of.get(rva, []):
            pair[rva] = ("MATCH", 100.0, s)
        else:
            pair[rva] = ("PARTIAL", round(100 * max((r for r, a, x in scores if a == rva and x == s), default=0.0), 1), s)
        used.add(s); positional.add(rva)
    for rva, s in xcu_initializers(coff(obj), tu).items():
        pair_by_position(rva, s)
    order = sorted(exact_of.items(), key=lambda kv: (map_index(MAP)['rows'][kv[0]]['role']!='owned',len(kv[1])))
    def same_eh_root(rva, syms):
        meta=map_index(MAP)['rows'][rva]
        if meta['role'] in ('unwind','funclet','ehhandler') and meta.get('parent'):
            prva=int(meta['parent'],16);ps=pair.get(prva,(None,None,None))[2]
            if ps:
                model=coff(obj);root=model.root(model.symbols[model.index(ps)]['section'])
                return [n for n in syms if model.root(model.symbols[model.index(n)]['section'])==root]
        return syms
    # 1) Identity before order: a compiled symbol carrying a row's own map name belongs to that row.
    for rva, syms in order:
        names = {info[rva][1]} | set(map_index(MAP)['rows'][rva].get('accepted_names', ()))
        own = [n for n in same_eh_root(rva, syms) if n in names and n not in used and n not in generated]
        if len(own) == 1:
            used.add(own[0]); pair[rva] = ("MATCH", 100.0, own[0])
    # 2) Byte-identical candidates left over are told apart by where their references land in retail; order only
    #    breaks ties between candidates whose references cannot be told apart.
    #    a symbol a body paired in step 1 calls or addresses, where retail's operand at that site lands at
    #    another address, lives there (one symbol, one address): it is never paired onto this row by bytes (s05: set<int>
    #    clear onto 0x2B54F9 while collectActiveTeams calls 0x72FE6).
    #    A body byte-identical to exactly one row, and that row to it alone, is placed as surely (s05's branch: the
    #    callers' rows still had placeholder names, so step 1 had not paired them).
    #    Placed by bytes: only this file's own functions, never a select-any COMDAT (an inline or template body may live
    #    at an address this file does not annotate, like the emitted clear itself).
    model_ = coff(obj)
    def own_body(n_):
        try:
            sec_ = model_.symbols[model_.index(n_)]['section']
        except Exception:
            return False
        return sec_ > 0 and model_.comdat.get(sec_) != 2
    placed = {}
    for r_, syms_ in exact_of.items():
        for n_ in syms_:
            if n_ not in generated and own_body(n_): placed.setdefault(n_, []).append(r_)
    occupants = {r_: [n_ for n_ in syms_ if n_ not in generated] for r_, syms_ in exact_of.items()}
    for r_, (st_, _p, s_) in pair.items():
        if st_ == "MATCH": placed[s_] = [r_]; occupants[r_] = [s_]
    elsewhere = caller_bound_targets(obj, bodies, placed, info, occupants)
    ranked = []
    for position, (rva, syms) in enumerate(order):
        if rva in pair:
            continue
        for n in syms:
            if n in elsewhere and rva not in elsewhere[n]:
                continue
            if lands_elsewhere(bodies[n], rva, info[rva][0], elsewhere):
                continue
            if n not in used and n not in generated:
                ranked.append((-reference_agreement(obj, bodies[n], rva, info[rva][0]), position, rva, n))
    for _score, _position, rva, n in sorted(ranked, key=lambda x: (x[0], x[1])):
        if rva in pair or n in used or n not in same_eh_root(rva, [n]):
            continue
        used.add(n); pair[rva] = ("MATCH", 100.0, n)
    # 0, continued) Terminators: retail's operand where an exactly paired body registers one (an initializer's atexit
    #    argument, a function's local static). Byte-identical bodies have their relocations at the same offsets.
    for rva, (st, _pct, s) in list(pair.items()):
        if st == "MATCH":
            for target, t in generated_operands(bodies[s], rva, generated):
                pair_by_position(target, t)
    for rva, (size, oldname, _) in info.items():
        if rva not in pair and oldname in bodies and oldname not in used and oldname not in generated:
            sc = max((r for r, a, s in scores if a == rva and s == oldname), default=0.0)
            used.add(oldname); pair[rva] = ("PARTIAL", round(100 * sc, 1), oldname)
    PLACEHOLDER_ = re.compile(r"[Rr]va[0-9A-Fa-f]{6,8}|Bfme|Gen_t_|_0x[0-9A-Fa-f]{6}")   # emission_policy.PLACEHOLDER
    def member_family(n_):
        # (member, class or template base): '?resize@?$hashtable@...' -> ('resize', 'hashtable'); '??0?$list@...' -> ('?0', 'list')
        m_ = re.match(r'\?(\?[0-9A-Z_]|[A-Za-z_]\w*@)(?:\?\$)?([A-Za-z_]\w*)', n_ or '')
        return (m_.group(1).rstrip('@'), m_.group(2)) if m_ else None
    def similarity_candidate(rva, s):
        # a select-any COMDAT carrying none of an owned row's names, and not another instantiation of the
        # same member of the same class or template, is another function's body: never a near miss of this row.
        if own_body(s) or map_index(MAP)['rows'][rva]['role'] != 'owned':
            return True
        names_ = {info[rva][1]} | set(map_index(MAP)['rows'][rva].get('accepted_names', ()))
        if not any(n_ and not PLACEHOLDER_.search(n_) for n_ in names_):
            return True                       # a placeholder-named row: similarity is the only lead to its identity
        fam_ = member_family(s)
        return s in names_ or (fam_ is not None and fam_ in {member_family(n_) for n_ in names_})
    for r, rva, s in sorted(scores, reverse=True):
        if r < 0.30:
            break
        if rva not in pair and s not in used and s not in generated and similarity_candidate(rva, s):
            used.add(s); pair[rva] = ("PARTIAL", round(100 * r, 1), s)
    names_at = ledger_names()
    # EH parents: the ledger name of each matched parent -> the symbol it compiled as
    parent_sym = {}
    for rva, (st, _pct, s) in pair.items():
        for n in names_at.get(rva, ()):
            parent_sym[n] = s
    for rva in rvas:
        if rva not in info:
            res["rows"].append({"rva": hex(rva), "status": "UNKNOWN_RVA"}); continue
        size, oldname, _ = info[rva]
        if rva in pair:
            st, pct, s = pair[rva]
            row = {"rva": hex(rva), "status": st, "symbol": s, "size": size}
            if rva in positional:
                row["paired_by"] = "position"
            if st == "MATCH":
                names = names_at.get(rva, frozenset([oldname]))
                role=map_index(MAP)['rows'][rva]['role']
                why = None if s.startswith('$L') and role in ('unwind','funclet') else identity(s, names, parent_sym)
                if why and role in ('comdat','icf_fold') and folded_identity(s,names):why=None
                if why:
                    row.update(status="MISNAMED", why=why)
                elif ANON.sub("", s) not in {ANON.sub("", n) for n in names} and s not in generated:
                    row["renamed_from"] = sorted(names)[0]     # a compiled _$E<n> is no name to rename a row to
            if st == "PARTIAL":
                row.update(pct=pct, old=oldname)
            res["rows"].append(row)
        else:
            res["rows"].append({"rva": hex(rva), "status": "MISSING", "size": size, "old": oldname})
    return res


EMITTED = {'comdat','icf_fold','ehhandler','funclet','unwind','initializer','terminator','thunk','import_thunk'}
def object_for(tu):
    matches=map_index(MAP)['objects'].get(tu,[])
    if len(matches)!=1: raise ValueError('TU must map to exactly one object: '+tu)
    return matches[0]
def object_rows(tu):
    oid=object_for(tu)['object_id']
    return map_index(MAP)['by_object'].get(oid,[])
def active_rvas(tu):
    return [int(r['rva'],16) for r in object_rows(tu) if r['role']=='owned' or r['role'] in EMITTED]
def included_headers(job,source):
    tu=source.relative_to(job).as_posix()
    deps=job/'obj'/(tu.replace('/','_')+'.obj.includes.json')
    if deps.exists():
        return list(dict.fromkeys(Path(p) for p in json.loads(deps.read_text()) if not p.startswith('!UNRESOLVED! ') and Path(p).is_file() and Path(p).resolve()!=source.resolve()))
    pending=[source];seen=set();headers=[]
    while pending:
        p=pending.pop();real=p.resolve()
        if real in seen:continue
        seen.add(real)
        if p!=source:headers.append(p)
        for token in re.findall(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]',p.read_text(errors='replace'),re.M):
            candidates=[p.parent/token,job/'include'/token]
            found=next((x for x in candidates if x.is_file()),None)
            if found is not None:pending.append(found)
    return headers
def trusted_external_header(path):
    roots=[VC, RUNNER/'vendor/stlport', RUNNER/'vendor/stlport-4.6', PILOT/'build/stlport46',RUNNER/'reference/shims/bfmealloc_alloconly',RUNNER/'vendor/dxsdk-aug2005/Include']
    return any(path.resolve().is_relative_to(root.resolve()) for root in roots)
def header_label(job,path):
    return str(path.relative_to(job)) if path.is_relative_to(job) else str(path)
def compiler_receipt_problems(job,tu,obj):
    path=obj.with_suffix(obj.suffix+'.compile.json')
    if not path.exists():return ['no compiler input/object receipt; recompile this TU']
    receipt=json.loads(path.read_text());problems=[]
    if hashlib.sha256((job/tu).read_bytes()).hexdigest()!=receipt['source_sha256']:problems.append('source changed since compilation')
    if hashlib.sha256(obj.read_bytes()).hexdigest()!=receipt['object_sha256']:problems.append('compiled object changed after its receipt')
    for name,digest in receipt['headers'].items():
        p=Path(name)
        if not p.exists() or hashlib.sha256(p.read_bytes()).hexdigest()!=digest:problems.append('header changed since compilation: '+name)
    return problems
def judge_tu(job,tu,led):
    if Path(tu).is_absolute() or '..' in Path(tu).parts or not tu.startswith('src/'):
        raise ValueError('invalid TU path')
    source=job/tu
    # Canonical map lives at the revision being judged; land refuses identity edits.
    rr=object_rows(tu);owned=[int(r['rva'],16) for r in rr if r['role']=='owned']
    annotated=[int(x,16) for x in ANNOT.findall(source.read_text())]
    res=pilot_judge_tu(job,tu,led)
    hint=compiler_support.optimization_hint(source.read_text(),object_for(tu).get('flag_profile',''),BASE)
    if hint:res['optimization_hint']=hint
    capture=job/'obj'/(tu.replace('/','_')+'.obj.includes.json')
    if capture.exists():res['gates'].extend('compiler include capture failed: '+x for x in json.loads(capture.read_text()) if x.startswith('!UNRESOLVED! '))
    side=job/'expected'/(tu[4:]+'.json')
    wanted=[r for r in rr if r['role'] in EMITTED]
    if not side.exists() or json.loads(side.read_text()).get('rows')!=wanted:
        res['gates'].append('expected sidecar differs from map identity')
    if annotated!=owned:res['gates'].append('annotations differ from complete ordered map-owned rows')
    # Retain pilot .cpp gates and apply banned/shortcut scans to every shared header too.
    headers=included_headers(job,source)
    for h in headers:
        if not h.resolve().is_relative_to(job.resolve()):
            if trusted_external_header(h):continue
            res['gates'].append('external project header is not carried in the revision: '+str(h))
        text=h.read_text(errors='replace')
        asm_free,_asm_problems=asm_policy.strip_allowed(text,header_label(job,h))
        if BANNED.search(asm_free) or any(p.search(re.sub(r'//.*','',text)) for p,_ in SHORTCUTS):
            res['gates'].append('header shortcut/banned construct: '+header_label(job,h))
    meta={int(r['rva'],16):r for r in rr}
    paired={int(r['rva'],16):r['symbol'] for r in res['rows'] if r.get('symbol') and r['status']=='MATCH'}
    obj=job/'obj'/(tu.replace('/','_')+'.obj')
    expected=[];owned_out=[]
    checker=relocation_checker(job,obj,paired,object_for(tu)) if obj.exists() else None
    for row in res['rows']:
        rva=int(row['rva'],16);m=meta[rva];row['role']=m['role'];row['credited']=False
        span=function_span(MAP,rva);row['code_size']=span['code_size'];row['comparison_size']=span['size'];row['jump_tables']=[r['rva'] for r in span['inline_tables']]
        if row['status']=='MATCH' and checker:
            parent=int(m['parent'],16) if m.get('parent') else None
            check=checker.verify(coff(obj).index(row['symbol']),rva,parent)
            row['relocation_check']=check
            candidate=folded_identity(row['symbol'],m['accepted_names']) if m['role'] in ('comdat','icf_fold') else None
            if candidate:row['folded_identity_candidate']={**candidate,'rva':'0x%08X'%rva}
            if check['status']=='MATCH':
                folds=[x for x in check.get('proofs',[]) if x.get('kind')=='folded_identity' and int(x['rva'],16)==rva]
                if folds:row['folded_identity']=folds[-1]
            if m['role']!='owned' and check['status']!='MATCH':row.update(status=check['status'],why=check['why'])
            # Equal bytes with a reference proven wrong is not a Function MATCH; it keeps its own status and cause.
            if m['role']=='owned' and check['status']=='WRONG_RELOCATION':row.update(status='WRONG_REFERENCE',why=check['why'])
        (owned_out if m['role']=='owned' else expected).append(row)
    for row in owned_out:
        row['credited']=row['status']=='MATCH'
        if row['credited']:row['proof']='proven' if row.get('relocation_check',{}).get('status')=='MATCH' else 'unproven'
    res['rows']=owned_out;res['expected']=expected;res['functions']=len(owned)
    res['raw_owned_matches']=sum(r['status']=='MATCH' for r in owned_out)
    if checker_hash()!=IMPORTED_CHECKER:res['gates'].append('CHECKER_CHANGED: re-run this judge')
    blockers=list(res['gates'])
    if obj.exists():blockers.extend(compiler_receipt_problems(job,tu,obj))
    blockers.extend(x['rva']+' owned references '+x['relocation_check']['status']+': '+x['relocation_check'].get('why','') for x in owned_out if x.get('relocation_check',{}).get('status','MATCH')!='MATCH')
    blockers.extend('%s owned %s'%(x['rva'],x['status']) for x in owned_out if x['status']!='MATCH')
    blockers.extend('%s emitted %s: %s'%(x['rva'],x['status'],x.get('why','')) for x in expected if x['status']!='MATCH')
    if res.get('error'):blockers.append('compile error: '+res['error'])
    if not owned_out:blockers.append('no owned functions to verify')
    order=[]
    if obj.exists():
        _,sections,symbols=build._object_layout(str(obj))
        for row in owned_out:
            if row.get('symbol'):
                positions={(x['section'],x['value']) for x in symbols if x['name']==row['symbol'] and x['section']>0}
                if len(positions)==1:order.append((int(row['rva'],16),next(iter(positions))))
                else:blockers.append(row['rva']+' has ambiguous/missing COFF position')
        if [v for _,v in order]!=sorted(v for _,v in order):blockers.append('compiled owned function order differs from retail RVA order')
        paired_symbols={r['symbol'] for r in owned_out+expected if r.get('symbol')}
        extras=sorted(set(code_symbols(obj))-paired_symbols)
        accepted_extras,rejected_extras=classify_extras(checker,paired_symbols,extras)
        if rejected_extras:blockers.append('unexpected compiled code symbols: '+', '.join(x['symbol']+' ('+x['why']+')' for x in rejected_extras))
    else:
        extras=[];accepted_extras=[];rejected_extras=[];blockers.append('no compiled object for order verification')
    manifest=job/'config/header_manifest.json'
    draft_by_path=draft_index(job,manifest) if headers and manifest.exists() else {}
    draft_dependencies=[]
    for h in headers:
        d=draft_by_path.get(h.resolve())
        if d:
            draft_dependencies.append(d['path'])
            if d.get('placeholder_virtuals') or d.get('field_conflicts'):
                blockers.append('unresolved draft '+d['path']+': '+'; '.join(d.get('needs',[])))
        elif re.search(r'virtual\s+void\s+vfunc_0x|// - unresolved field conflicts',h.read_text(errors='replace')):
            blockers.append('placeholder virtual in '+header_label(job,h))
    res['function_match']=sum(r['credited'] for r in owned_out)
    res['function_match_proven']=sum(r.get('proof')=='proven' for r in owned_out)
    res['function_match_unproven']=sum(r.get('proof')=='unproven' for r in owned_out)
    res['wrong_reference']=sum(r['status']=='WRONG_REFERENCE' for r in owned_out)
    res['owned_order']=[{'rva':hex(a),'section':b[0],'offset':b[1]} for a,b in order]
    res['unexpected_symbols']=[x['symbol'] for x in rejected_extras]
    res['accepted_extras']=accepted_extras
    res['extra_duplicate']=[x for x in accepted_extras if x['classification']=='extra_duplicate']
    res['extra_unreferenced']=[x for x in accepted_extras if x['classification']=='extra_unreferenced']
    res['rejected_extras']=rejected_extras
    res['all_extra_symbols']=extras
    folds=[proof for row in owned_out+expected for proof in row.get('relocation_check',{}).get('proofs',[]) if row.get('relocation_check',{}).get('status')=='MATCH' and proof.get('kind')=='folded_identity']
    folds += [proof for extra in accepted_extras for proof in extra.get('relocation_check',{}).get('proofs',[]) if proof.get('kind')=='folded_identity']
    res['folded_identity']=list({(x['rva'],x['compiled_name'],x['retail_name']):x for x in folds}.values())
    res['extent_evidence']=extent_evidence(tu)
    res['draft_dependencies']=draft_dependencies
    res['verification_blockers']=sorted(set(blockers))
    res['object_verified']=not blockers
    res['source_completeness']={'status':'unproven','reason':'Assigned owned/emitted rows were checked; the map extent and possible unassigned contributions do not prove a complete retail source file.','assigned_owned':len(owned),'matched_owned':res['function_match'],'object_verified':res['object_verified']}
    if res['object_verified']:record_replay(job,tu,rr,res)
    return res

_EXTENT_MAP=None
_EXTENT_INDEX={}
def extent_evidence(tu):
    global _EXTENT_MAP,_EXTENT_INDEX
    if _EXTENT_MAP is not MAP:
        text=sorted((r for r in MAP['rows'] if r.get('section')=='.text'),key=lambda r:int(r['rva'],16))
        spans={}
        for i,row in enumerate(text):
            oid=row.get('object');spans.setdefault(oid,[i,i])[1]=i
        index={}
        for obj in MAP['objects']:
            span=spans.get(obj['object_id'])
            left=bool(span and span[0]>0 and text[span[0]-1].get('object')=='unknown')
            right=bool(span and span[1]+1<len(text) and text[span[1]+1].get('object')=='unknown')
            index[obj.get('tu')]={'unknown_left':left,'unknown_right':right,'boundary_eligible':not (left and right),
                'reason':'unknown-row neighbours on both sides; assigned-row proof only' if left and right else 'no two-sided unknown-row island'}
        _EXTENT_INDEX=index
        _EXTENT_MAP=MAP
    return _EXTENT_INDEX[tu]

def record_replay(job,tu,rows,result):
    import fcntl,hashlib
    first=min(int(r['rva'],16) for r in rows if r['role']=='owned')
    last=max(int(r['rva'],16) for r in rows if r['role']=='owned')
    last_end=max(int(r['rva'],16)+function_span(MAP,int(r['rva'],16))['size'] for r in rows if r['role']=='owned')
    obj=job/'obj'/(tu.replace('/','_')+'.obj')
    content=hashlib.sha256(obj.read_bytes()).hexdigest()
    record={'object_id':object_for(tu)['object_id'],'tu':tu,'first_rva':'0x%08X'%first,'last_rva':'0x%08X'%last,'end_rva':'0x%08X'%last_end,'evidence_class':('R' if extent_evidence(tu)['boundary_eligible'] else 'V'),'snapshot':MAP['snapshot'],'source_sha256':hashlib.sha256((job/tu).read_bytes()).hexdigest(),'object_sha256':content,'headers_sha256':hashlib.sha256(json.dumps({header_label(job,h):hashlib.sha256(h.read_bytes()).hexdigest() for h in included_headers(job,job/tu)},sort_keys=True).encode()).hexdigest(),'compiler_sha256':hashlib.sha256(Path(CL).read_bytes()).hexdigest(),'expected_sha256':hashlib.sha256((job/'expected'/(tu[4:]+'.json')).read_bytes()).hexdigest(),'symbols_sha256':hashlib.sha256((job/'config/symbols.json').read_bytes()).hexdigest() if (job/'config/symbols.json').exists() else 'none','checker_sha256':IMPORTED_FULL,'retail_sha256':retail_pe().sha256}
    record.update(complete_file='unproven',complete_file_reason='assigned-row rebuild proof; complete original TU extent is not established',boundary_eligible=str(int(extent_evidence(tu)['boundary_eligible'])),boundary_reason=extent_evidence(tu)['reason'],previous_evidence_class='')
    path=PILOT/'replay_evidence.csv'
    with path.open('a+',newline='') as f:
        fcntl.flock(f,fcntl.LOCK_EX);f.seek(0)
        existing=list(csv.DictReader(f));f.seek(0,2)
        for old in existing:
            if any(o.get('tu')==old.get('tu') for o in MAP['objects']):
                extent=extent_evidence(old['tu']);old['boundary_eligible']=str(int(extent['boundary_eligible']));old['boundary_reason']=extent['reason']
                if not extent['boundary_eligible'] and old.get('evidence_class')=='R':
                    old['previous_evidence_class']='R';old['evidence_class']='V'
        writer=csv.DictWriter(f,fieldnames=list(record))
        f.seek(0);old_header=next(csv.reader(f),[])
        if old_header!=list(record):
            f.seek(0);f.truncate();writer.writeheader()
            for old in existing:writer.writerow({k:old.get(k,'not-recorded-prepass4') for k in record})
        f.seek(0,2)
        if not any(all(r.get(k)==v for k,v in record.items()) for r in existing):writer.writerow(record)



def disasm(blob, base):
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as f:
        f.write(blob); name = f.name
    try:
        out = subprocess.run(["objdump", "-D", "-b", "binary", "-mi386", "-Mintel", f"--adjust-vma={base}", name], capture_output=True, text=True).stdout
    finally:
        os.unlink(name)
    lines = []
    for l in out.splitlines():
        m = re.match(r"\s*([0-9a-f]+):\s+((?:[0-9a-f]{2} )+)\s*(.*)", l)
        if m:
            lines.append(f"{int(m.group(1),16)-base:5x}: {m.group(3)}")
    return lines


def show_diff(job, res_all, rva_text, led):
    rva = int(rva_text, 16)
    for res in res_all:
        for row in res["rows"]:
            if int(row["rva"], 16) == rva and row.get("symbol"):
                obj = job / "obj" / (res["tu"].replace("/", "_") + ".obj")
                b, rel = read_body(obj, row["symbol"])
                size = led[rva][0]
                body = b[:size] if len(b)>size and all(x==0xcc for x in b[size:]) else b
                ours = disasm(masked(body, rel, len(body)), 0)
                theirs = disasm(build.read_target_bytes(rva, size), 0)
                print(f"--- retail {hex(rva)} ({size} bytes)   +++ compiled {row['symbol']} ({len(body)} bytes); relocations zeroed in compiled")
                for l in difflib.unified_diff(theirs, ours, "retail", "compiled", lineterm="", n=2):
                    print(l)
                return
    print(f"{rva_text}: no compiled symbol paired with this function")


def judge_workers(task_count):
    try:count=int(os.environ.get('JUDGE_WORKERS','12'))
    except ValueError:raise ValueError('JUDGE_WORKERS must be a positive integer')
    if count<1:raise ValueError('JUDGE_WORKERS must be a positive integer')
    return min(count,max(1,task_count))

def cache_enabled(no_cache=False):
    return not no_cache and os.environ.get('JUDGE_CACHE','1').lower() not in ('0','off','false','no')

def cache_context(source,settings=None,walk=namespace):
    # WALK: namespace (every walk from scratch: the default and the end-of-run recheck) or, for the lookups of one run,
    # result_cache.remembered_namespace (one walk per root shared by the run's settings groups).
    flags,env,roots=compile_settings(source) if settings is None else settings
    tools=[Path(CL),VC/'Vc7/bin/c1.dll',VC/'Vc7/bin/c1xx.dll',VC/'Vc7/bin/c2.dll',VC/'Common7/IDE/mspdb71.dll',Path(WINE)]
    return {'checker':IMPORTED_CHECKER,'retail':tool_digest(RETAIL),
            'compiler':{str(p):tool_digest(p) for p in tools if p.is_file()},'base_flags':compiler_base(source),
            'source_flags':flags,
            'compile_paths':'relative-source-and-project-include-v1','compiler_contract':compilation_contract(flags,env,compiler_base(source)),
            'environment':{k:env.get(k,'') for k in ('INCLUDE','LIB','WINEPATH','WINEPREFIX','LANG','LC_ALL','WINEDLLOVERRIDES')},
            'external_roots':[str(p) for p in roots],'external_namespace':walk(roots)}

_WORK_JOB=None
_WORK_LED=None
_WORK_VIEW=None
_WORK_REQUESTS={}
_WORK_USE_CACHE=False
_WORK_IDENTITY={}

def keep_volatile_object(job, tu, result):
    """A result with no cache entry keeps its compiled object at PILOT/volatile_objects/<sha256>.obj (written once,
    atomically) and names it in result['volatile_object']: land.py's data rule reads compiled objects, and only cached
    results had one. Nothing else reads it; verdicts do not depend on it."""
    if result.get('cache_key'):
        return
    obj = Path(job) / 'obj' / (tu.replace('/', '_') + '.obj')
    try:
        data = obj.read_bytes()
    except OSError:
        return
    sha = hashlib.sha256(data).hexdigest(); store = PILOT / 'volatile_objects'; target = store / (sha + '.obj')
    try:
        if not target.exists():
            store.mkdir(parents=True, exist_ok=True)
            tmp = store / ('.%s.%d.part' % (sha, os.getpid())); tmp.write_bytes(data); os.replace(tmp, target)
        result['volatile_object'] = sha
    except OSError:
        pass


def judge_file(tu):
    with Permit() as permit:
        result=_judge_file(tu)
    result['host_permit_wait_s']=permit.wait_s
    return result

def _judge_file(tu):
    start=time.perf_counter();result=judge_tu(_WORK_JOB,tu,_WORK_LED)
    result['identity']=_WORK_IDENTITY
    result['cache_hit']=False
    if _WORK_USE_CACHE:
        if checker_hash()!=IMPORTED_CHECKER:raise RuntimeError('CHECKER_CHANGED: re-run judge')
        cache=ResultCache(PILOT/'result_cache')
        try:key,why=cache.store(_WORK_VIEW,_WORK_REQUESTS[tu],_WORK_JOB,tu,result)
        except (OSError,ValueError) as error:key,why=None,'cache storage bypassed: '+str(error)
        result.update(cache_key=key,cache_reason=why)
        keep_volatile_object(_WORK_JOB,tu,result)
    result['check_s']=round(time.perf_counter()-start,4)
    return result

class _NoPermit:
    wait_s=0
    def __enter__(self):return self
    def __exit__(self,*exc):return False

def setup_permit(needed):
    # Limit cold snapshot/metadata setup too. Release before fork: children take their own work permits.
    return Permit() if needed else _NoPermit()

def prepare_job(sha,job,needs_ledger):
    with setup_permit(needs_ledger) as permit:
        export(sha,job);loaded=ledger(job) if needs_ledger else None
    return loaded,permit.wait_s

def run_stats(results,misses,prepare_wait,lookup_s):
    return {'cache_hits':sum(bool(r.get('cache_hit')) for r in results),'cache_misses':len(misses),'prepare_permit_wait_s':prepare_wait,'host_slots':host_slots(),'host_permit_wait_s':round(sum(r.get('host_permit_wait_s',0) for r in results if not r.get('cache_hit')),4),'cache_lookup_s':round(lookup_s,4),'workers':judge_workers(len(misses)) if misses else 0}

def log_run(summary,results):
    LOG.parent.mkdir(exist_ok=True)
    with LOG.open('a') as log:log.write(json.dumps({'t':time.time(),**summary,'per_tu':[{'tu':r['tu'],'functions':r['functions'],'compile_s':r['compile_s'],'cache_hit':r['cache_hit']} for r in results]})+'\n')


def main(argv):
    global _WORK_JOB,_WORK_LED,_WORK_VIEW,_WORK_REQUESTS,_WORK_USE_CACHE,_WORK_IDENTITY
    diff=None;as_json=False;args=[];keep_job=False;no_cache=False;cache_only=False;all_files=False
    i=0
    while i<len(argv):
        if argv[i]=='--diff':diff=argv[i+1];i+=2;continue
        if argv[i]=='--keep-job':keep_job=True;i+=1;continue
        if argv[i]=='--json':as_json=True;i+=1;continue
        if argv[i]=='--cache-only':cache_only=True;i+=1;continue
        if argv[i]=='--no-cache':no_cache=True;i+=1;continue
        if argv[i]=='--all':all_files=True;i+=1;continue
        args.append(argv[i]);i+=1
    if not args:print(__doc__);return 2
    # An empty file list is a caller's bug (a branch already landed, a filter that matched nothing), never a request
    # for the whole tree: that takes --all, and --all with files is ambiguous.
    if (len(args)==1)!=all_files:print('judge.py: '+('no files given; pass --all to check every file' if len(args)==1 else '--all takes no file list'),file=sys.stderr);return 2
    if checker_hash()!=IMPORTED_CHECKER:raise RuntimeError('CHECKER_CHANGED: re-run judge')
    rev,tus=args[0],args[1:];start=time.perf_counter();view=GitInputs(BARE,rev)
    view.canonical(GitInputs(BARE,'main'))
    if not tus:tus=sorted(p for p in view.tree if p.startswith('src/') and Path(p).suffix.lower() in ('.cpp','.c'))
    use_cache=cache_enabled(no_cache) and diff is None
    cache=ResultCache(PILOT/'result_cache');found={};requests={};misses=[];reasons={}
    lookup_start=time.perf_counter();contexts={};context_sources={}
    for tu in tus:
        source=effective_source(tu,view.read(tu).decode('utf-8-sig',errors='replace'))
        try:
            if use_cache:
                settings=compile_settings(source);flags,environment,roots=settings
                group=json.dumps([flags,{k:environment.get(k,'') for k in ('INCLUDE','LIB','WINEPATH','WINEPREFIX','LANG','LC_ALL','WINEDLLOVERRIDES')},list(map(str,roots))],sort_keys=True)
                if group not in contexts:contexts[group]=cache_context(source,settings,walk=remembered_namespace);context_sources[group]=source
                request=view.request(tu,contexts[group])
            else:request=None
        except (OSError,ValueError):request=None
        requests[tu]=request
        hit,why=cache.lookup(view,request) if use_cache else (None,'result cache bypassed')
        if hit is not None and 'identity' in hit:found[tu]=hit
        else:misses.append(tu);reasons[tu]=why
    # Recheck each distinct compiler/include environment after the batch so a new shadowing file
    # cannot slip in between the first and last TU lookup. Actual external dependency bytes are checked too.
    # The lookups above reused one walk per external root (result_cache.remembered_namespace). Forget every walk first:
    # the environment check recomputes each group's context from scratch, walking each root once afresh after the last
    # lookup (groups share a root's new walk), and the repeated lookups below see the roots as they are now.
    forget_namespaces()
    for group,context in contexts.items():
        if cache_context(context_sources[group],walk=remembered_namespace)!=context:raise RuntimeError('CHECKER_CHANGED: compiler/include environment changed; retry judge')
    for tu in list(found):
        hit,why=cache.lookup(view,requests[tu])
        if hit is None:found.pop(tu);misses.append(tu);reasons[tu]=why
        else:found[tu]=hit
    lookup_s=time.perf_counter()-lookup_start
    if cache_only:
        if checker_hash()!=IMPORTED_CHECKER:raise RuntimeError('CHECKER_CHANGED: re-run judge')
        print(json.dumps({'summary':{'rev':rev,'sha':view.sha,'tus':len(tus),'cache_hits':len(found),'wall_s':round(time.perf_counter()-start,4)},'results':[found[t] for t in tus if t in found],'cache_misses':misses}));return 0
    remote_rc=remote_judge(argv,misses,cache_only,keep_job,diff)
    if remote_rc is not None:return remote_rc
    job=None;led=None;prepare_wait=0
    try:
        if misses or keep_job or diff:
            job=PILOT/'jobs'/uuid.uuid4().hex[:10];job.mkdir(parents=True)
            led,prepare_wait=prepare_job(view.sha,job,bool(misses or diff))
            if misses or diff:
                _WORK_IDENTITY={'map_snapshot':MAP['snapshot'],'symbols_sha256':digest_file(job/'config/symbols.json'),'checker_sha256':IMPORTED_FULL,'verdict_sha256':IMPORTED_CHECKER}
                _WORK_JOB=job;_WORK_LED=led;_WORK_VIEW=view;_WORK_REQUESTS=requests;_WORK_USE_CACHE=use_cache
                workers=judge_workers(len(misses))
                if len(misses)>1:
                    import multiprocessing
                    with concurrent.futures.ProcessPoolExecutor(max_workers=workers,mp_context=multiprocessing.get_context('fork')) as pool:
                        for result in pool.map(judge_file,misses):found[result['tu']]=result
                elif misses:
                    result=judge_file(misses[0]);found[result['tu']]=result
            if keep_job:
                for tu in tus:
                    if found[tu].get('cache_hit'):cache.materialize(found[tu],job,tu)
        results=[found[tu] for tu in tus]
        if checker_hash()!=IMPORTED_CHECKER:raise RuntimeError('CHECKER_CHANGED: re-run judge')
        if diff:show_diff(job,results,diff,led);return 0
        identity=results[0]['identity'] if results else _WORK_IDENTITY
        total={'MATCH':0,'MISNAMED':0,'PARTIAL':0,'MISSING':0,'COMPILE_ERROR':0,'UNKNOWN_RVA':0}
        for result in results:
            for row in result['rows']:
                if row.get('credited',False):total['MATCH']+=1
                elif row['status']!='MATCH':total[row['status']]=total.get(row['status'],0)+1
        summary={'rev':rev,'sha':view.sha,'tus':len(tus),'wall_s':round(time.perf_counter()-start,3),**total,
                 'FUNCTION_MATCH':sum(r['function_match'] for r in results),'FUNCTION_MATCH_PROVEN':sum(r.get('function_match_proven',0) for r in results),'FUNCTION_MATCH_UNPROVEN':sum(r.get('function_match_unproven',0) for r in results),'WRONG_REFERENCE':sum(r.get('wrong_reference',0) for r in results),'OBJECT_VERIFIED':sum(r['object_verified'] for r in results),
                 'gate_failures':sum(bool(r['gates']) for r in results),'expected_failures':sum(x['status']!='MATCH' for r in results for x in r['expected']),
                 'user':os.environ.get('PILOT_AGENT',''),'job_dir':str(job) if keep_job else None,**identity,
                 'checker_sha256':IMPORTED_FULL,'verdict_sha256':IMPORTED_CHECKER,**run_stats(results,misses,prepare_wait,lookup_s)}
        log_run(summary,results)
        if as_json:print(json.dumps({'summary':summary,'results':results}));return 0
        for result in results:
            print('== %s: Function MATCH %d/%d (references proven %d, unproven %d; %d equal bytes with a wrong reference); Object VERIFIED %s%s'%(result['tu'],result['function_match'],result['functions'],result.get('function_match_proven',0),result.get('function_match_unproven',0),result.get('wrong_reference',0),result['object_verified'],' [cache]' if result.get('cache_hit') else ''))
            for why in result['verification_blockers']:print('   NOT VERIFIED: '+why)
            for extra in result['accepted_extras']:print('   '+extra['classification']+': '+extra['symbol'])
        print('SUMMARY '+json.dumps(summary));return 0
    finally:
        if job is not None and not keep_job:shutil.rmtree(job,ignore_errors=True)


if __name__ == "__main__":
    try:sys.exit(main(sys.argv[1:]))
    except RuntimeError as error:
        if 'CHECKER_CHANGED' in str(error):print(str(error),file=sys.stderr);sys.exit(75)
        raise
