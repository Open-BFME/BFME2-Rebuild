// BattlePlanBonuses.h: the bonuses a battle plan hands to the objects it affects. SIZE 0x4C.
// Target facts (f10, 2026-11-17): BattlePlanUpdate's constructor 0x00497ED0 allocates 0x4C bytes
// (operator new 0x0002FDA0), runs the out-of-line default constructor 0x002AA14D (it runs the
// BitFlags<218> default constructor 0x0024C7B3 on +0x14 and +0x30 and returns this), then stores
// 1.0f at +0x00 and +0x10, zero at +0x04/+0x08/+0x0C and copies the module data's ValidMemberKindOf
// (data +0x54) into +0x14 and InvalidMemberKindOf (data +0x70) into +0x30 (seven dwords each).
// BattlePlanUpdate::xfer 0x00498517 transfers, in order: float +0x00, int +0x04, int +0x08, int +0x0C,
// float +0x10, then BitFlags<218>::xfer (0x002AC06A) on +0x14 and +0x30. Player +0xB8 points at one
// (Player::applyBattlePlanBonusesForObject 0x002AC917).
// Roles (from BattlePlanUpdate::rva00497AE8 0x00497AE8, which sets one counter per plan): +0x04 plan 1, +0x08 plan 3 with the
// sight-range scalar, +0x0C plan 2 with the armor scalar. The plan names follow the module data's INI tokens.
#pragma once
#ifndef BFME2_BATTLEPLANBONUSES_H
#define BFME2_BATTLEPLANBONUSES_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/BitFlags.h"

class BattlePlanBonuses
{
public:
	BattlePlanBonuses() throw();						// 0x002AA14D (nothrow: the constructor call in BattlePlanUpdate 0x00497ED0 has no unwind state)

	Real m_armorDamageFactor;									// +0x00 1.0f
	Int m_bombardmentActive;								// +0x04 set to 1 while plan 1 is applied (BattlePlanUpdate 0x00497AE8)
	Int m_searchActive;							// +0x08 set to 1 while plan 3 is applied; the sight scalar below goes with it
	Int m_holdLineActive;								// +0x0C set to 1 while plan 2 is applied; the armor scalar above goes with it
	Real m_sightRangeFactor;							// +0x10 1.0f
	KindOfMaskType m_validMemberKindOf;					// +0x14
	KindOfMaskType m_invalidMemberKindOf;				// +0x30
};
// SIZE 0x4C
typedef char BattlePlanBonuses_size[(sizeof(BattlePlanBonuses) == 0x4C) ? 1 : -1];
typedef char BattlePlanBonuses_off_m_validMemberKindOf[(offsetof(BattlePlanBonuses, m_validMemberKindOf) == 0x14) ? 1 : -1];
typedef char BattlePlanBonuses_off_m_invalidMemberKindOf[(offsetof(BattlePlanBonuses, m_invalidMemberKindOf) == 0x30) ? 1 : -1];

#endif
