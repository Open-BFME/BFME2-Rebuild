// LuaEventNugget.h: a weapon effect nugget. SIZE 0x134.
// Target facts (retail game.dat):
// - parseLuaEventNugget 0x002CCC90 (Weapon.cpp) allocates 0x134 bytes, runs the ctor 0x0050BD45, stores the
//   template at +0x104 and reads the nugget through 0x0050BBEB (base and class tables).
// - The ctor stores table VA 0x00C64F78 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x0050BD81, 1 0x0050BD7C, 2 0x0050BD7C, 5 0x0050BBCC, 6 0x0050BC25, 7 0x0047A699.
// - Members from the field table VA 0x00C64F18 (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_LUAEVENTNUGGET_H
#define BFME2_LUAEVENTNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"
#include "Common/AsciiString.h"

class INI;

class LuaEventNugget : public WeaponEffectNugget
{
public:
	LuaEventNugget();										// 0x0050BD45
	virtual ~LuaEventNugget();								// 0, deleting dtor 0x0050BD81
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x0050BD7C
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x0050BD7C
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050BBCC
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050BC25
	virtual Bool slot07() const;	// 7, 0x0047A699

	// 0x0050BBEB: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	AsciiString m_luaEvent;					// 0x128 LuaEvent
	Real m_radius;					// 0x12C Radius
	Bool m_sendToEnemies;					// 0x130 SendToEnemies
	Bool m_sendToAllies;					// 0x131 SendToAllies
	Bool m_sendToNeutral;					// 0x132 SendToNeutral
	UnsignedByte m_unk0x133[0x1];					// 0x133
};
// SIZE 0x134
typedef char LuaEventNugget_size[(sizeof(LuaEventNugget) == 0x134) ? 1 : -1];
typedef char LuaEventNugget_off_m_luaEvent[(offsetof(LuaEventNugget, m_luaEvent) == 0x128) ? 1 : -1];
typedef char LuaEventNugget_off_m_radius[(offsetof(LuaEventNugget, m_radius) == 0x12C) ? 1 : -1];
typedef char LuaEventNugget_off_m_sendToEnemies[(offsetof(LuaEventNugget, m_sendToEnemies) == 0x130) ? 1 : -1];
typedef char LuaEventNugget_off_m_sendToAllies[(offsetof(LuaEventNugget, m_sendToAllies) == 0x131) ? 1 : -1];
typedef char LuaEventNugget_off_m_sendToNeutral[(offsetof(LuaEventNugget, m_sendToNeutral) == 0x132) ? 1 : -1];

#endif
