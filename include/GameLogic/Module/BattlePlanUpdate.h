// Promoted from Draft by f10 (2026-11-17): constructor 0x00497ED0, destructor 0x004978A3 and xfer 0x00498517 are written against it.
// +0x40 is a BattlePlanBonuses (allocated in the constructor, never freed by the destructor); +0x44..+0x84 sixteen AsciiStrings
// (the constructor runs the eh vector constructor over them, the destructor the vector destructor); +0x84 an AudioHandle the
// destructor removes (TheAudio slot 0x6C) and xfer transfers (slot 0x160); +0x88 the vision object the update creates.
// BattlePlanUpdate.h: the BattlePlanUpdate module. SIZE 0x8C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00497ED0 calls UpdateModule first; it stores the
// tables 0x00C6F248 at +0x20, 0x00C4FBAC at +0x0, 0x00C4D640 at +0xC, 0x00C4FBA0 at +0x10, 0x00C4FB7C at +0x20. Members are what xfer (0x00498517) transfers after UpdateModule::xfer,
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
//   +0x0 slot 2 -> 0x0049795A ?name@Rva0049795ANamed@@QBEPBDXZ
//   +0x0 slot 5 -> 0x0049797F ?onObjectCreated@BattlePlanUpdate@@UAEXXZ
//   +0x0 slot 8 -> 0x004984CC ?cleanUp@SpectreGunshipUpdate@@QAEXXZ
//   +0xC slot 25 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x10 slot 0 -> 0x0049862B ?update@Rva0049862B@@QAEHXZ
//   +0x20 slot 0 -> 0x004976DB ?bfmeTransition@Gen_002854E0@@QAEXHHHIH@Z
//   +0x20 slot 1 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x20 slot 2 -> 0x00497960 ?get@Rva00497960CmpBoolField@@QBE_NXZ
//   +0x20 slot 3 -> 0x0049771E ?getCommandOption@BattlePlanUpdate@@UBE?AW4CommandOption@@XZ
//   +0x20 slot 5 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x20 slot 6 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x20 slot 7 -> 0x00497969 ?equals@Rva002857B0Object@@QAE_NH@Z
//   +0x20 slot 8 -> 0x005CB9FA ?_bfme_ro_v50@RenderObjClass@@UAE_NH@Z
//   +0x20 slot 10 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 11 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x20 slot 18 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x20 slot 21 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 22 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x20 slot 24 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?, XferRawBytes +0x24, XferRawBytes +0x28, XferRawBytes +0x2C, XferRawBytes +0x30, Real ?, Int at misaligned +0x3D, Int at misaligned +0x3D, Int at misaligned +0x3D, Real at misaligned +0x3D, ?xfer@?$BitFlags@$0NK@@@QAEXPAVXfer@@@Z ?, ?xfer@?$BitFlags@$0NK@@@QAEXPAVXfer@@@Z ?, slot160 ?.
// Interfaces after the base (own table -> class): +0x20 SpecialPowerUpdateInterface.
#pragma once
#ifndef BFME2_BATTLE_PLAN_UPDATE_H
#define BFME2_BATTLE_PLAN_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/SpecialPowerUpdateInterface.h"
#include "Common/AsciiString.h"
#include "Common/GameAudio.h"
#include "Common/AudioEventRef.h"

class Thing;
class ModuleData;
class Xfer;
class BattlePlanBonuses;
class SpecialPowerModuleInterface;
class Waypoint;
class SpecialPowerTemplate;

// Zero Hour's names for the plan and the transition (the update switches on both).
enum BattlePlanStatus
{
	PLANSTATUS_NONE = 0,
	PLANSTATUS_BOMBARDMENT = 1,
	PLANSTATUS_HOLDTHELINE = 2,
	PLANSTATUS_SEARCHANDDESTROY = 3
};

enum TransitionStatus
{
	TRANSITIONSTATUS_IDLE = 0,
	TRANSITIONSTATUS_UNPACKING = 1,
	TRANSITIONSTATUS_ACTIVE = 2,
	TRANSITIONSTATUS_PACKING = 3
};

// UpdateModule ends at 0x20; this class's members follow it.
typedef char BattlePlanUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class BattlePlanUpdate : public UpdateModule, public SpecialPowerUpdateInterface
{
public:
	virtual SpecialPowerUpdateInterface *getSpecialPowerUpdateInterface() { return this; }
	BattlePlanUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~BattlePlanUpdate();
	virtual void onObjectCreated();			// 0x0049797F: the primary table slot 5
	virtual void onDelete();				// slot 8, 0x004984CC (the map spells the row ?cleanUp@SpectreGunshipUpdate@@QAEXXZ)
	virtual UpdateSleepTime update();		// UpdateModuleInterface slot 0 (+0x10 table), 0x0049862B
	// SpecialPowerUpdateInterface slot 0 (+0x20 table), 0x004976DB: records the command option as the desired plan.
	virtual void initiateIntentToDoSpecialPower(const SpecialPowerTemplate *specialPowerTemplate, const Object *targetObj,
		const Coord3D *targetPos, UnsignedInt commandOptions, const Waypoint *way);

protected:
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 is the 13-byte jmp thunk 0x00497805 in this class's own range (as BoredUpdate's 0x00497805-style slot)
	// Turret helpers act on the object's AI interface (Object +0x258): 0x0049773F, 0x00497769, 0x0049778D.
	void enableTurret(Bool enable);
	void setStatus(TransitionStatus newStatus);	// 0x00497FF3
	void recenterTurret();
	Bool isTurretInNaturalPosition();

