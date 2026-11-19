// ParticleSystemFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x1BC.
// Target facts (retail game.dat):
// - parse 0x001E264F news 0x1BC bytes, runs the ctor 0x001E0AEF (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD908), reads the block through buildFieldParse (0x001DFD15: ParticleSystemFXNuggetFieldParse,
//   RVA 0x007DD428, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_PARTICLESYSTEMFXNUGGET_H
#define BFME2_PARTICLESYSTEMFXNUGGET_H

#include "GameLogic/FXList.h"

class ParticleSystemFXNugget : public FXNugget
{
public:
	ParticleSystemFXNugget();										// 0x001E0AEF

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;								// 1, 0x001E19CE
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E24DC
	virtual void GetAssetList(AssetList &assets, Int context) const;		// 3, 0x001E0C77

	// 0x001E19E8 (2804 bytes): creates the particle system; doFXPos passes no objects.
	void reallyDoFX(const Coord3D *primary, const Matrix3D *primaryMtx, const Object *primaryObj,
		const Object *secondaryObj) const;

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFD15
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E264F

	AsciiString m_name;								// 0x148 Name (parseAsciiString)
	Int m_count;										// 0x14C Count (parseInt)
	Coord3D m_offset;								// 0x150 Offset (parseCoord3D)
	GameClientRandomVariable m_radius;				// 0x15C Radius (parseGameClientRandomVariable)
	GameClientRandomVariable m_height;				// 0x168 Height (parseGameClientRandomVariable)
	GameClientRandomVariable m_initialDelay;			// 0x174 InitialDelay (parseGameClientRandomVariable)
	Real m_rotateX;									// 0x180 RotateX (parseAngleReal)
	Real m_rotateY;									// 0x184 RotateY (parseAngleReal)
	Real m_rotateZ;									// 0x188 RotateZ (parseAngleReal)
	Bool m_orientToObject;							// 0x18C OrientToObject (parseBool)
	Bool m_attachToObject;							// 0x18D AttachToObject (parseBool)
	AsciiString m_attachToBone;						// 0x190 AttachToBone (parseAsciiString)
	Bool m_createAtGroundHeight;						// 0x194 CreateAtGroundHeight (parseBool)
	Bool m_ricochet;									// 0x195 Ricochet (parseBool)
	AsciiString m_createBoneOverride;				// 0x198 CreateBoneOverride (parseAsciiString)
	AsciiString m_targetBoneOverride;				// 0x19C TargetBoneOverride (parseAsciiString)
	Bool m_createBoneAtTarget;						// 0x1A0 CreateBoneAtTarget (parseBool)
	Real m_targetCoeff;								// 0x1A4 TargetCoeff (parseReal)
	Int m_systemLife;								// 0x1A8 SystemLife (parseInt)
	Bool m_useTargetOffset;							// 0x1AC UseTargetOffset (parseBool)
	Bool m_setTargetMatrix;							// 0x1AD SetTargetMatrix (parseBool)
	Bool m_onlyIfOnLand;								// 0x1AE OnlyIfOnLand (parseBool)
	Bool m_onlyIfOnWater;							// 0x1AF OnlyIfOnWater (parseBool)
	Coord3D m_targetOffset;							// 0x1B0 TargetOffset (parseCoord3D)
};
typedef char ParticleSystemFXNugget_size[(sizeof(ParticleSystemFXNugget) == 0x1BC) ? 1 : -1];

extern const FieldParse ParticleSystemFXNuggetFieldParse[];			// RVA 0x007DD428

#endif
