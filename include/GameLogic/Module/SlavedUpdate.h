// SlavedUpdate.h: the SlavedUpdate module. SIZE 0x48 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A18C5 calls UpdateModule first; it stores the
// tables 0x00C6F248 at +0x20, 0x00C51F1C at +0x0, 0x00C51E60 at +0xC, 0x00C51E54 at +0x10, 0x00C51E30 at +0x20. Members are what xfer (0x004A1B54) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x20 slot 9 -> 0x0009853C ?slot08@IconSlot@Impl@RegionDetailsStructuresMovieClip@StrategicHUD@@UBEEXZ
//   +0x20 slot 10 -> 0x0057B93E ?rva0057B93E@Rva0057B93E@@QAEXD@Z
//   +0x20 slot 11 -> 0x0057B97C ?rva0057B97C@Rva0057B97C@@QAEHXZ
//   +0x20 slot 12 -> 0x0057B9D0 ?rva0057B9D0@Rva0057B9D0@@QAEAAUTreeHintRef00217D4C@@ABU2@@Z
//   +0x20 slot 13 -> 0x0057B9D8 ?rva0057B9D8@Rva0057B9D8@@QAEXXZ
//   +0x20 slot 14 -> 0x0057B94D ?get@Rva0057B94DCmpBoolField@@QBE_NXZ
//   +0x20 slot 15 -> 0x0057B957 ?get@Rva0057B957CmpBoolField@@QBE_NXZ
//   +0x20 slot 16 -> 0x0057BC20 ?rva0057BC20@Rva0057BC20@@QAEXXZ
//   +0x20 slot 17 -> 0x0057BC25 ?rva0057BC25@SelectionDetailsUIImpl@StrategicHUD@@UAEXXZ
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 5 -> 0x004A1950 ?rva004A1950@Rva004A1950@@UAEXXZ
//   +0xC slot 26 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x10 slot 0 -> 0x004A265E ?update@SlavedUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 3 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 12 -> 0x002533D3 ?getDie@DieModule@@UAEPAVDieModuleInterface@@XZ
//   +0x10 slot 46 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x10 slot 47 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 59 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 62 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x20 slot 0 -> 0x005801F2 ?getKey@?$CategoryModuleClass@$0A@@FXParticleSystem@@QBEPBDXZ
//   +0x20 slot 1 -> 0x004A1BFD ?onEnslave@SlavedUpdate@@UAEXPBVObject@@@Z
//   +0x20 slot 2 -> 0x004A1C05 ?onSlaverDie@SlavedUpdate@@UAEXPBVRva00263895Member@@@Z
//   +0x20 slot 3 -> 0x004A1C59 ?onSlaverDamage@SlavedUpdate@@UAEXPBVRva00263895Member@@@Z
//   +0x20 slot 4 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x20 slot 5 -> 0x004A1942 ?get@Rva004A1942ByteChaseField@@QBEEXZ
//   +0x20 slot 6 -> 0x004A18C0 ?enable@Rva004A18C0OneSetter@@QAEXXZ
//   +0x20 slot 7 -> 0x0009D6ED ?get@Rva0009D6EDByteField@@QBEEXZ
//   +0x20 slot 8 -> 0x004A1949 ?get@Rva004A1949ByteChaseField@@QBEEXZ
//   +0x20 slot 10 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 11 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
// Not established: transfers xfer makes that this view cannot place: slot10 ?, XferRawBytes +0x38.
// Interfaces after the base (own table -> class): +0x20 SlavedUpdateInterface.
// Layout and slot facts proven by rows (see the notes on each member):
// - m_slaverID +0x24 is what getSlaverID (interface slot 0, folded into 0x005801F2) returns,
//   startSlavedEffects stores and update looks up.
// - m_goalPos +0x28 is the Coord3D startSlavedEffects fills and stopSlavedEffects zeroes.
// - m_waitFrames +0x34 and m_repairState +0x38: the constructor zeroes them, endRepair 0x004A1C10
//   resets the state and arms the wait, update counts the wait down (it runs through the
//   +0x10 UpdateModuleInterface, so its `this` is the object + 0x10).
// - m_unk0x44 is the byte interface slot 6 sets and slot 7 returns (0x004A18C0 / 0x0009D6ED).
#pragma once
#ifndef BFME2_SLAVED_UPDATE_H
#define BFME2_SLAVED_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/SlavedUpdateInterface.h"
#include "GameLogic/Module/SlavedUpdateModuleData.h"
#include "Common/Coord3D.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char SlavedUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class SlavedUpdate : public UpdateModule, public SlavedUpdateInterface
{
public:
	SlavedUpdate(Thing *thing, const ModuleData *moduleData);	// 0x004A18C5
	virtual NameKeyType getModuleNameKey() const;	// 0x004A1875
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004A265E)
	virtual void onObjectCreated();	// Module slot 5, 0x004A1950 (the map names it Rva004A1950::rva004A1950)
	virtual SlavedUpdateInterface *getSlavedUpdateInterface() { return this; }	// BehaviorModule slot 26 (folded 0x004543A9)

	// SlavedUpdateInterface (+0x20)
	virtual ObjectID getSlaverID() const { return m_slaverID; }	// slot 0, folded into 0x005801F2
	virtual void onEnslave(const Object *slaver) { startSlavedEffects(slaver); }	// slot 1, 0x004A1BFD
	virtual void onSlaverDie(const DamageInfo *info);	// slot 2, 0x004A1C05
	virtual void onSlaverDamage(const DamageInfo *info);	// slot 3, 0x004A1C59
	virtual Bool isSelfTasking() const { return false; }	// slot 4, folded into 0x0047A699
	virtual Bool vslot05() const { return static_cast<const SlavedUpdateModuleData *>(getModuleData())->m_useSlaverAsControlForEvaObjectSightedEvents; }	// slot 5, 0x004A1942
	virtual void vslot06() { m_unk0x44 = true; }	// slot 6, 0x004A18C0
	virtual Bool vslot07() const { return m_unk0x44; }	// slot 7, folded into 0x0009D6ED
	virtual Bool vslot08() const { return static_cast<const SlavedUpdateModuleData *>(getModuleData())->m_dieOnMastersDeath; }	// slot 8, 0x004A1949

	void startSlavedEffects(const Object *slaver);	// 0x004A1A69
	void stopSlavedEffects();	// 0x004A1B17
	void endRepair();	// 0x004A1C10
	void setRepairModelConditionStates(ModelConditionFlagType flag);	// 0x004A197D
	void moveToNewRepairSpot();							// 0x004A2144 (Zero Hour's name): a new spot within the repair range of the master

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	ObjectID m_slaverID;                           // +0x24; xfer ObjectID
	Coord3D m_goalPos;                         // +0x28; xfer Coord3D
	Int m_waitFrames;                              // +0x34; xfer Int
	Int m_repairState;                             // +0x38; constructor store
	Bool m_unk0x3C;                                // +0x3C; xfer Bool
	ObjectID m_unk0x40;                            // +0x40; xfer ObjectID
	Bool m_unk0x44;                                // +0x44; xfer Bool
	Bool m_unk0x45;                                // +0x45; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x48
