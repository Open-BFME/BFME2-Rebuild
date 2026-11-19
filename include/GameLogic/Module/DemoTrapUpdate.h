// DemoTrapUpdate.h: the DemoTrapUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004959B1 calls UpdateModule first; it stores the
// tables 0x00C4ED3C at +0x0, 0x00BEFF90 at +0xC, 0x00C4ED30 at +0x10. Members are what xfer (0x00495A74) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x0049592F ?name@Rva0049592FNamed@@QBEPBDXZ
//   +0x0 slot 5 -> 0x004959FF ?onObjectCreated@DemoTrapUpdate@@UAEXXZ
//   +0x0 slot 14 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 16 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 17 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 27 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 29 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 43 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x10 slot 0 -> 0x00495ABA ?update@DemoTrapUpdate@@UAE?AW4UpdateSleepTime@@XZ
#pragma once
#ifndef BFME2_DEMO_TRAP_UPDATE_H
#define BFME2_DEMO_TRAP_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char DemoTrapUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class DemoTrapUpdate : public UpdateModule
{
public:
	DemoTrapUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~DemoTrapUpdate();
	virtual void onObjectCreated();	// override: onObjectCreated slot 5 at +0x0 (0x004959FF)
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00495ABA)

	void rva00495A2B();	// 0x00495A2B (ledger: Rva00495A2B::rva00495A2B; fires the detonation weapon, kills the trap with damage type 8 and sets the +0x24 flag)

protected:
	virtual void xfer(Xfer *xfer);

public:
	Int m_nextScanFrames;                          // +0x20; xfer Int
	Bool m_detonated;                               // +0x24; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x28
typedef char DemoTrapUpdate_size[(sizeof(DemoTrapUpdate) == 0x28) ? 1 : -1];
typedef char DemoTrapUpdate_off_m_nextScanFrames[(offsetof(DemoTrapUpdate, m_nextScanFrames) == 0x20) ? 1 : -1];
typedef char DemoTrapUpdate_off_m_detonated[(offsetof(DemoTrapUpdate, m_detonated) == 0x24) ? 1 : -1];

#endif
