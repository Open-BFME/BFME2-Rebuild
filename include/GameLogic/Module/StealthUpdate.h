// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// StealthUpdate.h: promoted from Draft by f10 (2026-11-17): constructor 0x00374B02 and xfer 0x00374942
// are written against it. The two 1,024-bit upgrade masks at +0x48 and +0xC8 are not transferred by xfer;
// rva003748BD (0x003748BD) fills them from the module data's RequiredUpgradeNames / ForbiddenUpgradeNames.
// StealthUpdate.h: the StealthUpdate module. SIZE 0x14C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00374B02 calls UpdateModule first; it stores the
// tables 0x00C17F20 at +0x0, 0x00BEFF90 at +0xC, 0x00C17F14 at +0x10. Members are what xfer (0x00374942) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00374AE8 ?loadPostProcess@StealthUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x00373D0F ?name@Rva00373D0FNamed@@QBEPBDXZ
//   +0x0 slot 4 -> 0x00373CCA ?rva000373CCA@StealthUpdate@@SA?AW4NameKeyType@@XZ
//   +0x0 slot 5 -> 0x00374FD0 ?rva00374FD0@Rva00374FD0@@QAEXXZ
//   +0x10 slot 0 -> 0x003756A8 ?update@StealthUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 1 -> 0x004A2D67 ?getDisabledTypesToProcess@AIUpdateInterface@@UBE?AV?$BitFlags@$0L@@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?, Int also read as AsciiString at +0x38, slot4 +0x38.
#pragma once
#ifndef BFME2_STEALTH_UPDATE_H
#define BFME2_STEALTH_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/BitFlags11.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/StealthUpdateModuleData.h"
#include "Common/UpgradeMaskType.h"

// The condition names StealthForbiddenConditions reads (VA 0x00DC03DC in StealthUpdate's object;
// retail has a copy in each object that defines the guard). Zero Hour's name and guard.
#ifdef DEFINE_STEALTHLEVEL_NAMES
static const char *TheStealthLevelNames[] =
{
	"ATTACKING", "MOVING", "USING_ABILITY", "FIRING_PRIMARY", "FIRING_SECONDARY", "FIRING_TERTIARY",
	"FIRING_QUATERNARY", "FIRING_QUINARY", "MOUNTED", "AWAY_FROM_TREES", "HORDEBRAIN_NOT_STEALTHED",
	"PRIMARY_FORMATION", "TAKING_DAMAGE",
	NULL
};
#endif

class Thing;
class ModuleData;
class Object;
class Xfer;
class ThingTemplate;
class Player;
class Coord3D;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char StealthUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

// The setStatusCallback record (0x003743BB): the status bit at +0 and the set flag at +4.
enum ObjectStatusTypes;
struct StealthStatusCallbackData
{
	ObjectStatusTypes status;
	Bool set;
};

// Return values of StealthUpdate::calcStealthedStatusForPlayer 0x003742E3 (0..5, read from its returns).
enum StealthLookType
{
	STEALTH_LOOK_NONE = 0,
	STEALTH_LOOK_VISIBLE_FRIENDLY = 1,
	STEALTH_LOOK_DISGUISED_ENEMY = 2,
	STEALTH_LOOK_VISIBLE_DETECTED = 3,
	STEALTH_LOOK_VISIBLE_FRIENDLY_DETECTED = 4,
	STEALTH_LOOK_INVISIBLE = 5
};

