// RayEffectFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x164.
// Target facts (retail game.dat):
// - parse 0x001E13A5 news 0x164 bytes, runs the ctor 0x001E01B2 (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD77C), reads the block through buildFieldParse (0x001DFB01: RayEffectFXNuggetFieldParse,
//   RVA 0x007DCB58, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_RAYEFFECTFXNUGGET_H
#define BFME2_RAYEFFECTFXNUGGET_H

#include "GameLogic/FXList.h"

class RayEffectFXNugget : public FXNugget
{
public:
	RayEffectFXNugget();										// 0x001E01B2

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;								// 1, 0x001E022D
	virtual void GetAssetList(AssetList &assets, Int context) const;		// 3, 0x001E0301

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFB01
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E13A5

	AsciiString m_name;								// 0x148 Name (parseAsciiString)
	Coord3D m_primaryOffset;							// 0x14C PrimaryOffset (parseCoord3D)
	Coord3D m_secondaryOffset;						// 0x158 SecondaryOffset (parseCoord3D)
};
typedef char RayEffectFXNugget_size[(sizeof(RayEffectFXNugget) == 0x164) ? 1 : -1];

extern const FieldParse RayEffectFXNuggetFieldParse[];			// RVA 0x007DCB58

#endif
