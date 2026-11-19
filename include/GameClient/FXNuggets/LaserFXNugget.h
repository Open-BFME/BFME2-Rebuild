// LaserFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x15C.
// Target facts (retail game.dat):
// - parse 0x001E1519 news 0x15C bytes, runs the ctor 0x001E04DE (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD7B8), reads the block through buildFieldParse (0x001DFB85: LaserFXNuggetFieldParse,
//   RVA 0x007DCFD8, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_LASERFXNUGGET_H
#define BFME2_LASERFXNUGGET_H

#include "GameLogic/FXList.h"

class LaserFXNugget : public FXNugget
{
public:
	LaserFXNugget();										// 0x001E04DE

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;								// 1, 0x001E04FE
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E05F9
	virtual void GetAssetList(AssetList &assets, Int context) const {}		// 3, empty (fold 0x0050B238)

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFB85
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E1519

	AsciiString m_laserName;							// 0x148 LaserName (parseAsciiString)
	Bool m_laserBackwards;							// 0x14C LaserBackwards (parseBool)
	Coord3D m_targetPositionOffsetFallback;			// 0x150 TargetPositionOffsetFallback (parseCoord3D)
};
typedef char LaserFXNugget_size[(sizeof(LaserFXNugget) == 0x15C) ? 1 : -1];

extern const FieldParse LaserFXNuggetFieldParse[];			// RVA 0x007DCFD8

#endif
