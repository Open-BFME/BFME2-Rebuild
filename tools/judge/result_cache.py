"""Dependency-keyed verifier results. Git blobs bind committed inputs; SHA256 binds read content.
An index key only finds candidates. Every actual dependency and include namespace is checked before use.
The cache contains compiler artifacts and decisions under a specific checker/toolchain/retail identity.
Schema 2: the repo's file list no longer keys a request. Each entry records the include-name scope that can change
the compiler's include resolution (the folded basenames of every file the compile read, plus every symlink and
submodule) and is valid only while the tree's entries under those names are unchanged. A compile error, a computed
#include or an 8.3 (~) spelling keeps the whole file list as its scope.
"""
import os,re,json,hashlib,subprocess,fcntl,tempfile,shutil,time
from pathlib import Path
SCHEMA=2
VOLATILE=re.compile(rb'\b__(?:DATE|TIME|TIMESTAMP)__\b|^\s*#\s*import\b',re.M)
H=lambda value:hashlib.sha256(value).hexdigest()
def encoded(value):return json.dumps(value,sort_keys=True,separators=(',',':')).encode()
def digest_file(path):
 with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def blob_id(payload):return hashlib.sha1(b'blob '+str(len(payload)).encode()+b'\0'+payload).hexdigest()
def atomic_json(path,value):
 path.parent.mkdir(parents=True,exist_ok=True)
 fd,name=tempfile.mkstemp(prefix=path.name+'.',dir=path.parent)
 try:
  with os.fdopen(fd,'wb') as stream:stream.write(encoded(value))
  os.replace(name,path)
 finally:
  if os.path.exists(name):os.unlink(name)

NAMESPACE_BOUND=50000
def _walk(root,budget):
 """One root's part of the include-resolution graph and the number of names in it (ValueError past BUDGET)."""
 root=Path(root);items=[];visited=set();count=0
 if root.is_dir():
  for base,dirs,files in os.walk(root,followlinks=True):
   stat=os.stat(base);identity=(stat.st_dev,stat.st_ino)
   if identity in visited:dirs[:]=[];continue
   visited.add(identity);dirs.sort();files.sort()
   for name in sorted(dirs+files):
    p=Path(base)/name;rel=p.relative_to(root).as_posix();count+=1
    if count>budget:raise ValueError('include namespace exceeds cache bound')
    if p.is_symlink():items.append((rel,'link',os.readlink(p)))
    elif p.is_dir():items.append((rel,'dir',p.stat().st_mode&0o777))
    else:items.append((rel,'file',p.stat().st_mode&0o777))
 return (str(root),str(root.resolve()),'dir' if root.is_dir() else 'missing',items),count

def _combine(parts):
 if sum(n for _e,n in parts)>NAMESPACE_BOUND:raise ValueError('include namespace exceeds cache bound')
 return H(encoded([e for e,_n in parts]))

def namespace(roots):
 """Include-resolution graph: names, permissions and symlink targets, following directory links once. Always walks."""
 parts=[];left=NAMESPACE_BOUND
 for root in roots:
  entry,n=_walk(root,left);parts.append((entry,n));left-=n
 return _combine(parts)

# Per-process memo of each root's walk. A run's compiler/include contexts (one per settings
# group) and cache lookups share one walk per root instead of one per group. judge.py calls forget_namespaces() after the
# run's last lookup, so its end-of-run recheck walks every root afresh (once, shared by the groups) and recomputes every
# environment a hit rests on from scratch. Stores always walk. remembered_namespace(roots) == namespace(roots) whenever
# no root changed since its walk.
_NAMESPACE_MEMO={}
def remembered_namespace(roots):
 parts=[]
 for root in roots:
  key=str(root)
  if key not in _NAMESPACE_MEMO:_NAMESPACE_MEMO[key]=_walk(root,NAMESPACE_BOUND)
  parts.append(_NAMESPACE_MEMO[key])
 return _combine(parts)
def forget_namespaces():_NAMESPACE_MEMO.clear()

def fold_name(name):
 # Wine resolves names case-insensitively and Windows drops trailing dots and spaces.
 return name.casefold().rstrip(' .')

