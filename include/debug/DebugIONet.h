// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DebugIONet.h: the debug I/O class that talks to the remote debug console over a named pipe (Zero Hour's debug_io_net.cpp,
// internal_io.h). Target facts (retail game.dat): vftable 0x007C09C8; Create 0x0003F7B0 allocates 8 bytes through
// DebugAllocMemory, so the object is the vptr and the pipe handle at +4 (CloseHandle in the destructor 0x0003F4F0 is
// skipped when it is INVALID_HANDLE_VALUE). Slot 2 is the base's CanRead, slot 4 EmergencyFlush the shared `ret` 0x0069E440,
// slot 6 Delete the shared thunk 0x000386C0 (`this->~DebugIONet(); DebugFreeMemory(this)`).
#pragma once
#ifndef BFME2_DEBUGIONET_H
#define BFME2_DEBUGIONET_H

#include "debug/DebugIOInterface.h"

void DebugFreeMemory(void *ptr);

class DebugIONet : public DebugIOInterface
{
public:
	DebugIONet();																// 0x0003F4A0
	virtual ~DebugIONet();														// slot 0: 0x0003F810 (deleting), body 0x0003F4F0

	virtual int Read(char *buf, int maxchar);									// slot 1: 0x0003F550
	virtual void Write(StringType type, const char *src, const char *str);		// slot 3: 0x0003F5D0
	virtual void EmergencyFlush() {}											// slot 4: 0x0069E440
	virtual void Execute(Debug &dbg, const char *cmd, bool structuredCmd, unsigned int argn, const char *const *argv);	// slot 5: 0x0003F690
	virtual void Delete()														// slot 6: 0x000386C0
	{
		this->~DebugIONet();
		DebugFreeMemory(this);
	}

	static DebugIOInterface *Create();											// 0x0003F7B0

private:
	void *m_pipe;																// +4 (HANDLE)
};

#endif
