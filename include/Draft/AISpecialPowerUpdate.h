// DRAFT (h05 modgen): generated from constructor RVA 0x004B2F2A and xfer RVA 0x004B3352; no function
// matches against it yet, so it stays in Draft until one does.
// AISpecialPowerUpdate.h: the AISpecialPowerUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B2F2A calls UpdateModule first; it stores the
// tables 0x00C56E1C at +0x0, 0x00C56D60 at +0xC, 0x00C56D50 at +0x10. Members are what xfer (0x004B3352) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004B2ECE ?name@AISpecialPowerUpdate@@QBEPBDXZ
//   +0x0 slot 13 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0xC slot 45 -> 0x004B33EB ?mapLoadPostProcess@AISpecialPowerUpdate@@UAEXPBVDict@@@Z
//   +0x10 slot 0 -> 0x004B327F ?update@AISpecialPowerUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?, slot4 +0x21, slot4 ?.
#pragma once
#ifndef BFME2_A_I_SPECIAL_POWER_UPDATE_H
#define BFME2_A_I_SPECIAL_POWER_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AISpecialPowerUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class AISpecialPowerUpdate : public UpdateModule
{
public:
	AISpecialPowerUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	const char *name() const;   // 0x004B2ECE (non-virtual literal name getter, QBE)
	virtual const char *GetSnapshotName() const;
	virtual ~AISpecialPowerUpdate();

protected:
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x20;                                // +0x20; xfer Bool
	Bool m_unk0x21;                                // +0x21; xfer Bool
	Int m_unk0x24;                                 // +0x24; constructor store
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x28
typedef char AISpecialPowerUpdate_size[(sizeof(AISpecialPowerUpdate) == 0x28) ? 1 : -1];
typedef char AISpecialPowerUpdate_off_m_unk0x20[(offsetof(AISpecialPowerUpdate, m_unk0x20) == 0x20) ? 1 : -1];
typedef char AISpecialPowerUpdate_off_m_unk0x21[(offsetof(AISpecialPowerUpdate, m_unk0x21) == 0x21) ? 1 : -1];
typedef char AISpecialPowerUpdate_off_m_unk0x24[(offsetof(AISpecialPowerUpdate, m_unk0x24) == 0x24) ? 1 : -1];

#endif
