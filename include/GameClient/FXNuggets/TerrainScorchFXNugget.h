// TerrainScorchFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x158.
// Target facts (retail game.dat):
// - parse 0x001E171C news 0x158 bytes, runs the ctor 0x001E08D9 (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD828), reads the block through buildFieldParse (0x001E0965: TerrainScorchFXNuggetFieldParse,
//   RVA 0x007DD8A0, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_TERRAINSCORCHFXNUGGET_H
#define BFME2_TERRAINSCORCHFXNUGGET_H

#include "GameLogic/FXList.h"

class TerrainScorchFXNugget : public FXNugget
{
public:
	TerrainScorchFXNugget();										// 0x001E08D9

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;								// 1, 0x001E0915
	virtual void GetAssetList(AssetList &assets, Int context) const {}		// 3, empty (fold 0x0050B238)

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001E0965
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E171C

	Int m_scorchType;										// 0x148 Type (parseScorchType)
	Real m_radius;									// 0x14C Radius (parseReal)
	ICoord2D m_randomRange;							// 0x150 RandomRange (parseICoord2D)
};
typedef char TerrainScorchFXNugget_size[(sizeof(TerrainScorchFXNugget) == 0x158) ? 1 : -1];

extern const FieldParse TerrainScorchFXNuggetFieldParse[];			// RVA 0x007DD8A0

#endif
