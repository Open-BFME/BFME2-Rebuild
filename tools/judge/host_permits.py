"""Host-wide local checker capacity. Cache lookups need no permit."""
import os,time,fcntl,errno
from pathlib import Path
from checker_configuration import STATE

def host_slots():
 value=os.environ.get('JUDGE_HOST_SLOTS',os.environ.get('LAND_JUDGE_SLOTS','20'))
 try:count=int(value)
 except ValueError:raise ValueError('JUDGE_HOST_SLOTS must be a positive integer')
 if count<1:raise ValueError('JUDGE_HOST_SLOTS must be a positive integer')
 return count

class Permit:
 def __init__(self,root=None,kind='work',count=None):
  self.root=Path(root) if root is not None else STATE/'locks';self.kind=kind;self.file=None;self.wait_s=0;self.count=count
 def __enter__(self):
  count=self.count or host_slots();self.root.mkdir(parents=True,exist_ok=True);start=time.perf_counter();offset=os.getpid()%count
  while True:
   for k in range(count):
    f=(self.root/('host-judge-'+self.kind+'-'+str((offset+k)%count))).open('a+')
    try:fcntl.flock(f,fcntl.LOCK_EX|fcntl.LOCK_NB)
    except OSError as error:
     f.close()
     if error.errno not in (errno.EACCES,errno.EAGAIN):raise
    else:
     self.file=f;self.wait_s=round(time.perf_counter()-start,4);return self
   time.sleep(.1)
 def __exit__(self,*exc):
  if self.file:self.file.close();self.file=None

def local_runs():
 """Judge runs that may work on this host at once (each: the parsed map plus up to JUDGE_WORKERS forked workers)."""
 value=os.environ.get('JUDGE_LOCAL_RUNS')
 if value is None:return max(1,-(-host_slots()//3))
 try:count=int(value)
 except ValueError:raise ValueError('JUDGE_LOCAL_RUNS must be a positive integer')
 if count<1:raise ValueError('JUDGE_LOCAL_RUNS must be a positive integer')
 return count

def mem_available_mb(path='/proc/meminfo'):
 """(MemAvailable, MemTotal) in MB, or None where the kernel does not say."""
 try:
  info={l.split(':')[0]:int(l.split()[1])//1024 for l in open(path) if l.startswith(('MemAvailable:','MemTotal:'))}
  return info['MemAvailable'],info['MemTotal']
 except (OSError,KeyError,ValueError,IndexError):return None

def local_run(root=None,sleep=time.sleep,meminfo=mem_available_mb,max_wait_s=1800):
 """Take one of the host's local-run slots and keep it until the process exits (the flock'd file stays open), then wait
 while MemAvailable is under JUDGE_LOCAL_MIN_MEM_MB (default: 6 GB, or an eighth of the memory where there is less),
 at most MAX_WAIT_S. Called before a judge run loads config/map.json, so runs queued for this host hold ~30 MB, not the
 parsed map (the checker host 2026-11-18: 64 GB, 325 judge processes needed 111 GB). Returns the held permit."""
 permit=Permit(root,kind='run',count=local_runs()).__enter__()
 start=time.perf_counter()
 while time.perf_counter()-start<max_wait_s:
  m=meminfo()
  if m is None:break
  floor=int(os.environ.get('JUDGE_LOCAL_MIN_MEM_MB',min(6144,m[1]//8)))
  if m[0]>=floor:break
  sleep(2)
 permit.mem_wait_s=round(time.perf_counter()-start,4)
 return permit
