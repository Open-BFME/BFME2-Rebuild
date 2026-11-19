// TunnelContain.h: the TunnelContain module. SIZE 0x9E8 (ModuleFactory operator new).
// Layout evidence: the constructor 0x0047DBF7 calls HordeGarrisonContain first; it stores the
// tables 0x00C47740 at +0x0, 0x00C43528 at +0xC, 0x00C47730 at +0x10, 0x00C475B8 at +0x20, 0x00C46064 at +0x24, 0x00C475B4 at +0x28, 0x00C463D8 at +0x2C, 0x00C488E0 at +0x30, 0x00C475A4 at +0x34. Members are what xfer (0x0047DBB0) transfers after HordeGarrisonContain::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004812B4 ?j_004812b4@@YAXXZ
//   +0x0 slot 2 -> 0x0047DB88 ?name@TunnelContain@@QBEPBDXZ
//   +0x0 slot 8 -> 0x0047DFFF ?onDelete@TunnelContain@@UAEXXZ
//   +0x0 slot 32 -> 0x0047DEE1 ?rva0047DEE1@TunnelContain@@UAEPAVRva0047DEE1Rider@@H@Z
//   +0x0 slot 33 -> 0x0047DD0F ?rva0047DD0F@TunnelContain@@UAE?AVRva0036AE51ListView@@XZ
//   +0x10 slot 0 -> 0x0047E017 ?update@TunnelContain@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 2 -> 0x004647CD ?get@Rva004647CDLeaField@@QBEPAXXZ
//   +0x20 slot 4 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x20 slot 7 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x20 slot 22 -> 0x0047DB95 ?bfmeGoBPC@@YGXPAVBfmeThingBPC@@PAX@Z
//   +0x20 slot 23 -> 0x0047DD2D ?onRemoving@TunnelContain@@UAEXPAVObject@@@Z
//   +0x20 slot 25 -> 0x0047E00F ?onSelling@TunnelContain@@UAEXXZ
//   +0x20 slot 28 -> 0x0047DDFF ?getContainMax@TunnelContain@@UBEHXZ
//   +0x20 slot 38 -> 0x0047DDA2 ?isValidContainerFor@TunnelContain@@UAE_NPAVObject@@_N1@Z
//   +0x20 slot 40 -> 0x0047DC75 ?addToContainList@TunnelContain@@UAEXPAVObject@@@Z
//   +0x20 slot 41 -> 0x0047DC88 ?removeFromContain@TunnelContain@@UAEXPAVObject@@H@Z
//   +0x20 slot 42 -> 0x0047E0CE ?removeAllContained@TunnelContain@@UAEXH@Z
//   +0x20 slot 58 -> 0x0047DF44 ?rva0047DF44@TunnelContain@@UAE_NH@Z
//   +0x20 slot 68 -> 0x0047DCDF ?iterateContained@TunnelContain@@UAEXP6AXPAX0@Z0I@Z
//   +0x20 slot 69 -> 0x0047DDD9 ?rva0047DDD9@TunnelContain@@UAEHPAX@Z
//   +0x20 slot 70 -> 0x0047DE12 ?rva0047DE12@TunnelContain@@UAE?AVRva0036AE51ListView@@XZ
//   +0x28 slot 0 -> 0x0047DFD9 ?onDie@TunnelContain@@UAEXPBVDamageInfo@@@Z
//   +0x34 slot 1 -> 0x0047DE30 ?rva0047DE30@TunnelContain@@UAEXXZ
//   +0x34 slot 3 -> 0x0047DB8E ?get@Rva0047DB8EByteField@@QBEEXZ
//   +0x34 slot 5 -> 0x0046261B ?rva0046261B@SlaughterHordeContain@@UAEX_N@Z
//   +0x34 slot 8 -> 0x004624E3 ?containReactToTransformChange@OpenContain@@UAEXXZ
//   +0x34 slot 15 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x34 slot 19 -> 0x0046294E ?get@Rva0046294EByteChaseField@@QBEEXZ
//   +0x34 slot 20 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x34 slot 21 -> 0x004639D5 ?rva004639D5@Rva004639D5@@UAEHH@Z
//   +0x34 slot 22 -> 0x00464FC7 ?rva00464FC7@SlaughterHordeContain@@UAE_NPAXH@Z
//   +0x34 slot 23 -> 0x0046322C ?rva0046322C@Rva0046322C@@QAEHXZ
//   +0x34 slot 24 -> 0x004780E7 ?rva004780E7@SlaughterHordeContain@@UAEPAVPlayer@@PBV2@@Z
//   +0x34 slot 25 -> 0x00479432 ?recalcApparentControllingPlayer@GarrisonContain@@UAEXXZ
//   +0x34 slot 26 -> 0x004783DC ?setOriginalTeam@Rva004783F4@@UAEXPAVTeam@@@Z
//   +0x34 slot 29 -> 0x0046481D ?rva0050B238@Rva005E62F1@@WCA@AEXPAXI@Z
//   +0x34 slot 31 -> 0x00479C0C ?rva00479C0C@HordeGarrisonContain@@UAEXPAVObject@@PAXPAUDamageInfoInput00462516@@@Z
//   +0x34 slot 32 -> 0x00479C2A ?rva0050B238@HordeGarrisonContain@@UAEXPAVObject@@_N@Z
//   +0x34 slot 34 -> 0x004647F0 ?Rva004647F0Adjust@@YIPAXPAX@Z
//   +0x34 slot 37 -> 0x00479BF3 ?rva00465011@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x34 slot 38 -> 0x00465078 ?rva00465078@OpenContainActionIface@@UAEXW4CommandSourceType@@@Z
//   +0x34 slot 40 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x34 slot 41 -> 0x00463138 ?rva00463138@OpenContainActionIface@@UAEXXZ
//   +0x34 slot 44 -> 0x00479D6D ?addToContain@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x34 slot 48 -> 0x0046274C ?rva0046274C@SlaughterHordeContain@@UAEXPAXH0@Z
//   +0x34 slot 49 -> 0x00462CE1 ?rva00462CE1@SlaughterHordeContain@@UAEPAVBfmeObject872Header@@PAV2@H@Z
//   +0x34 slot 52 -> 0x00462B93 ?rva00462B93@SlaughterHordeContain@@UAE_NPBVRva00462B93Arg@@@Z
//   +0x34 slot 53 -> 0x00462BFF ?rva00462BFF@OpenContainActionIface@@UBEDPBVObject@@0@Z
//   +0x34 slot 58 -> 0x004647D1 ?Rva004647D1Get@@YAMXZ
//   +0x34 slot 60 -> 0x004628AB ?rva004628AB@OpenContainActionIface@@UAE_NHH@Z
//   +0x34 slot 62 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x34 slot 66 -> 0x00479BDD ?rva004632E0@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x34 slot 68 -> 0x00464810 ?set@Rva00464810ByteSlot@@QAEXE@Z
//   +0x34 slot 69 -> 0x00462735 ?rva00462735@Rva00462735@@QAEXXZ
//   +0x34 slot 70 -> 0x00462CD3 ?get@Rva00462CD3Sub@@QBE_NXZ
//   +0x34 slot 71 -> 0x00463191 ?onContaining@TransportContain@@UAEXPAVObject@@_N@Z
//   +0x34 slot 72 -> 0x004631C9 ?rva004631C9@SlaughterHordeContain@@UAEXXZ
//   +0x34 slot 76 -> 0x004624EB ?rva004624EB@Rva004624EB@@UAE?AURva00462465@@XZ
//   +0x34 slot 78 -> 0x005C4AFD ?vslot143@AIUpdateInterface@@UBE?AW4CommandSourceType@@XZ
//   +0x34 slot 81 -> 0x00478547 ?calcBestGarrisonPosition@GarrisonContain@@UAE_NPAUCoord3D@@PBU2@@Z
//   +0x34 slot 82 -> 0x004788CE ?attemptBestFirePointPosition@GarrisonContain@@UAE_NPAVObject@@PAVWeapon@@PBUCoord3D@@@Z
//   +0x34 slot 83 -> 0x00478850 ?attemptBestFirePointPosition@GarrisonContain@@UAE_NPAVObject@@PAVWeapon@@0@Z
//   +0x34 slot 84 -> 0x00463291 ?rva00463291@OpenContainActionIface@@UAEPAVObject@@PAVRva2225E0Filter@@@Z
//   +0x34 slot 86 -> 0x003F3FCE ?get@Rva003F3FCEDwordField@@QBEHXZ
//   +0x34 slot 87 -> 0x004642D3 ?rva004642D3@SlaughterHordeContain@@UAEXXZ
//   +0x34 slot 88 -> 0x004647FD ?set@Rva004647FDByteSlot@@QAEXE@Z
//   +0x34 slot 89 -> 0x0046333B ?rva0046333B@Rva0046333B@@QAE_NPAH00000@Z
//   +0x34 slot 90 -> 0x00464825 ?get@Rva00464825DwordField@@QBEHXZ
//   +0x34 slot 91 -> 0x00479F29 ?rva004626CA@HordeGarrisonContain@@UAEPBUCoord3D@@XZ
//   +0x34 slot 92 -> 0x00479F62 ?rva004626AF@HordeGarrisonContain@@UAEPBUCoord3D@@XZ
//   +0x34 slot 93 -> 0x00479F9B ?rva00464829@HordeGarrisonContain@@UAEPBUCoord3D@@XZ
//   +0x34 slot 94 -> 0x0046480A ?rva0046480A@Rva0046480A@@QAEHH@Z
//   +0x34 slot 95 -> 0x004625BB ?rva004625BB@SlaughterHordeContain@@UAE_NPAXH@Z
//   +0x34 slot 96 -> 0x004625F0 ?rva004625F0@SlaughterHordeContain@@UAEXPAX@Z
//   +0x34 slot 97 -> 0x00462955 ?rva00462955@OpenContainActionIface@@UAE_NPAVObject@@H@Z
//   +0x34 slot 98 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x34 slot 100 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x34 slot 101 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_TUNNEL_CONTAIN_H
#define BFME2_TUNNEL_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/HordeGarrisonContain.h"
#include "GameLogic/Module/TunnelContainModuleData.h"

