// BannerCarrierUpdate.h: the BannerCarrierUpdate module. SIZE 0x20 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00496A98 calls UpdateModule first; it stores the
// tables 0x00C4F838 at +0x0, 0x00BEFF90 at +0xC, 0x00C4F82C at +0x10. Members are what xfer (0x0047F94C) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x00497524 ?update@BannerCarrierUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 1 -> 0x004A2D67 ?getDisabledTypesToProcess@AIUpdateInterface@@UBE?AV?$BitFlags@$0L@@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_BANNER_CARRIER_UPDATE_H
#define BFME2_BANNER_CARRIER_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/BitFlags11.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char BannerCarrierUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class BannerCarrierUpdate : public UpdateModule
{
public:
	BannerCarrierUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~BannerCarrierUpdate();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00497524)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
public:
	virtual DisabledMaskType getDisabledTypesToProcess() const { return DisabledMaskType(DisabledMaskType::kInit, 3); }	// UpdateModuleInterface slot 1: retail 0x004A2D67 (DISABLED_HELD only; shared body, filed under AIUpdateInterface)
};
// SIZE 0x20
typedef char BannerCarrierUpdate_size[(sizeof(BannerCarrierUpdate) == 0x20) ? 1 : -1];

#endif
