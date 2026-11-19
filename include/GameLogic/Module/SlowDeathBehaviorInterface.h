// SlowDeathBehaviorInterface.h: the four-slot interface SlowDeathBehavior places after its
// DieModuleInterface vptr (+0x24).
// Target facts (retail game.dat): SlowDeathBehavior's constructor stores the folded table of every
// four-slot pure interface (0x00C4EF80, slots 0-3 __purecall) at +0x24 before its own (0x00C42020).
// In its own table slot 0 is beginSlowDeath (0x0045DE65), slot 1 getProbabilityModifier
// (0x0045D5DA) and slot 2 isDieApplicable (0x0045D49E), all ledger-named with these signatures;
// slot 3 is the folded `mov al,1; ret` (0x0050B5C6), so it takes no argument and returns a Bool.
// Donor fact: Zero Hour names the interface SlowDeathBehaviorInterface (its first three methods
// match). Slot 3 has no name in the ledger or the donor: vslot03 is PROVISIONAL.
#pragma once
#ifndef BFME2_SLOW_DEATH_BEHAVIOR_INTERFACE_H
#define BFME2_SLOW_DEATH_BEHAVIOR_INTERFACE_H

#include "Common/GameType.h"

class DamageInfo;

class SlowDeathBehaviorInterface
{
public:
	virtual void beginSlowDeath(const DamageInfo *damageInfo) = 0;					// slot 0
	virtual Int getProbabilityModifier(const DamageInfo *damageInfo) const = 0;		// slot 1
	virtual Bool isDieApplicable(const DamageInfo *damageInfo) const = 0;			// slot 2
	virtual Bool vslot03() const = 0;												// slot 3 (PROVISIONAL name)
};

#endif
