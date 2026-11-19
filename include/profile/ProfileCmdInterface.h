// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ProfileCmdInterface.h: the profile library's debug command group (Zero Hour's internal_cmd.h). Target facts (retail
// game.dat): the registered factory list is resIf 0x00A0C630 / numResIf 0x00A0C634 (12-byte records: factory function, name,
// argument text; AddResultFunction 0x006C67F0 reallocates it through ProfileReAllocMemory), the instance keeps the result
// function count at +4 and the array at +8 (RunResultFunctions 0x006C6E30).
#pragma once
#ifndef BFME2_PROFILECMDINTERFACE_H
#define BFME2_PROFILECMDINTERFACE_H

#include "debug/DebugCmdInterface.h"
#include "profile/ProfileResultInterface.h"

class ProfileCmdInterface : public DebugCmdInterface
{
	struct Factory
	{
		ProfileResultInterface *(*func)(int, const char *const *);
		const char *name;
		const char *arg;
	};

	static unsigned int numResIf;				// 0x00A0C634
	static Factory *resIf;						// 0x00A0C630

	unsigned int numResFunc;					// +0x04
	ProfileResultInterface **resFunc;			// +0x08

public:
	ProfileCmdInterface() : numResFunc(0), resFunc(0) {}

	static void AddResultFunction(ProfileResultInterface *(*func)(int, const char *const *), const char *name, const char *arg);	// 0x006C67F0
	void RunResultFunctions();					// 0x006C6E30
	virtual bool Execute(Debug &dbg, const char *cmd, CommandMode cmdmode, unsigned int argn, const char *const *argv);	// 0x006C68C0
	virtual void Delete() {}
};

#endif
