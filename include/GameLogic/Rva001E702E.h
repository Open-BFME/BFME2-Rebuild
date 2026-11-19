// Rva001E702E.h: the object AIUpdateInterface+0x1F0 points to. Original name not
// recovered. WeaponFireSpecialAbilityUpdate::update (0x00492BEB) hands it the
// owner, a target position and 0 through 0x001E702E.
#pragma once
#ifndef BFME2_RVA001E702E_H
#define BFME2_RVA001E702E_H

#include "Common/GameType.h"

class Thing;
class Coord3D;

class Rva001E702E
{
public:
	void rva001E702E(Thing *thing, const Coord3D *pos, Int flag);
};

#endif
