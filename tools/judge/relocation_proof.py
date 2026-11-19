"""Fail-closed recursive comparison of linked relocation targets and compiler metadata."""
import struct,re,json
from binary_model import U
from identity_index import map_index,symbol_index,function_span
from emission_policy import folded_identity,compiler_local
ANON=re.compile(r'\?A0x[0-9A-Fa-f]{8}')
class Unverified(Exception):pass
CODE_ROLES=('owned','comdat','icf_fold','unwind','funclet','ehhandler','initializer','terminator','thunk','import_thunk')
class Mismatch(Exception):pass
MASK=0xffffffff
def pe_import_name(symbol):
 """The PE import name a compiled __imp_ symbol refers to: C++ decorated names as they are; C names without the
 leading underscore and without a stdcall @N suffix (__imp__RegQueryValueExA@24 -> RegQueryValueExA)."""
 name=symbol[len('__imp_'):]
 if name.startswith('?'):return name
 if name.startswith('_'):name=name[1:]
 return re.sub(r'@\d+$','',name)

# Import-library aliases (batch9c, grouped by equivalent imports): (DLL lower-case, the name a compiled __imp_ symbol imports)
# -> the other export names of its class: exports retail's import libraries bound to one import entry (import_aliases.py,
# generated from the real DLL's export addresses, or a reviewed retail-witnessed pair). Read as a literal, never executed.
def _import_aliases():
 import ast
 from pathlib import Path as _P
 tree=ast.parse((_P(__file__).resolve().parent/'import_aliases.py').read_text())
 classes=next(ast.literal_eval(n.value) for n in tree.body if isinstance(n,ast.Assign) and [t.id for t in n.targets if isinstance(t,ast.Name)]==['CLASSES'])
 out={}
 for c in classes:
  for name in c['names']:out.setdefault((c['dll'].lower(),name),set()).update(n for n in c['names'] if n!=name)
 return {k:frozenset(v) for k,v in out.items()}
IMPORT_ALIASES=_import_aliases()

