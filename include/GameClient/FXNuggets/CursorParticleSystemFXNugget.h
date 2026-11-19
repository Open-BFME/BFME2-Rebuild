// CursorParticleSystemFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x180.
// Target facts (retail game.dat):
// - parse 0x001E26CB news 0x180 bytes, runs the ctor 0x001E0C90 (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD91C), reads the block through buildFieldParse (0x001DFD36: CursorParticleSystemFXNuggetFieldParse,
//   RVA 0x007DD600, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_CURSORPARTICLESYSTEMFXNUGGET_H
#define BFME2_CURSORPARTICLESYSTEMFXNUGGET_H

#include "GameLogic/FXList.h"

class CursorParticleSystemFXNugget : public FXNugget
{
public:
	CursorParticleSystemFXNugget();										// 0x001E0C90

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;								// 1, 0x001E0D2D
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E0D73
	virtual void GetAssetList(AssetList &assets, Int context) const {}		// 3, empty (fold 0x0050B238)

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFD36
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E26CB

	AsciiString m_anim2DTemplateName;				// 0x148 Anim2DTemplateName (parseAsciiString)
	UnsignedInt m_burstCount;						// 0x14C BurstCount (parseUnsignedInt)
	GameClientRandomVariable m_particleLife;			// 0x150 ParticleLife (parseGameClientRandomVariable)
	GameClientRandomVariable m_systemLife;			// 0x15C SystemLife (parseGameClientRandomVariable)
	GameClientRandomVariable m_driftVelX;			// 0x168 DriftVelX (parseGameClientRandomVariable)
	GameClientRandomVariable m_driftVelY;			// 0x174 DriftVelY (parseGameClientRandomVariable)
};
typedef char CursorParticleSystemFXNugget_size[(sizeof(CursorParticleSystemFXNugget) == 0x180) ? 1 : -1];

extern const FieldParse CursorParticleSystemFXNuggetFieldParse[];			// RVA 0x007DD600

#endif
