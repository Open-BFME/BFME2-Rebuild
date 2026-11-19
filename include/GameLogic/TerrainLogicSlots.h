// TerrainLogicSlots.h: the terrain slots the flammable bodies reach, at their
// retail indices. Slot 19 is the underwater test and slot 25 the water depth at
// a point; the other slots are unnamed placeholders so the indices line up.
#pragma once
#ifndef BFME2_TERRAINLOGICSLOTS_H
#define BFME2_TERRAINLOGICSLOTS_H

#include "Common/GameType.h"

class TerrainLogicSlots
{
public:
	virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
	virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
	virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
	virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
	virtual void v16(); virtual void v17(); virtual void v18();
	virtual Bool isUnderwater(Real x, Real y, Real *waterZ = 0, Real *terrainZ = 0, Int unused = 0);	// slot 19
	virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
	virtual void v24();
	// Water depth at (x, y): waterZ - terrainZ when underwater, else 0.
	virtual Real Rva0027D815(Real x, Real y);	// slot 25
};

#endif
