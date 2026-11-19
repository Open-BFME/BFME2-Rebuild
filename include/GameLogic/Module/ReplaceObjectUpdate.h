// ReplaceObjectUpdate.h: the ReplaceObjectUpdate module, a SpecialAbilityUpdate that adds no member.
// Target facts (retail game.dat): the constructor 0x004B2987 calls SpecialAbilityUpdate's constructor
// first and stores its own tables; SIZE 0x88 is exactly SpecialAbilityUpdate's size. getModuleNameKey
// 0x004B29FF interns the name "ReplaceObjectUpdate" through the pool key (static at 0x00E03DA0).
// GetSnapshotName and the trigger slot are the map's rows; xfer 0x004B29DE is not yet written.
#pragma once
#ifndef BFME2_REPLACEOBJECTUPDATE_H
#define BFME2_REPLACEOBJECTUPDATE_H

#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

class ReplaceObjectUpdate : public SpecialAbilityUpdate
{
public:
	ReplaceObjectUpdate(Thing *thing, const ModuleData *moduleData);	// 0x004B2987
	virtual NameKeyType getModuleNameKey() const;						// 0x004B29FF

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);										// 0x004B29DE
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
// SIZE 0x88
typedef char ReplaceObjectUpdate_size[(sizeof(ReplaceObjectUpdate) == 0x88) ? 1 : -1];

#endif
