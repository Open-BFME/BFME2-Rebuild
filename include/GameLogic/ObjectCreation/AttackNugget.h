// AttackNugget.h: the OCL nugget that orders the primary object to attack a position and
// stamps a delivery decal (INI token Attack). SIZE 0x44.
// Target facts (retail game.dat):
// - AttackNugget::parse 0x001F0D96 news 0x44 bytes and runs the ctor 0x001F05C1: table VA
//   0x00BE0FF4, RadiusDecalTemplate ctor at +0x04, +0x38 = 0.0, +0x3C = 1, +0x40 = 0. The
//   dtor 0x001F0622 tears down +0x04 and restores the base table.
// - Field table VA 0x00BE10C0: NumberOfShots +0x3C (parseInt), WeaponSlot +0x40
//   (parseLookupList, the weapon-slot names), DeliveryDecal +0x04
//   (RadiusDecalTemplate::parseRadiusDecalTemplate), DeliveryDecalRadius +0x38 (parseReal).
// - Table: slot 3 0x001F0AD4 is the coordinate create (`ret 0x10`, nothing in eax); the
//   other slots are the base's.
// Name: the map's ctor, dtor and parser names.
#pragma once
#ifndef BFME2_ATTACKNUGGET_H
#define BFME2_ATTACKNUGGET_H

#include "GameLogic/ObjectCreationList.h"
#include "Common/INI/FieldParse.h"
#include "Common/RadiusDecalTemplate.h"

class INI;

class AttackNugget : public ObjectCreationNugget
{
public:
	AttackNugget();																// 0x001F05C1
	// No user-declared dtor: retail's 0x001F0622 skips the own-table store an explicit one makes.

	virtual void create(const Object *primaryObj, const Coord3D *primary, const Coord3D *secondary,
		UnsignedInt lifetimeFrames) const;										// 3, 0x001F0AD4
	virtual void GetAssetList(AssetList &assets, void *context) const {}		// 4, empty (fold 0x0050B238; the base's slot is pure)

	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001F0D96

	RadiusDecalTemplate m_deliveryDecalTemplate;								// 0x04 DeliveryDecal
	Real m_deliveryDecalRadius;													// 0x38 DeliveryDecalRadius
	Int m_numberOfShots;														// 0x3C NumberOfShots
	Int m_weaponSlot;															// 0x40 WeaponSlot (WeaponSlotType)
};
typedef char AttackNugget_size[(sizeof(AttackNugget) == 0x44) ? 1 : -1];
typedef char AttackNugget_off_m_deliveryDecalRadius[(offsetof(AttackNugget, m_deliveryDecalRadius) == 0x38) ? 1 : -1];
typedef char AttackNugget_off_m_weaponSlot[(offsetof(AttackNugget, m_weaponSlot) == 0x40) ? 1 : -1];

#endif