INCLUDE_LINE=re.compile(rb'^[ \t]*#[ \t]*include\b[ \t]*(.*)$',re.M)
def literal_includes_only(contents):
 """True when every #include in the compiler inputs names a literal path without an 8.3 short-name spelling."""
 for text in contents:
  for before,after in [(b'??=',b'#'),(b'%:',b'#')]:text=text.replace(before,after)
  text=re.sub(rb'\\\r?\n',b'',text)
  for operand in INCLUDE_LINE.findall(text):
   operand=operand.strip()
   match=re.match(rb'"([^"]*)"|<([^>]*)>',operand)
   if not match:return False
   if b'~' in (match.group(1) if match.group(1) is not None else match.group(2)):return False
 return True

def volatile_reason(contents):
 """Fail closed on implicit clocks, including line splicing and token-pasted builtin names."""
 text=b'\n'.join(contents)
 if b'\0' in text:return 'unsupported NUL/UTF16 compiler input'
 for before,after in [(b'??=',b'#'),(b'??/',b'\\'),(b'%:',b'#')]:text=text.replace(before,after)
 text=re.sub(rb'\\\r?\n',b'',text)
 def universal(match):
  value=int(match[1],16)
  return bytes([value]) if value<128 else match[0]
 text=re.sub(rb'\\u([0-9A-Fa-f]{4})',universal,text)
 if VOLATILE.search(text):return 'volatile builtin or #import in compiler inputs'
 if b'##' in text:
  tokens=set(re.findall(rb'[A-Za-z_][A-Za-z_0-9]*',text))
  for word in (b'__DATE__',b'__TIME__',b'__TIMESTAMP__'):
   reachable={0}
   for start in range(len(word)):
    if start not in reachable:continue
    for end in range(start+1,len(word)+1):
     if word[start:end] in tokens:reachable.add(end)
   if len(word) in reachable:return 'token paste can construct an implicit clock builtin'
 return None

class GitInputs:
 def __init__(self,bare,revision):
  self.bare=str(bare);self.sha=subprocess.check_output(['git','--git-dir',self.bare,'rev-parse',revision],text=True).strip()
  raw=subprocess.check_output(['git','--git-dir',self.bare,'ls-tree','-rz','--full-tree',self.sha]);self.tree={}
  for item in raw.split(b'\0'):
   if not item:continue
   meta,path=item.split(b'\t',1);mode,kind,oid=meta.decode().split();self.tree[path.decode('utf-8','surrogateescape')]=(mode,oid)
  names=[(p,m,oid if m=='120000' else '') for p,(m,oid) in sorted(self.tree.items())]
  self.namespace=H(encoded(names));self._bytes={};self._by_name=None
 def scoped_namespace(self,basenames):
  """Digest of the tree entries that could change resolution of includes with these folded basenames."""
  if basenames is None:return self.namespace
  if self._by_name is None:
   index={}
   for p,(m,oid) in self.tree.items():
    key='\0link' if m in ('120000','160000') else fold_name(p.rsplit('/',1)[-1])
    index.setdefault(key,[]).append((p,m,oid if m in ('120000','160000') else ''))
   self._by_name=index
  items=sorted(x for name in {'\0link',*basenames} for x in self._by_name.get(name,()))
  return H(encoded(['scope-v1',sorted(set(basenames)),items]))
 def read(self,path):
  if path not in self._bytes:self._bytes[path]=subprocess.check_output(['git','--git-dir',self.bare,'cat-file','blob',self.tree[path][1]])
  return self._bytes[path]
 def canonical(self,main):
  for name in ('config/map.json','config/header_manifest.json','config/symbols.json'):
   if self.tree.get(name)!=main.tree.get(name):raise ValueError('candidate '+name+' differs from generated main; rebase/regenerate before judging')
 def request(self,tu,context):
  if not tu.startswith('src/') or Path(tu).is_absolute() or '..' in Path(tu).parts:raise ValueError('invalid TU path')
  if self.tree.get(tu,('',''))[0] not in ('100644','100755'):return None
  source=self.read(tu)
  if volatile_reason([source,encoded(context)]):return None
  # All config and expected packet identities bind the result; canonical config was checked before lookup.
  controls={p:v for p,v in self.tree.items() if p.startswith('config/') or p in ('expected/'+tu[4:]+'.json','expected/'+tu[4:]+'.neighbours.json')}
  return json.loads(encoded({'schema':SCHEMA,'tu':tu,'source_sha256':H(source),'source_blob':self.tree[tu],'controls':controls,'context':context}))