class StealthUpdate : public UpdateModule
{
public:
	StealthUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~StealthUpdate();
	virtual UpdateSleepTime update();   // UpdateModuleInterface slot 0 (+0x10 table; 0x003756A8, defined in the next object)
	virtual DisabledMaskType getDisabledTypesToProcess() const { return DisabledMaskType(DisabledMaskType::kInit, 3); }	// UpdateModuleInterface slot 1: retail 0x004A2D67 (DISABLED_HELD only), as AutoAbilityBehavior
	virtual void onObjectCreated();   // slot 5 (0x00374FD0: a 5-byte `jmp rva003748BD`)

protected:
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess();   // 0x00374AE8: UpdateModule::loadPostProcess, then m_unk0x3C sets m_unk0x47, then rva003748BD (tail)

public:
	StealthUpdate *rva00373D28(Object *object);   // 0x00373D28 (public, placeholder name): object->rva002931F5(false) then its rva0028F4BC
	void rva00373EEC(UnsignedInt delay);   // 0x00373EEC (public, placeholder name): frame horizon; delay 0 sets m_unk0x24 = moduleData->m_stealthDelay + now
	UnsignedInt getStealthLevel() const;   // 0x00373D15 (public, QBE): m_unk0x2C when it is not negative, else module data +0xC (the forbidden-conditions word)
	void rva00373D5E();   // 0x00373D5E (public, placeholder name): finds the "ToggleHiddenSpecialAbilityUpdate" module and calls its slot 23 (turnOff)
	void disguiseAsObject(const Object *target);   // 0x00373DF0
	Bool rva003742B1();   // 0x003742B1 (public, placeholder name): forwards to rva00373F59 with the object's position and the producer's (or its own)
	Bool rva00373F59(const Coord3D *from, const Coord3D *to);   // 0x00373F59 (public, placeholder name; 856 B)
	StealthLookType calcStealthedStatusForPlayer(const Object *obj, const Player *player);   // 0x003742E3
	void markAsDetected(UnsignedInt framesDetected, Int detectionType, Object *detector, Bool flag);   // 0x0037446E
	void changeVisualDisguise();   // 0x00374BC8
	void setStatusOnSelfAndContained(const ObjectStatusTypes &status, Bool set);   // 0x003743CF (map: placeholder class Rva003743CF): sets the status on the AI via the callback, then the object
	void rva003748BD();   // 0x003748BD (public, placeholder name)
	Real getFriendlyOpacityMin() const;   // 0x00373D45: the module data's FriendlyOpacityMin (x87 result)
	UnsignedByte isOneRingReady();   // 0x00373EB6: the logic frame has reached the ring cooldown at +0x28 (updateOneRing tests the same word)
	Bool hasRequiredUpgrades() const;   // 0x00373EC6: the object's upgrades hold every required bit and no forbidden bit (the masks rva003748BD fills)
	void updateOneRing();   // 0x00374815 (map: placeholder class Rva00373EC6, which Object::rva0028F4BC hands back as the stealth module): One Ring start/refresh, f10
	protected:
	void hintDetectableWhileUnstealthed();   // 0x0037441E
	UpdateSleepTime calcSleepTime() const;   // 0x00373D4C (probe)
public:
	void rva00373D23();   // 0x00373D23 (public void, placeholder name)
	// Zero Hour's inline accessor (module data +0x8 StealthDelay): AIGroup::groupIdle 0x0036FC23 reads it in place.
	UnsignedInt getStealthDelay() const { return ((const StealthUpdateModuleData *)getModuleData())->m_stealthDelay; }
	UnsignedInt m_unk0x20;                         // +0x20; xfer UnsignedInt
	UnsignedInt m_unk0x24;                         // +0x24; xfer UnsignedInt
	UnsignedInt m_unk0x28;                         // +0x28; xfer UnsignedInt
	Int m_unk0x2C;                                 // +0x2C; xfer Int; signed (jge), read by getStealthLevel 0x00373D15
	Bool m_unk0x30;                                // +0x30; xfer Bool; read by calcSleepTime 0x00373D4C (set means NONE)
	Bool m_unk0x31;                                // +0x31; xfer Bool
	Bool m_unk0x32;                                // +0x32; xfer Bool
	Bool m_unk0x33;                                // +0x33; xfer Bool
	Bool m_unk0x34;                                // +0x34; xfer Bool
	Int m_unk0x38;                                 // +0x38; xfer Int; -1 in the constructor; disguiseAsObject 0x00373DF0 stores a Player index (+0x54) here
	const ThingTemplate *m_disguiseTemplate;       // +0x3C; xfer transfers its name (ThingTemplate +0x64) and finds it again on load; loadPostProcess 0x00374AE8 tests it
	UnsignedInt m_unk0x40;                         // +0x40; xfer UnsignedInt
	Bool m_unk0x44;                                // +0x44; xfer Bool
	Bool m_unk0x45;                                // +0x45; xfer Bool
	Bool m_unk0x46;                                // +0x46; xfer Bool
	Bool m_unk0x47;                                // +0x47; loadPostProcess 0x00374AE8 sets it to 1 when m_unk0x3C is set
	UpgradeMaskType m_requiredUpgradeMask;         // +0x48; constructor 0x00374B02 calls UpgradeMaskType() (0x001EAE6F); rva003748BD sets bit UpgradeTemplate+0x38 per RequiredUpgradeNames entry
	UpgradeMaskType m_forbiddenUpgradeMask;        // +0xC8; same, per ForbiddenUpgradeNames entry
	Bool m_unk0x148;                               // +0x148; xfer Bool
};
// SIZE 0x14C
typedef char StealthUpdate_size[(sizeof(StealthUpdate) == 0x14C) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x20[(offsetof(StealthUpdate, m_unk0x20) == 0x20) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x24[(offsetof(StealthUpdate, m_unk0x24) == 0x24) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x28[(offsetof(StealthUpdate, m_unk0x28) == 0x28) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x2C[(offsetof(StealthUpdate, m_unk0x2C) == 0x2C) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x30[(offsetof(StealthUpdate, m_unk0x30) == 0x30) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x31[(offsetof(StealthUpdate, m_unk0x31) == 0x31) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x32[(offsetof(StealthUpdate, m_unk0x32) == 0x32) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x33[(offsetof(StealthUpdate, m_unk0x33) == 0x33) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x34[(offsetof(StealthUpdate, m_unk0x34) == 0x34) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x38[(offsetof(StealthUpdate, m_unk0x38) == 0x38) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x40[(offsetof(StealthUpdate, m_unk0x40) == 0x40) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x44[(offsetof(StealthUpdate, m_unk0x44) == 0x44) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x45[(offsetof(StealthUpdate, m_unk0x45) == 0x45) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x46[(offsetof(StealthUpdate, m_unk0x46) == 0x46) ? 1 : -1];
typedef char StealthUpdate_off_m_requiredUpgradeMask[(offsetof(StealthUpdate, m_requiredUpgradeMask) == 0x48) ? 1 : -1];
typedef char StealthUpdate_off_m_forbiddenUpgradeMask[(offsetof(StealthUpdate, m_forbiddenUpgradeMask) == 0xC8) ? 1 : -1];
typedef char StealthUpdate_off_m_unk0x148[(offsetof(StealthUpdate, m_unk0x148) == 0x148) ? 1 : -1];

#endif
