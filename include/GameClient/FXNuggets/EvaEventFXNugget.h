// EvaEventFXNugget.h: an FX nugget (FXList.cpp). SIZE 0x154.
// Target facts (retail game.dat):
// - parse 0x001E12AD news 0x154 bytes, runs the ctor 0x001DFFFA (FXNugget's ctor 0x001DFEAA, then
//   table VA 0x00BDD754), reads the block through buildFieldParse (0x001DFABF: EvaEventFXNuggetFieldParse,
//   RVA 0x007DCAD8, then FXNuggetFieldParse) and appends the nugget to the list's +0x04.
// - Members from 0x148 are the field table's entries (token and parse function beside each).
// Name: the map's (class, field-table and parser names).
#pragma once
#ifndef BFME2_EVAEVENTFXNUGGET_H
#define BFME2_EVAEVENTFXNUGGET_H

#include "GameLogic/FXList.h"

class EvaEventFXNugget : public FXNugget
{
public:
	EvaEventFXNugget();										// 0x001DFFFA

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const {}								// 1, empty (fold 0x001F01E8)
	virtual void doFXObj(const Object *primary, const Object *secondary) const;	// 2, 0x001E0028
	virtual void GetAssetList(AssetList &assets, Int context) const {}		// 3, empty (fold 0x0050B238)

	static void buildFieldParse(MultiIniFieldParse &p);					// 0x001DFABF
	static void parse(INI *ini, void *instance, void *store, const void *userData);	// 0x001E12AD

	Int m_evaEventOwner;								// 0x148 EvaEventOwner (Rva001DEE74Parse)
	Int m_evaEventAlly;								// 0x14C EvaEventAlly (Rva001DEE74Parse)
	Int m_evaEventEnemy;								// 0x150 EvaEventEnemy (Rva001DEE74Parse)
};
typedef char EvaEventFXNugget_size[(sizeof(EvaEventFXNugget) == 0x154) ? 1 : -1];

extern const FieldParse EvaEventFXNuggetFieldParse[];			// RVA 0x007DCAD8

#endif
