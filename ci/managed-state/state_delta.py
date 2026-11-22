"""Carry an exact main-line ancestor across fully tracked input changes."""

from hashlib import sha256
from subprocess import CalledProcessError

from state_snapshot import OID, authoritative_units, git, load, safe_tu
from state_evidence import LIMIT, cached_pairs, header

# These are orchestration and publication paths. The comparison inputs and
# implementation retain full-refresh semantics. state.yml can be carried only
# across the reviewed producer-v2 to resume transport revision.
REPORT_DIRS = ("docs/", "worklist/", "ci/state/", "ci/progress/", "ci/alarms/",
               "ci/nightly/", "ci/landing/", "ci/admission/", "ci/header/", "ci/public-text/",
               "ci/managed-check/tests/", ".github/workflows/")
STATE_FLOW = ".github/workflows/state.yml"
TRANSPORT = {"ci/managed-state/state_delta.py", "ci/managed-state/state_fetch.py",
             "ci/managed-state/state_workflow.py",
             "ci/managed-state/recovery-artifacts.json", "ci/managed-state/state_recover.py",
             "ci/managed-state/state_transport.py", "ci/managed-state/state_origin.py",
             "ci/managed-state/bundle.sha256",
             "ci/README.md"}
RESUME_MIGRATION = ("5df8abc2a97aef390b5a0256a15e163ae4a312566c0ea893187da4a254aea8fc", "14286c7910816519777d5a46c720605d142dff009f5751843d4e4c5263e2111f")
HYDRATION_MIGRATION = ("c5495ba4c03861220f77ddc48cd6533617fde93b869d51a0accf479fcc760bcb", "ec1ec3a1489c459a4513cfef8937101998a740667c0662d15e4df00f102bce61")
STATE_FLOW_MIGRATIONS = {RESUME_MIGRATION, HYDRATION_MIGRATION}
# The one-shot PR orchestration change leaves every comparison input intact.
COMPACT_GRAPH = {
    "ci/managed-check/landing_read.py": (
        "fcbc657e031cff9f496470c5ad3fa7d01080357d3f277381584c48d35a3ce89f",
        "c156b8a77e440fa8f87d017389d8aea9e164e236704eba2baf26d190821b1cc1",
    ),
    "ci/managed-check/github_io.py": (
        "3a5c9373f5bb105a71b4f00a730a5361ac7b238372ccf7e7baadb51e17c6b744",
        "af57df171f74d66f557535dfe2b702c194bd51219828060b60dca6ff210c5f77",
    ),
    "ci/managed-check/bundle.sha256": (
        "8e8f38f80f0327372967ae319b28261bca972d0435dd21465875b03806fd9f12",
        "21c6f500a947b2b957366fa346de2864cb3d017901e0e528bfd50220589abf82",
    ),
}
AUTOMATIC_GRAPH = {
    "ci/managed-check/wide_recovery.py": (
        '43ab19a7fd87bbf9e00c7fecdaba498b1c0da45e749a692ae383db7e4247af7d',
        'af69c85ba0829c103afbcfe68d8b1788ee23ea34af0d87bb3f503f6734e4e433',
    ),
    "ci/managed-check/bundle.sha256": (
        '21c6f500a947b2b957366fa346de2864cb3d017901e0e528bfd50220589abf82',
        'ef8d015ae4a4c3b169b9572299f4171b68e4661b22ce63bf2bc121accb2fb336',
    ),
}
# A retained checkpoint can precede either migration, or both of them.
BUNDLE = "ci/managed-check/bundle.sha256"
COMBINED_GRAPH = COMPACT_GRAPH | AUTOMATIC_GRAPH | {
    path: (COMPACT_GRAPH[path][0], AUTOMATIC_GRAPH[path][1])
    for path in COMPACT_GRAPH.keys() & AUTOMATIC_GRAPH.keys()
}
TRANSPORT_LOG_GRAPH = {
    "ci/managed-check/github_io.py": (
        "af57df171f74d66f557535dfe2b702c194bd51219828060b60dca6ff210c5f77",
        "c6c89dbd8ec2923e8d954d09d79b23fd344e1d2be9b0c90f332f9abe16132228",
    ),
    "ci/managed-check/bundle.sha256": (
        "ef8d015ae4a4c3b169b9572299f4171b68e4661b22ce63bf2bc121accb2fb336",
        "9f3ebd824a6efbef574e88ec7a738df50c9f0985056af1055ef6804fe6014112",
    ),
}


