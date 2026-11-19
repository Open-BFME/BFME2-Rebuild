// DRAFT (h05 fpgen): AutoResolveWeapon from retail's field-parse table (VA 0x00C3AAC8+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x35; bases and vptr, if any, are the leading gap.
// Unresolved members: m_levelBonus (+0x4, Rva004195ECParse), m_damagePerRound (+0x14, LivingWorldAutoResolveWeaponTemplate::parseDamagePerRoundIntoTable).
#pragma once
#ifndef BFME2_AUTO_RESOLVE_WEAPON_H
#define BFME2_AUTO_RESOLVE_WEAPON_H

#include <stddef.h>
#include "Common/GameType.h"


class AutoResolveWeapon
{
public:

	UnsignedByte m_unk0x0[0x4];
	UnsignedByte m_levelBonus[0xC];                        // +0x4 LevelBonus; unresolved: Rva004195ECParse
	Int m_missPercentChance;                               // +0x10 MissPercentChance
	UnsignedByte m_damagePerRound[0x20];                   // +0x14 DamagePerRound; unresolved: LivingWorldAutoResolveWeaponTemplate::parseDamagePerRoundIntoTable
	Bool m_reduceAttackWhenHurt;                           // +0x34 ReduceAttackWhenHurt
};
typedef char AutoResolveWeapon_off_m_levelBonus[(offsetof(AutoResolveWeapon, m_levelBonus) == 0x4) ? 1 : -1];
typedef char AutoResolveWeapon_off_m_missPercentChance[(offsetof(AutoResolveWeapon, m_missPercentChance) == 0x10) ? 1 : -1];
typedef char AutoResolveWeapon_off_m_damagePerRound[(offsetof(AutoResolveWeapon, m_damagePerRound) == 0x14) ? 1 : -1];
typedef char AutoResolveWeapon_off_m_reduceAttackWhenHurt[(offsetof(AutoResolveWeapon, m_reduceAttackWhenHurt) == 0x34) ? 1 : -1];

#endif