class Resolver:
 def __init__(self,obj,pe,mapdoc,symbols,identity,paired=None,unit=None):
  # UNIT: the map object of the translation unit being judged (object_id, text_ranges); None disables the
  # kept-elsewhere rule below.
  self.unit=unit;self.paired=paired or {};self.obj=obj;self.pe=pe;self.map=mapdoc;self.symbols=symbols;self.identity=identity
  index=map_index(mapdoc);self.rows=index['rows'];self.names=index['names'];self.extra_name_index=index['normalized_names'];self.bindings={};self.reverse={};self.done=set();self.active=set();self.proofs=[];self.nodes=0;self.code_frames=[];self.unproven=[];self._alias_slots=None;self.block_notes={}
  self.named=symbols.get('symbols',{});self.tables=symbols.get('vtables',{});self.data_items=symbol_index(symbols)['data_items']
  # Retail vtables by address (any name): their listed slot counts and names, for the content proof in vtable().
  self.table_sizes={};self.table_names={}
  for tname,listed in self.tables.items():
   for v in listed:
    try:a=int(v['rva'],16);n=int(v['slots'])
    except (KeyError,TypeError,ValueError):continue
    self.table_sizes.setdefault(a,set()).add(n);self.table_names.setdefault(a,set()).add(tname)
  # Listed extents the map itself leaves open (batch8): '?:deleting-dtor at slot N (an unreferenced table may abut)'
  # (the table may end after slot N), and an end 'at next vtable X' where the map lists no table at X (the claimed end
  # is unsupported; retail's run of code pointers bounds the table). Every other listed length is exact.
  self.table_open={}
  for tname,listed in self.tables.items():
   for v in listed:
    try:a=int(v['rva'],16);n=int(v['slots'])
    except (KeyError,TypeError,ValueError):continue
    evidence=v.get('evidence') or ''
    abut=re.search(r'\?:deleting-dtor at slot (\d+) \(an unreferenced table may abut\)',evidence)
    ends=[int(x,16) for x in re.findall(r'X:ends at next vtable (0x[0-9A-Fa-f]+)',evidence)]
    if abut:self.table_open[a]=('abut',int(abut.group(1))+1,n)
    elif ends and any(x not in self.table_sizes for x in ends):self.table_open[a]=('unsupported end',None,n)
 def code_run(self,rva,limit=1024):
  """Retail's run of code pointers from RVA, up to the next listed table or the first dword not pointing into code."""
  k=0
  while k<limit:
   at=rva+4*k
   if k and at in self.table_sizes:break
   try:
    word=(U(self.pe.read(at,4))-self.pe.base)&MASK
    if not self.pe.section(word,1)['flags']&32:break
   except ValueError:break
   k+=1
  return k
 def open_length(self,rva,compiled):
  """The range of lengths the map leaves open for the table listed at RVA holds COMPILED slots: the length is
  unproven, not wrong. A length outside it, or any other listed table's different length, is wrong."""
  kind=self.table_open.get(rva)
  if not kind or compiled<=0:return False
  if kind[0]=='abut':return kind[1]<=compiled<kind[2]
  return compiled<=self.code_run(rva)
 def fail(self,why,mismatch=False):raise (Mismatch if mismatch else Unverified)(why)
 def kept_elsewhere(self,index,rva):
  """True when this unit's copy of the code target at RVA is a select-any COMDAT with external linkage (an inline or
  template function) that the map places outside this unit: in another object, or in none but outside this unit's
  text ranges. The linker keeps one copy per name and retail kept that other copy, so this one is discarded."""
  if not self.unit:return False
  s=self.obj.symbols[index]
  if s.get('storage')!=2 or s['section']<=0:return False
  if not self.obj.sections[s['section']-1]['characteristics']&0x1000 or self.obj.comdat.get(s['section'])!=2:return False
  row=self.rows[rva];owner=(row.get('object') or row.get('object_id') or 'unknown').split(':')[-1]
  if owner==self.unit.get('object_id'):return False
  for part in (self.unit.get('text_ranges') or '').split(';'):
   if '-' in part:
    lo,hi=(int(x,16) for x in part.split('-',1))
    if lo<=rva<hi:return False
  return True
 def candidate_applies(self,v,s):
  """Batch9b: a rebuilt-evidence identity (class 'R') scoped to a map object names a static of that object: it counts
  only for that static defined in the unit judged. An unscoped 'R' identity names an external and never counts for a
  static. Other candidates count as before."""
  static=s['section']>0 and s.get('storage')==3
  if v.get('scope'):return static and bool(self.unit) and self.unit.get('object_id')==v['scope']
  if v.get('evidence_class')=='R':return not static
  return True
 def import_alias_slots(self):
  """{IAT slot rva: designation} for the slots the map's import thunks designate for an IMPORT_ALIASES alias: a thunk
  row named for the alias whose retail bytes are jmp [slot], where retail's import directory names the slot with the
  alias's export. A slot that thunks of different imports jump through maps to None (unproven)."""
  if self._alias_slots is None:
   imports=getattr(self.pe,'imports',{});claims={}
   for a,row in self.rows.items():
    if row.get('role')!='import_thunk':continue
    try:b=self.pe.read(a,6)
    except ValueError:continue
    if b[:2]!=b'\xff\x25':continue
    slot=(U(b,2)-self.pe.base)&MASK;imp=imports.get(slot)
    if not imp or not imp.get('name'):continue
    for thunk_name in row.get('accepted_names') or [row.get('name','')]:
     want=pe_import_name('__imp_'+thunk_name)
     if want==imp['name']:claims.setdefault(slot,[]).append(None)
     elif imp['name'] in IMPORT_ALIASES.get((imp['dll'].lower(),want),()):
      claims.setdefault(slot,[]).append({'name':want,'thunk':a,'thunk_name':thunk_name,'import':'%s!%s'%(imp['dll'],imp['name'])})
   self._alias_slots={}
   for slot,found in claims.items():
    aliases={d['name'] for d in found if d}
    if not aliases:continue
    self._alias_slots[slot]=[d for d in found if d][0] if len(aliases)==1 and None not in found else None
   # a class retail imports at ONE slot of its DLL needs no designation: whichever name of the class the
   # source calls, the import library binds it to that one entry (retail's _mbsdup slot is where _strdup lands).
   groups={}
   for slot,imp in imports.items():
    if not imp.get('name'):continue
    others=IMPORT_ALIASES.get((imp['dll'].lower(),imp['name']))
    if others:groups.setdefault((imp['dll'].lower(),frozenset(others|{imp['name']})),[]).append(slot)
   for (dll,names),slots in groups.items():
    if len(slots)==1 and slots[0] not in claims:
     imp=imports[slots[0]]
     self._alias_slots[slots[0]]={'single':True,'names':names,'import':'%s!%s'%(imp['dll'],imp['name']),'own':imp['name']}
  return self._alias_slots
 def in_image(self,rva):
  try:self.pe.section(rva,1);return True
  except ValueError:return False
 def bind(self,index,rva):
  if index in self.bindings and self.bindings[index]!=rva:self.fail('inconsistent relocation binding for '+self.obj.symbols[index]['name'],True)
  self.bindings[index]=rva
 def named_data(self,index,rva,n,parent,name):
  """A defined data item whose address the name proves (map row or proven symbol evidence): the reference is proven
  as it would be with the item left external. Its contents are still compared: contents proven wrong fail the
  reference as before, while what they leave unproven (an unlisted vtable, a pointer to an unmapped function) is the
  item's own finding, kept in the proof for the data rule and not charged to the code that uses it (coordinator
  2026-11-17: a data definition must not cost code its proof)."""
  mark=len(self.unproven)
  self.block(self.obj.loc(index),rva,n,parent,'named-data')
  notes=self.unproven[mark:];del self.unproven[mark:]
  if notes:self.proofs.append({'kind':'named-data-contents-unproven','name':name,'rva':'0x%08X'%rva,'notes':notes[:8]})
 def code_loc(self,index):
  s=self.obj.symbols[index];return s['section']>0 and bool(self.obj.sections[s['section']-1]['characteristics']&32)
 def folded_frame_symbol(self,rva):
  """the compiled symbol whose code the walk is verifying at RVA (a frame is pushed only after code()'s
  identity, role and extent checks passed for it); None when no frame at RVA is open."""
  if rva is None:return None
  for f in reversed(self.code_frames):
   if f['rva']==rva:return self.obj.symbols[f['index']]['name']
  return None
 def folded_frame_root(self,parent):
  n=self.folded_frame_symbol(parent)
  if n is None:return self.parent_root(parent)
  roots={self.obj.root(self.obj.symbols[i]['section']) for i in self.obj.names.get(n,[]) if self.obj.symbols[i]['section']>0}
  if len(roots)!=1:self.fail('compiled parent is not uniquely identified at 0x%X'%parent)
  return next(iter(roots))
 def parent_root(self,parent):
  row=self.rows.get(parent)
  if not row:self.fail('parent missing from identity map: '+str(parent))
  found=set()
  for name in [self.paired.get(parent,'')]+row['accepted_names']:
   for i in self.obj.names.get(name,[]):
    if self.obj.symbols[i]['section']>0:found.add(self.obj.root(self.obj.symbols[i]['section']))
  if len(found)!=1:self.fail('compiled parent is not uniquely identified at 0x%X'%parent)
  root=next(iter(found))
  positions={x['value'] for x in self.obj.symbols if x.get('section')==root and x.get('type',0)&32 and not x['name'].startswith(('$L','__ehhandler$'))}
  if len(positions)>1:self.fail('associative root contains multiple functions; exact parent is not proven')
  return root
 def prologue_handler(self,parent):
  row=self.rows.get(parent)
  if not row or row['size']<10:return None
  b=self.pe.read(parent,10)
  # VC7.1's EAX handler, call __EH_prolog form. Other forms are deliberately not guessed.
  if b[0]!=0xb8 or b[5]!=0xe8:return None
  helper=(parent+10+struct.unpack('<i',b[6:10])[0])&MASK
  if helper not in self.names.get('__EH_prolog',set()):return None
  return (U(b,1)-self.pe.base)&MASK
 def handler_rva(self,parent):
  # The proof's note of the handler a parent's __EH_prolog loads; None for any other frame form (inline SEH pushes
  # its handler instead), which prologue_handler deliberately does not guess (m02 2026-11-17: '%X' % None crashed).
  h=self.frame_handler(parent)[0]
  return '0x%08X'%h if h is not None else None
 def frame_handler(self,parent):
  """(handler rva, frame form) the parent's prologue installs, from retail's bytes, or (None, None).
  'eh-prolog': mov eax,handler / call __EH_prolog (prologue_handler). 'inline-seh': the C++ EH frame VC7.1 writes inline:
  push -1 ... push offset handler ... mov fs:[0],esp within the prologue, in any of the orders retail schedules
  (55 8B EC 6A FF 68, 6A FF 68 .. 64 A1, 64 A1 .. 6A FF 68, 6A FF 64 A1 .. 68); the pushed constant counts only if its
  target has the handler stub's shape (mov eax,FuncInfo / jmp) and it is the only such push. Nothing else is guessed."""
  h=self.prologue_handler(parent)
  if h is not None:return h,'eh-prolog'
  row=self.rows.get(parent)
  if not row or row['size']<12:return None,None
  b=self.pe.read(parent,min(row['size'],48))
  minus1=b.find(bytes.fromhex('6aff'));install=b.find(bytes.fromhex('64892500000000'))
  if minus1<0 or minus1>12 or install<0:return None,None
  found=set()
  for k in range(minus1+2,install):
   if b[k]!=0x68 or k+5>install:continue
   target=(U(b,k+1)-self.pe.base)&MASK
   try:stub=self.pe.read(target,10)
   except ValueError:continue
   if stub[0]==0xb8 and stub[5]==0xe9:found.add(target)
  return (found.pop(),'inline-seh') if len(found)==1 else (None,None)
 def same_eh_parent_family(self,a,b):
  aa=self.frame_handler(a)[0];bb=self.frame_handler(b)[0]
  return aa is not None and aa==bb
 def witnessed_parents(self,rva,row,binary_parents):
  """a handler or funclet retail shares among several parents is proven only as a set: every parent retail
  witnesses (symbols eh_parents) must be a code row of the map, and a witnessed parent whose prologue installs a handler
  must install this one (for a handler row). Returns {parent rva: installed handler rva or None}."""
  out={}
  for p in sorted(binary_parents):
   prow=self.rows.get(p)
   if not prow or prow.get('role') not in CODE_ROLES:self.fail('retail EH parent 0x%X of 0x%X has no code row in the map'%(p,rva))
   h=self.frame_handler(p)[0]
   if row['role']=='ehhandler' and h is not None and h!=rva:self.fail('retail EH parent 0x%X installs handler 0x%X, not 0x%X'%(p,h,rva))
   out[p]=h
  return out
 def object_of(self,row):
  """A map row's object id; a preserved contract's 'retained:<id>' is the same object as <id>."""
  o=(row or {}).get('object')
  return o[len('retained:'):] if isinstance(o,str) and o.startswith('retained:') else o
 def folded_eh_owner(self,s,row,expected,context):
  """The linker folds identical EH handlers and unwind funclets across functions, and the map gives the folded copy to
  one of them (or, for a handler, only names it after one). Another function of the same object may use it when the
  compiled handler/funclet belongs to that function's own COMDAT group and, for a handler, carries that function's
  decoration; the caller then compares its bytes and references (FuncInfo, unwind map, funclets) with retail. Returns
  the map's parent, or None."""
  other=expected
  if other is None and row['role']=='ehhandler' and row['name'].startswith('__ehhandler$'):
   named=sorted(self.names.get(row['name'][len('__ehhandler$'):],()))
   other=named[0] if len(named)==1 else None
  if other is None and row['role'] in ('unwind','funclet'):
   # A folded funclet the map gives no parent: only through a handler this check is verifying (a code frame above
   # this one, inside the context function's verified code), mapped in the context function's object, compiled in
   # its COMDAT group. The caller then compares its bytes and references with retail.
   obj=self.object_of(self.rows[context])
   if obj in (None,'unknown') or self.object_of(row)!=obj:return None
   if not any(self.rows.get(f['rva'],{}).get('role')=='ehhandler' for f in self.code_frames):return None
   try:
    if self.obj.root(s['section'])!=self.parent_root(context):return None
   except Unverified:return None
   return context
  if other is None or other not in self.rows:return None
  obj=self.object_of(self.rows[context])
  if obj in (None,'unknown') or self.object_of(self.rows[other])!=obj:return None
  try:
   if self.obj.root(s['section'])!=self.parent_root(context):return None
  except Unverified:return None
  if row['role']=='ehhandler':
   ps=self.paired.get(context)
   if ps is None:
    found=[n for n in self.rows[context]['accepted_names'] if n in self.obj.names]
    ps=found[0] if len(found)==1 else None
   if ps is None or s['name']!='__ehhandler$'+ps:return None
  return other
 def code(self,index,rva,parent=None,member=False):
  s=self.obj.symbols[index];row=self.rows.get(rva)
  if not row:self.fail('code target 0x%X has no mapped identity'%rva)
  if member and row['role']!='icf_fold':self.fail('fold member target 0x%X is not a folded body (role %s)'%(rva,row['role']),True)
  if row['role'] not in CODE_ROLES:self.fail('code target has non-code map role '+row['role'])
  local=s['name'].startswith('$L') and row['role'] in ('unwind','funclet')
  expected=int(row['parent'],16) if row.get('parent') else None
  binary_parents={int(x['parent'],16) for x in self.symbols.get('eh_parents',{}).get('0x%08X'%rva,[])}
  context=parent if parent is not None else expected
  if row['role'] in ('unwind','funclet','ehhandler') and context is None and binary_parents:
   candidates=[]
   for a in sorted(binary_parents):
    try:
     if self.obj.root(s['section'])==self.parent_root(a):candidates.append(a)
    except Unverified:continue
   if len(candidates)==1:context=candidates[0]
  if binary_parents and expected is not None and expected not in binary_parents:self.fail('map parent contradicts retail EH operand graph at 0x%X'%rva,True)
  derived=row['role'] in ('unwind','funclet','ehhandler') and context in binary_parents
  shared=derived or row['role'] in ('unwind','funclet','ehhandler') and expected is not None and context is not None and expected!=context and self.same_eh_parent_family(expected,context)
  witnessed=self.witnessed_parents(rva,row,binary_parents) if shared and binary_parents else {}
  if row['role']=='ehhandler' and shared:
   ps=self.paired.get(context)
   if ps is None:
    found=[n for n in self.rows[context]['accepted_names'] if n in self.obj.names]
    ps=found[0] if len(found)==1 else None
   if (ps is None or s['name']!='__ehhandler$'+ps) and s['name']!='__ehhandler$'+(self.folded_frame_symbol(context) or ''):
    self.fail('shared EH alias has no exact compiled parent decoration',True)
  content_shared=None
  # Only inside the context function's own verified code: retail's operand there is the evidence that it uses this copy.
  if not shared and row['role'] in ('unwind','funclet','ehhandler') and context is not None and context in self.rows and expected!=context      and any(frame['rva']==context for frame in self.code_frames):
   content_shared=self.folded_eh_owner(s,row,expected,context)
  # member: an extra bound to this folded body by retail's own operand (emission_policy.fold_member). Its name is
  # not one the map lists here; everything but the name is checked, and the proof says the name is unproven.
  why=None if local or shared or content_shared or member else self.identity(s['name'],row['accepted_names'],{})
  fold=folded_identity(s['name'],[row['name']]) if row['role'] in ('comdat','icf_fold') and not member else None
  unmapped_fold=None
  if member:self.proofs.append({'kind':'icf-fold-member','rva':'0x%08X'%rva,'compiled':s['name'],'listed_as':sorted(row['accepted_names']),'name':'unproven: the map does not list it at this address'})
  if why and not fold and self.code_frames and row['role'] in ('icf_fold','comdat') and why.startswith('compiled as ') and not self.names.get(s['name']) and not self.extra_name_index.get(re.sub(r'\?A0x[0-9A-Fa-f]{8}','?A0xHASH',s['name'])):
   # The linker folded identical bodies; the map names one member and has no row for this one. Its body is still
   # compared below; the name stays unproven until the map lists it.
   unmapped_fold='fold name absent from map at 0x%X: compiled %s, map row %s'%(rva,s['name'],row['name'])
  elif why and not fold:self.fail('code target identity at 0x%X: '%rva+why,True)
  # A COMDAT kept in another unit (kept_elsewhere): this copy is discarded at link, so the reference is proven by
  # name, as a call into another unit is, and its body is judged in the unit the map places it in. Only when the map
  # row names this symbol exactly: a row with placeholder names only (identity allows a rename) has no name to prove
  # the reference by, so its body is compared as before (batch8: batch6 let placeholder rows through).
  named_exactly=ANON.sub('?A0xHASH',s['name']) in {ANON.sub('?A0xHASH',n) for n in row['accepted_names']}
  if self.code_frames and why is None and named_exactly and not compiler_local(s['name']) and not fold and row['role'] in ('comdat','owned') and self.kept_elsewhere(index,rva):
   self.bind(index,rva)
   self.proofs.append({'kind':'comdat-kept-elsewhere','rva':'0x%08X'%rva,'name':s['name'],'map_object':row.get('object') or row.get('object_id') or 'unknown'})
   return
  if row['role'] in ('unwind','funclet','ehhandler'):
   if not content_shared and (expected is None and not derived or context is not None and expected!=context and not shared):self.fail('wrong funclet/handler parent at 0x%X'%rva,True)
   parent=context if shared or content_shared else expected
   if self.obj.root(s['section'])!=self.folded_frame_root(parent):self.fail('COFF associative parent differs at 0x%X'%rva,True)
   if content_shared:self.proofs.append({'kind':'content-shared-EH','rva':'0x%08X'%rva,'map_parent':('0x%08X'%content_shared if content_shared!=context else None),'compiled_parent':'0x%08X'%context})
   if shared:self.proofs.append({'kind':'binary-shared-EH-parent','rva':'0x%08X'%rva,'canonical_parent':('0x%08X'%expected if expected is not None else None),'compiled_parent':'0x%08X'%parent,'handler_rva':self.handler_rva(parent),'frame':self.frame_handler(parent)[1],
    'witnessed_parents':['0x%08X'%p for p in sorted(witnessed)],'handlers':{'0x%08X'%p:('0x%08X'%h if h is not None else None) for p,h in sorted({**witnessed,parent:self.frame_handler(parent)[0]}.items())}})
  self.bind(index,rva)
  n=self.obj.extent(index,code=True);span=function_span(self.map,rva);size=span['size']
  if span['problems']:self.fail('; '.join(span['problems']))
  blob=self.obj.bytes(self.obj.loc(index),n)
  # Alignment bytes may end a section; never strip an instruction or the beginning of another function.
  if n>size and all(b==0xcc for b in blob[size:]):n=size
  if n!=size:self.fail(('dependency: callee ' if self.code_frames else '')+'code extent %d != retail %d at 0x%X'%(n,size,rva),not self.code_frames)
  if row['role'] in ('initializer','terminator'):self.fail('initializer/terminator global-parent write association is not proven')
  context=parent if row['role'] in ('unwind','funclet','ehhandler','thunk') else rva
  if row['role']=='thunk':
   raw=self.obj.bytes(self.obj.loc(index),size);ret=self.pe.read(rva,size)
   tail=[(o,k,j) for o,k,j in self.obj.relocs(self.obj.loc(index),size) if k==20 and o+4==size and o>0 and raw[o-1]==0xe9]
   if len(tail)!=1:self.fail('thunk has no unique terminal direct-jump target')
   off=tail[0][0];target=(rva+off+4+struct.unpack('<i',ret[off:off+4])[0])&MASK
   if expected is not None and expected!=target:self.fail('map thunk parent differs from retail tail-jump target',True)
   self.proofs.append({'kind':'thunk-target','rva':'0x%08X'%rva,'target':'0x%08X'%target})
  frame={'index':index,'rva':rva,**span};self.code_frames.append(frame)
  try:self.block(self.obj.loc(index),rva,size,context,'code')
  finally:self.code_frames.pop()
  if span['inline_tables']:self.proofs.append({'kind':'function-jump-table-extent','rva':'0x%08X'%rva,'code_size':span['code_size'],'comparison_size':size,'tables':[r['rva'] for r in span['inline_tables']]})
  if fold:self.proofs.append({**fold,'rva':'0x%08X'%rva})
  if unmapped_fold:self.unproven.append(unmapped_fold)
 def block(self,loc,rva,size,parent=None,kind='data'):
  key=(loc,rva,size,parent,kind)
  if key in self.done:self.unproven.extend(self.block_notes.get(key,()));return
  if key in self.active:return
  self.nodes+=1
  if self.nodes>5000 or len(self.active)>64:self.fail('comparison graph exceeds resource bound')
  self.active.add(key)
  try:
   cb=self.obj.bytes(loc,size);tb=self.pe.read(rva,size);mask=bytearray(size);rels=self.obj.relocs(loc,size)
   for off,typ,index in rels:
    if typ==0:continue
    if typ not in (6,7,20):self.fail('unsupported i386 relocation type %d'%typ)
    if off+4>size:self.fail('relocation operand crosses compared extent',True)
    if any(mask[off:off+4]):self.fail('overlapping relocation operands',True)
    mask[off:off+4]=b'\1'*4
   if any(a!=b and not mask[i] for i,(a,b) in enumerate(zip(cb,tb))):
    # A callee compiled in this unit that does not match yet leaves the reference unproven, not wrong.
    dependency=kind=='code' and len(self.code_frames)>1
    self.fail(('dependency: callee ' if dependency else '')+'%s bytes differ at retail 0x%X'%(kind,rva),not dependency)
   mark=len(self.unproven)
   for off,typ,index in rels:
    if typ==0:continue
    try:self.reference(index,typ,U(cb,off),U(tb,off),rva+off,cb,off,parent,kind)
    except Unverified as exc:self.unproven.append(str(exc))
   self.done.add(key);self.block_notes[key]=list(self.unproven[mark:]);self.proofs.append({'kind':kind,'rva':'0x%08X'%rva,'size':size,'compiled_section':loc[0],'compiled_offset':loc[1]})
  finally:self.active.remove(key)
 def local_label(self,index,target,addend,parent,container):
  """a $L label (catch block, continuation) of the .text section of exactly one real compiled function is
  that function's code: proven when retail's operand is the function's retail address plus the label's offset, inside
  its row. The function's retail address is the code frame comparing it, else the EH parent whose COFF root it is. A
  catch block (a $L code symbol) must also be named by retail's FuncInfo-catch-action record for that parent."""
  s=self.obj.symbols[index]
  if not s['name'].startswith('$L') or s['section']<=0 or self.obj.sections[s['section']-1]['name']!='.text':return False
  funcs=[x for x in self.obj.symbols if x.get('section')==s['section'] and x.get('type',0)&32 and x.get('name') and not x['name'].startswith(('$L','.','__ehhandler$'))]
  if len(funcs)!=1 or funcs[0]['value']>s['value']:return False
  base=funcs[0]['value'];frva=fsize=None
  for frame in reversed(self.code_frames):
   if self.obj.symbols[frame['index']]['section']==s['section']:frva,fsize=frame['rva'],frame['size'];break
  if frva is None and parent is not None:
   if self.obj.root(s['section'])!=self.parent_root(parent):return False
   frva,fsize=parent,function_span(self.map,parent)['size']
  if frva is None:return False
  offset=((s['value']+addend)&MASK)-base
  if not 0<=offset<fsize:self.fail('local label %s lies outside its function row 0x%X'%(s['name'],frva),True)
  expected=(frva+offset)&MASK
  if target!=expected:self.fail('local label %s target 0x%X, expected 0x%X'%(s['name'],target,expected),True)
  catch=bool(s.get('type',0)&32)
  if catch and not any(int(r['parent'],16)==frva and r.get('kind')=='FuncInfo-catch-action' for r in self.symbols.get('eh_parents',{}).get('0x%08X'%expected,[])):
   self.fail('catch block %s at 0x%X has no retail FuncInfo-catch-action record for 0x%X'%(s['name'],expected,frva))
  self.bind(index,(frva+s['value']-base)&MASK)
  self.proofs.append({'kind':'catch-block' if catch else 'local-label','name':s['name'],'rva':'0x%08X'%expected,'function':'0x%08X'%frva,'offset':offset,'container':container})
  return True
 def switch_reference(self,index,target,addend,parent):
  symbol=self.obj.symbols[index]
  if parent is None:return False
  if symbol.get('storage')!=3 or symbol.get('type',0)&32 or symbol['section']<=0:return False
  # $L compiler labels are scoped to this COFF function, not global map identities.
  known=() if symbol['name'].startswith('$L') else self.names.get(symbol['name'],())
  if known and any(self.rows[a]['role']!='jumptable' or self.rows[a].get('parent')!='0x%08X'%parent for a in known):return False
  if any(c.get('proven') for c in self.named.get(symbol['name'],{}).get('candidates',[])):return False
  if not (symbol['name'].startswith('$L') or symbol['name']==self.obj.sections[symbol['section']-1]['name']):return False
  for frame in reversed(self.code_frames):
   if frame['rva']!=parent or not frame['tables']:continue
   root=self.obj.symbols[frame['index']]
   if symbol['section']!=root['section']:continue
   offset=((symbol['value']+addend)&MASK)-root['value']
   if not 0<=offset<frame['size']:continue
   expected=(frame['rva']+offset)&MASK
   if known and ((expected-addend)&MASK) not in known:self.fail('wrong mapped jump-table identity',True)
   if target!=expected:self.fail('switch-local target differs: 0x%X expected 0x%X'%(target,expected),True)
   self.bind(index,(frame['rva']+symbol['value']-root['value'])&MASK)
   self.proofs.append({'kind':'switch-local-reference','name':symbol['name'],'rva':'0x%08X'%target,'parent':'0x%08X'%parent,'offset':offset});return True
  return False
 def reference(self,index,typ,addend,operand,place,body,off,parent,container):
  s=self.obj.symbols[index];name=s['name']
  definitions=[i for i in self.obj.names.get(name,[]) if self.obj.symbols[i]['section']>0]
  if s['section']==0 and definitions:
   if len(definitions)!=1:self.fail('multiple compiled definitions of '+name)
   return self.reference(definitions[0],typ,addend,operand,place,body,off,parent,container)
  preferred_known=bool(self.names.get(name)) or any(c.get('proven') for c in self.named.get(name,{}).get('candidates',[]))
  if index in self.obj.weak and not preferred_known:
   default,policy=self.obj.weak[index];target=self.obj.symbols[default]
   # VC7.1 emits a library-search weak vector-dtor reference with a scalar-dtor default.
   # Only a retail-identified vtable slot supplies selection evidence; ordinary weak references stay unknown.
   if policy!=2 or container!='vtable-slots' or not name.startswith('??_E') or target['name']!='??_G'+name[4:]:self.fail('weak external selection has no verified retail vtable/default contract: '+name)
   if default==index:self.fail('cyclic COFF weak external',True)
   self.proofs.append({'kind':'coff-deleting-dtor-default','preferred':name,'default':target['name'],'slot_rva':'0x%08X'%place,'search_policy':policy})
   return self.reference(default,typ,addend,operand,place,body,off,parent,container)
  if name=='__except_list':
   # The x86 linker absolute is VA zero. It is valid only as an FS segment operand, never generic data.
   fs=(off>=2 and body[off-2:off] in (b'\x64\xa1',b'\x64\xa3')) or (off>=3 and body[off-3]==0x64 and body[off-2] in (0x8b,0x89) and body[off-1]&0xc7==5)
   if typ!=6 or addend!=0 or operand!=0 or not fs:self.fail('__except_list requires an exact fs:[0] MOV operand',True)
   self.proofs.append({'kind':'special','name':name,'place':'0x%08X'%place});return
  if name in ('___ImageBase','__ImageBase'):
   if typ!=6 or operand!=(self.pe.base+addend)&MASK:self.fail('wrong __ImageBase operand',True)
   return
  if s['section']==-1:
   expected=(s['value']+addend)&MASK
   if typ not in (6,7) or operand!=expected:self.fail('absolute COFF symbol value differs: '+name,True)
   return
  target=(operand-self.pe.base if typ==6 else operand if typ==7 else place+4+struct.unpack('<i',struct.pack('<I',operand))[0])&MASK
  rva=(target-addend)&MASK
  # an operand that is no address in the image cannot be this reference (an immediate, not an image address).
  if not (self.in_image(target) or self.in_image(rva)):self.fail('retail operand at 0x%X is outside the image (0x%X): '%(place,operand)+name,True)
  if self.switch_reference(index,target,addend,parent):return
  if self.local_label(index,target,addend,parent,container):return
  if name.startswith('??_7'):
   self.vtable(index,rva,parent);return
  if s['section']>0 and name.startswith('$L') and self.code_loc(index):self.code(index,rva,parent);return
  if s['section']>0 and name.startswith('$T') and self.obj.sections[s['section']-1]['name'].startswith('.xdata') and container=='code':self.funcinfo(index,rva,parent);return
  # _$E<n> is numbered per object: every object's _$E2 shares the name. It never binds by name; a defined one is
  # bound below by retail's operand at this site (code()), and an undefined one cannot occur.
  if s['section']<=0 and name.startswith('__imp_'):
   alias=self.import_alias_slots().get(rva,False)
   if alias is None:self.fail('import slot 0x%X: the map\'s import thunks through it name different imports'%rva)
   if alias and alias.get('single'):
    want=pe_import_name(name)
    if want!=alias['own'] and want in alias['names']:
     self.bind(index,rva)
     self.proofs.append({'kind':'pe-import-alias','name':name,'rva':'0x%08X'%rva,'import':alias['import'],'thunk':None,'thunk_name':None,'class':sorted(alias['names'])});return
    alias=False                                       # its own name, or another import: the import-directory rule decides
   if alias:
    if pe_import_name(name)!=alias['name']:
     self.fail('import slot 0x%X is the %s alias slot (map import thunk 0x%08X %s jumps through it; retail names it %s); compiled %s'%(rva,alias['name'],alias['thunk'],alias['thunk_name'],alias['import'],name),True)
    self.bind(index,rva)
    self.proofs.append({'kind':'pe-import-alias','name':name,'rva':'0x%08X'%rva,'import':alias['import'],'thunk':'0x%08X'%alias['thunk'],'thunk_name':alias['thunk_name']});return
  local_name=compiler_local(name)
  choices=set() if local_name else set(self.names.get(name,()))
  entry={} if local_name else self.named.get(name,{})
  if any(v.get('scope') or v.get('evidence_class')=='R' for v in entry.get('candidates',[])):
   entry=dict(entry,candidates=[v for v in entry['candidates'] if self.candidate_applies(v,s)])
  choices.update(int(v['rva'],16) for v in entry.get('candidates',[]) if v.get('proven'))
  if choices:
   if rva not in choices:self.fail('wrong named target '+name+': 0x%X expected %s'%(rva,','.join(hex(x) for x in sorted(choices))),True)
   proven=[v for v in entry.get('candidates',[]) if v.get('proven')]
   duplicate_import=bool(proven) and all(v['kind'] in ('iat','runtime-import-thunk') and v.get('import_identity') for v in proven) and len({v['import_identity'] for v in proven})==1 and not self.names.get(name)
   if len(choices)!=1 and not duplicate_import:self.fail('ambiguous name identity '+name)
   self.bind(index,rva)
   if s['section']>0:
    if self.code_loc(index):self.code(index,rva,parent)
    else:
     n=self.obj.extent(index)
     if n>0:self.named_data(index,rva,n,parent,name)
   self.proofs.append({'kind':'named','name':name,'rva':'0x%08X'%rva,'evidence':','.join(sorted({v['evidence_class'] for v in entry.get('candidates',[]) if v.get('proven')})) or 'map-row'});return
  if s['section']<=0 and name.startswith('__imp_'):
   # The retail PE's own import directory names every IAT slot (DLL and function): an import is proven when the slot
   # retail's operand points at carries the compiled declaration's name, and wrong when it carries another import.
   slot=getattr(self.pe,'imports',{}).get(rva)
   if slot is not None and slot.get('name'):
    if slot['name']==pe_import_name(name):
     self.bind(index,rva);self.proofs.append({'kind':'pe-import-directory','name':name,'rva':'0x%08X'%rva,'import':'%s!%s'%(slot['dll'],slot['name'])});return
    self.fail('import slot 0x%X is %s!%s; compiled %s'%(rva,slot['dll'],slot['name'],name),True)
  if s['section']<=0:
   candidates=entry.get('candidates',[])
   self.fail('unproven external '+name+': '+ ('; '.join(v.get('why','hypothesis only') for v in candidates) if candidates else 'no mapped/import/reference-proven identity'))
  if self.code_loc(index):
   # Local funclets and code emitted under placeholder map names are checked by identity and associative parent.
   self.code(index,rva,parent);return
  loc=self.obj.loc(index);section=self.obj.sections[s['section']-1]
  if section['name'].startswith('.xdata') and name.startswith('$T') and container=='code':
   self.funcinfo(index,rva,parent);return
  literal=name.startswith(('??_C@','__real@','__xmm@','$SG','??_R0'))
  if literal:
   # A literal has initialized contents: retail's operand never points into a section's zero-filled tail (.bss),
   # however equal the bytes look there (s04 2026-11-17: INI's "" bound to a 1-byte .bss object at 0x009DF5B8).
   extent=max(self.obj.extent(index),1);target_section=self.pe.section(rva,extent)
   if 'raw_size' in target_section and rva+extent>target_section['rva']+target_section['raw_size']:
    self.fail('literal %s bound to zero-filled (uninitialized) retail data at 0x%X'%(name,rva),True)
  local_readonly=s.get('storage')==3 and not section['characteristics']&0x80000000
  local_static=parent is not None and any(n and n in name for n in self.rows.get(parent,{}).get('accepted_names',[]))
  readonly=not section['characteristics']&0x80000000 and not section['characteristics']&32
  content_readonly=False
  if readonly and not (literal or local_readonly or local_static):
   n=self.obj.extent(index)
   target_section=self.pe.section(rva,n)
   flags=target_section['flags']
   if flags&0x20000020:self.fail('read-only content target is executable: '+name,True)
   # Retail game.dat's .rdata carries the write flag (0xC0000040); only another writable section is mutable data.
   if flags&0x80000000 and target_section.get('name')!='.rdata':self.fail('read-only content target is writable: '+name,True)
   content_readonly=True
  if not (literal or local_readonly or local_static or content_readonly):self.fail('named data has no proven identity: '+name)
  if local_static:
   # A parent-encoded compiler local must have only that parent's references (including its funclets).
   item=self.data_items.get(rva)
   allowed={parent}|{a for a,r in self.rows.items() if r.get('parent') and int(r['parent'],16)==parent}
   refs=set(int(x,16) for x in re.findall(r'0x[0-9A-Fa-f]+',item.get('referenced_by_rvas',''))) if item else set()
   if not refs or not refs<=allowed:self.fail('mutable local has no exclusive parent-reference evidence: '+name)
  self.bind(index,rva)
  n=self.obj.extent(index)
  if n<=0:self.fail('anonymous data has no bounded compiled extent: '+name)
  self.block(loc,rva,n,parent,'literal' if literal else 'parent-local' if local_static else 'anonymous-data')
  if content_readonly:self.proofs.append({'kind':'content-proven-readonly','name':name,'rva':'0x%08X'%rva,'size':n,'identity':'content/reference binding; spelling not established'})
  if '0x%08X'%rva in self.symbols.get('typed_tables',{}):self.proofs.append({'kind':'binary-parser-table','rva':'0x%08X'%rva,'types':self.symbols['typed_tables']['0x%08X'%rva]})
 def require_retail_code(self,rva,context):
  if not self.pe.section(rva,1)['flags']&32:self.fail(context+' points at non-code retail memory',True)
 def require_code_pointer(self,index,context):
  sym=self.obj.symbols[index]
  if sym['section']>0:
   if not self.code_loc(index):self.fail(context+' points at a non-code COFF section',True)
  elif sym['section']==0:
   if not sym.get('type',0)&32:self.fail(context+' unresolved target is not a function symbol',True)
  else:self.fail(context+' points at an absolute/data symbol',True)
 def vtable(self,index,rva,parent):
  s=self.obj.symbols[index];name=s['name'];candidates=self.tables.get(name,[])
  allowed={int(v['rva'],16) for v in candidates}
  if candidates and rva not in allowed:self.fail('wrong vtable target '+name+' at 0x%X'%rva,True)
  # By content when the map does not bind this name to exactly one table (no entry: often identical tables the
  # linker folded under another class's name; several entries): retail's table at the operand's address is
  # compared slot by slot with the compiled one, every slot target through the same resolver. Its length is the
  # one listed for that address. A compiled table of another length is wrong (reference comparison: the
  # length is part of the table; batch5 left it unproven, which credited the constructor without a slot comparison).
  # Only where the map leaves the listed extent open (open_length) is a length it allows unproven: the compiled
  # slots are still compared against retail's, and a wrong slot is wrong.
  content=not candidates or len(allowed)!=1
  if content:
   sizes=self.table_sizes.get(rva,set())
   if len(sizes)!=1:self.fail(('vtable has no unambiguous class mapping: ' if not candidates else 'multiple class vtable locations need primary/secondary identity: ')+name)
  if s['section']<=0:self.fail('compiled vtable is undefined; cannot check its slots: '+name)
  listed=next(iter(sizes))*4 if content else int(next(v for v in candidates if int(v['rva'],16)==rva)['slots'])*4
  size=listed;open_note=None;compiled=self.obj.extent(index)
  if compiled!=listed:
   why=('vtable length %d differs from the retail table listed at 0x%X (%d slots): %s'%(compiled//4,rva,listed//4,name) if content else
        'vtable slot count differs for %s (compiled %d, listed %d at 0x%X)'%(name,compiled//4,listed//4,rva))
   if compiled%4 or not self.open_length(rva,compiled//4):self.fail(why,True)
   open_note=why+'; the map leaves this extent open (%s)'%self.table_open[rva][0];size=compiled
  loc=self.obj.loc(index);rel=self.obj.relocs(loc,size)
  for offset in range(0,size,4):
   found=[(o,t,j) for o,t,j in rel if o==offset and t!=0]
   if len(found)!=1 or found[0][1]!=6:self.fail('vtable slot has no exact DIR32 function relocation',True)
   self.require_code_pointer(found[0][2],'vtable slot')
   self.require_retail_code(U(self.pe.read(rva+offset,4))-self.pe.base,'vtable slot')
  self.bind(index,rva)
  try:self.block(loc,rva,size,parent,'vtable-slots')
  except Mismatch as exc:
   if '[vtable ' in str(exc):raise                    # already named by an inner table
   # Name the table and slot: the work lists turn this into 'declare the override retail has in slot k'.
   raise Mismatch('%s [vtable %s at 0x%X slot %s]'%(exc,name,rva,self.slot_of(loc,rva,size,str(exc))))
  if content:self.proofs.append({'kind':'vtable-content','rva':'0x%08X'%rva,'compiled':name,'slots':size//4,'listed_as':sorted(self.table_names.get(rva,()))})
  if open_note:self.fail(open_note)          # slots compared and equal; the length itself stays unproven
 def slot_of(self,loc,rva,size,why):
  """The slot index a vtable mismatch refers to (by the compiled target name and retail address it names), or '?'."""
  m=re.match(r'(?:dependency: callee )?wrong named target (\S+): 0x([0-9A-Fa-f]+)',why)
  if m:compiled,retail=m.group(1),int(m.group(2),16)
  else:
   m=re.match(r'(?:dependency: callee )?code target identity at 0x([0-9A-Fa-f]+): compiled as (\S+?)[;:]',why)
   if not m:return '?'
   compiled,retail=m.group(2),int(m.group(1),16)
  for off,typ,j in self.obj.relocs(loc,size):
   if typ and self.obj.symbols[j]['name']==compiled and (U(self.pe.read(rva+off,4))-self.pe.base)&MASK==retail:return str(off//4)
  return '?'
 def funcinfo(self,index,rva,parent):
  if parent is None:self.fail('FuncInfo has no identified parent')
  loc=self.obj.loc(index)
  if self.obj.root(loc[0])!=self.parent_root(parent):self.fail('FuncInfo belongs to a different COFF parent',True)
  cb=self.obj.bytes(loc,28);tb=self.pe.read(rva,28)
  if U(cb)!=0x19930520 or U(tb)!=0x19930520:self.fail('wrong/unsupported VC7.1 FuncInfo magic',True)
  for off in (0,4,12,20):
   if U(cb,off)!=U(tb,off):self.fail('FuncInfo field 0x%X differs'%off,True)
  states,tries,ips=U(cb,4),U(cb,12),U(cb,20)
  if states>4096 or tries>512 or ips>4096:self.fail('FuncInfo count exceeds structural bound')
  self.bind(index,rva)
  for off,count,width,kind in [(8,states,8,'unwind-map'),(16,tries,20,'try-map'),(24,ips,8,'ip-state-map')]:
   reloc=[(o,k,j) for o,k,j in self.obj.relocs(loc,28) if o==off and k!=0]
   if not count:
    if reloc or U(cb,off)!=U(tb,off):self.fail('non-null pointer for empty FuncInfo map at 0x%X'%off,True)
    continue
   if len(reloc)!=1 or reloc[0][1]!=6:self.fail('FuncInfo map pointer has no exact DIR32 relocation')
   target=U(tb,off)-self.pe.base
   if target<0:self.fail('FuncInfo map pointer outside image',True)
   self.table(reloc[0][2],U(cb,off),target,count,width,kind,parent)
  self.proofs.append({'kind':'funcinfo','rva':'0x%08X'%rva,'states':states,'try_blocks':tries,'parent':'0x%08X'%parent})
 def table(self,index,addend,rva,count,width,kind,parent):
  self.bind(index,rva-addend);loc=self.obj.loc(index,addend)
  if self.obj.root(loc[0])!=self.parent_root(parent):self.fail(kind+' belongs to wrong COFF parent',True)
  if kind in ('unwind-map','handler-array'):
   for k in range(count):
    at=(loc[0],loc[1]+k*width);rel=[(o,t,j) for o,t,j in self.obj.relocs(at,width) if t!=0]
    codeoff=4 if kind=='unwind-map' else 12
    for off,typ,target in rel:
     if off==codeoff:
      if typ!=6:self.fail(kind+' action must use DIR32',True)
      self.require_code_pointer(target,kind+' action')
      self.require_retail_code(U(self.pe.read(rva+k*width+codeoff,4))-self.pe.base,kind+' action')
     elif not (kind=='handler-array' and off==4 and typ==6):self.fail(kind+' has a relocation in a scalar field',True)
   self.block(loc,rva,count*width,parent,kind);return
  if kind!='try-map':self.block(loc,rva,count*width,parent,kind);return
  for k in range(count):
   at=(loc[0],loc[1]+k*20);cb=self.obj.bytes(at,20);tb=self.pe.read(rva+k*20,20)
   if cb[:16]!=tb[:16]:self.fail('try-block state/count fields differ',True)
   catches=U(cb,12)
   if catches>512:self.fail('handler array count exceeds bound')
   rel=[(o,t,j) for o,t,j in self.obj.relocs(at,20) if t!=0]
   if catches:
    if len(rel)!=1 or rel[0][:2]!=(16,6):self.fail('handler array pointer relocation invalid')
    self.table(rel[0][2],U(cb,16),U(tb,16)-self.pe.base,catches,16,'handler-array',parent)
   elif rel or cb[16:]!=tb[16:]:self.fail('empty try block has different handler pointer',True)
 def verify(self,index,rva,parent=None,kind='code',size=None):
  start=len(self.proofs);saved=(dict(self.bindings),dict(self.reverse),set(self.done),self.nodes);mark=len(self.unproven)
  try:
   if kind=='code':self.code(index,rva,parent)
   elif kind=='fold-member':self.code(index,rva,parent,member=True)
   elif kind=='funcinfo':self.funcinfo(index,rva,parent)
   elif kind=='vtable':self.vtable(index,rva,parent)
   else:self.block(self.obj.loc(index),rva,size or self.obj.extent(index),parent,kind)
   notes=self.unproven[mark:];del self.unproven[mark:]
   if notes:return {'status':'UNVERIFIED_RELOCATION','why':notes[0]+('' if len(notes)==1 else ' (+%d more unproven)'%(len(notes)-1)),'unproven':notes[:20],'proofs':self.proofs[start:]}
   return {'status':'MATCH','proofs':self.proofs[start:]}
  except (Unverified,Mismatch,ValueError,struct.error,KeyError,IndexError) as exc:
   del self.unproven[mark:]
   self.bindings,self.reverse,self.done,self.nodes=saved
   return {'status':'WRONG_RELOCATION' if isinstance(exc,Mismatch) else 'UNVERIFIED_RELOCATION','why':str(exc),'proofs':self.proofs[start:]}
