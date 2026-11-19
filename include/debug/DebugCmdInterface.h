// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DebugCmdInterface.h: a debug command group interface (Zero Hour's debug_cmd.h). Target facts (retail game.dat): the
// table of DebugCmdInterfaceDebug (0x007BE8C8) holds the deleting destructor 0x00039950, Execute 0x0003E570 and the shared
// Delete thunk 0x000386C0.
#pragma once
#ifndef BFME2_DEBUGCMDINTERFACE_H
#define BFME2_DEBUGCMDINTERFACE_H

class Debug;

class DebugCmdInterface
{
	DebugCmdInterface(const DebugCmdInterface &);
	DebugCmdInterface &operator=(const DebugCmdInterface &);

protected:
	virtual ~DebugCmdInterface() {}										// slot 0

public:
	explicit DebugCmdInterface() {}

	enum CommandMode
	{
		Normal,
		Structured,
		MAX
	};

	virtual bool Execute(Debug &dbg, const char *cmd, CommandMode cmdmode, unsigned int argn, const char *const *argv) = 0;	// slot 1
	virtual void Delete() = 0;											// slot 2
};

#endif
