// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// StructureToppleUpdate.h: the structure-topple update module (a building that falls over when
// it is destroyed). SIZE 0x58.
// Target facts (retail game.dat):
// - Layout: UpdateModule (0x20; its UpdateModuleInterface at +0x10), then the DieModuleInterface
//   at +0x20 (its table 0x00C1C780, one slot onDie). The members follow it.
// - Dtor 0x004A54A8 is empty (the base chain restores the tables).
// - getModuleNameKey 0x004A5511 is the pool key; GetSnapshotName 0x004A54C8 returns the literal name.
// - xfer 0x004A58BD: UpdateModule::xfer, then IsLightCRC (slot 4) skips the rest, then Version1,
//   then unsigned int at +0x24 (slot 30), Coord2D at +0x28 (slot 20), four raw bytes at +0x30
//   (XferRawBytes), four reals at +0x34..+0x40 (slot 28), int at +0x44 (slot 31) and a
//   Coord3DBase at +0x48 (slot 24). The real at +0x54 is not transferred.
// - Ctor 0x004A5556 stores -1 into +0x44, zeroes +0x24, +0x28..+0x30 and +0x3C..+0x50, and calls
//   setWakeFrame(getObject(), UPDATE_SLEEP_FOREVER). Its value at +0x54 comes from a call on
//   m_object+0xA8 (0x006BD7C0, not named yet), so the ctor is not written here.
// - onDie 0x004A5E5B overrides DieModuleInterface's slot 0 through this = +0x20.
// Not established yet: the member names (role names are placeholders below) and the call at 0x006BD7C0.
#pragma once
#ifndef BFME2_STRUCTURETOPPLEUPDATE_H
#define BFME2_STRUCTURETOPPLEUPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord2D.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/DieModuleInterface.h"

class Thing;
class ModuleData;
class Xfer;
class DamageInfo;
class Object;
class WeaponTemplate;

// The phases doPhaseStuff 0x004A584B selects the OCL list of (module data +0x70 + phase * 0xC). Retail's name
// table TheStructureTopplePhaseNames (VA 0x00DCBD30: "INITIAL", "DELAY", "FINAL") is what the "OCL" INI line
// indexes; doToppleStartFX passes the first.
enum StructureTopplePhaseType
{
	STRUCTURE_TOPPLE_PHASE_INITIAL = 0,
	STRUCTURE_TOPPLE_PHASE_DELAY = 1,
	STRUCTURE_TOPPLE_PHASE_FINAL = 2,
};
// Retail has a copy in each object that includes this header (two: VA 0x00DCBD30 is
// StructureToppleUpdate's), so it is a header static, as in Zero Hour.
static const char *TheStructureTopplePhaseNames[] =
{
	"INITIAL", "DELAY", "FINAL",
	NULL
};

// The state byte group at +0x30 (xfer moves it as four raw bytes).
enum StructureToppleStateType
{
	TOPPLESTATE_STANDING = 0,
	TOPPLESTATE_WAITINGFORTOPPLESTART = 1,
	TOPPLESTATE_TOPPLING = 2,
	TOPPLESTATE_WAITINGFORDONE = 3,
	TOPPLESTATE_DONE = 4,
};

// UpdateModule ends at 0x20; the die interface and this class's members follow it.
typedef char StructureToppleUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class StructureToppleUpdate : public UpdateModule, public DieModuleInterface
{
public:
	StructureToppleUpdate(Thing *thing, const ModuleData *moduleData);	// 0x004A5556
	virtual ~StructureToppleUpdate();									// 0x004A54A8
	virtual NameKeyType getModuleNameKey() const;						// 0x004A5511
	virtual void onDie(const DamageInfo *damageInfo);					// 0x004A5E5B
	virtual UpdateSleepTime update();									// override: update slot 0 at +0x10 (0x004A6042)
	virtual DieModuleInterface *getDie() { return this; }				// override: BehaviorModuleInterface slot 7 (retail 0x004543A9: the null-checked interface pointer)

protected:
	virtual const char *GetSnapshotName() const;						// 0x004A54C8
	void beginStructureTopple(const DamageInfo *damageInfo);			// 0x004A5C8C (IAE)
	virtual void xfer(Xfer *xfer);										// 0x004A58BD
	void doAngleFX(Real fromAngle, Real toAngle);						// 0x004A57DB (IAE)
	void doPhaseStuff(StructureTopplePhaseType phase, const Coord3D *position);	// 0x004A584B (IAE)
	void doToppleStartFX(Object *obj, const DamageInfo *damageInfo);	// 0x004A5B09 (IAE)
	void doToppleDoneStuff();											// 0x004A5628 (IAE)
	void doDamageLine(Object *building, const WeaponTemplate *weaponTemplate, Real jcos, Real jsin, Real facingWidth, Real toppleAngle);	// 0x004A5947 (IAE)
	void doToppleDelayBurstFX();										// 0x004A5B61 (IAE)
	void applyCrushingDamage(Real theta);								// 0x004A5EAC (IAE)

public:
	UnsignedInt m_toppleFrame;												// +0x24
	Coord2D m_toppleDirection;													// +0x28
	StructureToppleStateType m_toppleState;							// +0x30, transferred as four raw bytes
	Real m_toppleVelocity;														// +0x34
	Real m_accumulatedAngle;														// +0x38
	Real m_structuralIntegrity;														// +0x3C
	Real m_lastCrushedLocation;														// +0x40
	Int m_nextBurstFrame;														// +0x44
	Coord3D m_delayBurstLocation;												// +0x48
	Real m_buildingHeight;														// +0x54, not transferred
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x58
typedef char StructureToppleUpdate_size[(sizeof(StructureToppleUpdate) == 0x58) ? 1 : -1];
typedef char StructureToppleUpdate_off_m_toppleFrame[(offsetof(StructureToppleUpdate, m_toppleFrame) == 0x24) ? 1 : -1];
typedef char StructureToppleUpdate_off_m_delayBurstLocation[(offsetof(StructureToppleUpdate, m_delayBurstLocation) == 0x48) ? 1 : -1];
typedef char StructureToppleUpdate_off_m_buildingHeight[(offsetof(StructureToppleUpdate, m_buildingHeight) == 0x54) ? 1 : -1];

#endif
