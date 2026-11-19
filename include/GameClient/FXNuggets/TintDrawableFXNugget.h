// TintDrawableFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x168.
// Target facts (retail game.dat):
// - parse 0x001E1798 news 0x168 bytes, runs the ctor 0x001E0986 (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD8E0), reads the block through buildFieldParse (0x001DFCD3: TintDrawableFXNuggetFieldParse,
//   RVA 0x007DD278, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_TINTDRAWABLEFXNUGGET_H
#define BFME2_TINTDRAWABLEFXNUGGET_H

#include "GameLogic/FXList.h"

class TintDrawableFXNugget : public FXNugget
{
public:
	TintDrawableFXNugget();										// 0x001E0986

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const {}								// 1, empty (fold 0x001F01E8)
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E09D8
	virtual void GetAssetList(AssetList &assets, Int context) const {}		// 3, empty (fold 0x0050B238)

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFCD3
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E1798

	RGBColor m_color;								// 0x148 Color (Rva001DFC2AParse)
	UnsignedInt m_preColorTime;						// 0x154 PreColorTime (parseUnsignedInt)
	UnsignedInt m_postColorTime;						// 0x158 PostColorTime (parseUnsignedInt)
	UnsignedInt m_sustainedColorTime;				// 0x15C SustainedColorTime (parseUnsignedInt)
	Real m_frequency;								// 0x160 Frequency (parseReal)
	Real m_amplitude;								// 0x164 Amplitude (parseReal)
};
typedef char TintDrawableFXNugget_size[(sizeof(TintDrawableFXNugget) == 0x168) ? 1 : -1];

extern const FieldParse TintDrawableFXNuggetFieldParse[];			// RVA 0x007DD278

#endif
