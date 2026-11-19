// Armor.h: one [Armor] INI definition, its damage-type coefficients, and the store that owns them.
// ArmorTemplate is laid out from its parse functions and clear(): 27 coefficients from +0x04 (one per
// DamageType, DAMAGE_NUM_TYPES), the damage scalar at +0x70, a positive-flag int at +0x74 (-1 after
// clear, read as "> 0" by the Armor-level test), and the name string at +0x78. SIZE 0x7C.
#pragma once
#ifndef BFME2_GAMELOGIC_ARMOR_H
#define BFME2_GAMELOGIC_ARMOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameLogic/Damage.h"
#include <vector>
#include "Common/SubsystemInterface.h"
#include "Common/NameKeyGenerator.h"
#include "Common/STLTypedefs.h"

class INI;
class Object;

class ArmorTemplate
{
public:
	// COMDAT 0x001D8F68: nulls +0x78, clear(), then sets the name.
	ArmorTemplate(const AsciiString &name) { clear(); m_name = name; }
	void clear();																	// 0x001D8F1F
	Bool rva001D8E7F() const;													// 0x001D8E7F, m_unk0x74 > 0
	Real getFlankedPenalty() const;													// 0x001D8E88
	Real getDamageCoefficient(const DamageInfoInput &damageInfo) const;				// 0x001D8E8B

	static void parseArmorCoefficients(INI *ini, void *instance, void *store, const void *userData);	// 0x001D8E99
	static void parseDamageScalar(INI *ini, void *instance, void *store, const void *userData);		// 0x001D8F02

	Real m_flankedPenalty;					// +0x00 FlankedPenalty
	Real m_damageCoefficient[DAMAGE_NUM_TYPES];	// +0x04 Armor, one per DamageType
	Real m_damageScalar;					// +0x70 DamageScalar, 1.0 after clear()
	Int m_unk0x74;							// +0x74 -1 after clear(); ArmorStore::parseArmorDefinition sets 1 on a template a reload replaces and 0 on its replacement; 0x001D8E7F tests > 0
	AsciiString m_name;						// +0x78
};

// SIZE 0x7C
typedef char ArmorTemplate_size[(sizeof(ArmorTemplate) == 0x7C) ? 1 : -1];
typedef char ArmorTemplate_off_m_damageCoefficient[(offsetof(ArmorTemplate, m_damageCoefficient) == 0x04) ? 1 : -1];
typedef char ArmorTemplate_off_m_damageScalar[(offsetof(ArmorTemplate, m_damageScalar) == 0x70) ? 1 : -1];
typedef char ArmorTemplate_off_m_unk0x74[(offsetof(ArmorTemplate, m_unk0x74) == 0x74) ? 1 : -1];
typedef char ArmorTemplate_off_m_name[(offsetof(ArmorTemplate, m_name) == 0x78) ? 1 : -1];

// ArmorTemplateSet: what ThingTemplate::findArmorTemplateSet (0x0033DCB8) returns for a set of armor flags.
// Target facts: ActiveBody::validateArmorAndDamageFX 0x004BE1AE reads the armor name at +0x4 (an AsciiString:
// empty means no armor) and the DamageFX pointer at +0x8; +0x0 is not read there (provisional name).
class DamageFX;
class ArmorTemplateSet
{
public:
	UnsignedInt m_unk0x0;			// +0x0 (not read by validateArmorAndDamageFX)
	AsciiString m_armorName;		// +0x4
	DamageFX *m_damageFX;			// +0x8
};

// Armor: what an object holds, the name of its ArmorTemplate. SIZE 4.
// Target facts: ArmorStore::makeArmor 0x001D9001 copy-constructs the name into the returned
// Armor; 0x001D9083 passes `this` straight to the store's template lookup 0x001D901B.
class Armor
{
public:
	Armor(const AsciiString &name) : m_templateName(name) {}
	// 0x001D9083: the template's +0x74 > 0, or 0 when the store has no such template.
	Int rva001D9083() const;
	Real adjustDamage(const DamageInfoInput *info, const Object *obj, Bool flag) const;	// 0x001D90A1

	void clear() { m_templateName = AsciiString::TheEmptyString; }	// the armor with no template (ActiveBody::validateArmorAndDamageFX 0x004BE1AE)
	AsciiString m_templateName;		// +0x00
};
typedef char Armor_size[(sizeof(Armor) == 0x4) ? 1 : -1];

// ArmorStore (the map's Rva001D9670Store). SIZE 0x38.
// Target facts: ctor COMDAT 0x001D9670: SubsystemInterface's ctor, table VA 0x00BD9CB0 (14 slots: 0
// dtor 0x001D92C0 via ??_G 0x001D93B2, 4 0x001D8FAD, 5 0x001D93CE, the rest SubsystemInterface's
// folds), a hash_map at +0x0C (its ctor 0x001D9651; _M_find 0x002888D4 with the node's value at
// +0x08, operator[] 0x0041F4E5), vectors at +0x20 and +0x2C, then clears all three.
// parseArmorDefinition 0x001D9484 pushes a reload's replacement template onto +0x2C.
// TheArmorStore is VA 0x00DFDBC4.
class ArmorStore : public SubsystemInterface
{
public:
	ArmorStore()
	{
		m_armorTemplates.clear();
		m_unk0x20.clear();
		m_reloadedTemplates.clear();
	}
	virtual ~ArmorStore();														// 0x001D92C0

	virtual void init() {}
	virtual Bool attemptReload(Bool *needsRestart);								// 0x001D8FAD
	virtual Bool attemptRefresh();												// 0x001D93CE
	virtual void reset() {}
	virtual void update() {}

	Armor makeArmor(const AsciiString &name) const;								// 0x001D9001
	const ArmorTemplate *rva001D901B(const AsciiString &name) const;			// 0x001D901B, the template by name
	static void parseArmorDefinition(INI *ini);									// 0x001D9484

	_STL::hash_map<NameKeyType, ArmorTemplate *, rts::hash<NameKeyType>, _STL::equal_to<NameKeyType> > m_armorTemplates;	// 0x0C
	std::vector<ArmorTemplate *> m_unk0x20;										// 0x20 cleared by the ctor
	std::vector<ArmorTemplate *> m_reloadedTemplates;							// 0x2C
};
typedef char ArmorStore_size[(sizeof(ArmorStore) == 0x38) ? 1 : -1];

extern ArmorStore *TheArmorStore;

#endif