class ResultCache:
 def __init__(self,root):self.root=Path(root);self.root.mkdir(parents=True,exist_ok=True)
 def candidates(self,request):
  key=H(encoded(request));path=self.root/'index'/(key+'.json')
  try:return key,json.loads(path.read_text()).get('entries',[])
  except (OSError,ValueError,TypeError):return key,[]
 def lookup(self,view,request):
  if request is None:return None,'volatile or unsupported source inputs'
  key,entries=self.candidates(request)
  reason='no matching dependency result'
  for entry_key in reversed(entries):
   try:
    directory=self.root/'entries'/entry_key;entry=json.loads((directory/'result.json').read_text())
    if entry['request']!=request or entry['schema']!=SCHEMA:continue
    payload={k:v for k,v in entry.items() if k!='entry_key'}
    if H(encoded(payload))!=entry_key:raise ValueError('cache result integrity differs')
    for dep in entry['dependencies']:
     if dep['kind']=='repo':
      if view.tree.get(dep['path'])!=tuple(dep['blob']):raise ValueError('repo dependency changed: '+dep['path'])
     else:
      if digest_file(dep['path'])!=dep['sha256']:raise ValueError('external dependency changed: '+dep['path'])
    scope=entry['name_scope']
    if view.scoped_namespace(scope['basenames'])!=scope['digest']:raise ValueError('include name scope changed')
    for scope in entry.get('external_namespaces',[]):
     if remembered_namespace([scope['root']])!=scope['digest']:raise ValueError('external include resolution changed: '+scope['root'])
    if entry['object_sha256'] is not None:
     if digest_file(directory/'object.obj')!=entry['object_sha256']:raise ValueError('cached object changed')
    elif not entry['result'].get('error'):raise ValueError('result has no compiler artifact or compile error')
    result=json.loads(json.dumps(entry['result']));result.update(cache_hit=True,cache_key=entry_key,cache_artifact=str(directory),compile_s=0)
    return result,'validated actual dependencies'
   except (OSError,ValueError,TypeError,KeyError) as exc:reason=str(exc)
  return None,reason
 def store(self,view,request,job,tu,result):
  if request is None:return None,'volatile or unsupported source inputs'
  job=Path(job);obj=job/'obj'/(tu.replace('/','_')+'.obj');capture=obj.with_suffix('.obj.includes.json');receipt=obj.with_suffix('.obj.compile.json')
  if not capture.exists():return None,'no complete compiler include capture'
  includes=json.loads(capture.read_text())
  if any(p.startswith('!UNRESOLVED! ') for p in includes):return None,'compiler include path unresolved'
  dependencies=[];paths=[job/tu]+[Path(p) for p in includes];compiler_paths=set(paths);compiler_contents=[]
  paths += [job/p for p in request['controls']]
  seen=set()
  for path in paths:
   path=Path(path)
   if path in seen:continue
   seen.add(path);content=path.read_bytes()
   if path in compiler_paths:compiler_contents.append(content)
   try:relative=path.relative_to(job).as_posix()
   except ValueError:relative=None
   if relative is not None:
    meta=view.tree.get(relative)
    if not meta or meta[0] not in ('100644','100755') or blob_id(content)!=meta[1]:return None,'compiled repo input differs from committed blob: '+str(path)
    dependencies.append({'kind':'repo','path':relative,'blob':meta,'sha256':H(content)})
   else:dependencies.append({'kind':'external','path':str(path),'sha256':H(content)})
  volatile=volatile_reason(compiler_contents+[encoded(request['context'])])
  if volatile:return None,volatile
  if result.get('error') or not literal_includes_only(compiler_contents):basenames=None
  else:basenames=sorted({fold_name(Path(p).name) for p in includes}|{fold_name(Path(tu).name)})
  name_scope={'basenames':basenames,'digest':view.scoped_namespace(basenames)}
  object_sha=None
  if obj.exists():
   if not receipt.exists():return None,'no compiler artifact receipt'
   meta=json.loads(receipt.read_text());object_sha=digest_file(obj)
   if not meta.get('compiler_contract') or meta.get('compiler_contract')!=request['context'].get('compiler_contract'):return None,'unattested or different compiler contract'
   if meta.get('include_capture')!='msvc-showIncludes-v1':return None,'compiler include capture contract missing'
   if meta['source_sha256']!=digest_file(job/tu) or meta['object_sha256']!=object_sha:return None,'stale compiler artifact receipt'
   for path,digest in meta['headers'].items():
    if digest_file(path)!=digest:return None,'stale actual header receipt'
  elif not result.get('error'):return None,'no compiler artifact'
  known=[Path(p).resolve() for p in request['context'].get('external_roots',[])]
  other=set()
  for path in map(Path,includes):
   if path.is_relative_to(job) or any(path.resolve().is_relative_to(root) for root in known):continue
   other.add(path.parent)
  scopes=[{'root':str(path),'digest':namespace([path])} for path in sorted(other)]
  clean=json.loads(json.dumps(result));clean.pop('cache_hit',None)
  compiler_headers=[d for d in dependencies if (job/d['path'] if d['kind']=='repo' else Path(d['path'])) in {Path(p) for p in includes}]
  entry={'schema':SCHEMA,'request':request,'dependencies':dependencies,'compiler_headers':compiler_headers,'name_scope':name_scope,'external_namespaces':scopes,'object_sha256':object_sha,'result':clean}
  entry_key=H(encoded(entry));entry['entry_key']=entry_key;directory=self.root/'entries'/entry_key
  lockpath=self.root/'locks'/(H(encoded(request))+'.lock');lockpath.parent.mkdir(exist_ok=True)
  with lockpath.open('a+') as lock:
   fcntl.flock(lock,fcntl.LOCK_EX);directory.mkdir(parents=True,exist_ok=True)
   if obj.exists():
    temp=directory/('object.'+str(os.getpid())+'.tmp');shutil.copyfile(obj,temp);os.replace(temp,directory/'object.obj')
   atomic_json(directory/'result.json',entry)
   key,entries=self.candidates(request)
   if entry_key not in entries:entries.append(entry_key)
   # Bounded variants per source/control context; a removed artifact causes a safe miss for concurrent readers.
   discarded=entries[:-8];entries=entries[-8:]
   atomic_json(self.root/'index'/(key+'.json'),{'entries':entries})
   for old in discarded:
    if old!=entry_key:shutil.rmtree(self.root/'entries'/old,ignore_errors=True)
  return entry_key,'stored actual compiler inputs'

 def materialize(self,result,job,tu):
  """Expose the cached compiler artifact in a requested debug job, retaining its dependency certificate."""
  directory=Path(result['cache_artifact']);entry=json.loads((directory/'result.json').read_text());job=Path(job)
  obj=job/'obj'/(tu.replace('/','_')+'.obj');obj.parent.mkdir(exist_ok=True)
  headers={str(job/d['path']) if d['kind']=='repo' else d['path']:d['sha256'] for d in entry['compiler_headers']}
  if entry['object_sha256'] is not None:
   shutil.copyfile(directory/'object.obj',obj)
   atomic_json(obj.with_suffix('.obj.compile.json'),{'source_sha256':entry['request']['source_sha256'],'object_sha256':entry['object_sha256'],'headers':headers,'imported_from_result_cache':entry['entry_key'],'compiler_contract':entry['request']['context']['compiler_contract'],'include_capture':'msvc-showIncludes-v1'})
  atomic_json(obj.with_suffix('.obj.includes.json'),list(headers))
  return obj
