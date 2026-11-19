// FireWeaponUpdateEntry.h: one live FireWeaponNugget of a FireWeaponUpdate. SIZE 0x18.
// Target facts (old repo FireWeaponUpdate.cpp, Rva0048BDF8Entry): Rva0048BDF8Helper
// (0x0048BDF8) news one per nugget whose weapon template exists: the weapon +0x00,
// the next fire frame +0x04 (0xFFFFFFFF never), one-shot +0x08 and the offset
// +0x0C. The module's entry list (+0x24) holds them as ints.
#pragma once
#ifndef BFME2_FIREWEAPONUPDATEENTRY_H
#define BFME2_FIREWEAPONUPDATEENTRY_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"

class Weapon;

struct FireWeaponUpdateEntry
{
	FireWeaponUpdateEntry() : m_weapon(0), m_nextFireFrame(0xFFFFFFFF), m_oneShot(false) { m_offset.zero(); }
	~FireWeaponUpdateEntry();			// 0x0048B95F

	Weapon *m_weapon;					// 0x00
	UnsignedInt m_nextFireFrame;		// 0x04
	Bool m_oneShot;						// 0x08
	Coord3D m_offset;					// 0x0C
};
// SIZE 0x18

#endif
