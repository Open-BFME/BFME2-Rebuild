"""Linked-image emission policy: typed template folds and conservative COFF reachability."""
import re,subprocess,struct
from pathlib import Path
from functools import lru_cache
PLACEHOLDER=re.compile(r"[Rr]va[0-9A-Fa-f]{6,8}|Bfme|Gen_t_|_0x[0-9A-Fa-f]{6}")
ANON=re.compile(r"\?A0x[0-9A-Fa-f]{8}")
# VC7.1 names a dynamic initializer or atexit terminator _$E<n>, numbered afresh in every object: the name identifies
# nothing across objects, so it never binds by name. Its address comes from where it is used:
# the .CRT$XCU slot, the atexit operand of the body that registers it. ??__E/??__F are the later compilers' spellings
# of the same functions; VC7.1 never emits them, so on a map row they describe the role, not a compilable name.
COMPILER_LOCAL=re.compile(r"_?\$E\d+")
GENERATED_DESCRIPTIONS=('??__E','??__F')
def compiler_local(name):return bool(name) and COMPILER_LOCAL.fullmatch(name) is not None
# The demangler is the retained LLVM helper; failed/unsupported signatures never establish a fold.
# The demangler binary path is supplied by the local checker settings.
from checker_configuration import DEMANGLER
@lru_cache(maxsize=8192)
def demangle(name):
 try:
  p=subprocess.run([str(DEMANGLER)],input=name+'\n',text=True,capture_output=True,timeout=10,check=True)
  value=p.stdout.strip();return value if value and value!=name else None
 except OSError as error:raise RuntimeError('demangler unavailable: %s (%s)'%(DEMANGLER,error)) from error
 except subprocess.SubprocessError:return None

def split_args(text):
 level=0;out=[];start=0
 for i,c in enumerate(text):
  if c=='<':level+=1
  elif c=='>':
   level-=1
   if level<0:return None
  elif c==',' and not level:out.append(text[start:i].strip());start=i+1
 if level:return None
 out.append(text[start:].strip());return out if all(out) else None

@lru_cache(maxsize=8192)
def template_member_key(name):
 if not name.startswith('?') or '?$' not in name or PLACEHOLDER.search(name):return None
 text=demangle(name)
 if not text:return None
 found=[]
 for match in re.finditer(r'(?:[A-Za-z_]\w*::)*[A-Za-z_]\w*<',text):
  begin=match.end()-1;level=1;end=begin+1
  while end<len(text) and level:
   if text[end]=='<':level+=1
   elif text[end]=='>':level-=1
   end+=1
  if level or text[end:end+2]!='::':continue
  member=re.match(r'(~?[A-Za-z_]\w*)',text[end+2:])
  if not member:continue
  family=text[match.start():begin];raw=text[begin+1:end-1];args=split_args(raw)
  if not args:continue
  tail=text[end+2+len(member[0]):]
  # Constructors/destructors repeat class arguments. Function templates and operator spellings abstain.
  if tail.startswith('<'):
   if not tail.startswith('<'+raw+'>'):continue
   tail=tail[len(raw)+2:]
  if not tail.startswith('('):continue
  signature=text[:match.start()]+'OWNER::'+member[0]+tail
  for index,arg in sorted(enumerate(args),key=lambda pair:len(pair[1]),reverse=True):
   signature=re.sub(r'(?<![A-Za-z0-9_])'+re.escape(arg)+r'(?![A-Za-z0-9_])','ARG'+str(index),signature)
  found.append((family,member[0],signature,len(args)))
 return found[0] if len(found)==1 else None

def folded_identity(symbol,names):
 key=template_member_key(symbol)
 if key is None:return None
 aliases=[n for n in names if n!=symbol and template_member_key(n)==key]
 if len(aliases)!=1:return None
 return {'kind':'folded_identity','compiled_name':symbol,'retail_name':aliases[0],'template':key[0],'member':key[1]}

def extra_candidates(checker,name):
 if compiler_local(name):return set()
 norm=lambda n:ANON.sub('?A0xHASH',n)
 if not hasattr(checker,'extra_name_index'):
  index={}
  for n,addresses in checker.names.items():index.setdefault(norm(n),set()).update(addresses)
  checker.extra_name_index=index
 exact=set(checker.extra_name_index.get(norm(name),()))
 # Another instantiation is a counterpart only when a verified retail reference already binds it.
 # Similar template families at other RVAs may have different layouts and code; do not guess a fold.
 try:index=checker.obj.index(name)
 except ValueError:index=None
 bound=checker.bindings.get(index)
 if not exact and bound in checker.rows:
  row=checker.rows[bound]
  if row['role'] in ('comdat','icf_fold') and folded_identity(name,row['accepted_names']):exact.add(bound)

 return {a for a in exact if checker.rows[a]['role'] in ('owned','comdat','icf_fold','thunk','import_thunk')}

