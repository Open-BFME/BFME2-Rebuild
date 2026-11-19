// DynamicDecalFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x18C.
// Target facts (retail game.dat):
// - parse 0x001E149D news 0x18C bytes, runs the ctor 0x001E0429 (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD7A4), reads the block through buildFieldParse (0x001DFB43: DynamicDecalFXNuggetFieldParse,
//   RVA 0x007DCCF0, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_DYNAMICDECALFXNUGGET_H
#define BFME2_DYNAMICDECALFXNUGGET_H

#include "GameLogic/FXList.h"

class DynamicDecalFXNugget : public FXNugget
{
public:
	DynamicDecalFXNugget();										// 0x001E0429

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;								// 1, 0x001E0E43
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E04BC
	virtual void GetAssetList(AssetList &assets, Int context) const;		// 3, 0x001E1709

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFB43
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E149D

	AsciiString m_decalName;							// 0x148 DecalName (parseAsciiString)
	Int m_shader;									// 0x14C Shader (parseIndexList)
	Real m_size;										// 0x150 Size (parseReal)
	RGBColor m_color;								// 0x154 Color (parseRGBColor)
	Coord2D m_offset;								// 0x160 Offset (parseCoord2D)
	Bool m_orientToObject;							// 0x168 OrientToObject (parseBool)
	UnsignedInt m_opacityStart;						// 0x16C OpacityStart (parseUnsignedInt)
	Real m_opacityFadeTimeOne;						// 0x170 OpacityFadeTimeOne (parseReal)
	UnsignedInt m_opacityPeak;						// 0x174 OpacityPeak (parseUnsignedInt)
	Real m_opacityPeakTime;							// 0x178 OpacityPeakTime (parseReal)
	Real m_opacityFadeTimeTwo;						// 0x17C OpacityFadeTimeTwo (parseReal)
	UnsignedInt m_opacityEnd;						// 0x180 OpacityEnd (parseUnsignedInt)
	Real m_startingDelay;							// 0x184 StartingDelay (parseReal)
	Real m_lifetime;									// 0x188 Lifetime (parseReal)
};
typedef char DynamicDecalFXNugget_size[(sizeof(DynamicDecalFXNugget) == 0x18C) ? 1 : -1];

extern const FieldParse DynamicDecalFXNuggetFieldParse[];			// RVA 0x007DCCF0

#endif
