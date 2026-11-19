// AreaSet.h: sets of water areas and the observers they notify (GameLogic/Map/AreaSet.h,
// the path WorldBuilder's asserts carry).
// Target facts:
//  - WorldBuilder strings name the instantiations AreaSet<RiverArea>,
//    AreaSet<StandingWaterArea>, AreaSet<StandingWaveArea>, AreaSet<WaterArea> and
//    Observable<AreaSetObserver<RiverArea / StandingWaterArea / StandingWaveArea /
//    WaterArea> >::addObserver / dispatchNotice.
//  - AreaSetObserver's own table (game 0x00BC6F34, shared by every instantiation
//    because the bodies fold) has four slots: slot 0 `ret 4` (one argument),
//    slots 1..3 `ret 8` (two arguments). WorldBuilder names four observer
//    methods on StandingWaterAreaOptionsPanel::Impl and TerrainLogic:
//    onAreaAddedToSet and onRemovingAreaFromSet (TerrainLogic overrides slots 1
//    and 3, and WorldBuilder's asserts name exactly those two), plus
//    onDestroyingAreaSet and onAreaRemovedFromSet. The one-argument
//    onDestroyingAreaSet is slot 0 and onAreaRemovedFromSet slot 2.
//  - Argument types: slot 1/3 take an id and the area (TerrainLogic's overrides
//    look the area up in the observed set and use the id); the set argument of
//    slot 0 is inferred.
// AreaSet itself is not recovered yet (only declared).
#pragma once
#ifndef BFME2_AREASET_H
#define BFME2_AREASET_H

#include "Common/GameType.h"

template <class AREA> class AreaSet;

template <class AREA>
class AreaSetObserver
{
public:
	virtual void onDestroyingAreaSet(AreaSet<AREA> *set) {}		// slot 0
	virtual void onAreaAddedToSet(Int id, AREA *area) {}		// slot 1
	virtual void onAreaRemovedFromSet(Int id, AREA *area) {}	// slot 2
	virtual void onRemovingAreaFromSet(Int id, AREA *area) {}	// slot 3
};

#endif
