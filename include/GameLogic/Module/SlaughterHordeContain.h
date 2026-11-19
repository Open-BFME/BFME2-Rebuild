// SlaughterHordeContain.h: the SlaughterHordeContain module. SIZE 0x9EC (ModuleFactory operator new).
// Layout evidence: the constructor 0x0048034E calls HordeGarrisonContain first; it stores the
// tables 0x00C48AA0 at +0x0, 0x00C43528 at +0xC, 0x00C48A90 at +0x10, 0x00C48918 at +0x20, 0x00C46064 at +0x24, 0x00C48910 at +0x28, 0x00C463D8 at +0x2C, 0x00C488E0 at +0x30, 0x00C48B28 at +0x34. Members are what xfer (0x0048029A) transfers after HordeGarrisonContain::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004812B4 ?j_004812b4@@YAXXZ
//   +0x0 slot 2 -> 0x004803B0 ?Rva004803B0Get@@YAHXZ
//   +0x0 slot 20 -> 0x00480281 ?rva00480281@SlaughterHordeContain@@UAE_NPAVObject@@@Z
//   +0x0 slot 29 -> 0x00480584 ?rva00479B7F@SlaughterHordeContain@@UAEXPAVObject@@@Z
//   +0x0 slot 30 -> 0x004808E8 ?AddHordeMember@SlaughterHordeContain@@QAEXPAVObject@@@Z
//   +0x0 slot 32 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 33 -> 0x005CB9FA ?_bfme_ro_v50@RenderObjClass@@UAE_NH@Z
//   +0x0 slot 34 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 37 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 38 -> 0x0046261B ?rva0046261B@SlaughterHordeContain@@UAEX_N@Z
//   +0x0 slot 39 -> 0x004647CD ?get@Rva004647CDLeaField@@QBEPAXXZ
//   +0x0 slot 40 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 41 -> 0x004624E3 ?containReactToTransformChange@OpenContain@@UAEXXZ
//   +0x0 slot 42 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x0 slot 45 -> 0x0047840C ?get@Rva0047840CByteChase32Field@@QBEEXZ
//   +0x0 slot 52 -> 0x0046294E ?get@Rva0046294EByteChaseField@@QBEEXZ
//   +0x0 slot 54 -> 0x004639D5 ?rva004639D5@Rva004639D5@@UAEHH@Z
//   +0x0 slot 55 -> 0x00464FC7 ?rva00464FC7@SlaughterHordeContain@@UAE_NPAXH@Z
//   +0x0 slot 56 -> 0x0046322C ?rva0046322C@Rva0046322C@@QAEHXZ
//   +0x0 slot 57 -> 0x004780E7 ?rva004780E7@SlaughterHordeContain@@UAEPAVPlayer@@PBV2@@Z
//   +0x0 slot 58 -> 0x00479432 ?recalcApparentControllingPlayer@GarrisonContain@@UAEXXZ
//   +0x0 slot 59 -> 0x004783DC ?setOriginalTeam@Rva004783F4@@UAEXPAVTeam@@@Z
//   +0x0 slot 60 -> 0x00478C2C ?onContaining@GarrisonContain@@UAEXPAVObject@@_N@Z
//   +0x0 slot 61 -> 0x004806D3 ?rva00464D02@SlaughterHordeContain@@UAEXPAVObject@@@Z
//   +0x0 slot 62 -> 0x0046481D ?rva0050B238@Rva005E62F1@@WCA@AEXPAXI@Z
//   +0x0 slot 63 -> 0x00462504 ?rva00462504@SlaughterHordeContain@@UAEXXZ
//   +0x0 slot 64 -> 0x00479C0C ?rva00479C0C@HordeGarrisonContain@@UAEXPAVObject@@PAXPAUDamageInfoInput00462516@@@Z
//   +0x0 slot 65 -> 0x00479C2A ?rva0050B238@HordeGarrisonContain@@UAEXPAVObject@@_N@Z
//   +0x0 slot 66 -> 0x00462745 ?get@Rva00462745PtrChaseField@@QBEHXZ
//   +0x0 slot 67 -> 0x004647F0 ?Rva004647F0Adjust@@YIPAXPAX@Z
//   +0x0 slot 70 -> 0x00479BF3 ?rva00465011@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x0 slot 71 -> 0x00465078 ?rva00465078@OpenContainActionIface@@UAEXW4CommandSourceType@@@Z
//   +0x0 slot 74 -> 0x00463138 ?rva00463138@OpenContainActionIface@@UAEXXZ
//   +0x0 slot 76 -> 0x0048071D ?isValidContainerFor@CitadelSlaughterHordeContain@@UAE_NPAVObject@@_N1@Z
//   +0x0 slot 77 -> 0x00480471 ?addToContain@SlaughterHordeContain@@UAEXPAVObject@@@Z
//   +0x0 slot 78 -> 0x004638F1 ?addToContainList@OpenContain@@UAEXPAVObject@@@Z
//   +0x0 slot 79 -> 0x00479DCD ?rva00479DCD@Rva00479DCD@@QAEXPAVObject@@H@Z
//   +0x0 slot 80 -> 0x00478231 ?rva00478231@Rva00478231@@QAEX_N@Z
//   +0x0 slot 81 -> 0x0046274C ?rva0046274C@SlaughterHordeContain@@UAEXPAXH0@Z
//   +0x0 slot 82 -> 0x00462CE1 ?rva00462CE1@SlaughterHordeContain@@UAEPAVBfmeObject872Header@@PAV2@H@Z
//   +0x0 slot 85 -> 0x00462B93 ?rva00462B93@SlaughterHordeContain@@UAE_NPBVRva00462B93Arg@@@Z
//   +0x0 slot 86 -> 0x00462BFF ?rva00462BFF@OpenContainActionIface@@UBEDPBVObject@@0@Z
//   +0x0 slot 91 -> 0x004647D1 ?Rva004647D1Get@@YAMXZ
//   +0x0 slot 93 -> 0x004628AB ?rva004628AB@OpenContainActionIface@@UAE_NHH@Z
//   +0x0 slot 95 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x0 slot 96 -> 0x004631FA ?rva004631FA@SlaughterHordeContain@@UAE_NH@Z
//   +0x0 slot 99 -> 0x00479BDD ?rva004632E0@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x0 slot 101 -> 0x00464810 ?set@Rva00464810ByteSlot@@QAEXE@Z
//   +0x0 slot 102 -> 0x00462735 ?rva00462735@Rva00462735@@QAEXXZ
//   +0x0 slot 103 -> 0x00462CD3 ?get@Rva00462CD3Sub@@QBE_NXZ
//   +0x0 slot 104 -> 0x00463191 ?onContaining@TransportContain@@UAEXPAVObject@@_N@Z
//   +0x0 slot 105 -> 0x004631C9 ?rva004631C9@SlaughterHordeContain@@UAEXXZ
//   +0x0 slot 106 -> 0x004635EC ?rva004635EC@SlaughterHordeContain@@UAEXP6AXPAVObject@@PAX@Z1H@Z
//   +0x0 slot 107 -> 0x00462DE0 ?rva00462DE0@Rva00462DE0@@QAEHPAVRva2225E0Filter@@@Z
//   +0x0 slot 108 -> 0x004647D8 ?rva004647D8@Rva004647D8@@QAEPAXPAX@Z
//   +0x0 slot 109 -> 0x004624EB ?rva004624EB@Rva004624EB@@UAE?AURva00462465@@XZ
//   +0x0 slot 111 -> 0x005C4AFD ?vslot143@AIUpdateInterface@@UBE?AW4CommandSourceType@@XZ
//   +0x0 slot 114 -> 0x00478547 ?calcBestGarrisonPosition@GarrisonContain@@UAE_NPAUCoord3D@@PBU2@@Z
//   +0x0 slot 115 -> 0x004788CE ?attemptBestFirePointPosition@GarrisonContain@@UAE_NPAVObject@@PAVWeapon@@PBUCoord3D@@@Z
//   +0x0 slot 116 -> 0x00478850 ?attemptBestFirePointPosition@GarrisonContain@@UAE_NPAVObject@@PAVWeapon@@0@Z
//   +0x0 slot 117 -> 0x00463291 ?rva00463291@OpenContainActionIface@@UAEPAVObject@@PAVRva2225E0Filter@@@Z
//   +0x0 slot 119 -> 0x003F3FCE ?get@Rva003F3FCEDwordField@@QBEHXZ
//   +0x10 slot 0 -> 0x00480E72 ?update@SlaughterHordeContain@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 38 -> 0x00480661 ?isValidContainerFor@SlaughterHordeContain@@UAE_NPAVObject@@_N1@Z
//   +0x20 slot 84 -> 0x00481279 ?rva00481279@Rva00481279@@QAE_NHHHHHH@Z
//   +0x28 slot 0 -> 0x00480821 ?onDie@CitadelSlaughterHordeContain@@UAEXPBVDamageInfo@@@Z
// Not established: transfers xfer makes that this view cannot place: slot10 ?, ?xfer@HordeGarrisonContain@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_SLAUGHTER_HORDE_CONTAIN_H
#define BFME2_SLAUGHTER_HORDE_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/HordeGarrisonContain.h"
#include "Common/AsciiString.h"

