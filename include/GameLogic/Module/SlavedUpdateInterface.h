// SlavedUpdateInterface.h: the nine-slot interface SlavedUpdate places after UpdateModule (+0x20).
// Target facts (retail game.dat): SlavedUpdate's constructor stores the folded table of every
// nine-slot pure interface (0x00C6F248, slots 0-8 __purecall) at +0x20 before its own. In its own
// table slot 1 is onEnslave (0x004A1BFD, ledger `?onEnslave@SlavedUpdate@@UAEXPBVObject@@@Z`),
// slot 2 onSlaverDie and slot 3 onSlaverDamage (0x004A1C05 / 0x004A1C59, ledger-named, one pointer
// argument each). Slot 0 is `mov eax,[ecx+4]; ret`: the ObjectID at SlavedUpdate+0x24. Slot 4 is the
// folded `xor al,al; ret`. Slots 5 and 8 return bytes of the module data (+0x6D, +0x55), slot 6 sets
// the byte at SlavedUpdate+0x44 and slot 7 returns it.
// Donor fact: Zero Hour names the interface SlavedUpdateInterface, with getSlaverID, onEnslave,
// onSlaverDie, onSlaverDamage and isSelfTasking in this order; the first five slots fit that.
// PROVISIONAL: slots 5-8 have no name in the ledger or the donor (vslotNN). onSlaverDie and
// onSlaverDamage take a DamageInfo in the donor; the ledger spells the type with a placeholder.
#pragma once
#ifndef BFME2_SLAVED_UPDATE_INTERFACE_H
#define BFME2_SLAVED_UPDATE_INTERFACE_H

#include "Common/GameType.h"

class Object;
class DamageInfo;

class SlavedUpdateInterface
{
public:
	virtual ObjectID getSlaverID() const = 0;							// slot 0
	virtual void onEnslave(const Object *slaver) = 0;					// slot 1
	virtual void onSlaverDie(const DamageInfo *info) = 0;				// slot 2
	virtual void onSlaverDamage(const DamageInfo *info) = 0;			// slot 3
	virtual Bool isSelfTasking() const = 0;								// slot 4
	virtual Bool vslot05() const = 0;									// slot 5 (PROVISIONAL)
	virtual void vslot06() = 0;											// slot 6 (PROVISIONAL)
	virtual Bool vslot07() const = 0;									// slot 7 (PROVISIONAL)
	virtual Bool vslot08() const = 0;									// slot 8 (PROVISIONAL)
};

#endif
