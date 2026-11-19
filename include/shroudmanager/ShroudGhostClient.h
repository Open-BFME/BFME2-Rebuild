// ShroudGhostClient.h: a ghost (the fogged image of a structure) as the shroud
// manager sees it. SIZE 0xC on its own: vptr +0, vbptr +4, then the virtual
// ShroudClient. The class name and slots 0-1 are PROVISIONAL; slots 2-3 carry
// the map's names for W3DGhostObject's overrides.
// Target facts (retail game.dat; functions by RVA, tables by VA):
//  - Constructor 0x003058F9 (COMDAT, one caller: GhostObject's ctor) has the
//    same shape as ShroudObjectClient's: the vbtable 0x00C74ED4 and
//    ShroudClient's table at +8 when most-derived, then its own table
//    0x00C4EF80 (four __purecall).
//  - GhostObject (ctor 0x00305A8F) keeps that table at +4. W3DGhostObject
//    (ctor 0x00063912) stores 0x00BC5984: 0 0x00063C33, 1 0x0006341B,
//    2 snapShot 0x00064675 (ret 4), 3 freeSnapShot 0x00063A1D (ret 4).
//  - ShroudObjectClient's slot 2 returns a GhostObject converted to this base.
#pragma once
#ifndef BFME2_SHROUDGHOSTCLIENT_H
#define BFME2_SHROUDGHOSTCLIENT_H

#include "shroudmanager/ShroudClient.h"

class ShroudGhostClient : public virtual ShroudClient
{
public:
	virtual void vslot00() = 0;						// slot 0
	virtual void vslot01() = 0;						// slot 1
	virtual void snapShot(Int playerIndex) = 0;		// slot 2
	virtual void freeSnapShot(Int playerIndex) = 0;	// slot 3
};
// SIZE 0xC (0x8 inside a class that shares the virtual base)

#endif