class Thing;
class ModuleData;
class Xfer;

// HordeGarrisonContain ends at 0x9E4; this class's members follow it.
typedef char SlaughterHordeContain_base_size[(sizeof(HordeGarrisonContain) == 0x9E4) ? 1 : -1];

class SlaughterHordeContain : public HordeGarrisonContain
{
public:
	SlaughterHordeContain(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~SlaughterHordeContain();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00480E72)
	virtual Bool isValidContainerFor(Object *obj, Bool checkCapacity, Bool flag);	// override: isValidContainerFor slot 38 at +0x20 (0x00480661)
	virtual void addToContain(Object *obj);	// override: addToContain slot 39 at +0x20 (0x00480471)
	virtual void onRemoving(Object *obj);	// override: onRemoving slot 23 at +0x20 (0x004806D3; the map spells it rva00464D02)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x9E4;                        // +0x9E4; xfer UnsignedInt
	AsciiString m_unk0x9E8;                        // +0x9E8; xfer AsciiString
protected:
	virtual Bool containSlot12() { return true; }
	virtual Bool containSlot13() { return true; }
	virtual void loadPostProcess() { HordeGarrisonContain::loadPostProcess(); }	// slot 1: the forwarder retail folds at 0x004812B4 (Tunnel, ProductionQueueHorde, SlaughterHorde)
};
// SIZE 0x9EC
typedef char SlaughterHordeContain_size[(sizeof(SlaughterHordeContain) == 0x9EC) ? 1 : -1];
typedef char SlaughterHordeContain_off_m_unk0x9E4[(offsetof(SlaughterHordeContain, m_unk0x9E4) == 0x9E4) ? 1 : -1];
typedef char SlaughterHordeContain_off_m_unk0x9E8[(offsetof(SlaughterHordeContain, m_unk0x9E8) == 0x9E8) ? 1 : -1];

#endif
