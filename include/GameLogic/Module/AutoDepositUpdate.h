// AutoDepositUpdate.h: the AutoDepositUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0049A20F calls UpdateModule first; it stores the
// tables 0x00C504EC at +0x0, 0x00BEFF90 at +0xC, 0x00C504E0 at +0x10. Members are what xfer (0x004A0CC3) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x0049A3AE ?update@AutoDepositUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_AUTO_DEPOSIT_UPDATE_H
#define BFME2_AUTO_DEPOSIT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "Draft/AutoDepositUpdateModuleData.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AutoDepositUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class Player;
class AutoDepositUpdate : public UpdateModule
{
public:
	AutoDepositUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0049A3AE)
	const AutoDepositUpdateModuleData *getAutoDepositUpdateModuleData() const { return (const AutoDepositUpdateModuleData *)m_moduleData; }
	void awardInitialCaptureBonus(Player *player);	// 0x0049A26F (Player::becomingTeamMember 0x002AD4EC calls it on capture)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_depositOnFrame;                         // +0x20; xfer UnsignedInt
	Bool m_awardInitialCaptureBonus;                                // +0x24; xfer Bool
	Bool m_initialized;                                // +0x25; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x28
typedef char AutoDepositUpdate_size[(sizeof(AutoDepositUpdate) == 0x28) ? 1 : -1];
typedef char AutoDepositUpdate_off_m_unk0x20[(offsetof(AutoDepositUpdate, m_depositOnFrame) == 0x20) ? 1 : -1];
typedef char AutoDepositUpdate_off_m_unk0x24[(offsetof(AutoDepositUpdate, m_awardInitialCaptureBonus) == 0x24) ? 1 : -1];
typedef char AutoDepositUpdate_off_m_unk0x25[(offsetof(AutoDepositUpdate, m_initialized) == 0x25) ? 1 : -1];

#endif
