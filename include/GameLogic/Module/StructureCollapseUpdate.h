// StructureCollapseUpdate.h: the update module that makes a destroyed structure shudder, sink and burst apart.
// SIZE 0x44 (ModuleFactory operator new). Promoted out of Draft by f08 (2026-11-17).
// Layout is Zero Hour's: class StructureCollapseUpdate : public UpdateModule, public DieModuleInterface.
// Target facts (retail game.dat):
// - Ctor 0x004A42E4 calls UpdateModule's, stores the tables 0x00C52648 at +0, 0x00C5263C at +0x10 (update interface)
//   and 0x00C52638 at +0x20 (the die interface's, after its own pure table 0x00C1C780); no store at +0xC, so the
//   behavior interface table at +0xC is the one UpdateModule installs, with slot 7 (getDie) overridden by 0x004543A9. It zeroes +0x24..+0x34 and +0x38..+0x40 and calls setWakeFrame(UPDATE_SLEEP_FOREVER).
// - Primary table 0x00C52648: 0 deleting dtor 0x004A436F, 1 loadPostProcess 0x00497805 (a jmp to UpdateModule's, folded
//   across classes), 2 GetSnapshotName 0x004A4234, 3 xfer 0x004A4AB2, 4 getModuleNameKey 0x004A429F. It has no virtual of its own.
// - Update interface: slot 0 update 0x004A46BF (this = +0x10), slot 1 getDisabledTypesToProcess 0x004A4580 (one bit, 1).
// - Die interface slot 0: onDie 0x004A466E (this = +0x20).
// - Members, from xfer 0x004A4AB2: +0x24 collapse frame, +0x28 burst frame, +0x2C state (four raw bytes), +0x30 collapse velocity,
//   +0x34 current height, +0x38 the position the collapse is measured from (BFME 2 caches it in beginStructureCollapse).
// Names follow Zero Hour's StructureCollapseUpdate.h (EA, GPL-3.0-or-later).
#pragma once
#ifndef BFME2_STRUCTURE_COLLAPSE_UPDATE_H
#define BFME2_STRUCTURE_COLLAPSE_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/DieModuleInterface.h"
#include "Common/Coord3D.h"

class Thing;
class ModuleData;
class Xfer;
class DamageInfo;
class Coord3D;

// The collapse phases; doPhaseStuff 0x004A43E9 takes one. Five phases (COLLAPSE_PHASE_COUNT in the module data); their names
// are the entries of TheStructureCollapsePhaseNames (VA 0x00DCBC8C) that the OCL and FXList INI lines index.
enum StructureCollapsePhaseType
{
	STRUCTURE_COLLAPSE_PHASE_INITIAL = 0,
	STRUCTURE_COLLAPSE_PHASE_DELAY,
	STRUCTURE_COLLAPSE_PHASE_BURST,
	STRUCTURE_COLLAPSE_PHASE_ALMOST_FINAL,
	STRUCTURE_COLLAPSE_PHASE_FINAL
};

// The state of the collapse (Zero Hour's StructureCollapseStateType); xfer transfers it as four raw bytes.
enum StructureCollapseStateType
{
	STRUCTURE_COLLAPSE_STATE_STANDING = 0,
	STRUCTURE_COLLAPSE_STATE_WAITING_FOR_COLLAPSE_START,
	STRUCTURE_COLLAPSE_STATE_COLLAPSING,
	STRUCTURE_COLLAPSE_STATE_DONE
};

// UpdateModule ends at 0x20; this class's members follow it.
typedef char StructureCollapseUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class StructureCollapseUpdate : public UpdateModule, public DieModuleInterface
{
public:
	StructureCollapseUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~StructureCollapseUpdate();	// 0x004A4214: restores the four tables and jumps to ~UpdateModule (deleting thunk 0x004A436F)
	virtual NameKeyType getModuleNameKey() const;	// slot 4, 0x004A429F (the map spells it as a static of the class: rva0004A429F)
	virtual const char *GetSnapshotName() const;
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004A46BF)
	virtual DisabledMaskType getDisabledTypesToProcess() const;	// override: update slot 1 at +0x10 (0x004A4580)
	virtual DieModuleInterface *getDie() { return this; }	// override: BehaviorModuleInterface slot 7 (retail 0x004543A9: the null-checked interface pointer)
	virtual void onDie(const DamageInfo *damageInfo);	// 0x004A466E, DieModuleInterface slot 0 (this = +0x20)

protected:
	virtual void xfer(Xfer *xfer);
	void beginStructureCollapse(const DamageInfo *damageInfo);	// 0x004A459B (IAE)
	void doCollapseDoneStuff();					// 0x004A44B1 (IAE)
	Real getCollapseHeight() const;					// 0x004A438B (IBE): module height, or the template geometry's max height when destroyed when done
	void doPhaseStuff(StructureCollapsePhaseType phase, const Coord3D *pos);	// 0x004A43E9 (IAE): declared for beginStructureCollapse

public:
	UnsignedInt m_collapseFrame;                   // +0x24; xfer UnsignedInt; beginStructureCollapse 0x004A459B stores the frame the collapse starts
	UnsignedInt m_burstFrame;                      // +0x28; xfer UnsignedInt; update 0x004A46BF
	StructureCollapseStateType m_collapseState;    // +0x2C; xfer as 4 raw bytes (XferRawBytes); Zero Hour's m_collapseState
	Real m_collapseVelocity;                                // +0x30; xfer Real
	Real m_currentHeight;                                // +0x34; xfer Real
	Coord3D m_collapsePosition;                             // +0x38 (x), +0x3C (y), +0x40 (z); xfer as three Reals
};
// SIZE 0x44
typedef char StructureCollapseUpdate_size[(sizeof(StructureCollapseUpdate) == 0x44) ? 1 : -1];
typedef char StructureCollapseUpdate_off_m_collapseFrame[(offsetof(StructureCollapseUpdate, m_collapseFrame) == 0x24) ? 1 : -1];
typedef char StructureCollapseUpdate_off_m_burstFrame[(offsetof(StructureCollapseUpdate, m_burstFrame) == 0x28) ? 1 : -1];
typedef char StructureCollapseUpdate_off_m_collapseVelocity[(offsetof(StructureCollapseUpdate, m_collapseVelocity) == 0x30) ? 1 : -1];
typedef char StructureCollapseUpdate_off_m_currentHeight[(offsetof(StructureCollapseUpdate, m_currentHeight) == 0x34) ? 1 : -1];
typedef char StructureCollapseUpdate_off_m_collapsePosition[(offsetof(StructureCollapseUpdate, m_collapsePosition) == 0x38) ? 1 : -1];

#endif