def advance_graph(earlier, later):
    overlap = earlier.keys() & later.keys()
    if any(earlier[path][1] != later[path][0] for path in overlap):
        raise ValueError("Orchestration graph preimages do not form a continuous chain.")
    return earlier | later | {path: (earlier[path][0], later[path][1]) for path in overlap}


RESERVE_AND_LOG_GRAPH = advance_graph(AUTOMATIC_GRAPH, TRANSPORT_LOG_GRAPH)
COMBINED_LOG_GRAPH = advance_graph(COMBINED_GRAPH, TRANSPORT_LOG_GRAPH)
GRAPH_CHANGES = (COMPACT_GRAPH, AUTOMATIC_GRAPH, COMBINED_GRAPH,
                 TRANSPORT_LOG_GRAPH, RESERVE_AND_LOG_GRAPH, COMBINED_LOG_GRAPH)
INLINE_BINDING_GRAPH = {
    "ci/managed-check/inline_virtual/inline_virtual_receipt.py": (
        "7be4b2a0b897049c5cdbac32484d50ed794d34a112ff8f08c44e292483ca82ce",
        "5b4f047643182c119cc83ba467453a5c007dbe8173a0d792b95dcc6aa2163fc1",
    ),
    "ci/managed-check/virtual_atoms.py": (
        "e30a1ca021e2685efafbc0a0643b62e51ba85efd905526d06ab14c89ee19b9d0",
        "f5924475fecdda9d79eec784041ebd6e509d7f75b21a7ed087b8c9d01bd3aeaa",
    ),
    "ci/managed-check/bundle.sha256": (
        "9f3ebd824a6efbef574e88ec7a738df50c9f0985056af1055ef6804fe6014112",
        "d29b124bdb33956c3af86aed8f8ca50f36a1877acf4a00d291ba44eb852ad6a4",
    ),
}

GRAPH_CHANGES += (INLINE_BINDING_GRAPH,) + tuple(
    advance_graph(previous, INLINE_BINDING_GRAPH)
    for previous in (TRANSPORT_LOG_GRAPH, RESERVE_AND_LOG_GRAPH, COMBINED_LOG_GRAPH)
)
GRAPH_PATHS = set(COMBINED_LOG_GRAPH) | set(INLINE_BINDING_GRAPH)
ADMISSION_GRAPH = {
    "ci/managed-check/github_io.py": (
        'c6c89dbd8ec2923e8d954d09d79b23fd344e1d2be9b0c90f332f9abe16132228',
        '4a413fed5760e8e14e12c779961939d24111991ad7c888a3244a21810850edfa',
    ),
    "ci/managed-check/intake.py": (
        '3391fb495dccd4cab7bf9610d337c25696e9f56a27985faad5fab26def666b18',
        '4b8fa6c3272a24ed002fc6c16af5dccc0e4416fe17259a73a08f845a48028d04',
    ),
    "ci/managed-check/fork_admission.py": (
        None,
        '9d5d811357515f81e46ee729a1ced768846042f34711d1817712fcded00f45d7',
    ),
    "ci/managed-check/bundle.sha256": (
        'd29b124bdb33956c3af86aed8f8ca50f36a1877acf4a00d291ba44eb852ad6a4',
        'ca92c4cf6cd894d669c2c9876fee5f383f94d93daae987fe9ab7b45b16f7e51b',
    ),
}

GRAPH_CHANGES += (ADMISSION_GRAPH,) + tuple(
    advance_graph(previous, ADMISSION_GRAPH)
    for previous in GRAPH_CHANGES
    if previous[BUNDLE][1] == ADMISSION_GRAPH[BUNDLE][0]
)
GRAPH_PATHS |= set(ADMISSION_GRAPH)

