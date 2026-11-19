// cl: /O2 /Oy-
#include <string.h>
#include "debug/Debug.h"
// Tier A; object=O0003E450 evidence=W flags=align=A
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=proven:name
// The two heap wrappers the debug library hands the game: Win32 global heap,
// with the crash arm routed through the Debug manager at 0x009E0880 (slot
// +0x60 records the callsite, +0x6C CrashBegin, +0x38 operator<<, +0x4C
// CrashDone). The callsite recorder is the 33-byte fold at 0x00038790.
// FUNCTION: BFME2 0x0003E450 size=83 name=?DebugAllocMemory@@YAPAXI@Z evidence=N:ledger

extern "C" __declspec(dllimport) void *__stdcall GlobalAlloc(unsigned int flags, unsigned int bytes);
extern "C" __declspec(dllimport) void *__stdcall GlobalReAlloc(void *mem, unsigned int bytes, unsigned int flags);
extern "C" __declspec(dllimport) void *__stdcall GlobalFree(void *mem);
extern "C" __declspec(dllimport) unsigned int __stdcall GlobalSize(void *mem);

extern Debug *theDebug;

void *DebugAllocMemory(unsigned int numBytes)
{
	void *h = GlobalAlloc(0, numBytes);
	if (!h)
	{
		Debug::SkipNext(true);
		theDebug->SkipNext();
		(theDebug->CrashBegin(0, 0, 0) << "Debug mem alloc failed").CrashDone(true);
	}
	return h;
}

// FUNCTION: BFME2 0x0003E4B0 size=189 name=?DebugReAllocMemory@@YAPAXPAXI@Z evidence=N:ledger

void *DebugReAllocMemory(void *oldPtr, unsigned int newSize)
{
	// Windows doesn't like ReAlloc with NULL handle/ptr...
	if (!oldPtr)
		return newSize ? DebugAllocMemory(newSize) : 0;

	// Shrinking to 0 size is basically freeing memory
	if (!newSize)
	{
		GlobalFree(oldPtr);
		return 0;
	}

	// now try GlobalReAlloc first
	void *h = GlobalReAlloc(oldPtr, newSize, 0);
	if (!h)
	{
		// this failed (Windows doesn't like ReAlloc'ing larger
		// fixed memory blocks) - go with Alloc/Free instead
		h = GlobalAlloc(0, newSize);
		if (!h)
		{
			Debug::SkipNext(true);
			theDebug->SkipNext();
			(theDebug->CrashBegin(0, 0, 0) << "Debug mem realloc failed").CrashDone(true);
		}
		unsigned int oldSize = GlobalSize(oldPtr);
		memcpy(h, oldPtr, oldSize < newSize ? oldSize : newSize);
		GlobalFree(oldPtr);
	}

	return h;
}
