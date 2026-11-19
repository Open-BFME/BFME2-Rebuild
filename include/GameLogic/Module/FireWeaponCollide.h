// FireWeaponCollide.h: fires a weapon at whatever the object collides with. SIZE 0x1C.
// Target facts (retail game.dat):
// - ctor 0x004BB79A: CollideModule's ctor, the three tables (VA 0x00C5A0FC primary,
//   0x00C5A040 at +0x0C, 0x00C5A024 at +0x10), +0x14 = TheWeaponStore->allocateNewWeapon(
//   module data's CollideWeapon, PRIMARY_WEAPON) with the object's ID stored in the weapon
//   (+0x08), +0x18 = false.
// - onCollide 0x004BB822 calls primary slot 12 (+0x30), shouldFireWeapon 0x004BB8BA.
// - xfer 0x004BB85C: CollideModule::xfer, then outside a light CRC Version1, a Bool "has a
//   weapon", the weapon's snapshot when set, and the +0x18 Bool.
// Names: the map's (onCollide, shouldFireWeapon, xfer, ctor); members follow Zero Hour's
// FireWeaponCollide (m_collideWeapon, m_everFired).
#pragma once
#ifndef BFME2_FIREWEAPONCOLLIDE_H
#define BFME2_FIREWEAPONCOLLIDE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CollideModule.h"
#include "GameLogic/Module/FireWeaponCollideModuleData.h"

class Weapon;
class Xfer;

class FireWeaponCollide : public CollideModule
{
public:
	FireWeaponCollide(Thing *thing, const ModuleData *moduleData);					// 0x004BB79A
	virtual NameKeyType getModuleNameKey() const;									// 0x004BB755
	virtual void onCollide(Object *other, const Coord3D *loc, const Coord3D *normal);	// 0x004BB822

protected:
	virtual const char *GetSnapshotName() const;									// 0x004BB6FC
	virtual void xfer(Xfer *xfer);													// 0x004BB85C
	virtual Bool shouldFireWeapon();												// 0x004BB8BA, primary slot 12

	const FireWeaponCollideModuleData *getFireWeaponCollideModuleData() const
	{
		return (const FireWeaponCollideModuleData *)getModuleData();
	}

public:
	Weapon *m_collideWeapon;		// 0x14
	Bool m_everFired;				// 0x18
protected:
	virtual void loadPostProcess() { CollideModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to CollideModule's, ICF-folded across classes (h05)
};
// SIZE 0x1C
typedef char FireWeaponCollide_size[(sizeof(FireWeaponCollide) == 0x1C) ? 1 : -1];
typedef char FireWeaponCollide_off_m_collideWeapon[(offsetof(FireWeaponCollide, m_collideWeapon) == 0x14) ? 1 : -1];

#endif
