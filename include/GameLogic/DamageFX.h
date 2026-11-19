// DamageFX.h: the FX a damage type plays on its victim (one [DamageFX] INI block).
// Target facts (retail game.dat, Common/DamageFX.cpp rows):
// - The parsers index m_dfx as [damage type * 0x40 + veterancy level * 0x10]: four levels of
//   0x10-byte entries per damage type. parseAmount 0x00360689 writes +0x00, parseMajorFXList
//   0x00360702 +0x04, parseMinorFXList 0x00360774 +0x08, parseTime 0x003607E6 +0x0C.
// - parseCommonStuff 0x00360613 reads the levels (all four when the field has no level list,
//   else one name from the list in userData) and the damage type ("Default" = 0..29, else one
//   name from TheDamageFXTypeNames in Damage.h, VA 0x00DBFFF0), so the table has 30 damage types.
// - 0x003605E3 returns no list for a zero amount, else the major list when the amount reaches
//   the level-0 threshold and the minor one below it; 0x003608DF plays it on the victim.
// - Field table getFieldParse's myFieldParse (VA 0x00C168F8): AmountForMajorFX, MajorFX, MinorFX,
//   ThrottleTime, then the four Veterancy* forms.
// Names: the map's (parsers, field table) and the donor's (Zero Hour) member roles.
#pragma once
#ifndef BFME2_DAMAGEFX_H
#define BFME2_DAMAGEFX_H

#include "Common/GameType.h"
#include "Common/INI/FieldParse.h"

class INI;
class FXList;
class Object;

class DamageFX
{
public:
	enum { DAMAGEFX_TYPE_COUNT = 30, DAMAGEFX_LEVEL_COUNT = 4 };

	// 0x003605E3
	const FXList *rva003605E3(Int damageType, Real damageAmount, const Object *source);
	// 0x003605D5: the throttle time in frames for the damage FX type (the map files it under a placeholder class)
	Int rva003605D5(Int damageType, Int source);
	// 0x003608DF
	Bool rva003608DF(Int damageType, Real damageAmount, const Object *source, const Object *victim);
	const FieldParse *getFieldParse() const;										// 0x00360911

	static void parseAmount(INI *ini, void *instance, void *store, const void *userData);		// 0x00360689
	static void parseMajorFXList(INI *ini, void *instance, void *store, const void *userData);	// 0x00360702
	static void parseMinorFXList(INI *ini, void *instance, void *store, const void *userData);	// 0x00360774
	static void parseTime(INI *ini, void *instance, void *store, const void *userData);		// 0x003607E6

	struct DFX
	{
		Real m_amountForMajorFX;						// 0x0
		const FXList *m_majorDamageFXList;				// 0x4
		const FXList *m_minorDamageFXList;				// 0x8
		UnsignedInt m_damageFXThrottleTime;				// 0xC
	};

	DFX m_dfx[DAMAGEFX_TYPE_COUNT][DAMAGEFX_LEVEL_COUNT];	// 0x000
};
typedef char DamageFX_DFX_size[(sizeof(DamageFX::DFX) == 0x10) ? 1 : -1];
typedef char DamageFX_size[(sizeof(DamageFX) == 0x780) ? 1 : -1];


#endif
