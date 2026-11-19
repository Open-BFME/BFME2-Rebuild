// WeaponStore.h: the subsystem that owns every WeaponTemplate. SIZE 0x24.
// Target facts (retail game.dat):
// - ctor 0x002CC20E: SubsystemInterface's ctor, table VA 0x00C02114 (14 slots), STLport
//   vectors at +0x0C and +0x18. newWeaponTemplate 0x002CD5FC push_backs onto +0x0C and
//   findWeaponTemplatePrivate 0x002CADBE walks it comparing each template's name key (+0x0C).
// - Slots over SubsystemInterface's: 0 dtor 0x002CD5E0, 1 init (empty fold), 3
//   postProcessLoad 0x002CADFA, 4 attemptReload 0x002CAE34, 5 attemptRefresh 0x002CAE6C,
//   7 postReload 0x002C941C, 9 reset 0x002C9A34, 10 update (empty fold).
// - TheWeaponStore is VA 0x00DFEFDC (proven in symbols.json).
#pragma once
#ifndef BFME2_WEAPONSTORE_H
#define BFME2_WEAPONSTORE_H

#include <stddef.h>
#include <vector>
#include "Common/SubsystemInterface.h"
#include "GameLogic/Weapon.h"

class AsciiString;
class Weapon;
class WeaponTemplate;
class Object;
class INI;
class Coord3D;
enum NameKeyType;

class WeaponStore : public SubsystemInterface
{
	// WeaponSet::updateWeaponSet 0x002C8C97 looks templates up through the protected
	// findWeaponTemplatePrivate.
	friend class WeaponSet;

public:
	WeaponStore();																// 0x002CC20E
	virtual ~WeaponStore();														// 0x002CD5E0 (deleting)

	virtual void init() {}
	virtual void postProcessLoad();												// 0x002CADFA
	virtual Bool attemptReload(Bool *needsRestart);								// 0x002CAE34
	virtual Bool attemptRefresh();												// 0x002CAE6C
	virtual void postReload(Bool needsRestart);									// 0x002C941C
	virtual void reset();														// 0x002C9A34
	virtual void update() {}

	const WeaponTemplate *findWeaponTemplate(const AsciiString &name) const;	// 0x002CB8BF
	// FireWeaponUpdate 0x0048B9D0 allocates the module's weapon through this.
	Weapon *allocateNewWeapon(const WeaponTemplate *tmpl, WeaponSlotType wslot) const;	// 0x0028AA81
	void createAndFireTempWeapon(const WeaponTemplate *tmpl, const Object *source, const Coord3D *pos);	// 0x002CE904
	void rva002CE964(const WeaponTemplate *, const Object *, const Object *);
	// 0x002CDB0E: moves the template from the live vector to the retired one, marking its +0x174.
	void rva002CDB0E(WeaponTemplate *tmpl);
	void handleProjectileDetonation(const WeaponTemplate *tmpl, const Coord3D *pos, const Object *source,
		const Coord3D *dir, Int arg);											// 0x002CE8AA
	WeaponTemplate *newOverride(WeaponTemplate *tmpl);							// 0x002CE063
	static void parseWeaponTemplateDefinition(INI *ini);						// 0x002CE102

protected:
	WeaponTemplate *findWeaponTemplatePrivate(NameKeyType key) const;			// 0x002CADBE
	WeaponTemplate *newWeaponTemplate(const AsciiString &name);					// 0x002CD5FC
	void resetWeaponTemplates();												// 0x002C9A18

public:
	// 0x0C: reset 0x002C9A34, postProcessLoad 0x002CADFA and findWeaponTemplatePrivate 0x002CADBE
	// walk it by size() ((end - begin) >> 2) and index. Includers need // stlport46.
	std::vector<WeaponTemplate *> m_weaponTemplateVector;
	// 0x18: templates 0x002CDB0E took out of the live vector (it push_backs them here).
	std::vector<WeaponTemplate *> m_retiredTemplates;
};
// SIZE 0x24
typedef char WeaponStore_size[(sizeof(WeaponStore) == 0x24) ? 1 : -1];
typedef char WeaponStore_off_m_weaponTemplateVector[(offsetof(WeaponStore, m_weaponTemplateVector) == 0x0C) ? 1 : -1];

extern WeaponStore *TheWeaponStore;

#endif