WIDE_CLOCK_GRAPH = {
    "ci/managed-check/compact_matrix.py": (
        "9e5e479a33cfe39bd2cb48d565c00dde51c5ad1adcdae03a50c7f0285683e266",
        "5795922e9a9a034cb2d02e7e686ad3c7efb4a5ac018c2944a2807a47d913afe2",
    ),
    "ci/managed-check/compare.py": (
        "7a3d3976b32c89fa4f656c5a25d882a8172cc232ade04743f99fb84fa6b7e82b",
        "7096d164997ca3ab224a0edc77003898190af33431cdc0640de1e26b9a5072cf",
    ),
    "ci/managed-check/managed_matrix.py": (
        "313a21b37f4ac441aef9cb091893db268a07f709468b0b8f375710f311b3451c",
        "8d80ff76da20ee6d29a9e38502f713532f18c3f42622caed58e885f35201cba1",
    ),
    "ci/managed-check/recovery_artifacts.py": (
        "f81d84ed474e743cc0fdfeec982b919649b2d3d8b6f89c5275fbc896a71d71c9",
        "9f7c44bbf667ff5e64b183bf0d8ca44e5c0077b3131105751257faf10224c924",
    ),
    "ci/managed-check/recovery_broker.py": (
        "4fef8dd2172ea8d1c8f0415bedb9fdd31d2a70356a07f6e1e3ed9cee53ec7639",
        "dd6071bc1fe9e872f239e50e92b97358b349bbe8170e4d896729c805e42be55f",
    ),
    "ci/managed-check/wide.py": (
        "6767f2bb9500c5d3c96251e66bc6c88a8910ac78f71cc3d499e5d2243847ff04",
        "5bd0c4a37ca8fc131f8737fc3ce67047466cb3ebb3e7eb35c9705bf1173370f4",
    ),
    "ci/managed-check/wide_recovery.py": (
        "af69c85ba0829c103afbcfe68d8b1788ee23ea34af0d87bb3f503f6734e4e433",
        "57b74a0ebfa703aaae7c7fa47fcd22610058059cb5e27524b450a7e6c9d981ed",
    ),
    "ci/managed-check/bundle.sha256": (
        "efb05a79f87b5cd21373cbfcf854ecae90cc53fe5d3c381b1408fbc3ef4fa24e",
        "7e52d1b99613f8940f426bcf06fd16e3f4c60aa31c4e3757a9a82aeea99f2cf4",
    ),
}

GRAPH_CHANGES += (WIDE_CLOCK_GRAPH,) + tuple(
    advance_graph(previous, WIDE_CLOCK_GRAPH)
    for previous in GRAPH_CHANGES
    if previous[BUNDLE][1] == WIDE_CLOCK_GRAPH[BUNDLE][0]
)
GRAPH_PATHS |= set(WIDE_CLOCK_GRAPH)

WIDE_ADMISSION_GRAPH = {
    "ci/managed-check/fork_admission.py": (
        "9d5d811357515f81e46ee729a1ced768846042f34711d1817712fcded00f45d7",
        "25badb095dabc6c8a717145f3ea1e14342fbb5982fb10982608e9df7b8e01e34",
    ),
    "ci/managed-check/github_io.py": (
        "9f761ce5ada2907486f1665b98b68650ecc6b764c9685d2d6f168f0aebed3b66",
        "c34a1c9bde3d83f39346bdb8246ee74290cebd5058672d9e24b13388bb02c9e8",
    ),
    "ci/managed-check/intake.py": (
        "509d2ef40980f7c83a5be4f9c096046eb881dda19f4ea2273677e2a07297a493",
        "5c0dc9af03ab8dc962b62aa2ecb5a1b64218c989b387e8bd178aebc352e78bce",
    ),
    "ci/managed-check/recovery_broker.py": (
        "dd6071bc1fe9e872f239e50e92b97358b349bbe8170e4d896729c805e42be55f",
        "22a4cce5519019c86534849930a0f5704a5b7ee9f0b00d4eb06ede8f8869b51d",
    ),
    "ci/managed-check/bundle.sha256": (
        "7e52d1b99613f8940f426bcf06fd16e3f4c60aa31c4e3757a9a82aeea99f2cf4",
        "d8ea452fadaace93fd7785d045aac330da534c80e9402ef2b26d20f7c9d55a24",
    ),
}

GRAPH_CHANGES += (WIDE_ADMISSION_GRAPH,) + tuple(
    advance_graph(previous, WIDE_ADMISSION_GRAPH)
    for previous in GRAPH_CHANGES
    if previous[BUNDLE][1] == WIDE_ADMISSION_GRAPH[BUNDLE][0]
)
GRAPH_PATHS |= set(WIDE_ADMISSION_GRAPH)

