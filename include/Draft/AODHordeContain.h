// DRAFT (h05 modgen): generated from constructor RVA 0x0047B3DB and xfer RVA 0x0047B2FB; no function
// matches against it yet, so it stays in Draft until one does.
// AODHordeContain.h: the AODHordeContain module. SIZE 0x8E0 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0047B3DB calls HordeContain first; it stores the
// tables 0x00C46D28 at +0x0, 0x00C441B8 at +0xC, 0x00C45040 at +0x10, 0x00C46BB0 at +0x20, 0x00C45AA4 at +0x24, 0x00C46F78 at +0x28, 0x00C58790 at +0x2C, 0x00C43FFC at +0x30, 0x00C46B9C at +0x34, 0x00C47834 at +0xFC, 0x00C46930 at +0x11C. Members are what xfer (0x0047B2FB) transfers after HordeContain::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x0047A69F ?j_0047a69f@@YAXXZ
//   +0x0 slot 2 -> 0x0047B518 ?Rva0047B518Get@@YAHXZ
//   +0x0 slot 33 -> 0x0047B690 ?initOffsets@AODHordeContain@@QAEXPAX@Z
//   +0x0 slot 35 -> 0x0047A77E ?rva0047A77E@AODHordeContain@@QAEXXZ
//   +0x0 slot 36 -> 0x0047A895 ?rva0047A895@HordeContain@@QAEXPAVObject@@PBUCoord3D@@M_N@Z
//   +0x0 slot 37 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 39 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 41 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x0 slot 42 -> 0x0047680E ?rva0047680E@HorseHordeContain@@UAEXPBUCoord3D@@H@Z
//   +0x20 slot 22 -> 0x0047A3CB ?onContaining@AODHordeContain@@UAEXPAVObject@@_N@Z
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
//   +0x11C slot 7 -> 0x0047AC8F ?rva0047AC8F@AODHordeContain@@UAEPAUCoord3D@@PAU2@PAVObject@@H@Z
//   +0x11C slot 76 -> 0x0047B628 ?rva0047B628@Rva0047B628@@QAEXHPAX@Z
#pragma once
#ifndef BFME2_A_O_D_HORDE_CONTAIN_H
#define BFME2_A_O_D_HORDE_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/HordeContain.h"
#include "Common/Coord3D.h"

class Thing;
class ModuleData;
class Xfer;

// HordeContain ends at 0x30C; this class's members follow it.
typedef char AODHordeContain_base_size[(sizeof(HordeContain) == 0x30C) ? 1 : -1];

class AODHordeContain : public HordeContain
{
public:
	AODHordeContain(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~AODHordeContain();

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	void refreshTrackedLargeUnit();                  // 0x0047B2AA, non-virtual
	UnsignedByte m_gap0x30C[0xC];
	ObjectID m_unk0x318;                           // +0x318; xfer ObjectID (XferObjectID 0x007060B2 at 0x0047B31B); refresh 0x0047B2BC reads it
	UnsignedInt m_unk0x31C;                        // +0x31C; xfer UnsignedInt
	Coord3D m_unk0x320;                            // +0x320; xfer Coord3D
	Real m_unk0x32C;                               // +0x32C; refresh 0x0047B2E4 stores 17.0f (0x00BC653C)
	Real m_unk0x330;                               // +0x330; refresh 0x0047B2F1 stores GeometryInfo::getMaxHeightAbovePosition
	Real m_unk0x334;                               // +0x334; xfer Real
	UnsignedByte m_gap0x338[0x5A8];
};
// SIZE 0x8E0
typedef char AODHordeContain_size[(sizeof(AODHordeContain) == 0x8E0) ? 1 : -1];
typedef char AODHordeContain_off_m_unk0x318[(offsetof(AODHordeContain, m_unk0x318) == 0x318) ? 1 : -1];
typedef char AODHordeContain_off_m_unk0x31C[(offsetof(AODHordeContain, m_unk0x31C) == 0x31C) ? 1 : -1];
typedef char AODHordeContain_off_m_unk0x320[(offsetof(AODHordeContain, m_unk0x320) == 0x320) ? 1 : -1];
typedef char AODHordeContain_off_m_unk0x334[(offsetof(AODHordeContain, m_unk0x334) == 0x334) ? 1 : -1];

#endif