public:
	void createVisionObject();			// 0x00497A4B (QAE: public): creates the neutral vision object once
	void rva00497AE8(BattlePlanStatus newPlan);	// 0x00497AE8 (public, placeholder name): removes the active plan's bonuses, then applies the new plan's
	// SpecialPowerUpdateInterface slot 3 (+0x20 interface), retail 0x0049771E: the plan's command option.
	// SpecialPowerUpdateInterface slot 2 (+0x20 table), retail 0x00497960: a plan is unpacking, active or packing.
	virtual bool isSpecialAbility() const { return false; }	// SpecialPowerUpdateInterface slot 1 (+0x20 table 0x0084FB7C): the return-false fold 0x0047A699 (h05 r13)
	virtual bool isActive() const;
	virtual CommandOption getCommandOption() const;
	// SpecialPowerUpdateInterface slots 4-8 (+0x20 table 0x0084FB7C): 4 the false fold 0x0047A699, 5 the `ret 4` fold
	// 0x0047A69C, 6 `xor al,al; ret 4` 0x005CB9FF, 7 its own 0x00497969, 8 `mov al,1; ret 4` 0x005CB9FA.
	virtual bool doesSpecialPowerHaveOverridableDestinationActive() const { return false; }
	virtual void setSpecialPowerOverridableDestination(const Coord3D *loc) {}
	virtual bool isPowerCurrentlyInUse(const CommandButton *command = 0) const { return false; }
	virtual bool vslot07(int value);
	virtual bool vslot08(int) { return true; }
	// Retail 0x004977F8 (non-virtual): the active plan, only while m_status is 2.
	BattlePlanStatus getActiveBattlePlan() const;

	Int m_currentPlan;                                 // +0x24; xfer raw 4 bytes (an enum)
	Int m_desiredPlan;                             // +0x28; read by getCommandOption (1, 2, 3); xfer raw 4 bytes
	Int m_planAffectingArmy;                              // +0x2C; returned by getActiveBattlePlan (0x004977F8); xfer raw 4 bytes
	Int m_status;                              // +0x30; getActiveBattlePlan tests it against 2 (0x004977F8); xfer raw 4 bytes
	UnsignedInt m_nextReadyFrame;                         // +0x34; xfer UnsignedInt
	SpecialPowerModuleInterface *m_specialPowerModule;                         // +0x38; the constructor zeroes it; onObjectCreated 0x0049797F stores Object::rva0028BB9E's result
	Bool m_invalidSettings;                                // +0x3C; xfer Bool
	Bool m_centeringTurret;                                // +0x3D; xfer Bool
	BattlePlanBonuses *m_bonuses;                  // +0x40
	PooledAudioEventRTSRef m_unk0x44[16];          // +0x44..+0x84 (4-byte refcounted audio refs: onObjectCreated assigns them through OpaqueRefElement4::operator= 0x00239099; the eh vector stubs are its constructor 0x00726BE6 and destructor 0x0050F149)
	AudioHandle m_audioHandle;                     // +0x84; 1 in the constructor
	ObjectID m_visionObjectID;                            // +0x88; xfer ObjectID
};
// SIZE 0x8C
typedef char BattlePlanUpdate_size[(sizeof(BattlePlanUpdate) == 0x8C) ? 1 : -1];
typedef char BattlePlanUpdate_off_m_nextReadyFrame[(offsetof(BattlePlanUpdate, m_nextReadyFrame) == 0x34) ? 1 : -1];
typedef char BattlePlanUpdate_off_m_bonuses[(offsetof(BattlePlanUpdate, m_bonuses) == 0x40) ? 1 : -1];
typedef char BattlePlanUpdate_off_m_audioHandle[(offsetof(BattlePlanUpdate, m_audioHandle) == 0x84) ? 1 : -1];
typedef char BattlePlanUpdate_off_m_invalidSettings[(offsetof(BattlePlanUpdate, m_invalidSettings) == 0x3C) ? 1 : -1];
typedef char BattlePlanUpdate_off_m_centeringTurret[(offsetof(BattlePlanUpdate, m_centeringTurret) == 0x3D) ? 1 : -1];
typedef char BattlePlanUpdate_off_m_visionObjectID[(offsetof(BattlePlanUpdate, m_visionObjectID) == 0x88) ? 1 : -1];

#endif
