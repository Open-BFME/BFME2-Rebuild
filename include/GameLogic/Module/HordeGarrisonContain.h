// HordeGarrisonContain.h: the HordeGarrisonContain module. SIZE 0x9E4 (ModuleFactory operator new).
// Layout evidence: the constructor 0x0047A040 calls GarrisonContain first; it stores the
// tables 0x00C46570 at +0x0, 0x00C43528 at +0xC, 0x00C46560 at +0x10, 0x00C463E8 at +0x20, 0x00C46064 at +0x24, 0x00C463E4 at +0x28, 0x00C463D8 at +0x2C, 0x00C488E0 at +0x30, 0x00C48B28 at +0x34. Members are what xfer (0x00479D28) transfers after GarrisonContain::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00479D68 ?rva00479D68@Rva00479D68@@QAEXXZ
//   +0x0 slot 2 -> 0x0047A0E0 ?Rva0047A0E0Get@@YAHXZ
//   +0x0 slot 27 -> 0x00479BC5 ?rva00479BC5@Rva00479BC5@@QAEHW4ObjectID@@@Z
//   +0x0 slot 28 -> 0x00479FD4 ?rva00479FD4@Rva00479FD4Owner@@QAE_NPAVObject@@@Z
//   +0x0 slot 29 -> 0x00479B7F ?rva00479B7F@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x0 slot 30 -> 0x00479ADA ?rva00479ADA@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x0 slot 31 -> 0x00479B0A ?rva00479B0A@Rva00479B0A@@QAEXPAVObject@@@Z
//   +0x20 slot 26 -> 0x00479C0C ?rva00479C0C@HordeGarrisonContain@@UAEXPAVObject@@PAXPAUDamageInfoInput00462516@@@Z
//   +0x20 slot 27 -> 0x00479C2A ?rva0050B238@HordeGarrisonContain@@UAEXPAVObject@@_N@Z
//   +0x20 slot 32 -> 0x00479BF3 ?rva00465011@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x20 slot 38 -> 0x00479E62 ?isValidContainerFor@HordeGarrisonContain@@UAE_NPAVObject@@_N1@Z
//   +0x20 slot 39 -> 0x00479D6D ?addToContain@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x20 slot 41 -> 0x00479DCD ?rva00479DCD@Rva00479DCD@@QAEXPAVObject@@H@Z
//   +0x20 slot 61 -> 0x00479BDD ?rva004632E0@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x20 slot 86 -> 0x00479F29 ?rva004626CA@HordeGarrisonContain@@UAEPBUCoord3D@@XZ
//   +0x20 slot 87 -> 0x00479F62 ?rva004626AF@HordeGarrisonContain@@UAEPBUCoord3D@@XZ
//   +0x20 slot 88 -> 0x00479F9B ?rva00464829@HordeGarrisonContain@@UAEPBUCoord3D@@XZ
//   +0x20 slot 94 -> 0x00479643 ?update@GarrisonContain@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 95 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 96 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x28 slot 0 -> 0x00479CF6 ?rva00479CF6@Rva00479CF6@@QAEXH@Z
//   +0x28 slot 1 -> 0x0046261B ?rva0046261B@SlaughterHordeContain@@UAEX_N@Z
//   +0x28 slot 2 -> 0x004647CD ?get@Rva004647CDLeaField@@QBEPAXXZ
//   +0x28 slot 3 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x28 slot 4 -> 0x004624E3 ?containReactToTransformChange@OpenContain@@UAEXXZ
//   +0x28 slot 5 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x28 slot 6 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x28 slot 8 -> 0x0047840C ?get@Rva0047840CByteChase32Field@@QBEEXZ
//   +0x28 slot 15 -> 0x0046294E ?get@Rva0046294EByteChaseField@@QBEEXZ
//   +0x28 slot 16 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x28 slot 17 -> 0x004639D5 ?rva004639D5@Rva004639D5@@UAEHH@Z
//   +0x28 slot 18 -> 0x00464FC7 ?rva00464FC7@SlaughterHordeContain@@UAE_NPAXH@Z
//   +0x28 slot 19 -> 0x0046322C ?rva0046322C@Rva0046322C@@QAEHXZ
//   +0x28 slot 20 -> 0x004780E7 ?rva004780E7@SlaughterHordeContain@@UAEPAVPlayer@@PBV2@@Z
//   +0x28 slot 21 -> 0x00479432 ?recalcApparentControllingPlayer@GarrisonContain@@UAEXXZ
//   +0x28 slot 22 -> 0x004783DC ?setOriginalTeam@Rva004783F4@@UAEXPAVTeam@@@Z
//   +0x28 slot 23 -> 0x00478C2C ?onContaining@GarrisonContain@@UAEXPAVObject@@_N@Z
//   +0x28 slot 24 -> 0x00478D0A ?onRemoving@GarrisonContain@@UAEXPAVObject@@@Z
//   +0x28 slot 25 -> 0x0046481D ?rva0050B238@Rva005E62F1@@WCA@AEXPAXI@Z
//   +0x28 slot 26 -> 0x00462504 ?rva00462504@SlaughterHordeContain@@UAEXXZ
//   +0x28 slot 29 -> 0x00462745 ?get@Rva00462745PtrChaseField@@QBEHXZ
//   +0x28 slot 30 -> 0x004647F0 ?Rva004647F0Adjust@@YIPAXPAX@Z
//   +0x28 slot 34 -> 0x00465078 ?rva00465078@OpenContainActionIface@@UAEXW4CommandSourceType@@@Z
//   +0x28 slot 35 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x28 slot 37 -> 0x00463138 ?rva00463138@OpenContainActionIface@@UAEXXZ
//   +0x28 slot 41 -> 0x004638F1 ?addToContainList@OpenContain@@UAEXPAVObject@@@Z
//   +0x28 slot 43 -> 0x00478231 ?rva00478231@Rva00478231@@QAEX_N@Z
//   +0x28 slot 44 -> 0x0046274C ?rva0046274C@SlaughterHordeContain@@UAEXPAXH0@Z
//   +0x28 slot 45 -> 0x00462CE1 ?rva00462CE1@SlaughterHordeContain@@UAEPAVBfmeObject872Header@@PAV2@H@Z
//   +0x28 slot 48 -> 0x00462B93 ?rva00462B93@SlaughterHordeContain@@UAE_NPBVRva00462B93Arg@@@Z
//   +0x28 slot 49 -> 0x00462BFF ?rva00462BFF@OpenContainActionIface@@UBEDPBVObject@@0@Z
//   +0x28 slot 54 -> 0x004647D1 ?Rva004647D1Get@@YAMXZ
//   +0x28 slot 56 -> 0x004628AB ?rva004628AB@OpenContainActionIface@@UAE_NHH@Z
//   +0x28 slot 58 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x28 slot 59 -> 0x004631FA ?rva004631FA@SlaughterHordeContain@@UAE_NH@Z
//   +0x28 slot 64 -> 0x00464810 ?set@Rva00464810ByteSlot@@QAEXE@Z
//   +0x28 slot 65 -> 0x00462735 ?rva00462735@Rva00462735@@QAEXXZ
//   +0x28 slot 66 -> 0x00462CD3 ?get@Rva00462CD3Sub@@QBE_NXZ
//   +0x28 slot 67 -> 0x00463191 ?onContaining@TransportContain@@UAEXPAVObject@@_N@Z
//   +0x28 slot 68 -> 0x004631C9 ?rva004631C9@SlaughterHordeContain@@UAEXXZ
//   +0x28 slot 69 -> 0x004635EC ?rva004635EC@SlaughterHordeContain@@UAEXP6AXPAVObject@@PAX@Z1H@Z
//   +0x28 slot 70 -> 0x00462DE0 ?rva00462DE0@Rva00462DE0@@QAEHPAVRva2225E0Filter@@@Z
//   +0x28 slot 71 -> 0x004647D8 ?rva004647D8@Rva004647D8@@QAEPAXPAX@Z
//   +0x28 slot 72 -> 0x004624EB ?rva004624EB@Rva004624EB@@UAE?AURva00462465@@XZ
//   +0x28 slot 74 -> 0x005C4AFD ?vslot143@AIUpdateInterface@@UBE?AW4CommandSourceType@@XZ
//   +0x28 slot 77 -> 0x00478547 ?calcBestGarrisonPosition@GarrisonContain@@UAE_NPAUCoord3D@@PBU2@@Z
//   +0x28 slot 78 -> 0x004788CE ?attemptBestFirePointPosition@GarrisonContain@@UAE_NPAVObject@@PAVWeapon@@PBUCoord3D@@@Z
//   +0x28 slot 79 -> 0x00478850 ?attemptBestFirePointPosition@GarrisonContain@@UAE_NPAVObject@@PAVWeapon@@0@Z
//   +0x28 slot 80 -> 0x00463291 ?rva00463291@OpenContainActionIface@@UAEPAVObject@@PAVRva2225E0Filter@@@Z
//   +0x28 slot 82 -> 0x003F3FCE ?get@Rva003F3FCEDwordField@@QBEHXZ
//   +0x28 slot 83 -> 0x004642D3 ?rva004642D3@SlaughterHordeContain@@UAEXXZ
//   +0x28 slot 84 -> 0x004647FD ?set@Rva004647FDByteSlot@@QAEXE@Z
//   +0x28 slot 85 -> 0x0046333B ?rva0046333B@Rva0046333B@@QAE_NPAH00000@Z
//   +0x28 slot 86 -> 0x00464825 ?get@Rva00464825DwordField@@QBEHXZ
//   +0x28 slot 90 -> 0x0046480A ?rva0046480A@Rva0046480A@@QAEHH@Z
//   +0x28 slot 91 -> 0x004625BB ?rva004625BB@SlaughterHordeContain@@UAE_NPAXH@Z
//   +0x28 slot 92 -> 0x004625F0 ?rva004625F0@SlaughterHordeContain@@UAEXPAX@Z
//   +0x28 slot 93 -> 0x00462955 ?rva00462955@OpenContainActionIface@@UAE_NPAVObject@@H@Z
//   +0x28 slot 94 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x30 slot 0 -> 0x00479BCA ?isExitBusy@HordeGarrisonContain@@UBE_NXZ
//   +0x30 slot 2 -> 0x0047A14C ?exitObjectViaDoor@HordeGarrisonContain@@UAEXPAVObject@@W4ExitDoorType@@@Z
//   +0x30 slot 9 -> 0x00479CD9 ?getNaturalRallyPoint@Rva00479CD9@@UAE_NPAURva00479CD9Pos@@H@Z
//   +0x30 slot 12 -> 0x00480821 ?onDie@CitadelSlaughterHordeContain@@UAEXPBVDamageInfo@@@Z
// Not established: transfers xfer makes that this view cannot place: ?xfer@GarrisonContain@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_HORDE_GARRISON_CONTAIN_H
#define BFME2_HORDE_GARRISON_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/GarrisonContain.h"