def retained_sections(obj,kept):
 groups={};graph={};roots=set();notes=[]
 for sec,s in enumerate(obj.sections,1):
  root=obj.root(sec);groups.setdefault(root,[]).append(sec);graph.setdefault(root,set())
  if s['raw_size'] and not s['name'].startswith(('.debug','.drectve')) and not s['characteristics']&0x800: # IMAGE_SCN_LNK_REMOVE
   if not s['characteristics']&0x1000 or s['name'].startswith('.CRT'):roots.add(root)
 for name in kept:
  for index in obj.names.get(name,[]):
   if obj.symbols[index]['section']>0:roots.add(obj.root(obj.symbols[index]['section']))
 def local_sections(index,seen=None):
  seen=set() if seen is None else seen
  if index in seen:return set()
  seen.add(index);sym=obj.symbols[index];sec=sym['section']
  if sec>0:return {obj.root(sec)}
  out=set()
  if sec==0:
   for i in obj.names.get(sym['name'],[]):
    if obj.symbols[i]['section']>0:out.add(obj.root(obj.symbols[i]['section']))
   if index in obj.weak:out|=local_sections(obj.weak[index][0],seen)
  return out
 for sec,s in enumerate(obj.sections,1):
  if s['name']=='.drectve':
   text=obj.bytes((sec,0),s['raw_size']).decode('ascii',errors='replace')
   tokens=re.findall(r'(?:[^\s"]|"[^"]*")+',text)
   for token in tokens:
    option=token.lstrip('/-');key=option.split(':',1)[0].lower()
    if key in ('include','export','entry') and ':' in option:
     name=option.split(':',1)[1].strip('"').split(',')[0];name=name.split('=')[-1]
     for index in obj.names.get(name,[]):roots|=local_sections(index)
    elif key not in ('defaultlib','nodefaultlib','failifmismatch'):
     # Unknown linker directives may root otherwise unused symbols. Abstain rather than drop them.
     notes.append('unsupported linker directive '+token)
   continue
  if s['name'].startswith('.debug') or s['characteristics']&0x800:continue
  for off,kind,index in obj.relocs((sec,0),s['raw_size']):
   if kind!=0:graph[obj.root(sec)]|=local_sections(index)
 live=set(roots);pending=list(roots)
 while pending:
  for target in graph.get(pending.pop(),set()):
   if target not in live:live.add(target);pending.append(target)
 return live,notes

def selectable_copy(obj,sym,root,selection):
 positions={x['value'] for x in obj.symbols if x.get('section')==root and x.get('type',0)&32}
 return sym['section']==root and sym['value']==0 and positions=={0} and bool(obj.sections[root-1]['characteristics']&0x1000) and selection in (2,3,4,6)

def fold_member(checker,index,name):
 """An extra that the verified walk bound, through retail's own operand (a vtable slot, a call), to an address where
 the linker folded identical bodies (map role icf_fold), and whose body and relocations equal retail's there: one more
 member of that fold. The map lists other names at the address, so this name stays unproven (recorded in the proof);
 a body that differs, any other role, or no binding leaves the extra rejected."""
 bound=checker.bindings.get(index)
 if compiler_local(name) or bound is None or bound not in checker.rows or checker.rows[bound]['role']!='icf_fold':return None
 return bound,checker.verify(index,bound,kind='fold-member')

PLAIN_MEMBER=re.compile(r'\?([A-Za-z_]\w*)((?:@[A-Za-z_]\w*)+)@@[A-Z]')
SPECIAL_MEMBER=re.compile(r'\?\?([01])([A-Za-z_]\w*(?:@[A-Za-z_]\w*)*)@@[A-Z]')
def plain_member(name):
 """('member', ('Outer','Inner')) for a plain out-of-line member '?member@Inner@Outer@@...'; None for templates,
 operators, compiler-local, anonymous or placeholder names (the rule abstains on those). Constructors and
 destructors ('??0Class@@', '??1Class@@') give ('()', scopes) and ('~', scopes)."""
 if not name or PLACEHOLDER.search(name) or ANON.search(name) or '?$' in name:return None
 m=SPECIAL_MEMBER.match(name)
 if m:return ('~' if m.group(1)=='1' else '()'),tuple(reversed(m.group(2).split('@')))
 m=PLAIN_MEMBER.match(name)
 if not m:return None
 return m.group(1),tuple(reversed(m.group(2)[1:].split('@')))

def whole_section(obj,sym,root):
 positions={x['value'] for x in obj.symbols if x.get('section')==root and x.get('type',0)&32}
 return sym['section']==root and sym['value']==0 and positions=={0} and bool(obj.sections[root-1]['characteristics']&0x1000)

def unit_scopes(checker):
 """The class scopes this translation unit's own map rows name (accepted names of rows of the unit's object)."""
 if not hasattr(checker,'_gen6_unit_scopes'):
  from identity_index import map_index
  oid=(checker.unit or {}).get('object_id');scopes=set()
  if oid and oid!='unknown':
   index=map_index(checker.map)['by_object']
   for row in index.get(oid,[])+index.get('retained:'+oid,[]):
    for n in row.get('accepted_names') or ():
     pm=plain_member(n)
     if pm:scopes.add(pm[1])
  checker._gen6_unit_scopes=scopes
 return checker._gen6_unit_scopes

