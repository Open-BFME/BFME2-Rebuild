"""Explicit SDK/allocator inputs. No stand-in SDK fallback, no global source changes."""
import re
from pathlib import Path

def marker(text,name):return re.search(r'^// '+re.escape(name)+r'[ \t]*$',text,re.M) is not None
ALLOC_TOKEN='-Ireference/shims/bfmealloc_alloconly'
def allocator_requested(text,flags):return marker(text,'bfmealloc_alloconly') or ALLOC_TOKEN in flags

def sdk_options(text,vc,runner,wine_path,build):
 flags=[];roots=[]
 dx=marker(text,'dxsdk');platform=dx or marker(text,'psdk') or marker(text,'platformsdk')
 if dx:
  # Official August 2005 SDK, validated by its retained provenance manifest.
  directory=build.dxsdk_root()/'Include';flags.append('-I'+wine_path(directory));roots.append(directory)
 if platform:
  directory=vc/'Vc7/PlatformSDK/Include'
  if not directory.is_dir():raise ValueError('requested bundled Platform SDK is missing')
  flags.append('-I'+wine_path(directory));roots.append(directory)
 return flags,roots

def allocator_static_flags(flags):
 if '-U_STLP_USE_STATIC_LIB' in flags:raise ValueError('bfmealloc_alloconly requires _STLP_USE_STATIC_LIB')
 return [] if any(x.startswith('-D_STLP_USE_STATIC_LIB') for x in flags) else ['-D_STLP_USE_STATIC_LIB']


def optimization_hint(text,profile,base):
 flags=[]
 for line in text.splitlines():
  if line.startswith('// cl:'):flags.extend(line[6:].split())
 effective=None
 for flag in list(base)+flags:
  if flag.lower() in ('/o1','-o1','/o2','-o2','/od','-od','/ox','-ox'):effective='/'+flag[1:]
 fields=dict(x.split('=',1) for x in profile.split(';') if '=' in x)
 suggested='/O2' if fields.get('align')=='A' and fields.get('ehform')=='inline' else '/O1' if fields.get('align')=='U' and fields.get('ehform')=='__EH_prolog' else None
 if not suggested or effective==suggested:return None
 return {'suggested':suggested,'effective':effective,'evidence':profile,'action':'try the supported optimisation profile; hint only, no credit or flag changes'}
