// CitadelSlaughterHordeContain.h: the CitadelSlaughterHordeContain module. SIZE 0x9F0 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0048059F calls SlaughterHordeContain first; it stores the
// tables 0x00C48CC0 at +0x0, 0x00C43528 at +0xC, 0x00C48CB0 at +0x10, 0x00C48B38 at +0x20, 0x00C46064 at +0x24, 0x00C48910 at +0x28, 0x00C463D8 at +0x2C, 0x00C488E0 at +0x30, 0x00C48B28 at +0x34. Members are what xfer (0x0048030D) transfers after SlaughterHordeContain::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 2 -> 0x004805FA ?Rva004805FAGet@@YAHXZ
//   +0x0 slot 32 -> 0x00480E9A ?rva00480E9A@CitadelSlaughterHordeContain@@UAEXPAVObject@@@Z
//   +0x0 slot 33 -> 0x004807C7 ?rva004807C7@CitadelSlaughterHordeContain@@UAE_NPAVObject@@@Z
//   +0x10 slot 0 -> 0x00480FE2 ?update@CitadelSlaughterHordeContain@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 38 -> 0x0048071D ?isValidContainerFor@CitadelSlaughterHordeContain@@UAE_NPAVObject@@_N1@Z
// Not established: transfers xfer makes that this view cannot place: ?xfer@SlaughterHordeContain@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_CITADEL_SLAUGHTER_HORDE_CONTAIN_H
#define BFME2_CITADEL_SLAUGHTER_HORDE_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SlaughterHordeContain.h"

class Thing;
class ModuleData;
class Xfer;

// SlaughterHordeContain ends at 0x9EC; this class's members follow it.
typedef char CitadelSlaughterHordeContain_base_size[(sizeof(SlaughterHordeContain) == 0x9EC) ? 1 : -1];

class CitadelSlaughterHordeContain : public SlaughterHordeContain
{
public:
	CitadelSlaughterHordeContain(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00480FE2)
	virtual Bool isValidContainerFor(Object *obj, Bool checkCapacity, Bool flag);	// override: isValidContainerFor slot 38 at +0x20 (0x0048071D)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	ObjectID m_unk0x9EC;                           // +0x9EC; xfer ObjectID
};
// SIZE 0x9F0
typedef char CitadelSlaughterHordeContain_size[(sizeof(CitadelSlaughterHordeContain) == 0x9F0) ? 1 : -1];
typedef char CitadelSlaughterHordeContain_off_m_unk0x9EC[(offsetof(CitadelSlaughterHordeContain, m_unk0x9EC) == 0x9EC) ? 1 : -1];

#endif
