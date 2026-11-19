// FireLogicNugget.h: a weapon effect nugget. SIZE 0x1B4.
// Target facts (retail game.dat):
// - parseFireLogicNugget 0x002CCCF3 (Weapon.cpp) allocates 0x1B4 bytes, runs the ctor 0x0050BF8F, stores the
//   template at +0x104 and reads the nugget through 0x0050BDED (base and class tables).
// - The ctor stores table VA 0x00C650B8 (15 slots). Slots whose target differs from the
//   DamageNugget table: 0 0x0050B21C, 1 0x0050B5F1, 2 0x0050BD7C, 5 0x0050BDD5, 6 0x0050BE34.
// - Members from the field table VA 0x00C65068 (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_FIRELOGICNUGGET_H
#define BFME2_FIRELOGICNUGGET_H

#include "GameLogic/WeaponEffects/DamageNugget.h"

class INI;

class FireLogicNugget : public DamageNugget
{
public:
	FireLogicNugget();										// 0x0050BF8F
	virtual ~FireLogicNugget();								// 0, deleting dtor 0x0050B21C
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x0050B5F1
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x0050BD7C
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050BDD5
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050BE34

	// 0x0050BDED: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	Int m_logicType;					// 0x1A4 LogicType
	Int m_minMaxBurnRate;					// 0x1A8 MinMaxBurnRate
	Int m_minDecay;					// 0x1AC MinDecay
	Int m_maxResistance;					// 0x1B0 MaxResistance
};
// SIZE 0x1B4
typedef char FireLogicNugget_size[(sizeof(FireLogicNugget) == 0x1B4) ? 1 : -1];
typedef char FireLogicNugget_off_m_logicType[(offsetof(FireLogicNugget, m_logicType) == 0x1A4) ? 1 : -1];
typedef char FireLogicNugget_off_m_minMaxBurnRate[(offsetof(FireLogicNugget, m_minMaxBurnRate) == 0x1A8) ? 1 : -1];
typedef char FireLogicNugget_off_m_minDecay[(offsetof(FireLogicNugget, m_minDecay) == 0x1AC) ? 1 : -1];
typedef char FireLogicNugget_off_m_maxResistance[(offsetof(FireLogicNugget, m_maxResistance) == 0x1B0) ? 1 : -1];

#endif
