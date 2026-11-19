// ShroudObjectClient.h: a live object as the shroud manager sees it (the
// manager's owning object, as opposed to a ghost). SIZE 0xC on its own: vptr
// +0, vbptr +4, then the virtual ShroudClient.
// The class name is PROVISIONAL. Slots 4 and 5 carry WorldBuilder's names: the
// strings Object::ShroudGetValueRangeAndMask and Object::ShroudHideIfFogged are
// in the WB bodies 0x00CE1020 and 0x00CE13C0, which are these two slots of WB's
// table 0x01E5990C. The other slot names are PROVISIONAL, from what Object's
// bodies do.
// Target facts (retail game.dat; functions by RVA, tables by VA):
//  - Constructor 0x0028AADF (COMDAT, one caller: Object's ctor) takes the
//    most-derived flag. When the flag is set it stores the vbtable 0x00C74ED4
//    {-4, 4} at +4 and ShroudClient's table at +8. It always stores its own
//    table 0x00C40818 (six __purecall, ICF-folded with other six-slot tables).
//  - Object's table at +0x64, 0x00BFC2D8:
//    0 0x00290EE0 returns the template's name (ThingTemplate +0x64) by value;
//    1 0x000B3FD0 is empty (a folded `ret`);
//    2 0x00290EFE asks TheGhostObjectManager for a ghost of a structure and
//      returns it as GhostObject's ShroudGhostClient base (+4), or null;
//    3 0x002941EA fills the player mask and the front (shroud clearing), side
//      and rear reveal ranges (ret 0x10);
//    4 0x0028E5E0 ShroudGetValueRangeAndMask; 5 0x0028E775 ShroudHideIfFogged.
//  - ~Object (0x00299E36) calls TheShroudManager->UnRegisterObject(this + 0x64).
#pragma once
#ifndef BFME2_SHROUDOBJECTCLIENT_H
#define BFME2_SHROUDOBJECTCLIENT_H

#include "shroudmanager/ShroudClient.h"

class AsciiString;
class ShroudGhostClient;

class ShroudObjectClient : public virtual ShroudClient
{
public:
	virtual AsciiString ShroudGetName() = 0;									// slot 0
	virtual void vslot01() = 0;													// slot 1
	virtual ShroudGhostClient *ShroudCreateGhost() = 0;							// slot 2
	virtual void ShroudGetLookerRanges(Int *playerMask, Real *frontRange,
		Real *sideRange, Real *rearRange) = 0;									// slot 3
	virtual Real ShroudGetValueRangeAndMask(Int valueIndex, UnsignedInt *range,
		UnsignedInt *mask) const = 0;											// slot 4
	virtual Bool ShroudHideIfFogged(Int playerIndex) const = 0;					// slot 5
};
// SIZE 0xC (0x8 inside a class that shares the virtual base)

#endif
