"""Read-only per-snapshot indexes, shared by forked file workers. No proof decisions."""
import re
_ANON=re.compile(r"\?A0x[0-9A-Fa-f]{8}")
_MAP=None;_MAP_INDEX=None;_SYMBOLS=None;_SYMBOL_INDEX=None

def map_index(document):
 global _MAP,_MAP_INDEX
 if _MAP is document:return _MAP_INDEX
 rows={int(row['rva'],16):row for row in document['rows']};names={};accepted={};by_object={};objects={};normal={};tables={};sections={}
 for address,row in rows.items():
  sections.setdefault(row.get('section'),[]).append(row)
  if row.get('role')=='jumptable' and row.get('parent'):tables.setdefault(int(row['parent'],16),[]).append(row)
  accepted[address]=frozenset(row['accepted_names']);by_object.setdefault(row.get('object'),[]).append(row)
  for name in row['accepted_names']:
   names.setdefault(name,set()).add(address);normal.setdefault(_ANON.sub('?A0xHASH',name),set()).add(address)
 for values in by_object.values():values.sort(key=lambda r:int(r['rva'],16))
 for obj in document.get('objects',[]):objects.setdefault(obj.get('tu'),[]).append(obj)
 for values in sections.values():values.sort(key=lambda r:int(r['rva'],16))
 index={'tables':tables,'sections':sections,'rows':rows,'names':{k:frozenset(v) for k,v in names.items()},'normalized_names':{k:frozenset(v) for k,v in normal.items()},'accepted_names':accepted,'by_object':by_object,'objects':objects}
 _MAP_INDEX=index;_MAP=document;return index

def symbol_index(document):
 global _SYMBOLS,_SYMBOL_INDEX
 if _SYMBOLS is document:return _SYMBOL_INDEX
 index={'data_items':{int(r['rva'],16):r for r in document.get('data_items',[])}}
 _SYMBOL_INDEX=index;_SYMBOLS=document;return index

def function_span(document,address):
 index=map_index(document);row=index['rows'][address];end=address+row['size'];code_size=row['size'];used=[];problems=[]
 tables=sorted(index['tables'].get(address,[]),key=lambda r:int(r['rva'],16))
 for table in tables:
  if table.get('object')!=row.get('object'):problems.append('jump table '+table['rva']+' belongs to a different object');continue
  if table.get('section')!=row.get('section'):continue
  start=int(table['rva'],16)
  if start<end:problems.append('jump table '+table['rva']+' overlaps its mapped code extent');continue
  cursor=end
  for gap in index['sections'].get(row.get('section'),[]):
   lo=int(gap['rva'],16)
   if lo<cursor:continue
   if lo>=start:break
   if lo!=cursor or gap['role']!='padding':break
   cursor=lo+gap['size']
  if cursor!=start:problems.append('unproven gap before jump table '+table['rva']);continue
  end=start+table['size'];used.append(table)
 return {'size':end-address,'code_size':code_size,'tables':tables,'inline_tables':used,'problems':problems}
