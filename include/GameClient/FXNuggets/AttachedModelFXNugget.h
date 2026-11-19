// AttachedModelFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x154.
// Target facts (retail game.dat):
// - parse 0x001E168D news 0x154 bytes, runs the ctor 0x001E084A (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD814), reads the block through buildFieldParse (0x001DFBE8: AttachedModelFXNuggetFieldParse,
//   RVA 0x007DD118, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_ATTACHEDMODELFXNUGGET_H
#define BFME2_ATTACHEDMODELFXNUGGET_H

#include "GameLogic/FXList.h"

class AttachedModelFXNugget : public FXNugget
{
public:
	AttachedModelFXNugget();										// 0x001E084A

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const {}								// 1, empty (fold 0x001F01E8)
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E0878
	virtual void GetAssetList(AssetList &assets, Int context) const;		// 3, 0x001E1709

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFBE8
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E168D

	AsciiString m_modelname;							// 0x148 Modelname (parseAsciiString)
	Bool m_randomlyRotate;							// 0x14C RandomlyRotate (parseBool)
	Int m_expireTimer;								// 0x150 ExpireTimer (parseInt)
};
typedef char AttachedModelFXNugget_size[(sizeof(AttachedModelFXNugget) == 0x154) ? 1 : -1];

extern const FieldParse AttachedModelFXNuggetFieldParse[];			// RVA 0x007DD118

#endif