class Thing;
class ModuleData;
class Xfer;
class Object;

// GarrisonContain ends at 0x9E0; this class's members follow it.
typedef char HordeGarrisonContain_base_size[(sizeof(GarrisonContain) == 0x9E0) ? 1 : -1];

class HordeGarrisonContain : public GarrisonContain
{
public:
	HordeGarrisonContain(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~HordeGarrisonContain();	// out of line: subclass destructors tail-jump to it
	virtual void orderAllPassengersToExit(CommandSourceType cmdSource);	// ContainModuleInterface 32, 0x00479BF3 (forwards to the horde exit helper at +0x9C0)
	virtual Bool isExitBusy() const;	// 0x00479BCA: ExitInterface slot 0 (this at +0x30); busy until the frame stored at +0x9E0
	// Primary-table slots from this class's table 0x00846570 (32 slots; GarrisonContain's has 28):
	virtual Int getObjectGarrisonPointIndex(ObjectID id);				// 27: 0x00479BC5, a jump to GarrisonContain's 0x00479292
	virtual Bool garrisonSlot28(Object *obj);									// 28: 0x00479FD4
	virtual void garrisonSlot29(Object *obj);									// 29: 0x00479B7F (ProductionQueueHordeContain overrides it)
	virtual void garrisonSlot30(Object *obj);									// 30: 0x00479ADA (ProductionQueueHordeContain overrides it)
	virtual void garrisonSlot31(Object *obj);									// 31: 0x00479B0A

protected:
	// Retail slot (+0x20 slot 38) -> 0x00479E62 (protected virtual): the validity test TunnelContain calls first.
	virtual Bool isValidContainerFor(Object *obj, Bool checkCapacity, Bool flag);
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess() { GarrisonContain::loadPostProcess(); }	// slot 1: 0x00479D68, a jump to GarrisonContain's forwarder

public:
	// The exit interface compares this deadline with the current logic frame.
	UnsignedInt m_exitBusyUntilFrame;
};
// SIZE 0x9E4
typedef char HordeGarrisonContain_size[(sizeof(HordeGarrisonContain) == 0x9E4) ? 1 : -1];
typedef char HordeGarrisonContain_off_m_exitBusyUntilFrame[(offsetof(HordeGarrisonContain, m_exitBusyUntilFrame) == 0x9E0) ? 1 : -1];

#endif
