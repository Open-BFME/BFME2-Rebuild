// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DebugIOInterface.h: the debug library's I/O interface (Zero Hour's debug_io.h). Target facts (retail game.dat): its own
// vftable 0x007C0990 holds seven slots: 0 the deleting destructor 0x0003F340, 1 Read, 3 Write, 4 EmergencyFlush, 5 Execute
// and 6 Delete are __purecall (0x0003B810), 2 is `xor al, al; ret` (0x0073B660), a virtual Zero Hour lacks: DebugIOCon
// overrides it with a getter of its console-allocated flag (0x000408C0), which is the gate Zero Hour's DebugIOCon::Read
// applies, so the slot is named CanRead here (structural: role from that override). The string types: Write 0x0003F360
// compares the type with 5 for StructuredCmdReply, Debug's idle type is 7 (MAX) and its Log is 1, so BFME 2 has no Check
// type (Zero Hour has Assert, Check, Log, ...).
#pragma once
#ifndef BFME2_DEBUGIOINTERFACE_H
#define BFME2_DEBUGIOINTERFACE_H

class Debug;

class DebugIOInterface
{
public:
	enum StringType
	{
		Assert = 0,
		Log,
		Crash,
		Exception,
		CmdReply,
		StructuredCmdReply,
		Other,
		MAX
	};

	explicit DebugIOInterface() {}
	virtual ~DebugIOInterface() {}											// slot 0: 0x0003F340

	virtual int Read(char *buf, int maxchar) = 0;							// slot 1
	virtual bool CanRead() { return false; }								// slot 2: 0x0073B660
	virtual void Write(StringType type, const char *src, const char *str) = 0;	// slot 3
	virtual void EmergencyFlush() = 0;										// slot 4
	virtual void Execute(Debug &dbg, const char *cmd, bool structuredCmd, unsigned int argn, const char *const *argv) = 0;	// slot 5
	virtual void Delete() = 0;												// slot 6
};

#endif