BASE_CACHE_GRAPH = {
    "ci/managed-check/base_cache.py": (
        None,
        "dd852ef62365c5a1333f1d818ad84d539e19f4c41180be9ad97bdadcc0798970",
    ),
    "ci/managed-check/compare.py": (
        "7096d164997ca3ab224a0edc77003898190af33431cdc0640de1e26b9a5072cf",
        "33bcf90abbea57b28d61527a4af73d03c6ab2b0d3b16200578ad5289253c98e5",
    ),
    "ci/managed-check/bundle.sha256": (
        "d8ea452fadaace93fd7785d045aac330da534c80e9402ef2b26d20f7c9d55a24",
        "12b2491e2c66606e248986ec9b7a47670c4cf84f6f38db1b7e0c1e846a562bf5",
    ),
}

GRAPH_CHANGES += (BASE_CACHE_GRAPH,) + tuple(
    advance_graph(previous, BASE_CACHE_GRAPH)
    for previous in GRAPH_CHANGES
    if previous[BUNDLE][1] == BASE_CACHE_GRAPH[BUNDLE][0]
)
GRAPH_PATHS |= set(BASE_CACHE_GRAPH)

SOURCE_RECHECK_GRAPH = {
    "tools/gates/workflow.py": (
        "62d349176331acdab620a6c6154d9fab6495ee2491816cfdd87a996b0f77b826",
        "d31fc88e23c82e3c263af49d99a4f8f66d66e982e5396794c6947cdb7f085a72",
    ),
}
GRAPH_CHANGES += (SOURCE_RECHECK_GRAPH,)
GRAPH_PATHS |= set(SOURCE_RECHECK_GRAPH)

EMITTED_MSG_GRAPH = {
    "ci/managed-check/compare.py": (
        "0bc96c5aee42ed234c3600a9f726aba86c74763675c073d31161c1fa5adae969",
        "753b59e77e5338a51d6d3ee9897efaafa2e9e1633c5f3cfa913248a89f0bfb68",
    ),
    "ci/managed-check/bundle.sha256": (
        "1c7998cc3788b9efd66c714e7aff1fea6370bd698400fbda4629bdf209eba5b1",
        "a297c06e70fb86926416a15d5af6ac3b8ca2f31d3e3031614bd8c53c66b01195",
    ),
}
GRAPH_CHANGES = (SOURCE_RECHECK_GRAPH | EMITTED_MSG_GRAPH,) + GRAPH_CHANGES + (EMITTED_MSG_GRAPH,) + tuple(
    advance_graph(previous, EMITTED_MSG_GRAPH)
    for previous in GRAPH_CHANGES
    if BUNDLE in previous and previous[BUNDLE][1] == EMITTED_MSG_GRAPH[BUNDLE][0]
)
GRAPH_PATHS |= set(EMITTED_MSG_GRAPH)

EMITTED_ROWS_FIX_GRAPH = {
    "ci/managed-check/compare.py": (
        "753b59e77e5338a51d6d3ee9897efaafa2e9e1633c5f3cfa913248a89f0bfb68",
        "9b5ca88a540ed2a555944542dd16f9a4d3e3933e5a3a058a82f20362f5a79537",
    ),
    "ci/managed-check/wide.py": (
        "3f764d3df855f1d8aeed8bc05728667c3d376e9c2bd1a527cea196be434efefe",
        "618d17c6c5e8f83c64bf1c678a51f351f20a87087bbe2015c7ce0327cf6f3822",
    ),
    "ci/managed-check/bundle.sha256": (
        "a297c06e70fb86926416a15d5af6ac3b8ca2f31d3e3031614bd8c53c66b01195",
        "35c124efeef151a48c0cacc3a55125eedd52379ce0a1de23a76b76afd2ba59ee",
    ),
}
GRAPH_CHANGES += (EMITTED_ROWS_FIX_GRAPH,) + tuple(
    advance_graph(previous, EMITTED_ROWS_FIX_GRAPH)
    for previous in GRAPH_CHANGES
    if BUNDLE in previous and previous[BUNDLE][1] == EMITTED_ROWS_FIX_GRAPH[BUNDLE][0]
)
GRAPH_PATHS |= set(EMITTED_ROWS_FIX_GRAPH)

