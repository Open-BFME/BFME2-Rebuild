// ApplyRandomForceNugget.h: the OCL nugget that pushes the primary object with a random
// force (INI token ApplyRandomForce). SIZE 0x14.
// Target facts (retail game.dat):
// - ApplyRandomForceNugget::parse 0x001F0DE7 news 0x14 bytes, stores table VA 0x00BE0A00 and
//   zeroes four Reals at +0x04..+0x10 (the ctor COMDAT 0x001F01C8 does the same), then reads
//   its field table (parse's static, VA 0x00BE1110): MinForceMagnitude +0x04, MaxForceMagnitude +0x08
//   (parseReal), MinForcePitch +0x0C, MaxForcePitch +0x10 (parseAngleReal).
// - Table: slot 2 0x001F01F1 builds the force with calcRandomForce 0x001F0247 from the four
//   Reals and hands it to the primary object's physics (Object +0x25C, 0x003909FA); slot 3
//   is an empty override (fold 0x001F01E8, `ret 0x10`), since the base has no body there.
// Name: the map's parser and field-table names.
#pragma once
#ifndef BFME2_APPLYRANDOMFORCENUGGET_H
#define BFME2_APPLYRANDOMFORCENUGGET_H

#include "GameLogic/ObjectCreationList.h"
#include "Common/INI/FieldParse.h"

class INI;

class ApplyRandomForceNugget : public ObjectCreationNugget
{
public:
	ApplyRandomForceNugget() : m_minForceMagnitude(0.0f), m_maxForceMagnitude(0.0f),
		m_minForcePitch(0.0f), m_maxForcePitch(0.0f) {}							// COMDAT 0x001F01C8

	virtual void create(const Object *primaryObj, const Coord3D *primary, const Coord3D *secondary,
		UnsignedInt lifetimeFrames) const {}									// 3, fold 0x001F01E8
	virtual void create(const Object *primaryObj, const Object *secondaryObj,
		UnsignedInt lifetimeFrames) const;										// 2, 0x001F01F1
	virtual void GetAssetList(AssetList &assets, void *context) const {}		// 4, empty (fold 0x0050B238; the base's slot is pure)

	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001F0DE7

	Real m_minForceMagnitude;													// 0x04
	Real m_maxForceMagnitude;													// 0x08
	Real m_minForcePitch;														// 0x0C
	Real m_maxForcePitch;														// 0x10
};
typedef char ApplyRandomForceNugget_size[(sizeof(ApplyRandomForceNugget) == 0x14) ? 1 : -1];

#endif
