// UpdateModule.h: SAGE base of update modules. SIZE 0x20.
// Target facts (old repo Code/GameEngine/Include/GameLogic/Module/UpdateModule.h,
// re-read from the image): an UpdateModule has THREE vtable subobjects. The
// ctor ??0UpdateModule (0x00253390) is, in this order,
//   call ??0BehaviorModule            ; leaves 0x00BEEA7C at +0, 0x00BEE9C0 at +0x0C
//   mov  [esi+0x10], 0x00BE2B78       ; UpdateModuleInterface's own 3-purecall table
//   and/or [esi+0x14],0 / or [esi+0x18],-1 / or [esi+0x1C],-1
//   mov  [esi],     0x00BF004C        ; primary
//   mov  [esi+0x0C],0x00BEFF90        ; BehaviorModuleInterface, UpdateModule's copy
//   mov  [esi+0x10],0x00BFBCE4        ; UpdateModuleInterface
// so the interface subobject sits at +0x10, not +0x0C: +0x0C is the behavior
// interface UpdateModule inherits from BehaviorModule. sizeof is 0x20 and derived
// fields start there (AIGateUpdate 0x004B08E8); members are 0x14, 0x18, 0x1C.
// 0x00BE2B78 is three __purecall words (the interface's slots, no destructor),
// which is what pins the interface at three slots.
// Calls and casts therefore use +0x10: ?getUpdate@UpdateModule@@UAEPAVUpdateModuleInterface@@XZ
// is the 13-byte row at 0x002533D3, `lea eax,[ecx-0xC]; add ecx,4; ...` -- it
// takes the +0x0C behavior-interface pointer and returns object+0x10. The inline
// getUpdate() below reproduces those bytes. Never spell the update interface as
// an object at +0x0C.
// The primary table 0x00BF004C has 13 slots (the UpdateModule string at 0x00BF0080
// bounds it): 1 loadPostProcess (0x0058B03E), 2 GetSnapshotName (0x002533CD),
// 3 xfer (0x0044DF9F), 12 getUpdatePhase (0x0022C4CB, returns 2).
// ~UpdateModule is 0x0024A797: it stores 0x00BFBCE4 at +0x10, then restores
// BehaviorModule's 0x00BEEA7C and 0x00BEE9C0 at +0/+0x0C inline, then jumps to
// Snapshot's 0x0049B47C. setWakeFrame 0x0044DF71, frameToSleepTime 0x0044DF28.
// Proven by matched ctors, not by reading: ??0FlammableUpdate (0x0048C64F) puts
// DamageModuleInterface at +0x20 and its first member at +0x24, which only
// holds if UpdateModule is 0x20 with the interface at +0x10.
#pragma once
#ifndef BFME2_UPDATEMODULE_H
#define BFME2_UPDATEMODULE_H

#include "GameLogic/Module/BehaviorModule.h"
#include "Common/BitFlags11.h"

typedef BitFlags<11> DisabledMaskType;
extern DisabledMaskType DISABLEDMASK_NONE;	// 0x00A030CC

class UpdateModuleInterface
{
public:
	virtual UpdateSleepTime update() = 0;									// 0
	virtual DisabledMaskType getDisabledTypesToProcess() const = 0;			// 1
	virtual void rva0044DF8D(UpdateSleepTime wakeDelay) = 0;				// 2
};

class UpdateModule : public BehaviorModule, public UpdateModuleInterface
{
public:
	UpdateModule(Thing *thing, const ModuleData *moduleData);
	virtual ~UpdateModule() {}											// COMDAT 0x0024A797

	virtual UpdateModuleInterface *getUpdate() { return this; }
	virtual UpdateSleepTime update() = 0;
	virtual DisabledMaskType getDisabledTypesToProcess() const { return DISABLEDMASK_NONE; }	// COMDAT 0x00253376
	virtual void rva0044DF8D(UpdateSleepTime wakeDelay);
	virtual void xfer(Xfer *xfer);

	void friend_setNextCallFrame(UnsignedInt frame);
	Int rva0023C3DA(Int *phase);

	UnsignedInt m_nextCallFrameAndPhase;	// 0x14
	Int m_indexInLogic;						// 0x18
	Int m_unk0x1C;							// 0x1C

protected:
	friend class GameLogic;	// registerObject 0x00242AE9 asks getUpdatePhase to pick the phase queue
	virtual void loadPostProcess();
	virtual const char *GetSnapshotName() const;
	virtual SleepyUpdatePhase getUpdatePhase() const;						// slot 12

	void setWakeFrame(Object *obj, UpdateSleepTime wakeDelay);
	UpdateSleepTime getWakeFrame() const;
	UpdateSleepTime frameToSleepTime(UnsignedInt frame1, UnsignedInt frame2,
		UnsignedInt frame3, UnsignedInt frame4);
};
// SIZE 0x20

#endif