CUT12_GATES_AND_H2_GRAPH = {
    "tools/gates/config.json": (
        "2d0361d9efc1717ca387fb65359c60dc324b91be88d1ccafa347a172a9758b6d",
        "286aead9e860221789552e00f620dd45e7a70b2e033c95b6799b171edac6cd44",
    ),
    "tools/gates/rules.py": (
        "5eaf63471cbf7b08c03fdd72cc2fdb3f9e207a0a3b7790ccb966b61987994fc6",
        "abf702cf9409e67a51073cdb7bbf1851df6341b09868a9f4cc6ca7e1ed09022e",
    ),
    "tools/gates/test_gate.py": (
        "623e6723788683a7c37542b400ce5c70eae81effaca10fd9034d3a883fc49b47",
        "3bd32d9e59a6d51fbc43648bd93d360f9f47b407e2852ecdda25e407f4aee0f5",
    ),
    "tools/gates/test_public_delivery.py": (
        "5720e8ac40387613c01965da32ef4dbbb8dae731a2cf3fdbbe0e694ae41c1ded",
        "073710b7ff29eae5edeee8a3e7d67169387e44eadf06e1022443794c887e6e09",
    ),
    "ci/managed-check/header_producer.py": (
        "19106de010f61020ea4fcccf5db57a36969db037990deffd03881bc893e5b0c8",
        "11c41c57fb92ea42582545480e7da84f6de1b41f8642f0069c5abc97c1c5a9bc",
    ),
    "ci/managed-check/header_probe_plan.py": (
        "11e751e1a3b80aae3cd7909867357adc297628988b2b1e60065a3424ca928fe9",
        "53441502723614db269838f0bb5e203ea7f1d5409cf2553d5d48b67493eb0aa1",
    ),
    "ci/managed-check/bundle.sha256": (
        "35c124efeef151a48c0cacc3a55125eedd52379ce0a1de23a76b76afd2ba59ee",
        "80a8de235d4824307e47fd693296ff4097947e2133c48f72ec2730ab7acaf8b0",
    ),
}
GRAPH_CHANGES += (CUT12_GATES_AND_H2_GRAPH,) + tuple(
    advance_graph(previous, CUT12_GATES_AND_H2_GRAPH)
    for previous in GRAPH_CHANGES
    if BUNDLE in previous and previous[BUNDLE][1] == CUT12_GATES_AND_H2_GRAPH[BUNDLE][0]
)
GRAPH_PATHS |= set(CUT12_GATES_AND_H2_GRAPH)

CUT13_HEADER_TRUSTED_GRAPH = {
    "tools/gates/header_trusted.py": (
        None,
        "0320445ce074e7919add15245e5d45fcd4996137ed6af2bd930814bf0efe0b00",
    ),
    "tools/gates/header_source_preview.py": (
        "a09a7298441359f4f10303a2e8f012d0eed5f4bbde7f4a1623893f54c8d33ee3",
        "8100ed15bbb2594d884e89a905f47387d07ced2520616f15449ae440bff17ab1",
    ),
    "tools/gates/test_header_trusted.py": (
        None,
        "b014ebf68d8f463ad8cd134c178021dbebf59b8834a8e86f22bf126b2a2b5b44",
    ),
    "ci/managed-check/header_trusted_action.py": (
        None,
        "f5a167c0cce414c80e10e6cf3188d30028298b15ff805c2592ad42117d926565",
    ),
    "ci/managed-check/bundle.sha256": (
        "80a8de235d4824307e47fd693296ff4097947e2133c48f72ec2730ab7acaf8b0",
        "af8387224c9d53242728b6e4b70aa1ca97567670c00b6a8a3a2527cd1a28f49a",
    ),
}
GRAPH_CHANGES += (CUT13_HEADER_TRUSTED_GRAPH,) + tuple(
    advance_graph(previous, CUT13_HEADER_TRUSTED_GRAPH)
    for previous in GRAPH_CHANGES
    if BUNDLE in previous and previous[BUNDLE][1] == CUT13_HEADER_TRUSTED_GRAPH[BUNDLE][0]
)
GRAPH_PATHS |= set(CUT13_HEADER_TRUSTED_GRAPH)

COMPACT_SCOPE = {"ci/managed-state/state_delta.py", "ci/managed-state/state_origin.py",
                 "ci/managed-state/bundle.sha256"}
MAX_DIFF_BYTES = 4 * 1024**2
ANCESTOR_LIMIT = 128


def predecessors(root, main):
    return git(root, "rev-list", "--first-parent", "--skip=1", "--max-count",
               str(ANCESTOR_LIMIT), main).decode("ascii").splitlines()