class Thing;
class ModuleData;
class Xfer;
class Object;

// HordeGarrisonContain ends at 0x9E4; this class's members follow it.
typedef char TunnelContain_base_size[(sizeof(HordeGarrisonContain) == 0x9E4) ? 1 : -1];

class TunnelContain : public HordeGarrisonContain
{
public:
	// Its own primary-table slots after HordeGarrisonContain's, from table 0x00847740 (34 slots):
	virtual Object *tunnelSlot32(Int arg);									// 32: 0x0047DEE1, the first contained object whose horde interface accepts arg
	virtual ContainmentListView tunnelSlot33();								// 33: 0x0047DD0F
	TunnelContain(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual Bool containSlot52() { return false; }
	virtual TunnelContain *asTunnelContain() { return this; }
	virtual bool isGarrisonable() const { return false; }
	virtual Bool isImmuneToClearBuildingAttacks() const { return true; }
	virtual Int getContainMax() const;			// 0x0047DDFF: forwards to the controlling player's tunnel tracker (Player +0x2E8)
	virtual ContainmentListView getList();		// 0x0047DE12: ContainModuleInterface slot 70 (the primary-this twin at 0x0047DD0F is not a table entry we write)
	virtual UnsignedInt getCount(bool);			// 0x0047DDD9: ContainModuleInterface slot 69; ignores its argument and asks the tracker for the full count
	virtual Bool isValidContainerFor(Object *obj, Bool checkCapacity, Bool flag);	// 0x0047DDA2
	virtual void addToContainList(Object *obj);	// 0x0047DC75: ContainModuleInterface slot 40 (this at +0x20); the controlling player's tunnel tracker takes it
	virtual void iterateContained(ContainIterateFunc func, void *userData, UnsignedInt flags);	// 0x0047DCDF: slot 68 (the map spells the callback (void *, void *))
	virtual void removeAllContained(Bool exposeStealthUnits);	// 0x0047E0CE: slot 42 (the map spells the argument Int)
	virtual void onDelete();	// 0x0047DFFF: the base's, then rva0047DF81
	virtual void onContaining(Object *obj, Bool wasSelected);	// 0x0047DB95 (the map names it bfmeGoBPC, a placeholder; this is ContainModuleInterface slot 22)
	virtual Bool shouldDoOnBuildComplete() const { return m_needToRunOnBuildComplete; }	// 0x0047DB8E: CreateModuleInterface slot 3 of Tunnel's table reads +0x9E4
	virtual void onBuildComplete();	// 0x0047DE30
	virtual void onRemoving(Object *obj);	// 0x0047DD2D: ContainModuleInterface slot 23
	virtual void onDie(const DamageInfo *damageInfo);	// 0x0047DFD9: DieModuleInterface slot 0 (this at +0x28)
	void rva0047DF81();	// 0x0047DF81 (onDelete's tail call)
	const char *name() const;						// 0x0047DB88 (non-virtual, the literal)
	const TunnelContainModuleData *getTunnelContainModuleData() const { return (const TunnelContainModuleData *)getModuleData(); }
	virtual ~TunnelContain();

protected:
	virtual void xfer(Xfer *xfer);
	void scatterToNearbyPosition(Object *obj);	// 0x0047E16B

public:
	Bool m_needToRunOnBuildComplete;               // +0x9E4; xfer Bool, true from the constructor; 0x0047DE30 (the onBuildComplete body) clears it
	Bool m_isCurrentlyRegistered;                  // +0x9E5; xfer Bool; 0x0047DE30 sets it after tunnel->onTunnelCreated, rva0047DF81 clears it after onTunnelDestroyed
public:
	virtual UpdateSleepTime update();	// 0x0047E017, UpdateModuleInterface slot 0; retail's table has this class's own
protected:
	virtual void loadPostProcess() { HordeGarrisonContain::loadPostProcess(); }	// slot 1: the forwarder retail folds at 0x004812B4 (Tunnel, ProductionQueueHorde, SlaughterHorde)
};
// SIZE 0x9E8
typedef char TunnelContain_size[(sizeof(TunnelContain) == 0x9E8) ? 1 : -1];
typedef char TunnelContain_off_m_needToRunOnBuildComplete[(offsetof(TunnelContain, m_needToRunOnBuildComplete) == 0x9E4) ? 1 : -1];
typedef char TunnelContain_off_m_isCurrentlyRegistered[(offsetof(TunnelContain, m_isCurrentlyRegistered) == 0x9E5) ? 1 : -1];

#endif