typedef char SlavedUpdate_size[(sizeof(SlavedUpdate) == 0x48) ? 1 : -1];
typedef char SlavedUpdate_off_m_slaverID[(offsetof(SlavedUpdate, m_slaverID) == 0x24) ? 1 : -1];
typedef char SlavedUpdate_off_m_goalPos[(offsetof(SlavedUpdate, m_goalPos) == 0x28) ? 1 : -1];
typedef char SlavedUpdate_off_m_waitFrames[(offsetof(SlavedUpdate, m_waitFrames) == 0x34) ? 1 : -1];
typedef char SlavedUpdate_off_m_repairState[(offsetof(SlavedUpdate, m_repairState) == 0x38) ? 1 : -1];
typedef char SlavedUpdate_off_m_unk0x3C[(offsetof(SlavedUpdate, m_unk0x3C) == 0x3C) ? 1 : -1];
typedef char SlavedUpdate_off_m_unk0x40[(offsetof(SlavedUpdate, m_unk0x40) == 0x40) ? 1 : -1];
typedef char SlavedUpdate_off_m_unk0x44[(offsetof(SlavedUpdate, m_unk0x44) == 0x44) ? 1 : -1];
typedef char SlavedUpdate_off_m_unk0x45[(offsetof(SlavedUpdate, m_unk0x45) == 0x45) ? 1 : -1];

#endif
