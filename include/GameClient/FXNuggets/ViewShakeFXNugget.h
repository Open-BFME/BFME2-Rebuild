// ViewShakeFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x14C.
// Target facts (retail game.dat):
// - parse 0x001E1611 news 0x14C bytes, runs the ctor 0x001E07DF (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD7E0), reads the block through buildFieldParse (0x001E0829: ViewShakeFXNuggetFieldParse,
//   RVA 0x007DD7F4, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_VIEWSHAKEFXNUGGET_H
#define BFME2_VIEWSHAKEFXNUGGET_H

#include "GameLogic/FXList.h"

class ViewShakeFXNugget : public FXNugget
{
public:
	ViewShakeFXNugget();										// 0x001E07DF

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;								// 1, 0x001E0802
	virtual void GetAssetList(AssetList &assets, Int context) const {}		// 3, empty (fold 0x0050B238)

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001E0829
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E1611

	Int m_shakeType;										// 0x148 Type (parseShakeType)
};
typedef char ViewShakeFXNugget_size[(sizeof(ViewShakeFXNugget) == 0x14C) ? 1 : -1];

extern const FieldParse ViewShakeFXNuggetFieldParse[];			// RVA 0x007DD7F4

#endif
