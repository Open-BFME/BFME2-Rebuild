// CameraShakerVolumeFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x160.
// Target facts (retail game.dat):
// - parse 0x001E1595 news 0x160 bytes, runs the ctor 0x001E0714 (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD7CC), reads the block through buildFieldParse (0x001DFBA6: CameraShakerVolumeFXNuggetFieldParse,
//   RVA 0x007DD040, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_CAMERASHAKERVOLUMEFXNUGGET_H
#define BFME2_CAMERASHAKERVOLUMEFXNUGGET_H

#include "GameLogic/FXList.h"

class CameraShakerVolumeFXNugget : public FXNugget
{
public:
	CameraShakerVolumeFXNugget();										// 0x001E0714

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;								// 1, 0x001E0760
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E079E
	virtual void GetAssetList(AssetList &assets, Int context) const {}		// 3, empty (fold 0x0050B238)

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFBA6
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E1595

	Coord3D m_unk0x148;			// 0x148 not in the field table; the ctor zeroes it as three Reals
	Real m_radius;									// 0x154 Radius (parseReal)
	Real m_durationSeconds;							// 0x158 Duration_Seconds (parseReal)
	Real m_amplitudeDegrees;							// 0x15C Amplitude_Degrees (parseReal)
};
typedef char CameraShakerVolumeFXNugget_size[(sizeof(CameraShakerVolumeFXNugget) == 0x160) ? 1 : -1];

extern const FieldParse CameraShakerVolumeFXNuggetFieldParse[];			// RVA 0x007DD040

#endif
