// OathbreakersFadeAwayBehavior.h: promoted from include/Draft by h05 (round 10). Proven by 3 rows that MATCH against it on main (0x004B89DD, 0x004B8A22, 0x004B8A28);
// name, size, bases and virtual order are retail facts given below.
// OathbreakersFadeAwayBehavior.h: the OathbreakersFadeAwayBehavior module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B8A8A calls UpdateModule first; it stores the
// tables 0x00C5928C at +0x0, 0x00BEFF90 at +0xC, 0x00C59280 at +0x10. Members are what xfer (0x004B8A28) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 2 -> 0x004B8A22 ?name@OathbreakersFadeAwayBehavior@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004B8B41 ?update@OathbreakersFadeAwayBehavior@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_OATHBREAKERS_FADE_AWAY_BEHAVIOR_H
#define BFME2_OATHBREAKERS_FADE_AWAY_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char OathbreakersFadeAwayBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class OathbreakersFadeAwayBehavior : public UpdateModule
{
public:
	char const * name() const;	// 0x004B8A22
	OathbreakersFadeAwayBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~OathbreakersFadeAwayBehavior();

protected:
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x20;                                // +0x20; xfer Bool
	Real m_unk0x24;                                // +0x24; xfer Real
protected:
	virtual void loadPostProcess() {}	// retail slot: the shared empty body 0x000B3FD0 (h05, gen3 vtable-content list)
public:
	virtual UpdateSleepTime update();	// UpdateModuleInterface slot 0: 0x004B8B41
};
// SIZE 0x28
typedef char OathbreakersFadeAwayBehavior_size[(sizeof(OathbreakersFadeAwayBehavior) == 0x28) ? 1 : -1];
typedef char OathbreakersFadeAwayBehavior_off_m_unk0x20[(offsetof(OathbreakersFadeAwayBehavior, m_unk0x20) == 0x20) ? 1 : -1];
typedef char OathbreakersFadeAwayBehavior_off_m_unk0x24[(offsetof(OathbreakersFadeAwayBehavior, m_unk0x24) == 0x24) ? 1 : -1];

#endif