def graph_blob(root, revision, path):
    entry = git(root, "ls-tree", revision, "--", path)
    if not entry:
        return None
    if not entry.startswith(b"100644 blob "):
        raise ValueError("A graph control no longer has its approved file mode.")
    return sha256(git(root, "show", f"{revision}:{path}")).hexdigest()


def graph_change(root, parent, main, paths=None):
    best = None
    try:
        for transition in GRAPH_CHANGES:
            if paths is not None and set(transition) != paths:
                continue
            if all(tuple(graph_blob(root, rev, path) for rev in (parent, main)) == expected
                   for path, expected in transition.items()):
                if best is None or len(transition) > len(best):
                    best = transition
    except (OSError, ValueError, CalledProcessError):
        return None
    return best


def publication_path(root, parent, main, file):
    if file in GRAPH_PATHS:
        transition = graph_change(root, parent, main)
        return transition is not None and file in transition
    if file == STATE_FLOW:
        fingerprints = tuple(sha256(git(root, "show", f"{rev}:{file}")).hexdigest()
                             for rev in (parent, main))
        return fingerprints in STATE_FLOW_MIGRATIONS
    return (file in TRANSPORT or file.startswith(REPORT_DIRS)
            or ("/" not in file and file.endswith(".md")))


def source_changes(root, parent, main):
    """Return the dirty source paths, or None if the transition needs a full rebuild."""
    data = git(root, "diff-tree", "-r", "--raw", "-z", "--no-commit-id", "--no-renames",
               "--no-ext-diff", parent, main, "--")
    if len(data) > MAX_DIFF_BYTES:
        return None
    parts = data.split(b"\0")
    if parts.pop() != b"" or len(parts) % 2:
        return None
    changed = set()
    for meta, name in zip(parts[::2], parts[1::2], strict=True):
        file = name.decode("utf-8", "strict")
        info = meta.split()
        if (len(info) == 5 and info[0] in {b":100644", b":000000"}
                and info[1] in {b"100644", b"000000"} and info[4] in {b"A", b"D", b"M"}
                and publication_path(root, parent, main, file)):
            continue
        # New/deleted paths, configuration, ledgers, and name or mode changes
        # may alter symbolic lookup or introduce an earlier include candidate.
        if (not file.startswith(("src/", "include/")) or len(info) != 5
                or info[:2] != [b":100644", b"100644"] or info[4] != b"M"):
            return None
        changed.add(file.casefold())
    return changed


def unaffected(evidence, changed):
    clean = {}
    for tu, entry in evidence.items():
        row = entry["compiler"]
        if tu.casefold() in changed or (changed and row["status"] != "compiled"):
            continue
        # Only installed and authenticated receipts can supply these dependencies.
        if any(path.casefold() in changed for path, _ in row.get("dependencies", ())):
            continue
        clean[tu] = entry
    return clean


def inherited(root, main, snapshot, *, warn=print):
    if not snapshot or not snapshot.is_file():
        return {}
    try:
        old = load(snapshot, LIMIT)
        parent = old.get("sha", "")
        if not isinstance(parent, str) or not OID.fullmatch(parent) or parent not in predecessors(root, main):
            return {}
        if (changed := source_changes(root, parent, main)) is None:
            return {}
        tree = git(root, "rev-parse", parent + "^{tree}").decode().strip()
        units = authoritative_units(root, main)
        if set(old["evidence"]) != set(units):
            return {}
        keys = ("source", "contract")
        requests = [{"tu": name, **{key: old["evidence"][name]["compiler"][key] for key in keys}}
                    for name in units]
        if not all(safe_tu(item["tu"]) for item in requests):
            return {}
        identity = header(parent, tree)
        transition = graph_change(root, parent, main)
        bundle_pair = transition.get(BUNDLE) if transition else None
        if bundle_pair and identity["policy"] == bundle_pair[1]:
            identity["policy"] = bundle_pair[0]
        return unaffected(cached_pairs(old, identity, requests), changed)
    except (OSError, ValueError, TypeError, KeyError, AttributeError, CalledProcessError):
        warn("Previous exact state cannot be reused. A full initial refresh is required.")
        return {}


def merge(required, measured, carried):
    if set(measured) & set(carried) or set(required) != set(measured) | set(carried):
        raise ValueError("The new protected state does not cover each translation unit exactly once.")
    return measured | carried

