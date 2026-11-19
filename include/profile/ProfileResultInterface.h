// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ProfileResultInterface.h: a profile result function (Zero Hour's profile_result.h). Target facts (retail game.dat):
// ProfileCmdInterface::RunResultFunctions 0x006C6E30 calls slot 0 (WriteResults) and slot 1 (Delete) of each instance; the
// concrete classes' tables (0x008E84D0 DOT, 0x008E85D8 GTT, 0x008E85E4 CSV) hold WriteResults, the shared Delete thunk
// 0x006C74C0 and GetName.
#pragma once
#ifndef BFME2_PROFILERESULTINTERFACE_H
#define BFME2_PROFILERESULTINTERFACE_H

class ProfileResultInterface
{
	ProfileResultInterface(const ProfileResultInterface &);
	ProfileResultInterface &operator=(const ProfileResultInterface &);

public:
	virtual void WriteResults() = 0;									// slot 0
	virtual void Delete() = 0;											// slot 1
	virtual const char *GetName() const = 0;							// slot 2

protected:
	ProfileResultInterface() {}
};

#endif
