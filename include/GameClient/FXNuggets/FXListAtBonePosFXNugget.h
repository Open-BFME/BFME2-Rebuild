// FXListAtBonePosFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x150.
// Target facts (retail game.dat):
// - parse 0x001E1814 news 0x150 bytes, runs the ctor 0x001E0A57 (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD8F4), reads the block through buildFieldParse (0x001DFCF4: FXListAtBonePosFXNuggetFieldParse,
//   RVA 0x007DD2F8, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_FXLISTATBONEPOSFXNUGGET_H
#define BFME2_FXLISTATBONEPOSFXNUGGET_H

#include "GameLogic/FXList.h"

class FXListAtBonePosFXNugget : public FXNugget
{
public:
	FXListAtBonePosFXNugget();										// 0x001E0A57

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const {}								// 1, empty (fold 0x001F01E8)
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E2DB3
	virtual void GetAssetList(AssetList &assets, Int context) const;		// 3, 0x001E129B

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFCF4
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E1814

protected:
	// 0x001E2DD6: plays m_fx at the object's bones; doFXObj runs it with 0, then 1.
	void doFxAtBones(const Object *obj, Int pass) const;

public:
	const FXList *m_fx;								// 0x148 FX (parseFXList)
	AsciiString m_boneName;							// 0x14C BoneName (parseAsciiString)
};
typedef char FXListAtBonePosFXNugget_size[(sizeof(FXListAtBonePosFXNugget) == 0x150) ? 1 : -1];

extern const FieldParse FXListAtBonePosFXNuggetFieldParse[];			// RVA 0x007DD2F8

#endif
