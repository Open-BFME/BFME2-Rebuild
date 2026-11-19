// DRAFT (h05 modgen): generated from constructor RVA 0x0045961D and xfer RVA 0x00459579; no function
// matches against it yet, so it stays in Draft until one does.
// ClickReactionBehavior.h: the ClickReactionBehavior module. SIZE 0x2C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0045961D calls UpdateModule first; it stores the
// tables 0x00BE2B78 at +0x20, 0x00C4125C at +0x0, 0x00C411A0 at +0xC, 0x00C41194 at +0x10, 0x00C41188 at +0x20. Members are what xfer (0x00459579) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00459573 ?name@Rva00459573Named@@QBEPBDXZ
//   +0xC slot 33 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x10 slot 0 -> 0x0045969F ?update@Rva0045969F@@UAEHXZ
//   +0x10 slot 3 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 12 -> 0x002533D3 ?getDie@DieModule@@UAEPAVDieModuleInterface@@XZ
//   +0x10 slot 46 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x10 slot 47 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 59 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 62 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x20 slot 0 -> 0x004543E3 ?get@Rva004543A9LeaNegSbbField@@WBE@BEHXZ
//   +0x20 slot 1 -> 0x004596F9 ?rva004596F9@ClickReactionBehavior@@UAEXXZ
//   +0x20 slot 2 -> 0x00459864 ?rva00459864@Rva00459864@@QAEX_N@Z
//   +0x20 slot 4 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 5 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
// Not established: interface tables at +0x20 (their vptrs sit in the gaps there).
#pragma once
#ifndef BFME2_CLICK_REACTION_BEHAVIOR_H
#define BFME2_CLICK_REACTION_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char ClickReactionBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class ClickReactionBehavior : public UpdateModule
{
public:
	ClickReactionBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x20[0x4];
	Int m_unk0x24;                                 // +0x24; xfer Int
	Int m_unk0x28;                                 // +0x28; xfer Int
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x2C
typedef char ClickReactionBehavior_size[(sizeof(ClickReactionBehavior) == 0x2C) ? 1 : -1];
typedef char ClickReactionBehavior_off_m_unk0x24[(offsetof(ClickReactionBehavior, m_unk0x24) == 0x24) ? 1 : -1];
typedef char ClickReactionBehavior_off_m_unk0x28[(offsetof(ClickReactionBehavior, m_unk0x28) == 0x28) ? 1 : -1];

#endif
