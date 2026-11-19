// Coord3DBase.h: the plain three-float base of Coord3D.
// Target facts: retail spells Coord3D's own operands as this struct
// (?sub@Coord3D@@QAEXPBUCoord3DBase@@@Z export 1903, ?set@...PBUCoord3DBase export
// 1880, ??4Coord3DBase@@QAEAAU0@ABU0@@Z export, Xfer::operator==(Coord3DBase&) slot
// 24), so Coord3D derives from it. XferSaveAsText 0x0060DA04 reads x +0, y +4, z +8.
// No constructors: the retail assignment is the implicit one (export folded 0x0000353A).
#pragma once
#ifndef BFME2_COORD3DBASE_H
#define BFME2_COORD3DBASE_H

#include "Common/GameType.h"

struct Coord3DBase
{
	Real x;		// 0x0
	Real y;		// 0x4
	Real z;		// 0x8
};
// SIZE 0xC
typedef char Coord3DBaseSize[(sizeof(Coord3DBase) == 0xC) ? 1 : -1];

#endif
