/* msvcr71's per-handle record, the "semi-documented" structure STLport 4.6's src/fstream.cpp walks through __pioinfo to
 * read a file descriptor's flags (the body upstream declares inside the .cpp). Layout: osfhnd +0, osfile +4, pipech +5,
 * then, in the multithreaded CRT, lockinitflag +8 and the 24-byte critical section (size 0x24). */
#ifndef STLPORT_SHIM_IOINFO_H
#define STLPORT_SHIM_IOINFO_H

extern "C" {
  struct ioinfo {
    long osfhnd;    // the real os HANDLE
    char osfile;    // file handle flags
    char pipech;    // pipe buffer
#   ifdef _MT
    // multi-threaded locking
    int lockinitflag;
    CRITICAL_SECTION lock;
#   endif  /* _MT */
  };
}

#endif
