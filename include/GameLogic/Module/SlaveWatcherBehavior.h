// SlaveWatcherBehavior.h: the SlaveWatcherBehavior module. SIZE 0xA4 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004845CB calls UpdateModule first; it stores the
// tables 0x00C4A264 at +0x0, 0x00BEFF90 at +0xC, 0x00C4A258 at +0x10. Members are what xfer (0x00484673) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00484628 ?Rva00484628Get@@YAHXZ
//   +0x0 slot 14 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 16 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 17 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 27 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 29 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 43 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x10 slot 0 -> 0x004847A9 ?update@SlaveWatcherBehavior@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_SLAVE_WATCHER_BEHAVIOR_H
#define BFME2_SLAVE_WATCHER_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char SlaveWatcherBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class SlaveWatcherBehavior : public UpdateModule
{
public:
	SlaveWatcherBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004847A9)
	ObjectID getWatchedObjectID() const;	// 0x0030D377: the 4-byte getter of the first member at +0x20, folded with other classes' (the map spells it ?getDataLength@NetWrapperCommandMsg@@QAEIXZ); ObjectCreationUpgrade 0x004B463C reads the object it names (f03)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x20[0x84];
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0xA4
typedef char SlaveWatcherBehavior_size[(sizeof(SlaveWatcherBehavior) == 0xA4) ? 1 : -1];

#endif
