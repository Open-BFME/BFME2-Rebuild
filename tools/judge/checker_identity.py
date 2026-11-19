"""Checker identities shared by the judge, the result cache and the landing state cache; no compiler initialization.

verdict_digest: every input that can change a verdict, or which cached verdict is served. The result-cache key and
  main's recorded landing state use it, so a change here empties the cache.
  Excluded, because they decide only when, where and how fast a check runs, or what is printed: host_permits.py,
  and the top-level functions of bin/judge.py named in OPERATIONAL_FUNCTIONS (source lines cut by line range, so the
  digest does not depend on the Python version). Verdict code may not call them (test_identity_split.py), and this
  file itself is part of the digest, so editing the list empties the cache.
checker_digest: every byte of every checker file; for logs, receipts and audit.
"""
import ast,hashlib
from pathlib import Path
from checker_configuration import RUNNER_MODULES
OWN=['bin/judge.py','binary_model.py','relocation_proof.py','emission_policy.py','identity_index.py','result_cache.py','checker_identity.py','compiler_support.py','host_permits.py','upstream_proof.py','asm_policy.py','asm_allowlist.py','import_aliases.py']
REFERENCE=['build.py','cl_flags.py','stlport_overlay.py']
OPERATIONAL_FILES=('host_permits.py',)
OPERATIONAL_FUNCTIONS={'bin/judge.py':('remote_judge','judge_workers','cache_enabled','judge_file','setup_permit','run_stats','log_run','disasm','show_diff')}

def checker_digest(root):
 root=Path(root);h=hashlib.sha256()
 for name in OWN:
  path=root/name
  if path.exists():h.update(name.encode()+b'\0');h.update(path.read_bytes())
 for name in REFERENCE:
  path=RUNNER_MODULES/name
  if path.exists():h.update(('reference/'+name).encode()+b'\0');h.update(path.read_bytes())
 return h.hexdigest()

def verdict_source(name,payload):
 """PAYLOAD without its operational top-level functions. Anything unexpected keeps the whole file (conservative)."""
 names=OPERATIONAL_FUNCTIONS.get(name)
 if not names:return payload
 try:tree=ast.parse(payload)
 except (SyntaxError,ValueError):return payload
 drop=set();found=[]
 for node in tree.body:
  if isinstance(node,(ast.FunctionDef,ast.AsyncFunctionDef,ast.ClassDef)) and node.name in names:
   if node.name in found or not isinstance(node,(ast.FunctionDef,ast.AsyncFunctionDef)):return payload
   found.append(node.name);start=min([node.lineno]+[d.lineno for d in node.decorator_list])
   drop.update(range(start,node.end_lineno+1))
 lines=payload.split(b'\n')
 kept=[line for number,line in enumerate(lines,1) if number not in drop]
 return b'\n'.join(kept)+b'\0operational-cut:'+','.join(found).encode()

def verdict_digest(root):
 root=Path(root);h=hashlib.sha256(b'verdict-identity-v1\0')
 for name in OWN:
  if name in OPERATIONAL_FILES:continue
  path=root/name
  if path.exists():h.update(name.encode()+b'\0');h.update(verdict_source(name,path.read_bytes()))
 for name in REFERENCE:
  path=RUNNER_MODULES/name
  if path.exists():h.update(('reference/'+name).encode()+b'\0');h.update(path.read_bytes())
 return h.hexdigest()
