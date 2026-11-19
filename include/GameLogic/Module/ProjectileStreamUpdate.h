// ProjectileStreamUpdate.h: the ProjectileStreamUpdate module. SIZE 0x8C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0033EE25 calls UpdateModule first; it stores the
// tables 0x00C10DA8 at +0x0, 0x00BEFF90 at +0xC, 0x00C10D9C at +0x10. Members are what xfer (0x0033EF8E) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x0033F11F ?update@ProjectileStreamUpdate@@UAE?AW4UpdateSleepTime@@XZ
#pragma once
#ifndef BFME2_PROJECTILE_STREAM_UPDATE_H
#define BFME2_PROJECTILE_STREAM_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "Common/Coord3D.h"

class Thing;
class ModuleData;
class Xfer;
class Vector3;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char ProjectileStreamUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class ProjectileStreamUpdate : public UpdateModule
{
public:
	ProjectileStreamUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	enum { MAX_PROJECTILE_STREAM = 20 };		// every index wraps with % 20 (idiv 0x14)

	void addProjectile(ObjectID sourceID, ObjectID newID, ObjectID victimID, const Coord3D *victimPos);	// 0x0033EEE2
	virtual UpdateSleepTime update();										// 0x0033F11F
	void getAllPoints(Vector3 *points, Int *count);						// 0x0033F090

protected:
	void cullFrontOfList();												// 0x0033F033
	Bool considerDying();												// 0x0033F06B

public:
	// Zero Hour's ProjectileStreamUpdate fits BFME 2's ctor (0x0033EE25: rep stosd 20 IDs,
	// then +0x78, +0x70, +0x74, +0x7C, the Coord3D) and xfer's transfers exactly.
	ObjectID m_projectileIDs[MAX_PROJECTILE_STREAM];	// +0x20
	Int m_nextFreeIndex;							// +0x70; xfer Int
	Int m_firstValidIndex;							// +0x74; xfer Int
	ObjectID m_owningObject;						// +0x78
	ObjectID m_targetObject;						// +0x7C
	Coord3D m_targetPosition;						// +0x80; xfer Coord3D
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x8C
typedef char ProjectileStreamUpdate_size[(sizeof(ProjectileStreamUpdate) == 0x8C) ? 1 : -1];
typedef char ProjectileStreamUpdate_off_m_nextFreeIndex[(offsetof(ProjectileStreamUpdate, m_nextFreeIndex) == 0x70) ? 1 : -1];
typedef char ProjectileStreamUpdate_off_m_firstValidIndex[(offsetof(ProjectileStreamUpdate, m_firstValidIndex) == 0x74) ? 1 : -1];
typedef char ProjectileStreamUpdate_off_m_owningObject[(offsetof(ProjectileStreamUpdate, m_owningObject) == 0x78) ? 1 : -1];
typedef char ProjectileStreamUpdate_off_m_targetObject[(offsetof(ProjectileStreamUpdate, m_targetObject) == 0x7C) ? 1 : -1];
typedef char ProjectileStreamUpdate_off_m_targetPosition[(offsetof(ProjectileStreamUpdate, m_targetPosition) == 0x80) ? 1 : -1];

#endif
