// TeleportToCasterSpecialPower.h: the special ability that moves its stored
// objects (or everything in range) around its caster. Layout not recovered yet:
// only the methods the unit's own rows name are declared here.
// Target facts: ctor 0x004CD3A4, getModuleNameKey 0x004CD3FB, teleportList
// 0x004CD51B and the triggerAbilityEffect override 0x004CD68D; the pool-name
// string is "TeleportToCasterSpecialPower" (0x004CD3D5 returns its address).
#pragma once
#ifndef BFME2_TELEPORTTOCASTERSPECIALPOWER_H
#define BFME2_TELEPORTTOCASTERSPECIALPOWER_H

#include <vector>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Object;
class Coord3D;

class TeleportToCasterSpecialPower : public SpecialAbilityUpdate
{
public:
	TeleportToCasterSpecialPower(Thing *thing, const ModuleData *moduleData);
	virtual ~TeleportToCasterSpecialPower();

	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();
	void rva004CD4DC(Object *obj);
	void teleportList(const _STL::vector<ObjectID> *ids, const Coord3D *center);
protected:
	virtual const char *GetSnapshotName() const;	// slot 2: the class's own (`mov eax,"TeleportToCasterSpecialPower"; ret`) (h05 r9)
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};

#endif
