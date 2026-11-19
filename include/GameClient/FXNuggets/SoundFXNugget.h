// SoundFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x14C.
// Target facts (retail game.dat):
// - parse 0x001E1329 news 0x14C bytes, runs the ctor 0x001E00A3 (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD768), reads the block through buildFieldParse (0x001DFAE0: SoundFXNuggetFieldParse,
//   RVA 0x007DCB18, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_SOUNDFXNUGGET_H
#define BFME2_SOUNDFXNUGGET_H

#include "GameLogic/FXList.h"

class SoundFXNugget : public FXNugget
{
public:
	SoundFXNugget();										// 0x001E00A3

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;								// 1, 0x001E00C3
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E012E
	virtual void GetAssetList(AssetList &assets, Int context) const {}		// 3, empty (fold 0x0050B238)

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFAE0
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E1329

	OpaqueRefElement4 m_name;						// 0x148 Name (parseAudioEventRTS)
};
typedef char SoundFXNugget_size[(sizeof(SoundFXNugget) == 0x14C) ? 1 : -1];

extern const FieldParse SoundFXNuggetFieldParse[];			// RVA 0x007DCB18

#endif
