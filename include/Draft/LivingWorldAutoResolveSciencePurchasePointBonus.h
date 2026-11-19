// LivingWorldAutoResolveSciencePurchasePointBonus.h: one science-point bonus record.
//
// Target facts: parseBonusIniSubBlock 0x004135CD runs initFromINI over the
// table at 0x00C39A78 — MinSciencePurchasePointsForBonus (parseInt, +0x00),
// WeaponMultiplier (parseReal, +0x04), ArmorMultiplier (parseReal, +0x08) —
// then throws INIException(3, ...) when the int at +0 is negative. The
// resource twin's constructor seeds -1, 1.0f, 1.0f.
#pragma once
#ifndef BFME2_LIVINGWORLDAUTORESOLVESCIENCEPURCHASEPOINTBONUS_H
#define BFME2_LIVINGWORLDAUTORESOLVESCIENCEPURCHASEPOINTBONUS_H

class INI;

class LivingWorldAutoResolveSciencePurchasePointBonus
{
public:
	LivingWorldAutoResolveSciencePurchasePointBonus() :
		minimum(-1), firstMultiplier(1.0f), secondMultiplier(1.0f) {}

	void parseBonusIniSubBlock(INI *ini);

	int minimum;			// +0x00
	float firstMultiplier;		// +0x04
	float secondMultiplier;		// +0x08
};

#endif
