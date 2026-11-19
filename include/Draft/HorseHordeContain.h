// DRAFT (h05 modgen): generated from constructor RVA 0x004765E5 and xfer RVA 0x004766C8; no function
// matches against it yet, so it stays in Draft until one does.
// HorseHordeContain.h: the HorseHordeContain module. SIZE 0x30C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004765E5 calls HordeContain first; it stores the
// tables 0x00C45C38 at +0x0, 0x00C441B8 at +0xC, 0x00C45040 at +0x10, 0x00C45AC0 at +0x20, 0x00C45AA4 at +0x24, 0x00C46F78 at +0x28, 0x00C58790 at +0x2C, 0x00C43FFC at +0x30, 0x00C46B9C at +0x34, 0x00C47834 at +0xFC, 0x00C45838 at +0x11C. Members are what xfer (0x004766C8) transfers after HordeContain::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x0047A69F ?j_0047a69f@@YAXXZ
//   +0x0 slot 2 -> 0x0047664D ?Rva0047664DGet@@YAHXZ
//   +0x0 slot 35 -> 0x004766E3 ?j_004766e3@@YAXXZ
//   +0x0 slot 36 -> 0x00476D77 ?rva00476D77@HorseHordeContain@@QAEXPAVObject@@PBUCoord3D@@M_N@Z
//   +0x0 slot 39 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 40 -> 0x004766AA ?rva004766AA@HorseHordeContain@@UAEXPAVObject@@@Z
//   +0x0 slot 41 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x0 slot 42 -> 0x0047680E ?rva0047680E@HorseHordeContain@@UAEXPBUCoord3D@@H@Z
//   +0x20 slot 45 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x20 slot 99 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x20 slot 102 -> 0x0046E697 ?onDelete@HordeContain@@UAEXXZ
//   +0x20 slot 103 -> 0x004672B7 ?onCapture@TransportContain@@UAEXHH@Z
//   +0x20 slot 104 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x20 slot 106 -> 0x000910FB ?Create@GameFileClass@@UAEHXZ
//   +0x20 slot 107 -> 0x00462F75 ?removeFromContainList@OpenContain@@UAEXPAVObject@@@Z
//   +0x20 slot 108 -> 0x00466E58 ?rva00466E58@Rva00466E58@@QBEEH@Z
//   +0x20 slot 109 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 111 -> 0x00462C72 ?monitorConditionChanges@OpenContain@@MAEXXZ
//   +0x20 slot 112 -> 0x00464286 ?redeployOccupants@OpenContain@@MAEXXZ
//   +0x20 slot 113 -> 0x00464EE7 ?rva00464EE7@OpenContain@@UAE_NPAVObject@@@Z
//   +0x20 slot 115 -> 0x0046279C ?doLoadSound@OpenContain@@MAEXXZ
//   +0x20 slot 116 -> 0x0046282B ?doUnloadSound@OpenContain@@MAEXXZ
//   +0x20 slot 118 -> 0x00462E3A ?rva00462E3A@Rva00462E3A@@QAEXPAVObject@@HH@Z
//   +0x20 slot 119 -> 0x00467F3B ?killRidersWhoAreNotFreeToExit@TransportContain@@MAEXXZ
//   +0x11C slot 4 -> 0x004766C3 ?returnToFormation@HorseHordeContain@@UAEX_N@Z
//   +0x11C slot 15 -> 0x00476749 ?rva00476749@HorseHordeContain@@UAEXXZ
//   +0x11C slot 42 -> 0x004766BE ?rva004766BE@Rva004766BE@@QAEXPBVObject@@@Z
//   +0x11C slot 112 -> 0x0047676B ?rva0047676B@HorseHordeContain@@UAE_NM@Z
#pragma once
#ifndef BFME2_HORSE_HORDE_CONTAIN_H
#define BFME2_HORSE_HORDE_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/HordeContain.h"

class Thing;
class ModuleData;
class Xfer;

// HordeContain ends at 0x30C; this class's members follow it.
typedef char HorseHordeContain_base_size[(sizeof(HordeContain) == 0x30C) ? 1 : -1];

class HorseHordeContain : public HordeContain
{
public:
	HorseHordeContain(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~HorseHordeContain();
	virtual void returnToFormation(Bool flag);	// override: returnToFormation slot 4 at +0x11C (0x004766C3)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x30C
typedef char HorseHordeContain_size[(sizeof(HorseHordeContain) == 0x30C) ? 1 : -1];

#endif
