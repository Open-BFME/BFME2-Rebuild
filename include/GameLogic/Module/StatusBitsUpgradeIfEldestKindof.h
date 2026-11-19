// StatusBitsUpgradeIfEldestKindof.h: the StatusBitsUpgradeIfEldestKindof module. SIZE 0x20 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B4A32 calls StatusBitsUpgrade first; it stores the
// tables 0x00C57ADC at +0x0, 0x00C57A20 at +0xC, 0x00C579D8 at +0x10, 0x00C58790 at +0x18. Members are what xfer (0x004B4B7C) transfers after StatusBitsUpgrade::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 2 -> 0x004B4A86 ?Rva004B4A86Get@@YAHXZ
//   +0xC slot 40 -> 0x004B4A8C ?getCreate@ReplaceSelfUpgrade@@UAEPAVCreateModuleInterface@@XZ
//   +0x10 slot 2 -> 0x004B4AFF ?rva004CE2B0@StatusBitsUpgradeIfEldestKindof@@UAE_NPAVRva00406F9C@@@Z
//   +0x10 slot 18 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 22 -> 0x0045875F ?getDamage@UpgradeModule@@UAEPAVDamageModuleInterface@@XZ
//   +0x10 slot 28 -> 0x002533D3 ?getDie@DieModule@@UAEPAVDieModuleInterface@@XZ
//   +0x10 slot 61 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x10 slot 62 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 66 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 74 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
// Not established: transfers xfer makes that this view cannot place: ?xfer@Rva00485BB8@@QAEXPAVXfer@@@Z +0x1C.
#pragma once
#ifndef BFME2_STATUS_BITS_UPGRADE_IF_ELDEST_KINDOF_H
#define BFME2_STATUS_BITS_UPGRADE_IF_ELDEST_KINDOF_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/StatusBitsUpgrade.h"

class Thing;
class ModuleData;
class Xfer;

// StatusBitsUpgrade ends at 0x1C; this class's members follow it.
typedef char StatusBitsUpgradeIfEldestKindof_base_size[(sizeof(StatusBitsUpgrade) == 0x1C) ? 1 : -1];

class StatusBitsUpgradeIfEldestKindof : public StatusBitsUpgrade
{
public:
	StatusBitsUpgradeIfEldestKindof(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x1C[0x4];
};
// SIZE 0x20
typedef char StatusBitsUpgradeIfEldestKindof_size[(sizeof(StatusBitsUpgradeIfEldestKindof) == 0x20) ? 1 : -1];

#endif
