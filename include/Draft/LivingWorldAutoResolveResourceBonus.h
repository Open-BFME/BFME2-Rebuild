// LivingWorldAutoResolveResourceBonus.h: one resource bonus record.
//
// Target facts: parseBonusIniSubBlock 0x00413DCC runs initFromINI over the
// table at 0x00C39E58 — MinResourceBonus (parseInt, +0x00), WeaponMultiplier
// (parseReal, +0x04), ArmorMultiplier (parseReal, +0x08) — then throws
// INIException(3, "Must provide a MinResourceBonus, and it must be >= 0")
// when the int at +0 is negative.
#pragma once
#ifndef BFME2_LIVINGWORLDAUTORESOLVERESOURCEBONUS_H
#define BFME2_LIVINGWORLDAUTORESOLVERESOURCEBONUS_H

class INI;

class LivingWorldAutoResolveResourceBonus
{
public:
	LivingWorldAutoResolveResourceBonus() :
		minimum(-1), firstMultiplier(1.0f), secondMultiplier(1.0f) {}

	void parseBonusIniSubBlock(INI *ini);

	int minimum;			// +0x00
	float firstMultiplier;		// +0x04
	float secondMultiplier;		// +0x08
};

#endif