def icf_member(checker,obj,index,name,sym,root,selection,candidates,verify=True):
 """UpdateModule::loadPostProcess example: an out-of-line member function (its own NODUPLICATES COMDAT,
 selection 1) whose name the map lists at a retail address where the linker folded identical bodies of DIFFERENT
 functions (role icf_fold): retail keeps one copy for all of them, so every other unit's definition is an ICF
 duplicate with no row of its own. Accepted (no credit, no VERIFIED cost) when (a) its body and relocations verify
 at that address, (b) the map row is an icf_fold row with a fold group, lists this name, and lists at least one other
 Class::member besides it, while no other accepted name there is another spelling of this same Class::member (a
 respelling would mean two definitions of one function), and (c) the address is not this unit's own row and this
 unit's own rows name the member's class, so the definition sits in its class's unit and not in a stranger's.
 Returns (target, check) or None when the rule does not apply (the caller keeps its refusal)."""
 if selection!=1 or not whole_section(obj,sym,root):return None
 pm=plain_member(name)
 if pm is None or len(candidates)!=1:return None
 target=next(iter(candidates));row=checker.rows.get(target) or {}
 if row.get('role')!='icf_fold' or not row.get('fold_group'):return None
 names=row.get('accepted_names') or ()
 if name not in names:return None
 others=[plain_member(n) for n in names if n!=name]
 if any(o==pm for o in others):return None
 if not any(o is not None and o!=pm for o in others):return None
 oid=(checker.unit or {}).get('object_id')
 if not oid or checker.object_of(row)==oid:return None
 if pm[1] not in unit_scopes(checker):return None
 return target,(checker.verify(index,target,kind='fold-member') if verify else None)

NO_IDENTITY='extra is reachable from kept code/data, an associative parent or a linker root; no retail identity'
def classify_extras(checker,kept,extras):
 """order-independent. A pass that accepts an extra may bind (through its verified walk) another extra's
 retail address; extras rejected only for lacking a retail identity are classified again until nothing changes."""
 accepted,rejected=_classify_extras_once(checker,kept,extras)
 while True:
  retry=[r['symbol'] for r in rejected if r.get('why')==NO_IDENTITY]
  if not retry:return accepted,rejected
  more,still=_classify_extras_once(checker,kept,retry)
  if not more:return accepted,rejected
  accepted=accepted+more;rejected=[r for r in rejected if r.get('why')!=NO_IDENTITY]+still

def _classify_extras_once(checker,kept,extras):
 obj=checker.obj;live,notes=retained_sections(obj,kept);accepted=[];rejected=[]
 for name in extras:
  record={'symbol':name}
  try:
   index=obj.index(name);sym=obj.symbols[index];root=obj.root(sym['section']);selection=obj.comdat.get(root,0)
   record.update(section=sym['section'],comdat_selection=selection,referenced_by_kept=root in live)
   candidates=extra_candidates(checker,name)
   if candidates:
    # A known counterpart which differs must never slip through the unreferenced case.
    if not selectable_copy(obj,sym,root,selection):
     member=icf_member(checker,obj,index,name,sym,root,selection,candidates)
     if member is None:raise ValueError('retail duplicate is not a selectable COMDAT copy')
     target,check=member
     record.update(retail_rva='0x%08X'%target,retail_names=checker.rows[target]['accepted_names'],relocation_check=check)
     if check['status']!='MATCH':raise ValueError('ICF fold member at 0x%X but %s: %s'%(target,check['status'],check.get('why','')))
     record['classification']='extra_icf_member';accepted.append(record);continue
    bound=checker.bindings.get(index)
    if bound is not None:candidates&={bound}
    if len(candidates)!=1:raise ValueError('retail duplicate identity has multiple/contradictory locations')
    target=next(iter(candidates));check=checker.verify(index,target)
    record.update(retail_rva='0x%08X'%target,retail_names=checker.rows[target]['accepted_names'],relocation_check=check)
    if check['status']!='MATCH':raise ValueError('retail duplicate '+check['status']+': '+check.get('why',''))
    record['classification']='extra_duplicate';accepted.append(record);continue
   member=fold_member(checker,index,name)
   if member is not None:
    target,check=member
    if not selectable_copy(obj,sym,root,selection):
     if icf_member(checker,obj,index,name,sym,root,selection,{target},verify=False) is None:raise ValueError('fold member is not a selectable COMDAT copy')
    record.update(retail_rva='0x%08X'%target,retail_names=checker.rows[target]['accepted_names'],relocation_check=check)
    if check['status']!='MATCH':raise ValueError('bound to fold 0x%X but %s: %s'%(target,check['status'],check.get('why','')))
    record['classification']='extra_fold_member';accepted.append(record);continue
   if notes:raise ValueError('; '.join(notes))
   if root in live:raise ValueError(NO_IDENTITY)
   if not obj.sections[root-1]['characteristics']&0x1000 or selection not in (1,2,3,4,6):
    raise ValueError('extra has no independently discardable COMDAT section')
   record['classification']='extra_unreferenced';accepted.append(record)
  except (ValueError,KeyError,IndexError,struct.error) as exc:
   record.update(classification='extra_rejected',why=str(exc));rejected.append(record)
 return accepted,rejected
