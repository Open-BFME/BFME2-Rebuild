// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// WeaponTemplateSet.h: one WeaponSet block of a ThingTemplate (its INI conditions
// and the weapon template per slot). SIZE 0x368.
// Target facts (retail game.dat):
// - Field table VA 0x00C00A90, parsed by parseWeaponTemplateSet 0x002C8C7B
//   (`ini->initFromINI(this, table)`, then stores its ThingTemplate argument at +0):
//   Conditions +0x04 (BitFlags<104>, wrapper 0x002C8C06), ShareWeaponReloadTime
//   +0x35C, WeaponLockSharedAcrossSets +0x35D (parseBool), DefaultWeaponChoiceCritera
//   +0x360 (parseIndexList, names VA 0x00DBC29C), ReadyStatusSharedWithinSet +0x364
//   (parseBool). Weapon/AutoChooseSources/PreferredAgainst/OnlyAgainst/OnlyInCondition
//   are offset-0 entries whose parsers index the per-slot arrays with
//   scanIndexList(TheWeaponSlotTypeNames): Weapon +0x14 + 4*slot (parseWeaponTemplate,
//   0x002C726C), AutoChooseSources +0x2C + 4*slot (parseBitString32, 0x002C729F),
//   PreferredAgainst +0x44 + 0x1C*slot and OnlyAgainst +0xEC + 0x1C*slot (the kind-of
//   mask parse 0x00256499; 0x002C8C16/0x002C8C47), OnlyInCondition +0x194 + 0x4C*slot
//   (the model-condition parse 0x000B937E; 0x002C883E).
// - The inline ctor (COMDAT 0x0033A888, ThingTemplate.cpp) constructs +0x04 through
//   bitset<128>::reset, the three per-slot mask arrays through `??_H` (6 x 0x1C, 6 x
//   0x1C, 6 x 0x4C), then calls clear() 0x002C752F.
// - clear() zeroes +0x35C/+0x35D/+0x364 (bytes) and +0x360, the conditions, and per
//   slot the template, the masks, and sets the auto-choose mask to 0xFFFFFFFF.
// - Six slots: every per-slot loop runs to 6 (also WeaponSet's ctor 0x002C72D5).
// - Size: ThingTemplate's vector copy 0x0033A82D strides 0x368 (rep movsd 0xDA).
// Member names follow Zero Hour's WeaponTemplateSet where the field is the same;
// the rest are the INI tokens.
#pragma once
#ifndef BFME2_WEAPONTEMPLATESET_H
#define BFME2_WEAPONTEMPLATESET_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/BitFlags.h"
#include "Common/ModelConditionFlags.h"

// The INI name tables WeaponSet's parsers read, in retail order (WeaponSet's object: VA 0x00DBC284,
// 0x00DBC29C, 0x00DBC2B4). They are statics under Zero Hour's guards: retail has a copy of the
// command-source names in five objects (0x009A6100, 0x009B3C08, 0x009BC2B4, 0x009BFB44, 0x009D45A4),
// so they cannot be one external table.
#ifdef DEFINE_WEAPONSLOTTYPE_NAMES
static const char *TheWeaponSlotTypeNames[] =
{
	"PRIMARY", "SECONDARY", "TERTIARY", "QUATERNARY", "QUINARY",
	NULL
};
#endif

// DefaultWeaponChoiceCritera's names: six names and no terminator in retail.
#ifdef DEFINE_WEAPONCHOICECRITERIA_NAMES
static const char *TheWeaponChoiceCriteriaNames[] =
{
	"PREFER_MOST_DAMAGE", "PREFER_LONGEST_RANGE", "PREFER_GRAB_OVER_DAMAGE",
	"PREFER_LEAST_MOVEMENT", "SELECT_AT_RANDOM", "USE_WEAPONSET_DEFAULT_CRITERIA"
};
#endif

// AutoChooseSources' bit names.
#ifdef DEFINE_COMMANDSOURCEMASK_NAMES
static const char *TheCommandSourceMaskNames[] =
{
	"FROM_PLAYER", "FROM_SCRIPT", "FROM_AI",
	NULL
};
#endif

class INI;
class ThingTemplate;
class WeaponTemplate;
enum WeaponSlotType;

class WeaponTemplateSet
{
public:
	enum { WEAPONSLOT_COUNT = 6 };

