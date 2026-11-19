// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DebugIOOds.h: the debug I/O class that writes to the debugger (OutputDebugString; Zero Hour's debug_io_ods.cpp). Target
// facts (retail game.dat): vftable 0x007C09AC; the object is its vptr only (Create 0x0003F3D0 allocates 4 bytes through
// DebugAllocMemory); slot 1 Read is the shared `xor eax, eax; ret 8` stub 0x0003FD80, slot 4 EmergencyFlush the shared
// `ret` 0x0069E440, slot 5 Execute the shared `ret 0x14` stub 0x00743080 and slot 6 Delete the shared thunk 0x000386C0
// (`this->~DebugIOOds(); DebugFreeMemory(this)`).
#pragma once
#ifndef BFME2_DEBUGIOODS_H
#define BFME2_DEBUGIOODS_H

#include "debug/DebugIOInterface.h"

void DebugFreeMemory(void *ptr);

class DebugIOOds : public DebugIOInterface
{
public:
	DebugIOOds();																// 0x0003F380
	virtual ~DebugIOOds();														// slot 0: 0x0003F430 (deleting), body 0x0003F460

	virtual int Read(char *buf, int maxchar);									// slot 1: 0x0003FD80 (defined in debug_io_flat.cpp, the unit that holds the shared stub)
	virtual void Write(StringType type, const char *src, const char *str);		// slot 3: 0x0003F360
	virtual void EmergencyFlush() {}											// slot 4: 0x0069E440
	virtual void Execute(Debug &dbg, const char *cmd, bool structuredCmd, unsigned int argn, const char *const *argv) {}	// slot 5: 0x00743080
	virtual void Delete()														// slot 6: 0x000386C0
	{
		this->~DebugIOOds();
		DebugFreeMemory(this);
	}

	static DebugIOInterface *Create();											// 0x0003F3D0
};

#endif
