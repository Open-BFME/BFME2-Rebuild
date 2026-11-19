// BuffNuggetFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x190.
// Target facts (retail game.dat):
// - parse 0x001E323B news 0x190 bytes, runs the ctor 0x001E2FCE (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD9B4), reads the block through buildFieldParse (0x001DFB64: BuffNuggetFXNuggetFieldParse,
//   RVA 0x007DCEB8, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_BUFFNUGGETFXNUGGET_H
#define BFME2_BUFFNUGGETFXNUGGET_H

#include <vector>
#include "GameLogic/FXList.h"

class BuffNuggetFXNugget : public FXNugget
{
public:
	BuffNuggetFXNugget();										// 0x001E2FCE

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const {}								// 1, empty (fold 0x001F01E8)
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E311E
	virtual void GetAssetList(AssetList &assets, Int context) const;		// 3, 0x001E30F5

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFB64
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E323B

	Int m_buffType;									// 0x148 BuffType (parseIndexList)
	Bool m_isComplexBuff;							// 0x14C IsComplexBuff (parseBool)
	UnsignedInt m_buffLifeTime;						// 0x150 BuffLifeTime (parseDurationUnsignedInt)
	AsciiString m_buffThingTemplate;					// 0x154 BuffThingTemplate (parseAsciiString)
	AsciiString m_buffOrcTemplate;					// 0x158 BuffOrcTemplate (parseAsciiString)
	AsciiString m_buffInfantryTemplate;				// 0x15C BuffInfantryTemplate (parseAsciiString)
	AsciiString m_buffCavalryTemplate;				// 0x160 BuffCavalryTemplate (parseAsciiString)
	AsciiString m_buffTrollTemplate;					// 0x164 BuffTrollTemplate (parseAsciiString)
	AsciiString m_buffMumakilTemplate;				// 0x168 BuffMumakilTemplate (parseAsciiString)
	AsciiString m_buffShipTemplate;					// 0x16C BuffShipTemplate (parseAsciiString)
	AsciiString m_buffMonsterTemplate;				// 0x170 BuffMonsterTemplate (parseAsciiString)
	Real m_extrusion;								// 0x174 Extrusion (parseReal)
	RGBColor m_color;								// 0x178 Color (parseRGBColor)
	std::vector<AsciiString> m_unk0x184;			// 0x184 not in the field table; ctor 0x001E2FCE builds and clears it
};
typedef char BuffNuggetFXNugget_size[(sizeof(BuffNuggetFXNugget) == 0x190) ? 1 : -1];

extern const FieldParse BuffNuggetFXNuggetFieldParse[];			// RVA 0x007DCEB8

#endif