	// clear() 0x002C752F memsets the conditions and each slot's masks in place, while
	// other units reach the same masks' clear through an out-of-line COMDAT; until
	// BitFlags settles which, clear() is declared only.
	void clear();												// 0x002C752F

	Bool isSharedReloadTime() const;							// 0x002C74AB
	Bool isWeaponLockSharedAcrossSets() const;					// 0x002C74B2
	Bool isReadyStatusSharedWithinSet() const;					// 0x002C74B9
	const WeaponTemplate *getNth(WeaponSlotType n) const;		// 0x002C74C0
	KindOfMaskType *getNthPreferredAgainstMask(WeaponSlotType n);	// 0x002C74CB
	KindOfMaskType *getNthOnlyAgainstMask(WeaponSlotType n);		// 0x002C74D9
	ModelConditionFlags *getNthOnlyInCondition(WeaponSlotType n);	// 0x002C74EA

	void parseWeaponTemplateSet(INI *ini, const ThingTemplate *tmpl);	// 0x002C8C7B

private:
	static void parseWeapon(INI *ini, void *instance, void *store, const void *userData);			// 0x002C726C
	static void parseAutoChoose(INI *ini, void *instance, void *store, const void *userData);		// 0x002C729F
	static void parsePreferredAgainst(INI *ini, void *instance, void *store, const void *userData);	// 0x002C8C16

public:
	// The OnlyAgainst (0x002C8C47) and OnlyInCondition (0x002C883E) parsers are
	// public statics in the mangled placeholders; these names follow the tokens.
	static void parseOnlyAgainst(INI *ini, void *instance, void *store, const void *userData);
	static void parseOnlyInCondition(INI *ini, void *instance, void *store, const void *userData);

	const ThingTemplate *m_thingTemplate;						// 0x000 parseWeaponTemplateSet's argument
	WeaponSetFlags m_types;										// 0x004 Conditions
	const WeaponTemplate *m_template[WEAPONSLOT_COUNT];			// 0x014 Weapon
	UnsignedInt m_autoChooseMask[WEAPONSLOT_COUNT];				// 0x02C AutoChooseSources
	KindOfMaskType m_preferredAgainst[WEAPONSLOT_COUNT];		// 0x044 PreferredAgainst
	KindOfMaskType m_onlyAgainst[WEAPONSLOT_COUNT];				// 0x0EC OnlyAgainst
	ModelConditionFlags m_onlyInCondition[WEAPONSLOT_COUNT];	// 0x194 OnlyInCondition
	Bool m_isReloadTimeShared;									// 0x35C ShareWeaponReloadTime
	Bool m_isWeaponLockSharedAcrossSets;						// 0x35D WeaponLockSharedAcrossSets
	Int m_defaultWeaponChoiceCriteria;							// 0x360 DefaultWeaponChoiceCritera
	Bool m_isReadyStatusSharedWithinSet;						// 0x364 ReadyStatusSharedWithinSet
};
// SIZE 0x368
typedef char WeaponTemplateSet_size[(sizeof(WeaponTemplateSet) == 0x368) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_thingTemplate[(offsetof(WeaponTemplateSet, m_thingTemplate) == 0x000) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_types[(offsetof(WeaponTemplateSet, m_types) == 0x004) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_template[(offsetof(WeaponTemplateSet, m_template) == 0x014) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_autoChooseMask[(offsetof(WeaponTemplateSet, m_autoChooseMask) == 0x02C) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_preferredAgainst[(offsetof(WeaponTemplateSet, m_preferredAgainst) == 0x044) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_onlyAgainst[(offsetof(WeaponTemplateSet, m_onlyAgainst) == 0x0EC) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_onlyInCondition[(offsetof(WeaponTemplateSet, m_onlyInCondition) == 0x194) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_isReloadTimeShared[(offsetof(WeaponTemplateSet, m_isReloadTimeShared) == 0x35C) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_isWeaponLockSharedAcrossSets[(offsetof(WeaponTemplateSet, m_isWeaponLockSharedAcrossSets) == 0x35D) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_defaultWeaponChoiceCriteria[(offsetof(WeaponTemplateSet, m_defaultWeaponChoiceCriteria) == 0x360) ? 1 : -1];
typedef char WeaponTemplateSet_off_m_isReadyStatusSharedWithinSet[(offsetof(WeaponTemplateSet, m_isReadyStatusSharedWithinSet) == 0x364) ? 1 : -1];

#endif
