// FireWeaponNugget.h: the OCL nugget that fires a temporary weapon (INI token FireWeapon). SIZE 8.
// Target facts (retail game.dat):
// - FireWeaponNugget::parse 0x001F0D61 news 8 bytes, zeroes +0x04 and stores table VA
//   0x00BE09E8 (the ctor COMDAT 0x001F017B does the same), then reads parse's static field table (VA
//   0x00BE1060: Weapon, parseWeaponTemplate, +0x04) and adds the nugget to the list.
// - Table: slot 3 0x001F0188 fires m_weapon from the primary object at the secondary
//   position through WeaponStore::createAndFireTempWeapon when all three pointers and the
//   weapon are set; slot 4 0x001F01B9 forwards GetAssetList to the weapon template
//   (0x002CAC6E) when there is one. The other slots are the base's.
// Name: the map's parser and field-table names.
#pragma once
#ifndef BFME2_FIREWEAPONNUGGET_H
#define BFME2_FIREWEAPONNUGGET_H

#include "GameLogic/ObjectCreationList.h"
#include "Common/INI/FieldParse.h"

class INI;
class WeaponTemplate;

class FireWeaponNugget : public ObjectCreationNugget
{
public:
	FireWeaponNugget() : m_weapon(NULL) {}										// COMDAT 0x001F017B

	virtual void create(const Object *primaryObj, const Coord3D *primary, const Coord3D *secondary,
		UnsignedInt lifetimeFrames) const;										// 3, 0x001F0188
	virtual void GetAssetList(AssetList &assets, void *context) const;			// 4, 0x001F01B9

	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001F0D61

	const WeaponTemplate *m_weapon;												// 0x04 Weapon
};
typedef char FireWeaponNugget_size[(sizeof(FireWeaponNugget) == 0x8) ? 1 : -1];

#endif
