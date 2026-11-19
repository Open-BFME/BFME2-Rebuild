// LightPulseFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x164.
// Target facts (retail game.dat):
// - parse 0x001E1421 news 0x164 bytes, runs the ctor 0x001E0321 (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD790), reads the block through buildFieldParse (0x001DFB22: LightPulseFXNuggetFieldParse,
//   RVA 0x007DCBD8, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_LIGHTPULSEFXNUGGET_H
#define BFME2_LIGHTPULSEFXNUGGET_H

#include "GameLogic/FXList.h"

class LightPulseFXNugget : public FXNugget
{
public:
	LightPulseFXNugget();										// 0x001E0321

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;								// 1, 0x001E0373
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E03B4
	virtual void GetAssetList(AssetList &assets, Int context) const {}		// 3, empty (fold 0x0050B238)

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFB22
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E1421

	RGBColor m_color;								// 0x148 Color (parseRGBColor)
	Real m_radius;									// 0x154 Radius (parseReal)
	Real m_radiusAsPercentOfObjectSize;				// 0x158 RadiusAsPercentOfObjectSize (parsePercentToReal)
	UnsignedInt m_increaseTime;						// 0x15C IncreaseTime (parseDurationUnsignedInt)
	UnsignedInt m_decreaseTime;						// 0x160 DecreaseTime (parseDurationUnsignedInt)
};
typedef char LightPulseFXNugget_size[(sizeof(LightPulseFXNugget) == 0x164) ? 1 : -1];

extern const FieldParse LightPulseFXNuggetFieldParse[];			// RVA 0x007DCBD8

#endif
